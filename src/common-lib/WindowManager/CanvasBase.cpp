/**
 * @file CanvasBase.cpp
 * @brief implements of CanvasBase class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CanvasBase.h"

#include <wx/dcbuffer.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_GRAPH_MARGIN_LEFT	   0
#define DEFAULT_GRAPH_MARGIN_TOP	  10
#define DEFAULT_GRAPH_MARGIN_RIGHT	   0
#define DEFAULT_GRAPH_MARGIN_BOTTOM	   0
#define DEFAULT_MARGIN_LEFT			  50
#define DEFAULT_MARGIN_TOP			  10
#define DEFAULT_MARGIN_RIGHT		  20
#define DEFAULT_MARGIN_BOTTOM		  35

#define MINIMUM_DRAW_SIZE			  30


BEGIN_EVENT_TABLE( CanvasBase, Canvas )
END_EVENT_TABLE()


// constructor
CanvasBase::CanvasBase( wxWindow* parent, const int id, const int width, const int height )
		: Canvas( parent, id, width, height ) {
}

// destructor
CanvasBase::~CanvasBase() {
}

// get graphics object
kome::img::Graphics& CanvasBase::getGraphics() {
	return m_graphics;
}

// on draw
void CanvasBase::onDraw( wxDC& dc ) {
	// set device context
	m_graphics.setDC( &dc );

	// prepare
	onPrepare( dc );

	// check size
	if( m_graphics.getGraphWidth() < MINIMUM_DRAW_SIZE
			|| m_graphics.getGraphHeight() < MINIMUM_DRAW_SIZE ) {
		m_graphics.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
		m_graphics.setTextVAlign( kome::img::Graphics::VALIGN_MIDDLE );

		m_graphics.drawText(
			"too small",
			m_graphics.getWidth() / 2,
			m_graphics.getHeight() / 2
		);

		return;
	}

	// draw
	onDrawGraphics( dc );
}

// on size
void CanvasBase::onSize( const unsigned int width, const unsigned int height ) {
	// set graph size
	m_graphics.setSize( width, height );

	// set graph margin
	m_graphics.setGraphMargin(
		DEFAULT_GRAPH_MARGIN_LEFT,
		DEFAULT_GRAPH_MARGIN_TOP,
		DEFAULT_GRAPH_MARGIN_RIGHT,
		DEFAULT_GRAPH_MARGIN_BOTTOM
	);

	// set graph position
	m_graphics.setGraphPosition(
		DEFAULT_MARGIN_LEFT,
		DEFAULT_MARGIN_TOP,
		width - DEFAULT_MARGIN_RIGHT,
		height - DEFAULT_MARGIN_BOTTOM
	);
}

// prepare
void CanvasBase::onPrepare( wxDC& dc ) {
	// implement on subclass
}

// on draw graphics
void CanvasBase::onDrawGraphics( wxDC& dc ) {
	// implement on subclass

}

// set Displayed Range (spectrum)
void CanvasBase::setDisplayedRange(kome::objects::Spectrum* spec){
	// set range
	double minX = 0.0;
	double maxX = 0.0;
	double minY = 0.0;
	double maxY = 0.0;
	// get xy data
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	kome::core::XYData* xyData = NULL;
	if( spec != NULL ) {
		xyData = ptMgr.getXYData( spec );
	}
	
	if( xyData != NULL ) {
		minX = xyData->getMinX();
		maxX = xyData->getMaxX();
		minY = xyData->getMinY();
		maxY = xyData->getMaxY();
	}
	// set range
	double midX = ( minX + maxX ) / 2.0;
	double midY = ( minY + maxY ) / 2.0;

	minX = MIN( minX, midX - 0.01 );
	maxX = MAX( maxX, midX + 0.01 );
	minY = MIN( minY, midY - 0.01 );
	maxY = MAX( maxY, midY + 0.01 );
		
	m_graphics.setDisplayedRange( minX, maxX, minY, maxY );
}

// set Displayed Range (chromatogram)
void CanvasBase::setDisplayedRange(kome::objects::Chromatogram* chrom){
	// check the member
	double minX = 0.0;
	double maxX = 0.0;
	double minY = 0.0;
	double maxY = 0.0;

	// xy data
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	kome::core::XYData* xyData = NULL;
	if( chrom != NULL ) {
		xyData = ptMgr.getXYData( chrom );
	}
	if( xyData != NULL ) {
		minX = xyData->getMinX();
		maxX = xyData->getMaxX();
		minY = xyData->getMinY();
		maxY = xyData->getMaxY();
	}

	// set range
	double midX = ( minX + maxX ) / 2.0;
	double midY = ( minY + maxY ) / 2.0;

	minX = MIN( minX, midX - 0.01 );
	maxX = MAX( maxX, midX + 0.01 );
	minY = MIN( minY, midY - 0.01 );
	maxY = MAX( maxY, midY + 0.01 );
	
	m_graphics.setDisplayedRange( minX, maxX, minY, maxY );
}
