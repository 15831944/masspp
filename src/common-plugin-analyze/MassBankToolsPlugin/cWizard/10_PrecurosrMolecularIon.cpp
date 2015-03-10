/**
 * @file 10_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "10_PrecurosrMolecularIon.h"

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
BEGIN_EVENT_TABLE(PrecurosrMolecularIonWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
PrecurosrMolecularIonWizardPage::PrecurosrMolecularIonWizardPage(
	wxWizard* wparent, RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W10::IMG);
	createControls();
}

// create main sizer
wxSizer* PrecurosrMolecularIonWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	PrecurosrMolecularIonPanel* panel1 = new PrecurosrMolecularIonPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(PrecurosrMolecularIonPanel, kome::window::ParametersAndHelpPanel)
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, PrecurosrMolecularIonPanel::onSettingsPanel )
END_EVENT_TABLE()

PrecurosrMolecularIonPanel::PrecurosrMolecularIonPanel(
	wxWindow* parent, RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	, inParams_(NULL,false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
	, uniqueChkFg_(false)
{
	createControls();
}
				
PrecurosrMolecularIonPanel::~PrecurosrMolecularIonPanel(){}

wxSizer* PrecurosrMolecularIonPanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 3, MTR::W10::TITLE);
	sizer->AddGrowableRow(3);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);

	// get Ion Type Element (as [M+])
	std::vector<std::string>IonArr;
	m_rcElem->getMSIonElements(IonArr);

	SettingsValue* val = nullptr;

	// Precursor Type
	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	val->setRequired(true);
	std::for_each(IonArr.begin(), IonArr.end(),
		[&](const std::string& s){val->addEnumItem(s.c_str(),s.c_str(),s.c_str());});
	pfc::setParamAndFormAdpt(
		inParams_, val, page, kPrecursorType, kPrecursorType, HLP::WIZ::PREC_TYPE, true);

	ExtendSettingsPanel* panelx = new ExtendSettingsPanel(this, page, false, &inSettings_, true);
	sizer->Add(panelx, 1, wxALL|wxGROW, BORDER_SIZE);

	// Addtional ion param
	SettingsPage* pageEx = new SettingsPage(inParams_);

	m_rcElem->getMSIonSubTags(subtagArr_, subtagNgArr_);

	val = pfc::setSubTagListValue(inParams_, subtagArr_);
	// "[BASE_PEAK,%BPM%],[PRECURSOR_M/Z,%Precursor%]"
	std::string listdefval = FMT("[%s,%s],[%s,%s]", 
		k_MS_BasePeak.c_str(), kDefBPM.c_str(), k_MS_PrecMZ.c_str(), kDefPrecursor.c_str()); 
	val->setDefaultValue(listdefval.c_str());
	
	pfc::setParamAndFormAdpt(inParams_, val, pageEx, LBL::W10::LISTINFO, LBL::W10::LISTINFO, HLP::WIZ::PREC_MOLINFO,true);

	// Extend Panel
	panelEx = new ExtendSettingsPanel(this, pageEx, false, &inSettings_, true);

	sizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);

	// List End Msg
	wxStaticText* ListEndMsg = new wxStaticText(this, wxID_STATIC, HLP::WIZ::PROP_LIST);
	ListEndMsg->Wrap(-1);
	sizer->Add(ListEndMsg, 1, wxALL|wxGROW|wxALIGN_LEFT);

	return sizer;
}

// Trancefer From Window (virtual)
bool PrecurosrMolecularIonPanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}

	std::string prc = inSettings_.getParameterValue(LBL::W10::LISTINFO.c_str());
	if(!CHK::checkSubTags(prc, subtagNgArr_,true)) {
		return false;
	}

//  Ver 2.6.0 IonType Omit
//	std::string iontype = inSettings_.getParameterValue(kIonType.c_str());
	std::string prctype = inSettings_.getParameterValue(kPrecursorType.c_str());

//	m_rcElem->setIonType(iontype);
	m_rcElem->setPrcType(prctype);
	m_rcElem->setSubFcIon(prc);

	return true;
}

void PrecurosrMolecularIonPanel::onSettingsPanel(
	kome::window::SettingsPanelEvent& evt
) {
	kome::plugin::SettingsForm* form = evt.getForm();
	if(form == NULL) {
		return;
	}
	std::string formtitle = form->getTitle();
	
	// trap link panel
	if (LBL::W10::LISTINFO.compare(formtitle) == 0) {
		onCheckPrcInfo();
//		std::string exval = panelEx->getValue();
//		CHK::checkSubTags(exval, subtagNgArr_);
	}
}

void  PrecurosrMolecularIonPanel::onCheckPrcInfo()
{
	std::string exval = panelEx->getValue(LBL::W10::LISTINFO);
	bool fg,ufg;
	if (!exval.empty()) {
		ufg = !uniqueChkFg_;
		fg = CHK::checkSubTags(exval, subtagNgArr_, ufg);
		//正常系に戻ったらユニークチェックを戻す
		uniqueChkFg_ = !fg;
	}
}
