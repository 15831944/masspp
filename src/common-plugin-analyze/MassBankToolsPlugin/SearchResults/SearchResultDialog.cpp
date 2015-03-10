/**
 * @file SearchResultDialog.cpp
 * @brief Search Result Dialog
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SearchResultDialog.h"

#include "ResultListCtrl.h"
#include "../MassBankManager.h"

#include "SearchResultPanel.h"

#define GRID_GAP						3
#define BORDER_SIZE						3

#define ID_EXPORT_BUTTON	11001
#define ID_MIN1_MENU_BUTTON  11100
#define ID_MAX1_MENU_BUTTON  11110

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// ---------------------------------------------------------------------------------
// SearchResultListDialog
// ---------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SearchResultListDialog, kome::window::StandardDialog)
	EVT_BUTTON(ID_EXPORT_BUTTON, SearchResultListDialog::exportData)
	EVT_UPDATE_UI(ID_EXPORT_BUTTON, SearchResultListDialog::updateUI)
	EVT_COMMAND_RANGE(ID_MIN1_MENU_BUTTON, ID_MAX1_MENU_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, onSelectMenuButton)
	EVT_UPDATE_UI_RANGE(ID_MIN1_MENU_BUTTON, ID_MAX1_MENU_BUTTON, onUpdateUIMenuButton)
END_EVENT_TABLE()

// constructor
SearchResultListDialog::SearchResultListDialog(wxWindow* parent)
	: kome::window::StandardDialog(parent, "Filtered Search Results")
	, resultListCtrl_(NULL)
	,flags(0)
	,exportButton_(nullptr)
	,modalMode_(true)
{
}

// destructor
SearchResultListDialog::~SearchResultListDialog() 
{
}

// ADD @date 2011.08.16 <Tue> M.Fukuda#eisai 
wxSizer* SearchResultListDialog::createMainSizer() 
{	

	wxFlexGridSizer* sizer = new wxFlexGridSizer(3, 1 ,GRID_GAP, GRID_GAP);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);

	//// Set MainButtons
	onMakeMenuButtons("massbank_search_result", menu_button_infos_);
	wxBoxSizer* menuButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	int btCount(0);
	unsigned int flagVal(1);
	typedef std::vector<MenuButtonInfo>::iterator kITR;
	
	flags = MassBankManager::getInstance().getSearchResultDialogShowFlag();

	for (kITR it = menu_button_infos_.begin(), eit = menu_button_infos_.end(); it != eit; ++it) {
		std::string tooltip = FMT("Toggle1 %s on / off", it->func_item->getLongName());

		if((flags & flagVal) == 0) {
			it->button = new wxBitmapButton(
				this,
				ID_MIN1_MENU_BUTTON + btCount,
				it->ng_icon
			);
			it->enable = false;
		} else {
			it->button = new wxBitmapButton(
				this,
				ID_MIN1_MENU_BUTTON + btCount,
				it->ok_icon
			);
			it->enable = true;
		}
//		it->button->SetToolTip(wxT(tooltip.c_str()));
		btCount++;
		it->flagVal = flagVal;
		flagVal *= 2;
		menuButtonSizer->Add(it->button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
	}
	sizer->Add(menuButtonSizer, 0, wxALL|wxGROW,1);

	resultListCtrl_ = new ResultListCtrl(this, 400, 120, true);
	resultListCtrl_->create();
	sizer->Add(resultListCtrl_, 1, wxALL|wxGROW, 3);
	

	wxBoxSizer* bottomButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	exportButton_ = new wxButton(
		this,
		 ID_EXPORT_BUTTON,
		wxT( "Export.." ),
		wxDefaultPosition,
		wxDefaultSize
	);
	
	bottomButtonSizer->Add(exportButton_, 0, wxALIGN_RIGHT | wxALL, 1);

	sizer->Add(bottomButtonSizer, 0, wxALIGN_RIGHT, BORDER_SIZE);
	return sizer;
}

// メニューボタンクリック時
// 1.(Spec Canvas の切り替え)
// 2.(Prof　Text Boxの切り替え）
void SearchResultListDialog::onMakeMenuButtons(
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
void SearchResultListDialog::upDateMe() 
{
	if (resultListCtrl_ != NULL ) {
		resultListCtrl_->upDateMe();
		resultListCtrl_->updateList();
	}
}

// save data
void SearchResultListDialog::exportData(wxCommandEvent& evt) 
{
	resultListCtrl_->exportData();
}

// update UI
void SearchResultListDialog::updateUI( wxUpdateUIEvent& evt ) {
	if(resultListCtrl_->getNumberOfData() > 0) {
		exportButton_->Enable(true);
	} else {
		exportButton_->Enable(false);
	}
	evt.Enable( true );
}

// メニューボタンクリック時
void SearchResultListDialog::onSelectMenuButton(wxCommandEvent& evt){
	int idx = evt.GetId() - ID_MIN1_MENU_BUTTON;
	if( idx < 0 || idx >= static_cast<int>(menu_button_infos_.size())) {
		return;
	}

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
	flags ^= menu_button_infos_[idx].flagVal;

	MassBankManager::getInstance().setSearchResultDialogShowFlag(flags);
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
}

void SearchResultListDialog::onUpdateUIMenuButton(wxUpdateUIEvent& evt)
{
}

