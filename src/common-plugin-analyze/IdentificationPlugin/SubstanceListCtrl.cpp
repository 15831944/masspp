/**
 * @file SubstanceListCtrl.cpp
 * @brief implements of SubstanceListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SubstanceListCtrl.h"
#include "SubstanceDialog.h"	// @date 2013/01/22 <Add> OKADA

#define ID_DELETE		13351

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
kome::ident::SubstanceDialog* SubstanceListCtrl::m_dlg = NULL;			// @date 2013/01/23 <Add> OKADA


BEGIN_EVENT_TABLE( SubstanceListCtrl, kome::window::GridListCtrl )
END_EVENT_TABLE()


// constructor
SubstanceListCtrl::SubstanceListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 600, 300, false, true, true ) {
	setPopupMenuFlag( POPUP_COPY | POPUP_DELETE | POPUP_FILTER | POPUP_SAVE );

	setActivateName( "Open" );
}

// destructor
SubstanceListCtrl::~SubstanceListCtrl() {
}

// void clear values
void SubstanceListCtrl::clearValues() {
	m_values.clear();
}

// add value
void SubstanceListCtrl::addValue( const char* key, const char* value ) {
	m_values.push_back( std::make_pair( NVL( key, "" ), NVL( value, "" ) ) );
}

// on create
void SubstanceListCtrl::onCreate() {
	// rows
	for( unsigned int i = 0; i < m_values.size(); i++ ) {
		addData( i, false );
	}

	// columns
	addColumn( "Key",   kome::window::GridListCtrl::TYPE_STRING, 120 );
	addColumn( "Value", kome::window::GridListCtrl::TYPE_STRING, 140 );
}

// get string
std::string SubstanceListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	if( column == 0 ) {
		s = m_values[ data ].first;
	}
	else {
		s = m_values[ data ].second;
	}
            	
	return s;
}

// on activate data
void SubstanceListCtrl::onActivateDataOther( const long long data ) {
	// search
	IdentificationManager& mgr = IdentificationManager::getInstance();
	
	// 選択中のItem取得
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	if( items.size() == 0 ) {
		return;
	}
	long long selItem = m_data[ items[ 0 ] ];	// search ID取得

	SearchInfo searchInfo;
	mgr.getSearch( selItem, searchInfo );	// search IDからSearch情報を取得		// selItem＝search ID

	// search engine
	std::string engineName = mgr.getSearchEngineName( searchInfo.engineId );
	LOG_DEBUG( FMT("search engine = %s", engineName.c_str()));
	SearchEngine* engine = NULL;
	for( int i = 0; i < mgr.getNumberOfSearchEngines() && engine == NULL; i++ ) {
		SearchEngine* tmp = mgr.getSearchEngine( i );
		if( engineName.compare( tmp->getName() ) == 0 ) {
			engine = tmp;
		}
	}

	// open the result
	if( engine != NULL ) {
		if (!engine->openResult( searchInfo.result.c_str() ))	// ブラウザを呼び出して結果表示
		{
			// After calling getlasterrormessage once, the error is deleted.
			// We want to use it twice, so make a copy first.
			std::string lasterrormessage = getlasterrormessage();
			if (lasterrormessage.size() > 0)
			{
				// Display an error dialog if failed to display search results.
				std::string strError = "An error occurred while trying to display search results:\n";
				strError += lasterrormessage;
				kome::window::WindowTool::showError( strError.c_str() );
			}
			else
			{
				LOG_ERROR( FMT("An error occurred while trying to display search results.") );
			}
		}
	}
}
