/**
 * @file ProfileDrawer.cpp
 * @brief implements of ProfileDrawer class
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ProfileDrawer.h"

#include <list>
#include <math.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// @date 2011.02.28 <Add> M.Izumi ->
#define DRAWING_CHROM_STATUS_KEY			"DRAWING_CHROMATOGRAM"
#define DRAWING_SPEC_STATUS_KEY				"DRAWING_SPECTRUM"
#define DRAWING_STATUS_VALUE_PROFILE		"profile"
#define DRAWING_STATUS_VALUE_IMPULSE		"impulse"
//<-
#define SECTION								"Drawing"
#define SPEC_WIDTH							"WIDTH_SPEC"
#define SPEC_COLOR							"COLOR_SPEC"
#define CHROM_WIDTH							"WIDTH_CHROM"
#define	CHROM_COLOR							"COLOR_CHROM"
#define SPEC_DEFAULT_COLOR					"red"
#define CHROM_DEFAULT_COLOR					"blue"

// constructor
ProfileDrawer::ProfileDrawer() {
}

// destructor
ProfileDrawer::~ProfileDrawer() {
	for( std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* >::iterator it
			= m_spectrumDrawMap.begin(); it != m_spectrumDrawMap.end(); it++ ) {
		delete (*it).second;
	}
	m_spectrumDrawMap.clear();

	for( std::map<kome::objects::Chromatogram*, kome::img::DrawingXYData* >::iterator it
			= m_chromatogramDrawMap.begin(); it != m_chromatogramDrawMap.end(); it++ ) {
		delete (*it).second;
	}
	m_chromatogramDrawMap.clear();
}

// draw spectrum
void ProfileDrawer::drawSpectrum(
		kome::objects::Spectrum* spectrum,
		kome::img::Graphics* g
) {
	// check parameters
	if( spectrum == NULL ) {
		LOG_WARN( FMT( "Spectrum object is null." ) );
		return;
	}
	else if( g == NULL ) {
		LOG_WARN( FMT( "Graphics object is null." ) );
		return;
	}

	// get xy data
	kome::core::XYData* xyData = spectrum->getXYData();
	if( xyData == NULL ) {
		LOG_WARN( FMT( "Data points object is null." ) );
		return;
	}

	// get draw data
	kome::img::DrawingXYData* drawData = NULL;
	if( m_spectrumDrawMap.find( spectrum ) == m_spectrumDrawMap.end() ) {	// create draw data
		drawData = new kome::img::DrawingXYData( *xyData );

		m_spectrumDrawMap[ spectrum ] = drawData;
	}
	if( drawData == NULL ) {
		drawData = m_spectrumDrawMap[ spectrum ];
	}

	// set line style
	if( !g->isUnusedDefaultColor() ){
		g->setColor( kome::img::GraphTool::getColorFromIni( SECTION, SPEC_COLOR, SPEC_DEFAULT_COLOR ) );
	}
	if( !g->isUnusedDefaultThickness() ){
		g->setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION, SPEC_WIDTH ) );
	}

	// draw line
	if( spectrum->isCentroidMode() ) {
		drawImpulse( drawData, g );
	}
	else {
		drawProfile( drawData, g, 0.2 );
	}
}

// draw chromatogram
void ProfileDrawer::drawChromatogram(
		kome::objects::Chromatogram* chromatogram,
		kome::img::Graphics* g
) {
	// check parameters
	if( chromatogram == NULL ) {
		LOG_WARN( FMT( "Chromatogram object is null." ) );
		return;
	}
	else if( g == NULL ) {
		LOG_WARN( FMT( "Graphics object is null." ) );
		return;
	}

	// get xy data
	kome::core::XYData* xyData = chromatogram->getXYData();
	if( xyData == NULL ) {
		LOG_WARN( FMT( "Data points object is null." ) );
		return;
	}

	// get draw data
	kome::img::DrawingXYData* drawData = NULL;
	if( m_chromatogramDrawMap.find( chromatogram ) == m_chromatogramDrawMap.end() ) {	// create draw data
		drawData = new kome::img::DrawingXYData( *xyData );

		m_chromatogramDrawMap[ chromatogram ] = drawData;
	}
	if( drawData == NULL ) {
		drawData = m_chromatogramDrawMap[ chromatogram ];
	}
		
	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_CHROM_STATUS_KEY ), "" );
	if( val.compare( DRAWING_STATUS_VALUE_IMPULSE ) == 0 ){
		drawImpulse( drawData, g );
	}
	else {
		// draw profile
		drawProfileChrom( drawData, g, -1.0 );
	}
}

// draw profile (chromatogram)
void ProfileDrawer::drawProfileChrom(
		kome::img::DrawingXYData* drawData,
		kome::img::Graphics* g,
		double maxSpacing
) {
	// width
	unsigned int width = g->getGraphWidth();

	// get draw level
	int level = drawData->getLevel( width, g->getMinX(), g->getMaxX() );

	// get index
	int startIndex = drawData->searchIndex( level, g->getMinX() - MAX( maxSpacing, 0.0 ) );
	if( startIndex < 0 ) {
		startIndex = - startIndex - 2;
		startIndex = MAX( 0, startIndex );
	}

	int maxIndex = (int)drawData->getLength( level ) - 1;
	int endIndex = drawData->searchIndex( level, g->getMaxX() + MAX( maxSpacing, 0.0 ) );
	if( endIndex < 0 ) {
		endIndex = - endIndex - 1;
		endIndex = MIN( endIndex, maxIndex );
	}

	// get draw data
	std::vector< DrawData > drawDataArray;
	int prevPx = -1;
	for( int i = startIndex; i <= endIndex; i++ ) {
		// get coordinate
		double  x = drawData->getX( level, i );
		double ly = drawData->getLeftY( level, i );
		double ty = drawData->getTopY( level, i );
		double by = drawData->getBottomY( level, i );
		double ry = drawData->getRightY( level, i );

		// transform
		double px = double();
		double py = double();
		g->transformDataToPosition( x, 0.0, &px, &py );
		int ppx  = roundnum( px );

		// add
		if( ppx == prevPx && drawDataArray.size() > 0 ) {   // update
			DrawData& data = drawDataArray.back();

			data.rightY = ry;
			if( ty > data.topY ) {
				data.topY = ty;
			}
			if( by < data.bottomY ) {
				data.bottomY = by;
			}
		}
		else {		// add
			drawDataArray.resize( drawDataArray.size() + 1 );
			DrawData& data = drawDataArray.back();

			data.px	  = ppx;
			data.x	   = x;
			data.leftY   = ly;
			data.topY	= ty;
			data.bottomY = by;
			data.rightY  = ry;
		}
		prevPx = ppx;		
	}

	// zero position
	double zeroPx = double();
	double zeroPy = double();
	g->transformDataToPosition( 0.0, 0.0, &zeroPx, &zeroPy );
	int zeroPpy = roundnum( zeroPy );

	// create array
	std::vector< int > points;

	prevPx = -1;
	int prevRightY = 0;
	double prevX = -999999.9;
	
	for( unsigned int i = 0; i < drawDataArray.size(); i++ ) {		
		// draw data
		DrawData& data = drawDataArray[ i ];
		double x = data.x;

		// get coordinate
		double px  = double();
		double ply = double();
		double pty = double();
		double pby = double();
		double pry = double();
		g->transformDataToPosition( x, data.leftY,   &px, &ply );
		g->transformDataToPosition( x, data.topY,	&px, &pty );
		g->transformDataToPosition( x, data.bottomY, &px, &pby );
		g->transformDataToPosition( x, data.rightY,  &px, &pry );

		int ppx  = data.px;
		int pply = roundnum( ply );
		int ppty = roundnum( pty );
		int ppby = roundnum( pby );
		int ppry = roundnum( pry );

		// insert point
		if( ppx > prevPx + 1 ) {		// interpolation
			if( points.size() > 0 ) {   // insert previous right position
				if( prevRightY != points.back() ) {
					points.push_back( prevPx );
					points.push_back( prevRightY );
				}
			}

			if( pply != ppty ) {		// insert left point				
				points.push_back( ppx );
				points.push_back( pply );
			}
		}

		points.push_back( ppx );
		points.push_back( ppty );
		if( ppby != ppty ) {
			points.push_back( ppx );
			points.push_back( ppby );
		}

		// update previous
		prevPx = ppx;
		prevRightY = ppry;
		prevX = x;
	}
	if( maxSpacing > 0.0 && prevPx < g->getGraphPositionRight() ) {
		points.push_back( prevPx + 1 );
		points.push_back( zeroPpy );
		points.push_back( g->getGraphPositionRight() );
		points.push_back( zeroPpy );
	}

	// draw profile
	if( points.size() > 0 ) {
		// 色、太さの設定をiniファイルから取得する為の処理追加 @date 2013.04.26 <Add> M.izumi ->
		if( !g->isUnusedDefaultColor() ){
			g->setColor( kome::img::GraphTool::getColorFromIni( SECTION, CHROM_COLOR, CHROM_DEFAULT_COLOR ) );
		}
		if( !g->isUnusedDefaultThickness() ){
			g->setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION, CHROM_WIDTH ) );
		}
		//  @date 2013.04.26 <Add> M.izumi <-
		g->drawLines( &points[ 0 ], points.size() / 2 );
	}
}

// draw profile (spectrum)
void ProfileDrawer::drawProfile(
		kome::img::DrawingXYData* drawData,
		kome::img::Graphics* g,
		double maxSpacing
) {
	// width
	unsigned int width = g->getGraphWidth();

	// get draw level
	int level = drawData->getLevel( width, g->getMinX(), g->getMaxX() );

	// get index
	int startIndex = drawData->searchIndex( level, g->getMinX() - MAX( maxSpacing, 0.0 ) );
	if( startIndex < 0 ) {
		startIndex = - startIndex - 2;
		startIndex = MAX( 0, startIndex );
	}

	int maxIndex = (int)drawData->getLength( level ) - 1;
	int endIndex = drawData->searchIndex( level, g->getMaxX() + MAX( maxSpacing, 0.0 ) );
	if( endIndex < 0 ) {
		endIndex = - endIndex - 1;
		endIndex = MIN( endIndex, maxIndex );
	}

	// get draw data
	std::vector< DrawData > drawDataArray;
	double prevX = -1.0;	// @date 2011.02.28 <Add> M.Izumi
	int prevPx = -1;
	for( int i = startIndex; i <= endIndex; i++ ) {
		// get coordinate
		double  x = drawData->getX( level, i );
		double ly = drawData->getLeftY( level, i );
		double ty = drawData->getTopY( level, i );
		double by = drawData->getBottomY( level, i );
		double ry = drawData->getRightY( level, i );

		// transform
		double px = double();
		double py = double();
		g->transformDataToPosition( x, 0.0, &px, &py );
		int ppx  = roundnum( px );

		// add
		if( ppx == prevPx && drawDataArray.size() > 0 ) {   // update
			DrawData& data = drawDataArray.back();

			data.rightY = ry;
			if( ty > data.topY ) {
				data.topY = ty;
			}
			if( by < data.bottomY ) {
				data.bottomY = by;
			}
		}
		else {		// add
			drawDataArray.resize( drawDataArray.size() + 1 );
			DrawData& data = drawDataArray.back();

			data.px	  = ppx;
			data.x	   = x;
			data.leftY   = ly;
			data.topY	= ty;
			data.bottomY = by;
			data.rightY  = ry;
		}
		prevX = x;	// @date 2011.02.28 <Add> M.Izumi 
		prevPx = ppx;		
	}

	// zero position
	double zeroPx = double();
	double zeroPy = double();
	g->transformDataToPosition( 0.0, 0.0, &zeroPx, &zeroPy );

	// create array
	std::vector< int > points;

	prevPx = -1;
	int prevRightY = 0;
	prevX = -999999.9;			// @date 2011.02.28 <Add> M.Izumi 
	for( unsigned int i = 0; i < drawDataArray.size(); i++ ) {		
		// draw data
		DrawData& data = drawDataArray[ i ];
		double x = data.x;

		// get coordinate
		double px  = double();
		double ply = double();
		double pty = double();
		double pby = double();
		double pry = double();
		g->transformDataToPosition( x, data.leftY,   &px, &ply );
		g->transformDataToPosition( x, data.topY,	&px, &pty );
		g->transformDataToPosition( x, data.bottomY, &px, &pby );
		g->transformDataToPosition( x, data.rightY,  &px, &pry );

		int ppx  = data.px;
		int pply = roundnum( ply );
		int ppty = roundnum( pty );
		int ppby = roundnum( pby );
		int ppry = roundnum( pry );

		// insert point
		if( ppx > prevPx + 1 ) {		// interpolation
			if( points.size() > 0 ) {   // insert previous right position
				if( prevRightY != points.back() ) {
					points.push_back( prevPx );
					points.push_back( prevRightY );
				}
			}

			if( pply != ppty ) {		// insert left point				
				points.push_back( ppx );
				points.push_back( pply );
			}
		}

		points.push_back( ppx );
		points.push_back( ppty );
		if( ppby != ppty ) {
			points.push_back( ppx );
			points.push_back( ppby );
		}

		// update previous
		prevPx = ppx;
		prevRightY = ppry;
		prevX = x;
	}

	if( points.size() > 0 ) {
		g->drawLines( &points[ 0 ], points.size() / 2 );
	}
}

// draw impulse	
void ProfileDrawer::drawImpulse(
		kome::img::DrawingXYData* drawData,
		kome::img::Graphics* g
) {
	// width
	unsigned int width = g->getGraphWidth();

	// get draw level
	int level = drawData->getLevel( width, g->getMinX(), g->getMaxX() );

	// get index
	int startIndex = drawData->searchIndex( level, g->getMinX() );
	if( startIndex < 0 ) {
		startIndex = - startIndex - 2;
		startIndex = MAX( 0, startIndex );
	}

	int maxIndex = (int)drawData->getLength( level ) - 1;
	int endIndex = drawData->searchIndex( level, g->getMaxX() );
	if( endIndex < 0 ) {
		endIndex = - endIndex - 1;
		endIndex = MIN( endIndex, maxIndex );
	}

	int prevPx = -1;
	for( int i = startIndex; i <= endIndex; i++ ) {
		// get coordinate
		double x  = drawData->getX( level, i );
		double ty = drawData->getTopY( level, i );
		double by = drawData->getBottomY( level, i );

		// draw
		double px = double();
		double py = double();

		g->transformDataToPosition( x, 0.0, &px, &py );
		int pzeroy = roundnum( py );

		g->transformDataToPosition( x, ty, &px, &py );
		int ppx  = roundnum( px );
		int ppy = roundnum( py );

		g->drawLine( ppx, pzeroy, ppx, ppy );

		g->transformDataToPosition( x, by, &px, &py );
		ppx  = roundnum( px );
		ppy = roundnum( py );

		g->drawLine( ppx, pzeroy, ppx, ppy );
	}
}

// delete spectrum data
void ProfileDrawer::deleteSpectrumData( kome::objects::Spectrum* spectrum ) {
	// delete drawing data
	if( m_spectrumDrawMap.find( spectrum ) != m_spectrumDrawMap.end() ) {	// map has draw data
		delete m_spectrumDrawMap[ spectrum ];
		m_spectrumDrawMap.erase( spectrum );
	}else{
		for( std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* >::iterator it = m_spectrumDrawMap.begin();
			it != m_spectrumDrawMap.end(); it++ ){
			kome::objects::Spectrum* spec = it->first;
			if( spec != NULL ){
				if( spec->getOrgSpectrum() == spectrum->getOrgSpectrum() ){
					spec->deleteXYData();
					delete m_spectrumDrawMap[ spec ];
					m_spectrumDrawMap.erase( spec );
					break;
				}
			}
		}
	}

	// delete data points
	spectrum->deleteXYData();
}

// delete chromatogram data
void ProfileDrawer::deleteChromatogramData( kome::objects::Chromatogram* chromatogram ) {
	// delete drawing data
	if( m_chromatogramDrawMap.find( chromatogram ) != m_chromatogramDrawMap.end() ) {	// map has draw data
		delete m_chromatogramDrawMap[ chromatogram ];
		m_chromatogramDrawMap.erase( chromatogram );
	}

	// delete data points
	chromatogram->deleteXYData();
}

// on close sample
void ProfileDrawer::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// get spectrum list
	std::list< kome::objects::Spectrum* > specList;
	for( std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* >::iterator it = m_spectrumDrawMap.begin();
			it != m_spectrumDrawMap.end(); it++ ) {
		// spectrum
		kome::objects::Spectrum* spec = (*it).first;

		if( spec->getSample() == sample ) {
			// delete drawing data
			delete (*it).second;

			// insert to list
			specList.push_back( spec );
		}
	}	

	// delete spectrum from map
	for( std::list< kome::objects::Spectrum* >::iterator it = specList.begin(); it != specList.end(); it++ ) {
		m_spectrumDrawMap.erase( *it );
	}
	specList.clear();

	// get chromatogram list
	std::list< kome::objects::Chromatogram* > chromList;
	for( std::map< kome::objects::Chromatogram*, kome::img::DrawingXYData* >::iterator it = m_chromatogramDrawMap.begin();
			it != m_chromatogramDrawMap.end(); it++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = (*it).first;

		if( chrom->getSample() == sample ) {
			// delete drawing data
			delete (*it).second;

			// insert to list
			chromList.push_back( chrom );
		}
	}

	// delete chromatogram from map
	for( std::list< kome::objects::Chromatogram* >::iterator it = chromList.begin(); it != chromList.end(); it++ ) {
		m_chromatogramDrawMap.erase( *it );
	}
	chromList.clear();
}

// on close spectrum
void ProfileDrawer::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	deleteSpectrumData( spec );
}

// on close chromatogram
void ProfileDrawer::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	deleteChromatogramData( chrom );
}

// get instance
ProfileDrawer& ProfileDrawer::getInstance() {
	// create object (This is the only object.)
	static ProfileDrawer drawer;

	return drawer;
}
