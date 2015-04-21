/**
 * @file SpectrumCanvas.cpp
 * @brief implements of SpectrumCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumCanvas.h"

#include "WindowTool.h"

#include <list>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SPECTRUM_PROFILE_COLOR		   kome::img::ColorTool::RED
#define DRAW_SPEC_FUNC_TYPE			  "DRAW_SPEC"


BEGIN_EVENT_TABLE( SpectrumCanvas, ProfileCanvas )
	EVT_CONTEXT_MENU( SpectrumCanvas::onContextMenu )
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, SpectrumCanvas::onMenuPopup )
END_EVENT_TABLE()


// static member
std::set< SpectrumCanvas* > SpectrumCanvas::m_canvasSet;


// constructor
SpectrumCanvas::SpectrumCanvas(
		wxWindow* parent,
		const int width,
		const int height,
		kome::objects::Spectrum* spec
) : ProfileCanvas( parent, width, height ) {
	// initialize
	setSpectrum( spec );
	m_canvasSet.insert( this );

	setProfileColor( SPECTRUM_PROFILE_COLOR );
}

// destructor
SpectrumCanvas::~SpectrumCanvas() {
	m_canvasSet.erase( this );
}

// get base spectrum
kome::objects::Spectrum* SpectrumCanvas::getBaseSpectrum() {
	return getSpectrum();
}

// on mouse button down
bool SpectrumCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	return kome::plugin::PluginCallTool::onSpectrumButtonDown( *spec, &m_graphics, evt, this );
}

// on mouse button up
bool SpectrumCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();

	if( kome::plugin::PluginCallTool::onSpectrumButtonUp( *spec, &m_graphics, evt, this ) ) {
		return true;
	}
	else if( evt.rbutton()
			&& evt.x() >= 0 && evt.x() <= m_width && evt.y() >= 0 && evt.y() <= m_height ) {
		popupMenu( evt.x(), evt.y() );
	}
	return false;
}

// on mouse button double click
bool SpectrumCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	return kome::plugin::PluginCallTool::onSpectrumDoubleClick( *spec, &m_graphics, evt, this );
}

// on mouse cursor move
bool SpectrumCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	return kome::plugin::PluginCallTool::onSpectrumMouseMove( *spec, &m_graphics, evt, this );
}

// on mouse cursor enter
bool SpectrumCanvas::onMouseCursorEnter( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	return kome::plugin::PluginCallTool::onSpectrumMouseEnter( *spec, &m_graphics, evt, this );
}

// on mouse cursor leave
bool SpectrumCanvas::onMouseCursorLeave( kome::evt::MouseEvent& evt ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	return kome::plugin::PluginCallTool::onSpectrumMouseLeave( *spec, &m_graphics, evt, this );
}

// prepare
void SpectrumCanvas::onPrepareDraw( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	kome::plugin::PluginCallTool::prepareDrawSpectrum( *spec, g, this );
}

// on draw window background
void SpectrumCanvas::onDrawWindowBg( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	kome::plugin::PluginCallTool::drawSpectrumWindowBg( *spec, g, this );
}

// on draw graph background
void SpectrumCanvas::onDrawGraphBg( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	kome::plugin::PluginCallTool::drawSpectrumBg( *spec, g, this );
}

// on draw profile
void SpectrumCanvas::onDrawProfile( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	onDrawSpectrumProfile( g, *spec );
}

// on draw graph foreground
void SpectrumCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	kome::plugin::PluginCallTool::drawSpectrumFg( *spec, g, this );
}

// on draw window foreground
void SpectrumCanvas::onDrawWindowFg( kome::window::DCGraphics& g ) {
	kome::objects::Spectrum* spec = getBaseSpectrum();
	kome::plugin::PluginCallTool::drawSpectrumWindowFg( *spec, g, this );
}

// on draw spectrum profile
void SpectrumCanvas::onDrawSpectrumProfile(
		kome::window::DCGraphics& g,
		kome::objects::Spectrum& spec
) {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// DC
	wxDC* dc = g.getDC();

	// memory device context
	wxSize size = dc->GetSize();

	wxBitmap bitmap( size.GetWidth(), size.GetHeight() );
	wxMemoryDC memDc( bitmap );

	memDc.SetBackground( *wxWHITE_BRUSH );

	g.setDC( &memDc );

	// get functions
	std::vector< kome::plugin::PluginCall* > calls;
	for(unsigned int i =0; i < plgMgr.getNumberOfPlugins(); i++) {
		kome::plugin::PluginInfo* plginfo = plgMgr.getPlugin(i);
		for (unsigned int j = 0; j < plginfo->getNumberOfCalls(); j++) {
			kome::plugin::PluginCall* plgcall = plginfo->getCall(j);
			if(strcmp(plgcall->getType(), DRAW_SPEC_FUNC_TYPE) == 0 && plgcall->checkStatus()) {
				calls.push_back(plgcall);
			}
		}
	}

	// parameter
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setSpectrum( params, spec );
	kome::plugin::PluginCallTool::setCanvas( params, this );
	kome::plugin::PluginCallTool::setGraphics( params, g );

	// draw
	for( unsigned int i = 0; i < calls.size(); i++ ) {
		memDc.Clear();
		calls[ i ]->invoke( &params );

		// copy
		dc->Blit(
			0,
			0,
			size.GetWidth(),
			size.GetHeight(),
			&memDc,
			0,
			0,
			wxAND
		);
	}

	// restore DC
	g.setDC( dc );
}

// on add spectrum
kome::objects::Spectrum* SpectrumCanvas::onAddSpectrum( kome::objects::Spectrum* spec ) {
	// check
	for( unsigned int i = 0; i < getNumberOfSpectra(); i++ ) {
		if( getSpectrum( i ) == spec ) {
			return NULL;
		}
	}

	// clear spectra
	clearObjects();

	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( spec );	// @date 2011.02.10 <Add> M.Izumi

	return spec;
}

// on remove spectrum
void SpectrumCanvas::onRemoveSpectrum( kome::objects::Spectrum* spec ) {
}

// get canvas
SpectrumCanvas* SpectrumCanvas::getCanvas( kome::objects::Spectrum* spec ) {
	// search
	for( std::set< SpectrumCanvas* >::iterator it = m_canvasSet.begin();
			it != m_canvasSet.end(); it++ ) {
		SpectrumCanvas* canvas = *it;
		if( spec == canvas->getBaseSpectrum() ) {
			return canvas;
		}
	}
	return NULL;
}

// destroy all canvas
void SpectrumCanvas::destroyAllCanvas(){
	// store to array
	std::vector< SpectrumCanvas* > canvases;
	for( std::set< SpectrumCanvas* >::iterator it = m_canvasSet.begin();
			it != m_canvasSet.end(); it++ ) {
		canvases.push_back( *it );
	}

	// destroy
	for( unsigned int i = 0; i < canvases.size(); i++ ) {
		canvases[ i ]->Destroy();
	}
}

// creates menu
wxMenu* SpectrumCanvas::createMenu()
{
	bool flgSubView = isSubView();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	wxMenu* menu = NULL;

	if( flgSubView ){
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumSubViewMenu();
		menu = createPopupMenu( &plugin_menu );
	}else{
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumViewMenu();
		menu = createPopupMenu( &plugin_menu );
	}

	return menu;
}

void SpectrumCanvas::onMouseRightDown( wxMouseEvent& evt )
{
	return;
}

void SpectrumCanvas::onMouseRightUp( wxMouseEvent& evt )
{

	bool flgButtonUp = evt.ButtonUp();

	if( flgButtonUp == true ){
		;	// OK
	}else{
		return;	// UPイベントがほしいので、何もしない
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginMenu plugin_menu;

	onContextMenu( evt );

	return;
}

void SpectrumCanvas::onContextMenu( wxContextMenuEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

void SpectrumCanvas::onContextMenu( wxMouseEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

void SpectrumCanvas::onContextMenu( wxPoint ptScreen ){
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


void SpectrumCanvas::onMenuPopup( wxCommandEvent& evt )
{
	// ID
	int nEventId = evt.GetId();

	// object
	kome::objects::Variant obj = getActiveObject();

	kome::plugin::PluginMenu* ptrMenu = kome::window::Canvas::m_contextMenuTreeMap[nEventId];

	bool flgRet = ptrMenu->execute( obj );

	if( flgRet ){
		WindowTool::refresh();
	}

	return;
}
