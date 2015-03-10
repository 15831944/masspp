/**
 * @file VariablesListCtrl.cpp
 * @brief implements of VariablesListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.10.18
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "VariablesListCtrl.h"
#include "ScriptDialog.h"


using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( VariablesListCtrl, kome::window::GridListCtrl )
END_EVENT_TABLE()


// constructor
VariablesListCtrl::VariablesListCtrl( wxWindow* parent, ScriptDialog* dlg ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 300, 150, false, true, true ) {
	m_dlg = dlg;
	m_script = NULL;
}

// destructor
VariablesListCtrl::~VariablesListCtrl() {
}

// set script
void VariablesListCtrl::setScript( kome::objects::Script* script ) {	// @Date:2013/08/06	<Modify>	A.Ozaki
	// set the member
	m_script = script;

	// update list
	clearData( false );

	if( m_script != NULL ) {
		for( unsigned int i = 0; i < m_script->getNumberOfVariables(); i++ ) {
			addData( i, false );
		}
	}

	// update
	updateList();
}

// get script
kome::objects::Script* VariablesListCtrl::getScript() {	// @Date:2013/08/06	<Modify>	A.Ozaki
	return m_script;
}

// on create
void VariablesListCtrl::onCreate() {
	// columns
	addColumn( "Name", kome::window::GridListCtrl::TYPE_STRING );
	addColumn( "Description", kome::window::GridListCtrl::TYPE_STRING );
}

// get string
std::string VariablesListCtrl::getString( const long long data, const int column ) const {
	std::string ret;

	if( m_script != NULL ) {
		if( data >= 0 && data < (int)m_script->getNumberOfVariables() ) {
			// variable information
			if( column == 0 ) {    // name
				ret = m_script->getVariableName( data );
			}
			else if( column == 1 ) {    // description
				ret = m_script->getVariableDescription( data );
			}
		}
	}

	return ret;
}

// on activate
void VariablesListCtrl::onActivateData( const long long data ) {
	// get variable name
	if( data >= 0 && data < (int)m_script->getNumberOfVariables() && m_dlg != NULL ) {
		const char* name = m_script->getVariableName( data );
		m_dlg->insertText( name );
	}
}
