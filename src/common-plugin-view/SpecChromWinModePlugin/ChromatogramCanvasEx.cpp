/**
 * @file ChromatogramCanvasEx.cpp
 * @brief implements of ChromatogramCanvasEx class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramCanvasEx.h"

#include "SpecChromWindowManager.h"
#include "chain24x24.xpm"
#include "chain24x24_on.xpm"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ChromatogramCanvasEx, kome::window::ChromatogramCanvas )
END_EVENT_TABLE()


// constructor
ChromatogramCanvasEx::ChromatogramCanvasEx( wxWindow* parent, kome::objects::Chromatogram* chrom, const int width, const int height )
		: kome::window::ChromatogramCanvas( parent, width, height, chrom ), m_titleFont( 8 ) {
    // initialize
	m_buttonRect.l = -1000;
	m_buttonRect.r = -1000;
	m_buttonRect.t = -1000;
	m_buttonRect.b = -1000;

	m_startRt = -1.0;
	m_endRt = -1.0;
}

// destructor
ChromatogramCanvasEx::~ChromatogramCanvasEx() {
}

// on prepare draw
void ChromatogramCanvasEx::onPrepareDraw( kome::window::DCGraphics& g ) {
	// default
	kome::window::ChromatogramCanvas::onPrepareDraw( g );

	// top margin
	g.setFont( m_titleFont );
	int w = 0;
	int h = 0;
	g.getTextSize( "ABC", &w, &h );

	int top = std::max( 30, h + 2 );
	if( top > g.getGraphPositionTop() ) {
		g.setGraphPositionTop( top );
	}
}

// on mouse button down
bool ChromatogramCanvasEx::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// link mode
	if( mgr.isLinkMode() && evt.lbutton() ) {
		if( evt.x() >= m_graphics.getGraphPositionLeft()
				&& evt.x() <= m_graphics.getGraphPositionRight()
				&& evt.y() >= m_graphics.getGraphPositionTop()
				&& evt.y() <= m_graphics.getGraphPositionBottom()
		) {
			// start position
			double x = 0.0;
			double y = 0.0;

			m_graphics.transformPositionToData( (double)evt.x(), (double)evt.y(), &x, &y );
			m_startRt = x;
			m_endRt = x;

			return true;
		}
	}

	// button
	if( evt.x() >= m_buttonRect.l && evt.x() <= m_buttonRect.r
			&& evt.y() >= m_buttonRect.t && evt.y() <= m_buttonRect.b ) {
		const bool linkMode = mgr.isLinkMode();
		mgr.setLinkMode( !linkMode );

		return true;
	}

	// default
	return kome::window::ChromatogramCanvas::onMouseButtonDown( evt );
}

// on mouse button up
bool ChromatogramCanvasEx::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// open chromatogram
	if( m_startRt >= 0.0 ) {
		// start & end
		double startRt = std::min( m_startRt, m_endRt );
		startRt = std::max( startRt, m_graphics.getMinX() );
		startRt = std::min( startRt, m_graphics.getMaxX() );

		double endRt = std::max( m_startRt, m_endRt );
		endRt = std::max( endRt, m_graphics.getMinX() );
		endRt = std::min( endRt, m_graphics.getMaxX() );

		// chromatogram
		kome::objects::Chromatogram* chrom = getChromatogram();

		// spectrum
		kome::objects::Spectrum* spec = NULL;
		if( chrom != NULL ) {
			kome::objects::DataSet spectra;
			chrom->getSpectra( spectra, startRt, endRt );

			if( spectra.getNumberOfSpectra() == 1 ) {
				spec = spectra.getSpectrum( 0 );
			}
			else if( spectra.getNumberOfSpectra() > 1 ) {
				kome::objects::AveragedSpectrum* avgSpec = new kome::objects::AveragedSpectrum( chrom->getGroup() );
				for( unsigned int i = 0; i < spectra.getNumberOfSpectra(); i++ ) {
					avgSpec->addSpectrum( spectra.getSpectrum( i ) );
				}

				spec = avgSpec;

				kome::objects::Sample* sample = chrom->getSample();
				sample->getHiddenDataSet()->addSpectrum( avgSpec );
			}
		}

		if( spec != NULL ) {
			mgr.openFrame( spec );
		}

		// restore positions
		m_startRt = -1.0;
		m_endRt = -1.0;

		return true;
	}

	// default
	return kome::window::ChromatogramCanvas::onMouseButtonUp( evt );
}

// on mouse cursor move
bool ChromatogramCanvasEx::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	// position
	if( m_startRt >= 0.0 ) {
		double x = 0.0;
		double y = 0.0;
		m_graphics.transformPositionToData( (double)evt.x(), (double)evt.y(), &x, &y );
		m_endRt = x;

		return true;
	}

	// default
	return kome::window::ChromatogramCanvas::onMouseCursorMove( evt );
}

// draw graph fg
void ChromatogramCanvasEx::onDrawGraphFg( kome::window::DCGraphics& g ) {
	// default
	kome::window::ChromatogramCanvas::onDrawGraphFg( g );

	// color
	COLOR oldColor = g.getColor();
	g.setDrawStyle( kome::img::Graphics::STYLE_DOT );
	g.setColor( kome::img::ColorTool::BLACK );

	// draw positions
	double positions[] = { m_startRt, m_endRt };

	int num = sizeof( positions ) / sizeof( double );
	for( int i = 0; i < num; i++ ) {
		const double pos = positions[ i ];

		double px = 0.0;
		double py = 0.0;

		g.transformDataToPosition( pos, 0.0, &px, &py );

		int ppx = roundnum( px );

		if( ppx > g.getGraphPositionLeft() && ppx < g.getGraphPositionRight() ) {
			g.drawLine( ppx, g.getGraphPositionTop(), ppx, g.getGraphPositionBottom() );
		}
	}

	// color
	g.setColor( oldColor );
}

// draw graph bg
void ChromatogramCanvasEx::onDrawGraphBg( kome::window::DCGraphics& g ) {
	// default
	kome::window::ChromatogramCanvas::onDrawGraphFg( g );

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// RT position
	double startRt = mgr.getStartRt();
	double endRt = mgr.getEndRt();
	if( startRt < 0.0 || endRt < 0.0 || endRt < startRt || endRt < g.getMinX() || startRt > g.getMaxX() ) {
		return;
	}

	double px0 = 0.0;
	double px1 = 0.0;
	double py = 0.0;

	g.transformDataToPosition( startRt, 0.0, &px0, &py );
	g.transformDataToPosition( endRt, 0.0, &px1, &py );
	int ppx0 = roundnum( px0 );
	int ppx1 = roundnum ( px1 );

	// draw the position
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::LIME );
	g.setFillPattern( kome::img::Graphics::PATTERN_SOLID );
	g.setDrawStyle( kome::img::Graphics::STYLE_SOLID );

	if( ppx0 == ppx1 ) {
		g.drawLine( ppx0, g.getGraphPositionTop(), ppx1, g.getGraphPositionBottom() );
	}
	else {
		g.fillRect( ppx0, g.getGraphPositionTop(), ppx1 - ppx0, g.getGraphHeight() );
	}

	g.setColor( oldColor );
}

// draw window fg
void ChromatogramCanvasEx::onDrawWindowFg( kome::window::DCGraphics& g ) {
	// default
	kome::window::ChromatogramCanvas::onDrawWindowFg( g );

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// title
	kome::window::ChildFrame* frame = dynamic_cast< kome::window::ChildFrame* >( GetParent() );
	std::string title;
	if( frame != NULL ) {
		title = frame->GetTitle().c_str();
	}

	// color
	COLOR oldColor = g.getColor();

	// draw title
	g.setFont( m_titleFont );
	g.setColor( kome::img::ColorTool::BLACK );
	g.setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.drawText( title.c_str(), 1, 1 );

    // draw the link button
	g.setDrawStyle ( kome::img::Graphics::STYLE_SOLID );
	g.setFillPattern( kome::img::Graphics::PATTERN_SOLID );
	int t = 3;
	int b = 27;
	int r = g.getWidth() - 8;
	int l = r - 24;

	// bitmap
	wxBitmap bitmap( mgr.isLinkMode() ? chain24x24_on_xpm : chain24x24_xpm );
	wxDC* dc = g.getDC();
	dc->DrawBitmap( bitmap, l, t );

	// set rect
	m_buttonRect.l = l;
	m_buttonRect.r = r;
	m_buttonRect.t = t;
	m_buttonRect.b = b;

	g.setColor( oldColor );
}
