/**
 * @file 03_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "03_PeakDetection.h"

// Element Box
#include "RecordElements.h"

#include "WizardConstHeaders.h"

// Panel Support Headers
#include "Panels\ExtendSettingsPanel.h"
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
#include "Panels\CreatePanelTool.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


const std::string kPeakDetector("Peak Detector");
const std::string kPeakFilter("Peak Filtering");
const std::string k_list_Method("Method");

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------

BEGIN_EVENT_TABLE(PeakDetectionWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
PeakDetectionWizardPage::PeakDetectionWizardPage(
	wxWizard* wparent,RecordElements*& rcElem
) : kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{
	m_bitmap = 	getWizImg(MTR::W03::IMG);
	createControls();
}

// create main sizer
wxSizer* PeakDetectionWizardPage::createMainSizer() 
{
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	PeakDetectionPanel* panel1 = new PeakDetectionPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(PeakDetectionPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

// constructor
PeakDetectionPanel::PeakDetectionPanel(
	wxWindow* parent,
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings
	) : kome::window::ParametersAndHelpPanel(parent)
	, inParams_(NULL,false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
{
	createControls();
}

// destructor
PeakDetectionPanel::~PeakDetectionPanel(){}

// create sizer (virtual)
wxSizer* PeakDetectionPanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 1, MTR::W03::TITLE);
//	sizer->AddGrowableRow(2);

	using namespace kome::plugin;
	pfc::CreatePanelTool auxi(inParams_);
	auxi.setEnumComboBox(PLG::KEY::SpecPeakPicking,"",true,"gion");
	auxi.makeThis(kPeakDetector, HLP::WIZ::PEAKDETECT, true);

	auxi.setEnumListBox(k_list_Method,PLG::KEY::SpecPeakFilter,true);
	auxi.makeThis(kPeakFilter, HLP::WIZ::PEAKFILTER, true);

	ExtendSettingsPanel* panelEx = auxi.generateExtendPanel(this, inSettings_);
	sizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);
//	kome::window::SettingsPanel* panel1 = auxi.generateSettingPanel(this,inSettings_);
//	panelEx = auxi.generateExtendPanel(this, inSettings_);
//	panelEx = new ExtendSettingsPanel(this, page, false, &inSettings_, true);


	//// page
	//SettingsPage* page = new SettingsPage(inParams_);

	//// Peak Detector Select
	//SettingsValue* val = inParams_.addValue();
	//val->setType(SettingsValue::TYPE_STRING);
	//val->setEnumType(SettingsValue::ENUM_PLUGIN);
	//val->setEnumKey(PLG::KEY::SpecPeakPicking.c_str());
	//val->setRequired(true);
	//val->setDefaultValue("gion");
	//pfc::setParamAndFormAdpt(inParams_, val, page, kPeakDetector, kPeakDetector, HLP::WIZ::PEAKDETECT, true);

	//val = inParams_.addValue();
	//val->setType(SettingsValue::TYPE_LIST);
	//SettingsValue* childVal = inParams_.addValue();
	//childVal->setEnumType(SettingsValue::ENUM_PLUGIN);
	//childVal->setEnumKey(PLG::KEY::SpecPeakFilter.c_str());
	//childVal->setName(k_list_Method.c_str());
	//childVal->setRequired(true);
	//val->addChildValue(childVal);
	//pfc::setParamAndFormAdpt(inParams_, val, page, kPeakFilter, kPeakFilter, HLP::WIZ::PEAKFILTER, true);

	//// Extend Settings Panels (For Multi Action)
	//panelEx = new ExtendSettingsPanel(this, page, false, &inSettings_, true);

//	sizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}

// data transfer (virtual)
bool PeakDetectionPanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	// create detector name;
	pfc::spvAdpt sp(inSettings_);
	std::string xval = sp.val(kPeakDetector);
	kome::objects::SettingParameterValues* subval = sp.subvals(kPeakDetector);

//	std::string xval = inSettings_.getParameterValue(kPeakDetector.c_str());
//	kome::objects::SettingParameterValues* subval = inSettings_.getSubParameterValues(kPeakDetector.c_str());
	
	if (xval.empty()) {
		return false;
	}
	m_rcElem->setPeakDetector(xval, subval);
//	m_rcElem->setPeakFilter(inSettings_.getParameterValue(kPeakFilter.c_str()));
	m_rcElem->setPeakFilter(sp.val(kPeakFilter).c_str());

	return true;
}
