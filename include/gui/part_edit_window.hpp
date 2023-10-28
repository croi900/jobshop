#ifndef PART_EDIT_WINDOW_HGUARD
#define PART_EDIT_WINDOW_HGUARD

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


#define PART_NAME_BTN 101
#define PART_AMOUNT_BTN 102

/**
 * ONLY USE INSIDE MainWindow::* functions.
*/

class PartEditBox : public ChildWindow{
public:
    wxStaticText* name;
    wxStaticText* label_name;
    wxStaticText* label_amount;
    wxTextCtrl* input_name;
    wxTextCtrl* input_amount;
    wxSizer* parent_sizer;
    wxFrame* parent;

    // wxButton* save_button;

    std::reference_wrapper<PartOrder> part;

    void Clear() override;

    PartEditBox(std::reference_wrapper<PartOrder> p,wxSizer* parent_sizer,wxFrame* parent);

private:
};
#endif