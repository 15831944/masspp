/**
 * @file PlotCanvas.cpp
 * @brief implements of PlotCanvas class
 *
 * @author S.Tanaka
 * @date 2009.07.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PlotCanvas.h"

using namespace kome::matrix::groupplot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MARGIN_SIZE					  20
#define DIV_LEVEL					   8

#define SCALE_LENGTH				   6
#define TEXT_SPACE					   5
#define MIN_SCALE_SPACE				  10


BEGIN_EVENT_TABLE( PlotCanvas, kome::window::ProfileCanvas )
END_EVENT_TABLE()


// constructor
PlotCanvas::PlotCanvas(
		wxWindow* parent,
		const int width,
		const int height,
		const int peakId,
		bool group_plot,
		bool box_plot
) : kome::window::ProfileCanvas( parent, width, height ), m_peakId( peakId ) {
	// initialize
	m_minVal = -1.0;
	m_maxVal = -1.0;
	m_initialized = false;

	m_groupPlot = group_plot;
	m_boxPlot = box_plot;
}

// destructor
PlotCanvas::~PlotCanvas() {
}

// add group
void PlotCanvas::addGroup( const int groupId ) {
	m_groups.resize( m_groups.size() + 1 );
	m_groups.back().groupId = groupId;
}

// set peak ID
void PlotCanvas::setPeakId( const int peakId ) {
	// check the member
	if( m_peakId == peakId ) {
		return;
	}

	// create array
	m_peakId = peakId;
	createArray();

	Refresh();
}

// create array
void PlotCanvas::createArray() {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int project = identMgr.getSelectedMatrix();
	if( project < 0 ) {
		return;
	}

	// min, max
	m_minVal = -1.0;
	m_maxVal = -1.0;
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		// clear
		m_groups[ i ].positions.clear();

		// set
		std::vector< int > samples;
		identMgr.getMatrixSamplesFromGroup( samples, m_groups[ i ].groupId );
		for( unsigned int j = 0; j < samples.size(); j++ ) {
			const double val = identMgr.getNormalizedPeakValue( m_peakId, samples[ j ] );
			if( val >= 0.0 ) {
				if( m_minVal < 0.0 || val < m_minVal ) {
					m_minVal = val;
				}
				if( m_maxVal < 0.0 || val > m_maxVal ) {
					m_maxVal = val;
				}
			}			
		}
	}

	const double delta = 1.0e-12;

	m_minVal = std::max( 0.0, m_minVal );
	double range = std::max( m_maxVal - m_minVal, delta );
	m_minVal = std::max( 0.0, m_minVal - 0.05 * range );
	m_maxVal = std::max( m_maxVal + 0.05 * range, m_minVal + delta );

	// Spec No.79789 : Group Plot Dialogを表示するとアプリが落ちる不具合修正
	// @date 2010.12.27 <Add> M.Izumi ->
	if( m_minVal ==  m_maxVal ){
		m_minVal = 0.0;
	}
	//<-
	m_graphics.setDisplayedRange( 0.0, 100.0, m_minVal, m_maxVal );

	// array
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		GroupInfo* info = &( m_groups[ i ] );

		setGroup( info );
	}
}

// set group information
void PlotCanvas::setGroup( GroupInfo* group ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get group information
	std::string filePath;
	group->name = identMgr.getGroupName( group->groupId );

	// samples
	int project = identMgr.getSelectedMatrix();

	std::vector< int > samples;
	identMgr.getMatrixSamplesFromGroup( samples, group->groupId );

	// peak values
	std::vector< std::pair< double, COLOR > > vals;
	double total = 0.0;

	for( unsigned int i = 0; i < samples.size(); i++ ) {
		const double val = identMgr.getNormalizedPeakValue( m_peakId, samples[ i ] );
		const COLOR c = identMgr.getMatrixSampleColor( samples[ i ] );
		if( val >= 0.0 ) {
			vals.push_back( std::make_pair( val, c ) );
			total += val;
		}
	}
	if( vals.size() == 0 ) {
		group->avg = 0.0;
		return;
	}
	group->avg = total / (double)vals.size();
	std::sort( vals.begin(), vals.end(), lessVal );

	// logistic
	double rnd = 0.1;

	// clustering
	const double levelWidth = ( m_maxVal - m_minVal ) / (double)( DIV_LEVEL - 1 );
	double minWidth = 0.0;
	std::vector< std::vector< std::pair< double, COLOR > > > levelVals;

	if( vals.size() > 0 ) {
		levelVals.resize( vals.size() );
		for( unsigned int i = 0; i < vals.size(); i++ ) {
			levelVals[ i ].push_back( vals[ i ] );
		}
	}

	while( levelVals.size() > 1 && minWidth < levelWidth ) {
		// check the distance
		int minIdx = -1;
		minWidth = m_maxVal - m_minVal;
		for( int i = 0; i < (int)levelVals.size() - 1; i++ ) {
			const double delta = fabs( levelVals[ i + 1 ].back().first - levelVals[ i ].front().first );
			if( delta < minWidth ) {
				minWidth = delta;
				minIdx = i;
			}
		}

		// merge
		if( minIdx >= 0 && minWidth < levelWidth ) {
			for( unsigned int i = 0; i < levelVals[ minIdx + 1 ].size(); i++ ) {
				levelVals[ minIdx ].push_back( ( levelVals[ minIdx + 1 ] )[ i ] );
			}
			levelVals.erase( levelVals.begin() + ( minIdx + 1 ) );
		}
	}

	// positions
	for( unsigned int i = 0; i < levelVals.size();  i++ ) {
		// move higher value to middle
		std::list< std::pair< double, COLOR > > valList;

		const int idxEnd = (int)levelVals[ i ].size();
		std::pair< double, COLOR >* arr = &( (levelVals[ i ] )[ 0 ] );

		for( int j = idxEnd - 1; j >= 0; j-- ) {
			if( j % 2 == 0 ) {
				valList.push_front( arr[ j ] );
			}
			else {
				valList.push_back( arr[ j ] );
			}
		}

		// width
		const double w = 100.0 / (double)( levelVals[ i ].size() + 2 );
		int idx = 1;
		for( std::list< std::pair< double, COLOR > >::iterator it = valList.begin(); it != valList.end(); it++ ) {
			rnd = 4.0 * rnd * ( 1.0 - rnd );
			const double t = ( rnd > 0.5 ? 1.0 - rnd + 0.5 : 0.5 - rnd );
			PosInfo pos;
			pos.x = w * ( (double)idx + 0.1 + 0.8 * t );
			pos.y = (*it).first;
			pos.color = (*it).second;			
			group->positions.push_back( pos );
			idx++;
		}
	}
}

// prepare
void PlotCanvas::onPrepareDraw( kome::window::DCGraphics& g ) {
	// create array
	if( !m_initialized ) {
		createArray();
		m_initialized = true;
	}

	// prepare margin
	int leftMargin = TEXT_SPACE + SCALE_LENGTH;
	int topMargin = g.getGraphPositionTop();
	int rightMargin = 20;
	int bottomMargin = 2 * ( TEXT_SPACE + SCALE_LENGTH );

	// get title size
	int titleWidth = 0;
	int titleHeight = 0;
	g.getTextSize( "|title|", &titleWidth, &titleHeight );

	leftMargin += ( titleHeight + TEXT_SPACE );

	// get y scale information
	leftMargin += kome::img::GraphTool::getMaxWidthOfYLabels( g, MIN_SCALE_SPACE );
	bottomMargin += titleHeight;

	// set position
	g.setGraphPosition(
		leftMargin,
		topMargin,
		g.getWidth() - rightMargin,
		g.getHeight() - bottomMargin
	);

	// set display range
	g.setDisplayedRange( 0.0, 100.0, m_minVal, m_maxVal );

	// set position 
	double groupWidth = 400.0 / (double)( 5 * m_groups.size() + 1 );
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		m_groups[ i ].width = groupWidth;
		m_groups[ i ].pos = ( 3.0 + 5.0 * (double)i ) * groupWidth / 4.0;
	}
}


// on draw profile
void PlotCanvas::onDrawProfile( kome::window::DCGraphics& g ) {

	// state group plot
	if( getGroupPlot() ){
		// draw group plot
		drawGroupPlot( g );
	}

	// state box plot
	if( getBoxPlot() ){
		// draw box plot
		drawBoxPlot( g );
	}	
}

// on draw graph foreground
void PlotCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
	// state group plot
	if( getGroupPlot()){
		drawGroupMedian( g );
	}
	// state box plot
	if(  getBoxPlot() ){
		drawBoxMedian( g );
	}
}

// on draw foreground
void PlotCanvas::onDrawWindowFg( kome::window::DCGraphics& g ) {
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );

	// y scale
	kome::img::GraphTool::drawGraphRect( g );
	kome::img::GraphTool::drawYScale( g, 10, 6, 5, false );

	std::string yLabel = "Peak Value";
	int labelWidth = 0;
	int labelHeight = 0;
	g.getTextSize( yLabel.c_str(), &labelWidth, &labelHeight );
	g.drawText(
		yLabel.c_str(),
		TEXT_SPACE,
		( g.getGraphPositionTop() + g.getGraphPositionBottom() + labelWidth ) / 2,
		90
	);

	// draw group name
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		// string objects
		std::string name = m_groups[ i ].name;
		int w = roundnum( (double)g.getGraphWidth() * m_groups[ i ].width / 100.0 * 0.9 );
		int len = w / 8;
		if( (int)name.length() > len + 1 ) {
			name = name.substr( 0, len );
			name.append( "..." );
		}

		std::string nStr = FMT( "(n = %d)", m_groups[ i ].positions.size() );

		// coordinates
		double px = double();
		double py = double();
		int textWidth = int();
		int textHeight = int();
		g.transformDataToPosition( m_groups[ i ].pos, 0.0, &px, &py );
		g.getTextSize( name.c_str(), &textWidth, &textHeight );

		int nTextWidth = int();
		int nTextHeight = int();
		g.getTextSize( nStr.c_str(), &nTextWidth, &nTextHeight );

		// draw
		g.drawText( name.c_str(), roundnum( px ) - textWidth / 2, g.getGraphPositionBottom() + TEXT_SPACE );
		g.drawText( nStr.c_str(), roundnum( px ) - nTextWidth / 2, g.getGraphPositionBottom() + TEXT_SPACE + textHeight );
	}

	// restore color
	g.setColor( oldColor );
}

// compare
bool PlotCanvas::lessVal( std::pair< double, COLOR >& v0, std::pair< double, COLOR >& v1 ) {
	return ( v0.first < v1.first );
}

// draw group plot
void PlotCanvas::drawGroupPlot( kome::window::DCGraphics& g ){
	// prepare
	const int markSize = 3;
	COLOR oldColor = g.getColor();

	// ●を書いてる箇所
	// draw marks
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {	// each groups
		GroupInfo* group = &( m_groups[ i ] );
		const double groupPos = group->pos;
		const double groupWidth = group->width;

		for( unsigned int j = 0; j < group->positions.size(); j++ ) {	// each marks
			// color
			g.setColor( group->positions.at( j ).color );

			// position
			double x = groupPos - groupWidth / 2.0 + group->positions[ j ].x * groupWidth / 100.0;
			double y = group->positions[ j ].y;

			double px = double();
			double py = double();
			g.transformDataToPosition( x, y, &px, &py );

			// draw
			g.drawMark(
				kome::img::Graphics::MARK_FILLED_CIRCLE,
				roundnum( px ),
				roundnum( py ),
				markSize
			);
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw Group median
void PlotCanvas::drawGroupMedian( kome::window::DCGraphics& g ){
	const int markSize = 5;

	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );

	// average
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		const double avg = m_groups[ i ].avg;
		double lx = double();
		double ly = double();
		double rx = double();
		double ry = double();

		double x = ( ( m_groups[ i ].pos - m_groups[ i ].width / 2.0 ) + (m_groups[ i ].pos + m_groups[ i ].width / 2.0 ) ) / 2;
		
		g.transformDataToPosition(
			x,
			avg,
			&lx,
			&ly
		);
		
		// 平均値 描画
		g.drawMark( 
			kome::img::Graphics::MARK_PLUS,
			roundnum( lx ),
			roundnum( ly ),
			markSize
		);
	}

	// restore color
	g.setColor( oldColor );
}

// draw box plot
void PlotCanvas::drawBoxPlot( kome::window::DCGraphics& g ){
	// prepare
	COLOR oldColor = g.getColor();  
	g.setColor( kome::img::ColorTool::BLACK );

	// average
	for( unsigned int i = 0; i < m_groups.size(); i++ ) {    // 1グループ1箱ひげ図なので、グループ数分loop
		GroupInfo* group = &( m_groups[ i ] );               // グループの情報を得る
		
		std::vector< PosInfo > boxPos = group->positions;    // そのピーク位置における各サンプルのピーク値
		// yの値で昇順
		std::sort( boxPos.begin(), boxPos.end(), lessPos );  // ピーク値で昇順ソート

		int num = boxPos.size();
		if( num == 0 ){
			continue;
		}

		double val = 0.0;
		double med = 0.0;

		bool bDivisible = false;
		if( ( 1 + num ) % 2 == 0 ){
			val = ( 1 + num ) / 2;			
			
			med = boxPos[val-1].y;
			
			bDivisible = true;	
		}else{			
			double tmp = ( num + 1 ) / 2;
			val = floor( tmp );

			med = ( boxPos[val-1].y + boxPos[ val ].y ) / 2;
		}

		group->med = med;    // グループの中央値をセット

		// 第1四分位
		double quartile1;
		calcQuartile( val, bDivisible, boxPos, true, quartile1 );		
		
		// 第3四分位
		double quartile3;		
		calcQuartile( val, bDivisible, boxPos, false, quartile3 );

		// 箱 描画
		drawBox( g, group->pos, group->width, quartile1, quartile3 );
		
		// box plot Info
		BoxInfo info;
		info.quartile1 = quartile1;
		info.median = group->med;
		info.quartile3 = quartile3;
		info.positions = boxPos;   // ピーク値が値が昇順で入っている

		// 最大値
		drawLine( g, false, info, group->pos, group->width );
		
		// 最小値
		drawLine( g, true, info, group->pos, group->width );	
	}
	
	// restore color
	g.setColor( oldColor );
}

// 第1四分位算出
void PlotCanvas::calcQuartile( double val, bool bDivisible, std::vector< PosInfo > boxPos, bool bFirstQuartile, double& quartile ){
	quartile = 0.0;
	
	if( bFirstQuartile ){ // 第1四分位算出
		// 割り切れた
		if( bDivisible ){
			if( ( (int)val + 1 ) % 2 == 0 ){
				int tmp = ( (int)val + 1 ) / 2;
				quartile = boxPos[tmp-1].y;
			}
		}

		if( quartile == 0.0 ){
			double tmp = ( val + 1 ) / 2;
			int v1 = floor( tmp )-1;
			quartile = ( boxPos[v1].y + boxPos[v1+1].y )/ 2;
		}	
	}else{				// 第3四分位算出
		int num = boxPos.size();

		// 割り切れた
		if( bDivisible ){
			if( ( (int)val + num ) % 2 == 0 ){
				int tmp = ( val + num ) / 2;
				quartile = boxPos[tmp-1].y;
			}
		}

		if( quartile == 0.0 ){
			double tmp = ( val + num ) / 2;
			int v3 = floor( tmp )-1;
			quartile = ( boxPos[ v3 ].y + boxPos[v3+1].y ) / 2;
		}
	}
}

// draw box median
void PlotCanvas::drawBoxMedian( kome::window::DCGraphics& g ){
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );

	for( unsigned int i = 0; i < m_groups.size(); i++ ){
		GroupInfo* group = &( m_groups[ i ] );
		
		std::vector< PosInfo > boxPos = group->positions;

		if( boxPos.empty() ){
			continue;
		}
		// yの値で昇順
		std::sort( boxPos.begin(), boxPos.end(), lessPos );

		//中央値
		int num = boxPos.size();
		double med = group->med;

		double x = double();
		double px0 = double();
		double py0= double();
		double px1 = double();
		double py1 = double();

		g.transformDataToPosition( group->pos - group->width / 2.0, med, &px0, &py0 );
		g.transformDataToPosition( group->pos + group->width / 2.0, med, &px1, &py1 );
	
		// 中央値のライン
		g.drawLine( roundnum( px0 ), roundnum( py0 ), roundnum( px1 ), roundnum( py1 ) );
	}

	// restore color
	g.setColor( oldColor );
}

// less y position
bool PlotCanvas::lessPos( PosInfo& v0, PosInfo& v1 ){
	return ( v0.y < v1.y );
}

// draw box
void PlotCanvas::drawBox( 
	kome::window::DCGraphics& g, 
	double pos,
	double width,
	double quartile1,
	double quartile3
){
	// prepare
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );
	
	// position
	double lx = double();
	double ly = double();
	double rx = double();
	double ry = double();
		
	g.transformDataToPosition( pos - width / 2.0,	quartile3, &lx, &ly );
	g.transformDataToPosition( pos + width / 2.0, quartile1, &rx,	&ry	);
				
	int left   = roundnum( lx );
	int right  = roundnum( rx );
	int top	= roundnum( ly );
	int bottom = roundnum( ry );
		
	int rectPts[] = {
		left,  top,
		right, top,
		right, bottom,
		left,  bottom,
		left, top
	};

	// box plot
	g.drawLines( rectPts, sizeof( rectPts ) / ( sizeof( int ) * 2 ) );
	// restore color
	g.setColor( oldColor );
}

// draw line
void PlotCanvas::drawLine( 
	kome::window::DCGraphics& g,
	const bool bmin,
	BoxInfo info,
	double pos,
	double width
){
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );
	
	// position
	double lx = double();
	double ly = double();
	double rx = double();
	double ry = double();
			
	double xPos1 = pos - width / 4.0;
	double iqr = (info.quartile3 - info.quartile1 )*1.5;

	double yPos_up = 0.0;
	double yPos_dwn = 0.0;

	yPos_up = info.quartile3 + iqr;     // ひげを伸ばす最も上の位置
	yPos_dwn = info.quartile1 - iqr;    // ひげを伸ばす最も下の位置
		
	bool btmp = false;
	double up = 0.0;
	double dwn = 0.0;
	for( unsigned int i=0; i < info.positions.size(); i++ ){
		PosInfo tmpPos = info.positions[i];     // ピーク値が値が昇順で入っている
				
		if( !bmin ){ // 上ひげ
				if( yPos_up > info.positions[i].y && info.quartile3 < info.positions[i].y ){
				up = info.positions[i].y;   
				btmp = true;
			}
		}else{		// 下ひげ
			if( tmpPos.y > yPos_dwn ){  
				dwn = tmpPos.y;
				btmp = true;
				break;
			}
		}					
	}
	
	double xPos2 = pos + width / 4.0;
	g.transformDataToPosition( xPos1, ( !bmin ? up : dwn ), &lx, &ly );
	g.transformDataToPosition( xPos2, ( !bmin ? up : dwn ), &rx, &ry );
	
	if( btmp ){
		// 最大値・最小値のライン
		g.drawLine( roundnum( lx ), roundnum( ly ), roundnum( rx ), roundnum( ry ) );
	
		// 最大・最小値の真ん中の位置
		double xPos = ( xPos1+xPos2 ) / 2;
		// 縦のライン 描画
		drawVerticalLine( g, bmin, info, pos, width, xPos, up, dwn );
	}
	// 外れ値 描画
	drawOutlier( g, bmin, info, pos, width, yPos_up, yPos_dwn );
	// restore color
	g.setColor( oldColor );
}

// 縦のライン描画
void PlotCanvas::drawVerticalLine(
	kome::window::DCGraphics& g, 
	bool bmin,
	BoxInfo info, 
	double pos,
	double width,
	double xPos,
	double yPos_up,
	double yPos_dwn	
){
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );
	
	double lx = double();
	double ly = double();
	double rx = double();
	double ry = double();
		
	// 縦のライン
	if( bmin ){ // 最小値 → 第1四分位
		g.transformDataToPosition( xPos, yPos_dwn, &lx, &ly );
		g.transformDataToPosition( xPos, info.quartile1, &rx, &ry );
	}else{		// 最大値 → 第3四分位
		g.transformDataToPosition( xPos, yPos_up, &lx, &ly );
		g.transformDataToPosition( xPos, info.quartile3, &rx, &ry );
	}
	
	// 縦のライン描画
	g.drawLine( roundnum( lx ), roundnum( ly ), roundnum( rx ), roundnum( ry ) );

	// restore color
	g.setColor( oldColor );
}

// 外れ値
void PlotCanvas::drawOutlier(
	kome::window::DCGraphics& g, 
	const bool bmin,
	BoxInfo info, 
	double pos,
	double width,
	double yPos_up,
	double yPos_dwn
){
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );
	const int markSize = 3;
	
	// outlier
	for( unsigned int i=0; i < info.positions.size(); i++ ){
		PosInfo tmpPos = info.positions[i];

		bool bflg = false;
		if( !bmin ){ 
			if( yPos_up < tmpPos.y ){
				bflg = true;
			}
		}else{ 
			if( tmpPos.y < yPos_dwn ){
				bflg = true;
			}
		}
				
		if(  bflg  ){
			// position
			double x = pos - width / 2.0 + tmpPos.x * width / 100.0;
			double y = tmpPos.y;

			double px = double();
			double py = double();
			g.transformDataToPosition( x, y, &px, &py );

			// 外れ値 描画
			g.drawMark(
				kome::img::Graphics::MARK_CIRCLE,
				roundnum( px ),
				roundnum( py ),
				markSize
			);
		}
	}
	// restore color
	g.setColor( oldColor );
	
}
