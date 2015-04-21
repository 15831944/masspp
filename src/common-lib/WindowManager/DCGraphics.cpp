/**
 * @file DCGraphics.cpp
 * @brief implements of DCGraphics class
 *
 * @author S.Tanaka
 * @date 2006.09.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DCGraphics.h"

#include "FontTool.h"

#include <math.h>
#include <wx/wx.h>
#include <wx/image.h>

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// draw style conversion array
kome::core::ConversionElement< kome::img::Graphics::DrawStyle, int > g_graphicsDrawStyles[] = {
	{ kome::img::Graphics::STYLE_SOLID,	  wxSOLID },
	{ kome::img::Graphics::STYLE_DOT,		wxDOT },
	{ kome::img::Graphics::STYLE_LONG_DASH,  wxLONG_DASH },
	{ kome::img::Graphics::STYLE_SHORT_DASH, wxSHORT_DASH },
	{ kome::img::Graphics::STYLE_DOT_DASH,   wxDOT_DASH }
};

// fill pattern conversion array
kome::core::ConversionElement< kome::img::Graphics::FillPattern, int > g_graphicsFillPatterns[] = {
	{ kome::img::Graphics::PATTERN_SOLID,	  wxSOLID },
	{ kome::img::Graphics::PATTERN_BDIAGONAL,  wxBDIAGONAL_HATCH },
	{ kome::img::Graphics::PATTERN_FDIAGONAL,  wxFDIAGONAL_HATCH },
	{ kome::img::Graphics::PATTERN_CROSS,	  wxCROSS_HATCH },
	{ kome::img::Graphics::PATTERN_CROSS_DIAG, wxCROSSDIAG_HATCH },
	{ kome::img::Graphics::PATTERN_HORIZONTAL, wxHORIZONTAL_HATCH },
	{ kome::img::Graphics::PATTERN_VERTICAL,   wxVERTICAL_HATCH }
};

// fill style conversion array
kome::core::ConversionElement< kome::img::Graphics::FillStyle, int > g_graphicsFillStyles[] = {
	{ kome::img::Graphics::RULE_ODDEVEN, wxODDEVEN_RULE },
	{ kome::img::Graphics::RULE_WINDING, wxWINDING_RULE }
};


// construcotr
DCGraphics::DCGraphics() {
	m_drawMode = DRAW_NORMAL;
	m_dc = NULL;
	m_orgDc = NULL;
	m_memDc = NULL;
	m_bitmap = NULL;
}

DCGraphics::DCGraphics( wxDC* dc ) {
	m_drawMode = DRAW_NORMAL;
	m_dc = NULL;
	m_orgDc = NULL;
	m_memDc = NULL;
	m_bitmap = NULL;

	setDC( dc );
}

// destructor
DCGraphics::~DCGraphics() {
	if( m_memDc != NULL ) {
		delete m_memDc;
	}
	if( m_bitmap != NULL ) {
		delete m_bitmap;
	}
}

// set device context
void DCGraphics::setDC( wxDC* dc ) {
	// check the member
	if( m_orgDc == dc ) {
		return;
	}

	// set DC
	m_orgDc = dc;

	// udate
	setDrawMode( m_drawMode );
}

// get device context
wxDC* DCGraphics::getDC() {
	return m_dc;
}

// set draw mode
void DCGraphics::setDrawMode( DrawMode drawMode ) {
	// set the member
	m_drawMode = drawMode;

	// flush
	flush();

	// delete memory DC
	if( m_memDc != NULL ) {
		delete m_memDc;
		m_memDc = NULL;
	}
	if( m_bitmap != NULL ) {
		delete m_bitmap;
		m_bitmap = NULL;
	}

	// update member
	if( drawMode == DRAW_NORMAL ) {
		m_dc = m_orgDc;
	}
	else {
		// memory DC
		m_memDc = new wxMemoryDC();

		// update bitmap
		unsigned int width = m_width;
		unsigned int height = m_height;

		m_width = 0;
		m_height = 0;

		setSize( width, height );

		// background
		if( drawMode == DRAW_BUFFERED_OR ) {
			m_memDc->SetBackground( *wxBLACK_BRUSH );
		}
		else {
			m_memDc->SetBackground( *wxWHITE_BRUSH );
		}

		// clear
		m_memDc->Clear();

		// set DC
		m_dc = m_memDc;
	}

	// update
	onSetDrawStyle( m_drawStyle );
	onSetFillPattern( m_fillPattern );
	onSetFillStyle( m_fillStyle );
	onSetTextHAlign( m_hAlign );
	onSetTextVAlign( m_vAlign );
	onSetColor( m_color );
	onSetFont( m_font );
	onSetPen( m_thickness );	// @date 2013.04.23 <Add> M.Izumi	
}

// get draw mode
DCGraphics::DrawMode DCGraphics::getDrawMode() {
	return m_drawMode;
}

// set pen
void DCGraphics::setWxPen( const DrawStyle style, const COLOR color, int width ) {
	int s = CONVERT_FROM_ARRAY( style, (int)wxSOLID, g_graphicsDrawStyles );
	wxPen pen( wxColour( m_color ), width, s );
	m_dc->SetPen( pen );
}

// set brush
void DCGraphics::setWxBrush( const FillPattern pattern, const COLOR color ) {
	int p = CONVERT_FROM_ARRAY( pattern, (int)wxSOLID, g_graphicsFillPatterns );
	wxBrush brush( wxColour( m_color ), p );
	m_dc->SetBrush( brush );
}

// set font
void DCGraphics::setWxFont( kome::img::Font& font ) {
	// convert
	wxFont f = FontTool::convertFont( font );

	// set font
	m_dc->SetFont( f );
}

// set size
void DCGraphics::onSetSize( const unsigned int width, const unsigned int height ) {
	// delete bitmap
	if( m_bitmap == NULL ) {
		delete m_bitmap;
		m_bitmap = NULL;
	}

	// check parameters
	if( width == 0 || height == 0 ) {
		return;
	}

	// check the members
	if( m_drawMode == DRAW_NORMAL || m_memDc == NULL ) {
		return;
	}

	// create bitmap
	m_bitmap = new wxBitmap( width, height );
	m_memDc->SelectObject( *m_bitmap );
}

// set draw style
void DCGraphics::onSetDrawStyle( const DrawStyle style ) {
	// check device context
	if( m_dc == NULL ) {
		return;
	}

	// set pen
	setWxPen( style, m_color );
}

// set fill pattern
void DCGraphics::onSetFillPattern( const FillPattern pattern ) {
	// check device context
	if( m_dc == NULL ) {
		return;
	}

	// set brush
	setWxBrush( pattern, m_color );
}

// set fill style
void DCGraphics::onSetFillStyle( const FillStyle style ) {
}

// set text horizontal align
void DCGraphics::onSetTextHAlign( const TextHAlign hAlign ) {
}

// set text vertical align
void DCGraphics::onSetTextVAlign( const TextVAlign vAlign ) {
}

// set color
void DCGraphics::onSetColor( const COLOR color ) {
	// check device context
	if( m_dc == NULL ) {
		return;
	}

	// R,G,B
	unsigned char r = kome::img::ColorTool::getRed( color );
	unsigned char g = kome::img::ColorTool::getGreen( color );
	unsigned char b = kome::img::ColorTool::getBlue( color );

	// set
	setWxPen( m_drawStyle, color );
	setWxBrush( m_fillPattern, color );
	m_dc->SetTextForeground( wxColour( r, g, b ) );
}

// set font
void DCGraphics::onSetFont( kome::img::Font& font ) {
	// check device context
	if( m_dc == NULL ) {
		return;
	}
	
	// set font
	setWxFont( font );
}

// set pen
void DCGraphics::onSetPen( unsigned int width ){
	// check device context
	if( m_dc == NULL ) {
		return;
	}

	setWxPen( m_drawStyle, m_color, width );
}

// draw lines
void DCGraphics::onDrawLines( int* const points, const unsigned int num ) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// check parameters
	if( num == 0 || points == NULL ) {
		return;
	}

	// draw line
	int px0 = points[ 0 ];
	int py0 = points[ 1 ];
	for( unsigned int i = 1; i < num; i++ ) {
		int px1 = points[ i * 2 ];
		int py1 = points[ i * 2 + 1 ];

		m_dc->DrawLine( px0, py0, px1, py1 );

		px0 = px1;
		py0 = py1;
	}
	m_dc->DrawLine( px0, py0, px0 + 1, py0 );
}

// fill polygon
void DCGraphics::onFillPolygon( int* const points, const unsigned int num ){
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// check parameters
	if( num == 0 || points == NULL ) {
		return;
	}

	// create array
	wxPoint* pts = new wxPoint[ num ];
	for( unsigned int i = 0; i < num; i++ ) {
		pts[ i ].x = points[ 2 * i ];
		pts[ i ].y = points[ 2 * i + 1];
	}

	// set pen
	wxPen oldPen = m_dc->GetPen();
	m_dc->SetPen( *wxTRANSPARENT_PEN );

	// draw
	int s = CONVERT_FROM_ARRAY( m_fillStyle, (int)wxODDEVEN_RULE, g_graphicsFillStyles );
	m_dc->DrawPolygon( num, pts, 0, 0, s );

	// recover pen
	m_dc->SetPen( oldPen );

	// delete array
	delete[] pts;
}

// draw elliptic arc
void DCGraphics::onDrawEllipticArc(
		const int px,
		const int py,
		const int width,
		const int height,
		const double start,
		const double end,
		const bool fill
) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// brush
	wxBrush brush = m_dc->GetBrush();
	if( !fill ) {
		m_dc->SetBrush( *wxTRANSPARENT_BRUSH );
	}

	// draw
	m_dc->DrawEllipticArc( px, py, width, height, start, end );

	// restore
	if( !fill ) {
		m_dc->SetBrush( brush );
	}
}

// draw text
void DCGraphics::onDrawText( const char* str, const int px, const int py, const int angle ) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// check the parameter
	if( str == NULL ) {
		return;
	}

	// get size
	int width = int();
	int height = int();
	m_dc->GetTextExtent( wxT( NVL( str, "" ) ), &width, &height );

	// angle
	int ang = angle % 360;

	// get coordinate
	double x = 0.0;
	if( m_hAlign == kome::img::Graphics::HALIGN_CENTER ) {
		x = (double)width / 2.0;
	}
	else if( m_hAlign == kome::img::Graphics::HALIGN_RIGHT ) {
		x = (double)width;
	}

	double y = 0.0;
	if( m_vAlign == kome::img::Graphics::VALIGN_MIDDLE ) {
		y = (double)height / 2.0;
	}
	else if( m_vAlign == kome::img::Graphics::VALIGN_BOTTOM ) {
		y = (double)height;
	}

	if( ang != 0 ) {
		double theta = - kome::numeric::Math::PI * (double)ang / 180.0;
		double cosTheta = cos( theta );
		double sinTheta = sin( theta );

		double x0 = cosTheta * x - sinTheta * y;
		double y0 = sinTheta * x + cosTheta * y;

		x = x0;
		y = y0;
	}

	// draw text
	m_dc->DrawRotatedText(
		wxT( NVL( str, "" ) ),
		px - roundnum( x ),
		py - roundnum( y ),
		ang
	);
}

// get text size
void DCGraphics::onGetTextSize( const char* str, int* width, int* height ) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// check the parameter
	if( str == NULL ) {
		return;
	}

	// get size
	m_dc->GetTextExtent( wxT( str ), width, height );
}

// draw point
void DCGraphics::onDrawPoint( const int px, const int py ) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// draw point
	m_dc->DrawPoint( px, py );
}

// on draw bitmap
void DCGraphics::onDrawBitmap( kome::img::BitmapData& bitmap, const int px, const int py, const bool transparent ) {
	// check the member
	if( m_dc == NULL ) {
		return;
	}

	// image
	wxImage img( bitmap.getWidth(), bitmap.getHeight() );
	for( unsigned int i = 0; i < bitmap.getHeight(); i++ ) {
		for( unsigned int j = 0; j < bitmap.getWidth(); j++ ) {
			// color
			COLOR color = bitmap.getPixel( j, i );
			unsigned char r = kome::img::ColorTool::getRed( color );
			unsigned char g = kome::img::ColorTool::getGreen( color );
			unsigned char b = kome::img::ColorTool::getBlue( color );

			img.SetRGB( j, i, r, g, b );
		}
	}

	// bitmap
	wxBitmap bm( img );

	// set pixel
	m_dc->DrawBitmap( bm, px, py, transparent );
}

// get pixel color
COLOR DCGraphics::onGetPixel( const int px, const int py ) {
	if( m_dc == NULL ) {
		return -1;
	}

	// get color
	wxColour colour;
	m_dc->GetPixel( px, py, &colour );

	COLOR c = kome::img::ColorTool::getColor( colour.Red(), colour.Green(), colour.Blue() );
	return c;
}

// flush
void DCGraphics::onFlush() {
	// check members
	if( m_drawMode == DRAW_NORMAL
			|| m_memDc == NULL || m_orgDc == NULL
			|| m_width == 0 || m_height == 0 ) {
		return;
	}

	// logical function
	int lf = wxCOPY;
	if( m_drawMode == DRAW_BUFFERED_AND ) {
		lf = wxAND;
	}
	else if( m_drawMode == DRAW_BUFFERED_OR ) {
		lf = wxOR;
	}

	// flush
	m_orgDc->Blit(
		0, 0, m_width, m_height, m_memDc, 0, 0, lf
	);

	// clear
	m_memDc->Clear();
}
