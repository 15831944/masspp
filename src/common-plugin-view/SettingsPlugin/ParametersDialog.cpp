/**
 * @file ParametersDialog.cpp
 * @brief implements of ParametersDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ParametersDialog.h"

#include <wx/treebook.h>


#define GRID_GAP					 3
#define BORDER_SIZE				     3


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ParametersDialog, SettingsDialogBase )
END_EVENT_TABLE()


// constructor
ParametersDialog::ParametersDialog( wxWindow* parent )
		: SettingsDialogBase( parent, "Processing Options" ) {
}

// destructor
ParametersDialog::~ParametersDialog() {
}

// add panels
void ParametersDialog::addPanels() {
	// root chapter
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::SettingsChapter& rootChapter = plgMgr.getParametersRootChapter();
	rootChapter.sort();

	// add panels
	unsigned int count = addPanels( m_book, rootChapter, 0, 0 );

	// expand
	m_book->ExpandNode( 0 );
}

// add panels 
unsigned int ParametersDialog::addPanels(
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

		if( page->isCommon() ) {
			kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( book, page, (const char*)NULL );

			book->InsertSubPage( pos, panel, wxT( page->getTitle() ) );

			count++;
		}
	}

	return count;
}
