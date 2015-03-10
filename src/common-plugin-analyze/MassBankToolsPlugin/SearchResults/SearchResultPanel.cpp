/**
 * @file SearchResultPanel.cpp
 * @brief Search Result Panel
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SearchResultPanel.h"

#include "ResultListCtrl.h"
#include "../MassBankManager.h"

#define GRID_GAP						3
#define BORDER_SIZE						3

#define ID_EXPORT_BUTTON	10001
#define ID_SEARCH_BUTTON	10002
#define ID_MIN_MENU_BUTTON  10100
#define ID_MAX_MENU_BUTTON  10110

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// ---------------------------------------------------------------------------------
// SearchResultListPanel
// ---------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SearchResultListPanel, kome::window::StandardPanel)
	EVT_BUTTON(ID_EXPORT_BUTTON, SearchResultListPanel::exportData)
	EVT_BUTTON(ID_SEARCH_BUTTON, SearchResultListPanel::findKeywordFromData)
	EVT_UPDATE_UI(ID_EXPORT_BUTTON, SearchResultListPanel::updateUI)
	EVT_UPDATE_UI(ID_SEARCH_BUTTON, SearchResultListPanel::updateUI)
	EVT_COMMAND_RANGE(ID_MIN_MENU_BUTTON, ID_MAX_MENU_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, onSelectMenuButton)
	EVT_UPDATE_UI_RANGE(ID_MIN_MENU_BUTTON, ID_MAX_MENU_BUTTON, onUpdateUIMenuButton)
END_EVENT_TABLE()

// constructor
SearchResultListPanel::SearchResultListPanel(
	wxWindow* parent
	) : kome::window::StandardPanel(parent)
	, resultListCtrl_(NULL)
	, flags(0)
	, searchButton_(nullptr)
	, exportButton_(nullptr)
	, modalsync_(false)
{
	createControls();
}

// destructor
SearchResultListPanel::~SearchResultListPanel() 
{
}

void SearchResultListPanel::create() {
	this->createControls();
}

// ADD @date 2011.08.16 <Tue> M.Fukuda#eisai 
wxSizer* SearchResultListPanel::createMainSizer() 
{	
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3, 1 ,GRID_GAP, GRID_GAP);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);

	// Set Menu Buttons.
	wxBoxSizer* menuButtonSizer = onCreateMenuButtons();
	sizer->Add(menuButtonSizer, 0, wxALL|wxGROW,1);

	// Set ListCtrl
	resultListCtrl_ = new ResultListCtrl(this, -1, 120, false);
	resultListCtrl_->create();
	sizer->Add(resultListCtrl_, 1, wxALL|wxGROW, 3);
	
	// Set Bottom Buttoms.
	wxBoxSizer* bottomButtonSizer = onCreateBottomButtons();
	sizer->Add(bottomButtonSizer, 0, wxALIGN_RIGHT, BORDER_SIZE);

	return sizer;
}

wxBoxSizer* SearchResultListPanel::onCreateMenuButtons()
{
	onMakeMenuButtons("massbank_search_result", menu_button_infos_);
	wxBoxSizer* menuButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	int btCount(0);

	typedef std::vector<MenuButtonInfo>::iterator kITR;
	for (kITR it = menu_button_infos_.begin(), eit = menu_button_infos_.end(); it != eit; ++it) {
		std::string tooltip = FMT("Toggle %s on / off", it->func_item->getLongName());

		it->button = new wxBitmapButton(
			this,
			ID_MIN_MENU_BUTTON + btCount,
			it->ok_icon
		);

		it->button->SetToolTip(wxT(tooltip.c_str()));
		menuButtonSizer->Add(it->button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
		flags += it->flagVal;
		btCount++;
	}
	
	MassBankManager::getInstance().setSearchResultDialogShowFlag(flags);
	return menuButtonSizer;
}

wxBoxSizer* SearchResultListPanel::onCreateBottomButtons()
{
	wxBoxSizer* bottomButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	searchButton_ = new wxButton(this, ID_SEARCH_BUTTON, wxT("Search.."));
	bottomButtonSizer->Add(searchButton_, 0, wxALIGN_RIGHT | wxALL, 1);
	exportButton_ = new wxButton(this, ID_EXPORT_BUTTON, wxT( "Export.." ));
	bottomButtonSizer->Add(exportButton_, 0, wxALIGN_RIGHT | wxALL, 1);

	return bottomButtonSizer;
}


// メニューボタンクリック時
// 1.(Spec Canvas の切り替え)
// 2.(Prof　Text Boxの切り替え）
void SearchResultListPanel::onMakeMenuButtons(
	const char* funcType,
	std::vector<MenuButtonInfo>& refArr
){
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int num = plgMgr.getNumberOfFunctionItems(funcType);

	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	if(num > 0) {
		refArr.resize(num);
	} else {
		return;
	}
	unsigned int flagval(1);
	for(unsigned int i = 0 ; i < num; i++) {
		MenuButtonInfo& binfo = refArr[i];
		binfo.func_item = plgMgr.getFunctionItem(funcType, i);
		kome::plugin::PluginIcon* icon = binfo.func_item->getCall()->getIcon();

		if(icon != NULL) {
			// Set OK ICON			
			wxImage img(wxT(icon->getPath()), wxBITMAP_TYPE_PNG);
			wxBitmap bitmapOk(img);
			wxBitmap bitmapNg(img.ConvertToGreyscale());
			binfo.ok_icon.CopyFromBitmap(bitmapOk);
			binfo.ng_icon.CopyFromBitmap(bitmapNg);
		}
		binfo.flagVal = flagval;
		flagval *= 2;
	}
}

// Reload My data
void SearchResultListPanel::upDateMe() 
{
	if (resultListCtrl_ != NULL ) {
		resultListCtrl_->upDateMe();
		resultListCtrl_->updateList();
	}
}

// save data
void SearchResultListPanel::exportData(wxCommandEvent& evt) 
{
	resultListCtrl_->exportData();
}

// Search Key クリック時
void SearchResultListPanel::findKeywordFromData(wxCommandEvent& evt)
{
	wxTextEntryDialog dialog(
		this,
		wxT("Keyword Search"),
		wxT("Keyword"),
		wxT(""),
		wxOK|wxCANCEL);
	if (dialog.ShowModal() == wxID_OK) {
		modalsync_ = true;
		wxString keys = dialog.GetValue();
		// Modalの別画面を呼び出す
		MassBankManager::getInstance().searchKeywordFromDetailes(keys.mb_str());
		modalsync_ = false;
	}
}

// update UI
void SearchResultListPanel::updateUI( wxUpdateUIEvent& evt ) {
	
	MassBankManager& msbkmgr = MassBankManager::getInstance();
	bool isfg = msbkmgr.isVisibileMassBankRecordPane();
	for each(auto& btinf in menu_button_infos_) {
		btinf.button->Enable(isfg);
	}

	if(resultListCtrl_->getNumberOfData() > 0) {
		exportButton_->Enable(true);
		searchButton_->Enable(true);
	} else {
		exportButton_->Enable(false);
		searchButton_->Enable(false);
	}
	if(modalsync_) {
		unsigned int mflags = msbkmgr.getSearchResultDialogShowFlag();
		if (mflags != flags) {
			// Changed. so update.
			flags = mflags;
			int idx = 0;
			for each(auto& info in menu_button_infos_) {
				bool ebFg = ((flags & info.flagVal) != 0);
				if(info.enable != ebFg) {
					onToggleMenuButton(idx);
				}
				idx++;
			}
		}
	}

	evt.Enable( true );
}

// メニューボタンクリック時
void SearchResultListPanel::onSelectMenuButton(wxCommandEvent& evt){
	int idx = evt.GetId() - ID_MIN_MENU_BUTTON;
	if( idx < 0 || idx >= static_cast<int>(menu_button_infos_.size())) {
		return;
	}
	onToggleMenuButton(idx);

	flags ^= menu_button_infos_[idx].flagVal;

	MassBankManager::getInstance().setSearchResultDialogShowFlag(flags);
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
}

void SearchResultListPanel::onToggleMenuButton(const unsigned int idx)
{
	MenuButtonInfo* info = &menu_button_infos_[idx];
	if(info->enable) {
		// Need Dialog Close
		info->button->SetBitmapLabel(info->ng_icon);
		info->enable = false;
	} else {
		// Need Dialog Open
		info->button->SetBitmapLabel(info->ok_icon);
		info->enable = true;
	}
}

void SearchResultListPanel::onUpdateUIMenuButton(wxUpdateUIEvent& evt)
{
}

