/**
 * @file 06_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "06_Sampleinformation.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
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


const std::string kExDataBase = "Biological Species in External Database";

const std::string kSamples = "Sample Pareparations";

const std::string kWrnMsg =
	"One of database and accession is empty but both of them are required \n\
if you want to link to database for biological species.";

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------

BEGIN_EVENT_TABLE(SampleInformationWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
SampleInformationWizardPage::SampleInformationWizardPage(
	wxWizard* wparent, RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent), m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W06::IMG);
	createControls();
}

// create main sizer
wxSizer* SampleInformationWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SampleInformationPanel* panel1 = new SampleInformationPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(SampleInformationPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

SampleInformationPanel::SampleInformationPanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	, inParams_(NULL,false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
{
	createControls();
}

SampleInformationPanel::~SampleInformationPanel(){}

wxSizer* SampleInformationPanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 2, MTR::W06::TITLE);
	sizer->AddGrowableRow(3);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);

	// Scientific Name Text Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	pfc::setParamAndFormAdpt(inParams_, val, page, kScientificName, kScientificName, HLP::WIZ::SCIENTIFICNAME,true);

	SettingsGroup* gp = inParams_.addGroup();
	gp->setName(kExDataBase.c_str());

	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	pfc::setParamAndFormAdptGroup(inParams_, val, page, gp, kDataBase, kDataBase, HLP::WIZ::DATABASE, true);

	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	pfc::setParamAndFormAdptGroup(inParams_, val, page, gp, kAccession, kAccession, HLP::WIZ::ACCESSION,true);

	SettingsPage* page2 = new SettingsPage(inParams_);
	
	// Comment List Box
	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_LIST);

	kome::plugin::SettingsValue* subVal = inParams_.addValue();
	subVal->setType(SettingsValue::TYPE_STRING);
	subVal->setRequired(true);
	subVal->setName(k_list_Sample.c_str());
	val->addChildValue(subVal);
	pfc::setParamAndFormAdpt(inParams_, val, page2, kSamples, kSamples, HLP::WIZ::SAMPLES,true);

	kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(this, page, false, &inSettings_, true);
	kome::window::SettingsPanel* panel2 = new kome::window::SettingsPanel(this, page2, false, &inSettings_, true);

	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);
	sizer->Add(panel2, 1, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}

// Trancefer From Window (virtual)
bool SampleInformationPanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string spDB  = inSettings_.getParameterValue(kDataBase.c_str());
	std::string spAcc = inSettings_.getParameterValue(kAccession.c_str());

	if(spDB.empty() ^ spAcc.empty()) {
		kome::window::WindowTool::showWarning(kWrnMsg.c_str(),"warning");
		return false;
	}

	std::string spSct = inSettings_.getParameterValue(kScientificName.c_str());
	std::string spSpl = inSettings_.getParameterValue(kSamples.c_str());

	m_rcElem->setSPStatus(spSct, spDB, spAcc, spSpl);

	return true;
}
