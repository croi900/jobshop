#include "../include/gui/main_window.hpp"

 
bool App::OnInit()
{
    MainWindow *frame = new MainWindow();
    frame->Show(true);
    return true;
}

void MainWindow::BuildWindowLayout(){
    this->vbox = new wxBoxSizer(wxVERTICAL);
    auto hbox = new wxBoxSizer(wxHORIZONTAL);
    auto hbox2 = new wxBoxSizer(wxHORIZONTAL); 

    this->tree = new wxTreeCtrl(
        this,ID_Tree,wxDefaultPosition,
        wxDefaultSize,wxTR_DEFAULT_STYLE,
        wxDefaultValidator, "JobTree");
    if(this->tree == nullptr) {
        std::cerr << "main_window:constructor:cannot create tree\n";
        exit(0);
    }
    this->jobs_root = this->tree->AddRoot("Jobs");

    hbox->Add(this->tree, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(hbox,1,wxEXPAND | wxLEFT | wxTOP,0);
    // Part p("nigga steve",3);
    // auto wnd = new PartEditBox(p,vbox,this);
    // vbox->Add(wnd,1,wxEXPAND);
    this->SetSizer(vbox);
}

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Industrial Scheduler Service")
{

    this->SetWindowStyleFlag(wxBORDER_SIMPLE);
    
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_LoadJob, "&Import from human readable...\tCtrl-L",
                     "Load a job descripor file.");
    menuFile->Append(ID_LoadJSON, "&Import from JSON...\tCtrl-I",
                     "Load a job descripor file.");
    menuFile->Append(ID_SaveJSON, "&Export to JSON...\tCtrl-E",
                     "Load a job descripor file.");
    menuFile->Append(ID_Calculate, "&Calculate...\tCtrl-C",
                     "Calculate job timetable in vertical lines format.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("Please create or load a job file.");
 
    Bind(wxEVT_MENU, &MainWindow::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainWindow::OnLoad, this, ID_LoadJob);
    Bind(wxEVT_MENU, &MainWindow::OnLoadJSON, this, ID_LoadJSON);
    Bind(wxEVT_MENU, &MainWindow::OnSaveJSON, this, ID_SaveJSON);
    Bind(wxEVT_MENU, &MainWindow::OnCalculate, this, ID_Calculate);

    // Bind(wxEVT_TEXT,&MainWindow::PartNameCallback, this, PART_NAME_BTN); 
    // Bind(wxEVT_TEXT,&MainWindow::PartAmountCallback, this, PART_AMOUNT_BTN);

    BuildWindowLayout();

    

    // draw_plane = new BasicDrawPane( (wxFrame*) this->tree );
    // wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    // sizer->Add(draw_plane, 1, wxEXPAND);
}
 
void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void MainWindow::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MainWindow::RemoveAllChildWindows(){

    if(edit_window != nullptr){
        edit_window->Clear();
        delete edit_window;
    }
}

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_TREE_ITEM_ACTIVATED(ID_Tree,MainWindow::OnSelectedTreeItem)
    // EVT_TEXT_ENTER(PART_NAME_BTN,MainWindow::PartNameCallback)
    // EVT_TEXT_ENTER(PART_AMOUNT_BTN,MainWindow::PartAmountCallback)
    // EVT_PAINT(MainWindow::OnPaintTree)
wxEND_EVENT_TABLE()

// wxBEGIN_EVENT_TABLE(BasicDrawPane,wxPanel)
//     EVT_PAINT(BasicDrawPane::PaintEvent)
// wxEND_EVENT_TABLE()