/**
 * @file OverlappingChromatogramsCanvasEx.cpp
 * @brief implements of Chromatogram Canvas class
 *
 * @author S.Sugo
 * @date 2012.02.24
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingChromatogramsCanvasEx.h"
#include "OverlappingCanvasFunctions.h"	// Context Menu // toggleOverlappingDialog() call用

#include <list>

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PLUGIN_NAME	"Overlapping Canvas"	// @date 2011.06.20 <Add> M.Izumi
#define ICON_NAME	"overlapping_dlg"		// @date 2011.06.20 <Add> M.Izumi

#define UNSELECT_MENU_ID			10000	// @date 2011.10.26 <Add> M.Izumi

// for context menu
#define ID_POPUP_MENU_OVERLAPPING_SETTINGS	60003
#define STR_POPUP_MENU_OVERLAPPING_SETTINGS	"Overlapping Settings"


// @date 2012.02.24 sugo add -->
#define MINIMUM_DRAW_SIZE		30
#define MIN_SCALE_SPACE			10
#define SCALE_LENGTH			6
#define TEXT_SPACE				5

// π
const double	g_dPi		= (3.1415926535897932384626433832795);

// π/180
const double	g_dPiAng	= (1.7453292519943295769236907684886e-2);
// @date 2012.02.24 <-- sugo

BEGIN_EVENT_TABLE( OverlappingChromatogramsCanvasEx, ChromatogramCanvas )
	EVT_MENU( UNSELECT_MENU_ID, OverlappingChromatogramsCanvasEx::onUnSelect )
	EVT_CONTEXT_MENU( OverlappingChromatogramsCanvasEx::onContextMenu )	// for context menu	// @date 2012/12/07 <Add> OKADA
	EVT_MENU( ID_POPUP_MENU_OVERLAPPING_SETTINGS, OverlappingChromatogramsCanvasEx::onOverlappingSettings )	// for context menu
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, OverlappingChromatogramsCanvas::onMenuPopup )	//　@date 2012/11/21
	EVT_UPDATE_UI( ID_POPUP_MENU_OVERLAPPING_SETTINGS, onUpdatePopupMenu )
END_EVENT_TABLE()


// static member
std::set< OverlappingChromatogramsCanvasEx* > OverlappingChromatogramsCanvasEx::m_canvasSet;


// constructor
OverlappingChromatogramsCanvasEx::OverlappingChromatogramsCanvasEx( wxWindow* parent, const int width, const int height )
		: ChromatogramCanvas( parent, width, height, NULL ) {
	// initialize
	m_canvasSet.insert( this );

	m_focusedChrom = NULL;

	// CanvasStyle 設定 @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS | OVERLAPPING;

	// @date 2012.02.24 sugo add -->
	m_offset = 0;
	m_angle = 0;
	m_bAutoYScale = false;
	m_bChangeSize = true;
	m_nLengthX = 0;
	m_nLengthY = 0;
	m_nSlantX = 0;
	m_nSlantY = 0;
	m_nZeroPointX = 0;
	m_nZeroPointY = 0;
	// @date 2012.02.24 <-- sugo 

	m_colorIdx = 0;
	m_colors.resize( 9 );
	m_colors[ 0 ] = kome::img::ColorTool::getColor( 0x00, 0x00, 0xff );
	m_colors[ 1 ] = kome::img::ColorTool::getColor( 0x00, 0x80, 0x00 );
	m_colors[ 2 ] = kome::img::ColorTool::getColor( 0xff, 0xa5, 0x00 );
	m_colors[ 3 ] = kome::img::ColorTool::getColor( 0x80, 0x00, 0x80 );
	m_colors[ 4 ] = kome::img::ColorTool::getColor( 0x00, 0xbf, 0xff );
	m_colors[ 5 ] = kome::img::ColorTool::getColor( 0x80, 0x80, 0x00 );
	m_colors[ 6 ] = kome::img::ColorTool::getColor( 0xff, 0x00, 0xff );
	m_colors[ 7 ] = kome::img::ColorTool::getColor( 0x00, 0xff, 0x7f );
	m_colors[ 8 ] = kome::img::ColorTool::getColor( 0xa5, 0x2a, 0x2a );

	m_index = -1;
}

// destructor
OverlappingChromatogramsCanvasEx::~OverlappingChromatogramsCanvasEx() {
	m_canvasSet.erase( this );

	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		delete m_overlappingChroms[ i ].chrom;
	}
	m_overlappingChroms.clear();
}

// clears selected chromatograms
void OverlappingChromatogramsCanvasEx::clearSelectedChroms() {
	// clear
	m_selectedChroms.clear();

	// active object
	updateActiveObject();
}

// add selected chromatogram
void OverlappingChromatogramsCanvasEx::addSelectedChrom( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// search
	int idx = searchSelectedChrom( chrom );
	if( idx >= 0 ) {
		return;
	}

	// add
	m_selectedChroms.push_back( chrom );

	// active object
	updateActiveObject();
}

// remove selected chromatogram
void OverlappingChromatogramsCanvasEx::removeSelectedChrom( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// remove
	int idx = searchSelectedChrom( chrom );
	if( idx >= 0 ) {
		m_selectedChroms.erase( m_selectedChroms.begin() + idx );
	}

	// active object
	updateActiveObject();
}

// get the number of selected chromatograms
unsigned int OverlappingChromatogramsCanvasEx::getNumberOfSelectedChroms() {
	return m_selectedChroms.size();
}

// get selected chromatogram
kome::objects::Chromatogram* OverlappingChromatogramsCanvasEx::getSelectedChrom( const unsigned int idx ) {
	if( idx >= m_selectedChroms.size() ) {
		return NULL;
	}
	return m_selectedChroms[ idx ];
}

// update active object
void OverlappingChromatogramsCanvasEx::updateActiveObject() {
	// active object
	kome::objects::Variant obj;

	if( m_selectedChroms.size() == 1 ) {	// select
		obj.type = kome::objects::Variant::CHROMATOGRAM;
		obj.prim.pt = m_selectedChroms[ 0 ];
	}
	else {	// unselect
		obj.type = kome::objects::Variant::UNKNOWN;
		obj.prim.pt = NULL;
	}

	setActiveObject( obj );
}

// search selected chromatogram
int OverlappingChromatogramsCanvasEx::searchSelectedChrom( kome::objects::Chromatogram* chrom ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_selectedChroms.size() && idx < 0; i++ ) {
		if( chrom->getOrgChromatogram() == m_selectedChroms[ i ]->getOrgChromatogram() ) {
			idx = (int)i;
		}
	}

	return idx;
}

// get base chromatogram
kome::objects::Chromatogram* OverlappingChromatogramsCanvasEx::getBaseChromatogram() {
	return &m_dummy;
}

// on add chromatogram
kome::objects::Chromatogram* OverlappingChromatogramsCanvasEx::onAddChromatogram( kome::objects::Chromatogram* chrom ) {
	// search
	int idx = -1;

	// @date 2011/08/04 <Add> OKADA ->
	for( unsigned int i = 0; i < getNumberOfChromatograms() && idx < 0; i++ ) {
		if( ((getChromatogram(i))->getOrgChromatogram()) == (chrom->getOrgChromatogram()) &&
			chrom->getOperationFlag() ) {
			idx = (int)i;
		}
	}
	// @date 2011/08/04 <Add> OKADA <-

	// check the chromatogram
	if( chrom == NULL || idx >= 0 ) {
		return NULL;
	}

	// color
	COLOR color = m_colors[ m_colorIdx ];
	m_colorIdx = ( m_colorIdx + 1 ) % (int)m_colors.size();

	// add
	m_overlappingChroms.resize( m_overlappingChroms.size() + 1 );
	m_overlappingChroms.back().chrom = new kome::operation::TransformedChromatogram( *chrom, chrom->getOperationFlag() );
	m_overlappingChroms.back().color = color;
	m_overlappingChroms.back().flg = true;
	m_dummy.addChromatogram( m_overlappingChroms.back().chrom );
	kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );

	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( getBaseChromatogram() );	// @date 2011.02.10 <Add> M.Izumi

	// update
	update();

	return m_overlappingChroms.back().chrom;
}

// on remove chromatogram
void OverlappingChromatogramsCanvasEx::onRemoveChromatogram( kome::objects::Chromatogram* chrom ) {
	// search
	int idx = -1;
	for( int i = 0; i < (int)m_overlappingChroms.size() && idx < 0; i++ ) {
		if( m_overlappingChroms[ i ].chrom == chrom
				|| m_overlappingChroms[ i ].chrom->getOrgChromatogram() == chrom->getOrgChromatogram() ) {
			idx = i;
		}
	}

	// remove
	if( idx >= 0 ) {
		m_dummy.removeChromatogram( chrom );
		kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );

		delete m_overlappingChroms[ idx ].chrom;
		m_overlappingChroms.erase( m_overlappingChroms.begin() + idx );
	}
	removeSelectedChrom( chrom );
	
	// update
	update();
}

// set color
void OverlappingChromatogramsCanvasEx::setColor( kome::objects::Chromatogram* chrom, const COLOR color ) {
	// search
	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		if( chrom == NULL
				|| m_overlappingChroms[ i ].chrom == chrom
				|| m_overlappingChroms[ i ].chrom->getOrgChromatogram() == chrom->getOrgChromatogram() ) {
			// set color
			m_overlappingChroms[ i ].color = color;
		}
	}
}

// get color
COLOR OverlappingChromatogramsCanvasEx::getColor( const unsigned int index ) {
	if( index >= m_overlappingChroms.size() ) {
		return m_colors[ 0 ];
	}

	return m_overlappingChroms[ index ].color;
}

// update canvas
void OverlappingChromatogramsCanvasEx::update() {
	// update canvas
	m_dummy.deleteXYData();
	kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );
	WindowTool::refresh();
}

// on mouse button down
bool OverlappingChromatogramsCanvasEx::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	// object
	kome::objects::Variant obj = getActiveObject();

	// select chromatogram
	if( evt.x() >= m_graphics.getGraphPositionLeft()
			&& evt.x() <= m_graphics.getGraphPositionRight()
			&& evt.y() >= m_graphics.getGraphPositionTop()
			&& evt.y() <= m_graphics.getGraphPositionBottom()
			&& m_focusedChrom != NULL
			&& obj.prim.pt != m_focusedChrom
	) {
		clearSelectedChroms();
		addSelectedChrom( m_focusedChrom );
		
		// @date 2013.03.14 <Add> M.Izumi
		for( unsigned int i=0; i < m_overlappingChroms.size(); i++ ){
			// information
			ChromInfo* info = &( m_overlappingChroms[ i ] );
			if( m_focusedChrom->getOrgChromatogram() == info->chrom->getOrgChromatogram() ){
				m_index = i;
			}
		}


		setCallFunctionSelectedChromatogram( m_focusedChrom );	//  @date 2011.10.26 <Add> M.Izumi

		return true;
	}

	return ChromatogramCanvas::onMouseButtonDown( evt );
}

// on mouse cursor move
bool OverlappingChromatogramsCanvasEx::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	if( ChromatogramCanvas::onMouseCursorMove( evt ) ) {
		return true;
	}
	else if( !evt.lbutton() && !evt.rbutton() ) {
		return true;
	}
	return false;
}

// on draw profile
void OverlappingChromatogramsCanvasEx::onDrawProfile( kome::window::DCGraphics& g ) {

	// forcus overlapping chromatogram num
	int nNumForcusChrom = -1;

	// draw profiles
	int nChromNum = m_overlappingChroms.size();
	int nParameter = (nChromNum > 2 ? (nChromNum - 1) : 1);
	m_focusedChrom = NULL;
	int focusLevel = 0;
	kome::objects::Chromatogram* focusedChrom = NULL;

	// original position 
	int nLeft = g.getGraphPositionLeft();
	int nRight = g.getGraphPositionRight();
	int nTop = g.getGraphPositionTop();
	int nBottom = g.getGraphPositionBottom();
	double dMinY = g.getMinY();
	double dMaxY = g.getMaxY();

	if( m_bAutoYScale ){
		g.setMinY(0.0);
	}

	for( int i = 0; i < nChromNum; i++ ) {	// each chromatograms
		// information
		ChromInfo* info = &( m_overlappingChroms[ i ] );

		// draw
		if( info->flg ) {
			// shift the graphic position
			g.setGraphPosition(m_nZeroPointX + i * m_nSlantX / nParameter,
							   m_nZeroPointY - m_nLengthY - i * m_nSlantY / nParameter,
							   m_nZeroPointX + m_nLengthX + i * m_nSlantX / nParameter,
							   m_nZeroPointY - i * m_nSlantY / nParameter);
			if( m_bAutoYScale ){
				g.setMaxY(GetMaxY(info->chrom, g.getMinX(), g.getMaxX()));
			}

			
			g.setColor( info->color );

			// 色、太さのDefault設定を反映させる為の処理追加 @date 2013.04.26 <Add> M.izumi
			g.setUnusedDefaultColor( true );
			g.setUnusedDefaultThickness( true );

			// draw profile
			g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
										  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
										  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
			onDrawChromatogramProfile( g, *( info->chrom ) );
			g.getDC()->DestroyClippingRegion();

			// check focused
			if( focusLevel < 2
					&& m_cursorPos.px >= g.getGraphPositionLeft()
					&& m_cursorPos.px <= g.getGraphPositionRight()
					&& m_cursorPos.py >= g.getGraphPositionTop()
					&& m_cursorPos.py <= g.getGraphPositionBottom()
			) {
				// get color
				COLOR c = g.getPixel( m_cursorPos.px, m_cursorPos.py );

				if( c != kome::img::ColorTool::WHITE ) {
					focusedChrom = info->chrom;
					nNumForcusChrom = i;
					m_focusedChrom = info->chrom->getOrgChromatogram();
					
					focusLevel = 2;
				}

				if( focusLevel < 1 ) {
					int searchPos[] = {
						m_cursorPos.px - 1, m_cursorPos.py,
						m_cursorPos.px + 1, m_cursorPos.py,
						m_cursorPos.px,	 m_cursorPos.py - 1,
						m_cursorPos.px,	 m_cursorPos.py + 1
					};

					int num = sizeof( searchPos ) / sizeof( int ) / 2;
					for( int j = 0; j < num && focusLevel < 1; j++ ) {
						int px = searchPos[ j * 2 ];
						int py = searchPos[ j * 2 + 1 ];

						px = CLAMP( px, g.getGraphPositionLeft(), g.getGraphPositionRight() );
						py = CLAMP( py, g.getGraphPositionTop(),  g.getGraphPositionBottom() );

						c = g.getPixel( px, py );

						if( c != kome::img::ColorTool::WHITE ) {
							focusedChrom = info->chrom;
							nNumForcusChrom = i;
							m_focusedChrom = info->chrom->getOrgChromatogram();
							focusLevel = 1;
						}
					}
				}
			}
		}
	}

	// draw selected chromatogram
	for( unsigned int i = 0; i < m_selectedChroms.size(); i++ ) {
		// SPEC 82217 選択波形を強調表示 @date 2011.09.16 <Mod> M.Izumi
		for( int j=0; j < nChromNum; j++ ){
			if( m_selectedChroms[i]->getOrgChromatogram() == m_overlappingChroms[j].chrom->getOrgChromatogram() &&
				m_index == j ){
				g.getDC()->SetPen( wxPen( m_overlappingChroms[ j ].color, 3 ) );
				
				// shift the graphic position
				g.setGraphPosition(m_nZeroPointX + j * m_nSlantX / nParameter,
								   m_nZeroPointY - m_nLengthY - j * m_nSlantY / nParameter,
								   m_nZeroPointX + m_nLengthX + j * m_nSlantX / nParameter,
								   m_nZeroPointY - j * m_nSlantY / nParameter);
				if( m_bAutoYScale ){
					g.setMaxY(GetMaxY(m_selectedChroms[i], g.getMinX(), g.getMaxX()));
				}

				g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
											  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
											  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
				kome::plugin::PluginCallTool::drawChromatogram( *( m_selectedChroms[ i ] ), g, this );
				g.getDC()->DestroyClippingRegion();
			}
		}
	}

	// draw focused chromatogram
	if( focusedChrom != NULL ) {
		// tooltip
		m_tooltip = focusedChrom->getTitle();
		kome::objects::DataGroupNode* group = focusedChrom->getGroup();
		while( group != NULL ) {
			m_tooltip = FMT( "%s - %s", m_tooltip.c_str(), group->getName() );
			group = group->getParentGroup();
		}

		// shift the graphic position
		g.setGraphPosition(m_nZeroPointX + nNumForcusChrom * m_nSlantX / nParameter,
						   m_nZeroPointY - m_nLengthY - nNumForcusChrom * m_nSlantY / nParameter,
						   m_nZeroPointX + m_nLengthX + nNumForcusChrom * m_nSlantX / nParameter,
						   m_nZeroPointY - nNumForcusChrom * m_nSlantY / nParameter);
		if( m_bAutoYScale ){
			g.setMaxY(GetMaxY(focusedChrom, g.getMinX(), g.getMaxX()));
		}

		// draw
		g.setColor( kome::img::ColorTool::YELLOW );
		g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
									  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
									  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
		g.getDC()->DestroyClippingRegion();
		kome::plugin::PluginCallTool::drawChromatogram( *focusedChrom, g, this );
	}

	// after drawing ,reset the graphic position to the original position
	g.setGraphPosition(nLeft, nTop, nRight, nBottom);
	if( m_bAutoYScale ){
		g.setMinY(dMinY);
		g.setMaxY(dMaxY);
	}
	// @date 2012.02.24 <-- sugo
}

// unselect all								   @date 2011.10.26 <Add> M.Izumi
void OverlappingChromatogramsCanvasEx::onUnSelect( wxCommandEvent& evt ){
	clearSelectedChroms();
	setCallFunctionSelectedChromatogram( NULL );
}

// set call function ( selected chromatogram )		@date 2011.10.26 <Add> M.Izumi
void OverlappingChromatogramsCanvasEx::setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom ){
	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "select_chromatogram","overlapping" );
	
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
	
	if( item != NULL ){
		item->getCall()->invoke( &params );
	}

}

// @date 2012.02.24 sugo add -->
// get chromatogram number
int OverlappingChromatogramsCanvasEx::GetOverlappingChromatogramNum( unsigned int nNum ) {
	int nRet = -1;
	if( nNum < m_overlappingChroms.size() ){
		nRet = m_overlappingChroms[ nNum ].num;
	}
	return nRet;
}

// exchange data
void OverlappingChromatogramsCanvasEx::exchange(int nS,int nE){
	ChromInfo temp;
	temp.chrom = m_overlappingChroms[nS].chrom;
	temp.color = m_overlappingChroms[nS].color;
	temp.flg = m_overlappingChroms[nS].flg;
	temp.num = m_overlappingChroms[nS].num;

	m_overlappingChroms[nS].chrom = m_overlappingChroms[nE].chrom;
	m_overlappingChroms[nS].color = m_overlappingChroms[nE].color;
	m_overlappingChroms[nS].flg = m_overlappingChroms[nE].flg;
	m_overlappingChroms[nS].num = m_overlappingChroms[nE].num;

	m_overlappingChroms[nE].chrom = temp.chrom;
	m_overlappingChroms[nE].color = temp.color;
	m_overlappingChroms[nE].flg = temp.flg;
	m_overlappingChroms[nE].num = temp.num;
}

// set 'Auto Y Scale'
void OverlappingChromatogramsCanvasEx::SetAutoYScale( bool scale ){
	m_bAutoYScale = scale;
}

// set offset
void OverlappingChromatogramsCanvasEx::SetOffset( int offset ){
	m_offset = offset;
}

// set angle
void OverlappingChromatogramsCanvasEx::SetAngle( int angle ){
	m_angle = angle;
}

// get 'Auto Y Scale'
bool OverlappingChromatogramsCanvasEx::GetAutoYScale(  ){
	return m_bAutoYScale;
}

// get offset
int OverlappingChromatogramsCanvasEx::GetOffset(  ){
	return m_offset;
}

// get angle
int OverlappingChromatogramsCanvasEx::GetAngle(  ){
	return m_angle;
}

// get MaxY
double OverlappingChromatogramsCanvasEx::GetMaxY( kome::objects::Chromatogram* chrom, const double minX, const double maxX ){
	// xy data
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	kome::core::XYData* xyData = NULL;
	double maxY = 0.01;

	// check the member
	if( chrom != NULL ) {
		xyData = ptMgr.getXYData( chrom );
	}
	if( xyData != NULL ) {
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

	return maxY;
}

// drawing
void OverlappingChromatogramsCanvasEx::onDrawGraphics( wxDC& dc ) {
	// original position 
	int nLeft = m_graphics.getGraphPositionLeft();
	int nRight = m_graphics.getGraphPositionRight();
	int nTop = m_graphics.getGraphPositionTop();
	int nBottom = m_graphics.getGraphPositionBottom();

	// draw window background
	COLOR oldColor = m_graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = m_graphics.getDrawStyle();
	
	// on draw window background
	m_graphics.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
								m_nZeroPointX + m_nLengthX, m_nZeroPointY);
	onDrawWindowBg( m_graphics );
	m_graphics.setGraphPosition(nLeft, nTop, nRight, nBottom);

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

	m_graphics.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
								m_nZeroPointX + m_nLengthX, m_nZeroPointY);
	onDrawGraphBg( m_graphics );
	m_graphics.setGraphPosition(nLeft, nTop, nRight, nBottom);

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

	m_graphics.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
								m_nZeroPointX + m_nLengthX, m_nZeroPointY);
	onDrawGraphFg( m_graphics );
	m_graphics.setGraphPosition(nLeft, nTop, nRight, nBottom);

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

	// after drawing ,reset the graphic position to the original position
	m_graphics.setGraphPosition(nLeft, nTop, nRight, nBottom);
}

// drawing graph foreground
void OverlappingChromatogramsCanvasEx::onDrawWindowFg( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();

	// check the chromatogram
	if( chrom == NULL ){
		return;
	}
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// original position 
	int nLeft = g.getGraphPositionLeft();
	int nRight = g.getGraphPositionRight();
	int nTop = g.getGraphPositionTop();
	int nBottom = g.getGraphPositionBottom();
	int nZeroPointX = (m_nSlantX < 0 ? (nLeft - m_nSlantX) : nLeft);

	g.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
					   m_nZeroPointX + m_nLengthX, m_nZeroPointY);

	// set graphics
	COLOR oldColor = g.getColor();
	kome::img::Font oldFont = g.getFont();

	g.setUnusedDefaultColor( false );
	g.setUnusedDefaultThickness( false );
	kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, FONT_AXIS );
	g.setFont( font );
	if( !g.isUnusedDefaultColor() ){
		g.setColor( kome::img::GraphTool::getColorFromIni( SECTION, COLOR_AXIS, COLOR_DEF_AXIS ));
	}
	if( !g.isUnusedDefaultThickness() ){
		g.setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION, WIDTH_AXIS ) );	
	}

	// draw rect
	// rect
	int rectPoints[] = {
		g.getGraphPositionLeft(),  g.getGraphPositionTop(),
		g.getGraphPositionBottom(),g.getGraphPositionRight()
	};

	// draw
	g.drawLine(nZeroPointX, nBottom - m_nLengthY, nZeroPointX, nBottom);
	g.drawLine(nZeroPointX, nBottom, nZeroPointX + m_nLengthX, nBottom);
	g.drawLine(nZeroPointX + m_nSlantX, nBottom - m_nSlantY, nZeroPointX + m_nLengthX + m_nSlantX, nBottom - m_nSlantY);
	g.drawLine(nZeroPointX, nBottom, nZeroPointX + m_nSlantX, nBottom - m_nSlantY);
	g.drawLine(nZeroPointX + m_nLengthX, nBottom, nZeroPointX + m_nLengthX + m_nSlantX, nBottom - m_nSlantY);

	// draw x title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_BOTTOM );

	kome::objects::DataGroupNode* group = chrom->getGroup();

	g.drawText(
		( group == NULL ? "RT" : group->getChromXTitle() ),
		( g.getGraphPositionLeft() + g.getGraphPositionRight() ) / 2,
		g.getHeight() - 2 * TEXT_SPACE,
		0
	);

	// draw y title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.drawText(
		( group == NULL ? "intensity" : group->getChromYTitle() ),
		TEXT_SPACE,
		( g.getGraphPositionTop() + g.getGraphPositionBottom() ) / 2,
		90
	);

	// draw x scale
	kome::img::GraphTool::drawXScale( g, MIN_SCALE_SPACE, SCALE_LENGTH, TEXT_SPACE, false );

	// draw y scale
	if( m_bAutoYScale ) {
		kome::img::GraphTool::drawYScale( g, MIN_SCALE_SPACE, SCALE_LENGTH, TEXT_SPACE, FALSE, 0.0, 100.0 );
		g.setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
		g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
		g.drawText( "(%)", 1, 1 );
	}
	else {
		kome::img::GraphTool::drawYScale( g, MIN_SCALE_SPACE, SCALE_LENGTH, TEXT_SPACE, false );
	}

	// restore color
	g.setColor( oldColor );
	// restore font 
	g.setFont( oldFont );

	// after drawing ,reset the graphic position to the original position
	g.setGraphPosition(nLeft, nTop, nRight, nBottom);
}

// on draw
void OverlappingChromatogramsCanvasEx::onDraw( wxDC& dc ) {
	// set device context
	m_graphics.setDC( &dc );

	// prepare
	onPrepare( dc );

	// position check
	if( m_bChangeSize ){
		m_bChangeSize = false;
	}
	else{
		m_graphics.setGraphPositionLeft((m_nSlantX < 0 ? (m_nZeroPointX + m_nSlantX) : m_nZeroPointX));
		m_graphics.setGraphPositionRight(m_graphics.getGraphPositionLeft() + m_nLengthX + abs(m_nSlantX));
		m_graphics.setGraphPositionBottom(m_nZeroPointY);
		m_graphics.setGraphPositionTop(m_graphics.getGraphPositionBottom() - m_nLengthY - m_nSlantY);
	}

	// original position 
	int nLeft = m_graphics.getGraphPositionLeft();
	int nRight = m_graphics.getGraphPositionRight();
	int nTop = m_graphics.getGraphPositionTop();
	int nBottom = m_graphics.getGraphPositionBottom();
	int nChromNum = m_overlappingChroms.size();

	if( nChromNum > 1 ){
		double dLen = (double)(nBottom - nTop)
					/ (1.0 + (double)(nChromNum - 1) * (double)m_offset / 100.0 * sin((double)m_angle * g_dPiAng));
		m_nLengthY = roundnum(dLen);
		dLen *= (double)(nChromNum - 1) * (double)m_offset / 100.0 * cos((double)m_angle * g_dPiAng);
		m_nSlantX = (int)(dLen < 0.0 ? (dLen - 0.5) : (dLen + 0.5));
		m_nSlantY = nBottom - nTop - m_nLengthY;
		m_nLengthX = nRight - nLeft - abs(m_nSlantX);
	}
	else{
		m_nLengthX = nRight - nLeft;
		m_nLengthY = nBottom - nTop;
		m_nSlantX = 0;
		m_nSlantY = 0;
	}
	m_nZeroPointX = (m_nSlantX < 0 ? (nLeft - m_nSlantX) : nLeft);
	m_nZeroPointY = nBottom;

	// check size
	if( (m_graphics.getGraphWidth() < MINIMUM_DRAW_SIZE) ||
		(m_graphics.getGraphHeight() < MINIMUM_DRAW_SIZE) ||
		(m_nLengthX <= 0) || (m_nLengthY <= 0) ) {
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

	// after drawing ,reset the graphic position to the original position
	m_graphics.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
								m_nZeroPointX + m_nLengthX, m_nZeroPointY);
}

// on size
void OverlappingChromatogramsCanvasEx::onSize( const unsigned int width, const unsigned int height ) {
	ChromatogramCanvas::onSize( width, height );
	m_bChangeSize = true;
}
// @date 2012.02.24 <-- sugo


// creates menu
wxMenu* OverlappingChromatogramsCanvasEx::createMenu()
{
	bool flgSubView = isSubView();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// ここで、Overlapping Settings を追加
	wxMenu* menu = NULL;

	if( flgSubView ){
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getChromatogramSubViewMenu();
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}else{
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getChromatogramViewMenu();
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}

	if( menu == NULL){
		menu = new wxMenu();
	}

	bool bCheckable = true;	// check可能なボタン
	wxMenuItem* item = new wxMenuItem(
		menu,
		ID_POPUP_MENU_OVERLAPPING_SETTINGS,
		wxT( STR_POPUP_MENU_OVERLAPPING_SETTINGS ),
		wxT( "" ),
		bCheckable ? wxITEM_CHECK : wxITEM_NORMAL
	);
	item->SetCheckable( bCheckable );
	menu->Append( item );

	// もし、トグル仕様でチェックマークが必要なら、createPopupMenuRecursive()内でitemを使ってAppendしているやり方と同様にする。
	// チェックマークのon/offは、EVT_UPDATE_UI()イベントハンドラを使う。呼ばれた先での処理は、onUpdatePopupMenu()

	return menu;
}

// on mouse right down
void OverlappingChromatogramsCanvasEx::onMouseRightDown( wxMouseEvent& evt )
{
	return;
}

// on mouse right up
void OverlappingChromatogramsCanvasEx::onMouseRightUp( wxMouseEvent& evt )
{
	bool flgButtonUp = evt.ButtonUp();

	if( flgButtonUp == true ){
		;	// OK
	}else{
		return;	// UPイベントがほしいので、何もしない
	}
	onContextMenu( evt );

	return;
}

// on context menu
void OverlappingChromatogramsCanvasEx::onContextMenu( wxContextMenuEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void OverlappingChromatogramsCanvasEx::onContextMenu( wxMouseEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void OverlappingChromatogramsCanvasEx::onContextMenu( wxPoint ptScreen ){
	wxPoint pt;
	
	pt = ptScreen;	
	
	// create menu
	wxMenu* menu = createMenu();
	if( menu == NULL ) {
		return;
	}

	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}

// on menu popup
void OverlappingChromatogramsCanvasEx::onMenuPopup( wxCommandEvent& evt )
{
	// ID
	int nEventId = evt.GetId();

	// object
	kome::objects::Variant obj = getActiveObject();

	kome::plugin::PluginMenu* ptrMenu = kome::window::Canvas::m_contextMenuTreeMap[nEventId];

	ptrMenu->execute( obj );

	return;
}

// Context Menu	
void OverlappingChromatogramsCanvasEx::onOverlappingSettings( wxCommandEvent& evt ){

	toggleOverlappingDialog( NULL );

	return;
}

//　on update popup menu
void OverlappingChromatogramsCanvasEx::onUpdatePopupMenu( wxUpdateUIEvent& evt )
{
	int nEventId = evt.GetId();

	if( nEventId == ID_POPUP_MENU_OVERLAPPING_SETTINGS ){
		;	// OK
	}else{
		return;	// event id が異なる
	}

	// active object
	kome::objects::Variant obj;
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::Sample* acSample = aoMgr.getActiveSample();

		if( acSample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode();
		}// ※acSampleがNULLの場合はobj初期値を使用する
	}
	else {
		obj = frame->getActiveObject();
	}

	// Overlapping Settingsが表示されているか否か調べる
	kome::objects::Variant ret = checkOverlappingDialog( NULL );	// ret.prim.boolValがtrue = Overlapping Settingsが表示されている

	bool checked = ret.prim.boolVal;
	evt.Check( checked );

	return;
}
