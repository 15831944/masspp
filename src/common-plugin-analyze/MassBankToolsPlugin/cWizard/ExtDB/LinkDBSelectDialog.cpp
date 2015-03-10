/**
 * @file   LinkDBSelectDialog.h
 * @brief  Link DB Access Select Dialog
 *
 * @author M.Fukuda
 * @date   2013.03.06 
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#include "LinkDBSelectDialog.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_COMBOB 7382

const std::string kINIPARAM_KEGG = "MASSBANK_KEGG_MOL_FLAG";

const std::string kWarnKEGG = 
	"For commercial users:\nYou should obtain a license agreement of KEGG, for commercial purposes.\n\
See the help page or the user's manual for details.\n";

// EVENT TABLE
BEGIN_EVENT_TABLE(DBACCSelectDialog, kome::window::StandardDialog)
	EVT_COMBOBOX(wxID_ANY, onSelectComBoBox)
END_EVENT_TABLE()

// constructor
DBACCSelectDialog::DBACCSelectDialog(
	wxWindow* parent, 
	std::vector<std::pair<std::string, std::string>>& dbarr
) : kome::window::StandardDialog(parent, "Select the link")
  , dbArr_(dbarr){}

// destructor
DBACCSelectDialog::~DBACCSelectDialog(){}

// create main sizer (virtual)
wxSizer* DBACCSelectDialog::createMainSizer() 
{
	selectId_ = 0;
	
	// main sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 1, GRID_GAP, GRID_GAP);

	// toplabel
	wxStaticText* toplabel = new wxStaticText(
		this,wxID_ANY,wxT("Select the link to import the chemical structure."));
	sizer->Add(toplabel,1, wxALL | wxGROW, BORDER_SIZE);

	// combo box
	wxArrayString vals;
	typedef std::pair<std::string, std::string> PItr;

	std::for_each(dbArr_.begin(), dbArr_.end(),
		[&](const PItr& tpair) {vals.Add(wxT(tpair.first + ": " + tpair.second));});

	wxComboBox* cbb = new wxComboBox(
		this, ID_COMBOB, vals[0], wxDefaultPosition, wxDefaultSize, 
		vals, wxCB_DROPDOWN|wxCB_READONLY);
	
	// combobox label
	wxStaticText* linklabel = new wxStaticText(this,wxID_ANY,wxT("Link"));

	// combo box & label
	wxBoxSizer* holsizer = new wxBoxSizer(wxHORIZONTAL);
	holsizer->Add(linklabel,0, wxALL | wxGROW, BORDER_SIZE);
	holsizer->Add(cbb,1, wxALL | wxGROW, BORDER_SIZE);
	sizer->Add( holsizer, 0, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// onSelect Combo Box
void DBACCSelectDialog::onSelectComBoBox(wxCommandEvent& evt) 
{
	// 選ばれた順番を返す
	selectId_ = evt.GetSelection();
	if (selectId_ < 0 || selectId_ > (int)dbArr_.size()) {
		selectId_ = -1;
	} else {
		// KEGG なら。アクセスの警告を表示。
		if(dbArr_[selectId_].first == "KEGG") {	
			kome::window::MessageCheckDialog::openDialogAfterChecking(
				this,
				"Caution",
				kWarnKEGG.c_str(),
				kINIPARAM_KEGG.c_str(), 
				kome::window::MessageDialog::ICON_WARNING
				);
		}
	}
}
