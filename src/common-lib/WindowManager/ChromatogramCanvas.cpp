/**
 * @file ChromatogramCanvas.cpp
 * @brief implements of Chromatogram Canvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramCanvas.h"

#include "WindowTool.h"

#include <list>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CHROMATOGRAM_PROFILE_COLOR	   kome::img::ColorTool::BLUE
#define DRAW_CHROM_FUNC_TYPE			 "DRAW_CHROM"


BEGIN_EVENT_TABLE( ChromatogramCanvas, ProfileCanvas )
	EVT_CONTEXT_MENU( ChromatogramCanvas::onContextMenu )	// for context menu	// @date 2012/12/07 <Add> OKADA
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, ChromatogramCanvas::onMenuPopup )
END_EVENT_TABLE()


// static member
std::set< ChromatogramCanvas* > ChromatogramCanvas::m_canvasSet;


// constructor
ChromatogramCanvas::ChromatogramCanvas(
		wxWindow* parent,
		const int width,
		const int height,
		kome::objects::Chromatogram* chrom
) : ProfileCanvas( parent, width, height ) {
	// initialize
	setChromatogram( chrom );
	m_canvasSet.insert( this );

	setProfileColor( CHROMATOGRAM_PROFILE_COLOR );
}

// destructor
ChromatogramCanvas::~ChromatogramCanvas() {
	m_canvasSet.erase( this );
}

// get base chromatogram
kome::objects::Chromatogram* ChromatogramCanvas::getBaseChromatogram() {
	return getChromatogram();
}

// on mouse button down
bool ChromatogramCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramButtonDown( *chrom, &m_graphics, evt, this );
}

// on mouse button up
bool ChromatogramCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();

	if( kome::plugin::PluginCallTool::onChromatogramButtonUp( *chrom, &m_graphics, evt, this ) ) {
		return true;
	}
	else if( evt.rbutton()
			&& evt.x() >= 0 && evt.x() <= m_width && evt.y() >= 0 && evt.y() <= m_height  ) {
		popupMenu( evt.x(), evt.y() );
	}
	return false;
}

// on mouse button double click
bool ChromatogramCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramDoubleClick( *chrom, &m_graphics, evt, this );
}

// on mouse cursor move
bool ChromatogramCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramMouseMove( *chrom, &m_graphics, evt, this );
}

// on mouse cursor enter
bool ChromatogramCanvas::onMouseCursorEnter( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramMouseEnter( *chrom, &m_graphics, evt, this );
}

// on mouse cursor leave
bool ChromatogramCanvas::onMouseCursorLeave( kome::evt::MouseEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramMouseLeave( *chrom, &m_graphics, evt, this );
}

// on Key Down
bool ChromatogramCanvas::onKeyDown( kome::evt::KeyEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramKeyDown( *chrom, &m_graphics,evt, this );
}

// on Key Up
bool ChromatogramCanvas::onKeyUp( kome::evt::KeyEvent& evt ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	return kome::plugin::PluginCallTool::onChromatogramKeyUp( *chrom, &m_graphics, evt, this );
}


// on prepare
void ChromatogramCanvas::onPrepareDraw( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	kome::plugin::PluginCallTool::prepareDrawChromatogram( *chrom, g, this );
}

// on draw window background
void ChromatogramCanvas::onDrawWindowBg( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	kome::plugin::PluginCallTool::drawChromatogramWindowBg( *chrom, g, this );
}

// on draw graph background
void ChromatogramCanvas::onDrawGraphBg( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	kome::plugin::PluginCallTool::drawChromatogramBg( *chrom, g, this );
}

// on draw profile
void ChromatogramCanvas::onDrawProfile( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	onDrawChromatogramProfile( g, *chrom );
}

// on draw graph foreground
void ChromatogramCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	kome::plugin::PluginCallTool::drawChromatogramFg( *chrom, g, this );
}

// on draw window foreground
void ChromatogramCanvas::onDrawWindowFg( kome::window::DCGraphics& g ) {
	kome::objects::Chromatogram* chrom = getBaseChromatogram();
	kome::plugin::PluginCallTool::drawChromatogramWindowFg( *chrom, g, this );
}

// on draw chromatogram profile
void ChromatogramCanvas::onDrawChromatogramProfile(
		kome::window::DCGraphics& g,
		kome::objects::Chromatogram& chrom
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
			if(strcmp(plgcall->getType(), DRAW_CHROM_FUNC_TYPE) == 0 && plgcall->checkStatus()) {
				calls.push_back(plgcall);
			}
		}
	}

	// parameter
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setChromatogram( params, chrom );
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

// on add chromatogram
kome::objects::Chromatogram* ChromatogramCanvas::onAddChromatogram( kome::objects::Chromatogram* chrom ) {
	// check
	for( unsigned int i = 0; i < getNumberOfChromatograms(); i++ ) {
		if( getChromatogram( i ) == chrom ) {
			return NULL;
		}
	}

	// clear
	clearObjects();
	
	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( chrom );	// @date 2011.02.10 <Add> M.Izumi

	return chrom;
}

// on remove chromatogram
void ChromatogramCanvas::onRemoveChromatogram( kome::objects::Chromatogram* chrom ) {
}

// get canvas
ChromatogramCanvas* ChromatogramCanvas::getCanvas( kome::objects::Chromatogram* chrom ) {
	// search
	for( std::set< ChromatogramCanvas* >::iterator it = m_canvasSet.begin();
			it != m_canvasSet.end(); it++ ) {
		ChromatogramCanvas* canvas = *it;
		if( chrom == canvas->getBaseChromatogram() ) {
			return canvas;
		}
	}
	return NULL;
}

// destroy all canvas
void ChromatogramCanvas::destroyAllCanvas(){
	// store to array
	std::vector< ChromatogramCanvas* > canvases;
	for( std::set< ChromatogramCanvas* >::iterator it = m_canvasSet.begin();
			it != m_canvasSet.end(); it++ ) {
		canvases.push_back( *it );
	}

	// destroy
	for( unsigned int i = 0; i < canvases.size(); i++ ) {
		canvases[ i ]->Destroy();
	}
}

// creates menu
wxMenu* ChromatogramCanvas::createMenu()
{
	bool flgSubView = isSubView();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
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

	return menu;
}

// on mouse right down
void ChromatogramCanvas::onMouseRightDown( wxMouseEvent& evt )
{
	return;
}

// on mouse right up
void ChromatogramCanvas::onMouseRightUp( wxMouseEvent& evt )
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

// on context menu
void ChromatogramCanvas::onContextMenu( wxContextMenuEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void ChromatogramCanvas::onContextMenu( wxMouseEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	onContextMenu( ptScreen );
}

// on context menu
void ChromatogramCanvas::onContextMenu( wxPoint ptScreen ){
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
void ChromatogramCanvas::onMenuPopup( wxCommandEvent& evt )
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
