/**
 * @file ClrGraphics.cpp
 * @brief implements of ClrGraphics class
 *
 * @author S.Tanaka
 * @date 2006.09.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrGraphics.h"

#include "FontWrapper.h"
#include "BitmapDataWrapper.h"
#include "ConversionArrays.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrGraphics::ClrGraphics() {
}

// destructor
ClrGraphics::~ClrGraphics() {
	ClrObjectPool::getInstance().removeGraphics( *this );
}

// set base graphics
void ClrGraphics::setBaseGraphics( ClrGraphicsBase^ baseGraphics ) {
	m_baseGraphics = baseGraphics;
}

// get base graphics
ClrGraphicsBase^ ClrGraphics::getBaseGraphics() {
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	return baseGraphics;
}

// set size
void ClrGraphics::onSetSize( const unsigned int width, const unsigned int height ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	baseGraphics->onSetSize( width, height );
}

// set draw style
void ClrGraphics::onSetDrawStyle( const DrawStyle style ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	GraphicsWrapper::DrawStyle s
		= CONVERT_FROM_ARRAY( style, GraphicsWrapper::DrawStyle::SOLID, g_graphicsDrawStyles );
	baseGraphics->onSetDrawStyle( s );
}

// set fill pattern
void ClrGraphics::onSetFillPattern( const FillPattern pattern ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	GraphicsWrapper::FillPattern p
		= CONVERT_FROM_ARRAY( pattern, GraphicsWrapper::FillPattern::SOLID, g_graphicsFillPatterns );
	baseGraphics->onSetFillPattern( p );
}

// set fill style
void ClrGraphics::onSetFillStyle( const FillStyle style ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	GraphicsWrapper::FillStyle s
		= CONVERT_FROM_ARRAY( style, GraphicsWrapper::FillStyle::ODDEVEN, g_graphicsFillStyles );
	baseGraphics->onSetFillStyle( s );
}

// set text horizontal align
void ClrGraphics::onSetTextHAlign( const TextHAlign hAlign ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	GraphicsWrapper::TextHAlign ha
		= CONVERT_FROM_ARRAY( hAlign, GraphicsWrapper::TextHAlign::LEFT, g_graphicsHAligns );
	baseGraphics->onSetTextHAlign( ha );
}

// set text vertical align
void ClrGraphics::onSetTextVAlign( const TextVAlign vAlign ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	GraphicsWrapper::TextVAlign va
		= CONVERT_FROM_ARRAY( vAlign, GraphicsWrapper::TextVAlign::TOP, g_graphicsVAligns );
	baseGraphics->onSetTextVAlign( va );
}

// set color
void ClrGraphics::onSetColor( const COLOR color ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	unsigned char r = kome::img::ColorTool::getRed( color );
	unsigned char g = kome::img::ColorTool::getGreen( color );
	unsigned char b = kome::img::ColorTool::getBlue( color );

	baseGraphics->onSetColor( r, g, b );
}

// set font
void ClrGraphics::onSetFont( kome::img::Font& font ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	baseGraphics->onSetFont( gcnew FontWrapper( font ) );
}

// set pen
void ClrGraphics::onSetPen( unsigned int width ){
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// set
	baseGraphics->onSetPen( width );
}

// draw line
void ClrGraphics::onDrawLines(
		int* const points,
		const unsigned int num
) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// create array
	cli::array< int, 2 >^ ptArray = gcnew cli::array< int, 2 >( num, 2 );
	for( unsigned int i = 0; i < num; i++ ) {
		ptArray[ i, 0 ] = points[ i * 2 ];
		ptArray[ i, 1 ] = points[ i * 2 + 1 ];
	}

	// draw
	baseGraphics->onDrawLines( ptArray );
}

// fill polygon
void ClrGraphics::onFillPolygon(
		int* const points,
		const unsigned int num
) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// create array
	cli::array< int, 2 >^ ptArray = gcnew cli::array< int, 2 >( num, 2 );
	for( unsigned int i = 0; i < num; i++ ) {
		ptArray[ i, 0 ] = points[ i * 2 ];
		ptArray[ i, 1 ] = points[ i * 2 + 1 ];
	}

	// fill polygon
	baseGraphics->onFillPolygon( ptArray );
}

// draw elliptic arc
void ClrGraphics::onDrawEllipticArc(
		const int px,
		const int py,
		const int width,
		const int height,
		const double start,
		const double end,
		const bool fill
) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// draw
	baseGraphics->onDrawEllipticArc( px, py, width, height, start, end, fill );
}

// draw text
void ClrGraphics::onDrawText(
		const char* str,
		const int px,
		const int py,
		const int angle
) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// String
	System::String^ s = ClrObjectTool::convertString( str, "" );

	// draw text
	baseGraphics->onDrawText( s, px, py, angle );
}

// get text size
void ClrGraphics::onGetTextSize( const char* str, int* width, int* height ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// get text size
	baseGraphics->onGetTextSize(
		ClrObjectTool::convertString( str, "" ), 
		*width,
		*height
	);
}

// draw point
void ClrGraphics::onDrawPoint( int px, int py ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// draw point
	baseGraphics->onDrawPoint( px, py );
}

// draw bitmap
void ClrGraphics::onDrawBitmap( kome::img::BitmapData& bitmap, const int px, const int py, const bool transparent ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	// bitmap object
	BitmapDataWrapper^ bm = gcnew BitmapDataWrapper( bitmap );

	// draw bitmap
	baseGraphics->onDrawBitmap( bm, px, py, transparent );
}

// get pixel
COLOR ClrGraphics::onGetPixel( const int px, const int py ) {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return 0;
	}

	System::Drawing::Color color = baseGraphics->onGetPixel( px, py );

	COLOR c = kome::img::ColorTool::getColor( color.R, color.G, color.B );
	return c;
}

// flush
void ClrGraphics::onFlush() {
	// check member
	ClrGraphicsBase^ baseGraphics = m_baseGraphics;
	if( baseGraphics == nullptr ) {
		return;
	}

	baseGraphics->onFlush();
}
