/**
 * @file 08_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "08_SpectrumInformation.h"

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


//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(SpectrumInformationWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
SpectrumInformationWizardPage::SpectrumInformationWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W08::IMG);
	createControls();
}

// create main sizer
wxSizer* SpectrumInformationWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SpectrumInformationPanel* panel1 = new SpectrumInformationPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(SpectrumInformationPanel, kome::window::ParametersAndHelpPanel)
    EVT_KOME_SETTINGS_PANEL(wxID_ANY, SpectrumInformationPanel::onSettingsPanel )
END_EVENT_TABLE()

SpectrumInformationPanel::SpectrumInformationPanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings
) : kome::window::ParametersAndHelpPanel(parent)
	,inParams_(NULL,false)
	,inSettings_(inSettings)
	,m_rcElem(rcElem)
	,uniqueChkFg_(false)
{
	createControls();
}
				
SpectrumInformationPanel::~SpectrumInformationPanel(){}

//adpt
void apAddEnum(
	kome::plugin::SettingsValue*& val,
	const std::string& ln,
	const std::string& sn,
	const std::string& dsc
){
	val->addEnumItem(ln.c_str(), sn.c_str(), dsc.c_str());
}

wxSizer* SpectrumInformationPanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 3, MTR::W08::TITLE);
	sizer->AddGrowableRow(3);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);
	SettingsPage* page2 = new SettingsPage(inParams_);

	// MS Stage Combo Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	apAddEnum(val, kAuto, kAuto, HLP::WIZ::MS::AUTO);
	apAddEnum(val, kNone, kNone, HLP::WIZ::MS::NONE);

	std::string tmpMS, tmpHelp;
	tmpMS = "MS";
	const char* basems = HLP::WIZ::MS::BASE.c_str();
	tmpHelp = FMT("The %d%s",1, basems);
	val->addEnumItem(tmpMS.c_str(), tmpMS.c_str(), tmpHelp.c_str());

	for (unsigned int i = 2; i <= 8; i++) {
		tmpMS = FMT("MS%d", i);
		tmpHelp = FMT("The %d%s",i, basems);
		val->addEnumItem(tmpMS.c_str(), tmpMS.c_str(), tmpHelp.c_str());
	}
	val->setRequired(true);
	pfc::setParamAndFormAdpt(inParams_, val, page, kMSStage, kMSStage, HLP::WIZ::MSSTAGE,true);

	// polarity
	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	
	apAddEnum(val, kAuto, kAuto, HLP::WIZ::POL::AUTO);
	apAddEnum(val, kNone, kNone, HLP::WIZ::POL::NONE);
	apAddEnum(val, kPositive, kPositive, HLP::WIZ::POL::POSI);
	apAddEnum(val, kNegative, kNegative, HLP::WIZ::POL::NEGA);
	val->setRequired(true);
	pfc::setParamAndFormAdpt(inParams_, val, page, kPolarity, kPolarity, HLP::WIZ::POLARITY, true);

	ExtendSettingsPanel* panelx = new ExtendSettingsPanel(this, page, false, &inSettings_, true);
	sizer->Add(panelx, 1, wxALL|wxGROW, BORDER_SIZE);

	// Addtional SpecPanel
	m_rcElem->getACSpecSubTags(subtagArr_, subtagngArr_);
	val = pfc::setSubTagListValue(inParams_, subtagArr_);

	// Set Def Value "[COLLISION_ENERGY,%Collision Energy%]"
	std::string listdefval = FMT("[%s,%s]", k_AC_Spec_ColEnergy.c_str(), kDefCollesionEnergy.c_str());
	val->setDefaultValue(listdefval.c_str());

	pfc::setParamAndFormAdpt(inParams_, val, page2, LBL::W08::LISTINFO, LBL::W08::LISTINFO, HLP::WIZ::SPECADDINFO,true);

	panelEx = new ExtendSettingsPanel(this, page2, false, &inSettings_, true);

	sizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);
	
	// List End Msg
	wxStaticText* ListEndMsg = new wxStaticText(this, wxID_STATIC, HLP::WIZ::PROP_LIST);
	ListEndMsg->Wrap(-1);
	sizer->Add(ListEndMsg, 1, wxALL|wxGROW|wxALIGN_LEFT);

	return sizer;
}

// Trancefer From Window (virtual)
bool SpectrumInformationPanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string mssub = inSettings_.getParameterValue(LBL::W08::LISTINFO.c_str());
	if(!CHK::checkSubTags(mssub, subtagngArr_, true)) {
		return false;
	}

	std::string msst = inSettings_.getParameterValue(kMSStage.c_str());
	std::string pol = inSettings_.getParameterValue(kPolarity.c_str());

	m_rcElem->setMSStage_Polarity(msst,pol);
	m_rcElem->setSubSpec(mssub);
	return true;
}

void SpectrumInformationPanel::onSettingsPanel(
	kome::window::SettingsPanelEvent& evt
) {
	kome::plugin::SettingsForm* form = evt.getForm();
	if(form == NULL) {
		return;
	}
	std::string formtitle = form->getTitle();
	// trap link panel
	if (LBL::W08::LISTINFO.compare(formtitle) == 0) {
		onCheckSpecAddInfo();
	}
}

void SpectrumInformationPanel::onCheckSpecAddInfo() 
{
	std::string exval = panelEx->getValue(LBL::W08::LISTINFO);
	bool fg,ufg;
	if (!exval.empty()) {
		ufg = !uniqueChkFg_;
		fg = CHK::checkSubTags(exval, subtagngArr_, ufg);
		//正常系に戻ったらユニークチェックを戻す
		uniqueChkFg_ = !fg;
	}
}

