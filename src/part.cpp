#include "../include/part.hpp"

Part::Part(/* args */)
{
}

Part::~Part()
{
}

std::string Part::get_name(){
    return this->name;
}


std::size_t Part::get_id(){
    return this->id;
}

void Part::set_name(const std::string &new_name){
    this->name = new_name;
}


void Part::set_id(std::size_t new_id){
    this->id = new_id;
}