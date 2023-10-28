#ifndef MACHINE_HGUARD
#define MACHINE_HGUARD
#include <string>

class Machine
{
private:
    std::string name;
    std::size_t capacity;
    std::size_t cooldown;
    std::size_t id;
    
    bool in_use = false;    
public:
    Machine(std::string name, std::size_t cap,
            std::size_t cooldown) : name(name),
            capacity(cap),cooldown(cooldown){};

    Machine(std::string name, std::size_t cap,
            std::size_t cooldown, std::size_t id) : 
            name(name), capacity(cap),cooldown(cooldown),
            id(id){};

    Machine(std::string name, std::size_t id):
        name(name), id(id){};

    std::string get_name();
    std::size_t get_capacity();
    std::size_t get_cooldown();
    std::size_t get_id();

    void set_name(const std::string& new_name);
    void set_capacity(std::size_t new_cap);
    void set_cooldown(std::size_t new_cool);
    void set_id(std::size_t new_id);
    
    Machine(/* args */);
    ~Machine();
};

#endif