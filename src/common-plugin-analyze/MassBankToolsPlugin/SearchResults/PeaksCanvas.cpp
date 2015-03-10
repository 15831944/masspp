/**
 * @file PeaksCanvas.cpp
 * @brief implements of PeaksCanvas
 *
 * @author S.Tanaka
 * @date 2009.09.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "PeaksCanvas.h"

#include "../MassBankManager.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( PeaksCanvas, kome::window::ProfileCanvas )
END_EVENT_TABLE()

#define MARGIN_SIZE		20
#define SCALE_LENGTH	6
#define TEXT_SPACE		5
#define MIN_SCALE_SPACE	10

// constructor
PeaksCanvas::PeaksCanvas( wxWindow* parent )
	: kome::window::ProfileCanvas( parent, 100, 100 ) 
{
	m_rangeOp = OP_NONE;
}

// destructor
PeaksCanvas::~PeaksCanvas() 
{
}

// on mouse button down
bool PeaksCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) 
{
	if( evt.lbutton() ) {
		// set point
		m_click.px = evt.x();
		m_click.py = evt.y();
		m_drag.px = evt.x();
		m_drag.py = evt.y();

		// range operation
		if( evt.x() < m_graphics.getGraphPositionLeft()
				&& evt.y() > m_graphics.getGraphPositionTop()
				&& evt.y() < m_graphics.getGraphPositionBottom()
		) {	// y range
			m_rangeOp = OP_Y_RANGE;
			return true;
		}
		else if( evt.x() > m_graphics.getGraphPositionLeft()
				&& evt.x() < m_graphics.getGraphPositionRight()
				&& evt.y() > m_graphics.getGraphPositionBottom()
		) {	// x range
			m_rangeOp = OP_X_RANGE;
			return true;
		}
		else {
			m_rangeOp = OP_NONE;
		}
	}
	else if( evt.rbutton() ) {
		// pop range
		if( evt.x() < m_graphics.getGraphPositionLeft()
				&& evt.y() > m_graphics.getGraphPositionTop()
				&& evt.y() < m_graphics.getGraphPositionBottom()
		) {	// y range
			if( m_yRange.size() > 0 ) {
				m_yRange.pop();
				return true;
			}
		}
		else if( evt.x() > m_graphics.getGraphPositionLeft()
				&& evt.x() < m_graphics.getGraphPositionRight()
				&& evt.y() > m_graphics.getGraphPositionBottom()
		) {	// x range
			if( m_xRange.size() > 0 ) {
				m_xRange.pop();
				return true;
			}
		}
	}

	return false;
}

// on mouse button up
bool PeaksCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	// return value
	bool ret = false;
 
	// drag point
	m_drag.px = evt.x();
	m_drag.py = evt.y();

	// set range
	if( evt.lbutton() ) {
		if( m_rangeOp != OP_NONE ) {
			// positions
			double x0 = double();
			double y0 = double();
			double x1 = double();
			double y1 = double();

			m_graphics.transformPositionToData( (double)m_click.px, (double)m_click.py, &x0, &y0 );
			m_graphics.transformPositionToData( (double)m_drag.px, (double)m_drag.py, &x1, &y1 );

			// range
			double midX = ( x0 + x1 ) / 2.0;
			double minX = std::min( x0, x1 );
			double maxX = std::max( x0, x1 );

			minX = std::max( minX, m_graphics.getMinX() );
			minX = std::min( midX - 0.005, minX );
			minX = std::max( 0.0, minX );
			maxX = std::min( maxX, m_graphics.getMaxX() );
			maxX = std::max( maxX, midX + 0.01 );

			double midY = ( y0 + y1 ) / 2.0;
			double minY = std::min( y0, y1 );
			double maxY = std::max( y0, y1 );

			minY = std::max( minY, m_graphics.getMinY() );
			minY = std::min( midY - 0.005, minY );
			minY = std::max( 0.0, minY );
			maxY = std::min( maxY, m_graphics.getMaxY() );
			maxY = std::max( maxY, midY + 0.01 );

			// push
			if( m_rangeOp == OP_X_RANGE ) {	// x range
				if( m_click.px != m_drag.py ) {
					m_xRange.push( std::make_pair( minX, maxX ) );
					ret = true;
				}
			}
			else if( m_rangeOp == OP_Y_RANGE ) {	// y range
				if( m_click.py != m_drag.py ) {
					m_yRange.push( std::make_pair( minY, maxY ) );
					ret = true;
				}
			}
		}

		m_rangeOp = OP_NONE;
	}

	return ret;
}

// on mouse double click
bool PeaksCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	// clear ranges
	if( evt.lbutton() ) {
		if( evt.x() < m_graphics.getGraphPositionLeft()
				&& evt.y() > m_graphics.getGraphPositionTop()
				&& evt.y() < m_graphics.getGraphPositionBottom()
		) {	// y range
			if( m_yRange.size() > 0 ) {
				while( m_yRange.size() > 0 ) {
					m_yRange.pop();
				}
				return true;
			}
		}
		else if( evt.x() > m_graphics.getGraphPositionLeft()
				&& evt.x() < m_graphics.getGraphPositionRight()
				&& evt.y() > m_graphics.getGraphPositionBottom()
		) {	// x range
			if( m_xRange.size() > 0 ) {
				while( m_xRange.size() > 0 ) {
					m_xRange.pop();
				}
				return true;
			}
		}
	}

	return false;
}

// on mouse curesor move
bool PeaksCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	// set drag point
	if( evt.lbutton() ) {
		m_drag.px = evt.x();
		m_drag.py = evt.y();

		return ( m_rangeOp != OP_NONE );
	}

	return false;
}

// on prepare draw
void PeaksCanvas::onPrepareDraw( kome::window::DCGraphics& g ) {
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// xy data
	kome::core::XYData* xyData = mgr.getActiveRecordPeaks();
	if( xyData == NULL ) {
		return;
	}

	// font
	kome::img::Font font( 8 );
	g.setFont( font );

	// range
	double minX = xyData->getMinX();
	double maxX = xyData->getMaxX();
	double midX = ( minX + maxX ) / 2.0;
	double xRange = ( maxX - minX ) * 1.05 / 2.0;
	minX = std::max( 0.0, midX - xRange );
	maxX = std::max( minX + 0.01, midX + xRange );
	if( m_xRange.size() > 0 ) {
		minX = m_xRange.top().first;
		maxX = m_xRange.top().second;
	}

	double minY = 0.0;
	double maxY = 0.01;

	if( m_yRange.size() == 0 ) {
		int startIdx = xyData->searchIndex( minX );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 1;
		}

		int endIdx = xyData->searchIndex( maxX );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 2;
		}

		for( int i = startIdx; i <= endIdx; i++ ) {
			maxY = std::max( maxY, xyData->getY( i ) );
		}
	}
	else {
		minY = m_yRange.top().first;
		maxY = m_yRange.top().second;
	}

	m_graphics.setDisplayedRange( minX, maxX, minY, maxY );

	// prepare margin
	int leftMargin = TEXT_SPACE + SCALE_LENGTH;
	int topMargin = g.getGraphPositionTop();
	int rightMargin = 20;
	int bottomMargin = 2 * ( TEXT_SPACE + SCALE_LENGTH );

	// get title size
	int titleWidth = 0;
	int titleHeight = 0;
	g.getTextSize( "|title|", &titleWidth, &titleHeight );

	leftMargin += ( titleHeight + TEXT_SPACE );
	bottomMargin += titleHeight;

	// get y scale information
	leftMargin += kome::img::GraphTool::getMaxWidthOfYLabels( g, MIN_SCALE_SPACE );
	bottomMargin += titleHeight;

	// set position
	g.setGraphPosition(
		leftMargin,
		topMargin,
		g.getWidth() - rightMargin,
		g.getHeight() - bottomMargin
	);
	g.setGraphMarginTop( 10 );
}

// on draw window background
void PeaksCanvas::onDrawWindowBg( kome::window::DCGraphics& g ) {
	// selected range
	if( m_rangeOp != OP_NONE ) {
		// color
		COLOR oldColor = g.getColor();
		g.setColor( kome::img::ColorTool::GRAY );

		// fill rect
		int px0 = std::min( m_click.px, m_drag.px );
		px0 = std::max( px0, g.getGraphPositionLeft() );

		int px1 = std::max( m_click.px, m_drag.px );
		px1 = std::min( px1, g.getGraphPositionRight() );

		int py0 = std::min( m_click.py, m_drag.py );
		py0 = std::max( py0, g.getGraphPositionTop() );

		int py1 = std::max( m_click.py, m_drag.py );
		py1 = std::min( py1, g.getGraphPositionBottom() );

		if( m_rangeOp == OP_X_RANGE ) {
			py0 = g.getGraphPositionBottom();
			py1 = (int)g.getHeight();
		}
		else if( m_rangeOp == OP_Y_RANGE ) {
			px0 = 0;
			px1 = g.getGraphPositionLeft();
		}

		g.fillRect( px0, py0, px1 - px0, py1 - py0 );

		// restore color
		g.setColor( oldColor );
	}
}

// on draw graph background 
void PeaksCanvas::onDrawGraphBg( kome::window::DCGraphics& g ) {
}

// on draw profile
void PeaksCanvas::onDrawProfile( kome::window::DCGraphics& g ) {
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	
	// draw peaks(ì‡ïîÇ≈êFÇÃêÿÇËë÷Ç¶ëŒâûçœÇ›)
	mgr.drawRecordPeaks( g );
}

// on draw graph foreground
void PeaksCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
}

// on draw graph foreground
void PeaksCanvas::onDrawWindowFg( kome::window::DCGraphics& g ) {
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// xy data
	kome::core::XYData* xyData = mgr.getActiveRecordPeaks();
	if( xyData == NULL ) {
		return;
	}

	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );

	// draw rect
	kome::img::GraphTool::drawGraphRect( g );

	// draw scale
	kome::img::GraphTool::drawXScale( g, 10, 6, 5, false );
	kome::img::GraphTool::drawYScale( g, 10, 6, 5, false );

	// draw x title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_BOTTOM );
	g.drawText(
		"m/z",
		( g.getGraphPositionLeft() + g.getGraphPositionRight() ) / 2,
		g.getHeight() - 2 * TEXT_SPACE,
		0
	);

	// draw y title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.drawText(
		"Int.",
		TEXT_SPACE,
		( g.getGraphPositionTop() + g.getGraphPositionBottom() ) / 2,
		90
	);

	// draw selected range
	if( m_rangeOp != OP_NONE ) {
		int px0 = std::min( m_click.px, m_drag.px );
		px0 = std::max( px0, g.getGraphPositionLeft() );

		int px1 = std::max( m_click.px, m_drag.px );
		px1 = std::min( px1, g.getGraphPositionRight() );

		int py0 = std::min( m_click.py, m_drag.py );
		py0 = std::max( py0, g.getGraphPositionTop() );

		int py1 = std::max( m_click.py, m_drag.py );
		py1 = std::min( py1, g.getGraphPositionBottom() );

		// draw line
		g.setDrawStyle( kome::img::Graphics::STYLE_DOT );

		if( m_rangeOp == OP_X_RANGE ) {
			py0 = g.getGraphPositionTop();
			py1 = (int)g.getHeight();

			g.drawLine( px0, py0, px0, py1 );
			if( px0 != px1 ) {
				g.drawLine( px1, py0, px1, py1 );
			}
		}
		else if( m_rangeOp == OP_Y_RANGE ) {
			px0 = 0;
			px1 = g.getGraphPositionRight();

			g.drawLine( px0, py0, px1, py0 );
			if( py0 != py1 ) {
				g.drawLine( px0, py1, px1, py1 );
			}
		}

		g.setDrawStyle( kome::img::Graphics::STYLE_SOLID );
	}

	// restore color
	g.setColor( oldColor );
}
