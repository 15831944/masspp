/**
 * @file DataMapCanvas.cpp
 * @brief implements of DataMapCanvas class
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataMapCanvas.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( DataMapCanvas, kome::window::Canvas )
END_EVENT_TABLE()


#define DEFAULT_GRAPH_MARGIN_LEFT			0
#define DEFAULT_GRAPH_MARGIN_TOP			0
#define DEFAULT_GRAPH_MARGIN_RIGHT			0
#define DEFAULT_GRAPH_MARGIN_BOTTOM			0
#define DEFAULT_MARGIN_LEFT				   50
#define DEFAULT_MARGIN_TOP				   10
#define DEFAULT_MARGIN_RIGHT			   20
#define DEFAULT_MARGIN_BOTTOM			   35

#define MINIMUM_DRAW_SIZE				   30


// static member
std::set< DataMapCanvas* > DataMapCanvas::m_canvasSet;


// constructor
DataMapCanvas::DataMapCanvas( wxWindow* parent, kome::objects::DataMapInfo& dataMap )
		: kome::window::Canvas( parent, wxID_ANY, parent->GetClientSize().GetWidth(), parent->GetClientSize().GetHeight() ) {
	// initialize
	setDataMap( &dataMap );
	m_canvasSet.insert( this );

	kome::objects::Variant obj;
	obj.type = kome::objects::Variant::DATA_MAP;
	obj.prim.pt = &dataMap;
	setActiveObject( obj );
}

// destructor
DataMapCanvas::~DataMapCanvas() {
	m_canvasSet.erase( this );
}

// updates data map
void DataMapCanvas::updatePoints( kome::objects::DataMapInfo* dataMap ) {
	// check the member
	double minX = 0.0;
	double maxX = 0.0;
	double minY = 0.0;
	double maxY = 0.0;

	// get xy data
	if( dataMap != NULL ) {
		minX = dataMap->getMinRt();
		maxX = dataMap->getMaxRt();
		minY = dataMap->getMinMz();
		maxY = dataMap->getMaxMz();
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

// on draw
void DataMapCanvas::onDraw( wxDC& dc ) {
	// data map
	kome::objects::DataMapInfo* dataMap = getDataMap();

	// set device context
	m_graphics.setDC( &dc );

	// prepare
	kome::plugin::PluginCallTool::prepareDrawDataMap( *dataMap, m_graphics, this );

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
void DataMapCanvas::onSize( const unsigned int width, const unsigned int height ) {
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

// on mouse button down
bool DataMapCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	return kome::plugin::PluginCallTool::onDataMapButtonDown( *dataMap, &m_graphics, evt, this );
}

// on mouse button up	// @date 2012/11/06 <Mod> OKADA
bool DataMapCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	if( kome::plugin::PluginCallTool::onDataMapButtonUp( *dataMap, &m_graphics, evt, this ) ){
		return true;
	}
	else if( evt.rbutton()
			&& evt.x() >= 0 && evt.x() <= m_width && evt.y() >= 0 && evt.y() <= m_height ) {
		popupMenu( evt.x(), evt.y() );
	}
	return false;
}

// on mouse button double click
bool DataMapCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	return kome::plugin::PluginCallTool::onDataMapDoubleClick( *dataMap, &m_graphics, evt, this );
}

// on mouse cursor move
bool DataMapCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	return kome::plugin::PluginCallTool::onDataMapMouseMove( *dataMap, &m_graphics, evt, this );
}

// on mouse cursor enter
bool DataMapCanvas::onMouseCursorEnter( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	return kome::plugin::PluginCallTool::onDataMapMouseEnter( *dataMap, &m_graphics, evt, this );
}

// on mouse cursor leave
bool DataMapCanvas::onMouseCursorLeave( kome::evt::MouseEvent& evt ) {
	kome::objects::DataMapInfo* dataMap = getDataMap();
	return kome::plugin::PluginCallTool::onDataMapMouseLeave( *dataMap, &m_graphics, evt, this );
}

// on draw
void DataMapCanvas::onDrawGraphics( wxDC& dc ) {
	// data map
	kome::objects::DataMapInfo* dataMap = getDataMap();

	// set dc
	m_graphics.setDC( &dc );
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );

	// window background
	kome::plugin::PluginCallTool::drawDataMapWindowBg( *dataMap, m_graphics, this );

	// memory dc
	wxMemoryDC memDc;
	wxBitmap bitmap( m_graphics.getGraphWidth(), m_graphics.getGraphHeight() );
	memDc.SelectObject( bitmap );
	memDc.SetBackground( *wxWHITE_BRUSH );
	memDc.Clear();

	m_graphics.setDC( &memDc );
	m_graphics.setTranslate(
		- m_graphics.getGraphPositionLeft(),
		- m_graphics.getGraphPositionTop()
	);

	// background
	kome::plugin::PluginCallTool::drawDataMapBg( *dataMap, m_graphics, this );

	// data map
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_BUFFERED_AND );
	kome::plugin::PluginCallTool::drawDataMap( *dataMap, m_graphics, this );

	// foreground
	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );
	kome::plugin::PluginCallTool::drawDataMapFg( *dataMap, m_graphics, this );

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

	// window foreground
	m_graphics.setDC( &dc );
	m_graphics.resetTransform();

	kome::plugin::PluginCallTool::drawDataMapWindowFg( *dataMap, m_graphics, this );

	// memory dc
	memDc.SelectObject( wxNullBitmap );
}

// on add data map
kome::objects::DataMapInfo* DataMapCanvas::onAddDataMap( kome::objects::DataMapInfo* dataMap ) {
	// clear
	clearDataMaps();

	// update points
	updatePoints( dataMap );

	// active object
	kome::objects::Variant obj;
	obj.type = kome::objects::Variant::DATA_MAP;
	obj.prim.pt = dataMap;
	setActiveObject( obj );

	return dataMap;
}

// on remove data map
void DataMapCanvas::onRemoveDataMap( kome::objects::DataMapInfo* dataMap ) {
	dataMap = NULL;	// warning C4100	// @date 2013/06/10 <Add> OKADA
}

// get canvas
DataMapCanvas* DataMapCanvas::getCanvas( kome::objects::DataMapInfo* dataMap ) {
	
	// search
	for( std::set< DataMapCanvas* >::iterator it = m_canvasSet.begin();
			it != m_canvasSet.end(); it++ ) {
		DataMapCanvas* canvas = *it;
		if( dataMap == canvas->getDataMap() ) {
			return canvas;
		}
	}
	return NULL;
}

// conbert dot to RT or m/z.
int DataMapCanvas::convertPos2Data( double x, double y, double * p_dX, double * p_dY )
{
	m_graphics.transformPositionToData( x, y, p_dX, p_dY );

	return 0;
}

// conbert RT or m/z to pixel.
int DataMapCanvas::convertData2Pos( double dMz, double dRt, double * p_dX, double * p_dY )
{
	m_graphics.transformDataToPosition( dMz, dRt, p_dX, p_dY );

	return 0;
}


// conbert RT or m/z to pixel.
int DataMapCanvas::getGraphPosition( int *top, int *bottom, int *left, int *right )
{
	*top	= m_graphics.getGraphPositionTop();
	*bottom	= m_graphics.getGraphPositionBottom();
	*left	= m_graphics.getGraphPositionLeft();
	*right	= m_graphics.getGraphPositionRight();	

	return 0;
}

bool DataMapCanvas::isTooSmall( void ){
	if( m_graphics.getGraphWidth() < MINIMUM_DRAW_SIZE
		|| m_graphics.getGraphHeight() < MINIMUM_DRAW_SIZE ) {
		return true;
	}

	return false;
}

// creates menu
wxMenu* DataMapCanvas::createMenu()
{
	bool flgSubView = isSubView();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	wxMenu* menu = NULL;

	if( flgSubView ){
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getDataMapSubViewMenu();
		menu = createPopupMenu( &plugin_menu );
	}else{
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getDataMapViewMenu();
		menu = createPopupMenu( &plugin_menu );
	}

	return menu;
}
