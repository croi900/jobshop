#include "../include/machine.hpp"

Machine::Machine(/* args */)
{
}

Machine::~Machine()
{
}

std::string Machine::get_name(){
    return this->name;
}

std::size_t Machine::get_capacity(){
    return this->capacity;
}

std::size_t Machine::get_cooldown(){
    return this->cooldown;
}

std::size_t Machine::get_id(){
    return this->id;
}

void Machine::set_name(const std::string &new_name){
    this->name = new_name;
}

void Machine::set_capacity(std::size_t new_cap)
{
    this->capacity = new_cap;
}

void Machine::set_cooldown(std::size_t new_cool)
{
    this->cooldown = new_cool;
}

void Machine::set_id(std::size_t new_id){
    this->id = new_id;
}