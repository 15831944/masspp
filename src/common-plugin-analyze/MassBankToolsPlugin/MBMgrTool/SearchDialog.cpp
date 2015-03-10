/**
 * @file SearchDialog.h
 * @brief implement of SearchDialog
 *
 * @author S.Tanaka
 * @date 2010.03.26
 * 
 * @modify M.Fukuda
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "SearchDialog.h"

//#include "MassBankManager.h"
#include "InstrumentSelectDialog.h"
#include "../XMLHandler\settingParamsKeywords.h"

// Extends
#include "../cWizard\Panels\AuxiliaryToolOfCreatingPanel.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE				3
#define GRID_GAP				3
#define INSTRUMENTS_BUTTON_ID	999

// EVENT TABLE
BEGIN_EVENT_TABLE( SearchDialog, kome::window::StandardDialog )
	EVT_BUTTON( INSTRUMENTS_BUTTON_ID, SearchDialog::onCall_InstDialog )
END_EVENT_TABLE()

// constructor
SearchDialog::SearchDialog(
	wxWindow* parent,
	const SearchType type,
	kome::objects::SettingParameterValues& settings,
	const char* title,
	kome::objects::Spectrum* spec
	) : kome::window::StandardDialog(parent, title)
	, m_type(type)
	, m_settings(settings)
	, m_params(NULL, false)
	, spec_(spec)
{
}

// destructor
SearchDialog::~SearchDialog() {
}

// creates similar spectra search page
void SearchDialog::createSimilarSpectraSearchPage(
	kome::plugin::ParameterSettings& params, 
	kome::plugin::SettingsPage* page 
) {
	kome::plugin::SettingsValue* val = nullptr;
	int msstage(-1);
	double precursor(-1.0);

	// Spectrum Check.
	if(spec_ != nullptr) {
		msstage = spec_->getMsStage();
		if (msstage > 1) {
			precursor = spec_->getPrecursor();
		}
	}

	// unit
	val = onCreateTolUnitVals(params);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::UNIT, "Tolerance Unit", "Select the tolerance unit. (""unit"" means Da.)", true);

	// Set Tolerance
	val = pfc::createDoubleSTValue(params,0.0, pfc::kdNoUse, "0.3", true);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::TOL, "Tolerance","Error tolerance.", true);

	// Set Cut Off
	val = pfc::createIntegerSTValue(params, 0, 999, "5", true);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::CUTOFF, "Cutoff", "Threshold of the relative peak intensity.", true);

	// precursor
	std::string prcStr = (precursor > 0.0) ? FMT("%f",precursor) : "";
	val = pfc::createDoubleSTValue(params, 0.0, pfc::kdNoUse, prcStr, false);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::PRECURSOR, "Precursor (m/z)", "If you search MSn (n>1), you can specify precursor m/z.", false);

	// Ms Type
	val = onCreateMsTypeVals(params);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::MSTYPE, "MS Type", "", false);

	// Set Ion Mode
	val = onCreateIonVals(params);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::IONMODE, "Ion Mode", "Select positive or negative ion mode.", true);

	// Set Max Number of Results
	val = pfc::createIntegerSTValue(params, 1, pfc::knNoUse, "", false);
	pfc::setParamAndFormAdpt(params, val, page, CPRM::SPEC::MAXNUM, "Max Number of Results", "Set the max number of results.", true);
}

// create peak search page
void SearchDialog::createPeakSearchPage(
		kome::plugin::ParameterSettings& params,
		kome::plugin::SettingsPage* page,
		const bool diff
) {
	// Set Select m/z Edit List
	std::string title = ( diff ? "m/z Difference" : "m/z" );
	const std::string lstprm = (diff) ? CPRM::DIFF::VALLIST : CPRM::SRCH::MZLIST;
	const std::string intprm = (diff) ? CPRM::DIFF::INTNSTY : CPRM::SRCH::INTNSTY;
	const std::string tolprm = (diff) ? CPRM::DIFF::TOL : CPRM::SRCH::TOL;
	const std::string mstprm = (diff) ? CPRM::DIFF::MSTYPE : CPRM::SRCH::MSTYPE;
	const std::string ionprm = (diff) ? CPRM::DIFF::IONMODE : CPRM::SRCH::IONMODE;
	const std::string numprm = (diff) ? CPRM::DIFF::MAXNUM : CPRM::SRCH::MAXNUM;

	// Set M/Z List
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_LIST);

	kome::plugin::SettingsValue* childVal = params.addValue();
	childVal->setType(kome::plugin::SettingsValue::TYPE_DOUBLE);
	childVal->getNumberRestriction().setInclusiveMinValue( 0.0 );
	childVal->setName(title.c_str());

	val->addChildValue(childVal);
	val->setRequired(true);

	pfc::setParamAndFormAdpt(params, val, page, lstprm, title, "", true);

	// Relative intensity
	val = pfc::createIntegerSTValue(params, 0, 999, "5", true);
	pfc::setParamAndFormAdpt(params, val, page, intprm, "Relative Intensity (0-999)", "", true);
	
	// Tolerance
	val = pfc::createDoubleSTValue(params, 0.0, pfc::kdNoUse, "0.3", true);
	pfc::setParamAndFormAdpt(params, val, page, tolprm, "Tolerance", "", true);

	// Ms Type
	val = onCreateMsTypeVals(params);
	pfc::setParamAndFormAdpt(params, val, page, mstprm, "MS Type", "", false);

	// Ion Mode
	val = onCreateIonVals(params);
	pfc::setParamAndFormAdpt(params, val, page, ionprm, "Ion Mode", "", true);

	// Max Number Of Results
	val = pfc::createIntegerSTValue(params, 1, pfc::knNoUse, "", false);
	pfc::setParamAndFormAdpt(params, val, page, numprm, "Max Number of Results", "", true);
}

// Set ElumItems - Unit Vals Type (unit or ppm) -
kome::plugin::SettingsValue* SearchDialog::onCreateTolUnitVals(
	kome::plugin::ParameterSettings& params
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_STRING);
	val->addEnumItem("unit", "unit", "unit");
	val->addEnumItem("ppm", "ppm", "ppm");
	val->setDefaultValue("unit");
	val->setRequired(true);
	return val;
}


// Set ElumItems - Ion Type (+ or - or both) -
kome::plugin::SettingsValue* SearchDialog::onCreateIonVals(
	kome::plugin::ParameterSettings& params
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_STRING);
	val->addEnumItem("Positive", "positive", "positive");
	val->addEnumItem("Negative", "negative", "negative");
	val->addEnumItem("Both", "both", "positive & negative");
	val->setDefaultValue("both");
	val->setRequired(true);
	return val;
}

// Set ElumItems - Ms Type -
kome::plugin::SettingsValue* SearchDialog::onCreateMsTypeVals(
	kome::plugin::ParameterSettings& params
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_STRING);
	val->addEnumItem("All", "All", "All");
	val->addEnumItem("MS", "MS", "MS");
	val->addEnumItem("MS2", "MS2", "MS2");
	val->addEnumItem("MS3", "MS3", "MS3");
	val->setDefaultValue("All");
	val->setRequired(true);
	return val;
}

// creates main sizer
wxSizer* SearchDialog::createMainSizer() 
{
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	kome::plugin::ParameterSettings params( NULL, true );

	// create page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage(m_params);
	if(m_type == TYPE_DIFF) {
		createPeakSearchPage( m_params, page, true );
	} else if(m_type == TYPE_PEAKS) {
		createPeakSearchPage( m_params, page, false );
	} else {
		createSimilarSpectraSearchPage( m_params, page );
	}

	// panel
	kome::window::SettingsPanel* panel = 
		new kome::window::SettingsPanel(this, page, true, &m_settings ,true);
	sizer->Add(panel, 1, wxALL | wxGROW, BORDER_SIZE);

	return sizer;
}

// Create Button Sizer
wxSizer* SearchDialog::createButtonSizer() 
{	
	wxGridSizer* sizer = new wxGridSizer(2, 1 ,GRID_GAP, GRID_GAP);

	wxButton* instButton = new wxButton(
		this,
		INSTRUMENTS_BUTTON_ID,
		wxT( "Instruments..." )
	);
	sizer->Add( instButton, 1, wxALIGN_CENTER, BORDER_SIZE );

	wxGridSizer* btSizer = new wxGridSizer( 1, 2, GRID_GAP, GRID_GAP );

	wxButton* okButton = new wxButton(
		this,
		wxID_OK,
		wxT( "&Search" ),
		wxDefaultPosition,
		wxDefaultSize
	);

	btSizer->Add( okButton, 1, wxALL | wxGROW, BORDER_SIZE );

	// add Cancel button
	wxButton* cnclButton = new wxButton(
		this,
		wxID_CANCEL,
		wxT( "&Cancel" ),
		wxDefaultPosition,
		wxDefaultSize
	);
	btSizer->Add( cnclButton, 1, wxALL | wxGROW, BORDER_SIZE );
	sizer->Add(btSizer, 1, wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// Call instrument Dialog
void SearchDialog::onCall_InstDialog(wxCommandEvent& evt) 
{
	InstrumentSelectDialog dlg(this->GetParent());
	dlg.ShowModal();
}
