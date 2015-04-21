/**
 * @file OverlappingSpectraCanvas.cpp
 * @brief implements of OverlappingSpectraCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingSpectraCanvas.h"

#include <list>

#include "WindowTool.h"

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SPECTRUM_PROFILE_COLOR		   kome::img::ColorTool::RED
#define OVERLAPPING_COLOR			   kome::img::ColorTool::GREEN

#define PLUGIN_NAME  	               "Overlapping Canvas"	// @date 2011.06.20 <Add> M.Izumi
#define ICON_NAME	                   "overlapping_dlg"		// @date 2011.06.20 <Add> M.Izumi

#define UNSELECT_MENU_ID			   10000	//  @date 2011.10.26 <Add> M.Izumi

BEGIN_EVENT_TABLE( OverlappingSpectraCanvas, SpectrumCanvas )
	EVT_MENU( UNSELECT_MENU_ID, OverlappingSpectraCanvas::onUnSelect ) 
END_EVENT_TABLE()


// static member
std::set< OverlappingSpectraCanvas* > OverlappingSpectraCanvas::m_canvasSet;


// constructor
OverlappingSpectraCanvas::OverlappingSpectraCanvas( wxWindow* parent, const int width, const int height )
		: SpectrumCanvas( parent, width, height, NULL ) {
	// initialize
	m_canvasSet.insert( this );

	m_focusedSpec = NULL;
	// CanvasStyle ê›íË @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS | OVERLAPPING;
}

// destructor
OverlappingSpectraCanvas::~OverlappingSpectraCanvas() {
	m_canvasSet.erase( this );

	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		delete m_overlappingSpectra[ i ].spec;
	}
	m_overlappingSpectra.clear();
}

// clears selected spectra
void OverlappingSpectraCanvas::clearSelectedSpectra() {
	// clear
	m_selectedSpectra.clear();

	// active object
	updateActiveObject();
}

// add selected spectrum
void OverlappingSpectraCanvas::addSelectedSpec( kome::objects::Spectrum* spec ) {
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
void OverlappingSpectraCanvas::removeSelectedSpec( kome::objects::Spectrum* spec ) {
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
unsigned int OverlappingSpectraCanvas::getNumberOfSelectedSpectra() {
	return m_selectedSpectra.size();
}

// get selected spectrum
kome::objects::Spectrum* OverlappingSpectraCanvas::getSelectedSpec( const unsigned int idx ) {
	if( idx >= m_selectedSpectra.size() ) {
		return NULL;
	}
	return m_selectedSpectra[ idx ];
}

// update active object
void OverlappingSpectraCanvas::updateActiveObject() {
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
int OverlappingSpectraCanvas::searchSelectedSpec( kome::objects::Spectrum* spec ) {
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
kome::objects::Spectrum* OverlappingSpectraCanvas::getBaseSpectrum() {
	return &m_dummy;
}

// add spectrum
kome::objects::Spectrum* OverlappingSpectraCanvas::onAddSpectrum( kome::objects::Spectrum* spec ) {
	// search spectrum
	int idx = -1;
	
	// @date 2011/08/04 <Add> OKADA ->
	// specÇÃå^ÇÕ[kome::io::msb::MsbSpectrum]ÅAgetSpectrum()ÇÃå^ÇÕ[kome::operation::TransformedSpectrum]Ç≈Ç†ÇËî‰ärÇµÇƒÇ‡àÍívÇµÇ»Ç¢ÇΩÇﬂÅA
	// getOrgSpectrum()ä÷êîÇ≈óºé“ÇìØÇ∂å^[kome::io::msb::MsbSpectrum]Ç…ïπÇπÇƒî‰ärÇ∑ÇÈÇÊÇ§ïœçX
	for( unsigned int i = 0; i < getNumberOfSpectra() && idx < 0; i++ ) {
		if( (spec->getOrgSpectrum()) == ((getSpectrum( i ))->getOrgSpectrum()) ) {
			idx = (int)i;
		}
	}
	// @date 2011/08/04 <Add> OKADA <-

	// check the spectrum
	if( idx >= 0 || spec == NULL ) {
		return NULL;
	}

	// color
	COLOR color = SPECTRUM_PROFILE_COLOR;
	for( unsigned int i = 0;
			i < m_overlappingSpectra.size() && color == SPECTRUM_PROFILE_COLOR ; i++ ) {
		if( m_overlappingSpectra[ i ].color == color ) {
			color = OVERLAPPING_COLOR;
		}
	}

	// add
	m_overlappingSpectra.resize( m_overlappingSpectra.size() + 1 );
	m_overlappingSpectra.back().spec = new kome::operation::TransformedSpectrum( *spec );
	m_overlappingSpectra.back().color = color;
	m_overlappingSpectra.back().flg = true;
	m_dummy.addSpectrum( m_overlappingSpectra.back().spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );
	
	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( getBaseSpectrum() );	// @date 2011.02.10 <Add> M.Izumi
	
	// update
	update();

	return m_overlappingSpectra.back().spec;
}

// remove spectrum
void OverlappingSpectraCanvas::onRemoveSpectrum( kome::objects::Spectrum* spec ) {
	// search
	int idx = -1;
	for( int i = 0; i < (int)m_overlappingSpectra.size() && idx < 0; i++ ) {
		if( m_overlappingSpectra[ i ].spec == spec 
				|| m_overlappingSpectra[ i ].spec->getOrgSpectrum() == spec->getOrgSpectrum() ) {
			idx = i;
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
void OverlappingSpectraCanvas::update() {
	// update window
	m_dummy.deleteXYData();
	kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );
	WindowTool::refresh();
}

// set color
void OverlappingSpectraCanvas::setColor( kome::objects::Spectrum* spec, const COLOR color ) {
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
COLOR OverlappingSpectraCanvas::getColor( const unsigned int index ) {
	if( index >= m_overlappingSpectra.size() ) {
		return SPECTRUM_PROFILE_COLOR;
	}

	return m_overlappingSpectra[ index ].color;
}

// on mouse button up
bool OverlappingSpectraCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
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
		
		setCallFunctionSelectedSpectra( m_focusedSpec ); //  @date 2011.10.26 <Add> M.Izumi

		return true;
	}
	
	// default
	return SpectrumCanvas::onMouseButtonDown( evt );
}



// on mouse cursor move
bool OverlappingSpectraCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	if( SpectrumCanvas::onMouseCursorMove( evt ) ) {
		return true;
	}
	else if( !evt.lbutton() && !evt.rbutton() ) {
		return true;
	}
	return false;
}

// on draw profile
void OverlappingSpectraCanvas::onDrawProfile( kome::window::DCGraphics& g ) {

	// draw profiles
	m_focusedSpec = NULL;

	int focusLevel = 0;
	kome::objects::Spectrum* focusedSpec = NULL;

	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {	// each spectra
		// information
		SpecInfo* info = &( m_overlappingSpectra[ i ] );

		// draw
		if( info->flg ) {
			g.setColor( info->color );
			g.setUnusedDefaultColor( true );	// @date 2013.04.24 <Add> M.Izumi
			g.setUnusedDefaultThickness( true );

			onDrawSpectrumProfile( g, *( info->spec ) );

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
		// SPEC 82217 ëIëîgå`Çã≠í≤ï\é¶ @date 2011.09.16 <Mod> M.Izumi
		for( unsigned int j=0; j < m_overlappingSpectra.size(); j++ ){
			if( m_selectedSpectra[i]->getOrgSpectrum() == m_overlappingSpectra[j].spec->getOrgSpectrum() ){
				g.getDC()->SetPen( wxPen( m_overlappingSpectra[ j ].color, 3 ) );
				kome::plugin::PluginCallTool::drawSpectrum( *( m_selectedSpectra[ i ] ), g, this );
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

		// draw
		g.setColor( kome::img::ColorTool::YELLOW );
		kome::plugin::PluginCallTool::drawSpectrum( *focusedSpec, g, this );
	}
}

// create menu
wxMenu* OverlappingSpectraCanvas::createMenu() {
	// get menu
	wxMenu* menu = SpectrumCanvas::createMenu();

	// create menu
	if( menu == NULL ) {
		menu = new wxMenu();		
		menu->Append( UNSELECT_MENU_ID, wxT( "&Unselect All" ) );	//  @date 2011.10.26 <Add> M.Izumi
	}

	return menu;
}

// unselect all								   @date 2011.10.26 <Add> M.Izumi
void OverlappingSpectraCanvas::onUnSelect( wxCommandEvent& evt ){
	clearSelectedSpectra();
	setCallFunctionSelectedSpectra( NULL );
}

// set call function ( selected spectra )		@date 2011.10.26 <Add> M.Izumi
void OverlappingSpectraCanvas::setCallFunctionSelectedSpectra( kome::objects::Spectrum* spec ){
	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "select_spectrum","" );
	
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setSpectrum( params, *spec );
	
	if( item != NULL ){
		item->getCall()->invoke( &params );
	}

}
