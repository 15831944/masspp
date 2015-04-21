/**
 * @file FontCanvas.cpp
 * @brief implements of FontCanvas class
 *
 * @author S.Tanaka
 * @date 2013.04.08
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "FontCanvas.h"

#include <wx/dcbuffer.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( FontCanvas, CanvasBase )
END_EVENT_TABLE()


// constructor
FontCanvas::FontCanvas( wxWindow* parent, const int id, const int width, const int height )
		: CanvasBase( parent, id, width, height ), m_font( 10 ) {
    // initialize
    m_color = kome::img::ColorTool::BLACK;
	m_text = "AaBbCc0123";
}

// destructor
FontCanvas::~FontCanvas() {
}

// set font
void FontCanvas::setFont( kome::img::Font& font ) {
	m_font = font;
}

// get font
kome::img::Font& FontCanvas::getFont() {
	return m_font;
}

// set color
void FontCanvas::setColor( const COLOR color ) {
	m_color = color;
}

// get color
COLOR FontCanvas::getColor() {
	return m_color;
}

// set text
void FontCanvas::setText( const char* text ) {
	m_text = NVL( text, "" );
}

// get text
const char* FontCanvas::getText() {
	return m_text.c_str();
}

// on draw
void FontCanvas::onDraw( wxDC& dc ) {
	// set device context
	m_graphics.setDC( &dc );

	// size
	wxSize size = GetClientSize();

	// draw
	m_graphics.setFont( m_font );
	m_graphics.setColor( m_color );
	m_graphics.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	m_graphics.setTextVAlign( kome::img::Graphics::VALIGN_MIDDLE );

	m_graphics.drawText( m_text.c_str(), size.GetX() / 2, size.GetY() / 2 );
}
