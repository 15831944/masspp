/**
 * @file Ab3dAlignmentMC.cpp
 * @brief implements of Ab3dAlignmentMC class
 *
 * @author S.Tanaka
 * @date 2009.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dAlignmentMC.h"

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
#define MZ_TOL_PARAM_NAME				  "tol"
#define MZ_INTERVAL_PARAM_NAME			 "interval"

#define BASELINE_TYPE_NAME				 "XYDATA_BASELINE"
#define SMOOTHING_TYPE_NAME				"XYDATA_FILTER"


#define LIST_SIZE                    64


// constructor
Ab3dAlignmentMC::Ab3dAlignmentMC( kome::objects::SettingParameterValues* settings )
		: Ab3dAlignment( settings ) {
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

	// m/z tolerance
	m_mzTol = 0.5;
	if( settings != NULL ) {
		m_mzTol = settings->getDoubleValue( MZ_TOL_PARAM_NAME, m_mzTol );
	}

	// m/z interval
	m_mzInterval = 1.0;
	if( settings != NULL ) {
		m_mzInterval = settings->getDoubleValue( MZ_INTERVAL_PARAM_NAME, m_mzInterval );
	}
}

// destructor
Ab3dAlignmentMC::~Ab3dAlignmentMC() {
	// timer
	m_timer.stop();

	LOG_DEBUG( FMT( "AB3dAlignment (MC): %.2f sec", m_timer.getTotalTime() ) );

	// delete dir
	if( !m_tmpDir.empty() ) {
		kome::core::MsppManager& mgr = kome::core::MsppManager::getInstance();
		std::string dir = getpath( mgr.getTmpDir(), m_tmpDir.c_str() );

		removedirs( dir.c_str() );
	}

	// initialize
	initMc();
}

// set tmp dir
void Ab3dAlignmentMC::setTmpDir() {
	// check the member
	if( !m_tmpDir.empty() ) {
		return;
	}

	// get dir
	kome::core::MsppManager& mgr = kome::core::MsppManager::getInstance();
	m_tmpDir = FMT( "alignment/xic/%08x", (long)this & 0xffffffff );

	std::string path = getpath( mgr.getTmpDir(), m_tmpDir.c_str() );
	makedirs( path.c_str() );
}

// initializes MC information
void Ab3dAlignmentMC::initMc() {
	// clear
	m_mcMap.clear();
	m_mcList.clear();
}

// get MC information
std::pair< kome::core::XYData*, kome::core::XYData* > Ab3dAlignmentMC::getCorrectionPoint( const double mz ) {
	// create MC information
	int idx = roundnum( mz ) / m_mzInterval;
	if( idx < 0 ) {
		return std::make_pair( (kome::core::XYData*)NULL, (kome::core::XYData*)NULL );
	}

	// create correction points
	if( m_mcMap.find( idx ) == m_mcMap.end() ) {
		// MC points
		std::vector< double > ctrlInts;
		std::vector< double > trmtInts;
		int rowCnt = 0;
		int colCnt = 0;
		double ctrlStart = 0.0;
		double trmtStart = 0.0;
		createPoints(
			m_mzInterval * (double)idx,
			ctrlInts,
			trmtInts,
			&rowCnt,
			&colCnt,
			&ctrlStart,
			&trmtStart
		);

		// execute dynamic programing
		std::vector< double > scoreMatrix;
		kome::numeric::DynamicPrograming* dp = NULL;
		initScoreMatrix( scoreMatrix, rowCnt, colCnt );

		dp = execDp(
			ctrlInts,
			trmtInts,
			scoreMatrix,
			rowCnt,
			colCnt,
			ctrlStart,
			trmtStart
		);

		if( dp == NULL ) {
			return std::make_pair( (kome::core::XYData*)NULL, (kome::core::XYData*)NULL );
		}

		kome::core::DataPoints pts;
		kome::core::Point< int > tbPt( -1, -1 );
		dp->traceback(
			boost::bind(
				&Ab3dAlignmentMC::traceback,
				this,
				_1,
				_2,
				&pts,
				&tbPt,
				ctrlStart,
				trmtStart
			)
		);

		// alignment
		getCorrectionPoints( m_mcMap[ idx ].first, pts );

		// for inverse
		kome::core::XYData* dps0 = &( m_mcMap[ idx ].first );
		kome::core::XYData* dps1 = &( m_mcMap[ idx ].second );

		for( unsigned int i = 0; i < dps0->getLength(); i++ ) {
			dps1->addPoint( dps0->getY( i ), dps1->getY( i ) );
		}

		// list
		m_mcList.push_back( idx );
	}

	// delete
	while( m_mcList.size() > LIST_SIZE ) {
		int tmp = m_mcList.front();
		m_mcMap.erase( tmp );
		m_mcList.pop_front();
	}

	// return value
	std::pair< kome::core::XYData*, kome::core::XYData* > ret;
	ret.first = &( m_mcMap[ idx ].first );
	ret.second = &( m_mcMap[ idx ].second );

	return ret;
}

// get score
double Ab3dAlignmentMC::getScore(
		const int row,
		const int col,
		std::vector< double >& ctrlInts,
		std::vector< double >& trmtInts,
		std::vector< double >& scoreMatrix,
		const int rowCnt,
		const int colCnt
) {
	// check the size
	if( row < 0 || row >= rowCnt || col < 0 || col >= colCnt ) {
		return 0.0;
	}

	// index
	int index = row * colCnt + col;
	if( scoreMatrix[ index ] >= -1.0 ) {
		return scoreMatrix[ index ];
	}

	// calculate
	double score = kome::numeric::Statistics::pearson(
			&( ctrlInts[ 0 ] ) + ( row * ( m_pointDensity - 1 ) ),
			&( trmtInts[ 0 ] ) + ( col * ( m_pointDensity - 1 ) ),
			m_pointDensity
	);
	scoreMatrix[ index ] = score;

	return score;
}

// creates points
void Ab3dAlignmentMC::createPoints(
		const double mz,
		std::vector< double >& ctrlInts,
		std::vector< double >& trmtInts,
		int* rowCnt,
		int* colCnt,
		double* ctrlStart,
		double* trmtStart
) {
	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// control chromatogram
	kome::objects::DataSet ctrlSpectra;
	getSpectra( getStandardSample(), ctrlSpectra );
	kome::objects::SpectraChromatogram ctrlChrom( ctrlSpectra );
	ctrlChrom.setMzRange( std::max( 0.0, mz - m_mzTol ), mz + m_mzTol );
	ctrlChrom.setMz( mz );
	ctrlChrom.setTIC();
	ctrlChrom.setFilter( 0x01 );

	// treatment chromatogram
	kome::objects::DataSet trmtSpectra;
	getSpectra( getTreatmentSample(), trmtSpectra );
	kome::objects::SpectraChromatogram trmtChrom( trmtSpectra );
	trmtChrom.setMzRange( std::max( 0.0, mz - m_mzTol ), mz + m_mzTol );
	trmtChrom.setMz( mz );
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

	int row = ctrlMaxIndex - ctrlMinIndex + 1;
	int col = trmtMaxIndex - trmtMinIndex + 1;

	*rowCnt = row;
	*colCnt = col;

	const double cs = m_windowWidth * (double)ctrlMinIndex;
	const double ts = m_windowWidth * (double)trmtMinIndex;

	*ctrlStart = cs;
	*trmtStart = ts;

	int ctrlNum = row * ( m_pointDensity - 1 ) + 1;
	int trmtNum = col * ( m_pointDensity - 1 ) + 1;

	// create array
	ctrlInts.clear();
	ctrlInts.resize( ctrlNum );
	for( int i = 0; i < ctrlNum; i++ ) {
		ctrlInts[ i ] = 0.0;
	}

	trmtInts.clear();
	trmtInts.resize( trmtNum );
	for( int i = 0; i < trmtNum; i++ ) {
		trmtInts[ i ] = 0.0;
	}

	// intensities
	double idxWidth = m_windowWidth / (double)( m_pointDensity - 1 );

	double startPos = cs - m_windowWidth / 2.0 + idxWidth / 2.0;
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
			ctrlInts[ idx ] = std::max( y, ctrlInts[ idx ] );
		}
		else {
			ctrlInts[ idx ] = y;
			for( int j = prev + 1; j < idx; j++ ) {
				double tmpX = startPos + (double)j * idxWidth;
				double tmpY = ( ( tmpX - prevX ) * y + ( x - tmpX ) * prevY ) / ( x - prevX );

				ctrlInts[ j ] = tmpY;
			}
		}

		// previous
		prev = idx;
		prevX = x;
		prevY = y;
	}

	startPos = ts - m_windowWidth / 2.0 + idxWidth / 2.0;
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
			trmtInts[ idx ] =std::max( y, trmtInts[ idx ] );
		}
		else {
			trmtInts[ idx ] = y;
			for( int j = prev + 1; j < idx; j++ ) {
				double tmpX = startPos + (double)j * idxWidth;
				double tmpY = ( ( tmpX - prevX ) * y + ( x - tmpX ) * prevY ) / ( x - prevX );

				trmtInts[ j ] = tmpY;
			}
		}

		// previous
		prev = idx;
		prevX = x;
		prevY = y;
	}
}

// initialize score matrix
void Ab3dAlignmentMC::initScoreMatrix(
	   std::vector< double >& scoreMatrix,
	   const unsigned int row,
	   const unsigned int col
) {
	// create matrix
	unsigned int num = row * col;
	scoreMatrix.resize( num );

	// set init value
	for( unsigned int i = 0; i < num; i++ ) {
		scoreMatrix[ i ] = -10.0;
	}
}

// execute dynamic programing
kome::numeric::DynamicPrograming* Ab3dAlignmentMC::execDp(
	  std::vector< double >& ctrlInts,
	  std::vector< double >& trmtInts,
	  std::vector< double >& scoreMatrix,
	  const int rowCnt,
	  const int colCnt,
	  const double ctrlStart,
	  const double trmtStart
) {
	// create DP object
	kome::numeric::DynamicPrograming* dp = new kome::numeric::DynamicPrograming( rowCnt, colCnt );
	dp->setRowTitleFunction( boost::bind( &Ab3dAlignmentMC::getRowTitle, this, _1, ctrlStart ) );
	dp->setColTitleFunction( boost::bind( &Ab3dAlignmentMC::getColTitle, this, _1, trmtStart ) );

	// execute
	bool ret = dp->calculate(
		boost::bind(
			&Ab3dAlignmentMC::getScore,
			this,
			_1,
			_2,
			boost::ref( ctrlInts ),
			boost::ref( trmtInts ),
			boost::ref( scoreMatrix ),
			rowCnt,
			colCnt
		),
		m_gapPenalty,
		m_invalidScore
	);

	LOG_DEBUG(
		FMT(
			"AB3D Alignment : DP [row = %d, col = %d, Score = %f]",
			rowCnt,
			colCnt,
			dp->getAccumulatedScore( rowCnt - 1, colCnt - 1 )
		)
	);

	if( !ret ) {
		delete dp;
		dp = NULL;
	}

	return dp;
}

// traceback
void Ab3dAlignmentMC::traceback (
		int row,
		int col,
		kome::core::XYData* xyData,
		kome::core::Point< int >* tbPt,
		const double ctrlStart,
		const double trmtStart
) {
	if( row > tbPt->py && col > tbPt->px ) {
		double x = ctrlStart + (double)col * m_windowWidth;
		double y = trmtStart + (double)row * m_windowWidth;

		xyData->addPoint( x, y );
	}
	tbPt->py = row;
	tbPt->px = col;
}

// get row title
std::string Ab3dAlignmentMC::getRowTitle( const int row, const double ctrlStart ) {
	std::string title;

	if( row < 0 ) {
		title = "Control.";
	}
	else {
		double rt = ctrlStart +  (double)row * m_windowWidth;
		title = FMT( "%.2f", rt );
	}

	return title;
}

// get column title
std::string Ab3dAlignmentMC::getColTitle( const int col, const double trmtStart ) {
	std::string title;

	if( col < 0 ) {
		title = "Treatment.";
	}
	else {
		double rt = trmtStart +  (double)col * m_windowWidth;
		title = FMT( "%.2f", rt );
	}


	return title;
}

// prepare
void Ab3dAlignmentMC::onPrepare(
        kome::objects::Sample* standard,
		kome::objects::Sample* treatment,
		kome::core::Progress* progress
) {
	// log
	LOG_INFO(
		FMT(
			"Prepare AB3D Alignment (MC Fitting) [tol=%.2f, interval=%.2f, baseline=%s, smoothing=%s, width=%.2f, density=%d, gap=%.2f, invalid=%.2f]",
			m_mzTol,
			m_mzInterval,
			( m_baselineItem == NULL ) ? "none" : m_baselineItem->getShortName(),
			( m_smoothingItem == NULL ) ? "none" : m_smoothingItem->getShortName(),
			m_windowWidth,
			m_pointDensity,
			m_gapPenalty,
			m_invalidScore
		)
	);

	// initialize
	initMc();

	progress->fill();
}

// on convert RT
double Ab3dAlignmentMC::onConvertRt( const double rt, const double mz ) {
	// points data
	kome::core::XYData* xyData = getCorrectionPoint( mz ).first;
	if( xyData == NULL ) {
		return rt;
	}

	// return value
	double ret = kome::numeric::Interpolation::linear( *xyData, rt );
	return ret;
}

// on convert m/z
double Ab3dAlignmentMC::onConvertMz( const double rt, const double mz ) {
	return mz;
}

// on invert RT
double Ab3dAlignmentMC::onInvertRt( const double rt, const double mz ) {
	// points data
	kome::core::XYData* xyData = getCorrectionPoint( mz ).second;
	if( xyData == NULL ) {
		return rt;
	}

	// return value
	double ret = kome::numeric::Interpolation::linear( *xyData, rt );
	return ret;
}

// on invert m/z
double Ab3dAlignmentMC::onInvertMz( const double rt, const double mz ) {
	return mz;
}
