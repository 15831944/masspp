/**
 * @file ScaleDrawer.cpp
 * @brief implements of ScaleDrawer class
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ScaleDrawer.h"

#include <list>
#include <math.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_FONT_SIZE			8
#define SCALE_LENGTH				6
#define TEXT_SPACE					5
#define MIN_SCALE_SPACE				10

#define SECTION						"Drawing"
#define WIDTH_AXIS					"WIDTH_AXIS"
#define COLOR_AXIS					"COLOR_AXIS"
#define FONT_AXIS					"FONT_AXIS"
#define COLOR_DEF_AXIS				"black"

// constructor
ScaleDrawer::ScaleDrawer() : m_font( DEFAULT_FONT_SIZE ) {
}

// destructor
ScaleDrawer::~ScaleDrawer() {
}

// prepare to draw
void ScaleDrawer::prepareToDraw( kome::img::Graphics* g ) {
	// font
	kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, FONT_AXIS );
	g->setFont( font );
	
	// prepare margin
	int leftMargin = TEXT_SPACE + SCALE_LENGTH;
	int topMargin = TEXT_SPACE;
	int rightMargin = 20;
	int bottomMargin = TEXT_SPACE + SCALE_LENGTH;

	// get title size
	int titleWidth = 0;
	int titleHeight = 0;
	g->getTextSize( "|title|", &titleWidth, &titleHeight );

	leftMargin += ( titleHeight + TEXT_SPACE );
	bottomMargin += ( titleHeight + TEXT_SPACE );
	topMargin += titleHeight / 2;

	// get y scale information
	leftMargin += kome::img::GraphTool::getMaxWidthOfYLabels( *g, MIN_SCALE_SPACE );
	bottomMargin += titleHeight;

	leftMargin = std::max( leftMargin, (int)g->getGraphPositionLeft() );
	topMargin = std::max( topMargin, (int)g->getGraphPositionTop() );
	rightMargin = std::max( rightMargin, (int)(g->getWidth() - g->getGraphPositionRight() ) );
	bottomMargin = std::max( bottomMargin, (int)( g->getHeight() - g->getGraphPositionBottom() ) );

	// set position
	g->setGraphPosition(
		leftMargin,
		topMargin,
		g->getWidth() - rightMargin,
		g->getHeight() - bottomMargin
	);
}

// draw spectrum scale
void ScaleDrawer::drawScale( kome::objects::Spectrum* spec, kome::img::Graphics* g ) {
	// group
	kome::objects::DataGroupNode* group = spec->getGroup();

	// draw scale
	drawScale(
		g,
		( group == NULL ? "m/z" : group->getSpecXTitle() ),
		( group == NULL ? "intensity" : group->getSpecYTitle() )
	);
}

// draw chromatogram scale
void ScaleDrawer::drawScale( kome::objects::Chromatogram* chrom, kome::img::Graphics* g ) {
	// group
	kome::objects::DataGroupNode* group = chrom->getGroup();

	// draw scale
	drawScale(
		g,
		( group == NULL ? "rt" : group->getChromXTitle() ),
		( group == NULL ? "intensity" : group->getChromYTitle() )
	);
}

// draw scale
void ScaleDrawer::drawScale( kome::objects::DataMapInfo* dataMap, kome::img::Graphics* g ) {
	// title
	drawScale( g, "m/z", "RT" );
}

// draw scale
void ScaleDrawer::drawScale( kome::img::Graphics* g, const char* xTitle, const char* yTitle ) {
	// set graphics
	// フォントの設定をiniファイルから取得する為の処理追加 @date 2013.04.26 <Add> M.izumi ->
	kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, FONT_AXIS );
	g->setFont( font );

	COLOR oldColor = g->getColor();
	kome::img::Font	oldFont = g->getFont();

	if( !g->isUnusedDefaultColor() ){
		g->setColor( kome::img::GraphTool::getColorFromIni( SECTION, COLOR_AXIS, COLOR_DEF_AXIS ));
	}else{
		g->setColor( kome::img::ColorTool::BLACK );
	}
	if( !g->isUnusedDefaultThickness() ){
		g->setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION, WIDTH_AXIS ) );
	}
	// draw rect
	kome::img::GraphTool::drawGraphRect( *g );

	// draw x title
	g->setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g->setTextVAlign( kome::img::Graphics::VALIGN_BOTTOM );
	g->drawText(
		xTitle,
		( g->getGraphPositionLeft() + g->getGraphPositionRight() ) / 2,
		g->getHeight() - 2 * TEXT_SPACE,
		0
	);

	// draw y title
	g->setTextHAlign( kome::img::Graphics::HALIGN_CENTER );
	g->setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g->drawText(
		yTitle,
		TEXT_SPACE,
		( g->getGraphPositionTop() + g->getGraphPositionBottom() ) / 2,
		90
	);

	// draw x scale
	kome::img::GraphTool::drawXScale( *g, MIN_SCALE_SPACE, SCALE_LENGTH, TEXT_SPACE, false );

	// draw y scale
	kome::img::GraphTool::drawYScale( *g, MIN_SCALE_SPACE, SCALE_LENGTH, TEXT_SPACE, false );

	// restore color
	g->setColor( oldColor );
	// restore font
	g->setFont( oldFont );
}

// get instance
ScaleDrawer& ScaleDrawer::getInstance() {
	// create object (This is the only object.)
	static ScaleDrawer drawer;

	return drawer;
}

