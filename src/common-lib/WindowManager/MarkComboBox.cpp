/**
 * @file MarkComboBox.cpp
 * @brief implements of MarkComboBox class
 *
 * @author S.Tanaka
 * @date 2009.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MarkComboBox.h"

#include "DCGraphics.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( MarkComboBox, wxBitmapComboBox )
END_EVENT_TABLE()


// mark combo
MarkComboBox::MarkComboBox( wxWindow* parent, const int id, const bool addNone )
		: wxBitmapComboBox(
			parent,
			id,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			0,
			NULL,
			wxCB_READONLY
		) {
	// create array
	if( addNone ) {
		m_marks.push_back( kome::img::Graphics::MARK_NONE );
	}
	m_marks.push_back( kome::img::Graphics::MARK_CIRCLE );
	m_marks.push_back( kome::img::Graphics::MARK_DELTA );
	m_marks.push_back( kome::img::Graphics::MARK_NABLA );
	m_marks.push_back( kome::img::Graphics::MARK_SQUARE );
	m_marks.push_back( kome::img::Graphics::MARK_DIAMOND );
	m_marks.push_back( kome::img::Graphics::MARK_STAR );
	m_marks.push_back( kome::img::Graphics::MARK_PLUS );
	m_marks.push_back( kome::img::Graphics::MARK_TIMES );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_CIRCLE );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_DELTA );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_NABLA );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_SQUARE );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_DIAMOND );
	m_marks.push_back( kome::img::Graphics::MARK_FILLED_STAR );

	// creeate combo
	create();
}

// destructor
MarkComboBox::~MarkComboBox() {
}

// get selected mark
kome::img::Graphics::Mark MarkComboBox::getSelectedMark() {
	// get selection
	int sel = GetSelection();
	if( sel < 0 || sel >= (int)m_marks.size() ) {
		return kome::img::Graphics::MARK_NONE;
	}

	return m_marks[ sel ];
}

// search
void MarkComboBox::selectMark( const kome::img::Graphics::Mark mark ) {
	// search 
	int sel = -1;
	for( unsigned int i = 0; i < m_marks.size() && sel < 0; i++ ) {
		if( m_marks[ i ] == mark ) {
			sel = (int)i;
		}
	}

	// select
	if( sel >= 0 ) {
		Select( sel );
	}
}

// create combo box
void MarkComboBox::create() {
	// DC
	const unsigned int iconSize = 5;

	wxMemoryDC memDc;
	wxBitmap bitmap( 16, 16 );

	memDc.SetBackground( *wxWHITE_BRUSH );

	DCGraphics g( &memDc );
	g.setSize( 16, 16 );
	g.setGraphMargin( 0, 0, 0, 0 );

	// bitmap
	for( unsigned int i = 0; i < m_marks.size(); i++ ) {
		memDc.SelectObject( bitmap );
		memDc.Clear();

		g.setColor( kome::img::ColorTool::BLACK );
		g.drawMark( m_marks[ i ], 8, 8, iconSize );

		memDc.SelectObject( wxNullBitmap );

		Append( wxEmptyString, bitmap );
	}
	Select( 0 );
}
