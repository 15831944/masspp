/**
 * @file OverlappingChromatogramsCanvas.cpp
 * @brief implements of Chromatogram Canvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingChromatogramsCanvas.h"

#include "WindowTool.h"

#include <list>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CHROMATOGRAM_PROFILE_COLOR	   kome::img::ColorTool::BLUE
#define OVERLAPPING_COLOR				kome::img::ColorTool::GREEN

#define PLUGIN_NAME	"Overlapping Canvas"	// @date 2011.06.20 <Add> M.Izumi
#define ICON_NAME	"overlapping_dlg"		// @date 2011.06.20 <Add> M.Izumi

#define UNSELECT_MENU_ID			10000	// @date 2011.10.26 <Add> M.Izumi

BEGIN_EVENT_TABLE( OverlappingChromatogramsCanvas, ChromatogramCanvas )
	EVT_MENU( UNSELECT_MENU_ID, OverlappingChromatogramsCanvas::onUnSelect )
END_EVENT_TABLE()


// static member
std::set< OverlappingChromatogramsCanvas* > OverlappingChromatogramsCanvas::m_canvasSet;


// constructor
OverlappingChromatogramsCanvas::OverlappingChromatogramsCanvas( wxWindow* parent, const int width, const int height )
		: ChromatogramCanvas( parent, width, height, NULL ) {
	// initialize
	m_canvasSet.insert( this );

	m_focusedChrom = NULL;

	// CanvasStyle ê›íË @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS | OVERLAPPING;
}

// destructor
OverlappingChromatogramsCanvas::~OverlappingChromatogramsCanvas() {
	m_canvasSet.erase( this );

	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		delete m_overlappingChroms[ i ].chrom;
	}
	m_overlappingChroms.clear();
}

// clears selected chromatograms
void OverlappingChromatogramsCanvas::clearSelectedChroms() {
	// clear
	m_selectedChroms.clear();

	// active object
	updateActiveObject();
}

// add selected chromatogram
void OverlappingChromatogramsCanvas::addSelectedChrom( kome::objects::Chromatogram* chrom ) {
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
void OverlappingChromatogramsCanvas::removeSelectedChrom( kome::objects::Chromatogram* chrom ) {
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
unsigned int OverlappingChromatogramsCanvas::getNumberOfSelectedChroms() {
	return m_selectedChroms.size();
}

// get selected chromatogram
kome::objects::Chromatogram* OverlappingChromatogramsCanvas::getSelectedChrom( const unsigned int idx ) {
	if( idx >= m_selectedChroms.size() ) {
		return NULL;
	}
	return m_selectedChroms[ idx ];
}

// update active object
void OverlappingChromatogramsCanvas::updateActiveObject() {
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
int OverlappingChromatogramsCanvas::searchSelectedChrom( kome::objects::Chromatogram* chrom ) {
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
kome::objects::Chromatogram* OverlappingChromatogramsCanvas::getBaseChromatogram() {
	return &m_dummy;
}

// on add chromatogram
kome::objects::Chromatogram* OverlappingChromatogramsCanvas::onAddChromatogram( kome::objects::Chromatogram* chrom ) {
	// search
	int idx = -1;

	// @date 2011/08/04 <Add> OKADA ->
	for( unsigned int i = 0; i < getNumberOfChromatograms() && idx < 0; i++ ) {
		if( ((getChromatogram(i))->getOrgChromatogram()) == (chrom->getOrgChromatogram()) ) {
			idx = (int)i;
		}
	}
	// @date 2011/08/04 <Add> OKADA <-

	// check the chromatogram
	if( chrom == NULL || idx >= 0 ) {
		return NULL;
	}

	// color
	COLOR color = CHROMATOGRAM_PROFILE_COLOR;
	for( unsigned int i = 0;
			i < m_overlappingChroms.size() && color == CHROMATOGRAM_PROFILE_COLOR; i++ ) {
		if( m_overlappingChroms[ i ].color == color ) {
			color = OVERLAPPING_COLOR;
		}
	}

	// add
	m_overlappingChroms.resize( m_overlappingChroms.size() + 1 );
	m_overlappingChroms.back().chrom = new kome::operation::TransformedChromatogram( *chrom );
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
void OverlappingChromatogramsCanvas::onRemoveChromatogram( kome::objects::Chromatogram* chrom ) {
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
void OverlappingChromatogramsCanvas::setColor( kome::objects::Chromatogram* chrom, const COLOR color ) {
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
COLOR OverlappingChromatogramsCanvas::getColor( const unsigned int index ) {
	if( index >= m_overlappingChroms.size() ) {
		return CHROMATOGRAM_PROFILE_COLOR;
	}

	return m_overlappingChroms[ index ].color;
}

// update canvas
void OverlappingChromatogramsCanvas::update() {
	// update canvas
	m_dummy.deleteXYData();
	kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );
	WindowTool::refresh();
}

// on mouse button down
bool OverlappingChromatogramsCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
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
		
		setCallFunctionSelectedChromatogram( m_focusedChrom );	//  @date 2011.10.26 <Add> M.Izumi

		return true;
	}

	return ChromatogramCanvas::onMouseButtonDown( evt );
}

// on mouse cursor move
bool OverlappingChromatogramsCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	if( ChromatogramCanvas::onMouseCursorMove( evt ) ) {
		return true;
	}
	else if( !evt.lbutton() && !evt.rbutton() ) {
		return true;
	}
	return false;
}

// on draw profile
void OverlappingChromatogramsCanvas::onDrawProfile( kome::window::DCGraphics& g ) {
	// draw profiles
	m_focusedChrom = NULL;

	int focusLevel = 0;
	kome::objects::Chromatogram* focusedChrom = NULL;

	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {	// each chromatograms
		// information
		ChromInfo* info = &( m_overlappingChroms[ i ] );

		// draw
		if( info->flg ) {
			g.setColor( info->color );
			g.setUnusedDefaultColor( true );	// @date 2013.04.24 <Add> M.Izumi
			g.setUnusedDefaultThickness( true );

			// draw profile
			onDrawChromatogramProfile( g, *( info->chrom ) );

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
		// SPEC 82217 ëIëîgå`Çã≠í≤ï\é¶ @date 2011.09.16 <Mod> M.Izumi
		for( unsigned int j=0; j < m_overlappingChroms.size(); j++ ){
			if( m_selectedChroms[i]->getOrgChromatogram() == m_overlappingChroms[j].chrom->getOrgChromatogram() ){
				g.getDC()->SetPen( wxPen( m_overlappingChroms[ j ].color, 3 ) );
				kome::plugin::PluginCallTool::drawChromatogram( *( m_selectedChroms[ i ] ), g, this );
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

		// draw
		g.setColor( kome::img::ColorTool::YELLOW );
		kome::plugin::PluginCallTool::drawChromatogram( *focusedChrom, g, this );
	}
}

// create menu
wxMenu* OverlappingChromatogramsCanvas::createMenu() {
	wxMenu* menu = ChromatogramCanvas::createMenu();

	// create menu
	if( menu == NULL ) {
		menu = new wxMenu();
		
		menu->Append( UNSELECT_MENU_ID, wxT( "&Unselect All" ) );	//  @date 2011.10.26 <Add> M.Izumi
	}

	return menu;

}

// unselect all								   @date 2011.10.26 <Add> M.Izumi
void OverlappingChromatogramsCanvas::onUnSelect( wxCommandEvent& evt ){
	clearSelectedChroms();
	setCallFunctionSelectedChromatogram( NULL );
}

// set call function ( selected chromatogram )		@date 2011.10.26 <Add> M.Izumi
void OverlappingChromatogramsCanvas::setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom ){
	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "select_chromatogram","" );
	
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
	
	if( item != NULL ){
		item->getCall()->invoke( &params );
	}

}
