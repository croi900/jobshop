#include "../include/gui/part_edit_window.hpp"



#define NAME_ID 101
#define LABEL_ID 102

PartEditBox::PartEditBox(std::reference_wrapper<PartOrder> p,wxSizer* parent_sizer, wxFrame* parent)
            :part(p),parent(parent),parent_sizer(parent_sizer)
{
    auto vbox = new wxBoxSizer(wxVERTICAL);
    auto hbox1 = new wxBoxSizer(wxHORIZONTAL);
    auto hbox2 =  new wxBoxSizer(wxHORIZONTAL);
    
    name = new wxStaticText(parent,wxID_ANY,part.get().first.get_name());
    
    label_name = new wxStaticText(parent,wxID_ANY,"Name: ");
    input_name = new wxTextCtrl(parent,PART_NAME_BTN, part.get().first.get_name());
    input_name->SetWindowStyleFlag(wxTE_PROCESS_ENTER);

    label_amount = new wxStaticText(parent,wxID_ANY,"Amount: ");
    
    auto amount_str = std::to_string(part.get().second);
    input_amount = new wxTextCtrl(parent,PART_AMOUNT_BTN,amount_str);
    
    hbox1->Add(label_name,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox1->Add(input_name,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox2->Add(label_amount,1,wxLEFT | wxRIGHT | wxEXPAND);
    hbox2->Add(input_amount,1,wxLEFT | wxRIGHT | wxEXPAND);

    input_name->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
    input_amount->SetWindowStyleFlag(wxTE_PROCESS_ENTER);

    parent_sizer->Add(name,0);
    parent_sizer->Add(hbox1,0, wxEXPAND,5);
    parent_sizer->Add(hbox2,0, wxEXPAND,5);
    // parent_sizer->AddSpacer(25);
    parent_sizer->Layout();

}

void PartEditBox::Clear(){
    parent->RemoveChild(name);
    parent->RemoveChild(label_name);
    parent->RemoveChild(input_name);
    parent->RemoveChild(label_amount);
    parent->RemoveChild(input_amount);

    delete name;
    delete label_name;
    delete input_name;
    delete label_amount;
    delete input_amount;

    // parent_sizer->Remove()
}