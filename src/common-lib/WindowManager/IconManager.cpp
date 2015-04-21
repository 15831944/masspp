/**
 * @file IconManager.cpp
 * @brief implements of IconManager class
 *
 * @author S.Tanaka
 * @date 2006.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "IconManager.h"

#include <wx/wx.h>
#include <wx/imaglist.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IconManager::IconManager() {
}

// destructor
IconManager::~IconManager() {
	// delete icon
	for( unsigned int i = 0; i < m_icons.size(); i++ ) {
		delete m_icons[ i ];
	}
}

// get icon
int IconManager::getIconIndex( const char* name ) {
	// create icon map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	// get icon
	std::string s = NVL( name, "" );
	if( m_iconMap.find( s ) == m_iconMap.end() ) {
		return -1;
	}

	return m_iconMap[ s ];
}

// get number of icons
unsigned int IconManager::getNumberOfIcons() {
	// create icon map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	return m_icons.size();
}

// get icon object
wxIcon* IconManager::getIcon( const int index ) {
	// create icon map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	// check index
	if( index < 0 || index >= (int)m_icons.size() ) {
		return NULL;
	}

	return m_icons[ index ];
}

// get icon object
wxIcon* IconManager::getIcon( const char* name ) {
	return getIcon( getIconIndex( name ) );
}

// get icon object
wxIcon* IconManager::getIcon( kome::plugin::PluginIcon* icon ) {
	// create icon map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	// index
	int index = -1;
	std::map< kome::plugin::PluginIcon*, int >::iterator it = m_indexMap.find( icon );
	if( it != m_indexMap.end() ) {
		index = (*it).second;
	}

	// get icon object
	return getIcon( index );
}

// get image list
wxImageList* IconManager::createImageList( const unsigned int width, const unsigned int height ) {
	// create icon map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	// image list object
	wxImageList* imgList = new wxImageList( width, height );

	// add icon
	for( unsigned int i = 0; i < m_icons.size(); i++ ) {
		imgList->Add( *m_icons[ i ] );
	}

	return imgList;
}

// create icon map
void IconManager::createIconMap() {
	// plug-in manager
	kome::plugin::PluginManager& plgMgr
		= kome::plugin::PluginManager::getInstance();

	// get icon info
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		// plug-in
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );

		for( unsigned int j = 0; j < plugin->getNumberOfIcons(); j++ ) {
			// icon information
			kome::plugin::PluginIcon* iconInfo = plugin->getIcon( j );

			// create icon
			wxIcon* icon = new wxIcon();
			loadBitmap( icon, iconInfo );

			// get index
			int index = m_icons.size();

			// add to array
			m_icons.push_back( icon );

			// set to map
			m_iconMap[ iconInfo->getName() ] = index;
			m_iconMap[ FMT( "%s@%s", iconInfo->getName(), plugin->getName() ) ] = index;
			m_indexMap[ iconInfo ] = index;
		}
	}
}

// load bitmap data
void IconManager::loadBitmap( wxIcon* icon, kome::plugin::PluginIcon* iconInfo ) {
	// check parameters 
	if( icon == NULL || iconInfo == NULL ) { 
		return;
	}

	// get bitmap
	wxBitmap bitmap( wxT( iconInfo->getPath() ), wxBITMAP_TYPE_PNG );

	// load
	icon->CopyFromBitmap( bitmap );
}

// get object
IconManager& IconManager::getInstance() {
	// create object (This is the only object.)
	static IconManager mgr;

	return mgr;
}
