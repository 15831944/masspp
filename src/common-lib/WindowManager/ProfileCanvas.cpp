/**
 * @file ProfileCanvas.cpp
 * @brief implements of ProfileCanvas class
 *
 * @author S.Tanaka
 * @date 2008.09.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ProfileCanvas.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ProfileCanvas, CanvasBase )
END_EVENT_TABLE()


// constructor
ProfileCanvas::ProfileCanvas( wxWindow* parent, const int width, const int height )
		: CanvasBase( parent, wxID_ANY, width, height ) {
	m_profileColor = kome::img::ColorTool::BLACK;
}

// destructor
ProfileCanvas::~ProfileCanvas() {
}

// set profile color
void ProfileCanvas::setProfileColor( const COLOR color ) {
	m_profileColor = color;
}

// get profile color
COLOR ProfileCanvas::getProfileColor() {
	return m_profileColor;
}

// on prepare
void ProfileCanvas::onPrepare( wxDC& dc ) {
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );	// No.79270 グラフのY軸スケールが変化する不具合修 @date 2011.02.22 <Add> M.Izumi
	onPrepareDraw( m_graphics );
}

// on draw graphics
void ProfileCanvas::onDrawGraphics( wxDC& dc ) {
	// draw window background
	COLOR oldColor = m_graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = m_graphics.getDrawStyle();
	
	onDrawWindowBg( m_graphics );	// m/z範囲選択時の塗りつぶし処理はここ

	m_graphics.setColor( oldColor );
	m_graphics.setDrawStyle( oldStyle );

	// memory device context
	wxMemoryDC memDc;
	m_graphics.setDC( &memDc );
	m_graphics.setTranslate(
		- m_graphics.getGraphPositionLeft(),
		- m_graphics.getGraphPositionTop()
	);

	memDc.SetBackground( *wxWHITE_BRUSH );

	// draw graph background
	wxBitmap bgBmp( m_graphics.getGraphWidth(), m_graphics.getGraphHeight() );
	memDc.SelectObject( bgBmp );

	memDc.Clear();
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	onDrawGraphBg( m_graphics );

	memDc.SelectObject( wxNullBitmap );

	dc.DrawBitmap(
		bgBmp,
		m_graphics.getGraphPositionLeft(),
		m_graphics.getGraphPositionTop(),
		false
	);

	m_graphics.setColor( oldColor );
	m_graphics.setDrawStyle( oldStyle );

	// draw profile
	wxBitmap profBmp( m_graphics.getGraphWidth(), m_graphics.getGraphHeight() );
	memDc.SelectObject( profBmp );

	memDc.Clear();
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	m_graphics.setColor( m_profileColor );

	onDrawProfile( m_graphics );

	memDc.SelectObject( wxNullBitmap );
	profBmp.SetMask( new wxMask( profBmp, *wxWHITE ) );

	dc.DrawBitmap(
		profBmp,
		m_graphics.getGraphPositionLeft(),
		m_graphics.getGraphPositionTop(),
		true
	);

	m_graphics.setColor( oldColor );
	m_graphics.setDrawStyle( oldStyle );

	// draw graph foreground
	wxBitmap fgBmp( m_graphics.getGraphWidth(), m_graphics.getGraphHeight() );
	memDc.SelectObject( fgBmp );

	memDc.Clear();
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	onDrawGraphFg( m_graphics );

	memDc.SelectObject( wxNullBitmap );
	fgBmp.SetMask( new wxMask( fgBmp, *wxWHITE ) );

	dc.DrawBitmap(
		fgBmp,
		m_graphics.getGraphPositionLeft(),
		m_graphics.getGraphPositionTop(),
		true
	);

	m_graphics.setColor( oldColor );
	m_graphics.setDrawStyle( oldStyle );

	// restore graphics object
	m_graphics.setDC( &dc );
	m_graphics.resetTransform();

	// draw window foreground
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	onDrawWindowFg( m_graphics );

	m_graphics.setColor( oldColor );
	m_graphics.setDrawStyle( oldStyle );
}

// on prepare draw
void ProfileCanvas::onPrepareDraw( kome::window::DCGraphics& g ) {
	// implement on subclass
}

// on draw window background
void ProfileCanvas::onDrawWindowBg( kome::window::DCGraphics& g ) {
}

// on draw graph background
void ProfileCanvas::onDrawGraphBg( kome::window::DCGraphics& g ) {
}

// on draw profile
void ProfileCanvas::onDrawProfile( kome::window::DCGraphics& g ) {
}

// on draw graph foreground
void ProfileCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
}

// on draw window foreground
void ProfileCanvas::onDrawWindowFg( kome::window::DCGraphics& g ) {
}
