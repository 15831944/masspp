/**
 * @file DataMapDrawer.cpp
 * @brief implements of DataMapDrawer class
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataMapDrawer.h"
#include "ColorTheme.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define NAVI_TOP				 10
#define NAVI_HEIGHT				 20
#define NAVI_SPACE				 10

#define SECTION					 "Data Map Draw"
#define THEME_PARAM_NAME		 "COLOR_THEME"
#define SCALING_PARAM_NAME		 "DRAW_SCALE"


// constructor
DataMapDrawer::DataMapDrawer() {
	m_scaling = SCALE_LINEAR;
	m_theme = NULL;

	createThemes();
	updateStatus();
}

// destructor
DataMapDrawer::~DataMapDrawer() {
	// delete themes
	for( unsigned int i = 0; i < m_themes.size(); i++ ) {
		delete m_themes[ i ];
	}
	m_themes.clear();
}

// draw
void DataMapDrawer::draw( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap ) {
	// check map
	if( m_map.find( &dataMap ) == m_map.end() ) {
		createBitmap( g, dataMap );
	}

	// get bitmap
	BitmapInfo* bitmap = &( m_map[ &dataMap ] );

	// draw
	g.drawBitmap(
		bitmap->bitmap,
		g.getGraphPositionLeft(),
		g.getGraphPositionTop(),
		false
	);
}

// draw navi bar
void DataMapDrawer::drawNaviBar( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap ) {
	// color
	COLOR oldColor = g.getColor();
	g.setColor( kome::img::ColorTool::BLACK );

	// check map
	if( m_map.find( &dataMap ) == m_map.end() ) {
		createBitmap( g, dataMap );
	}

	// get bitmap
	BitmapInfo* bitmap = &( m_map[ &dataMap ] );

	// draw value
	if( bitmap->naviLeft > 0 ) {
		// intensity
		std::string minIntStr = FMT( "%.2f", bitmap->minInt );
		std::string maxIntStr = FMT( "%.2f", bitmap->maxInt );

		int left = g.getGraphPositionLeft();
		int right = g.getGraphPositionRight();

		// min value
		g.setTextVAlign( kome::img::Graphics::VALIGN_MIDDLE );
		g.setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
		g.drawText( minIntStr.c_str(), left, NAVI_TOP + NAVI_HEIGHT / 2 );

		// max value
		g.setTextHAlign( kome::img::Graphics::HALIGN_RIGHT );
		g.drawText( maxIntStr.c_str(), right, NAVI_TOP + NAVI_HEIGHT / 2 );
	}

	// left, right
	int left = g.getGraphPositionLeft() + bitmap->naviLeft;
	int right = g.getGraphPositionRight() - bitmap->naviRight;

	// draw bar
	g.drawBitmap( bitmap->navi, left, NAVI_TOP, false );

	// draw rect
	g.setColor( kome::img::ColorTool::BLACK );
	int rectPts[] = {
		left, NAVI_TOP,
		left, ( NAVI_TOP + NAVI_HEIGHT ),
		right, ( NAVI_TOP + NAVI_HEIGHT ),
		right, NAVI_TOP,
		left, NAVI_TOP
	};
	g.drawLines( rectPts, 5 );

	// restore
	g.setColor( oldColor );
}

// prepare
void DataMapDrawer::prepare( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap ) {
	// top margin
	int topPos = g.getGraphPositionTop();
	topPos = std::max( topPos, NAVI_TOP + NAVI_HEIGHT + NAVI_SPACE );
	g.setGraphPositionTop( topPos );

	// check map
	if( m_map.find( &dataMap ) == m_map.end() ) {
		return;
	}

	// graph size
	int width = (int)g.getGraphPositionRight() - (int)g.getGraphPositionLeft() + 1;
	int height = (int)g.getGraphPositionBottom() - (int)g.getGraphPositionTop() + 1;

	// check bitmap information
	BitmapInfo* info = &( m_map[ &dataMap ] );
	double minInt = m_theme->getMinIntensity( dataMap );
	double maxInt = m_theme->getMaxIntensity( dataMap );
	if( fabs( g.getMinY() - info->minRt ) > 0.01 || fabs( g.getMaxY() - info->maxRt ) > 0.01
			|| fabs( g.getMinX() - info->minMz ) > 0.01 || fabs( g.getMaxX() - info->maxMz ) > 0.01
			|| fabs( minInt - info->minInt ) > 0.01 || fabs( maxInt - info->maxInt ) > 0.01
			|| width != info->width || height != info->height 
	) {
		deleteData( dataMap );
	}
}

// delete
void DataMapDrawer::deleteData( kome::objects::DataMapInfo& dataMap ) {
	m_map.erase( &dataMap );
}

// update satatus
void DataMapDrawer::updateStatus() {
	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// theme
	ColorTheme* theme = NULL;
	std::string themeName;
	if( ini != NULL ) {
		themeName = ini->getString( SECTION, THEME_PARAM_NAME, "" );
	}
	for( unsigned int i = 0; i < m_themes.size() && theme == NULL; i++ ) {
		ColorTheme* tmp = m_themes[ i ];
		if( themeName.compare( tmp->getName() ) == 0 ) {
			theme = tmp;
		}
	}

	if( theme == NULL ) {
		theme = m_themes[ 0 ];
	}
	if( m_theme != theme ) {
		m_map.clear();
		m_theme = theme;
	}

	// scale
	Scaling scaling = SCALE_LINEAR;
	std::string scaleName;
	if( ini != NULL ) {
		scaleName = ini->getString( SECTION, SCALING_PARAM_NAME, "" );
	}

	if( scaleName.compare( "square" ) == 0 ) {
		scaling = SCALE_SQUARE;
	}
	else if( scaleName.compare( "log" ) == 0 ) {
		scaling = SCALE_LOG;
	}

	if( m_scaling != scaling ) {
		m_map.clear();
		m_scaling = scaling;
	}
}

// delete
void DataMapDrawer::deleteData( kome::objects::Sample& sample ) {
	// get data maps
	std::vector< kome::objects::DataMapInfo* > dataMaps;

	for( std::map< kome::objects::DataMapInfo*, BitmapInfo >::iterator it = m_map.begin();
			it != m_map.end(); it++ ) {
		// sample
		kome::objects::DataMapInfo* dataMap = (*it).first;
		kome::objects::DataGroupNode* group = &( dataMap->getGroup() );
		kome::objects::Sample* rd = group->getSample();

		// add to array
		if( rd == &sample ) {
			dataMaps.push_back( dataMap );
		}
	}

	// delete
	for( unsigned int i = 0; i < dataMaps.size(); i++ ) {
		deleteData( *( dataMaps[ i ] ) );
	}
}

// create bitmap data
void DataMapDrawer::createBitmap( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap ) {
	// delete
	deleteData( dataMap );

	// size
	int width = (int)g.getGraphPositionRight() - (int)g.getGraphPositionLeft() + 1;
	int height = (int)g.getGraphPositionBottom() - (int)g.getGraphPositionTop() + 1;
	if( width <= 0 || height <= 0 ) {
		return;
	}
	int size = width * height;

	// get bitmap
	BitmapInfo* info = &( m_map[ &dataMap ] );

	// unit
	double dRt = ( dataMap.getMaxRt() - dataMap.getMinRt() ) / (double)( dataMap.getRowSize() - 1 );
	double dMz = ( dataMap.getMaxMz() - dataMap.getMinMz() ) / (double)( dataMap.getColSize() - 1 );

	double* ints = new double[ size ];
	bool* flgs = new bool[ size ];
	for( int i = 0; i < size; i++ ) {
		ints[ i ] = 0.0;
		flgs[ i ] = false;		
	}

	// set int
	for( int i = 0; i < (int)dataMap.getRowSize(); i++ ) {
		// RT
		double rt = dataMap.getMinRt() + dRt * (double)i;

		for( int j = 0; j < (int)dataMap.getColSize(); j++ ) {
			// m/z
			double mz = dataMap.getMinMz() + dMz * (double)j;

			// position
			double x = double();
			double y = double();
			g.transformDataToPosition( mz, rt, &x, &y );
			int px = roundnum( x ) - g.getGraphPositionLeft();
			int py = roundnum( y ) - g.getGraphPositionTop();

			if( px >= 0 && px < width && py >= 0 && py < height ) {
				int idx = py * width + px;
				double intensity = dataMap.getIntensity( i, j );

				// set intensity
				if( !flgs[ idx ] || fabs( intensity ) > fabs( ints[ idx ] ) ) {
					flgs[ idx ] = true;
					ints[ idx ] = intensity;
				}
			}
		}
	}

	// set intensities
	for( int i = 0; i < height; i++ ) {
		for( int j = 0; j < width; j++ ) {
			// index
			int idx = i * width + j;

			// get intensity
			if( !flgs[ idx ] ) {
				// RT, mz
				double rt = double();
				double mz = double();

				g.transformPositionToData(
					g.getGraphPositionLeft() + j,
					g.getGraphPositionTop() + i,
					&mz,
					&rt
				);

				double rowIdx = ( rt - dataMap.getMinRt() ) / dRt;
				double colIdx = ( mz - dataMap.getMinMz() ) / dMz;

				int rowIdx0 = (int)floor( rowIdx );
				int rowIdx1 = (int)ceil( rowIdx );
				int colIdx0 = (int)floor( colIdx );
				int colIdx1 = (int)ceil( colIdx );

				if( rowIdx0 < (int)dataMap.getRowSize() && rowIdx1 >= 0
						&& colIdx0 < (int)dataMap.getColSize() && colIdx1 >= 0 ) {

					// index
					if( rowIdx0 < 0 ) {
						rowIdx0 = 0;
					}
					if( rowIdx1 >= (int)dataMap.getRowSize() ) {
						rowIdx1 = (int)dataMap.getRowSize() - 1;
					}
					if( colIdx0 < 0 ) {
						colIdx0 = 0;
					}
					if( colIdx1 >= (int)dataMap.getColSize() ) {
						colIdx1 = (int)dataMap.getColSize() - 1;
					}

					double rRate0 = (double)rowIdx1 - rowIdx;
					double rRate1 = 1.0 - rRate0;
					double cRate0 = (double)colIdx1 - colIdx;
					double cRate1 = 1.0 - cRate0;

					// intensity
					double intensity = dataMap.getIntensity( rowIdx0, colIdx0 ) * ( rRate0 * cRate0 )
									+ dataMap.getIntensity( rowIdx0, colIdx1 ) * ( rRate0 * cRate1 )
									+ dataMap.getIntensity( rowIdx1, colIdx0 ) * ( rRate1 * cRate0 )
									+ dataMap.getIntensity( rowIdx1, colIdx1 ) * ( rRate1 * cRate1 );

					ints[ idx ] = intensity;
					flgs[ idx ] = true;
				}
			}
		}
	}

	// intensity
	double minInt = m_theme->getMinIntensity( dataMap );
	double maxInt = m_theme->getMaxIntensity( dataMap );
	double scaledMinInt = getScaledIntensity( minInt );
	double scaledMaxInt = getScaledIntensity( maxInt );

	// bitmap
	info->bitmap.resize( width, height );
	for( int i = 0; i < height; i++ ) {
		for( int j = 0; j < width; j++ ) {
			// index
			int idx = i * width + j;

			// color
			const double int0 = ints[ idx ];
			const double intensity = getScaledIntensity( int0 );
			COLOR color = m_theme->getColor( intensity, scaledMinInt, scaledMaxInt );
			info->bitmap.setPixel( j, i, color );
		}
	}

	// navi bitmap
	int cx = 0;
	int cy = 0;

	std::string s = FMT( "%.2f", minInt );
	g.getTextSize( s.c_str(), &cx, &cy );
	int naviLeft = cx + 1;

	s = FMT( "%.2f", maxInt );
	g.getTextSize( s.c_str(), &cx, &cy );
	int naviRight = cx + 1;

	int naviWidth = g.getGraphPositionRight() - g.getGraphPositionLeft() - naviLeft - naviRight;
	if( naviWidth < 30 ) {
		naviLeft = 0;
		naviRight = 0;
		naviWidth = g.getGraphPositionRight() - g.getGraphPositionLeft();
		naviWidth = MAX( naviWidth, 0 );
	}
	info->navi.resize( naviWidth, NAVI_HEIGHT );
	if( naviWidth > 1 ) {
		for( int j = 0; j < naviWidth; j++ ) {
			double intensity = minInt + ( maxInt - minInt ) * (double)j / (double)( naviWidth - 1 );
			intensity = getScaledIntensity( intensity );

			COLOR color = m_theme->getColor( intensity, scaledMinInt, scaledMaxInt );
			for( int i = 0; i < NAVI_HEIGHT; i++ ) {
				info->navi.setPixel( j, i, color );
			}
		}
	}

	// set information
	info->minRt = g.getMinY();
	info->maxRt = g.getMaxY();
	info->minMz = g.getMinX();
	info->maxMz = g.getMaxX();
	info->minInt = minInt;
	info->maxInt = maxInt;
	info->width = width;
	info->height = height;
	info->naviLeft = naviLeft;
	info->naviRight = naviRight;

	// delete
	delete[] ints;
	delete[] flgs;
}

// get scaled intensity
double DataMapDrawer::getScaledIntensity( const double intensity ) {
	if( m_scaling == SCALE_LINEAR ) {		// linear
		return intensity;
	}

	// check sign
	bool neg = ( intensity < 0.0 );
	double val = fabs( intensity );

	// get value
	if( m_scaling == SCALE_SQUARE ) {
		val = sqrt( val );
	}
	else if( m_scaling == SCALE_LOG ) {
		val = log( val + 1.0 );
	}

	if( neg ) {
		val = - val;
	}

	return val;
}

// create theme
void DataMapDrawer::createThemes() {
	// gray scale
	ColorTheme* theme = new ColorTheme( "grayscale" );
	theme->setZeroMiddle( false );
	theme->setHighWidth( 0.1 );

	theme->addColor( 0.0, kome::img::ColorTool::WHITE );
	theme->addColor( 1.0, kome::img::ColorTool::BLACK );
	m_themes.push_back( theme );

	// thermography
	theme = new ColorTheme( "thermography" );
	theme->setZeroMiddle( false );
	theme->setHighWidth( 0.1 );

	theme->addColor( 0.0, kome::img::ColorTool::NAVY );
	theme->addColor( 0.5, kome::img::ColorTool::BLUE );
	theme->addColor( 1.0, kome::img::ColorTool::AQUA );
	theme->addColor( 1.0, kome::img::ColorTool::GREEN );
	theme->addColor( 1.0, kome::img::ColorTool::YELLOW );
	theme->addColor( 1.0, kome::img::ColorTool::RED );
	m_themes.push_back( theme );

	// flame
	theme = new ColorTheme( "flame" );
	theme->setZeroMiddle( true );
	theme->setHighWidth( 4.0 );

	theme->addColor( 4.0, kome::img::ColorTool::AQUA );
	theme->addColor( 16.0, kome::img::ColorTool::BLUE );
	theme->addColor( 1.0, kome::img::ColorTool::BLACK );
	theme->addColor( 1.0, kome::img::ColorTool::RED );
	theme->addColor( 16.0, kome::img::ColorTool::YELLOW );
	m_themes.push_back( theme );

	// french
	theme = new ColorTheme( "french" );
	theme->setZeroMiddle( true );
	theme->setHighWidth( 0.3 );

	theme->addColor( 0.3, kome::img::ColorTool::BLUE );
	theme->addColor( 1.0, kome::img::ColorTool::WHITE );
	theme->addColor( 1.0, kome::img::ColorTool::RED );
	m_themes.push_back( theme );

	// deepsea
	theme = new ColorTheme( "deepsea" );
	theme->setZeroMiddle( false );
	theme->setHighWidth( 0.1 );
	theme->addColor( 0.0, kome::img::ColorTool::BLACK );
	theme->addColor( 0.5, kome::img::ColorTool::NAVY );
	theme->addColor( 1.0, kome::img::ColorTool::BLUE );
	theme->addColor( 1.0, kome::img::ColorTool::AQUA );
	m_themes.push_back( theme );
}

// on close sample
void DataMapDrawer::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	deleteData( *sample );
}

// on close DataMap
void DataMapDrawer::onCloseDataMap(  kome::objects::DataMapInfo* dataMap, const bool deleting ){
	m_map.erase( dataMap );
}

// get instance
DataMapDrawer& DataMapDrawer::getInstance() {
	// create object (This is the only object.)
	static DataMapDrawer drawer;

	return drawer;
}
