/**
 * @file SettingsDialogPopup.cpp
 * @brief implements of SettingsDialogPopup class
 *
 * @author OKADA,H
 * @date 2012/11/20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsDialogPopup.h"

#include <wx/treebook.h>

#define GRID_GAP					  3
#define BORDER_SIZE					  3


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SettingsDialogPopup, SettingsDialogBase )
END_EVENT_TABLE()


// constructor
SettingsDialogPopup::SettingsDialogPopup( wxWindow* parent )
		: SettingsDialogBase( parent, "Options" ) {
	// Context Menu
	m_PropertiesPageType = PROP_NONE;			// Constructor
}

// destructor
SettingsDialogPopup::~SettingsDialogPopup() {
}

// add panels
void SettingsDialogPopup::addPanels() {
	// root chapter
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::SettingsChapter& rootChapter = plgMgr.getSettingsRootChapter();
	rootChapter.sort();

	checkPanels( rootChapter );

	// add panels
	unsigned int count = addPanels( m_book, rootChapter, 0, 0 );

	// Context Menu
	m_PropertiesPageType = PROP_NONE;			// addPanels()の出口で消す

	// expand
	m_book->ExpandNode( 0 );
}

// add panels 
unsigned int SettingsDialogPopup::addPanels(
		wxTreebook* book,
		kome::plugin::SettingsChapter& chapter,
		unsigned int pos,
		unsigned int count
) {
	// check the chapter
	if( !chapter.isCommon() ) {
		return count;
	}

	// add chapter
	if( chapter.getParentChapter() == NULL ) {		// top level

		if( chapter.getFlgContextDisplay() ){
			book->AddPage( NULL, wxT( chapter.getName() ) );
			pos = count;
			count++;
		}
	}
	else {
		if( chapter.getFlgContextDisplay() ){
			book->InsertSubPage( pos, NULL, wxT( chapter.getName() ) );
			pos = count;
			count++;
		}
	}

	// child chapter
	for( unsigned int i = 0; i < chapter.getNumberOfChildren(); i++ ) {
		count = addPanels( book, *( chapter.getChildChapter( i ) ), pos, count );
	}

	// panels
	for( unsigned int i = 0; i < chapter.getNumberOfPages(); i++ ) {
		// panel
		kome::plugin::SettingsPage* page = chapter.getPage( i );

		int flgInsert = false;

		// Context Menu
		PropertiesPageType pt = page->getPropertiesPageType();
		if( m_PropertiesPageType == PROP_NONE ){	// 事前にProperties Page Typeが設定されていない
			flgInsert = true;	// 処理続行
		}else if( pt == m_PropertiesPageType ){	// 事前に設定したProperties Page Type（m_PropertiesPageType）と読み込まれたProperties Page Type（pt）が一致
			flgInsert = true;	// 処理続行
		}

		if( page->isCommon() ) {
			if( flgInsert ){
				kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( book, page, (const char*)NULL );

				book->InsertSubPage( pos, panel, wxT( page->getTitle() ) );
				count++;
			}
		}
	}

	return count;
}

void SettingsDialogPopup::setPropertiesPageType( PropertiesPageType pt ){
	m_PropertiesPageType = pt;
}

// add panels 
unsigned int SettingsDialogPopup::checkPanels(
		kome::plugin::SettingsChapter& chapter
) {

	int nRet = 0;

	// check the chapter
	if( !chapter.isCommon() ) {
		return nRet;
	}
	
	// child chapter
	chapter.setFlgContextDisplay( false );	// 事前にクリア
	for( unsigned int i = 0; i < chapter.getNumberOfChildren(); i++ ) {
		int nRetValue = checkPanels( *( chapter.getChildChapter( i ) ) );
		if( nRetValue == 0 ){
			;
		}else{
			chapter.setFlgContextDisplay( 1 );
			nRet = 1;
		}
	}

	// panels
	for( unsigned int i = 0; i < chapter.getNumberOfPages(); i++ ) {
		// panel
		kome::plugin::SettingsPage* page = chapter.getPage( i );

		bool flgInsert = false;

		// Context Menu
		PropertiesPageType pt = page->getPropertiesPageType();
		if( m_PropertiesPageType == PROP_NONE ){	// 事前にProperties Page Typeが設定されていない
			flgInsert = true;	// 処理続行
		}else if( pt == m_PropertiesPageType ){	// 事前に設定したProperties Page Type（m_PropertiesPageType）と読み込まれたProperties Page Type（pt）が一致
			flgInsert = true;	// 処理続行
		}

		if( page->isCommon() ) {
			if( flgInsert ){
				chapter.setFlgContextDisplay( flgInsert );	// 表示項目があるならon
				nRet = 1;
			}
		}
	}

	return nRet;
}
