/**
 * @file CheckGridListPanel.cpp
 * @brief implements of CheckGridListPanel class
 *
 * @author S.Tanaka
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"

#include "CheckGridListPanel.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE       3

#define ID_ALL_BUTTON     999



BEGIN_EVENT_TABLE( CheckGridListPanel, HelpGetterPanel )
	EVT_CHECKBOX( ID_ALL_BUTTON, CheckGridListPanel::onAll )
	EVT_UPDATE_UI( ID_ALL_BUTTON, CheckGridListPanel::onUpdateAll )
END_EVENT_TABLE()


// constructor
CheckGridListPanel::CheckGridListPanel(
		wxWindow* parent,
		const int id,
		AllButtonAlign align,
		const char* allLabel,
		const bool showNum ) : HelpGetterPanel( parent, id ) {
	// initialize
	m_checkList = NULL;
	m_allCheckBox = NULL;
	m_align = align;
	m_allLabel = allLabel;
	m_showNum = showNum;
	m_numText = NULL;
}

// destructor
CheckGridListPanel::~CheckGridListPanel() {
}

// set check list
void CheckGridListPanel::setCheckGridListCtrl( CheckGridListCtrl* checkList ) {
	m_checkList = checkList;

	createControls();
}

// get check list
CheckGridListCtrl* CheckGridListPanel::getCheckGridListCtrl() {
	return m_checkList;
}

// set the number of selected items
void CheckGridListPanel::setNumberOfSelectedItems( const int num ) {
	// check the member
	if( m_numText == NULL ) {
		return;
	}

	// display
	std::string s;
	if( num > 0 ) {
		s = FMT( "%d Selected", num );
	}
	else {
		s = "None Selected";
	}

	m_numText->SetLabel( wxT( s.c_str() ) );
}

// create main sizer
wxSizer* CheckGridListPanel::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// check list
	if( m_checkList == NULL ) {
		return sizer;
	}
	m_checkList->create();

	// button
	if(( m_align != NONE_ALL ) && (!m_allLabel.empty())) {
		m_allCheckBox = new wxCheckBox(
			this,
			ID_ALL_BUTTON,
			wxT( m_allLabel.c_str() ),
			wxDefaultPosition,
			wxDefaultSize,
			wxCHK_3STATE
		);
	}

	// sizer
	if(( m_align == TOP_ALL ) && (!m_allLabel.empty())) {
		sizer->Add( m_allCheckBox, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	sizer->Add( m_checkList, 1, wxALL | wxGROW, BORDER_SIZE );

	if(( m_align == BOTTOM_ALL ) && (!m_allLabel.empty())) {
		sizer->Add( m_allCheckBox, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	// show num
	int cnt = 0;
	m_numText = new wxStaticText( this, wxID_ANY, wxT( "None Selected" ) );
	sizer->Add( m_numText, 0, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// on check all
void CheckGridListPanel::onAll( wxCommandEvent& evt ) {
	// selection
	bool checked = true;

	for( unsigned int i = 0; i < m_checkList->getNumberOfData() && checked; i++ ) {
		const long long data = m_checkList->getData( i );

		if( !m_checkList->isSelected( data ) ) {
			checked = false;
		}
	}

	// select or deselect
	if( checked ) {
		m_checkList->deselectAll();
	}
	else {
		m_checkList->selectAll();
	}

    Refresh();
}

// on update All check box
void CheckGridListPanel::onUpdateAll( wxUpdateUIEvent& evt ) {
	// selection
	bool checked = true;
	bool unchecked = true;

	for( unsigned int i = 0; i < m_checkList->getNumberOfData(); i++ ) {
		const long long data = m_checkList->getData( i );

		if( m_checkList->isSelected( data ) ) {
			unchecked = false;
		}
		else {
			checked = false;
		}
	}

	if (m_allCheckBox != NULL)
	{
		// enable
		m_allCheckBox->Enable( !checked || !unchecked );

		// state
		if( checked && !unchecked ) {
			m_allCheckBox->Set3StateValue( wxCHK_CHECKED );
		}
		else if( !checked && unchecked ) {
			m_allCheckBox->Set3StateValue( wxCHK_UNCHECKED );
		}
		else {
			m_allCheckBox->Set3StateValue( wxCHK_UNDETERMINED );
		}
	}
}

// set the description
void CheckGridListPanel::setDescription( const char* desc ) {
	m_desc = NVL( desc, "" );
}

// set the image file path
void CheckGridListPanel::setImageFilePath( const char* path ) {
	m_imageFilePath = NVL( path, "" );
}

// get the description
std::string CheckGridListPanel::getDescription( wxWindow* wnd ) {
	return m_desc;
}

// get the image file path
std::string CheckGridListPanel::getImageFilePath( wxWindow* wnd ) {
	return m_imageFilePath;
}
