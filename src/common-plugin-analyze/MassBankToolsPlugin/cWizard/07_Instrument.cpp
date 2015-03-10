/**
 * @file 07_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "07_Instrument.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\InstrumentAccessPanel.h"
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
//#include "Panels\PanelFactory.h"

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
BEGIN_EVENT_TABLE(InstrumentWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
InstrumentWizardPage::InstrumentWizardPage(
	wxWizard* wparent, RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent), m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W07::IMG);
	createControls();
}

// create main sizer
wxSizer* InstrumentWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	InstrumentPanel* panel1 = new InstrumentPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(InstrumentPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

InstrumentPanel::InstrumentPanel(
	wxWindow* parent, RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	,inParams_(NULL,false), inSettings_(inSettings), m_rcElem(rcElem)
{
	createControls();
}

InstrumentPanel::~InstrumentPanel(){}


wxSizer* InstrumentPanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 2, MTR::W07::TITLE);
//	sizer->AddGrowableRow(2);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);
	
	//Œ»Ý‚ÌInstrument‚©‚çî•ñ‚ð“üŽè
	using namespace kome::objects;
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	Sample* tmpsample = aoMgr.getActiveSample();
	std::string keyStr = tmpsample->getGroup(0)->getProperties().getStringValue("Instrument","");

	kome::window::SettingsPanel* vend = 
		createVenderInstrumentComboPanel(this, inParams_, inSettings_, keyStr,HLP::WIZ::VENDOR, HLP::WIZ::INSTRUMENTS);

	// make Instrument Group (Dress)
	wxStaticBox* stBox = new wxStaticBox(this, wxID_STATIC, wxT(kInstrument.c_str()));
	wxStaticBoxSizer* stBoxSizer = new wxStaticBoxSizer(stBox, wxVERTICAL);
	stBoxSizer->Add(vend, 1, wxALL|wxGROW, BORDER_SIZE);

	// get Instrument Type Form MassBank Mgr
	std::vector<std::string> instTypeArr;
	getInstumentTypeFromMgr(instTypeArr);

	// Instrument Type Combo Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	std::for_each(instTypeArr.begin(), instTypeArr.end(),
		[&](const std::string& s){val->addEnumItem(s.c_str(),s.c_str(),s.c_str());});
	val->addEnumItem("###","","");
	val->setRequired(true);

	pfc::setParamAndFormAdpt(inParams_, val, page, kInstrumentType, kInstrumentType, HLP::WIZ::INSTTYPE, true);
	kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(this, page, false, &inSettings_, true);

	sizer->Add(stBoxSizer, 1, wxALL|wxGROW, BORDER_SIZE);
	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}

// Trancefer From Window (virtual)
bool InstrumentPanel::TransferDataFromWindow() {
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}

	std::string vend = inSettings_.getParameterValue(kVender.c_str());
	std::string instName = inSettings_.getParameterValue(kInstrumentName.c_str());
	std::string instType = inSettings_.getParameterValue(kInstrumentType.c_str());

	return m_rcElem->setACInstruments(vend, instName, instType);
}
