/**
 * @file OverlappingSpectraCanvasEx.cpp
 * @brief implements of OverlappingSpectraCanvasEx class
 *
 * @author S.Sugo
 * @date 2012.02.21
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingSpectraCanvasEx.h"
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

#define UNSELECT_MENU_ID			10000	//  @date 2011.10.26 <Add> M.Izumi

// for context menu
#define ID_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS	60004
#define STR_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS	"Overlapping Settings"

// @date 2012.02.22 sugo add -->
#define MINIMUM_DRAW_SIZE		30
#define MIN_SCALE_SPACE			10
#define SCALE_LENGTH			6
#define TEXT_SPACE				5

// π
const double	g_dPi		= (3.1415926535897932384626433832795);

// π/180
const double	g_dPiAng	= (1.7453292519943295769236907684886e-2);
// @date 2012.02.22 <-- sugo

BEGIN_EVENT_TABLE( OverlappingSpectraCanvasEx, SpectrumCanvas )
	EVT_MENU( UNSELECT_MENU_ID, OverlappingSpectraCanvasEx::onUnSelect ) 
	EVT_CONTEXT_MENU( OverlappingSpectraCanvasEx::onContextMenu )	// for context menu	// @date 2012/12/07 <Add> OKADA
	EVT_MENU( ID_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS, OverlappingSpectraCanvasEx::onOverlappingSettings )	// for context menu
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, OverlappingSpectraCanvasEx::onMenuPopup )	//　@date 2012/11/21
	EVT_UPDATE_UI( ID_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS, onUpdatePopupMenu )
END_EVENT_TABLE()


// static member
std::set< OverlappingSpectraCanvasEx* > OverlappingSpectraCanvasEx::m_canvasSet;


// constructor
OverlappingSpectraCanvasEx::OverlappingSpectraCanvasEx( wxWindow* parent, const int width, const int height )
		: SpectrumCanvas( parent, width, height, NULL ) {
	// initialize
	m_canvasSet.insert( this );

	m_focusedSpec = NULL;
	// CanvasStyle 設定 @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS | OVERLAPPING;

	// @date 2012.02.22 sugo add -->
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
	// @date 2012.02.22 <-- sugo 

	m_colorIdx = 0;
	m_colors.resize( 9 );
	m_colors[ 0 ] = kome::img::ColorTool::getColor( 0xff, 0x00, 0x00 );
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
OverlappingSpectraCanvasEx::~OverlappingSpectraCanvasEx() {
	m_canvasSet.erase( this );

	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		delete m_overlappingSpectra[ i ].spec;
	}
	m_overlappingSpectra.clear();
}

// clears selected spectra
void OverlappingSpectraCanvasEx::clearSelectedSpectra() {
	// clear
	m_selectedSpectra.clear();

	// active object
	updateActiveObject();
}

// add selected spectrum
void OverlappingSpectraCanvasEx::addSelectedSpec( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// search
	int idx = searchSelectedSpec( spec );
	if( idx >= 0 ) {
		return;
	}

	// add
	m_selectedSpectra.push_back( spec );

	// active object
	updateActiveObject();
}

// remove selected spectrum
void OverlappingSpectraCanvasEx::removeSelectedSpec( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// remove
	int idx = searchSelectedSpec( spec );
	if( idx >= 0 ) {
		m_selectedSpectra.erase( m_selectedSpectra.begin() + idx );
	}

	// active object
	updateActiveObject();
}

// get the number of selected spectra
unsigned int OverlappingSpectraCanvasEx::getNumberOfSelectedSpectra() {
	return m_selectedSpectra.size();
}

// get selected spectrum
kome::objects::Spectrum* OverlappingSpectraCanvasEx::getSelectedSpec( const unsigned int idx ) {
	if( idx >= m_selectedSpectra.size() ) {
		return NULL;
	}
	return m_selectedSpectra[ idx ];
}

// update active object
void OverlappingSpectraCanvasEx::updateActiveObject() {
	// object
	kome::objects::Variant obj;

	if( m_selectedSpectra.size() == 1 ) {	// select
		obj.type = kome::objects::Variant::SPECTRUM;
		obj.prim.pt = m_selectedSpectra[ 0 ];
	}
	else {	// unselect
		obj.type = kome::objects::Variant::UNKNOWN;
		obj.prim.pt = NULL;
	}

	setActiveObject( obj );
}

// search selected spectrum
int OverlappingSpectraCanvasEx::searchSelectedSpec( kome::objects::Spectrum* spec ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_selectedSpectra.size() && idx < 0; i++ ) {
		if( spec->getOrgSpectrum() == m_selectedSpectra[ i ]->getOrgSpectrum() ) {
			idx = (int)i;
		}
	}

	return idx;
}

// get base spectrum
kome::objects::Spectrum* OverlappingSpectraCanvasEx::getBaseSpectrum() {
	return &m_dummy;
}

// add spectrum
kome::objects::Spectrum* OverlappingSpectraCanvasEx::onAddSpectrum( kome::objects::Spectrum* spec ) {
	// search spectrum
	int idx = -1;
	int nNum;

	// @date 2011/08/04 <Add> OKADA ->
	// specの型は[kome::io::msb::MsbSpectrum]、getSpectrum()の型は[kome::operation::TransformedSpectrum]であり比較しても一致しないため、
	// getOrgSpectrum()関数で両者を同じ型[kome::io::msb::MsbSpectrum]に併せて比較するよう変更
	for( unsigned int i = 0; i < getNumberOfSpectra() && idx < 0; i++ ) {
		if( (spec->getOrgSpectrum()) == ((getSpectrum( i ))->getOrgSpectrum()) &&
			spec->getOperationFlag() ) {
			idx = (int)i;
		}
	}
	nNum = getNumberOfSpectra();
	// @date 2011/08/04 <Add> OKADA <-

	// check the spectrum
	if( idx >= 0 || spec == NULL ) {
		return NULL;
	}

	// color
	COLOR color = m_colors[ m_colorIdx ];
	m_colorIdx = ( m_colorIdx + 1 ) % (int)( m_colors.size() );

	// add
	m_overlappingSpectra.resize( m_overlappingSpectra.size() + 1 );
	m_overlappingSpectra.back().spec = new kome::operation::TransformedSpectrum( *spec, spec->getOperationFlag() );
	m_overlappingSpectra.back().color = color;
	m_overlappingSpectra.back().flg = true;
	m_overlappingSpectra.back().num = nNum;

	m_dummy.addSpectrum( m_overlappingSpectra.back().spec );

	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( getBaseSpectrum() );	// @date 2011.02.10 <Add> M.Izumi


	// update
	update();

	return m_overlappingSpectra.back().spec;
}

// remove spectrum
void OverlappingSpectraCanvasEx::onRemoveSpectrum( kome::objects::Spectrum* spec ) {
	// search
	int idx = -1;
	for( int i = 0; i < (int)m_overlappingSpectra.size() && idx < 0; i++ ) {
		if( m_overlappingSpectra[ i ].spec == spec 
				|| m_overlappingSpectra[ i ].spec->getOrgSpectrum() == spec->getOrgSpectrum() ) {
			idx = i;
		}
	}

	// num change
	int cnt,mnum;
	mnum = m_overlappingSpectra[idx].num;
	for( cnt = 0;cnt < (int)m_overlappingSpectra.size() ; cnt++ ) {
		if(m_overlappingSpectra[cnt].num > mnum) {
			m_overlappingSpectra[cnt].num -= 1;
		}
	}

	// remove
	if( idx >= 0 ) {
		m_dummy.removeSpectrum( spec );
		kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );

		delete m_overlappingSpectra[ idx ].spec;
		m_overlappingSpectra.erase( m_overlappingSpectra.begin() + idx );
	}
	removeSelectedSpec( spec );
		
	// update
	update();

}

// update 
void OverlappingSpectraCanvasEx::update() {
	// update window
	m_dummy.deleteXYData();
	kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );
	WindowTool::refresh();
}

// set color
void OverlappingSpectraCanvasEx::setColor( kome::objects::Spectrum* spec, const COLOR color ) {
	// search
	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		if( spec == NULL
				|| m_overlappingSpectra[ i ].spec == spec
				|| m_overlappingSpectra[ i ].spec->getOrgSpectrum() == spec->getOrgSpectrum() ) {
			// set color
			m_overlappingSpectra[ i ].color = color;
		}
	}
}

// get color
COLOR OverlappingSpectraCanvasEx::getColor( const unsigned int index ) {
	if( index >= m_overlappingSpectra.size() ) {
		return m_colors[ 0 ];
	}

	return m_overlappingSpectra[ index ].color;
}

// on mouse button up
bool OverlappingSpectraCanvasEx::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	// active object
	kome::objects::Variant obj = getActiveObject();

	// select spectrum
	if( evt.x() >= m_graphics.getGraphPositionLeft()
			&& evt.x() <= m_graphics.getGraphPositionRight()
			&& evt.y() >= m_graphics.getGraphPositionTop()
			&& evt.y() <= m_graphics.getGraphPositionBottom()
			&& m_focusedSpec != NULL
			&& obj.prim.pt != m_focusedSpec
	) {
		clearSelectedSpectra();
		addSelectedSpec( m_focusedSpec );
		
		// @date 2013.03.14 <Add> M.Izumi
		for( unsigned int i=0; i < m_overlappingSpectra.size(); i++ ){	// @date 2013/06/11 <Mod> OKADA
			// information
			SpecInfo* info = &( m_overlappingSpectra[ i ] );
			if( m_focusedSpec->getOrgSpectrum() == info->spec->getOrgSpectrum() ){
				m_index = i;
			}
		}
		setCallFunctionSelectedSpectra( m_focusedSpec ); //  @date 2011.10.26 <Add> M.Izumi

		return true;
	}
	
	// default
	return SpectrumCanvas::onMouseButtonDown( evt );
}

// on mouse cursor move
bool OverlappingSpectraCanvasEx::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	if( SpectrumCanvas::onMouseCursorMove( evt ) ) {
		return true;
	}
	else if( !evt.lbutton() && !evt.rbutton() ) {
		return true;
	}
	return false;
}

// on draw profile
void OverlappingSpectraCanvasEx::onDrawProfile( kome::window::DCGraphics& g ) {

	// forcus overlapping spectra num
	int nNumForcusSpec = -1;

	// draw profiles
	int nSpecNum = m_overlappingSpectra.size();
	int nParameter = (nSpecNum > 2 ? (nSpecNum - 1) : 1);
	m_focusedSpec = NULL;
	int focusLevel = 0;
	kome::objects::Spectrum* focusedSpec = NULL;

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

	for( int i = 0; i < nSpecNum; i++ ) {	// each spectra
		// information
		SpecInfo* info = &( m_overlappingSpectra[ i ] );

		// draw
		if( info->flg ) {
			// shift the graphic position
			g.setGraphPosition(m_nZeroPointX + i * m_nSlantX / nParameter,
							   m_nZeroPointY - m_nLengthY - i * m_nSlantY / nParameter,
							   m_nZeroPointX + m_nLengthX + i * m_nSlantX / nParameter,
							   m_nZeroPointY - i * m_nSlantY / nParameter);
			if( m_bAutoYScale ){
				g.setMaxY(GetMaxY(info->spec, g.getMinX(), g.getMaxX()));
			}
			g.setColor( info->color );

			// 色、太さのDefault設定を反映させる為の処理追加 @date 2013.04.26 <Add> M.izumi
			g.setUnusedDefaultColor( true );
			g.setUnusedDefaultThickness( true );

			// draw graph on the new position
			g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
										  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
										  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
			onDrawSpectrumProfile( g, *( info->spec ) );
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
					focusedSpec = info->spec;
					nNumForcusSpec = i;
					m_focusedSpec = info->spec->getOrgSpectrum();
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
							focusedSpec = info->spec;
							nNumForcusSpec = i;
							m_focusedSpec = info->spec->getOrgSpectrum();
							focusLevel = 1;
						}
					}
				}
			}
		}
	}

	// draw selected spectrum
	for( unsigned int i = 0; i < m_selectedSpectra.size(); i++ ) {
		// SPEC 82217 選択波形を強調表示 @date 2011.09.16 <Mod> M.Izumi
		for( int j=0; j < nSpecNum; j++ ){
			if( m_selectedSpectra[i]->getOrgSpectrum() == m_overlappingSpectra[j].spec->getOrgSpectrum() && m_index == j ){
				g.getDC()->SetPen( wxPen( m_overlappingSpectra[ j ].color, 3 ) );

				// shift the graphic position
				g.setGraphPosition(m_nZeroPointX + j * m_nSlantX / nParameter,
								   m_nZeroPointY - m_nLengthY - j * m_nSlantY / nParameter,
								   m_nZeroPointX + m_nLengthX + j * m_nSlantX / nParameter,
								   m_nZeroPointY - j * m_nSlantY / nParameter);
				if( m_bAutoYScale ){
					g.setMaxY(GetMaxY(m_selectedSpectra[i], g.getMinX(), g.getMaxX()));
				}

				g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
											  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
											  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
				kome::plugin::PluginCallTool::drawSpectrum( *( m_selectedSpectra[ i ] ), g, this );
				g.getDC()->DestroyClippingRegion();
			}
		}
	}

	// draw focused spectrum
	if( focusedSpec != NULL ) {
		// tooltip
		m_tooltip = focusedSpec->getTitle();
		kome::objects::DataGroupNode* group = focusedSpec->getGroup();
		while( group != NULL ) {
			m_tooltip = FMT( "%s - %s", m_tooltip.c_str(), group->getName() );
			group = group->getParentGroup();
		}

		// shift the graphic position
		g.setGraphPosition(m_nZeroPointX + nNumForcusSpec * m_nSlantX / nParameter,
						   m_nZeroPointY - m_nLengthY - nNumForcusSpec * m_nSlantY / nParameter,
						   m_nZeroPointX + m_nLengthX + nNumForcusSpec * m_nSlantX / nParameter,
						   m_nZeroPointY - nNumForcusSpec * m_nSlantY / nParameter);
		if( m_bAutoYScale ){
			g.setMaxY(GetMaxY(focusedSpec, g.getMinX(), g.getMaxX()));
		}

		// draw
		g.setColor( kome::img::ColorTool::YELLOW );
		g.getDC()->SetClippingRegion( g.getGraphPositionLeft() - nLeft, g.getGraphPositionTop() - nTop,
									  g.getGraphPositionRight() - g.getGraphPositionLeft() + 1,
									  g.getGraphPositionBottom() - g.getGraphPositionTop() + 1 );
		kome::plugin::PluginCallTool::drawSpectrum( *focusedSpec, g, this );
		g.getDC()->DestroyClippingRegion();
	}

	// after drawing ,reset the graphic position to the original position
	g.setGraphPosition(nLeft, nTop, nRight, nBottom);
	if( m_bAutoYScale ){
		g.setMinY(dMinY);
		g.setMaxY(dMaxY);
	}
	// @date 2012.02.23 <-- sugo
}

// unselect all								   @date 2011.10.26 <Add> M.Izumi
void OverlappingSpectraCanvasEx::onUnSelect( wxCommandEvent& evt ){
	clearSelectedSpectra();
	setCallFunctionSelectedSpectra( NULL );
}

// set call function ( selected spectra )		@date 2011.10.26 <Add> M.Izumi
void OverlappingSpectraCanvasEx::setCallFunctionSelectedSpectra( kome::objects::Spectrum* spec ){
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "select_spectrum","overlapping" );
	
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setSpectrum( params, *spec );
	
	if( item != NULL ){
		item->getCall()->invoke( &params );
	}

}

// @date 2012.02.22 sugo add -->
// get spectrum number
int OverlappingSpectraCanvasEx::GetOverlappingSpectrumNum( unsigned int nNum ) {
	int nRet = -1;
	if( nNum < m_overlappingSpectra.size() ){
		nRet = m_overlappingSpectra[ nNum ].num;
	}
	return nRet;
}

// exchange data
void OverlappingSpectraCanvasEx::exchange(int nS,int nE){
	SpecInfo temp;
	temp.spec = m_overlappingSpectra[nS].spec;
	temp.color = m_overlappingSpectra[nS].color;
	temp.flg = m_overlappingSpectra[nS].flg;
	temp.num = m_overlappingSpectra[nS].num;

	m_overlappingSpectra[nS].spec = m_overlappingSpectra[nE].spec;
	m_overlappingSpectra[nS].color = m_overlappingSpectra[nE].color;
	m_overlappingSpectra[nS].flg = m_overlappingSpectra[nE].flg;
	m_overlappingSpectra[nS].num = m_overlappingSpectra[nE].num;

	m_overlappingSpectra[nE].spec = temp.spec;
	m_overlappingSpectra[nE].color = temp.color;
	m_overlappingSpectra[nE].flg = temp.flg;
	m_overlappingSpectra[nE].num = temp.num;
}

// set 'Auto Y Scale'
void OverlappingSpectraCanvasEx::SetAutoYScale( bool scale ){
	m_bAutoYScale = scale;
}

// set offset
void OverlappingSpectraCanvasEx::SetOffset( int offset ){
	m_offset = offset;
}

// set angle
void OverlappingSpectraCanvasEx::SetAngle( int angle ){
	m_angle = angle;
}

// get 'Auto Y Scale'
bool OverlappingSpectraCanvasEx::GetAutoYScale(  ){
	return m_bAutoYScale;
}

// get offset
int OverlappingSpectraCanvasEx::GetOffset(  ){
	return m_offset;
}

// get angle
int OverlappingSpectraCanvasEx::GetAngle(  ){
	return m_angle;
}

// get MaxY
double OverlappingSpectraCanvasEx::GetMaxY( kome::objects::Spectrum* spec, const double minX, const double maxX ){

	double maxY = 0.01;

	// check the member
	if( spec != NULL ){
		maxY = spec->getMaxIntensity( minX, maxX );
	}

	return maxY;
}

// drawing
void OverlappingSpectraCanvasEx::onDrawGraphics( wxDC& dc ) {
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
void OverlappingSpectraCanvasEx::onDrawWindowFg( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();

	// check the spectrum
	if( spec == NULL ){
		return;
	}
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}
	else if( (m_nLengthX <= 0) || (m_nLengthY <= 0) ){
		return;
	}

	// original position 
	int nLeft = g.getGraphPositionLeft();
	int nRight = g.getGraphPositionRight();
	int nTop = g.getGraphPositionTop();
	int nBottom = g.getGraphPositionBottom();

	g.setGraphPosition(m_nZeroPointX, m_nZeroPointY - m_nLengthY,
					   m_nZeroPointX + m_nLengthX, m_nZeroPointY);

	// set graphics
	COLOR oldColor = g.getColor();
	kome::img::Font oldFont = g.getFont();

	// 色、太さの設定をiniファイルから取得する為の処理追加 @date 2013.04.26 <Add> M.izumi ->
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
	// @date 2013.04.26 <Add> M.izumi <-

	// draw
	g.drawLine(m_nZeroPointX, m_nZeroPointY - m_nLengthY, m_nZeroPointX, m_nZeroPointY);
	g.drawLine(m_nZeroPointX, m_nZeroPointY, m_nZeroPointX + m_nLengthX, m_nZeroPointY);
	g.drawLine(m_nZeroPointX + m_nSlantX, m_nZeroPointY - m_nSlantY, m_nZeroPointX + m_nLengthX + m_nSlantX, m_nZeroPointY - m_nSlantY);
	g.drawLine(m_nZeroPointX, m_nZeroPointY, m_nZeroPointX + m_nSlantX, m_nZeroPointY - m_nSlantY);
	g.drawLine(m_nZeroPointX + m_nLengthX, m_nZeroPointY, m_nZeroPointX + m_nLengthX + m_nSlantX, m_nZeroPointY - m_nSlantY);

	// draw x title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_BOTTOM );

	kome::objects::DataGroupNode* group = spec->getGroup();

	g.drawText(
		( group == NULL ? "m/z" : group->getSpecXTitle() ),
		( g.getGraphPositionLeft() + g.getGraphPositionRight() ) / 2,
		g.getHeight() - 2 * TEXT_SPACE,
		0
	);

	// draw y title
	g.setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.drawText(
		( group == NULL ? "intensity" : group->getSpecYTitle() ),
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
	// restor font
	g.setFont( oldFont );

	// after drawing ,reset the graphic position to the original position
	g.setGraphPosition(nLeft, nTop, nRight, nBottom);
}

// on draw
void OverlappingSpectraCanvasEx::onDraw( wxDC& dc ) {
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
	int nSpecNum = m_overlappingSpectra.size();

	if( nSpecNum > 1 ){
		double dLen = (double)(nBottom - nTop)
					/ (1.0 + (double)(nSpecNum - 1) * (double)m_offset / 100.0 * sin((double)m_angle * g_dPiAng));
		m_nLengthY = roundnum(dLen);
		dLen *= (double)(nSpecNum - 1) * (double)m_offset / 100.0 * cos((double)m_angle * g_dPiAng);
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
void OverlappingSpectraCanvasEx::onSize( const unsigned int width, const unsigned int height ) {
	SpectrumCanvas::onSize( width, height );
	m_bChangeSize = true;
}
// @date 2012.02.22 <-- sugo

// creates menu
wxMenu* OverlappingSpectraCanvasEx::createMenu()
{
	bool flgSubView = isSubView();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// ここで、Overlapping Settings を追加
	wxMenu* menu = NULL;

	if( flgSubView ){
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumSubViewMenu();
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}else{
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumViewMenu();
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}

	if( menu == NULL){
		menu = new wxMenu();
	}

	bool bCheckable = true;	// check可能なボタン
	wxMenuItem* item = new wxMenuItem(
		menu,
		ID_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS,
		wxT( STR_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS ),
		wxT( "" ),
		bCheckable ? wxITEM_CHECK : wxITEM_NORMAL
	);
	item->SetCheckable( bCheckable );
	menu->Append( item );

	// もし、トグル仕様でチェックマークが必要なら、createPopupMenuRecursive()内でitemを使ってAppendしているやり方と同様にする。
	// チェックマークのon/offは、EVT_UPDATE_UI()イベントハンドラを使う。呼ばれた先での処理は、onUpdatePopupMenu()

	return menu;
}

//　on mouse right down
void OverlappingSpectraCanvasEx::onMouseRightDown( wxMouseEvent& evt )
{
	return;
}

// on mouse right up
void OverlappingSpectraCanvasEx::onMouseRightUp( wxMouseEvent& evt )
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
void OverlappingSpectraCanvasEx::onContextMenu( wxContextMenuEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void OverlappingSpectraCanvasEx::onContextMenu( wxMouseEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void OverlappingSpectraCanvasEx::onContextMenu( wxPoint ptScreen ){
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
void OverlappingSpectraCanvasEx::onMenuPopup( wxCommandEvent& evt )
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
void OverlappingSpectraCanvasEx::onOverlappingSettings( wxCommandEvent& evt ){

	toggleOverlappingDialog( NULL );

	return;
}

//　on update popup menu
void OverlappingSpectraCanvasEx::onUpdatePopupMenu( wxUpdateUIEvent& evt )
{
	int nEventId = evt.GetId();

	if( nEventId == ID_POPUP_MENU_SPECTRUM_OVERLAPPING_SETTINGS ){
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
