/**
 * @file UTestPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "UTestPluginFunctions.h"              
#include "UTest.h"


using namespace kome::statistic;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




// calculate p-value
kome::objects::Variant calcUTestPValue( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "U-test cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() != 2 ) {
		LOG_ERROR_CODE( FMT( "Impossible to calculate u-test unless the number of groups is two. [Currently: %d group(s)]", groups.size() ), ERR_OTHER );
		return ret;
	}

	// samples
	std::vector< int > samples0;
	std::vector< int > samples1;
	identMgr.getMatrixSamplesFromGroup( samples0, groups[ 0 ] );
	identMgr.getMatrixSamplesFromGroup( samples1, groups[ 1 ] );
	if( samples0.size() < 2 || samples1.size() < 2 ) {
		LOG_ERROR_CODE( FMT( "Impossible to calculate u-test unless two or more samples are specified in each group." ), ERR_OTHER );
		return ret;
	}

	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	if( peaks.size() == 0 ) {
		LOG_ERROR( FMT( "There are no peaks in the matrix." ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	progress->setRange( 0, peaks.size() );
	progress->setPosition( 0 );

	// add column
	int col = identMgr.addMatrixColumn( "p value (u-Test)", kome::ident::TYPE_UDOUBLE, matrix );
	identMgr.deleteAppendedValueFromMatrixColumn( col );

	// t-test
	for( unsigned int i = 0; i < peaks.size() && !progress->isStopped(); i++ ) {
		const int peakId = peaks[ i ];

		std::vector< double > vals0;
		std::vector< double > vals1;

		for( unsigned int j = 0; j < samples0.size(); j++ ) {
			const double val = identMgr.getNormalizedPeakValue( peakId, samples0[ j ] );
			if( val >= 0.0 ) {
				vals0.push_back( val );
			}
		}

		for( unsigned int j = 0; j < samples1.size(); j++ ) {
			const double val = identMgr.getNormalizedPeakValue( peakId, samples1[ j ] );
			if( val >= 0.0 ) {
				vals1.push_back( val );
			}
		}

		if( vals0.size() >= 2 && vals1.size() >= 2 ) {
			double p = UTest::test( &( vals0[ 0 ] ), vals0.size(), &( vals1[ 0 ] ), vals1.size() );
			if( p >= 0.0 ) {
				identMgr.setAppendedValue( peaks[ i ], col, FMT( "%f", p ).c_str() );
			}
		}

		progress->setPosition( i + 1 );
	}

	if( !progress->isStopped() ) {
		progress->fill();
	}

	ret.prim.boolVal = true;
	return ret;
}
