/**
 * @file Ab3dAlignmentTIC.cpp
 * @brief implements of Ab3dAlignmentTIC class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dAlignmentTIC.h"

#include <boost/bind.hpp>


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BASELINE_PARAM_NAME				"base_sub"
#define SMOOTHING_PARAM_NAME			   "smoothing"
#define WINDOW_WIDTH_PARAM_NAME			"window_width"
#define POINT_DENSITY_PARAM_NAME		   "density"
#define GAP_PENALTY_PARAM_NAME			 "gap_penalty"
#define INVALID_SCORE_PARAM_NAME		   "min_score"


#define BASELINE_TYPE_NAME				 "XYDATA_BASELINE"
#define SMOOTHING_TYPE_NAME				"XYDATA_FILTER"


// constructor
Ab3dAlignmentTIC::Ab3dAlignmentTIC( kome::objects::SettingParameterValues* settings )
		: Ab3dAlignment( settings ) {
	// initialize
	m_row = 0;
	m_col = 0;
	m_ctrlStart = 0.0;
	m_trmtStart = 0.0;

	m_ctrlIntensities = NULL;
	m_trmtIntensities = NULL;

	m_scoreMatrix = NULL;
	m_dp = NULL;

	// settings
	std::string s;

	// plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// baseline
	const char* baseline = NULL;
	if( settings != NULL ) {
		baseline = settings->getParameterValue( BASELINE_PARAM_NAME );
	}
	m_baselineItem = plgMgr.getFunctionItem( BASELINE_TYPE_NAME, baseline );

	// spectrum filter
	const char* smoothing = NULL;
	if( settings != NULL ) {
		smoothing = settings->getParameterValue( SMOOTHING_PARAM_NAME );
	}
	m_smoothingItem = plgMgr.getFunctionItem( SMOOTHING_TYPE_NAME, smoothing );

	// window width
	m_windowWidth = 10.0;
	if( settings != NULL ) {
		m_windowWidth = settings->getDoubleValue( WINDOW_WIDTH_PARAM_NAME, 10.0 );
	}

	// point density 
	m_pointDensity = 5;
	if( settings != NULL ) {
		m_pointDensity = settings->getIntValue( POINT_DENSITY_PARAM_NAME, 5 );
	}
	if( m_pointDensity < 2 ) {
		m_pointDensity = 2;
	}

	// gap penalty	
	m_gapPenalty = 0.4;
	if( settings != NULL ) {
		m_gapPenalty = settings->getDoubleValue( GAP_PENALTY_PARAM_NAME, 0.4 );
	}

	// invalid score
	m_invalidScore = -100.0;
	if( settings != NULL ) {
		m_invalidScore = settings->getDoubleValue( INVALID_SCORE_PARAM_NAME, -100.0 );
	}
}

// destructor
Ab3dAlignmentTIC::~Ab3dAlignmentTIC() {
	if( m_ctrlIntensities != NULL ) {
		delete[] m_ctrlIntensities;
	}
	if( m_trmtIntensities != NULL ) {
		delete[] m_trmtIntensities;
	}
	if( m_scoreMatrix != NULL ) {
		delete[] m_scoreMatrix;
	}
	if( m_dp != NULL ) {
		delete m_dp;
	}
}

// get score
double Ab3dAlignmentTIC::getScore( const int row, const int col ) {
	// check the size
	if( row < 0 || row >= m_row || col < 0 || col >= m_col ) {
		return 0.0;
	}

	// index
	int index = row * m_col + col;
	if( m_scoreMatrix[ index ] >= -1.0 ) {
		return m_scoreMatrix[ index ];
	}

	// calculate
	double score = kome::numeric::Statistics::pearson(
			m_ctrlIntensities + ( row * ( m_pointDensity - 1 ) ),
			m_trmtIntensities + ( col * ( m_pointDensity - 1 ) ),
			m_pointDensity
	);
	m_scoreMatrix[ index ] = score;

	return score;
}

// creates points
void Ab3dAlignmentTIC::createPoints() {
	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// chromatograms
	kome::objects::DataSet ctrlSpectra;
	getSpectra( getStandardSample(), ctrlSpectra );
	kome::objects::SpectraChromatogram ctrlChrom( ctrlSpectra );
	ctrlChrom.setTIC();
	ctrlChrom.setFilter( 0x01 );

	kome::objects::DataSet trmtSpectra;
	getSpectra( getTreatmentSample(), trmtSpectra );
	kome::objects::SpectraChromatogram trmtChrom( trmtSpectra );
	trmtChrom.setTIC();
	trmtChrom.setFilter( 0x01 );

	// add operation
	if( m_baselineItem != NULL ) {
		kome::operation::BaselineSubtract* bs = new kome::operation::BaselineSubtract();

		kome::objects::SettingParameterValues* baselineSettings = NULL;
		if( m_settings != NULL ) {
			baselineSettings = m_settings->getSubParameterValues( BASELINE_PARAM_NAME, m_baselineItem->getShortName() );
		}
		bs->setBaselineInfo( m_baselineItem->getCall(), baselineSettings );

		ptMgr.addOperation( &ctrlChrom, bs );
		ptMgr.addOperation( &trmtChrom, bs );
	}
	if( m_smoothingItem != NULL ) {
		kome::operation::Filter* filter = new kome::operation::Filter();

		kome::objects::SettingParameterValues* filterSettings = NULL;
		if( m_settings != NULL ) {
			filterSettings = m_settings->getSubParameterValues( SMOOTHING_PARAM_NAME, m_smoothingItem->getShortName() );
		}

		filter->setFilterInfo( m_smoothingItem->getCall(), filterSettings );

		ptMgr.addOperation( &ctrlChrom, filter );
		ptMgr.addOperation( &trmtChrom, filter );
	}

	// xy data
	kome::core::XYData* ctrlData = ctrlChrom.getXYData();
	kome::core::XYData* trmtData = trmtChrom.getXYData();

	int ctrlMinIndex = roundnum( ctrlData->getMinX() * 60.0 / m_windowWidth );
	int ctrlMaxIndex = roundnum( ctrlData->getMaxX() * 60.0 / m_windowWidth );
	int trmtMinIndex = roundnum( trmtData->getMinX() * 60.0 / m_windowWidth );
	int trmtMaxIndex = roundnum( trmtData->getMaxX() * 60.0 / m_windowWidth );

	m_row = ctrlMaxIndex - ctrlMinIndex + 1;
	m_col = trmtMaxIndex - trmtMinIndex + 1;

	m_ctrlStart = m_windowWidth * (double)ctrlMinIndex;
	m_trmtStart = m_windowWidth * (double)trmtMinIndex;

	int ctrlNum = m_row * ( m_pointDensity - 1 ) + 1;
	int trmtNum = m_col * ( m_pointDensity - 1 ) + 1;

	// create array
	if( m_ctrlIntensities != NULL ) {
		delete[] m_ctrlIntensities;
	}
	if( m_row > 0 ) {
		m_ctrlIntensities = new double[ ctrlNum ];

		fillzero( m_ctrlIntensities, sizeof( double) * ctrlNum );
	}
	if( m_trmtIntensities != NULL ) {
		delete[] m_trmtIntensities;
	}
	if( m_col > 0 ) {
		m_trmtIntensities = new double[ trmtNum ];

		fillzero( m_trmtIntensities, sizeof( double ) * trmtNum );
	}

	// intensities
	double idxWidth = m_windowWidth / (double)( m_pointDensity - 1 );

	double startPos = m_ctrlStart - m_windowWidth / 2.0 + idxWidth / 2.0;
	int prev = 0;
	double prevX = 0.0;
	double prevY = 0.0;	
	for( unsigned int i = 0; i < ctrlData->getLength(); i++ ) {		// control
		// x, y
		double x = ctrlData->getX( i ) * 60.0;
		double y = ctrlData->getY( i );

		// index
		int idx = (int)roundnum( ( x - startPos ) / idxWidth );
		int maxIdx = ctrlNum - 1;
		idx = CLAMP( idx, 0, maxIdx );

		// intensity
		if( i == 0 || idx == prev ) {
			m_ctrlIntensities[ idx ] = MAX( y, m_ctrlIntensities[ idx ] );
		}
		else {
			m_ctrlIntensities[ idx ] = y;
			for( int j = prev + 1; j < idx; j++ ) {
				double tmpX = startPos + (double)j * idxWidth;
				double tmpY = ( ( tmpX - prevX ) * y + ( x - tmpX ) * prevY ) / ( x - prevX );

				m_ctrlIntensities[ j ] = tmpY;
			}
		}

		// previous
		prev = idx;
		prevX = x;
		prevY = y;
	}

	startPos = m_trmtStart - m_windowWidth / 2.0 + idxWidth / 2.0;
	prev = 0;
	prevX = 0.0;
	prevY = 0.0;	
	for( unsigned int i = 0; i < trmtData->getLength(); i++ ) {		// treatment
		// x, y
		double x = trmtData->getX( i ) * 60.0;
		double y = trmtData->getY( i );

		// index
		int idx = (int)roundnum( ( x - startPos ) / idxWidth );
		int maxIdx = trmtNum - 1;
		idx = CLAMP( idx, 0, maxIdx );

		// intensity
		if( i == 0 || idx == prev ) {
			m_trmtIntensities[ idx ] = MAX( y, m_trmtIntensities[ idx ] );
		}
		else {
			m_trmtIntensities[ idx ] = y;
			for( int j = prev + 1; j < idx; j++ ) {
				double tmpX = startPos + (double)j * idxWidth;
				double tmpY = ( ( tmpX - prevX ) * y + ( x - tmpX ) * prevY ) / ( x - prevX );

				m_trmtIntensities[ j ] = tmpY;
			}
		}

		// previous
		prev = idx;
		prevX = x;
		prevY = y;
	}
}

// initialize score matrix
void Ab3dAlignmentTIC::initScoreMatrix() {
	// delete
	if( m_scoreMatrix != NULL ) {
		delete[] m_scoreMatrix;
	}

	// create matrix
	unsigned int num = m_row * m_col;
	m_scoreMatrix = new double[ num ];

	// set init value
	for( unsigned int i = 0; i < num; i++ ) {
		m_scoreMatrix[ i ] = -10.0;
	}
}

// execute dynamic programing
bool Ab3dAlignmentTIC::execDp( kome::core::Progress* progress ) {
	// create DP object
	if( m_dp != NULL ) {
		delete m_dp;
	}
	m_dp = new kome::numeric::DynamicPrograming( m_row, m_col );
	m_dp->setRowTitleFunction( boost::bind( &Ab3dAlignmentTIC::getRowTitle, this, _1 ) );
	m_dp->setColTitleFunction( boost::bind( &Ab3dAlignmentTIC::getColTitle, this, _1 ) );

	// execute
	bool ret = m_dp->calculate(
		boost::bind( &Ab3dAlignmentTIC::getScore, this, _1, _2 ),
		m_gapPenalty,
		m_invalidScore,
		progress
	);

	LOG_DEBUG(
		FMT(
			"AB3D Alignment : DP [row = %d, col = %d, Score = %f]",
			m_row,
			m_col,
			m_dp->getAccumulatedScore( m_row - 1, m_col - 1 )
		)
	);

	return ret;
}

// traceback
void Ab3dAlignmentTIC::traceback( int row, int col, kome::core::XYData* xyData ) {
	if( row > m_tbPt.py && col > m_tbPt.px ) {
		double x = m_ctrlStart + (double)col * m_windowWidth;
		double y = m_trmtStart + (double)row * m_windowWidth;

		xyData->addPoint( x, y );
	}
	m_tbPt.py = row;
	m_tbPt.px = col;
}

// get row title
std::string Ab3dAlignmentTIC::getRowTitle( const int row ) {
	std::string title;

	if( row < 0 ) {
		title = "Control.";
	}
	else if( row < m_row ) {
		double rt = m_ctrlStart +  (double)row * m_windowWidth;
		title = FMT( "%.2f", rt );
	}

	return title;
}

// get column title
std::string Ab3dAlignmentTIC::getColTitle( const int col ) {
	std::string title;

	if( col < 0 ) {
		title = "Treatment.";
	}
	else if( col < m_col ) {
		double rt = m_trmtStart +  (double)col * m_windowWidth;
		title = FMT( "%.2f", rt );
	}

	return title;
}

// prepare
void Ab3dAlignmentTIC::onPrepare(
		kome::objects::Sample* standard,
		kome::objects::Sample* treatment,
		kome::core::Progress* progress
) {
	// log
	LOG_INFO(
		FMT(
			"Prepare AB3D Alignment (TIC Fitting) [baseline=%s, smoothing=%s, width=%.2f, density=%d, gap=%.2f, invalid=%.2f]",
			( m_baselineItem == NULL ) ? "none" : m_baselineItem->getShortName(),
			( m_smoothingItem == NULL ) ? "none" : m_smoothingItem->getShortName(),
			m_windowWidth,
			m_pointDensity,
			m_gapPenalty,
			m_invalidScore
		)
	);

	// create points
	createPoints();
	if( m_row == 0 || m_col == 0 ) {
		LOG_WARN_CODE( FMT( "Failed to get chromatogram data." ), ERR_OTHER );
		return;
	}

	// initialize score matrix
	initScoreMatrix();

	// execute dynamic programing
	if( !execDp( progress ) ) {
		return;
	}

	// trace back
	kome::core::DataPoints pts;

	m_tbPt.px = -1;
	m_tbPt.py = -1;
	m_dp->traceback( boost::bind( &Ab3dAlignmentTIC::traceback, this, _1, _2, &pts ) );

	// alignment
	getCorrectionPoints( m_corrPts, pts );

	// inverce points
	for( unsigned int i = 0; i < m_corrPts.getLength(); i++ ) {
		m_invPts.addPoint( m_corrPts.getY( i ), m_corrPts.getX( i ) );
	}
}


// on convert RT
double Ab3dAlignmentTIC::onConvertRt( const double rt, const double mz ) {
	// return value
	double ret = kome::numeric::Interpolation::linear( m_corrPts, rt );
	return ret;
}

// on convert m/z
double Ab3dAlignmentTIC::onConvertMz( const double rt, const double mz ) {
	return mz;
}

// on invert RT
double Ab3dAlignmentTIC::onInvertRt( const double rt, const double mz ) {
	// return value
	double ret = kome::numeric::Interpolation::linear( m_invPts, rt );
	return ret;
}

// on invert m/z
double Ab3dAlignmentTIC::onInvertMz( const double rt, const double mz ) {
	return mz;
}
