#ifndef MACHINE_EDIT_WINDOW_HGUARD
#define MACHINE_EDIT_WINDOW_HGUARD

#include <wx-3.2/wx/wx.h>
#include <wx-3.2/wx/grid.h>
#include <wx-3.2/wx/treectrl.h>
#include <wx-3.2/wx/wfstream.h>
#include <wx-3.2/wx/sizer.h>
#include <wx-3.2/wx/event.h>

#include <unordered_map>
#include <string>
#include "../job.hpp"
#include "child_window.hpp"

class MachineEditBox : public ChildWindow{
public:
    wxStaticText* name;
    wxStaticText* label_name;
    wxStaticText* label_capacity;
    wxStaticText* label_cooldown;
    wxTextCtrl* input_name;
    wxTextCtrl* input_capacity;
    wxTextCtrl* input_cooldown;
    wxSizer* parent_sizer;
    wxFrame* parent;
    std::reference_wrapper<Machine> machine;

    void Clear() override;

    MachineEditBox(std::reference_wrapper<Machine> p,wxSizer* parent_sizer,wxFrame* parent);

private:
};
#endif