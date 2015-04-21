/**
 * @file GridNumberTextCtrl.cpp
 * @brief implements of GridNumberTextCtrl class
 *
 * @author S.Tanaka
 * @date 2011.08.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NumberTextCtrl.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( NumberTextCtrl, wxTextCtrl )
	EVT_CHAR( onChar )
	EVT_CONTEXT_MENU( NumberTextCtrl::onContextMenu )
END_EVENT_TABLE()


// constructor
NumberTextCtrl::NumberTextCtrl(
    	wxWindow* parent,
    	const int id,
    	const bool integer,
    	const bool neg )
    : wxTextCtrl(
            parent,
			id,
			wxEmptyString,
			wxDefaultPosition,
			wxSize( ( integer ? 100 : 125 ), -1 )
) {
    // initialize
    m_integer = integer;
    m_neg = neg;

	// length
	int len = ( integer ? 9 : 15 );
	if( neg ) {
		len += 1;
	}
	SetMaxLength( len );
}

// destructor
NumberTextCtrl::~NumberTextCtrl() {
}

// on char
void NumberTextCtrl::onChar( wxKeyEvent& evt ) {
	// check the value
	bool ok = false;

	char c = (char)evt.GetKeyCode();
	int ic = evt.GetKeyCode();

	if( c == '0' || c == '1' || c == '2' || c == '3' || c == '4'
		    || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'
			|| ic == WXK_BACK || ic == WXK_DELETE
			|| ic == WXK_LEFT || ic == WXK_RIGHT || ic == WXK_TAB ){
        ok = true;
	}
	else {
		std::string s = GetValue().c_str();

		if( c == '-' ) {
			if( m_neg && s.find( "-" ) == s.npos && GetInsertionPoint() == 0 ) {
				ok = true;
			}
		}
		else if( c == '.' ) {
			if( !m_integer && s.find( "." ) == s.npos ) {
				ok = true;
			}
		}
		// Add shortcut key operation @date 2013.04.17 <Add> M.Izumi
		else if( ic == WXK_CONTROL || ic == 22 || ic == 1 || ic == 3 || ic == 24 ){ // [ctrl+a], [ctrl+c], [ctrl+v], [ctrl+x]
			ok = true;
		}
	}

	// default
	if( ok ) {
		OnChar( evt );
	}
}
// context menu
void NumberTextCtrl::onContextMenu( wxContextMenuEvent& evt ) {

}

