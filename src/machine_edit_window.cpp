#include "../include/gui/machine_edit_window.hpp"

MachineEditBox::MachineEditBox(std::reference_wrapper<Machine> p,wxSizer* parent_sizer, wxFrame* parent)
            :machine(p),parent(parent),parent_sizer(parent_sizer)
{
    auto vbox = new wxBoxSizer(wxVERTICAL);
    auto hbox1 = new wxBoxSizer(wxHORIZONTAL);
    auto hbox2 =  new wxBoxSizer(wxHORIZONTAL);
    auto hbox3 =  new wxBoxSizer(wxHORIZONTAL);
    
    name = new wxStaticText(parent,wxID_ANY,machine.get().get_name());
    
    label_name = new wxStaticText(parent,wxID_ANY,"Name: ");
    input_name = new wxTextCtrl(parent,wxID_ANY,machine.get().get_name());

    label_capacity = new wxStaticText(parent,wxID_ANY,"Capacity: ");
    auto cap_str = std::to_string(machine.get().get_capacity());
    input_capacity = new wxTextCtrl(parent,wxID_ANY,cap_str);

    label_cooldown = new wxStaticText(parent,wxID_ANY,"Cooldown: ");
    auto cool_str = std::to_string(machine.get().get_cooldown());
    input_cooldown = new wxTextCtrl(parent,wxID_ANY,cool_str);

    input_capacity->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
    input_cooldown->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
    input_name->SetWindowStyleFlag(wxTE_PROCESS_ENTER);

    
    hbox1->Add(label_name,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox1->Add(input_name,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox2->Add(label_capacity,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox2->Add(input_capacity,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox3->Add(label_cooldown,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox3->Add(input_cooldown,1,wxLEFT | wxRIGHT | wxEXPAND);

    parent_sizer->Add(name,0);
    parent_sizer->Add(hbox1,0, wxEXPAND,5);
    parent_sizer->Add(hbox2,0, wxEXPAND,5);
    parent_sizer->Add(hbox3,0, wxEXPAND,5);
    // parent_sizer->AddSpacer(25);
    parent_sizer->Layout();

}

void MachineEditBox::Clear(){
    parent->RemoveChild(name);
    parent->RemoveChild(label_name);
    parent->RemoveChild(input_name);
    parent->RemoveChild(label_capacity);
    parent->RemoveChild(input_capacity);
    parent->RemoveChild(label_cooldown);
    parent->RemoveChild(input_cooldown);

    delete name;
    delete label_name;
    delete input_name;
    delete label_capacity;
    delete input_capacity;
    delete label_cooldown;
    delete input_cooldown;

    // parent_sizer->Remove()
}