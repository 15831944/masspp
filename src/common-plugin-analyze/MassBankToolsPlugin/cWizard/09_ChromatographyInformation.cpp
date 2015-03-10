/**
 * @file 09_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "09_ChromatographyInformation.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
//#include "Panels\PanelFactory.h"
#include "Panels\ExtendSettingsPanel.h"

#include "ErrorCheck.h"

// Define String
#include "WizardConstHeaders.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// Chrom

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
//#include "images\massbank_flow09.xpm"
BEGIN_EVENT_TABLE(ChromatographyInformationWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
ChromatographyInformationWizardPage::ChromatographyInformationWizardPage(
	wxWizard* wparent, RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W09::IMG);
	createControls();
}

// create main sizer
wxSizer* ChromatographyInformationWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	ChromatographyInformationPanel* panel1 = new ChromatographyInformationPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(ChromatographyInformationPanel, kome::window::ParametersAndHelpPanel)
    EVT_KOME_SETTINGS_PANEL(wxID_ANY, ChromatographyInformationPanel::onSettingsPanel )
END_EVENT_TABLE()

ChromatographyInformationPanel::ChromatographyInformationPanel(
	wxWindow* parent, RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	,inParams_(NULL,false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
	,uniqueChkFg_(false)
{
	createControls();
}

ChromatographyInformationPanel::~ChromatographyInformationPanel(){}


wxSizer* ChromatographyInformationPanel::createMainSizer(
) {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 2, MTR::W09::TITLE);
	sizer->AddGrowableRow(2);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);
	
	// Addtional chromPanel
	m_rcElem->getACChromSubTags(subtagArr_, subtagngArr_);
	SettingsValue* val = pfc::setSubTagListValue(inParams_, subtagArr_);

	//	default Set Value : [RETENTION_TIME,%RT%]
	std::string listdefval = FMT("[%s,%s]", k_AC_CH_RetentionTime.c_str(), kDefRetenstionTime.c_str());
	val->setDefaultValue(listdefval.c_str());

	pfc::setParamAndFormAdpt(inParams_, val, page,LBL::W09::LISTINFO, LBL::W09::LISTINFO, HLP::WIZ::CHROMADDINFO,true);

	//kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(this, page, false, &inSettings_, true);
//	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);
	panelEx = new ExtendSettingsPanel(this, page, false, &inSettings_, true);

	sizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);

	// List End Msg
	wxStaticText* ListEndMsg = new wxStaticText(this, wxID_STATIC, HLP::WIZ::PROP_LIST);
	ListEndMsg->Wrap(-1);
	sizer->Add(ListEndMsg, 1, wxALL|wxGROW|wxALIGN_LEFT);

	return sizer;
}

// Trancefer From Window (virtual)
bool ChromatographyInformationPanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string chromsub = inSettings_.getParameterValue(LBL::W09::LISTINFO.c_str());
	if(!CHK::checkSubTags(chromsub, subtagngArr_, true)) {
		return false;
	}

	m_rcElem->setSubChrom(chromsub);
	return true;
}

void ChromatographyInformationPanel::onSettingsPanel(
	kome::window::SettingsPanelEvent& evt
) {
	kome::plugin::SettingsForm* form = evt.getForm();
	if(form == NULL) {
		return;
	}
	std::string formtitle = form->getTitle();
	// trap link panel
	if (LBL::W09::LISTINFO.compare(formtitle) == 0) {
		onCheckChromInfo();
	}
}

void ChromatographyInformationPanel::onCheckChromInfo(
){
	std::string exval = panelEx->getValue(LBL::W09::LISTINFO);
	bool fg,ufg;
	if (!exval.empty()) {
		ufg = !uniqueChkFg_;
		fg = CHK::checkSubTags(exval, subtagngArr_, ufg);
		//正常系に戻ったらユニークチェックを戻す
		uniqueChkFg_ = !fg;
	}
}
