/**
 * @file ProgressDialog.cpp
 * @brief implements of ProgressDialog class
 *
 * @author S.Tanaka
 * @date 2011.05.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation.
 */


#include "stdafx.h"
#include "ProgressDialog.h"

#include "CollapsiblePane.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MESSAGE_MARGIN                 12
#define BORDER_SIZE						3
#define GRID_GAP                        3

#define DIALOG_WIDTH                  400
#define INFO_HEIGHT                   250




BEGIN_EVENT_TABLE( ProgressDialog, StandardDialog )
END_EVENT_TABLE()


// constructor
ProgressDialog::ProgressDialog( wxWindow* parent, const char* title )
		: StandardDialog( parent, title ) {
	// initialize
	m_stopped = false;
	m_range = 100;

	m_gauge = NULL;
	m_msgText = NULL;
	m_elapsedText = NULL;
	m_estimatedText = NULL;
	m_remainingText = NULL;
	m_infoText = NULL;
	m_disabler = NULL;
	m_sizer = NULL;
	m_infoCtrl = NULL;

	// timer start
	m_timer.start();
}

// destructor
ProgressDialog::~ProgressDialog() {
	if( m_disabler != NULL ) {
		delete m_disabler;
	}
	m_disabler = NULL;
}

// set range
void ProgressDialog::setRange( const int range ) {
	// range
	m_range = std::max( 1, range );

	// gauge
	if( m_gauge != NULL ) {
		m_gauge->SetRange( m_range );
	}
}

// get range
int ProgressDialog::getRange() {
	return m_range;
}

// set position
void ProgressDialog::setPos( const int pos ) {
	// position
	if( pos <= 0 ) {
		return;
	}
	m_pos = std::min( pos, m_range );

	// gauge
	if( m_gauge != NULL ) {
		m_gauge->SetValue( m_pos );
	}

	// time
	if( m_elapsedText != NULL && m_estimatedText != NULL && m_remainingText != NULL ) {
		double elapsed = m_timer.getTotalTime();
		double estimated = elapsed * (double)m_range / (double)m_pos;
		double remaining = estimated - elapsed;

		// set time
		std::string str = getTimeString( elapsed );
		m_elapsedText->SetLabel( wxT( str.c_str() ) );

		str = getTimeString( estimated );
		m_estimatedText->SetLabel( wxT( str.c_str() ) );

		str = getTimeString( remaining );
		m_remainingText->SetLabel( wxT( str.c_str() ) );
	}
}

// get position
int ProgressDialog::getPos() {
	return m_pos;
}

// set message
void ProgressDialog::setMessage( const char* msg ) {
	// message
	m_message = NVL( msg, "" );

	if( m_msgText != NULL ) {
		m_msgText->SetLabel( wxT( m_message.c_str() ) );
	}
}

// get message
const char* ProgressDialog::getMessage() {
	return m_message.c_str();
}

// set information
void ProgressDialog::setInformation( const char* title, const char* info ) {
	// set title
	m_infoTitle = NVL( title, "" );

	// remove old control
	bool collapsed = true;
	if( m_sizer != NULL && m_infoCtrl != NULL ) {
		collapsed = ((CollapsiblePane*)m_infoCtrl)->IsCollapsed();
		m_sizer->Remove( m_infoCtrl );
		m_infoCtrl->Destroy();
		m_infoCtrl = NULL;
	}

	// create information control
	if( m_sizer != NULL && !m_infoTitle.empty() ) {
		CollapsiblePane* pane = new CollapsiblePane(
			this,
			m_infoTitle.empty() ? "Information" : m_infoTitle.c_str()
		);
		m_infoCtrl = pane;

		m_sizer->Add( pane, 1, wxALL | wxGROW, BORDER_SIZE );

		// info
		wxWindow* win = pane->GetPane();
		m_infoText = new wxTextCtrl(
			win,
			wxID_ANY,
			wxT( m_info.c_str() ),
			wxDefaultPosition,
			wxSize( DIALOG_WIDTH, INFO_HEIGHT ),
			wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL
		);

		// add to pane
		wxSizer* paneSizer = new wxBoxSizer( wxVERTICAL );
		paneSizer->Add( m_infoText, 1, wxALL | wxGROW, BORDER_SIZE );

		win->SetSizer( paneSizer );
		paneSizer->SetSizeHints( win );
		pane->Collapse( collapsed );
	}

	setInformation( info );
	Layout();
	Fit();
	Refresh();
}

// set information
void ProgressDialog::setInformation( const char* info ) {
	// information
	m_info = NVL( info, "" );

	if( m_infoText != NULL ) {
		m_infoText->SetValue( wxT( m_info.c_str() ) );
	}
}

// get information title
const char* ProgressDialog::getInformationTitle() { 
	return m_infoTitle.c_str();
}

// get informatino
const char* ProgressDialog::getInformation() {
	return m_info.c_str();
}

// stopped
bool ProgressDialog::isStopped() {
	return m_stopped;
}

// get time string
std::string ProgressDialog::getTimeString( const double sec ) {
	// sec, min, hour
	int s = roundnum( sec );
	int m = s / 60;
	int h = m / 60;
	m = m % 60;
	s = s % 60;

	// string
	std::string str = FMT( "%02d:%02d:%02d", h, m, s );
	return str;
}

// create main sizer
wxSizer* ProgressDialog::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	m_sizer = sizer;

	wxFlexGridSizer* basicSizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	basicSizer->AddGrowableCol( 0 );

	// message text
	m_msgText = new wxStaticText( this, wxID_ANY, wxT( m_message.c_str() ) );
	basicSizer->Add( m_msgText, 1, wxALL | wxGROW, MESSAGE_MARGIN );

	// gauge
	m_gauge = new wxGauge(
		this,
		wxID_ANY,
		m_range,
		wxDefaultPosition,
		wxSize( DIALOG_WIDTH, -1 ),
		wxGA_HORIZONTAL | wxGA_SMOOTH
	);
	m_gauge->Pulse();
	basicSizer->Add( m_gauge, 1, wxALL | wxGROW, BORDER_SIZE );

	// time sizer
	wxGridSizer* timeSizer = new wxGridSizer( 3, 2, GRID_GAP, 20 );

	timeSizer->Add(
		new wxStaticText(
			this,
			wxID_ANY,
			wxT( "Elapsed Time:" ),
			wxDefaultPosition,
			wxDefaultSize,
			wxALIGN_RIGHT
		),
		1,
		wxALL | wxGROW,
		BORDER_SIZE
	);

	m_elapsedText = new wxStaticText( this, wxID_ANY, wxEmptyString );
	timeSizer->Add( m_elapsedText, 1, wxALL | wxGROW, BORDER_SIZE );

	timeSizer->Add(
		new wxStaticText(
			this,
			wxID_ANY,
			wxT( "Estimated Time:" ),
			wxDefaultPosition,
			wxDefaultSize,
			wxALIGN_RIGHT
		),
		1,
		wxALL | wxGROW,
		BORDER_SIZE
	);

	m_estimatedText = new wxStaticText( this, wxID_ANY, wxEmptyString );
	timeSizer->Add( m_estimatedText, 1, wxALL | wxGROW, BORDER_SIZE );

	timeSizer->Add(
		new wxStaticText(
			this,
			wxID_ANY,
			wxT( "Remaining Time:" ),
			wxDefaultPosition,
			wxDefaultSize,
			wxALIGN_RIGHT
		),
		1,
		wxALL | wxGROW,
		BORDER_SIZE
	);

	m_remainingText = new wxStaticText( this, wxID_ANY, wxEmptyString );
	timeSizer->Add( m_remainingText, 1, wxALL | wxGROW, BORDER_SIZE );

	basicSizer->Add( timeSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	sizer->Add( basicSizer, 0, wxALL | wxGROW, BORDER_SIZE );

	// info
	if( !m_info.empty() ) {
		setInformation( m_infoTitle.c_str(), m_info.c_str() );
	}

	return sizer;
}

// create button sizer
wxSizer* ProgressDialog::createButtonSizer() {
	return CreateButtonSizer( wxCANCEL );
}

// on cancel
bool ProgressDialog::onCancel() {
	// set flag
	m_stopped = true;

	// default
	return StandardDialog::onCancel();
}

// show
bool ProgressDialog::Show( bool show ) {
	// disabler
	if( show ) {
		if( m_disabler == NULL ) {
			m_disabler = new wxWindowDisabler( this );
		}
	}
	else {
		if( m_disabler != NULL ) {
			delete m_disabler;
		}
		m_disabler = NULL;
	}

	// default
	return StandardDialog::Show( show );
}
