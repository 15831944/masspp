/**
 * @file BitmapCanvas.cpp
 * @brief implements of BitmapCanvas class
 *
 * @author S.Tanaka
 * @date 2013.01.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "BitmapCanvas.h"

#include "noimage.xpm"

#include <wx/dcbuffer.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( BitmapCanvas, Canvas )
END_EVENT_TABLE()


// constructor
BitmapCanvas::BitmapCanvas( wxWindow* parent, const int width, const int height )
        : Canvas( parent, wxID_ANY, width, height ) {
	// initialize
	m_bitmap = new wxBitmap( noimage_xpm );

	// scroll rate
	SetScrollRate( 10, 10 );
}

// destructor
BitmapCanvas::~BitmapCanvas() {
	if( m_bitmap != NULL ) {
		delete m_bitmap;
		m_bitmap = NULL;
	}
}

// set image file path
void BitmapCanvas::setImageFile( const char* path ) {
	// check the path
	std::string p = NVL( path, "" );
	if( p.compare( m_path ) == 0 ) {
		return;
	}
	m_path = p;

	// delete
	if( m_bitmap != NULL ) {
		delete m_bitmap;
		m_bitmap = NULL;
	}

	// create bitmap
	if( p.empty() ) {
		m_bitmap = new wxBitmap( noimage_xpm );
	}
	else {
		m_bitmap = new wxBitmap( p.c_str(), wxBITMAP_TYPE_PNG );
	}

	// virtual size
	if( m_bitmap == NULL ) {
		SetVirtualSize( 0, 0 );
	}
	else {
		wxSize size = GetSize();
		int w0 = size.GetWidth();
		int w1 = m_bitmap->GetWidth();
		int h0 = size.GetHeight();
		int h1 = m_bitmap->GetHeight();
		int w = std::max( w0, w1 );
		int h = std::max( h0, h1 );
		SetSize( w, h );
		SetVirtualSize( w, h );
	}

	// refresh
	Refresh();
}

// draw
void BitmapCanvas::onDraw( wxDC& dc ) {
	dc.DrawBitmap( *m_bitmap, 0, 0 );
}

// on size
void BitmapCanvas::onSize( const unsigned int width, const unsigned int height ) {
}
