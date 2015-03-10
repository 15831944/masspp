/**
 * @file SettingsDialog.cpp
 * @brief implements of SettingsDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsDialog.h"

#include <wx/treebook.h>


#define GRID_GAP				3
#define BORDER_SIZE				3


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SettingsDialog, SettingsDialogBase )
END_EVENT_TABLE()


// constructor
SettingsDialog::SettingsDialog( wxWindow* parent )
		: SettingsDialogBase( parent, "Options" ) {
}

// destructor
SettingsDialog::~SettingsDialog() {
}

// add panels
void SettingsDialog::addPanels() {
	// root chapter
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::SettingsChapter& rootChapter = plgMgr.getSettingsRootChapter();
	rootChapter.sort();

	// add panels
	unsigned int count = addPanels( m_book, rootChapter, 0, 0 );

	// expand
	m_book->ExpandNode( 0 );
}

// add panels 
unsigned int SettingsDialog::addPanels(
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
		book->AddPage( NULL, wxT( chapter.getName() ) );
	}
	else {
		book->InsertSubPage( pos, NULL, wxT( chapter.getName() ) );
	}
	pos = count;
	count++;

	// child chapter
	for( unsigned int i = 0; i < chapter.getNumberOfChildren(); i++ ) {
		count = addPanels( book, *( chapter.getChildChapter( i ) ), pos, count );
	}

	// panels
	for( unsigned int i = 0; i < chapter.getNumberOfPages(); i++ ) {
		// panel
		kome::plugin::SettingsPage* page = chapter.getPage( i );

		
		PropertiesPageType pt = page->getPropertiesPageType();	// @date 2012/12/03 <Add> OKADA
		if( pt == PROP_NONE ){									// @date 2012/12/03 <Add> OKADA
			if( page->isCommon() ) {
				kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( book, page, (const char*)NULL );

				book->InsertSubPage( pos, panel, wxT( page->getTitle() ) );

				count++;
			}
		}else{													// @date 2012/12/03 <Add> OKADA
			;	// Context MenuÇ≈égÇÌÇÍÇÈÇ‡ÇÃÇÕèúäO
		}														// @date 2012/12/03 <Add> OKADA
	}

	return count;
}
