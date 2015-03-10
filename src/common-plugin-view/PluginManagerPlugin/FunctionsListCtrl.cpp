/**
 * @file FunctionsListCtrl.cpp
 * @brief implementation of FunctionsListCtrl class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "FunctionsListCtrl.h"
#include "PluginManagerPluginManager.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( FunctionsListCtrl, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
FunctionsListCtrl::FunctionsListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::CheckGridListCtrl( parent, wxID_ANY, width, height, false ) {
	m_callCol = 0;
	m_typeCol = 1;
	m_descriptionCol = 2;

	m_appendedCol = 0;
}

// destructor
FunctionsListCtrl::~FunctionsListCtrl() {
}


// get the number of functions
unsigned int FunctionsListCtrl::getNumberOfFunctions() {
	return m_vecFunctions.size();
}

// get Function
FUNCTIONS FunctionsListCtrl::getFunction( const unsigned int idx ) {
	return m_vecFunctions[ idx ];
}

// clear spectra
void FunctionsListCtrl::clearFunctions() {
	if( m_vecFunctions.empty() ){
		;	// 空っぽなら何もしない
	}else{
		m_vecFunctions.clear();
	}
	clearData( false );
}

// add plugins
void FunctionsListCtrl::addFunctions( FUNCTIONS function ) {
	addData( m_vecFunctions.size(), false );
	this->setSelected( m_vecFunctions.size(), function.flgActive );	// @date 2013/02/26 <Add> OKADA
	m_vecFunctions.push_back( function );
}


// on create
void FunctionsListCtrl::onCreate() {
	// header

	/** plugin column */
	m_callCol = -1;

	/** provider column */
	m_typeCol = -1;

	/** path column */
	m_descriptionCol = -1;


	m_appendedCol = 0;

	addColumn( "Call", TYPE_STRING );
	m_callCol = m_appendedCol;
	m_appendedCol += 1;
	
	addColumn( "Type", TYPE_STRING );
	m_typeCol = m_appendedCol;
	m_appendedCol += 1;
	
	addColumn( "Description", TYPE_STRING );
	m_descriptionCol = m_appendedCol;
	m_appendedCol += 1;
	
	// add data
	for( unsigned int i = 0; i < m_vecFunctions.size(); i++ ) {
		addData( i, false );
	}
}

// get double value
double FunctionsListCtrl::getDouble( const long long data, const int column ) const {

	// get value
	double val = double();

	return val;
}

// get int value
int FunctionsListCtrl::getInt( const long long data, const int column ) const {

	// get value
	int val = int();

	return val;
}

// get string value
std::string FunctionsListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	if( column == m_callCol ) {	// Call
		std::string strCall = m_vecFunctions[data].strName;

		s = strCall;
	}
	if( column == m_typeCol ) {	// Type
		std::string strType = m_vecFunctions[data].strType;

		s = strType;
	}
	if( column == m_descriptionCol ) {	// Description
		std::string strDescription = m_vecFunctions[data].strDescription;

		s = strDescription;
	}

	return s;
}


void FunctionsListCtrl::onSelectItem( const int itemRaw ){

	const int item = this->m_data[itemRaw];	// @date 2013/05/28 <Add> OKADA

	bool flgSelected = this->isSelected( item );	// flgSelectedには選択後の値が入る

	long long llVal = this->getListData( itemRaw );	// @date 2013/05/28 <Mod> OKADA
	int nVal = llVal;

	// @date 2013/06/10 <Add> OKADA ------->
	if( nVal < 0 ){
		return;
	}
	// @date 2013/06/10 <Add> OKADA <-------

	if( (size_t)nVal < m_vecFunctions.size() ){	// @date 2013/06/10 <Mod> OKADA
		FUNCTIONS function = m_vecFunctions[nVal];

		PluginManagerView& mgr = PluginManagerView::getInstance();

		mgr.updateActiveTable( function.nPluginIndex, function.uFunctionIndex, flgSelected );

		// チェックボックスが変更されたので、パッケージをCustomizedに変更
		mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );

	}
	
	return;
}

// on select all
void FunctionsListCtrl::onSelectAll(){
	unsigned int nSize = m_vecFunctions.size();

	for( unsigned int i=0; i<nSize; i++ ){
		m_vecFunctions[i].flgActive = true;

		// @date 2013/04/19 <Add> OKADA ------->
		// 個々のチェックボックスがクリックされたときと同様に、m_vectActiveTableを更新
		FUNCTIONS function = m_vecFunctions[i];
		PluginManagerView& mgr = PluginManagerView::getInstance();
		mgr.updateActiveTable( function.nPluginIndex, function.uFunctionIndex, true );
		// @date 2013/04/19 <Add> OKADA <-------

	}

	// チェックボックスが変更されたので、パッケージをCustomizedに変更
	PluginManagerView& mgr = PluginManagerView::getInstance();
	mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );

}

// on deselect all
void FunctionsListCtrl::onDeselectAll(){
	unsigned int nSize = m_vecFunctions.size();

	for( unsigned int i=0; i<nSize; i++ ){
		m_vecFunctions[i].flgActive = false;

		// @date 2013/04/19 <Add> OKADA ------->
		// 個々のチェックボックスがクリックされたときと同様に、m_vectActiveTableを更新
		FUNCTIONS function = m_vecFunctions[i];
		PluginManagerView& mgr = PluginManagerView::getInstance();
		mgr.updateActiveTable( function.nPluginIndex, function.uFunctionIndex, false );
		// @date 2013/04/19 <Add> OKADA <-------

	}
	// チェックボックスが変更されたので、パッケージをCustomizedに変更
	PluginManagerView& mgr = PluginManagerView::getInstance();
	mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );
}

// on size
void FunctionsListCtrl::onSizePluginMgr( wxSizeEvent& evt ) {	// @date 2013/05/13 <Add> OKADA
	onSize( evt );
}
