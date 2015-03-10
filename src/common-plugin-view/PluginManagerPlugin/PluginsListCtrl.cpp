/**
 * @file PluginsListCtrl.cpp
 * @brief implementation of PluginsListCtrl class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PluginsListCtrl.h"
#include "PluginManagerPluginManager.h"
#include "PluginManagerDlg.h"


using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PluginsListCtrl, kome::window::GridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, kome::pluginmanager::view::PluginsListCtrl::OnSelect )
END_EVENT_TABLE()


// constructor
PluginsListCtrl::PluginsListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::GridListCtrl( parent, wxID_ANY, width, height, false, true, true ) {	// 最後のtrueで１つだけ選択できる設定にしています
	m_pluginCol = 0;
	m_providerCol = 1;
	m_pathCol = 2;

	m_appendedCol = 0;
	m_nSelectedRawNo = -1;

	m_parent = parent;

	// auto column width
	this->setAutoColumnWidth( true );	// @date 2013/05/10 <Add> OKADA

	// @date 2013/05/13 <Add> OKADA
	wxSize wxsizeWindow = m_parent->GetSize();
	int ddd = 0;

}

// destructor
PluginsListCtrl::~PluginsListCtrl() {
}

// get the number of spectra
unsigned int PluginsListCtrl::getNumberOfPlugins() {
	return m_vecPlugins.size();
}

// get spectrum
PLUGIN PluginsListCtrl::getPlugin( const unsigned int idx ) {
	return m_vecPlugins[ idx ];
}

// clear spectra
void PluginsListCtrl::clearPlugins() {
	m_vecPlugins.clear();
	clearData( false );
}

// add plugins
void PluginsListCtrl::addPlugins( PLUGIN plugin ) {
	addData( m_vecPlugins.size(), false );
	m_vecPlugins.push_back( plugin );
}

// on create
void PluginsListCtrl::onCreate() {
	// header

	/** plugin column */
	m_pluginCol = -1;

	/** provider column */
	m_providerCol = -1;

	/** path column */
	m_pathCol = -1;

	m_appendedCol = 0;

	addColumn( "Plugin", TYPE_STRING );
	m_pluginCol = m_appendedCol;
	m_appendedCol += 1;
	
	addColumn( "Provider", TYPE_STRING );
	m_providerCol = m_appendedCol;
	m_appendedCol += 1;
	
	addColumn( "Path", TYPE_STRING );
	m_pathCol = m_appendedCol;
	m_appendedCol += 1;
	
	// add data
	for( unsigned int i = 0; i < m_vecPlugins.size(); i++ ) {
		addData( i, false );
	}

}

// get double value
double PluginsListCtrl::getDouble( const long long data, const int column ) const {

	// get value
	double val = double();

	return val;
}

// get int value
int PluginsListCtrl::getInt( const long long data, const int column ) const {

	// get value
	int val = int();

	return val;
}

// get string value
std::string PluginsListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	if( column == m_pluginCol ) {	// Plugin
		std::string strPlugin = m_vecPlugins[data].strName;

		s = strPlugin;
	}
	if( column == m_providerCol ) {	// Provider
		std::string strProvider = m_vecPlugins[data].strProvider;

		s = strProvider;
	}
	if( column == m_pathCol ) {	// Path
		std::string strPath = m_vecPlugins[data].strPath;

		s = strPath;
	}

	return s;
}

// Plug-ins リスト選択時の処理
void PluginsListCtrl::OnSelect( wxListEvent& evt ){

	OnSelectPlugin();

}

// get selected raw no
int PluginsListCtrl::getSelectedRawNo(){

	return m_nSelectedRawNo;
}

// on select plugin
void PluginsListCtrl::OnSelectPlugin(){

	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );

	int nSelectedRowNo = items[0];					// @date 2013/05/28 <Add> OKADA
	int nSelectedNo = this->m_data[nSelectedRowNo];	// @date 2013/05/28 <Add> OKADA
	
	// @date 2013/06/10 <Add> OKADA ------->
	if( nSelectedNo < 0 ){
		return;
	}
	// @date 2013/06/10 <Add> OKADA <-------

	if( (size_t)nSelectedNo < m_vecPlugins.size() ){	// @date 2013/06/10 <Mod> OKADA
 
		m_nSelectedRawNo = nSelectedNo;

		PLUGIN plugin = m_vecPlugins[nSelectedNo];

		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		int nNumPlugins = plgMgr.getNumberOfPlugins();
		kome::plugin::PluginInfo* ptrPluginInfo = plgMgr.getPlugin( plugin.nCnt );

		PluginManagerView& mgr = PluginManagerView::getInstance();
		PluginManagerDlg* ptrDlg = mgr.getDlg();
		if( ptrDlg != NULL ){
			std::string strSelectedPackage = ptrDlg->getSelectedPackage();

			unsigned int uCalls = ptrPluginInfo->getNumberOfCalls();
			for( unsigned int uIndexCall=0; uIndexCall<uCalls; uIndexCall++ ){

				kome::plugin::PluginCall* ptrPluginCall = ptrPluginInfo->getCall( uIndexCall );
				bool flgHas = ptrPluginCall->hasPackage( strSelectedPackage.c_str() );
				
			}
	
			ptrDlg->onSelectPlugin();

		}
	}

	return;
}

// on size
void PluginsListCtrl::onSizePluginMgr( wxSizeEvent& evt ) {	// @date 2013/05/13 <Add> OKADA
	onSize( evt );

}
