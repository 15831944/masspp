/**
 * @file 11_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "11_SoftwareInformation.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
#include "panels\ExtendSettingsPanel.h"
#include "Panels\DataProcessing.h"

// Define String
#include "WizardConstHeaders.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
//#include "images\massbank_flow11.xpm"
BEGIN_EVENT_TABLE(SoftwareInformationWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
SoftwareInformationWizardPage::SoftwareInformationWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem
) : kome::window::StandardWizardPage(wparent), m_rcElem(rcElem)

{
	m_bitmap = getWizImg(MTR::W11::IMG);

//	m_bitmap = wxBitmap(massbank_flow11_xpm);
	createControls();
}

// create main sizer
wxSizer* SoftwareInformationWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SoftwareInformationPanel* panel1 = new SoftwareInformationPanel(this, inSettings_, m_rcElem);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(SoftwareInformationPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

SoftwareInformationPanel::SoftwareInformationPanel(
	wxWindow* parent,
	kome::objects::SettingParameterValues& inSettings,
	RecordElements*& rcElem)
	: kome::window::ParametersAndHelpPanel(parent),
		inParams_(NULL, false), inSettings_(inSettings), m_rcElem(rcElem)
{
	createControls();
}
				
SoftwareInformationPanel::~SoftwareInformationPanel(){}


wxSizer* SoftwareInformationPanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 1, MTR::W11::TITLE);
	sizer->AddGrowableRow(2);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);

	// Processing List Box
	kome::window::SettingsPanel* dpPanel = createDataProccessingListBox(this, inParams_, inSettings_,true);

	// Software Text Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string defVal = FMT("Mass++ %s", msppMgr.getVersion());
	val->setDefaultValue(defVal.c_str());
	pfc::setParamAndFormAdpt(inParams_, val, page, kSoftware, kSoftware, HLP::WIZ::SOFTWARE);

	// Peak Detection Text Box

	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	pfc::setParamAndFormAdpt(inParams_, val, page, kPeakDetection, kPeakDetection, HLP::WIZ::DETECTOR);

	m_panel = new ExtendSettingsPanel(this, page, false, &inSettings_, false);
	//kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(this, page, false, &inSettings_, false);

	sizer->Add(dpPanel, 1, wxALL|wxGROW, BORDER_SIZE);
	sizer->Add(m_panel, 1, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}


bool SoftwareInformationPanel::TransferDataToWindow() {
	if(!kome::window::ParametersAndHelpPanel::TransferDataToWindow()) {
		return false;
	}
	std::string tmp = m_rcElem->getPkDtStr();
	m_panel->setValue(kPeakDetection, tmp);
	tmp = m_panel->getValue(kSoftware);
	return true;
}

bool SoftwareInformationPanel::TransferDataFromWindow(){

	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string software = inSettings_.getParameterValue(kSoftware.c_str());
	std::string peakDetc = inSettings_.getParameterValue(kPeakDetection.c_str());
	std::string dtprocs = inSettings_.getParameterValue(kDataProcessing.c_str());

	// ‚±‚ÌŠÖ”‚ª“à•”‚ðƒNƒŠƒA‚µ‚Ä‚µ‚Ü‚¤‚Ì‚Åæ‚ÉŒÄ‚Î‚È‚¢‚Æ
	m_rcElem->setDataProc(dtprocs);

	// ‚±‚ê‚ç‚ª‚«‚¦‚Ä‚µ‚Ü‚¤B
	m_rcElem->setSoft_Peaks(software,peakDetc);

	return true;
}
