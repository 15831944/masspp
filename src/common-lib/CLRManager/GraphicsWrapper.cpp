/**
 * @file GraphicsWrapper.cpp
 * @brief implements of GraphicsWrapper class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GraphicsWrapper.h"

#include "FontWrapper.h"
#include "ClrObjectTool.h"
#include "ConversionArrays.h"
#include "BitmapDataWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
GraphicsWrapper::GraphicsWrapper( kome::img::Graphics& g ) : m_graphics( g ) {
}

// destructor
GraphicsWrapper::~GraphicsWrapper() {
}

// get Graphics object
kome::img::Graphics& GraphicsWrapper::getGraphics() {
	return m_graphics;
}

// set the position of the graph
void GraphicsWrapper::setGraphPosition(
		int left,
		int top,
		int right,
		int bottom
) {
	m_graphics.setGraphPosition( left, top, right, bottom );
}

// set x coordinate in left end of graph
void GraphicsWrapper::setGraphPositionLeft( int left ) {
	m_graphics.setGraphPositionLeft( left );
}

// set y coordinate in top end of graph
void GraphicsWrapper::setGraphPositionTop( int top ) {
	m_graphics.setGraphPositionTop( top );
}

// set x coordinate in right end of graph
void GraphicsWrapper::setGraphPositionRight( int right ) {
	m_graphics.setGraphPositionRight( right );

}

// set y coordinate in bottom end of graph
void GraphicsWrapper::setGraphPositionBottom( int bottom ) {
	m_graphics.setGraphPositionBottom( bottom );
}

// get x coordinate in left end of graph
int GraphicsWrapper::getGraphPositionLeft() {
	return m_graphics.getGraphPositionLeft();

}

// get y coordinate in top end of graph
int GraphicsWrapper::getGraphPositionTop() {
	return m_graphics.getGraphPositionTop();
}

// get x coordinate in right end of graph
int GraphicsWrapper::getGraphPositionRight() {
	return m_graphics.getGraphPositionRight();
}

// get y coordinate in bottom end of graph
int GraphicsWrapper::getGraphPositionBottom() {
	return m_graphics.getGraphPositionBottom();
}

// set graph margin information
void GraphicsWrapper::setGraphMargin(
		int left,
		int top,
		int right,
		int bottom
) {
	m_graphics.setGraphMargin( left, top, right, bottom );
}

// set length of margin left in graph
void GraphicsWrapper::setGraphMarginLeft( int left ) {
	m_graphics.setGraphMarginLeft( left );
}

// set length of margin top in graph
void GraphicsWrapper::setGraphMarginTop( int top ) {
	m_graphics.setGraphMarginTop( top );
}

// sets length of margin right in graph
void GraphicsWrapper::setGraphMarginRight( int right ) {
	m_graphics.setGraphMarginRight( right );
}

// set length of margin bottom in graph
void GraphicsWrapper::setGraphMarginBottom( int bottom ){
	m_graphics.setGraphMarginBottom( bottom );
}

// get length of margin left in graph
int GraphicsWrapper::getGraphMarginLeft() {
	return m_graphics.getGraphMarginLeft();
}

// get length of margin top in graph
int GraphicsWrapper::getGraphMarginTop() {
	return m_graphics.getGraphMarginTop();
}

// get length of margin right in graph
int GraphicsWrapper::getGraphMarginRight() {
	return m_graphics.getGraphMarginRight();
}

// get length of margin bottom in graph
int GraphicsWrapper::getGraphMarginBottom() {
	return m_graphics.getGraphMarginBottom();
}

// set size of drawing area
void GraphicsWrapper::setSize( unsigned int width, unsigned int height ) {
	m_graphics.setSize( width, height );
}

// get width of drawing area
unsigned int GraphicsWrapper::getWidth() {
	return m_graphics.getWidth();
}

// get height of drawing area
unsigned int GraphicsWrapper::getHeight() {
	return m_graphics.getHeight();
}

// get width of graph drawing area
int GraphicsWrapper::getGraphWidth() {
	return m_graphics.getGraphWidth();
}

// get height of graph drawing area
int GraphicsWrapper::getGraphHeight() {
	return m_graphics.getGraphHeight();
}

// set range of data
void GraphicsWrapper::setDisplayedRange(
		double minX,
		double maxX,
		double minY,
		double maxY
) {
	m_graphics.setDisplayedRange( minX, maxX, minY, maxY );

}

// set minimum x of displayed range
void GraphicsWrapper::setMinX( double minX ) {
	m_graphics.setMinX( minX );
}

// set maximum x of displayed range
void GraphicsWrapper::setMaxX( double maxX ) {
	m_graphics.setMaxX( maxX );
}

// set minimum y of displayed range
void GraphicsWrapper::setMinY( double minY ) {
	m_graphics.setMinY( minY );
}

// set maximum y of displayed range
void GraphicsWrapper::setMaxY( double maxY ) {
	m_graphics.setMaxY( maxY );
}

// get minimum x of displayed range
double GraphicsWrapper::getMinX() {
	return m_graphics.getMinX();
}

// get maximum x of displayed range
double GraphicsWrapper::getMaxX() {
	return m_graphics.getMaxX();
}

// get minimum y of displayed range
double GraphicsWrapper::getMinY() {
	return m_graphics.getMinY();
}

// get maximum y of displayed range
double GraphicsWrapper::getMaxY() {
	return m_graphics.getMaxY();
}

// set scale x
void GraphicsWrapper::setScaleX( double scX ) {
	m_graphics.setScaleX( scX );
}

// set scale y
void GraphicsWrapper::setScaleY( double scY ) {
	m_graphics.setScaleY( scY );
}

// set scale
void GraphicsWrapper::setScale( double scX, double scY ) {
	m_graphics.setScale( scX, scY );
}

// get scale x
double GraphicsWrapper::getScaleX() {
	return m_graphics.getScaleX();
}

// get scale y
double GraphicsWrapper::getScaleY() {
	return m_graphics.getScaleY();
}

// set shear x
void GraphicsWrapper::setShearX( double shX ) {
	m_graphics.setShearX( shX );
}

// set shear y
void GraphicsWrapper::setShearY( double shY ) {
	m_graphics.setShearY( shY );
}

// set shear
void GraphicsWrapper::setShear( double shX, double shY ) {
	m_graphics.setShear( shX, shY );
}

// get shear x
double GraphicsWrapper::getShearX() {
	return m_graphics.getShearX();
}

// get shear y
double GraphicsWrapper::getShearY() {
	return m_graphics.getShearY();
}

// set translate x
void GraphicsWrapper::setTranslateX( double trX ) {
	m_graphics.setTranslateX( trX );
}

// set translate y
void GraphicsWrapper::setTranslateY( double trY ) {
	m_graphics.setTranslateY( trY );
}

// set translate
void GraphicsWrapper::setTranslate( double trX, double trY ) {
	m_graphics.setTranslate( trX, trY );
}

// get translate x
double GraphicsWrapper::getTranslateX() {
	return m_graphics.getTranslateX();
}

// get translate y
double GraphicsWrapper::getTranslateY() {
	return m_graphics.getTranslateY();
}

// reset transfer matrix
void GraphicsWrapper::resetTransform() {
	m_graphics.resetTransform();
}

// transform data to position
void GraphicsWrapper::transformDataToPosition(
		double x,
		double y,
		double% px,
		double% py
) {
	double tmpPx = double();
	double tmpPy = double();

	m_graphics.transformDataToPosition( x, y, &tmpPx, &tmpPy );

	px = tmpPx;
	py = tmpPy;
}

// transform position to data
void GraphicsWrapper::transformPositionToData(
		double px,
		double py,
		double% x,
		double% y
) {
	double tmpX = double();
	double tmpY = double();

	m_graphics.transformPositionToData( px, py, &tmpX, &tmpY );

	x = tmpX;
	y = tmpY;
}

// set draw style 
void GraphicsWrapper::setDrawStyle( DrawStyle style ) {
	kome::img::Graphics::DrawStyle s
		= INVERT_FROM_ARRAY( style, kome::img::Graphics::STYLE_SOLID, g_graphicsDrawStyles );
	m_graphics.setDrawStyle( s );
}

// get draw style
GraphicsWrapper::DrawStyle GraphicsWrapper::getDrawStyle() {
	DrawStyle s = CONVERT_FROM_ARRAY( m_graphics.getDrawStyle(), DrawStyle::SOLID, g_graphicsDrawStyles );
	return s;
}

// set fill pattern
void GraphicsWrapper::setFillPattern( FillPattern pattern ) {
	kome::img::Graphics::FillPattern p
		= INVERT_FROM_ARRAY( pattern, kome::img::Graphics::PATTERN_SOLID, g_graphicsFillPatterns );
	m_graphics.setFillPattern( p );
}

// get fill pattern
GraphicsWrapper::FillPattern GraphicsWrapper::getFillPattern() {
	FillPattern p = CONVERT_FROM_ARRAY( m_graphics.getFillPattern(), FillPattern::SOLID, g_graphicsFillPatterns );
	return p;
}

// set fill style
void GraphicsWrapper::setFillStyle( FillStyle style ) {
	kome::img::Graphics::FillStyle s
		= INVERT_FROM_ARRAY( style, kome::img::Graphics::RULE_ODDEVEN, g_graphicsFillStyles );
	m_graphics.setFillStyle( s );
}

// get fill style
GraphicsWrapper::FillStyle GraphicsWrapper::getFillStyle() {
	FillStyle s = CONVERT_FROM_ARRAY( m_graphics.getFillStyle(), FillStyle::ODDEVEN, g_graphicsFillStyles );
	return s;
}

// set text horizontal align
void GraphicsWrapper::setTextHAlign( TextHAlign hAlign ) {
	kome::img::Graphics::TextHAlign ha
		= INVERT_FROM_ARRAY( hAlign, kome::img::Graphics::HALIGN_LEFT, g_graphicsHAligns );
	m_graphics.setTextHAlign( ha );
}

// get text horizontal align
GraphicsWrapper::TextHAlign GraphicsWrapper::getTextHAlign() {
	TextHAlign ha = CONVERT_FROM_ARRAY( m_graphics.getTextHAlign(), TextHAlign::LEFT, g_graphicsHAligns );
	return ha;
}

// set text vertical align
void GraphicsWrapper::setTextVAlign( TextVAlign vAlign ) {
	kome::img::Graphics::TextVAlign va
		= INVERT_FROM_ARRAY( vAlign, kome::img::Graphics::VALIGN_TOP, g_graphicsVAligns );
	m_graphics.setTextVAlign( va );
}

// get text vertical align
GraphicsWrapper::TextVAlign GraphicsWrapper::getTextVAlign() {
	TextVAlign va = CONVERT_FROM_ARRAY( m_graphics.getTextVAlign(), TextVAlign::TOP, g_graphicsVAligns );
	return va;
}

// set color
void GraphicsWrapper::setColor( System::Drawing::Color color ) {
	COLOR c = kome::img::ColorTool::getColor(
		color.R,
		color.G,
		color.B
	);
	m_graphics.setColor( c );
}

// get color
System::Drawing::Color GraphicsWrapper::getColor() {
	COLOR c = m_graphics.getColor();

	System::Drawing::Color color = System::Drawing::Color::FromArgb(
		0xff,
		kome::img::ColorTool::getRed( c ),
		kome::img::ColorTool::getGreen( c ),
		kome::img::ColorTool::getBlue( c )
	);

	return color;
}

// set font
void GraphicsWrapper::setFont( FontWrapper^ font ) {
	m_graphics.setFont( font->getFont() );
}

// get font
FontWrapper^ GraphicsWrapper::getFont() {
	return gcnew FontWrapper( m_graphics.getFont() );
}

// set thickness line
void GraphicsWrapper::setThicknessLine( int width ){
	m_graphics.setThicknessLine( width );
}

// get thickness line
int GraphicsWrapper::getThicknessLine(){
	return m_graphics.getThicknessLine();
}

// draw line
void GraphicsWrapper::drawLine( int px0, int py0, int px1, int py1 ) {
	m_graphics.drawLine( px0, py0, px1, py1 );
}

// draws line
void GraphicsWrapper::drawLines(
		cli::array< int, 2 >^ points
) {
	// create array
	int* ptArray = NULL;
	int num = points->GetLength( 0 );
	if( num > 0 ) {
		ptArray = new int[ num * 2 ];
		for( int i = 0; i < num; i++ ) {
			ptArray[ i * 2 ] = points[ i, 0 ];
			ptArray[ i * 2 + 1 ] = points[ i, 1 ];
		}
	}

	// draw
	m_graphics.drawLines( ptArray, num );

	// delete array
	if( ptArray != NULL ) {
		delete[] ptArray;
	}
}

// fill rect
void GraphicsWrapper::fillRect(
		int px,
		int py,
		int width,
		int height
) {
	// draw
	m_graphics.fillRect( px, py, width, height );
}

// fill polygon
void GraphicsWrapper::fillPolygon(
		cli::array< int, 2 >^ points
) {
	// create array
	int* ptArray = NULL;
	int num = points->GetLength( 0 );
	if( num > 0 ) {
		ptArray = new int[ num * 2 ];
		for( int i = 0; i < num; i++ ) {
			ptArray[ i * 2 ] = points[ i, 0 ];
			ptArray[ i * 2 + 1 ] = points[ i, 1 ];
		}
	}

	// fill
	m_graphics.fillPolygon( ptArray, num );

	// delete array
	if( ptArray != NULL ) {
		delete[] ptArray;
	}
}

// draw circle
void GraphicsWrapper::drawCircle( int cx, int cy, int r, bool fill ) {
	m_graphics.drawCircle( cx, cy, r, fill );
}

// draw arc
void GraphicsWrapper::drawArc( int cx, int cy, int r, double start, double end, bool fill ) {
	m_graphics.drawArc( cx, cy, r, start, end, fill );
}

// draw ellipse
void GraphicsWrapper::drawEllipse( int px, int py, int width, int height, bool fill ) {
	m_graphics.drawEllipse( px, py, width, height, fill );
}

// draw elliptic arc
void GraphicsWrapper::drawEllipticArc(
		int px,
		int py,
		int width,
		int height,
		double start,
		double end,
		bool fill
) {
	m_graphics.drawEllipticArc( px, py, width, height, start, end, fill );
}

// draws text
void GraphicsWrapper::drawText(
		System::String^ str,
		int px,
		int py,
		int angle
) {
	// string
	std::string s = ClrObjectTool::convertString( str );

	// draw text
	m_graphics.drawText( s.c_str(), px, py, angle );
}

// get text size
void GraphicsWrapper::getTextSize( System::String^ str, int% width, int% height ) {
	// string
	std::string s = ClrObjectTool::convertString( str );

	// get size
	int w = 0;
	int h = 0;
	m_graphics.getTextSize( s.c_str(), &w, &h );

	// store
	width = w;
	height = h;
}

// draw point
void GraphicsWrapper::drawPoint( int px, int py ) {
	m_graphics.drawPoint( px, py );
}

// draw bitmap
void GraphicsWrapper::drawBitmap( BitmapDataWrapper^ bitmap, int px, int py, bool transparent ) {
	if( bitmap != nullptr ) {
		m_graphics.drawBitmap( bitmap->getBitmapData(), px, py, transparent );
	}
}

// draw mark
void GraphicsWrapper::drawMark( Mark mark, int px, int py, int r ) {
	kome::img::Graphics::Mark m
		= INVERT_FROM_ARRAY( mark, kome::img::Graphics::MARK_NONE, g_graphicsMarks );
	m_graphics.drawMark( m, px, py, r );
}

// get pixel
System::Drawing::Color GraphicsWrapper::getPixel( int px, int py ) {
	COLOR c = m_graphics.getPixel( px, py );

	System::Drawing::Color color = System::Drawing::Color::FromArgb(
		0xff,
		kome::img::ColorTool::getRed( c ),
		kome::img::ColorTool::getGreen( c ),
		kome::img::ColorTool::getBlue( c )
	);

	return color;
}

// flush
void GraphicsWrapper::flush() {
	m_graphics.flush();
}
