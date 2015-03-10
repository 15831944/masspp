/**
 * @file .ExternalDbSearchDialog.cpp
 * @brief Extarnal Database Search Dialog
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "ExternalDbSearchDialog.h"

const std::string kWarnKEGG = 
	"For commercial users:\n\
You should obtain a license agreement of KEGG, for commercial purposes.\n\
See the help page or the user's manual for details.\n";

const std::string kINIPARAM_KEGG = 
	"MASSBANK_KEGG_MOL_FLAG";

#define SECTION "MassBank"
#define EXDBKey "SearchForExtranalDatabaseNames"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE(SearchExternalDbCheckList, kome::window::CheckGridListCtrl)
END_EVENT_TABLE()

SearchExternalDbCheckList::SearchExternalDbCheckList(
	wxWindow* parent,
	const std::vector<kome::ident::ExternalDB*>& dbs
	) : kome::window::CheckGridListCtrl(parent, wxID_ANY, 300, 150, false)
	, keggIdx_(0)
	, announcefirsttimeFg_(true)
	, dbs_(dbs)
{
}

SearchExternalDbCheckList::~SearchExternalDbCheckList(void)
{
}

kome::ident::ExternalDB* 
	SearchExternalDbCheckList::getExternalDB(const unsigned int idx) const
{
	if (idx < dbs_.size()) {
		return dbs_[idx];
	}
	return nullptr;
}


void SearchExternalDbCheckList::onCreate() 
{
	addColumn("DbName", kome::window::GridListCtrl::TYPE_STRING);
	this->onSetupCheckList();
}

std::string SearchExternalDbCheckList::getString(
	const long long data, const int column) const
{
	return dbs_[data]->getName();
}

void SearchExternalDbCheckList::onSelectAll()
{
	showKEGGAccessConfirmation();
}

void SearchExternalDbCheckList::onSelectItem(const int item)
{
	if(item == keggIdx_) {
		showKEGGAccessConfirmation();
	}
	kome::window::CheckGridListCtrl::onSelectItem(item);
}

const bool SearchExternalDbCheckList::showKEGGAccessConfirmation()
{
	if(announcefirsttimeFg_) {
		if(this->isSelected(keggIdx_)) {
			kome::window::MessageCheckDialog::openDialogAfterChecking(
				this,
				"Caution",
				kWarnKEGG.c_str(),
				kINIPARAM_KEGG.c_str(), 
				kome::window::MessageDialog::ICON_WARNING
				);
//			this->setSelected(keggIdx_,true);
			// 起動時、二回目以降は表示しない。
			announcefirsttimeFg_ = false;
		}
	} else {
		return true;
	}
	return true;
}

#define EX_DB "EXTERNAL_DB"

void SearchExternalDbCheckList::onSetupCheckList(void)
{
	unsigned int num = dbs_.size();

	unsigned int mycnt(0);
	std::string keggKey("KEGG");
	std::string iniStr = this->getParameterIni();
	std::string dbname("");
	bool selectFg;
	for(unsigned int i = 0; i < num; i++) {
		// external DB
		dbname = dbs_[i]->getName();
		addData(mycnt, false);
		selectFg = iniStr.find(dbname) != std::string::npos;
		setSelected(mycnt, selectFg); 

		if(dbname == keggKey) {
			keggIdx_ = mycnt;
		}
		mycnt++;
	}
}

const bool SearchExternalDbCheckList::getSelectedDB(std::vector<kome::ident::ExternalDB*>& refArr)
{
	std::vector<long long> selectDatas;
	this->getSelectedData(selectDatas);
	if (selectDatas.empty()) {
		return false;
	}
	std::string rstStr("");
	kome::ident::ExternalDB* edb = nullptr;
	for each(auto& idx in selectDatas) {
		edb = this->getExternalDB(idx);
		refArr.push_back(edb);
		if(!rstStr.empty()) rstStr += ",";
		rstStr += edb->getName();
	}
	this->setParameterIni(rstStr);
	return true;
}

void SearchExternalDbCheckList::setParameterIni(const std::string& val)
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();
	if( ini != NULL ) {
		ini->setString(SECTION, EXDBKey, val.c_str());
	}
}

const std::string SearchExternalDbCheckList::getParameterIni()
{
	// parameters
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();
	if(ini != NULL) {
		return ini->getString(SECTION, EXDBKey, "");
	}
	return "";
}

// -------------------------------------------------------*
//
//     DIALOG
//
// -------------------------------------------------------*

BEGIN_EVENT_TABLE(SearchExternalDbDialog, kome::window::StandardDialog)
END_EVENT_TABLE()

// constructor
SearchExternalDbDialog::SearchExternalDbDialog(
	wxWindow* parent,
	const std::vector<kome::ident::ExternalDB*>& dbs,
	const std::string& defKeyword
	) : kome::window::StandardDialog(parent, "select External Database")
	, searchKeyword_(defKeyword)
	, extDbs_()
	, searchKey_(nullptr)
	, cklist_(nullptr)
	, dbs_(dbs)
{
}

// destructor
SearchExternalDbDialog::~SearchExternalDbDialog(){}

// createMainSizer
wxSizer* SearchExternalDbDialog::createMainSizer()
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3,1,0,0);

	// Add Search Key Text Box
	wxStaticText* stxt = new wxStaticText(this, wxID_STATIC,
		wxT("Input Search Keyword."),
		wxDefaultPosition,
		wxDefaultSize,
		wxALIGN_LEFT);
	sizer->Add(stxt, 1, wxALL|wxGROW, 2);

	searchKey_ = new wxTextCtrl(this, wxID_ANY);
	searchKey_->SetValue(wxT(searchKeyword_));
	sizer->Add(searchKey_, 1, wxALL|wxGROW, 2);


	// Add List
	kome::window::CheckGridListPanel* cglPanel = new kome::window::CheckGridListPanel( this );
	cklist_ = new SearchExternalDbCheckList(cglPanel, dbs_);
	cklist_->create();

	sizer->Add (cglPanel, 1, wxALL| wxGROW, 2);
	return sizer;
}

// onOK Button
bool SearchExternalDbDialog::onOK()
{
	// default
	return kome::window::StandardDialog::onOK();
}

bool SearchExternalDbDialog::TransferDataFromWindow()
{
	searchKeyword_ = searchKey_->GetValue().c_str();
	if (searchKeyword_.empty()) {
		std::string msg("Input Search KeyWord.");
		kome::window::WindowTool::showInfo(msg.c_str());
		return false;
	}

	if (!cklist_->getSelectedDB(extDbs_)) {
	// No Selected Button.
		std::string msg("Select 1 or more DataBase Name.");
		kome::window::WindowTool::showInfo(msg.c_str());
		return false;
	}

	return true;
}
