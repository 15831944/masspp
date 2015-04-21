/**
 * @file DrawingXYData.cpp
 * @brief implements of DrawingXYData class
 *
 * @author S.Tanaka
 * @date 2006.09.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DrawingXYData.h"

#include <math.h>
#include <boost/bind.hpp>

using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define MIN_WIDTH  0.01
#define MAX_LEVEL  10


// constructor
DrawingXYData::DrawingXYData( kome::core::XYData& xyData ) {
	// initialize
	m_data = NULL;
	m_xyData = &xyData;

	// create data to draw profile fast
	createArray();
}

// destructor
DrawingXYData::~DrawingXYData() {
	if( m_data != NULL ) {
		delete[] m_data;
		m_data = NULL;
	}
}

// update
void DrawingXYData::updatePoints() {
	createArray();
}

// get the number of points
unsigned int DrawingXYData::getLength( const unsigned short level ) {
	// check parameter
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}

	// get the number of points
	if( nFixedLevel == 0 ) {
		return m_xyData->getLength();
	}
	return (unsigned int)m_data[ nFixedLevel - 1 ].size();
}

// get x
double DrawingXYData::getX(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get x
	if( nFixedLevel == 0 ) {
		return m_xyData->getX( index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].x;
}

// get left x
double DrawingXYData::getLeftX(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );

	// get left x
	if( nFixedLevel == 0 ) {
		return getY( *m_xyData, index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].leftX;
}

// get left y
double DrawingXYData::getLeftY(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get left y
	if( nFixedLevel == 0 ) {
		return getY( *m_xyData, index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].leftY;
}

// get top x
double DrawingXYData::getTopX(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get top y
	if( nFixedLevel == 0 ) {
		return m_xyData->getX( index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].topX;
}

// get top y
double DrawingXYData::getTopY(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get top y
	if( nFixedLevel == 0 ) {
		return getY( *m_xyData, index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].topY;
}

// get bottom x
double DrawingXYData::getBottomX(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get bottom x
	if( nFixedLevel == 0 ) {
		return m_xyData->getX( index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].bottomX;
}

// get bottom y
double DrawingXYData::getBottomY(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get bottom y
	if( nFixedLevel == 0 ) {
		return getY( *m_xyData, index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].bottomY;
}

// get right x
double DrawingXYData::getRightX(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get right x
	if( nFixedLevel == 0 ) {
		return m_xyData->getX( index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].rightX;
}

// get right y
double DrawingXYData::getRightY(
		const unsigned short level,
		const unsigned int index
) {
	// check index
	unsigned short nFixedLevel = level;
	if( nFixedLevel > MAX_LEVEL ) {
		nFixedLevel = MAX_LEVEL;	// @date 2012/01/18 <Add> OKADA
	}
	checkIndex( nFixedLevel, index );
		
	// get right y
	if( nFixedLevel == 0 ) {
		return getY( *m_xyData, index );
	}
	return m_data[ nFixedLevel - 1 ][ index ].rightY;
}

// search index
int DrawingXYData::searchIndex( const unsigned short level, const double x ) {
	// comparator
	class SearchHelper {
	public:
		// compare to search
		static int compare( double x0, double x1 ) {
			if( x0 < x1 ) {
				return -1;
			}
			if( x0 > x1 ) {
				return 1;
			}
			return 0;
		}

		// get x
		static double getX( int index, short level, DrawingXYData* data ) {
			return data->getX( level, index );
		}
	};

	// search
	return kome::core::SearchTool::binarySearch< double, double >(
		x,
		boost::bind( SearchHelper::getX, _1, level, this ),
		SearchHelper::compare,
		getLength( level )
	);
}

// check index
void DrawingXYData::checkIndex(
		const unsigned short level,
		const unsigned int index
) {
	// get number of points
	unsigned int length = getLength( level );

	// check index
	if( index >= length ) {
		std::string msg = FMT(
			"The index(%d) is larger than max index(%d).",
			index,
			length
		);

		LOG_WARN( msg );
		throw msg;
	}
}

// create array
void DrawingXYData::createArray() {
	// initialize
	if( m_data != NULL ) {
		delete[] m_data;
	}

	// prepare
	m_data = new std::vector< SumPoint >[ MAX_LEVEL ];

	// create array
	double width = MIN_WIDTH;
	for( unsigned int i = 0; i < MAX_LEVEL; i++ ) {
		// prepare
		int prevIdx = int();

		// create each array
		for( unsigned int j = 0; j < getLength( i ); j++ ) {
			// get coordinates
			double x = getX( i, j );
			double leftX = getLeftX( i, j );
			double leftY = getLeftY( i, j );
			double topX = getTopX( i, j );
			double topY = getTopY( i, j );
			double bottomX = getBottomX( i, j );
			double bottomY = getBottomY( i, j );
			double rightX = getRightX( i, j );
			double rightY = getRightY( i, j );

			// get index
			int index = roundnum( x / width );

			// update data
			if( j > 0 && index == prevIdx ) {		// update last data
				SumPoint* pt = &m_data[ i ].back();

				pt->rightX = rightX;
				pt->rightY = rightY;
				if( topY > pt->topY ) {
					pt->topX = topX;
					pt->topY = topY;
				}
				if( bottomY < pt->bottomY ) {
					pt->bottomX = bottomX;
					pt->bottomY = bottomY;
				}
			}
			else {	// push data
				SumPoint lastData = {
					width * (double)index,
					leftX,
					leftY,
					topX,
					topY,
					bottomX,
					bottomY,
					rightX,
					rightY
				};

				m_data[ i ].push_back( lastData );
			}

			// keep index
			prevIdx = index;
		}

		// update width
		width *= 2.0;
	}
}

// get y
double DrawingXYData::getY( kome::core::XYData& xyData, const int idx ) {
	// peaks
	kome::objects::Peaks* peaks = dynamic_cast< kome::objects::Peaks* >( &xyData );
	if( peaks == NULL ) {
		return xyData.getY( idx );
	}

	// apex
	return peaks->getPeak( idx )->getApexY();
}

// get level
unsigned short DrawingXYData::getLevel(
		const unsigned int width,
		const double minX,
		const double maxX
) {
	// check width
	if( width == 0 ) {
		return MAX_LEVEL;
	}

	// calc level
	double range = maxX - minX;
	double pxRange = range / (double)width;

	int level = (int)floor( log( pxRange / MIN_WIDTH ) / log( 2.0 ) );

	level = CLAMP( level, 0, MAX_LEVEL );

	return level;
}
