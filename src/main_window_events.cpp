#include "../include/gui/main_window.hpp"

#include <iostream>

void MainWindow::OnSelectedTreeItem(wxTreeEvent& event){
    this->active_item = event.GetItem();
    
    auto item_name = std::string(
        this->tree->GetItemText(event.GetItem()).c_str());
    if(item_name == "Job") return;

    auto root_name = this->tree->GetItemText(
        this->tree->GetItemParent(event.GetItem()));

    auto job_name = std::string(this->tree->GetItemText(
        this->tree->GetItemParent(
            this->tree->GetItemParent(event.GetItem()))).c_str());

    this->job = this->all_jobs[job_name];


    if(root_name == "Machines"){
        if(auto machine = this->job->get_machine_by_name(item_name)){
            std::cout << machine.value().get().get_name() << std::endl;

            if(this->edit_window != nullptr &&  this->edit_window != NULL )
                RemoveAllChildWindows();

            auto meb = new MachineEditBox(machine.value(),this->GetSizer(),this);
            meb->input_capacity->Bind(wxEVT_TEXT_ENTER,&MainWindow::MachineCapacityCallback,this);
            meb->input_cooldown->Bind(wxEVT_TEXT_ENTER,&MainWindow::MachineCooldownCallback,this);
            meb->input_name->Bind(wxEVT_TEXT_ENTER,&MainWindow::MachineNameCallback,this);

            this->edit_window = meb;
        }
    }else if(root_name == "Parts"){
        if(auto part = this->job->get_order_by_name(item_name)){
            std::cout << part.value().get().first.get_name() << std::endl;
            
            if(this->edit_window != nullptr &&  this->edit_window != NULL )
                RemoveAllChildWindows();

            auto peb = new PartEditBox(part.value(),this->GetSizer(),this); 
            peb->input_name->Bind(wxEVT_TEXT_ENTER,&MainWindow::PartNameCallback,this);
            peb->input_amount->Bind(wxEVT_TEXT_ENTER,&MainWindow::PartAmountCallback,this);

            this->edit_window = peb;
        }
    }    
}

void MainWindow::PartNameCallback(wxCommandEvent& event){
    auto pbox = (PartEditBox*)this->edit_window;
    pbox->part.get().first.set_name(
        std::string(event.GetString().c_str())
    );

    this->tree->SetItemText(this->active_item, event.GetString());
}

void MainWindow::PartAmountCallback(wxCommandEvent& event){
    auto pbox = (PartEditBox*)this->edit_window;
    pbox->part.get().second = 
        text2num(std::string(event.GetString().c_str()));

}

void MainWindow::MachineNameCallback(wxCommandEvent& event){
    auto mbox = (MachineEditBox*)this->edit_window;
    mbox->machine.get().set_name(
        std::string(event.GetString().c_str())
    );

    this->tree->SetItemText(this->active_item, event.GetString());
}

void MainWindow::MachineCapacityCallback(wxCommandEvent& event){
    auto mbox = (MachineEditBox*)this->edit_window;
    mbox->machine.get().set_capacity(
        text2num(std::string(event.GetString().c_str()))
    );
}

void MainWindow::MachineCooldownCallback(wxCommandEvent& event){
   auto mbox = (MachineEditBox*)this->edit_window;
    mbox->machine.get().set_cooldown(
        text2num(std::string(event.GetString().c_str()))
    );
}

void MainWindow::OnCalculate(wxCommandEvent& event){
    Scheduler calc(*this->job);
    std::ofstream out("scripts/test_matrix_sort_T.sched");
    out << calc.schedule_queue_as_parsable();

    

    // auto now = time(NULL);
    // while(time(NULL) < now + 2){
        // system("python3 scripts/visualizer.py");
        // std::cerr << now << ' ' << time(NULL) << std::endl;
    // }
    // sleep(5);
    // system("gv schedule.pdf");
}