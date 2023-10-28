#include "../include/scheduler.hpp"

Scheduler::~Scheduler(){

}

Scheduler::Scheduler(Job& job) : job(job){
    build_id_map();
    create_dataframe();
    build_time_matrix();
    build_machine_queue();
    build_insertion_queue();
    // dfs_intersection();
    // matrix_intersection_sort();

    dfs_intersection_sort();
    // optimize_queue();
}

void Scheduler::build_id_map(){
    size_t idx = 1;
    for (auto & machine : job.machines)
    {
        sidm[machine.get_id()] = idx;
        ridm[idx] = machine.get_id();

        idx += 1;
    }
    idx = 1;
    for (auto & order : job.orders)
    {
        auto part = order.first;

        sidp[part.get_id()] = idx;
        ridp[idx] = part.get_id();

        idx += 1;
    }
}

void Scheduler::create_dataframe(){
    dataframe.resize(job.orders.size()+1);
    for(auto& submat : dataframe){
        submat.resize(job.machines.size()+1);
        for(auto& line : submat){
            line.resize(job.machines.size()+1);
        }
    }
    size_t id = 1;
    std::cout   <<  "x: " << dataframe.size()
                << " y: " << dataframe[0].size() 
                << " z: " << dataframe[0][0].size() << std:: endl;

    for(auto& order : job.orders){
        auto pid = sidp[order.first.get_id()]; // id of parts(in order i think)
        auto range = job.operations.equal_range(order.first.get_id());
        
        size_t u,v;
        bool first = false;
        
        for(auto it = range.first; it != range.second; ++it){
            // std::cout << pid << " " << it->second.first << std::endl;

            if(!first){
                u = v = sidm[it->second.first];
                std::cout << "@pid: " << pid << u << " v: " << v << std::endl; 

                dataframe[pid][u][v] = dataframe[pid][v][u]  = 1;
                first = true;
            }else{
                u = v;
                v = sidm[it->second.first];
                std::cout << "pid: " << pid << " u: " << u << " v: " << v << std::endl; 

                dataframe[pid][u][v] = dataframe[pid][v][u] = 1;
            }
        }
        std::cout << std::endl;
    }
}

void Scheduler::build_time_matrix(){
    T.resize(job.orders.size() + 1);
    for(auto& line : T){
        line.resize(job.machines.size() + 1);
    }

    for(auto& order : job.orders){
        auto pid = sidp[order.first.get_id()]; // id of parts(in order i think)
        auto range = job.operations.equal_range(order.first.get_id());
                
        for(auto it = range.first; it != range.second; ++it){
           auto m = sidm[it->second.first]; // machine id
           auto mc = job.get_machine_by_id(it->second.first).get_cooldown(); //machine cooldown
           auto pt = it->second.second; // process time
           T[pid][m] = mc + pt;
        }
        // std::cout << std::endl;
    }

    T2 = T;

    for(auto & line : T2){
        // std::partial_sum(line.begin(),line.end(),line.begin(), std::plus<float>());
        float ival = line[1];

        for(int i = 2; i < line.size(); ++i){
            if(line[i] > 0){
                ival += line[i];
                line[i] = ival;
            }
        }

    }
}

void Scheduler::build_machine_queue(){
    Q.resize(job.machines.size() + 1);
}

void Scheduler::build_insertion_queue(){
    Qs.resize(job.machines.size() + 1);

    for(int p_id = 1; p_id < T.size(); ++p_id){
        for(int m_id = 1; m_id < T[p_id].size(); ++m_id){
            if(m_id > 0){
                Qs[m_id].push_back(p_id);
                break;
            }
        }
    }

    for(int i = 1; i < Qs.size(); ++i){
        sort(Qs[i].begin(), Qs[i].end(),
            [i,this](size_t& id1, size_t& id2){
                return T[id1][i] < T[id2][i];
            });
    }
}

void Scheduler::dfs_assign(size_t id, Visited& vis, Adjacency& adj, float t, size_t part_id){
    vis[id] = 1;

    // t =  ;
    if(Q[id].empty() == false)
        t += Q[id].back().dur + Q[id].back().time ;

    Q[id].push_back({t,T[part_id][id],part_id});
    // std::cerr << "Neam haladit " << id << " " << part_id  ;
    for(int i = 1; i < adj[id].size(); ++i){
        if(!vis[i] && adj[id][i] && id != i){
            dfs_assign(i,vis,adj,t + T[part_id][id],part_id);
        }
    }
}

void Scheduler::dfs_intersection(){

    for(int i = 1; i < dataframe.size(); ++i){
        std::vector<size_t> vis(dataframe[i][0].size());

        size_t beg = 0;
        for(int j = 1; j < dataframe[i].size(); ++j){
            if(dataframe[i][j][j] != 0){
                beg = j;
                break;
            }
        }
        float t = 0;
        if(Q[beg].empty() == false)
            t += Q[beg].back().time + Q[beg].back().dur;
        dfs_assign(beg,vis,dataframe[i],t,i);
    }

}


void Scheduler::dfs_intersection_sort(){
    

    for(int m_id = 1; m_id < Qs.size(); ++m_id){
        while(Qs[m_id].empty() == false){
            auto i = Qs[m_id].front();
            Qs[m_id].pop_front();
            std::vector<size_t> vis(dataframe[i][0].size());

            size_t beg = 0;
            for(int j = 1; j < dataframe[i].size(); ++j){
                if(dataframe[i][j][j] != 0){
                    beg = j;
                    break;
                }
            }
            float t = 0;
            if(Q[beg].empty() == false)
                t += Q[beg].back().time + Q[beg].back().dur;
            dfs_assign(beg,vis,dataframe[i],t,i);
        }
    }
}


void Scheduler::optimize_queue(){
    for(int i = 1; i < Q.size(); ++i){
        for(int j = 1; j < Q[i].size(); ++j){
            std::cerr << Q[i][j].time << " " << (Q[i][j-1].time + Q[i][j-1].dur) 
                        << " " << (Q[i][j-1].time ) << " " << ( Q[i][j-1].dur)
                        << std::endl;
            Q[i][j].time = Q[i][j-1].time + Q[i][j-1].dur;
        }
    }
}

//BROKEN FOR NOW
void Scheduler::matrix_intersection_sort(){
    for(int m_id = 1; m_id < T[0].size(); ++m_id){ // for each machine
        std::vector<size_t> intersections;
        for(int p_id = 1; p_id < T.size(); ++p_id){ //find intersections            
            if(T[p_id][m_id] > 0){
                intersections.push_back(p_id);
            }
        }

        sort(intersections.begin(),intersections.end(),
            [m_id,this](size_t& id1, size_t& id2){
                return T[id1][m_id] < T[id2][m_id];
            });

        for(auto& id : intersections){
            
            float t = 0;
            if(Q[m_id].empty() == false)
                t += Q[m_id].back().time + Q[m_id].back().dur;
            // Q[m_id].push_back({t,T[id][m_id],id});

            std::vector<size_t> vis(dataframe[id].size());
            dfs_assign(m_id,vis,dataframe[id],t,id);
        }
    }
}

std::string Scheduler::as_string(){
    std::stringstream ss;

    for(size_t i = 1; i < dataframe.size(); ++i){
        ss<<"Part " << i << ": \n";
        for(size_t j = 1; j < dataframe[i].size(); ++j){
            for(size_t k = 1; k < dataframe[i][j].size(); ++k){
                ss<<dataframe[i][j][k] << " ";
            }
            ss << '\n';
        }
        ss << '\n';
    }

    ss << '\n' << "Time Matrix (T):" << '\n';

    for(int i = 1; i < T.size(); ++i){
        for(int j = 1; j < T[i].size(); ++j){
            ss << T[i][j] << "\t\t";
        }
        ss << '\n';
    }

    ss << '\n' << "Accumulated Time Matrix(T2):" << '\n';

    for(int i = 1; i < T2.size(); ++i){
        for(int j = 1; j < T2[i].size(); ++j){
            ss << T2[i][j] << "\t\t";
        }
        ss << '\n';
    }

    ss << '\n' << "Schedule Queue(Q):" << '\n';

    for(int i = 1; i < Q.size(); ++i){
        ss << "Machine " << i << ": ";
        for(auto & entry : Q[i]){
            ss << "[ " << entry.pid << ", " << entry.time << " ] ";
        }

        ss << '\n';
    }

    return ss.str();
}

std::string Scheduler::schedule_queue_as_parsable(){
    std::stringstream ss;

    for(int i = 1; i < Q.size(); ++i){
        for(auto & entry : Q[i]){
            ss << entry.pid << " " << entry.time << " " << entry.dur << ' ';
        }

        ss << '\n';
    }

    return ss.str();
}