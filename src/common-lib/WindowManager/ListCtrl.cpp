/**
 * @file GridListCtrl.cpp
 * @brief implements of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2011.05.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ListCtrl.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ListCtrl, wxListCtrl )
	EVT_SIZE( ListCtrl::onSize )
END_EVENT_TABLE()


// constructor
ListCtrl::ListCtrl(
		wxWindow* parent,
		const int id,
		const int width,
		const int height,
		const int style,
		const bool autoColumnWidth
) : wxListCtrl(
		parent,
		id,
		wxDefaultPosition,
		wxSize( width, height ),
		style
) {
    m_autoColumnWidth = autoColumnWidth;
}

// destructor
ListCtrl::~ListCtrl() {
}

// set auto column width
void ListCtrl::setAutoColumnWidth( const bool autoColumnWidth ) {
	m_autoColumnWidth = autoColumnWidth;
}

// get auto column width
bool ListCtrl::isAutoColumnWidth() {
	return m_autoColumnWidth;
}

// set column widths 
void ListCtrl::setColumnWidths() {
	// client size
	wxSize size = GetClientSize();
	int width = size.GetWidth();

	// total width
	std::vector< int > cols;
	int total = 0;
	int num = GetColumnCount();
	for( int i = 0; i < num; i++ ) {
		int col = GetColumnWidth( i );
		if( col < 1 ) {
			col = 1;
		}

		cols.push_back( col );
		total += col;
	}

	// set width
	if( total < width ) {
		for( int i = 0; i < num; i++ ) {
			int col = cols[ i ];
			int newCol = col * width / total;
			total -= col;
			width -= newCol;

			SetColumnWidth( i, newCol );
		}
	}
}

// on size
void ListCtrl::onSize( wxSizeEvent& evt ) {
	if( m_autoColumnWidth ) {
		setColumnWidths();
	}
}
