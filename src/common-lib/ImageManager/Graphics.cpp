/**
 * @file Graphics.cpp
 * @brief implements of Graphics class
 *
 * @author S.Tanaka
 * @date 2006.09.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Graphics.h"

using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FONT_DEFAULT_SIZE 8

// constructor
Graphics::Graphics() : m_tr( kome::numeric::MatrixTool::getIdentityMatrix( 3 ) ),
		m_trInv( kome::numeric::MatrixTool::getIdentityMatrix( 3 ) ),
		m_ctr( kome::numeric::MatrixTool::getIdentityMatrix( 3 ) ),
		m_font( 8 ) {
	// init
	m_width = 0;
	m_height = 0;

	fillzero( &m_graphPos, sizeof( m_graphPos ) );
	fillzero( &m_graphMargin, sizeof( m_graphMargin ) );
	fillzero( &m_range, sizeof( m_range ) );

	// transformation
	resetTransform();

	m_trFlg = false;

	m_drawStyle   = STYLE_SOLID;
	m_fillPattern = PATTERN_SOLID;
	m_fillStyle   = RULE_ODDEVEN;
	m_hAlign	  = HALIGN_LEFT;
	m_vAlign	  = VALIGN_TOP;
	m_color	   = ColorTool::BLACK;

	m_thickness = 1;	// @date 2013.04.23 <Add> M.Izumi
	m_bUnusedColor = false;
	m_bUnusedThickness = false;
}

// destructor
Graphics::~Graphics() {
}

// set graph position
void Graphics::setGraphPosition(
		const int left,
		const int top,
		const int right,
		const int bottom
) {
	// set position
	setGraphPositionLeft( left );
	setGraphPositionTop( top );
	setGraphPositionRight( right );
	setGraphPositionBottom( bottom );	
}

// set x coordinate in left end of graph
void Graphics::setGraphPositionLeft( const int left ) {
	if( left != m_graphPos.left ) {
		m_graphPos.left = left;
		m_trFlg = false;
	}
}

// set y coordinate in top end of graph
void Graphics::setGraphPositionTop( const int top ) {
	if( top != m_graphPos.top ) {
		m_graphPos.top = top;
		m_trFlg = false;
	}
}

// set x coordinate in right end of graph
void Graphics::setGraphPositionRight( const int right ) {
	if( right != m_graphPos.right ) {
		m_graphPos.right = right;
		m_trFlg = false;
	}
}

// set y coordinate in bottom end of graph
void Graphics::setGraphPositionBottom( const int bottom ) {
	if( bottom != m_graphPos.bottom ) {
		m_graphPos.bottom = bottom;
		m_trFlg = false;
	}
}

// get x coordinate in left end of graph
int Graphics::getGraphPositionLeft() {
	return m_graphPos.left;
}

// get y coordinate in top end of graph
int Graphics::getGraphPositionTop() {
	return m_graphPos.top;
}

// get x coordinate in right end of graph
int Graphics::getGraphPositionRight() {
	return m_graphPos.right;
}

// get y coordinate in bottom end of graph
int Graphics::getGraphPositionBottom() {
	return m_graphPos.bottom;
}

// set graph margin
void Graphics::setGraphMargin(
	const int left,
	const int top,
	const int right,
	const int bottom
) {
	// set margin
	setGraphMarginLeft( left );
	setGraphMarginTop( top );
	setGraphMarginRight( right );
	setGraphMarginBottom( bottom );
}

// set length of margin left in graph
void Graphics::setGraphMarginLeft( const int left ) {
	if( m_graphMargin.left != left ) {
		m_graphMargin.left = left;
		m_trFlg = false;
	}
}

// get length of margin left in graph
int Graphics::getGraphMarginLeft() {
	return m_graphMargin.left;
}

// set length of margin top in graph
void Graphics::setGraphMarginTop( const int top ) {
	if( m_graphMargin.top != top ) {
		m_graphMargin.top = top;
		m_trFlg = false;
	}
}

// get length of margin top in graph
int Graphics::getGraphMarginTop() {
	return m_graphMargin.top;
}

// set length of margin right in graph
void Graphics::setGraphMarginRight( const int right ) {
	if( m_graphMargin.right != right ) {
		m_graphMargin.right = right;
		m_trFlg = false;
	}
}

// get length of margin right in graph
int Graphics::getGraphMarginRight() {
	return m_graphMargin.right;
}

// set length of margin bottom in graph
void Graphics::setGraphMarginBottom( const int bottom ) {
	if( m_graphMargin.bottom != bottom ) {
		m_graphMargin.bottom = bottom;
		m_trFlg = false;
	}
}

// get length of margin bottom in graph
int Graphics::getGraphMarginBottom() {
	return m_graphMargin.bottom;
}

// set size of drawing area
void Graphics::setSize( const unsigned int width, const unsigned int height ) {
	// check 
	if( width == m_width && height == m_height ) {
		return;
	}

	// set size
	onSetSize( width, height );
	m_width = width;
	m_height = height;
}

// get width of drawing area
unsigned int Graphics::getWidth() {
	return m_width;
}

// get height of drawing area
unsigned int Graphics::getHeight() {
	return m_height;
}

// get width of graph drawing area
int Graphics::getGraphWidth() {
	return ( m_graphPos.right - m_graphPos.left );
}

// get height of graph drawing area
int Graphics::getGraphHeight() {
	return ( m_graphPos.bottom - m_graphPos.top );
}

// sets displayed range
void Graphics::setDisplayedRange(
		const double minX,
		const double maxX,
		const double minY,
		const double maxY
) {
	// set displayed range
	setMinX( minX );
	setMaxX( maxX );
	setMinY( minY );
	setMaxY( maxY );
}

// set min x
void Graphics::setMinX( const double minX ) {
	if( m_range.minX != minX ) {
		m_range.minX = minX;
		m_trFlg = false;
	}
}

// set max x
void Graphics::setMaxX( const double maxX ) {
	if( m_range.maxX != maxX ) {
		m_range.maxX = maxX;
		m_trFlg = false;
	}
}

// set min y
void Graphics::setMinY( const double minY ) {
	if( m_range.minY != minY ) {
		m_range.minY = minY;
		m_trFlg = false;
	}
}

// set max y
void Graphics::setMaxY( const double maxY ) {
	if( m_range.maxY != maxY ) {
		m_range.maxY = maxY;
		m_trFlg = false;
	}
}

// get min x
double Graphics::getMinX() {
	return m_range.minX;
}

// get max x
double Graphics::getMaxX() {
	return m_range.maxX;
}

// get min y
double Graphics::getMinY() {
	return m_range.minY;
}

// get max y
double Graphics::getMaxY() {
	return m_range.maxY;
}

// set scale x
void Graphics::setScaleX( const double scX ) {
	m_ctr( 0, 0 ) = scX;
}

// set scale y
void Graphics::setScaleY( const double scY ) {
	m_ctr( 1, 1 ) = scY;
}

// set scale
void Graphics::setScale( const double scX, const double scY ) {
	setScaleX( scX );
	setScaleY( scY );
}

// get scale x
double Graphics::getScaleX() {
	return m_ctr( 0, 0 );
}

// get scale y
double Graphics::getScaleY() {
	return m_ctr( 1, 1 );
}

// set shear x
void Graphics::setShearX( const double shX ) {
	m_ctr( 0, 1 ) = shX;
}

// set shear y
void Graphics::setShearY( const double shY ) {
	m_ctr( 1, 0 ) = shY;
}

// set shear
void Graphics::setShear( const double shX, const double shY ) {
	setShearX( shX );
	setShearY( shY );
}

// get shear x
double Graphics::getShearX() {
	return m_ctr( 0, 1 );
}

// get shear y
double Graphics::getShearY() {
	return m_ctr( 1, 0 );
}

// set translate x
void Graphics::setTranslateX( const double trX ) {
	m_ctr( 0, 2 ) = trX;
}

// set translate y
void Graphics::setTranslateY( const double trY ) {
	m_ctr( 1, 2 ) = trY;
}

// set translate
void Graphics::setTranslate( const double trX, const double trY ) {
	setTranslateX( trX );
	setTranslateY( trY );
}

// get translate x
double Graphics::getTranslateX() {
	return m_ctr( 0, 2 );
}

// get translate y
double Graphics::getTranslateY() {
	return m_ctr( 1, 2 );
}

// reset transform matrix
void Graphics::resetTransform() {
	m_ctr = kome::numeric::MatrixTool::getIdentityMatrix( 3 );

	m_trFlg = false;
}

// transform data to position
void Graphics::transformDataToPosition(
		const double x,
		const double y,
		double* px,
		double* py
) {
	// create matrix
	if( !m_trFlg ) {
		createMatrix();
	}

	// transform
	if( px != NULL ) {
		*px = m_tr( 0, 0 ) * x + m_tr( 0, 1 ) * y + m_tr( 0, 2 );
	}
	if( py != NULL ) {
		*py = m_tr( 1, 0 ) * x + m_tr( 1, 1 ) * y + m_tr( 1, 2 );
	}
}

// transform position to data
void Graphics::transformPositionToData(
		const double px,
		const double py,
		double* x,
		double* y
) {
	// create matrix
	if( !m_trFlg ) {
		createMatrix();
	}

	// transform
	if( x != NULL ) {
		*x = m_trInv( 0, 0 ) * px + m_trInv( 0, 1 ) * py + m_trInv( 0, 2 );
	}
	if( y != NULL ) {
		*y = m_trInv( 1, 0 ) * px + m_trInv( 1, 1 ) * py + m_trInv( 1, 2 );
	}
}


// create matrix
void Graphics::createMatrix() {
	// get graph position
	// @date 2011.09.07 <Mod> M.Izumi ->
	// SPEC 82307 :To view the error dialog window open. (Heat Map) 
	int tmp_l = m_graphPos.left   + m_graphMargin.left;
	int tmp_t = m_graphPos.top	+ m_graphMargin.top;
	int tmp_r = m_graphPos.right  - m_graphMargin.right;
	int tmp_b = m_graphPos.bottom - m_graphMargin.bottom;
	if( tmp_l >= tmp_r || tmp_t >= tmp_b ){
		return;
	}
	double l = (double)tmp_l;
	double t = (double)tmp_t;
	double r = (double)tmp_r;
	double b = (double)tmp_b;
	
	// data range
	double x0 = m_range.minX;
	double x1 = m_range.maxX;
	double y0 = m_range.minY;
	double y1 = m_range.maxY;

	// ( 0, 0 ) ( 1, 1 ) -> ( Xmin, Ymin ) ( Xmax, Ymax )
	kome::core::Matrix m0( 3, 3 );
	m0( 0, 0 ) = x1- x0;   m0( 0, 1 ) = 0.0;	   m0( 0, 2 ) = x0;
	m0( 1, 0 ) = 0.0;	  m0( 1, 1 ) = y1 - y0;   m0( 1, 2 ) = y0;
	m0( 2, 0 ) = 0.0;	  m0( 2, 1 ) = 0.0;	   m0( 2, 2 ) = 1.0;

	// ( 0, 0 ) -> ( 1, 1 ) -> ( left, bottom ) ( right, top )
	kome::core::Matrix m1( 3, 3 );
	m1( 0, 0 ) = r - l;	m1( 0, 1 ) = 0.0;	   m1( 0, 2 ) = l;
	m1( 1, 0 ) = 0.0;	  m1( 1, 1 ) = t - b;	 m1( 1, 2 ) = b;
	m1( 2, 0 ) = 0.0;	  m1( 2, 1 ) = 0.0;	   m1( 2, 2 ) = 1.0;

	// ( Xmin, Ymin ) ( Xmax, Ymax ) -> ( left, bottom ) ( right, top )
	m_tr = m1 * m0.invert();
	m_trInv = m_tr.invert();

	// set flag
	m_trFlg = true;
}

// set draw style
void Graphics::setDrawStyle( const DrawStyle style ) {
	if( m_drawStyle != style ) {
		m_drawStyle = style;
		onSetDrawStyle( style );
	}
}

// get draw style
Graphics::DrawStyle Graphics::getDrawStyle() {
	return m_drawStyle;
}

// set fill pattern
void Graphics::setFillPattern( const FillPattern pattern ) {
	if( m_fillPattern != pattern ) {
		m_fillPattern = pattern;
		onSetFillPattern( pattern );
	}
}

// get fill pattern
Graphics::FillPattern Graphics::getFillPattern() {
	return m_fillPattern;
}

// set fill style
void Graphics::setFillStyle( const FillStyle style ) {
	if( m_fillStyle != style ) {
		m_fillStyle = style;
		onSetFillStyle( style );
	}
}

// get fill style
Graphics::FillStyle Graphics::getFillStyle() {
	return m_fillStyle;
}

// set text horizontal align
void Graphics::setTextHAlign( const TextHAlign hAlign ) {
	if( m_hAlign != hAlign ) {
		m_hAlign = hAlign;
		onSetTextHAlign( hAlign );
	}
}

// get text horizontal align
Graphics::TextHAlign Graphics::getTextHAlign() {
	return m_hAlign;
}

// set text vertical align
void Graphics::setTextVAlign( const TextVAlign vAlign ) {
	if( m_vAlign != vAlign ) {
		m_vAlign = vAlign;
		onSetTextVAlign( vAlign );
	}
}

// get text vertical align
Graphics::TextVAlign Graphics::getTextVAlign() {
	return m_vAlign;
}

// set color
void Graphics::setColor( const COLOR color ) {
	if( m_color != color ) {
		m_color = color;
		onSetColor( color );
	}
}

// get color
COLOR Graphics::getColor() {
	return m_color;
}

// set font
void Graphics::setFont( Font& font ) {
	m_font = font;
	onSetFont( font );
}

// get font
Font& Graphics::getFont() {
	return m_font;
}

// set thickness line
void Graphics::setThicknessLine( int width ){
	onSetPen( width );
}

// get thickeness line
int Graphics::getThicknessLine(){
	return m_thickness;
}

// draw line
void Graphics::drawLine( const int px0, const int py0, const int px1, const int py1 ) {
	// points array
	int points[] = { px0, py0, px1, py1 };

	// draw line
	drawLines( points, sizeof( points ) / ( sizeof( int ) * 2 ) );
}

// draw lines
void Graphics::drawLines(
		int* const points,
		const unsigned int num
) {
	// check parameters
	if( num == 0 || points == NULL ) {
		return;
	}

	// transform
	int* pts = new int[ num * 2 ];
	for( unsigned int i = 0; i < num; i++ ) {
		double x = (double)points[ i * 2 ];
		double y = (double)points[ i * 2 + 1];

		pts[ i * 2 ]	 = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
		pts[ i * 2 + 1 ] = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );
	}

	// draw lines
	onDrawLines( pts, num );

	// delete array
	delete[] pts;
}

// fill rect
void Graphics::fillRect(
		const int px,
		const int py,
		const int width,
		const int height
) {
	// create array
	int points[] = {
		px,			 py,
		px,			 ( py + height ),
		( px + width ), ( py + height ),
		( px + width ), py
	};

	fillPolygon( points, 4 );
}

// fill polygon
void Graphics::fillPolygon(
		int* const points,
		const unsigned int num
) {
   // check parameters
	if( num == 0 || points == NULL ) {
		return;
	}

	// transform
	int* pts = new int[ num * 2 ];
	for( unsigned int i = 0; i < num; i++ ) {
		double x = (double)points[ i * 2 ];
		double y = (double)points[ i * 2 + 1];

		pts[ i * 2 ]	 = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
		pts[ i * 2 + 1 ] = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );
	}

	// fill polygon
	onFillPolygon( pts, num );

	// delete array
	delete[] pts;
}

// draw circle
void Graphics::drawCircle( const int cx, const int cy, const int r, const bool fill ) {
	drawArc( cx, cy, r, 0.0, 360.0, fill );
}

// draw arc
void Graphics::drawArc(
		const int cx,
		const int cy,
		const int r,
		const double start,
		const double end,
		const bool fill
) {
	drawEllipticArc(
		cx - r,
		cy - r,
		2 * r,
		2 * r,
		start,
		end,
		fill
	);
}

// draw ellipse
void Graphics::drawEllipse(
		const int px,
		const int py,
		const int width,
		const int height,
		const bool fill
) {
	drawEllipticArc(
		px,
		py,
		width,
		height,
		0.0,
		360.0,
		fill
	);
}

// draw elliptic arc
void Graphics::drawEllipticArc(
		const int px,
		const int py,
		const int width,
		const int height,
		const double start,
		const double end,
		const bool fill
) {
	double x = (double)px;
	double y = (double)py;

	onDrawEllipticArc(
		roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) ),
		roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) ),
		width,
		height,
		start,
		end,
		fill
	);
}

// draw text
void Graphics::drawText(
		const char* str,
		const int px,
		const int py,
		int angle
) {
	// transform
	double x = (double)px;
	double y = (double)py;

	int ppx = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
	int ppy = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );

	// draw text
	onDrawText( str, ppx, ppy, angle );
}

// get text size
void Graphics::getTextSize( const char* str, int* width, int* height ) {
	if( str == NULL ) {
		return;
	}
	onGetTextSize( str, width, height );
}

// draw point
void Graphics::drawPoint( const int px, const int py ) {
	// transform
	double x = (double)px;
	double y = (double)py;

	int ppx = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
	int ppy = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );

	// draw point
	onDrawPoint( ppx, ppy );
}

// draw bitmap
void Graphics::drawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent ) {
	// transform
	double x = (double)px;
	double y = (double)py;

	int ppx = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
	int ppy = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );

	// draw bitmap
	onDrawBitmap( bitmap, ppx, ppy, transparent );	
}

// draw mark
void Graphics::drawMark(
		const Mark mark,
		const int px,
		const int py,
		const int r
) {
	// check the size & shape
	if( r < 0 || mark == MARK_NONE ) {
		return;
	}
	if( r == 0 ) {
		drawPoint( px, py );
	}

	// pattern
	FillPattern oldPattern = getFillPattern();
	setFillPattern( PATTERN_SOLID );

	// double values
	double cx = (double)px;
	double cy = (double)py;
	double cr = (double)r;

	// draw
	if( mark == MARK_CIRCLE ) {	// circle
		drawCircle( px, py, r, false );
	}
	else if( mark == MARK_FILLED_CIRCLE ) {	// filled circle
		drawCircle( px, py, r, true );
	}
	else if( mark == MARK_PLUS ) {	// plus
		drawLine( px - r, py, px + r, py );
		drawLine( px, py - r, px, py + r );
	}
	else if( mark == MARK_TIMES ) {	// times
		int len = roundnum( cr * 0.7071 );
		drawLine( px - len, py - len, px + len, py + len );
		drawLine( px + len, py - len, px - len, py + len );
	}
	else {	// polygon
		std::vector< int > vertices;
		bool fill = false;
		const double pi = kome::numeric::Math::PI;

		if( mark == MARK_DELTA || mark == MARK_FILLED_DELTA ) {	// delta
			const double cos30 = 0.8660;
			const double sin30 = 0.5000;			
		
			vertices.push_back( roundnum( cx ) );
			vertices.push_back( roundnum( cy - cr ) );

			vertices.push_back( roundnum( cx - cr * cos30 ) );
			vertices.push_back( roundnum( cy + cr * sin30 ) );

			vertices.push_back( roundnum( cx + cr * cos30 ) );
			vertices.push_back( roundnum( cy + cr * sin30 ) );

			fill = ( mark == MARK_FILLED_DELTA );
		}
		else if( mark == MARK_NABLA || mark == MARK_FILLED_NABLA ) {	// nabla
			const double cos30 = 0.8660;
			const double sin30 = 0.5000;

			vertices.push_back( roundnum( cx ) );
			vertices.push_back( roundnum( cy + cr ) );

			vertices.push_back( roundnum( cx - cr * cos30 ) );
			vertices.push_back( roundnum( cy - cr * sin30 ) );

			vertices.push_back( roundnum( cx + cr * cos30 ) );
			vertices.push_back( roundnum( cy - cr * sin30 ) );

			fill = ( mark == MARK_FILLED_NABLA );
		}
		else if( mark == MARK_SQUARE || mark == MARK_FILLED_SQUARE ) {	// square
			const double cos45 = 0.7071;
			const double sin45 = 0.7071;

			vertices.push_back( roundnum( cx - cr * cos45 ) );
			vertices.push_back( roundnum( cy - cr * sin45 ) );

			vertices.push_back( roundnum( cx + cr * cos45 ) );
			vertices.push_back( roundnum( cy - cr * sin45 ) );

			vertices.push_back( roundnum( cx + cr * cos45 ) );
			vertices.push_back( roundnum( cy + cr * sin45 ) );

			vertices.push_back( roundnum( cx - cr * cos45 ) );
			vertices.push_back( roundnum( cy + cr * sin45 ) );

			fill = ( mark == MARK_FILLED_SQUARE );
		}
		else if( mark == MARK_DIAMOND || mark == MARK_FILLED_DIAMOND ) {	// diamond
			vertices.push_back( px );
			vertices.push_back( py - r );

			vertices.push_back( px - r );
			vertices.push_back( py );

			vertices.push_back( px );
			vertices.push_back( py + r );

			vertices.push_back( px + r );
			vertices.push_back( py );

			fill = ( mark == MARK_FILLED_DIAMOND );
		}
		else if( mark == MARK_STAR || mark == MARK_FILLED_STAR ) {	// star
			const double smallSize = cr * 0.3820;
			const double cos54 = 0.5878;
			const double sin54 = 0.8090;
			const double cos18 = 0.9511;
			const double sin18 = 0.3090;

			vertices.push_back( roundnum( cx ) );
			vertices.push_back( roundnum( cy - cr ) );

			vertices.push_back( roundnum( cx + smallSize * cos54 ) );
			vertices.push_back( roundnum( cy - smallSize * sin54 ) );

			vertices.push_back( roundnum( cx + cr * cos18 ) );
			vertices.push_back( roundnum( cy - cr * sin18 ) );

			vertices.push_back( roundnum( cx + smallSize * cos18 ) );
			vertices.push_back( roundnum( cy + smallSize * sin18 ) );

			vertices.push_back( roundnum( cx + cr * cos54 ) );
			vertices.push_back( roundnum( cy + cr * sin54 ) );

			vertices.push_back( roundnum( cx ) );
			vertices.push_back( roundnum( cy + smallSize ) );

			vertices.push_back( roundnum( cx - cr * cos54 ) );
			vertices.push_back( roundnum( cy + cr * sin54 ) );

			vertices.push_back( roundnum( cx - smallSize * cos18 ) );
			vertices.push_back( roundnum( cy + smallSize * sin18 ) );

			vertices.push_back( roundnum( cx - cr * cos18 ) );
			vertices.push_back( roundnum( cy - cr * sin18 ) );

			vertices.push_back( roundnum( cx - smallSize * cos54 ) );
			vertices.push_back( roundnum( cy - smallSize * sin54 ) );

			fill = ( mark == MARK_FILLED_STAR );
		// @date 2011.03.29 <Add> M.Izumi ->
		// 閾値を表示するための三角形
		}else if( mark == MARK_TRIANGLE ){
			
			vertices.push_back( px );
			vertices.push_back( py + r );

			vertices.push_back( px + r );
			vertices.push_back( py );

			vertices.push_back( px );
			vertices.push_back( py - r );
			fill = true;
		}
		//<-

		// draw polygon
		if( vertices.size() >= 2 ) {
			if( fill ) {
				fillPolygon( &( vertices[ 0 ] ), vertices.size() / 2 );
			}

			vertices.push_back( vertices[ 0 ] );
			vertices.push_back( vertices[ 1 ] );

			drawLines( &( vertices[ 0 ] ), vertices.size() / 2 );
		}
	}
}

// get pixel color
COLOR Graphics::getPixel( const int px, const int py ) {
	// transform
	double x = (double)px;
	double y = (double)py;

	int ppx = roundnum( x * m_ctr( 0, 0 ) + y * m_ctr( 0, 1 ) + m_ctr( 0, 2 ) );
	int ppy = roundnum( x * m_ctr( 1, 0 ) + y * m_ctr( 1, 1 ) + m_ctr( 1, 2 ) );

	// get pixel color
	return onGetPixel( ppx, ppy );
}

// flush
void Graphics::flush() {
	onFlush();
}
