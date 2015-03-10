/**
 * @file 15_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#include "rel_stdafx.h"
#include "15_FinalSequence.h"
#include "RecordElements.h"

#include "Panels\SpectraGridList.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_SELECT_LIST 999

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(FinalSequenceWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
FinalSequenceWizardPage::FinalSequenceWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem
)	: kome::window::StandardWizardPage(wparent), m_rcElem(rcElem)
{
	m_bitmap = wxNullBitmap;
	createControls();
}

// create main sizer
wxSizer* FinalSequenceWizardPage::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	FinalSequencePanel* panel1 = new FinalSequencePanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(FinalSequencePanel, kome::window::ParametersAndHelpPanel)
	EVT_LIST_ITEM_SELECTED(ID_SELECT_LIST, onSct)
END_EVENT_TABLE()

// constructor
FinalSequencePanel::FinalSequencePanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings
) : kome::window::ParametersAndHelpPanel(parent)
	, m_rcElem(rcElem)
	, inParams_(NULL,false)
	, m_settings(inSettings)
{
	createControls();
}

// destructor
FinalSequencePanel::~FinalSequencePanel(){}

wxSizer* FinalSequencePanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 1, GRID_GAP, GRID_GAP);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);

	using namespace kome::plugin;

	// page
	SettingsPage* page = new SettingsPage(inParams_);

	m_listp = new SpectraGridList(this,ID_SELECT_LIST, 400,300);
	m_listp->create();

	sizer->Add(m_listp, 1, wxALL|wxGROW, BORDER_SIZE);
	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_canvas = new kome::window::SpectrumCanvas(this, 200, 300, NULL);
	m_text = new wxTextCtrl(this, wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(200,-1),wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);

	sizer2->Add(m_canvas, 0, wxALL|wxGROW, BORDER_SIZE);
	sizer2->Add(m_text, 1, wxALL|wxGROW, BORDER_SIZE);

	sizer->Add(sizer2, 0, wxALL|wxGROW, BORDER_SIZE);

	return sizer;
}

// 画面移行時
bool FinalSequencePanel::TransferDataToWindow() {
	initSpecList();
	showSelectedInfo(0);
	
	return true;
}

// Spectrum Listの初期化
void FinalSequencePanel::initSpecList()
{
	//通常のSpectrumのセット
	std::vector<kome::objects::Spectrum*> specArr;
	m_rcElem->getSpectra(specArr);
	m_listp->setSelectedSpectra(specArr);
	// AveregedSpectraのセット
	setAverageSpectra();
//
}

// AveragedSpectraの初期化
void FinalSequencePanel::setAverageSpectra()
{
	std::vector<kome::objects::AveragedSpectrum*>& avespectra = m_rcElem->getAveSpectra();

	std::vector<kome::objects::Spectrum*> rst;
	for each(auto p in avespectra) {
		rst.push_back(p);
	}
	if(!rst.empty()){
		m_listp->setSelectedSpectra(rst,false);
	}
}

void FinalSequencePanel::showSelectedInfo(const int idx)
{
	kome::objects::Spectrum* spec = m_listp->getSelectSpectrum(idx);
	std::string rst("");
	kome::objects::Spectrum* spec2 = nullptr;

	m_rcElem->getMassBankPreText(spec,spec2,rst);
	m_text->SetValue(wxT(rst.c_str()));
	m_canvas->setSpectrum(spec2);

	//m_text->SetValue(wxT(m_rcElem->getMassBankPreTexts(spec).c_str()));
	//m_canvas->setSpectrum(spec);
	this->Refresh();
}


void FinalSequencePanel::onSct(wxListEvent& evt) {
	int f = evt.GetItem().GetId();
	showSelectedInfo(f);
}
