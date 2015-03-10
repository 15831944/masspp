/**
 * @file RecordPeaksDialog.cpp
 * @brief implements of RecordPeaksDialog
 *
 * @author S.Tanaka
 * @date 2009.09.15
 * 
 * @modify M.Fukuda
 * @date 2011.08.25
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "RecordPeaksDialog.h"
#include "../MassBankManager.h"
#include "PeaksCanvas.h"

#include "ImageCanvas.h"

#include <wx/splitter.h>

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE	3
#define GRID_GAP	3

// Event Table
BEGIN_EVENT_TABLE( RecordPeaksDialog, kome::window::StandardPanel )
END_EVENT_TABLE()

// constructor
RecordPeaksDialog::RecordPeaksDialog(wxWindow* parent) 
	: kome::window::StandardPanel(parent)
	, chemical_formula_(nullptr)
	, m_PeakCV(nullptr)
	, m_ProfTC(nullptr)
	, detail_splitter_(nullptr)
	, canvas_splitter_(nullptr)
	, oldflag_(6)
{
	createControls();
}

// destructor
RecordPeaksDialog::~RecordPeaksDialog() {
}

// creates main sizer
wxSizer* RecordPeaksDialog::createMainSizer() 
{
// +------+------+
// | peak | PNG  |
// +------+------+
// | infomation  |
// +-------------+

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1, 1 ,GRID_GAP, GRID_GAP);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableCol(0);

	int detail_width = 400;
	int detail_height = 240;

	// detail splitter
	detail_splitter_ = new wxSplitterWindow(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(detail_width, detail_height),
		wxSP_3D
	);

	detail_splitter_->SetSashGravity(0.75); // Rize Keep 3 : 1
	detail_splitter_->SetMinimumPaneSize(20);

	sizer->Add(detail_splitter_, 1, wxALL | wxGROW, BORDER_SIZE);

	int canvas_height = 180 - BORDER_SIZE;
	int peakcanvas_width = 200 - BORDER_SIZE;
	int chemicalPNG_witdh = 200;

	// canvas splitter
	canvas_splitter_ = new wxSplitterWindow(
		detail_splitter_,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(detail_width, canvas_height),
		wxSP_3D
	);
	
	canvas_splitter_->SetSashGravity(0.5);
	canvas_splitter_->SetMinimumPaneSize(20);

	// chemical formula
	chemical_formula_ = 
		new ImageCanvas(
		canvas_splitter_, chemicalPNG_witdh, canvas_height);

	// Spectrum canvas
	m_PeakCV = new PeaksCanvas(canvas_splitter_);

	// Set Canvas Splitter
	canvas_splitter_->Initialize(m_PeakCV);
	canvas_splitter_->SplitVertically(m_PeakCV, chemical_formula_, peakcanvas_width);

	// Peak Information
	m_ProfTC = new wxTextCtrl(
		detail_splitter_,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( -1, 60 ),
		wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL
	);

	MassBankManager& mgr = MassBankManager::getInstance();
	const char* info = mgr.getActiveRecordInfo();
	m_ProfTC->SetValue(wxT( NVL( info, "" )));

	// initialize splitter
	detail_splitter_->Initialize(canvas_splitter_);
	detail_splitter_->SplitHorizontally(canvas_splitter_, m_ProfTC, canvas_height);

	return sizer;
}

// update data info
void RecordPeaksDialog::upDateInfo() 
{
	MassBankManager& mgr = MassBankManager::getInstance();
	unsigned int flags = mgr.getSearchResultDialogShowFlag();

	// Show detailes
	if (oldflag_ != flags) {
		oldflag_ = flags;
		if((flags & 2) == 0) {
			detail_splitter_->Unsplit();
			m_ProfTC->Show(false);
		} else {
			m_ProfTC->Show(true);
			detail_splitter_->SplitHorizontally(canvas_splitter_, m_ProfTC, 180);
		}

		if ((flags & 1) == 0) {
			canvas_splitter_->Unsplit();
			canvas_splitter_->SplitVertically(chemical_formula_, m_PeakCV);
			canvas_splitter_->Unsplit();
			m_PeakCV->Show(false);
		} else {
			m_PeakCV->Show(true);
			canvas_splitter_->SplitVertically(m_PeakCV, chemical_formula_);
		}
	}

	chemical_formula_->setImageFile(mgr.getActiveRecordPngPath());
	
	chemical_formula_->Refresh();
	m_ProfTC->SetValue(wxT(NVL( mgr.getActiveRecordInfo(), "" )));
	m_ProfTC->Update();
	m_PeakCV->Refresh();
}

// on update UI
void RecordPeaksDialog::onUpdateOverlap( wxUpdateUIEvent& evt ) 
{
	// manager
//	MassBankManager& mgr = MassBankManager::getInstance();

	// active spectrum
	kome::objects::Spectrum* spec = NULL;
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		spec = obj.getSpectrum();
	}
	if( spec != NULL ) {
		spec = spec->getOrgSpectrum();
	}

	// update UI
	evt.Enable( spec != NULL );
}
