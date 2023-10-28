#ifndef MAIN_WINDOW_HGUARD
#define MAIN_WINDOW_HGUARD

#define DEBUG_PRINTS

#include <wx-3.2/wx.h>
#include <wx-3.2/wx/grid.h>
#include <wx-3.2/wx/treectrl.h>
#include <wx-3.2/wx/wfstream.h>
#include <wx-3.2/wx/sizer.h>
#include <wx-3.2/wx/event.h>

#include <unordered_map>
#include <vector>
#include <string>
#include <any>

#include "../job.hpp"
#include "part_edit_window.hpp"
#include "machine_edit_window.hpp"
#include "../scheduler.hpp"

class App : public wxApp
{
public:
    bool OnInit() override;
};
 

class MainWindow : public wxFrame
{
public:
    MainWindow();
 
private:
    std::unordered_map<std::string, Job*> all_jobs;
    ChildWindow* edit_window = 0;

    wxTreeCtrl* tree;
    wxTreeItemId jobs_root;
    wxTreeItemId active_item;
    wxBoxSizer* vbox;
    Job* job = nullptr;
    
    // BasicDrawPane* draw_plane;

    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnLoad(wxCommandEvent& event);
    void OnLoadJSON(wxCommandEvent& event);
    void OnSaveJSON(wxCommandEvent& event);
    void OnCalculate(wxCommandEvent& event);
    
    void ActivateMainWindowTools();
    void BuildWindowLayout();

    void OnSelectedTreeItem(wxTreeEvent& event);
    void OnPaintTree(wxPaintEvent& event);
    
    void PartNameCallback(wxCommandEvent& event);
    void PartAmountCallback(wxCommandEvent& event);


    void MachineNameCallback(wxCommandEvent& event);
    void MachineCooldownCallback(wxCommandEvent& event);
    void MachineCapacityCallback(wxCommandEvent& event);

    void RemoveAllChildWindows();

    wxDECLARE_EVENT_TABLE();
    
};
 
enum
{
    ID_Hello = 1,
    ID_LoadJob = 2,
    ID_Tree = 4,
    TEST_TextBox = 3,
    ID_LoadJSON = 5,
    ID_SaveJSON = 6,
    ID_Calculate = 7
};


#endif