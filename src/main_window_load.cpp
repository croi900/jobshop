#include "../include/gui/main_window.hpp"


void MainWindow::OnLoad(wxCommandEvent& event){
    wxFileDialog 
        openFileDialog(this, _("Open Job file"), "", "",
                       "", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    std::string path(openFileDialog.GetPath().c_str());
    this->job = new Job(path);

    if(job != nullptr){
        this->all_jobs[this->job->name] = this->job;
        #ifdef DEBUG_PRINTS
            std::cout << this->job->as_string() << std::endl;
        #endif
        ActivateMainWindowTools();
    }
}

void MainWindow::OnLoadJSON(wxCommandEvent& event){
    wxFileDialog 
        openFileDialog(this, _("Open Job JSON file"), "", "",
                       "JSON files (*.json)|*.json", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    std::string path(openFileDialog.GetPath().c_str());
    this->job = new Job();
    job->json_load(path);
    
    if(job != nullptr){
        this->all_jobs[this->job->name] = this->job;
        #ifdef DEBUG_PRINTS
            std::cout << this->job->as_string() << std::endl;
        #endif
        ActivateMainWindowTools();
    }
}

void MainWindow::OnSaveJSON(wxCommandEvent& event){
    wxFileDialog 
        openFileDialog(this, _("Open Job file"), "", "",
                       "", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    std::string path(openFileDialog.GetPath().c_str());
    

    if(this->job != nullptr){
        this->job->json_save(path);
    }
}


void MainWindow::ActivateMainWindowTools(){
    

    // if(!tree->GetRootItem().IsOk())

    if(job == nullptr){
        std::cerr<<"main_window_load:ActivateMainWindowTools: job ptr was null\n";
        exit(0);
    }

    auto job_root = tree->AppendItem(this->jobs_root,job->name.c_str());
    auto machine_root = tree->AppendItem(job_root,"Machines");
    for(auto& machine : job->machines){
        tree->AppendItem(machine_root,machine.get_name().c_str());
    }

    auto part_root = tree->AppendItem(job_root,"Parts");
    for(auto& ord : job->orders){
        tree->AppendItem(part_root,ord.first.get_name().c_str());
    }
    // tree->AddRoot(root1);
}