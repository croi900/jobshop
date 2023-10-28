#ifndef PART_HGUARD
#define PART_HGUARD
#include <string>
#include <vector>
#include "machine.hpp"

class Part
{
private:
    std::string name;
    std::size_t id;
    // // A vector representing pointers to
    // // the machines needed to produce a part
    // // in the order of needed production
    // std::vector<prod_step> prod_line; 
public:
    Part(std::string name) : name(name) {};
    Part(std::string name, std::size_t id) : name(name), id(id) {};
    Part(/* args */);

    std::string get_name();
    std::size_t get_id();

    void set_id(std::size_t new_id);
    void set_name(const std::string& new_name);

    // void add_step(Machine* machine, std::size_t time);
    ~Part();
};

typedef std::pair<Part,int> PartOrder;

#endif