/**
 * @file CommonsMathPluginFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author satstnka
 * @date 2012.02.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CommonsMathPluginFunctions.h"

#include "CommonsMathManager.h"


using namespace kome::math;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// ttest
kome::objects::Variant calcTTestPValue( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	CommonsMathManager& mgr = CommonsMathManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "T-test cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() != 2 ) {
		LOG_ERROR_CODE( FMT( "Impossible to calculate t-test unless the number of groups is two. [Currently: %d group(s)]", groups.size() ), ERR_OTHER );
		return ret;
	}

	// samples
	std::vector< int > samples0;
	std::vector< int > samples1;
	identMgr.getMatrixSamplesFromGroup( samples0, groups[ 0 ] );
	identMgr.getMatrixSamplesFromGroup( samples1, groups[ 1 ] );
	if( samples0.size() < 2 || samples1.size() < 2 ) {
		LOG_ERROR_CODE( FMT( "Impossible to calculate t-test unless two or more samples are specified in each group." ), ERR_OTHER );
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
	int col = identMgr.addMatrixColumn( "p value (t-Test)", kome::ident::TYPE_UDOUBLE, matrix );
	identMgr.deleteAppendedValueFromMatrixColumn( col );

	// t-test
	for( unsigned int i = 0; i < peaks.size() && !progress->isStopped(); i++ ) {
		std::vector< double > vals0;
		std::vector< double > vals1;

		for( unsigned int j = 0; j < samples0.size(); j++ ) {
			const double val = identMgr.getNormalizedPeakValue( peaks[ i ], samples0[ j ] );
			if( val >= 0.0 ) {
				vals0.push_back( val );
			}
		}

		for( unsigned int j = 0; j < samples1.size(); j++ ) {
			const double val = identMgr.getNormalizedPeakValue( peaks[ i ], samples1[ j ] );
			if( val >= 0.0 ) {
				vals1.push_back( val );
			}
		}

		if( vals0.size() >= 2 && vals1.size() >= 2 ) {
			double p = mgr.tTest( &( vals0[ 0 ] ), vals0.size(), &( vals1[ 0 ] ), vals1.size() );
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

// anova
kome::objects::Variant calcAnovaValue( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	CommonsMathManager& mgr = CommonsMathManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "ANOVA cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() < 2 ) {
		LOG_ERROR_CODE( FMT( "Impossible to calculate ANOVA for less than two groups. [Currently: %d group(s)]", groups.size() ), ERR_OTHER );
		return ret;
	}

	// samples
	std::vector< std::vector< int > > samples;
	samples.resize( groups.size() );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples[ i ], groups[ i ] );
		if( samples[ i ].size() < 2 ) {
			LOG_ERROR_CODE( FMT( "Impossible to calculate ANOVA if less than two samples are specified in each group." ), ERR_OTHER );
			return ret;
		}
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
	int col = identMgr.addMatrixColumn( "p value (ANOVA)", kome::ident::TYPE_UDOUBLE, matrix );
	identMgr.deleteAppendedValueFromMatrixColumn( col );

	// t-test
	for( unsigned int i = 0; i < peaks.size() && !progress->isStopped(); i++ ) {
		std::vector< double* > vals;
		vals.resize( groups.size() );

		std::vector< int > sizes;
		sizes.resize( groups.size() );

		std::vector< std::vector< double > > valueArray;
		valueArray.resize( groups.size() );

		bool flg = true;
		for( unsigned int j = 0; j < samples.size() && flg; j++ ) {
			for( unsigned int k = 0; k < samples[ j ].size(); k++ ) {
				double val = identMgr.getNormalizedPeakValue( peaks[ i ], samples[ j ].at( k ) );
				if( val >= 0.0 ) {
					valueArray[ j ].push_back( val );
				}
			}
			if( valueArray[ j ].size() < 2 ) {
				flg = false;
			}
		}

		// anova
		if( flg ) {
			for( unsigned int j = 0; j < valueArray.size(); j++ ) {
				vals[ j ] = &( valueArray[ j ][ 0 ] );
				sizes[ j ] = valueArray[ j ].size();
			}

			double p = mgr.anova( &( vals[ 0 ] ), &( sizes[ 0 ] ), vals.size() );
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
