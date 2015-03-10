/**
 * @file 12_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "12_PeakAnnotation.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\AuxiliaryToolOfCreatingPanel.h"

#include <boost/assign/list_of.hpp>

#include "WizardConstHeaders.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


const std::string kAnnotation2("Annotation");


//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(PeakAnnotationWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
PeakAnnotationWizardPage::PeakAnnotationWizardPage(
	wxWizard* wparent, RecordElements*& rcElem) 
	: kome::window::StandardWizardPage(wparent), m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W12::IMG);
	createControls();
}

// create main sizer
wxSizer* PeakAnnotationWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	PeakAnnotationPanel* panel1 = new PeakAnnotationPanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(PeakAnnotationPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

PeakAnnotationPanel::PeakAnnotationPanel(
	wxWindow* parent, RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	,inParams_(NULL,false), inSettings_(inSettings), m_rcElem(rcElem)
{
	createControls();
}

PeakAnnotationPanel::~PeakAnnotationPanel(){}

wxSizer* PeakAnnotationPanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 1, MTR::W12::TITLE);
	using namespace kome::plugin;
	sizer->AddGrowableRow(2);

	// page
	SettingsPage* page = new SettingsPage(inParams_);
	std::vector<std::string> pkCols = boost::assign::list_of
		("m/z")("m/z Tol.")("m/z Tol.unit")("Start RT")("End RT")
		("Precursor")("Precursor Tol.")("Precursor Tol.unit");
	std::vector<std::string> pkCol2s = boost::assign::list_of("Type")("Formula");

	// Prefix Text Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_LIST);

	SettingsValue* subVal;
	int nk = 0;
	std::for_each(pkCols.begin(), pkCols.end(), [&](const std::string& cols) {
		subVal = inParams_.addValue();
		if(nk == 2||nk == 7) {
			subVal->setType(kome::plugin::SettingsValue::TYPE_STRING);
			subVal->addEnumItem("Da","Da","Da");
			subVal->addEnumItem("ppm","ppm","ppm");
			subVal->setRequired(true);
		} else {
			subVal->setType(kome::plugin::SettingsValue::TYPE_DOUBLE);
			subVal->getNumberRestriction().setInclusiveMinValue(0.0);
		}
		subVal->setName(cols.c_str());
		// êÊì™ÇÃmz ÇÃÇ›ïKê{
		if (nk <= 1) subVal->setRequired(true);
		val->addChildValue(subVal);
		nk++;
	});

	std::for_each(pkCol2s.begin(), pkCol2s.end(), [&](const std::string& cols) {
		subVal = inParams_.addValue();
		subVal->setType(kome::plugin::SettingsValue::TYPE_STRING);
		subVal->setName(cols.c_str());
		val->addChildValue(subVal);
	});

	pfc::setParamAndFormAdpt(inParams_, val, page, kAnnotation2, kAnnotation2, HLP::WIZ::ANNOTATION,true);
	kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(this, page, false, &inSettings_, true);
	panel1->SetSize(400, 500);

	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}

bool PeakAnnotationPanel::TransferDataFromWindow() {
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string annostr = inSettings_.getParameterValue(kAnnotation2.c_str());
	return m_rcElem->setAnnoDatas(annostr);
}
