#include "../include/job.hpp"


Job::Job(/* args */)
{
}

Job::~Job()
{
}

//comment out this line to disable
//debug prints
// #define DEBUG_PRINTS


#ifdef DEBUG_PRINTS
#include<iostream>
#endif

bool is_machine_list_begin(std::string& line){
    conv_to_parsable(line);

#ifdef DEBUG_PRINTS
    std::cout << ((line.find("availablemachines") != std::string::npos)?
        "FOUND MACHINE LIST BEGIN":
        "DIDNT FIND MACHINE LIST BEGIN")
         << " - " << line << std::endl;
#endif

    return line.find("availablemachines") != std::string::npos;
}

bool is_comment(const std::string& line){
    for(auto & c : line)
        if(!isspace(c))
            return c == '#';
    return false;
}

int parse_machine(std::string& line, Job* job){
    auto id = extract_first_num(line);
    auto m_name = extract_first_words(line,[](char c){
        return isspace(c) || isalpha(c) || c=='-' || c=='_';
    });

    if(!has_chars(m_name)) return ERROR_BAD_NAME;
    
    while(isspace(m_name.back()))
        m_name.pop_back();

    for(auto & existing_machine : job->machines){
        if(id == existing_machine.get_id()){
            return ERROR_ID_EXISTS;
        }
    }

    #ifdef DEBUG_PRINTS
    std::cout << "PARSED MACHINE NAME " 
    << m_name << " ID " << id << std::endl;
    #endif    
    
    Machine mach(m_name,id);
    job->machines.push_back(mach);

    return 1;
}

bool is_machine_feature_begin(std::string line){
    conv_to_parsable(line);

#ifdef DEBUG_PRINTS
    std::cout << ((line.find("machinefeatures") != std::string::npos)?
        "FOUND MACHINE FEATURE LIST BEGIN":
        "DIDNT FIND MACHINE FEATURE LIST BEGIN")
         << " - " << line << std::endl;
#endif

    return line.find("machinefeatures") != std::string::npos;
}

bool is_cap_line(std::string& line){
    std::string type = extract_first_words(line);
    conv_to_parsable(type);
    std::cout<<type<<std::endl;
    return type.find("capacity") != std::string::npos;
}

bool is_cool_line(std::string& line){
    std::string type = extract_first_words(line);
    conv_to_parsable(type);

    return type.find("cooldown") != std::string::npos;
}

int parse_machine_feature(std::string& line1, std::string& line2, Job* job){
    auto machine_id = extract_first_num(line1);

    bool found_machine = false;
    for(auto & existing_machine : job->machines){
        if(machine_id == existing_machine.get_id()){
            found_machine = true;
        }
    }
    if(!found_machine) return ERROR_BAD_ID;

    std::string cap_str = extract_last_words(line1);
    std::string cool_str = extract_last_words(line2);


    #ifdef DEBUG_PRINTS
    std::cout << "CAP_STR: " << cap_str << '\n';
    std::cout << "COOl_STR: " << cool_str<<std::endl;
    #endif

    std::size_t capacity = text2num(cap_str);
    std::size_t cooldown = text2num(cool_str);

    //should optimise to an update queue
    //if number of machines ever happens to be
    //over 10^5
    for(auto & existing_machine : job->machines){
        if(machine_id == existing_machine.get_id()){
            existing_machine.set_capacity(capacity);
            existing_machine.set_cooldown(cooldown);
        }
    }

    return 1;
}

enum{
    Begin,
    ParsingMachines,
    ParsingFeatures,
    ParsingParts,
    ParsingOperations
};

bool is_part_list_begin(std::string line){
    conv_to_parsable(line);

#ifdef DEBUG_PRINTS
    std::cout << ((line.find("partlist") != std::string::npos)?
        "FOUND PART LIST BEGIN":
        "DIDNT FIND PART LIST BEGIN")
         << " - " << line << std::endl;
#endif

    return line.find("partlist") != std::string::npos;
}

int parse_part(std::string& line, Job* job){
    auto id = extract_first_num(line);
    auto name = extract_first_words(line);
    auto amount = text2num(separate_by_char_trim(line, '-').second);
    for(auto& p : job->orders){
        if(p.first.get_id() == id){
            return ERROR_ID_EXISTS;
        }
    }

    if(!has_chars(name))
        return ERROR_BAD_NAME;

    #ifdef DEBUG_PRINTS
    std::cout << "PARSED PART " << name << " ID " << id <<" AMOUNT "<< 
    separate_by_char_trim(line, '-').second << std::endl;
    #endif

    Part p(name,id);
    job->orders.push_back({p,amount});

    return 0;
}

bool is_part_op_begin(std::string line){
    conv_to_parsable(line);

#ifdef DEBUG_PRINTS
    std::cout << ((line.find("partoperations") != std::string::npos)?
        "FOUND PART LIST BEGIN":
        "DIDNT FIND PART LIST BEGIN")
         << " - " << line << std::endl;
#endif

    return line.find("partoperations") != std::string::npos;
}

int handle_error(int code, const std::string& line){
    if(code == ERROR_BAD_NAME){
        std::cerr<<"BAD NAME PARSED:> " << line
                    << std::endl;
        //throw_error(ERROR_BAD_NAME);
        return 0;
    }
    if(code == ERROR_ID_EXISTS){
        std::cerr<<"ID ALREAD EXISTS:> " << line
                    << std::endl;
        //throw_error(ERROR_ID_EXISTS);
        return 0;
    }
    if(code == ERROR_BAD_ORDERING){
        std::cerr<<"BAD ORDERING PARSED:> " << line
                << std::endl;
        //throw_error(ERROR_BAD_NAME);
        return 0;
    }
    if(code == ERROR_BAD_ID){
        std::cerr<<"ID DOESNT EXIST:> " << line
                << std::endl;
        //throw_error(ERROR_ID_EXISTS);
        return 0;
    }
    return 1;
}

int process_op_text(std::string& text, Job* job, size_t& part_id){
    std::stringstream ss(text);
    std::string line;
    while(std::getline(ss,line)){
        auto result = separate_by_char_trim(line,':');

        if(part_id == 0)
            part_id = extract_first_num(result.first);
        auto machine_name = extract_first_words(line,[](char c){
            return isspace(c) || isalpha(c) || c=='-' || c=='_';
        });
        conv_to_parsable(machine_name);

        auto process_time = text2num(result.second);
        auto machine_id = -1;

        size_t max_lev_dist = UINT64_MAX;
        for (auto & mach : job->machines)
        {
            auto mach_name = extract_first_words(mach.get_name(),[](char c){
                return isspace(c) || isalpha(c) || c=='-' || c=='_';
            });
            conv_to_parsable(mach_name);

            auto lev_dist = levdist(mach_name,machine_name);

            if(lev_dist < max_lev_dist){
                machine_id = mach.get_id();
                max_lev_dist = lev_dist;
            }
        }

        if(machine_id == -1){
            std::cerr << "ERROR PARSING OPERATION " <<
            "part id: " << part_id << " " <<
            "machine name: " << machine_name << " " <<
            "processing time: " << process_time << "\n";
            return ERROR_BAD_NAME;
        }
        #ifdef DEBUG_PRINTS
        else{
            std::cout << "PARSED OPERATION" <<
            "\npart id: " << part_id << " " <<
            "\nmachine name: " << machine_name << " " <<
            "\nmachine id: " << machine_id << " " <<
            "\nprocessing time: " << process_time << "\n";
        }
        #endif
        
        job->add_operations(part_id,machine_id,process_time);
    }        
    return 1;
}

Job::Job(const std::string& path_to_config){
    std::ifstream in(path_to_config);
    this->name = get_filename(path_to_config);

    bool machine_list_begin = false;
    bool machine_feature_begin = false;
    bool part_list_begin = false;
    bool part_operations_beign = false;
    bool found_first_feature = false;

    std::string line;

    std::string cap_line;
    std::string cool_line;

    std::string op_txt;
    bool new_part_op_list = false;

    bool expects_cap_line = true;
    bool line_pair_parsed = false;

    int state = Begin;

    size_t part_id = 0;

    while(std::getline(in,line)){
        if(!has_chars(line) || is_comment(line)) continue;

        if(state == Begin){
            if(is_machine_list_begin(line)){
                state = ParsingMachines;
                continue;
            }
        }
        if(state == ParsingMachines){
            if(is_machine_feature_begin(line)){
                state = ParsingFeatures;
                continue;
            }

            auto code = parse_machine(line, this);
            if(!handle_error(code,line)) break;
            continue;
        }

        if(state == ParsingFeatures){
            
            std::string line2;
            std::getline(in,line2);

            if( is_part_list_begin(line)){
                state = ParsingParts;
                line = line2;
            }
            if(is_part_list_begin(line2)){
                state = ParsingParts;
            }
            if(state == ParsingFeatures){
                auto code = parse_machine_feature(line,line2,this);
                if(!handle_error(code,line)) break;
                continue;
            }

        }

        if(state == ParsingParts){
            if(is_part_op_begin(line))
            {
                state = ParsingOperations;
                continue;
            }

            auto code = parse_part(line,this);
            if(!handle_error(code,line)) break;
            continue;
        }
        if(state == ParsingOperations){
            if(new_part_op_list == false){
                if(begins_with_number(line)){
                    new_part_op_list = true;
                }
            }
            if(new_part_op_list == true){
                if(begins_with_number(line)){
                    process_op_text(op_txt,this,part_id);
                    op_txt = "";
                    part_id = 0;
                }
                op_txt += line + '\n';
            }
        }
    }
    if(state == ParsingOperations){
        if(new_part_op_list == true){
            process_op_text(op_txt,this,part_id);
        }
    }
}

void Job::add_operations(size_t part_id, size_t machine_id, float time){
    this->operations.insert({part_id, {machine_id,time}});
}
Machine& Job::get_machine_by_id(std::size_t id){
    for(auto& m : this->machines){
        if(m.get_id() == id){
            return m;
        }
    }

    std::cerr<<"couldn't find Machine with id: " << id << '\n';
    exit(0);
}

PartOrder& Job::get_order_by_id(std::size_t id){
    for(auto& o : this->orders){
        if(o.first.get_id() == id){
            return o;
        }
    }

    std::cerr<<"couldn't find Part Order with id: " << id << '\n';
    exit(0);
}

std::optional<std::reference_wrapper<Machine>> 
Job::get_machine_by_name(const std::string& name){
    for(auto& m : this->machines){
        if(m.get_name() == name){
            return m;
        }
    }

    std::cerr<<"couldn't find Machine with name: " << name << '\n';
    return {};
}

std::optional<std::reference_wrapper<PartOrder>> 
Job::get_order_by_name(const std::string& name){
    for(auto& o : this->orders){
        if(o.first.get_name() == name){
            return o;
        }
    }

    std::cerr<<"couldn't find Part Order with name: " << name << '\n';
    return {};
}

std::string Job::as_string(){
    std::stringstream ss;
    ss << "Machines:\n";
    for(auto & machine : this->machines){
        ss << "\t"<<machine.get_name()<<" [ID: " << machine.get_id() << "]\n"; 
        ss << "\t\t->Capacity: "<<machine.get_capacity()<<"\n"; 
        ss << "\t\t->Cooldown: "<<machine.get_cooldown()<<"\n"; 
    }
    ss << "\nParts & Orders:\n";
    for(auto & order : this->orders){
        ss << "\t"<<order.first.get_name()<<" [ID: " << order.first.get_id() << "]\n"; 
        ss << "\t\t->Amont: "<< order.second << "\n";
        ss << "\t\t->Process: \n";
        auto process = this->operations.equal_range(order.first.get_id());
        int idx = 1;
        for(auto op = process.first; op != process.second; ++op){
            ss << "\t\t\t" << idx << ". " << get_machine_by_id(op->second.first).get_name() << '\n';
            idx++;
        }
    }

    return ss.str();
}

void Job::json_save(const std::string& path){
    nlohmann::json j = {
        {"name",this->name}
    };

    j["machines"] = j.array();
    for(auto & machine : this->machines){
        nlohmann::json machine_j;
        machine_j["id"] = machine.get_id();
        machine_j["name"] = machine.get_name();
        machine_j["capacity"] = machine.get_capacity();
        machine_j["cooldown"] = machine.get_cooldown();
        j["machines"] += machine_j;
    }

    j["parts"] = j.array();
    for(auto & order : this->orders){
        auto process = this->operations.equal_range(order.first.get_id());
        nlohmann::json part_j;
        part_j["id"] = order.first.get_id();
        part_j["name"] = order.first.get_name();
        part_j["amount"] = order.second;
        part_j["process"] = j.array();
        for(auto op = process.first; op != process.second; ++op){
            nlohmann::json op_json;
            op_json["machine"] = op->second.first;
            op_json["duration"] = op->second.second;
            part_j["process"] += op_json;
        }
        j["parts"] += part_j;
    }
    #ifdef DEBUG_PRINTS
        std::cout << std::setw(4) << j << std::endl;
    #endif

    std::ofstream out(path);
    out << std::setw(4) << j;
}
// #define DEBUG_PRINTS
void Job::json_load(const std::string& path){
    std::ifstream in(path);
    nlohmann::json j;
    j = j.parse(in);

    this->machines.clear();
    this->orders.clear();
    this->operations.clear();
    
    this->name = j["name"].get<std::string>();

    for(auto& mj : j["machines"]){
        Machine machine(
            mj["name"].get<std::string>(),
            mj["capacity"].get<size_t>(),
            mj["cooldown"].get<size_t>(),
            mj["id"].get<size_t>());
        this->machines.push_back(machine);
    }


    for(auto& pj : j["parts"]){
        Part part(
            pj["name"].get<std::string>(),
            pj["id"].get<size_t>());

        this->orders.push_back({part, pj["amount"].get<int>()});
        for(auto& opj : pj["process"]){
            this->operations.insert(
                {
                    part.get_id(),
                    {
                        opj["machine"].get<size_t>(),
                        opj["duration"].get<float>()
                    }
                }
            );
        }
    }
    #ifdef DEBUG_PRINTS
    std::cout << "=============================FROM JSON LOAD=============================\n";
    std::cout << this->as_string() << std::endl;
    std::cout << "========================================================================\n";
    #endif
}
// #undef DEBUG_PRINTS
// if(!line_pair_parsed){
            //     if(expects_cap_line){
            //         bool cap = is_cap_line(line);
            //         if(!cap){
            //             std::cout<<"[ERROR]EXPECTED CAP LINE:> "
            //                 << line << std::endl;
            //             break;
            //         }
            //         expects_cap_line != expects_cap_line;
            //         cap_line = line;
            //     }
            //     if(!expects_cap_line){
            //         bool cool = is_cool_line(line);
            //         if(!cool){
            //             std::cout<<"[ERROR]EXPECTED COOL LINE:> "
            //                     << line << std::endl;
            //             break;
            //         }
            //         cool_line = line;
            //         line_pair_parsed = true;
            //     }
            // }
            // else{
            //     auto code = parse_machine_feature(cap_line,cool_line,this);
            //     if(code == ERROR_BAD_ORDERING){
            //         std::cout<<"BAD ORDERING PARSED:> " << cap_line << '\n' << cool_line
            //                 << std::endl;
            //         //throw_error(ERROR_BAD_NAME);
            //         break;
            //     }
            //     if(code == ERROR_BAD_ID){
            //         std::cout<<"ID DOESNT EXIST:> " << line
            //                 << std::endl;
            //         //throw_error(ERROR_ID_EXISTS);
            //         break;
            //     }
            // }