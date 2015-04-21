/**
 * @file ParametersAndHelpDialog.cpp
 * @brief implements of ParametersAndHelpDialog class
 *
 * @author S.Tanaka
 * @date 2014.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporration All rights reserved.
 */


#include "stdafx.h"
#include "ParametersAndHelpDialog.h"

#include "HelpGetterPanel.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP						3


BEGIN_EVENT_TABLE( ParametersAndHelpDialog, StandardDialog )
	EVT_CHILD_FOCUS( ParametersAndHelpDialog::onFocus )
END_EVENT_TABLE()


// constructor
ParametersAndHelpDialog::ParametersAndHelpDialog(
	   wxWindow* parent,
	   const char* title,
	   const char* commitBtnName,
	   const int px,
	   const int py,
	   const int width,
	   const int height
) : StandardDialog( parent, title, commitBtnName, px, py, width, height ) {
	// style
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	m_help = NULL;

}

// destructor
ParametersAndHelpDialog::~ParametersAndHelpDialog() {
}

// set help
void ParametersAndHelpDialog::setDescription( wxWindow* wnd, const char* help ) {
	m_helpMap[ wnd ] = NVL( help, "" );
}

// set image file path
void ParametersAndHelpDialog::setImageFilePath( wxWindow* wnd, const char* path ) {
	m_imageMap[ wnd ] = NVL( path, "" );
}

// create top sizer
wxSizer* ParametersAndHelpDialog::createTopSizer() {
	// top sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );
	topSizer->AddGrowableRow( 0 );
	topSizer->AddGrowableRow( 1 );

	// main sizer
	wxSizer* fieldSizer = createMainSizer();
	topSizer->Add( fieldSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// help panel
	m_help = new HelpPanel( this );
	topSizer->Add( m_help, 1, wxGROW | wxALL, BORDER_SIZE );

	// button sizer
	wxSizer* buttonSizer = createButtonSizer();
	topSizer->Add( buttonSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return topSizer;
}


// child focus event
void ParametersAndHelpDialog::onFocus( wxChildFocusEvent& evt ) {
	// Window
	wxWindow* wnd = evt.GetWindow();
	ParametersAndHelpPanel* panel = dynamic_cast< ParametersAndHelpPanel* >( wnd );
	wxDialog* dlg = dynamic_cast< wxDialog* >( wnd );
	wxPanel* pnl = dynamic_cast< wxPanel* >( wnd );
	if( dlg != NULL || pnl != NULL ) {
		return;
	}

	// text & bitmap
	std::string text;
	std::string bitmap;

	if( m_helpMap.find( wnd ) != m_helpMap.end() ) {
		text = m_helpMap[ wnd ];
	}

	if( m_imageMap.find( wnd ) != m_imageMap.end() ) {
		bitmap = m_imageMap[ wnd ];
	}

	if( text.empty() && bitmap.empty() ) {
		return;
	}

	// help
	if( text.empty() && bitmap.empty() ) {
		bool flg = true;
		wxWindow* parent = wnd->GetParent();
		while( parent != NULL && parent != this && flg ) {
			HelpGetterPanel* help = dynamic_cast< HelpGetterPanel* >( parent );
			panel = dynamic_cast< ParametersAndHelpPanel* >( parent );
			dlg = dynamic_cast< wxDialog* >( parent );
			if( help != NULL ) {
				if( text.empty() ) {
					text = help->getDescription( wnd );
				}
				if( bitmap.empty() ) {
					bitmap = help->getImageFilePath( wnd );
				}
			}
			else if( dlg != NULL || panel != NULL ) {
				return;
			}
			parent = parent->GetParent();
		}
	}

	// set
	if( !text.empty() ) {
		m_help->setDescription( text.c_str() );
		m_help->setImageFilePath( bitmap.c_str() );
	}
}
