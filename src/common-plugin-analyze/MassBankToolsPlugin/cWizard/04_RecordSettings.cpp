/**
 * @file 04_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "04_RecordSettings.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\CreatePanelTool.h"
#include "Panels\AuxiliaryToolOfCreatingPanel.h"

// suport functions
#include "calcFunc\getNowDate.h"

// Define String
#include "WizardConstHeaders.h"
#include "CommonLicenseArray.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(RecordSettingsWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
RecordSettingsWizardPage::RecordSettingsWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem) 
 : kome::window::StandardWizardPage(wparent)
 , m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W04::IMG);
	createControls();
}

// create main sizer
wxSizer* RecordSettingsWizardPage::createMainSizer() 
{
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	RecordSettingsPanel* panel1 = new RecordSettingsPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(RecordSettingsPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

// constructor
RecordSettingsPanel::RecordSettingsPanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings
) : kome::window::ParametersAndHelpPanel(parent)
	, m_rcElem(rcElem)
	, inParams_(NULL,false)
	, inSettings_(inSettings)
{
	createControls();
}
				
// destructor
RecordSettingsPanel::~RecordSettingsPanel(){}

// create sizer (virtual)
wxSizer* RecordSettingsPanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 3, MTR::W04::TITLE);
	sizer->AddGrowableRow(2);
	sizer->AddGrowableRow(3);
	sizer->AddGrowableRow(4);

	// Get CCL License
	std::vector<std::string> enums, dscs;
	CCL::getLisenceElum(enums, dscs);
	pfc::ComboElm cbm;
	cbm.setVals(enums, dscs, enums);

	// setUp panel Auxi
	pfc::CreatePanelTool auxi(inParams_);

	// Title Text Box
	auxi.makeStr(kTitle, HLP::WIZ::TITLE, true, DFT::MGW::TITLE);

	// Date Text Box
	auxi.makeStr(kDate, HLP::WIZ::DATE, false, ext::getNowDate(), false);

	// Authors Text Box
	auxi.makeStr(kAuthors, HLP::WIZ::AUTHOR);

	// CC License Combo box
	auxi.setComboBox(cbm, true);
	auxi.makeThis(kLicense, HLP::WIZ::LICENSE);

	// Copyright Text Box
	auxi.makeStr(kCopyright, HLP::WIZ::COPYRIGHT);

	// Publication Text Box
	auxi.makeStr(kPublication, HLP::WIZ::PUBLICATION);

	kome::window::SettingsPanel* panel1 = auxi.generateSettingPanel(this, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);

	// Next Panel
	pfc::CreatePanelTool auxi2(inParams_);

	// Comment List Box
	auxi2.setListBox(false, DFT::MGW::FILENAME);
	auxi2.setListElmString(k_list_Comment, true);
	auxi2.makeThis(kComment, HLP::WIZ::COMMENT);

	kome::window::SettingsPanel* panel2 = auxi2.generateSettingPanel(this, inSettings_);
	sizer->Add(panel2, 1, wxALL|wxGROW, BORDER_SIZE);

	// List End Msg
	wxStaticText* ListEndMsg = new wxStaticText(this, wxID_STATIC, HLP::WIZ::PROP_LIST);
	ListEndMsg->Wrap(-1);
	sizer->Add(ListEndMsg, 1, wxALL|wxGROW|wxALIGN_LEFT);

	return sizer;
}

// Trancefer From Window (virtual)
bool RecordSettingsPanel::TransferDataFromWindow() {
	// record info
		// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	pfc::spvAdpt sp(inSettings_);

	m_rcElem->setRecordInfo(
		sp.val(kTitle),
		sp.val(kDate),
		sp.val(kAuthors),
		sp.val(kLicense),
		sp.val(kCopyright),
		sp.val(kPublication)
	);
	// comments
	m_rcElem->setComments(sp.val(kComment));

	return true;
}

