/**
 * @file EMailTextCtrl.cpp
 * @brief implements of GridEmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.09.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "EMailTextCtrl.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( EMailTextCtrl, wxTextCtrl )
	EVT_CHAR( onChar )
END_EVENT_TABLE()

// constructor
EMailTextCtrl::EMailTextCtrl(wxWindow* parent, const int id)
    : wxTextCtrl( parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize ){
}

// destructor
EMailTextCtrl::~EMailTextCtrl() {
}

// on char
void EMailTextCtrl::onChar( wxKeyEvent& evt ) {
	// check the value
	bool ok = false;

	char c = (char)evt.GetKeyCode();
	int ic = evt.GetKeyCode();
	
	if( c == '0' || c == '1' || c == '2' || c == '3' || c == '4'
		    || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'
			|| ic == WXK_BACK || ic == WXK_DELETE
			|| ic == WXK_LEFT || ic == WXK_RIGHT || ic == WXK_TAB ) {
        ok = true;
	}
	else {
		std::string s = GetValue().c_str();
		// get insert point 
		int ipos = GetInsertionPoint()-1;
		if( ipos == -1 ){
			ipos = 0;
		}
		
		// @が1つ以上、.が連続して入力される時のみ入力不可
		// その他のチェックはとりあえずなし必要であれば追加していく。 @date 2011.09.21 <Add> M.Izumi
		if( s.find( "@" ) == -1 && c == '@' ){
			ok = true;
		}else if( c == '@' || (c == '.' && s[ipos] == '.') ){
	
		}else{
			ok = true;
		}
	}
	
	// default
	if( ok ) {
		OnChar( evt );
	}
}
