/**
 * @file CairoGraphics.cpp
 * @brief implements of Graphics object
 *
 * @author S.Tanaka
 * @date 2008.10.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CairoGraphics.h"

#include "BitmapData.h"

#include <math.h>
#include <cairo/cairo.h>


using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CairoGraphics::CairoGraphics( const unsigned int w, const unsigned h ) {
	// initialize
	m_cairo = NULL;
	m_surface = NULL;

	// set size
	setSize( w, h );
}

// destructor
CairoGraphics::~CairoGraphics() {
	if( m_cairo != NULL ) {
		cairo_destroy( (cairo_t*)m_cairo );
	}
	if( m_surface != NULL ) {
		cairo_surface_destroy( (cairo_surface_t*)m_surface );
	}

	m_cairo = NULL;
	m_surface = NULL;
}

// get cairo
void* CairoGraphics::getCairo() {
	return m_cairo;
}

// get surface
void* CairoGraphics::getSurface() { 
	return m_surface;
}

// save PNG file
bool CairoGraphics::savePng( const char* filePath ) {
	// check the member
	if( m_surface == NULL ) {
		return false;
	}

	// destroy
	if( m_cairo != NULL ) {
		cairo_destroy( (cairo_t*)m_cairo );
	}

	// save
	cairo_status_t status = cairo_surface_write_to_png(
		(cairo_surface_t*)m_surface,
		filePath
	);

	// create cairo
	m_cairo = cairo_create( (cairo_surface_t*)m_surface );

	return ( status == CAIRO_STATUS_SUCCESS );
}

// on size
void CairoGraphics::onSetSize( const unsigned int width, const unsigned int height ) {
	// check size
	if( width == getWidth() && height == getHeight() ) {
		return;
	}

	// destroy
	if( m_cairo != NULL ) {
		cairo_destroy( (cairo_t*)m_cairo );
	}
	if( m_surface != NULL ) {
		cairo_surface_destroy( (cairo_surface_t*)m_surface );
	}

	// check the size
	if( width == 0 || height == 0 ) {
		m_cairo = NULL;
		return;
	}

	// surface
	m_surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, width, height );

	// cairo
	m_cairo = cairo_create( (cairo_surface_t*)m_surface );

	// set draw information
	onSetDrawStyle( m_drawStyle );
	onSetFillPattern( m_fillPattern );
	onSetFillStyle( m_fillStyle );
	onSetTextHAlign( m_hAlign );
	onSetTextVAlign( m_vAlign );
	onSetColor( m_color );
	onSetFont( m_font );
	onSetPen( m_thickness );	// @date 2013.04.23 <Add> M.Izumi

	// background color
	cairo_set_source_rgb( (cairo_t*)m_cairo, 1.0, 1.0, 1.0 );   // white
	cairo_paint( (cairo_t*)m_cairo );
}

// on set draw style
void CairoGraphics::onSetDrawStyle( const DrawStyle style ) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// width
	cairo_set_line_width( (cairo_t*)m_cairo, 1.0 );

	// create array
	double* dashes = NULL;
	int num = 0;

	if( style == STYLE_DOT ) {
		num = 2;
		dashes = new double[ num ];

		dashes[ 0 ] = 2.0;
		dashes[ 1 ] = 2.0;
	}
	else if( style == STYLE_LONG_DASH ) {
		num = 2;
		dashes = new double[ num ];

		dashes[ 0 ] = 8.0;
		dashes[ 1 ] = 4.0;
	}
	else if( style == STYLE_SHORT_DASH ) {
		num = 2;
		dashes = new double[ num ];

		dashes[ 0 ] = 4.0;
		dashes[ 1 ]  = 4.0;
	}
	else if( style == STYLE_DOT_DASH ) {
		num = 4;
		dashes = new double[ num ];

		dashes[ 0 ] = 4.0;
		dashes[ 1 ] = 2.0;
		dashes[ 2 ] = 2.0;
		dashes[ 3 ] = 2.0;
	}

	cairo_set_dash( (cairo_t*)m_cairo, dashes, num, 0.0 );

	// delete
	if( dashes != NULL ) {
		delete[] dashes;
	}
}

// on set fill patter
void CairoGraphics::onSetFillPattern( const FillPattern pattern ) {
}

// on set fill style
void CairoGraphics::onSetFillStyle( const FillStyle style ) {
}

// on set text horizontal align
void CairoGraphics::onSetTextHAlign( const TextHAlign hAlign ) {
}

// on set text vertical align
void CairoGraphics::onSetTextVAlign( const TextVAlign vAlign ) {
}

// on set color
void CairoGraphics::onSetColor( const COLOR color ) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	cairo_set_source_rgba(
		(cairo_t*)m_cairo,
		(double)ColorTool::getRed( color ) / 255.0,
		(double)ColorTool::getGreen( color ) / 255.0,
		(double)ColorTool::getBlue( color ) / 255.0,
		1.0
	);
}

// on set font
void CairoGraphics::onSetFont( Font& font ) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// family
	std::string family;
	if( font.getFamily() == Font::FAMILY_SERIF ) {
		family = "serif";
	}
	else if( font.getFamily() == Font::FAMILY_SANS_SERIF ) {
		family = "sans-serif";
	}
	else if( font.getFamily() == Font::FAMILY_CURSIVE ) {
		family = "cursive";
	}
	else if( font.getFamily() == Font::FAMILY_FANTASY ) {
		family = "fantasy";
	}
	else if( font.getFamily() == Font::FAMILY_MONOSPACE ) {
		family = "monospace";
	}

	// slant
	cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL;
	if( font.getStyle() == Font::STYLE_ITALIC ) {
		slant = CAIRO_FONT_SLANT_ITALIC;
	}
	else if( font.getStyle() == Font::STYLE_SLANT ) {
		slant = CAIRO_FONT_SLANT_OBLIQUE;
	}

	// weight
	cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;
	if( font.getWeight() == Font::WEIGHT_BOLD ) {
		weight = CAIRO_FONT_WEIGHT_BOLD;
	}

	// set font
	cairo_select_font_face(
		(cairo_t*)m_cairo,
		family.c_str(),
		slant,
		weight
	);

	// size
	cairo_set_font_size( (cairo_t*)m_cairo, (double)font.getSize() * 1.5 );
}

// on set Pen
void CairoGraphics::onSetPen( unsigned int width ){
}

// on draw lines
void CairoGraphics::onDrawLines(
		int* const points,
		const unsigned int num
) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// anti alias mode
	cairo_set_antialias( (cairo_t*)m_cairo, CAIRO_ANTIALIAS_NONE );

	// draw
	for( unsigned int i = 0; i < num; i++ ) {
		if( i == 0 ) {
			cairo_move_to( (cairo_t*)m_cairo, (double)points[ i * 2 ], (double)points[ i * 2 + 1 ] );
		}
		else {
			cairo_line_to( (cairo_t*)m_cairo, (double)points[ i * 2 ], (double)points[ i * 2 + 1 ] );
		}
	}

	// stroke
	cairo_stroke( (cairo_t*)m_cairo );

}

// on fill polygon
void CairoGraphics::onFillPolygon(
		int* const points,
		const unsigned int num
) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// draw
	for( unsigned int i = 0; i < num; i++ ) {
		if( i == 0 ) {
			cairo_move_to( (cairo_t*)m_cairo, (double)points[ i * 2 ], (double)points[ i * 2 + 1 ] );
		}
		else {
			cairo_line_to( (cairo_t*)m_cairo, (double)points[ i * 2 ], (double)points[ i * 2 + 1 ] );
		}
	}
	cairo_close_path( (cairo_t*)m_cairo );

	// fill
	cairo_fill_preserve( (cairo_t*)m_cairo );
}

// on draw elliptic arc
void CairoGraphics::onDrawEllipticArc(
		const int px,
		const int py,
		const int width,
		const int height,
		const double start,
		const double end,
		const bool fill
) {
	// push
	cairo_save( (cairo_t*)m_cairo );

	// draw
	cairo_translate(
		(cairo_t*)m_cairo,
		(double)px + (double)width / 2.0,
		(double)py + (double)height / 2.0
	);
	cairo_scale( (cairo_t*)m_cairo, (double)width / 2.0, (double)height / 2.0 );
	cairo_arc(
		(cairo_t*)m_cairo,
		0.0,
		0.0,
		1.0,
		start * kome::numeric::Math::PI / 180.0,
		end * kome::numeric::Math::PI / 180.0
	);

	// fill
	if( fill ) {
		cairo_fill_preserve( (cairo_t*)m_cairo );
	}

	// pop
	cairo_restore( (cairo_t*)m_cairo );
}

// on draw text
void CairoGraphics::onDrawText(
		const char* str,
		const int px,
		const int py,
		const int angle
) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// extents
	cairo_text_extents_t extents;
	cairo_text_extents( (cairo_t*)m_cairo, str, &extents );

	// text position
	double tx = 0.0;
	double ty = 0.0;

	if( m_hAlign == HALIGN_CENTER ) {
		tx = (double)extents.width / 2.0;
	}
	else if( m_hAlign == HALIGN_RIGHT ) {
		tx = (double)extents.width;
	}

	if( m_vAlign == VALIGN_MIDDLE ) {
		ty = - (double)extents.height / 2.0;
	}
	else if( m_vAlign == VALIGN_TOP ) {
		ty = - (double)extents.height;
	}

	// matrix
	kome::core::Matrix m0( 3, 3 );	// set text position
	m0( 0, 0 ) = 1.0;		m0( 0, 1 ) = 0.0;		m0( 0, 2 ) = -tx;
	m0( 1, 0 ) = 0.0;		m0( 1, 1 ) = 1.0;		m0( 1, 2 ) = -ty;
	m0( 2, 0 ) = 0.0;		m0( 2, 1 ) = 0.0;		m0( 2, 2 ) = 1.0;

	double theta = - (double)angle * kome::numeric::Math::PI / 180.0;
	double cosTheta = cos( theta );
	double sinTheta = sin( theta );

	kome::core::Matrix m1( 3, 3 );	// rotate
	m1( 0, 0 ) = cosTheta;   m1( 0, 1 ) = - sinTheta; m1( 0, 2 ) = 0.0;
	m1( 1, 0 ) = sinTheta;   m1( 1, 1 ) = cosTheta;   m1( 1, 2 ) = 0.0;
	m1( 2, 0 ) = 0.0;		m1( 2, 1 ) = 0.0;		m1( 2, 2 ) = 1.0;

	kome::core::Matrix m2( 3, 3 );	// move
	m2( 0, 0 ) = 1.0;		m2( 0, 1 ) = 0.0;		m2( 0, 2 ) = (double)px;
	m2( 1, 0 ) = 0.0;		m2( 1, 1 ) = 1.0;		m2( 1, 2 ) = (double)py;
	m2( 2, 0 ) = 0.0;		m2( 2, 1 ) = 0.0;		m2( 2, 2 ) = 1.0;

	// mult
	kome::core::Matrix m = m2 * m1 * m0;

	// set matrix
	cairo_matrix_t mat;
	mat.xx = m( 0, 0 );	  mat.xy = m( 0, 1 );	  mat.x0 = m( 0, 2 );
	mat.yx = m( 1, 0 );	  mat.yy = m( 1, 1 );	  mat.y0 = m( 1, 2 );

	cairo_set_matrix( (cairo_t*)m_cairo, &mat );

	// draw text
	cairo_move_to( (cairo_t*)m_cairo, 0.0, 0.0 );
	cairo_show_text( (cairo_t*)m_cairo, str );

	// restore
	cairo_identity_matrix( (cairo_t*)m_cairo );
}

// on get text size
void CairoGraphics::onGetTextSize(
		const char* str,
		int* width,
		int* height
) {
	// check the member
	if( m_cairo == NULL ) {
		return;
	}

	// extents
	cairo_text_extents_t extents;

	cairo_text_extents( (cairo_t*)m_cairo, str, &extents );

	// store
	if( width != NULL ) {
		*width = roundnum( extents.width );
	}
	if( height != NULL ) {
		*height = roundnum( extents.height );
	}
}

// on draw point
void CairoGraphics::onDrawPoint( const int px, const int py ) {
}

// on draw bitmap
void CairoGraphics::onDrawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent ) {
	// create data
	unsigned char* data = NULL;
	if( bitmap.getWidth() > 0 && bitmap.getHeight() ) {
		data = new unsigned char[ 4 * bitmap.getWidth() * bitmap.getHeight() ];
		for( unsigned int i = 0; i < bitmap.getHeight(); i++ ) {
			for( unsigned int j = 0; j < bitmap.getWidth(); j++ ) {
				int idx = 4 * ( bitmap.getWidth() * i + j );
				COLOR color = bitmap.getPixel( j, i );

				data[ idx + 0 ] = ColorTool::getBlue( color );
				data[ idx + 1 ] = ColorTool::getGreen( color );
				data[ idx + 2 ] = ColorTool::getRed( color );
				data[ idx + 3 ] = 0;
			}
		}
	}

	// create surface
	int stride = cairo_format_stride_for_width( CAIRO_FORMAT_RGB24, bitmap.getWidth() );
	cairo_surface_t* surface = cairo_image_surface_create_for_data(
		data,
		CAIRO_FORMAT_RGB24,
		bitmap.getWidth(),
		bitmap.getHeight(),
		stride
	);

	// draw
	cairo_set_source_surface( (cairo_t*)m_cairo, surface, (double)px, (double)py );
	cairo_paint( (cairo_t*)m_cairo );

	// delete
	cairo_surface_destroy( (cairo_surface_t*)m_surface );

	if( data != NULL ) {
		delete[] data;
	}
}

// on get pixel
COLOR CairoGraphics::onGetPixel( const int px, const int py ) {
	// get color
	COLOR c = ColorTool::BLACK;

	// part of the way

	return c;
}

// on flush
void CairoGraphics::onFlush() {
}
