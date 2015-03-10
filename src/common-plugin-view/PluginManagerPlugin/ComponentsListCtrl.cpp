/**
 * @file ComponentsListCtrl.cpp
 * @brief implementation of ComponentsListCtrl class
 *
 * @author okada_h
 * @date 2013.08.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ComponentsListCtrl.h"
#include "PluginManagerPluginManager.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ComponentsListCtrl, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
ComponentsListCtrl::ComponentsListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::CheckGridListCtrl( parent, wxID_ANY, width, height, false ) {
	m_callName = 0;			// @date 2013/08/22 <Add> OKADA
	m_descriptionCol = 1;	// @date 2013/08/22 <Mod> OKADA

	m_appendedCol = 0;
}

// destructor
ComponentsListCtrl::~ComponentsListCtrl() {
}


// get the number of functions
unsigned int ComponentsListCtrl::getNumberOfFunctions() {
	return m_vecComponents.size();
}

// get Function
COMPONENTS ComponentsListCtrl::getComponent( const unsigned int idx ) {
	return m_vecComponents[ idx ];
}

// clear spectra
void ComponentsListCtrl::clearComponents() {
	if( m_vecComponents.empty() ){
		;	// 空っぽなら何もしない
	}else{
		m_vecComponents.clear();
	}
	clearData( false );
}

// add plugins
void ComponentsListCtrl::addComponents( COMPONENTS function ) {
	// >>>>>>	@Date:2013/09/08	<Add>	A.Ozaki
	// 「リスト下部の選択数が合わない」対策
	// false→true
	// trueで項目を追加しないと、選択数が合いません
	//
	addData( m_vecComponents.size(), true );
	//
	// <<<<<<	@Dtae:2013/09/08	<Add>	A.Ozaki

	this->setSelected( m_vecComponents.size(), function.flgActive );	// @date 2013/02/26 <Add> OKADA
	m_vecComponents.push_back( function );
}


// on create
void ComponentsListCtrl::onCreate() {
	// header

	/** plugin name */
	m_callName = -1;			// @date 2013/08/22 <Add> OKADA

	/** path column */
	m_descriptionCol = -1;


	m_appendedCol = 0;

	addColumn( "Name", TYPE_STRING );	// @date 2013/08/22 <Add> OKADA
	m_callName = m_appendedCol;			// @date 2013/08/22 <Add> OKADA
	m_appendedCol += 1;					// @date 2013/08/22 <Add> OKADA

	addColumn( "Description", TYPE_STRING );
	m_descriptionCol = m_appendedCol;
	m_appendedCol += 1;
	
	// add data
	for( unsigned int i = 0; i < m_vecComponents.size(); i++ ) {
		addData( i, false );
	}
}

// get double value
double ComponentsListCtrl::getDouble( const long long data, const int column ) const {

	// get value
	double val = double();

	return val;
}

// get int value
int ComponentsListCtrl::getInt( const long long data, const int column ) const {

	// get value
	int val = int();

	return val;
}

// get string value
std::string ComponentsListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// >>>>>>	@Date:2013/09/08	<Add>	A.Ozaki
	// 「リスト下部の選択数が合わない」対策
	// 空の時には、空白を返すようにします
	//
	if  ( 0 >= m_vecComponents.size( ) )
	{
		s.clear( );
		return	s;
	}
	//
	// <<<<<<	@Date:2013/09/08	<Add>	A.Ozaki

	if( column == m_callName ) {	// Name
		std::string strCall = m_vecComponents[data].strName;

		s = strCall;
	}
	if( column == m_descriptionCol ) {	// Description
		std::string strDescription = m_vecComponents[data].strDescription;

		s = strDescription;
	}

	return s;
}

// on select item
void ComponentsListCtrl::onSelectItem( const int itemRaw ){

	const int item = this->m_data[itemRaw];

	bool flgSelected = this->isSelected( item );	// flgSelectedには選択後の値が入る

	long long llVal = this->getListData( itemRaw );	
	int nVal = llVal;

	if( nVal < 0 ){
		return;
	}

	if( (size_t)nVal < m_vecComponents.size() ){
		COMPONENTS component = m_vecComponents[nVal];

		PluginManagerView& mgr = PluginManagerView::getInstance();

		mgr.updateActiveTableCompo( component.nPluginIndex, component.uCompornentIndex, flgSelected );

		// チェックボックスが変更されたので、パッケージをCustomizedに変更
		mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );

	}
	
	return;
}

// on select all
void ComponentsListCtrl::onSelectAll(){
	unsigned int nSize = m_vecComponents.size();

	for( unsigned int i=0; i<nSize; i++ ){
		m_vecComponents[i].flgActive = true;

		// 個々のチェックボックスがクリックされたときと同様に、m_vectActiveTableを更新
		COMPONENTS component = m_vecComponents[i];
		PluginManagerView& mgr = PluginManagerView::getInstance();
		mgr.updateActiveTableCompo( component.nPluginIndex, component.uCompornentIndex, true );
	}

	// チェックボックスが変更されたので、パッケージをCustomizedに変更
	PluginManagerView& mgr = PluginManagerView::getInstance();
	mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );
}

// on deselect all
void ComponentsListCtrl::onDeselectAll(){
	unsigned int nSize = m_vecComponents.size();

	for( unsigned int i=0; i<nSize; i++ ){
		m_vecComponents[i].flgActive = false;

		// 個々のチェックボックスがクリックされたときと同様に、m_vectActiveTableを更新
		COMPONENTS component = m_vecComponents[i];
		PluginManagerView& mgr = PluginManagerView::getInstance();
		mgr.updateActiveTableCompo( component.nPluginIndex, component.uCompornentIndex, false );
	}
	// チェックボックスが変更されたので、パッケージをCustomizedに変更
	PluginManagerView& mgr = PluginManagerView::getInstance();
	mgr.selectPackage( STR_PACKAGE_CUSTOMIZED );
}

// on size
void ComponentsListCtrl::onSizePluginMgr( wxSizeEvent& evt ) {
	onSize( evt );
}
