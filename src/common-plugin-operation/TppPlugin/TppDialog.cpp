/**
 * @file TppDialog.cpp
 * @brief implements of TppDialog class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppDialog.h"
#include "TppSearchGrid.h"
#include "TppManager.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;
using namespace kome::ident;

#define DEFAULTSIZE_HEIGHT 400
#define DEFAULTSIZE_WIDTH 520

#define DIALOG_TITLE "TPP"
#define PEP_XML_SUFFIX ".pep.xml"

// static member
TppDialog* TppDialog::m_dlg = NULL;

BEGIN_EVENT_TABLE( TppDialog,  kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_CHECKBOX( ID_CHECK_ALL, TppDialog::OnClickCheckAll )
	EVT_BUTTON( ID_BUTTON_APPLY, TppDialog::OnClickButtonApply )
	EVT_BUTTON( ID_BUTTON_CLOSE, TppDialog::OnClickButtonClose )
END_EVENT_TABLE()

// constructor
TppDialog::TppDialog( wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, DIALOG_TITLE, NULL,	// @Date:2014/01/24	<extend help>	A.Ozaki
		-1, -1, DEFAULTSIZE_WIDTH, DEFAULTSIZE_HEIGHT) {
	// initialize
	m_listSearchResult = NULL;
	m_checkAll = NULL;
	m_btnApply = NULL;
	m_btnClose = NULL;
	m_textComment = NULL;

}

// destructor
TppDialog::~TppDialog() {
}

// create main sizer.
wxSizer* TppDialog::createMainSizer()
{
	//create main sizer (row 3, col 1)
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer(3,1,5,5);
	mainSizer->AddGrowableRow(0);
	mainSizer->AddGrowableCol(0);
	mainSizer->AddGrowableCol(2);

	//create footer sizer.
	wxFlexGridSizer* footerSizer = new wxFlexGridSizer(1,4,5,5);
	footerSizer->AddGrowableCol(1);

	//create search result list.
	m_listSearchResult = new TppSearchGrid(this,ID_LIST_SEARCH_RESULT);
	m_listSearchResult->create();
	mainSizer->Add(m_listSearchResult, 0, wxALL | wxGROW, 20);

	//create checkbox.
	m_checkAll = new wxCheckBox(this, ID_CHECK_ALL, "All");
	mainSizer->Add(m_checkAll, 1, wxLEFT | wxGROW, 20);

	//create text "Comment:"
	wxStaticText* label = new wxStaticText(this, wxID_ANY, "Comment:"); 
	footerSizer->Add(label, 0, wxLEFT, 20);

	//create comment box.
	m_textComment = new	wxTextCtrl(this, ID_TEXTCONTROL_COMMENT);
	footerSizer->Add(m_textComment, 1, wxLEFT | wxRIGHT | wxGROW, 5);
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_textComment, "write a comment." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki
	
	//create apply button.
	m_btnApply = new wxButton(this, ID_BUTTON_APPLY, "Apply");
	if(m_listSearchResult->getRowSize() == 0){
		m_btnApply->Enable(false);
	}
	footerSizer->Add(m_btnApply, 2, wxLEFT, 5);

	//create close button.
	m_btnClose = new wxButton(this, ID_BUTTON_CLOSE, "Close");
	footerSizer->Add(m_btnClose, 3, wxLEFT, 5);

	//footer sizer add to main sizer.
	mainSizer->Add(footerSizer, 2, wxALL | wxGROW, 5);

	return mainSizer;
}

// clicked Apply button event
void TppDialog::OnClickButtonApply( wxCommandEvent& evt )
{
	std::vector<long long> selectedData;
	m_listSearchResult->getSelectedData(selectedData);

	if(selectedData.size() == 0){
		return;
	};
	
		
	Tpp::TppManager& tppMgr = TppManager::getInstance();

	ident::IdentificationManager& identMgr = ident::IdentificationManager::getInstance();

	std::string folderPath = tppMgr.createResultFolder();
	std::string targets = "";
	std::vector< int > searches;
	identMgr.getSearches(searches);

	for each(long long index in selectedData){
		if(m_listSearchResult->isSelected(index)){
			SearchResult result;
			identMgr.getSearch(index, result);

			std::string strDate = result.getDate();
			int find;
			while((find = strDate.find(':', 0)) != std::string::npos){
				strDate.erase(find, 1);
			}
			while((find = strDate.find(' ', 0)) != std::string::npos){
				strDate.erase(find, 1);
			}
			std::string strFileName = FMT("%s_%s%s",
				strDate.c_str(), result.getTitle().c_str(), PEP_XML_SUFFIX);
			identMgr.writePepXML(FMT("%s\\%s%s",folderPath.c_str(),
				TPP_PERSONAL_XML_FOLDERNAME, strFileName.c_str()).c_str(),
				result, true, NULL);
			tppMgr.setPepXML(strFileName);
			targets += FMT("%s(%s),",result.getTitle().c_str(), result.getSearchEngine()->getName().c_str());
		}
	}

	tppMgr.createResult("");

	wxDateTime now = wxDateTime::Now();
	std::string dateTime = FMT("%02d %s %04d %02d:%02d:%02d",
		now.GetDay(),
		wxDateTime::GetMonthName(
			now.GetMonth(), wxDateTime::Name_Abbr).c_str(),
		now.GetYear(),
		now.GetHour(),
		now.GetMinute(),
		now.GetSecond());

	tppMgr.insertResult(
		dateTime.c_str(),
		targets.c_str(),
		m_textComment->GetValue().c_str(),
		folderPath.c_str()
		);

}

// clicked Close button event
void TppDialog::OnClickButtonClose( wxCommandEvent& evt ){
	this->Close();
}

// clicked All Checkbox event
void TppDialog::OnClickCheckAll( wxCommandEvent& evt ){
	if(m_checkAll->GetValue()){
		m_listSearchResult->selectAll();
	}
	else{
		m_listSearchResult->deselectAll();
	}
}
