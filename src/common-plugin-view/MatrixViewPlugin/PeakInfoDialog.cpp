/**
 * @file PeakInfoDialog.cpp
 * @brief implementsation of PeakInfoDialog class
 *
 * @author S.Tanaka
 * @date 2012.04.19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "PeakInfoDialog.h"
#include "MatrixViewManager.h"

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE      5


BEGIN_EVENT_TABLE( PeakInfoDialog, kome::window::StandardDialog )
	EVT_CLOSE( PeakInfoDialog::close )
END_EVENT_TABLE()


// constructor
PeakInfoDialog::PeakInfoDialog( wxWindow* pParent, kome::plugin::PluginFunctionItem* item, int peakId ) 
        : kome::window::StandardDialog( pParent, "" ) {
	// initialize
	m_peakId = peakId;
	m_wnd = NULL;
	m_sizer = NULL;
	m_item = item;
}

// destructor
PeakInfoDialog::~PeakInfoDialog() {
	if( m_wnd != NULL ){
		delete m_wnd;
	}
	m_wnd = NULL;
}

// close
void PeakInfoDialog::close(wxCloseEvent& evt ){
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	mgr.onClosePeakInfoDlg();
}

// set peak ID
void PeakInfoDialog::setPeakId( const int peakId ) {	
	
	// check the peak ID
	if( peakId == m_peakId ) {
		return;
	}

	// set
	m_peakId = peakId;

	// window
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setInt( params, "peak", m_peakId );
	kome::plugin::PluginCallTool::setParentWindow( params, this );
	
	wxWindow* wnd = (wxWindow*)( m_item->getCall()->invoke( &params ).prim.pt );

	if( wnd != NULL && wnd != m_wnd ) {
		m_sizer->Replace( m_wnd, wnd );
		m_wnd = NULL;	// @date 2012.10.16 <Mod> M.Izumi
		m_wnd = wnd;
		Refresh();
		m_wnd->Refresh();
	}

	setTitle();	
}

// set title
void PeakInfoDialog::setTitle() {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// peak info
	double rt = identMgr.getMatrixPeakRt( m_peakId );
	double mz = identMgr.getMatrixPeakMz( m_peakId );

	// title
	std::string title = m_item->getLongName();
	if( rt >= 0.0 || mz >= 0.0 ) {
		title.append( " [" );
		if( rt >= 0.0 ) {
			title.append( FMT( "rt=%.2f", rt ) );
		}
		if( mz >= 0.0 ) {
			if( rt >= 0.0 ) {
				title.append( ", " );
			}
			title.append( FMT( "mz=%.2f", mz ) );
		}
		title.append( "]" );
	}

	// set title
	SetTitle( wxT( title.c_str() ) );
}

// create top sizer
wxSizer* PeakInfoDialog::createMainSizer() {
	
	// sizer
	m_sizer = new wxBoxSizer( wxVERTICAL );

	// window
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setInt( params, "peak", m_peakId );
	kome::plugin::PluginCallTool::setParentWindow( params, this );

	m_wnd = (wxWindow*)( m_item->getCall()->invoke( &params ).prim.pt );
	if( m_wnd != NULL ) {
		m_sizer->Add( m_wnd, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	// title
	setTitle();

	return m_sizer;
}

// create button sizer
wxSizer* PeakInfoDialog::createButtonSizer() {
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add(
		new wxButton( this, wxID_OK, wxT( "Close" ) ),
		0,
		wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	return sizer;
}
