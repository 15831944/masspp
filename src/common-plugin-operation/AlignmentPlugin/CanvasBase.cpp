/**
 * @file CanvasBase.cpp
 * @brief implements of CanvasBase class
 *
 * @author S.Tanaka
 * @date 2007.08.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CanvasBase.h"
#include "AlignmentManager.h"

#include <float.h>


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_GRAPH_MARGIN_LEFT		0
#define DEFAULT_GRAPH_MARGIN_TOP		5
#define DEFAULT_GRAPH_MARGIN_RIGHT		0
#define DEFAULT_GRAPH_MARGIN_BOTTOM		0
#define DEFAULT_MARGIN_LEFT			   10
#define DEFAULT_MARGIN_TOP			   10
#define DEFAULT_MARGIN_RIGHT		   20
#define DEFAULT_MARGIN_BOTTOM		   25

#define SPACE							5
#define SCALE_PARAM					   10
#define SCALE_LENGTH					2

#define MIN_RANGE					 0.01


BEGIN_EVENT_TABLE( CanvasBase, kome::window::Canvas )
END_EVENT_TABLE()


// constructor
CanvasBase::CanvasBase( wxWindow* parent, const int width, const int height )
		: kome::window::Canvas( parent, wxID_ANY, width, height ) {
	m_xLabelFlg = false;
	m_yLabelFlg = false;
	m_xGridFlg = false;
	m_yGridFlg = false;
	m_posFlg = false;
	m_baseZeroFlg = false;
	m_title = "";
}

// destructor
CanvasBase::~CanvasBase() {
}

// get chromatograms
void CanvasBase::getChromatograms(
		std::vector< kome::objects::Chromatogram* >& chroms,
		std::vector< COLOR >&  colors
) {
}

// set graph range
void CanvasBase::setGraphRange( std::vector< kome::objects::Chromatogram* >& chroms ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// get x range
	double minX = mgr.getStart();
	double maxX = mgr.getEnd();
	if( minX < 0.0 || maxX < 0.0 ) {
		minX = 0.0;
		maxX = 0.0;
		for( unsigned int i = 0; i < chroms.size(); i++ ) {
			// xy data
			kome::objects::Chromatogram* chrom = chroms[ i ];
			kome::core::XYData* xyData = chrom->getXYData();

			// min x
			double tmpX = xyData->getMinX();
			if( i == 0 || tmpX < minX ) {
				minX = tmpX;
			}

			// max x
			tmpX = xyData->getMaxX();
			if( i == 0 || tmpX > maxX ) {
				maxX = tmpX;
			}
		}
	}

	if( maxX - minX < MIN_RANGE ) {
		double midX = ( minX + maxX ) / 2.0;
		minX = midX - MIN_RANGE / 2.0;
		maxX = midX + MIN_RANGE / 2.0;
	}

	// get y range
	double maxY = 0.0;
	double minY = m_baseZeroFlg ? 0.0 : FLT_MAX;
	for( unsigned int i = 0; i < chroms.size(); i++ ) {
		// xy data
		kome::objects::Chromatogram* chrom = chroms[ i ];
		kome::core::XYData* xyData = chrom->getXYData();

		// start index
		int startIdx = xyData->searchIndex( minX );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 2;
			startIdx = std::max( startIdx, 0 );
		}

		// end index
		int endIdx = xyData->searchIndex( maxX );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 1;
			endIdx = std::min( endIdx, (int)xyData->getLength() - 1 );
		}

		// y
		for( int j = startIdx; j <= endIdx; j++ ) {
			double y = xyData->getY( j );

			maxY = MAX( y, maxY );
			if( !m_baseZeroFlg ) {
				minY = MIN( y, minY );
			}
		}
	}

	if( minY > maxY ) {
		minY = 0.0;
	}
	if( maxY - minY < MIN_RANGE ) {
		maxY = minY + MIN_RANGE;
	}

	// set range
	m_graphics.setDisplayedRange( minX, maxX, m_baseZeroFlg ? 0.0 : minY, maxY );
}

// on draw
void CanvasBase::onDraw( wxDC& dc ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// chromatograms
	std::vector< kome::objects::Chromatogram* > chroms;
	std::vector< COLOR > colors;
	getChromatograms( chroms, colors );
	if( chroms.size() == 0 ) {
		return;
	}

	// set DC
	m_graphics.setDC( &dc );
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	// set range
	setGraphRange( chroms );

	// position
	if( m_yLabelFlg ) {
		int labelWidth = kome::img::GraphTool::getMaxWidthOfYLabels( m_graphics, SCALE_PARAM );
		m_graphics.setGraphPosition(
			DEFAULT_MARGIN_LEFT + labelWidth,
			DEFAULT_MARGIN_TOP,
			(int)m_graphics.getWidth() - DEFAULT_MARGIN_RIGHT,
			(int)m_graphics.getHeight() - DEFAULT_MARGIN_BOTTOM
		);
	}

	// scale
	if( m_xLabelFlg ) {
		kome::img::GraphTool::drawXScale( m_graphics, SCALE_PARAM, SCALE_LENGTH, SPACE, m_xGridFlg );
	}
	if( m_yLabelFlg ) {
		kome::img::GraphTool::drawYScale( m_graphics, SCALE_PARAM, SCALE_LENGTH, SPACE, m_yGridFlg );
	}

	// memory dc
	wxMemoryDC memDc;
	wxBitmap bitmap( m_graphics.getGraphWidth(), m_graphics.getGraphHeight() );
	memDc.SelectObject( bitmap );
	memDc.SetBackground( *wxWHITE_BRUSH );
	memDc.Clear();

	m_graphics.setDC( &memDc );
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_BUFFERED_AND );
	m_graphics.setTranslate(
		- m_graphics.getGraphPositionLeft(),
		- m_graphics.getGraphPositionTop()
	);

	// profile
	COLOR oldColor = m_graphics.getColor();
	m_graphics.setUnusedDefaultColor( true );

	for( unsigned int i = 0; i < chroms.size(); i++ ) {
		COLOR color = colors[ i ];
		if( color != kome::img::ColorTool::WHITE ) {
			m_graphics.setColor( color );
			kome::plugin::PluginCallTool::drawChromatogram( *( chroms[ i ] ), m_graphics, this );
		}
	}

	m_graphics.setColor( oldColor );

	// copy
	dc.Blit(
		m_graphics.getGraphPositionLeft(),
		m_graphics.getGraphPositionTop(),
		m_graphics.getGraphWidth(),
		m_graphics.getGraphHeight(),
		&memDc,
		0,
		0,
		wxAND
	);

	// restore
	m_graphics.setDC( &dc );
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );
	m_graphics.resetTransform();

	// title
	if (!m_title.empty())
	{
		int nWidth = 0;
		int nHeight = 0;

		m_graphics.getTextSize(m_title.c_str(), &nWidth, &nHeight);
		m_graphics.drawText(m_title.c_str(), m_graphics.getGraphPositionRight() - nWidth - 4, m_graphics.getGraphPositionTop() + 2);
	}

	// position
	if( m_posFlg ) {
		int pos = mgr.getCursorPos();
		if( pos > m_graphics.getGraphPositionLeft() && pos < m_graphics.getGraphPositionRight() ) {
			int posLine[] = {
				pos, m_graphics.getGraphPositionTop(),
				pos, m_graphics.getGraphPositionBottom()
			};
			m_graphics.drawLines( posLine, 2 );
		}
	}

	// range
	int startPos = mgr.getDragStartPos();
	int endPos = mgr.getDragEndPos();
	if( startPos >= m_graphics.getGraphPositionLeft() && startPos <= m_graphics.getGraphPositionRight() ) {
		// left, right
		int left = MIN( startPos, endPos );
		left = CLAMP( left, m_graphics.getGraphPositionLeft(), m_graphics.getGraphPositionRight() );

		int right = MAX( startPos, endPos );
		right = CLAMP( right, m_graphics.getGraphPositionLeft(), m_graphics.getGraphPositionRight() );

		// draw range
		int posLine[] = {
			left, m_graphics.getGraphPositionTop(),
			left, m_graphics.getGraphPositionBottom()
		};
		if( right > left ) {
			m_graphics.drawLines( posLine, 2 );
		}

		posLine[ 0 ] = right;
		posLine[ 2 ] = right;
		m_graphics.drawLines( posLine, 2 );
	}

	// rect
	kome::img::GraphTool::drawGraphRect( m_graphics );

	// memory dc
	memDc.SelectObject( wxNullBitmap );
}

// on size
void CanvasBase::onSize( const unsigned int width, const unsigned int height ) {
	// set size
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

// on mouse button down
bool CanvasBase::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int px = evt.x();
	int py = evt.y();

	// button down
	bool refresh = false;
	if( evt.lbutton() ) {   // start to drag
		if( px >= m_graphics.getGraphPositionLeft() && px <= m_graphics.getGraphPositionRight() ) {
			mgr.setCursorPos( 0 );
			mgr.setDragStartPos( px );
			mgr.setDragEndPos( px );
			refresh = true;
		}
	}
	else if( evt.rbutton() ) {		// pop range
		if( px >= m_graphics.getGraphPositionLeft() && px <= m_graphics.getGraphPositionRight()
				&& py >= m_graphics.getGraphPositionBottom() ) {
			mgr.popRange();
			refresh = true;
		}
	}		

	return refresh;
}

// button up
bool CanvasBase::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int px = evt.x();
	int py = evt.y();

	// button up
	bool refresh = false;
	if( evt.lbutton() ) {   // stop dragging
		// position
		int startPos = mgr.getDragStartPos();
		int endPos = px;

		mgr.setDragStartPos( 0 );
		mgr.setDragEndPos( 0 );
		if( m_posFlg ) {
			mgr.setCursorPos( px );
			refresh = true;
		}
		if( px >= m_graphics.getGraphPositionLeft()
				&& px <= m_graphics.getGraphPositionRight()
				&& py >= m_graphics.getGraphPositionTop()
				&& py <= m_graphics.getGraphPositionBottom()
		) {
			if( startPos >= m_graphics.getGraphPositionLeft() && startPos <= m_graphics.getGraphPositionRight() ) {
				// left, right
				int left = MIN( startPos, endPos );
				left = CLAMP( left, m_graphics.getGraphPositionLeft(), m_graphics.getGraphPositionRight() );
				int right = MAX( startPos, endPos );
				right = CLAMP( right, m_graphics.getGraphPositionLeft(), m_graphics.getGraphPositionRight() );

				// range
				if( left < right && m_graphics.getMaxX() - m_graphics.getMinX() >= 1.1 * MIN_RANGE ) {
					double y = double();
					double minX = double();
					double maxX = double();

					m_graphics.transformPositionToData( (double)left - 0.5, 0.0, &minX, &y );
					minX = CLAMP( minX, m_graphics.getMinX(), m_graphics.getMaxX() );

					m_graphics.transformPositionToData( (double)right + 0.5, 0.0, &maxX, &y );
					maxX = CLAMP( maxX, m_graphics.getMinX(), m_graphics.getMaxX() );

					if( maxX - minX < MIN_RANGE ) {
						double midX = ( minX + maxX ) / 2.0;
						minX = midX - MIN_RANGE / 2.0;
						maxX = midX + MIN_RANGE / 2.0;
					}

					// push range
					mgr.pushRange( minX, maxX );
				}
				refresh = true;
			}
		}
	}

	return refresh;
}

// double click
bool CanvasBase::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int py = evt.y();
	int px = evt.x();

	// double click
	bool refresh = false;
	if( evt.lbutton() ) {   // clear range
		if(  px >= m_graphics.getGraphPositionLeft()
			&& px <= m_graphics.getGraphPositionRight()
			&& py >= m_graphics.getGraphPositionTop()
			&& py <= m_graphics.getGraphPositionBottom() ) {
	
			mgr.clearRange();
			refresh = true;
		}
	}

	// refresh
	return refresh;
}

// on mouse move
bool CanvasBase::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int px = evt.x();

	// move
	bool refresh = false;
	int startPos = mgr.getDragStartPos();

	if( evt.lbutton() && startPos >= m_graphics.getGraphPositionLeft()
			&& startPos <= m_graphics.getGraphPositionRight() ) {   // dragging
		mgr.setDragEndPos( px );
		refresh = true;
	}
	else if( m_posFlg ) {
		mgr.setCursorPos( px );
		refresh = true;
	}

	// refresh
	return refresh;
}

// enter
bool CanvasBase::onMouseCursorEnter( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int px = evt.x();

	// enter
	bool refresh = false;
	if( m_posFlg ) {
		mgr.setCursorPos( px );
		refresh = true;
	}

	// refresh
	return refresh;
}

// leave
bool CanvasBase::onMouseCursorLeave( kome::evt::MouseEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	//position
	int px = evt.x();

	// enter
	bool refresh = false;
	if( m_posFlg ) {
		mgr.setCursorPos( 0 );
		refresh = true;
	}

	// refresh
	return refresh;
}
