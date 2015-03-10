/**
 * @file SpectrumCanvasEx.cpp
 * @brief implements of SpectrumCanvasEx class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumCanvasEx.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SpectrumCanvasEx, kome::window::SpectrumCanvas )
END_EVENT_TABLE()



// constructor
SpectrumCanvasEx::SpectrumCanvasEx( wxWindow* parent, kome::objects::Spectrum* spec, const int width, const int height )
		: kome::window::SpectrumCanvas( parent, width, height, spec ), m_titleFont( 8 ) {
}

// destructor
SpectrumCanvasEx::~SpectrumCanvasEx() {
}

// on prepare draw
void SpectrumCanvasEx::onPrepareDraw( kome::window::DCGraphics& g ) {
	// default
	kome::window::SpectrumCanvas::onPrepareDraw( g );

	// top margin
	g.setFont( m_titleFont );
	int w = 0;
	int h = 0;
	g.getTextSize( "ABC", &w, &h );

	int top = h + 2;
	if( top > g.getGraphPositionTop() ) {
		g.setGraphPositionTop( top );
	}
}

// draw window fg
void SpectrumCanvasEx::onDrawWindowFg( kome::window::DCGraphics& g ) {
	// default
	kome::window::SpectrumCanvas::onDrawWindowFg( g );

	// title
	kome::window::ChildFrame* frame = dynamic_cast< kome::window::ChildFrame* >( GetParent() );
	std::string title;
	if( frame != NULL ) {
		title = frame->GetTitle().c_str();
	}

	// draw
	COLOR oldColor = g.getColor();
	g.setFont( m_titleFont );
	g.setColor( oldColor );
	g.setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.drawText( title.c_str(), 1, 1 );

	g.setColor( oldColor );
}
