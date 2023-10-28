#ifndef JOB_HGUARD
#define JOB_HGUARD
#include <string>
#include <vector>
#include <map>
#include <optional>

#include "part.hpp"
#include "machine.hpp"
#include "error_codes.hpp"
#include "text2num/text2num.hpp"
#include "parse_utils.hpp"

#include "json.hpp"
// struct job_step {
//     Part* part;
//     Machine*machine;
// };

typedef std::multimap<size_t,std::pair<size_t,float>> bipartite_graph;

class Job
{
private:
public:
    bipartite_graph operations; 

    std::vector<PartOrder> orders;
    std::vector<Machine> machines;

    std::string name;
    /*
        The two optimisations I implement will be for
        total shortest production time(calculate_path_time)
        and highest amount of parts as quick as possible.

        The apparent difference would be the fact that some
        paths may produce less parts quickly but have an overall
        shorter time while others may produce more parts quickly
        but have an overall longer time.
    */

    void calculate_path_time();
    void calculate_path_parts();

    void add_operations(size_t id_part, size_t id_machine, float time);

    Machine& get_machine_by_id(std::size_t);
    PartOrder& get_order_by_id(std::size_t); 

    std::optional<std::reference_wrapper<Machine>> 
    get_machine_by_name(const std::string&);
    
    std::optional<std::reference_wrapper<PartOrder>> 
    get_order_by_name(const std::string&);

    std::string as_string();

    void json_load(const std::string& path);
    void json_save(const std::string& path);

    Job(const std::string & path_to_config);
    // Job(std::ifstream& stream);
    Job(/* args */);
    ~Job();
};

#endif