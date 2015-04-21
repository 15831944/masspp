/**
 * @file DynamicPrograming.cpp
 * @brief implements of DynamicPrograming class
 *
 * @author S.Tanaka
 * @date 2007.08.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DynamicPrograming.h"

#include <list>
#include <utility>
#include <float.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DynamicPrograming::DynamicPrograming( const int row, const int col )
		: m_row( row ), m_col( col ) {
	// initialize
	m_rowTitleFun = getDefaultRowTitle;
	m_colTitleFun = getDefaultColTitle;

	// create array
	if( m_row >= 0 && m_col >= 0 ) {
		m_scoreArray = new ScoreInfo[ ( m_row + 1 ) * ( m_col + 1 ) ];
	}
	else {
		m_scoreArray = NULL;
	}

	// initialize array
	initArray();
}

// destructor
DynamicPrograming::~DynamicPrograming() {
	if( m_scoreArray != NULL ) {
		delete[] m_scoreArray;
	}
}

// set the function to get row title
void DynamicPrograming::setRowTitleFunction( boost::function< std::string ( int ) > rowTitleFun ) {
	m_rowTitleFun = rowTitleFun;
}

// set the function to get column title
void DynamicPrograming::setColTitleFunction( boost::function< std::string ( int ) > colTitleFun ) {
	m_colTitleFun = colTitleFun;
}

// get index
int DynamicPrograming::getIndex( const int row, const int col ) {
	// check parameters
	if( row < -1 || row >= m_row || col < -1 || col >= m_col ) {
		return -1;
	}

	// get index
	int index = ( row + 1 ) * ( m_col + 1 ) + ( col + 1 );

	return index;
}

// initialize array
void DynamicPrograming::initArray() {
	for( int i = -1; i < m_row; i++ ) {
		for( int j = -1; j < m_col; j++ ) {
			// Score Information
			int index = getIndex( i, j );
			ScoreInfo* info = m_scoreArray + index;

			info->acScore = 0.0;
			info->elScore = FLT_MAX;
			info->valid = false;
			info->dir = DIR_NONE;
		}
	}
}

// write log
void DynamicPrograming::writeLog() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// log file name
	std::string id = FMT( "%08x", ( 0xffffffff & (long long)this ) );

	std::string acPrefix = FMT( "dp_ac-%s", id.c_str() );
	std::string acFileName = msppMgr.getTmpFileName( acPrefix.c_str(), ".txt" );
	std::string acFilePath = getpath( msppMgr.getTmpDir(), acFileName.c_str() );

	std::string elPrefix = FMT( "dp_el-%s", id.c_str() );
	std::string elFileName = msppMgr.getTmpFileName( elPrefix.c_str(), ".txt" );
	std::string elFilePath = getpath( msppMgr.getTmpDir(), elFileName.c_str() );

	// open file
	FILE* acFp = fileopen( acFilePath.c_str(), "w" );
	FILE* elFp = fileopen( elFilePath.c_str(), "w" );

	// write header
	for( int j = -1; j < m_col; j++ ) {
		std::string colTitle = m_colTitleFun( j );

		fprintf( acFp, "\t%s", colTitle.c_str() );
		if( j >= 0 ) {
			fprintf( elFp, "\t%s", colTitle.c_str() );
		}
	}
	fprintf( acFp, "\n" );
	fprintf( elFp, "\n" );

	// write matrix
	for( int i = -1; i < m_row; i++ ) {
		// row title
		std::string rowTitle = m_rowTitleFun( i );

		fprintf( acFp, "%s", rowTitle.c_str() );
		if( i >= 0 ) {
			fprintf( elFp, "%s", rowTitle.c_str() );
		}

		// scores
		for( int j = -1; j < m_col; j++ ) {
			// score
			int index = getIndex( i, j );
			ScoreInfo* info = m_scoreArray + index;

			// acumulated score
			fprintf( acFp, "\t" );
			if( info->valid ) {
				fprintf( acFp, "%.3f", info->acScore );

				if( info->dir == DIR_UP ) {
					fprintf( acFp, " (N)" );
				}
				else if( info->dir == DIR_LEFT ) {
					fprintf( acFp, " (W)" );
				}
				else if( info->dir == DIR_UPLEFT ) {
					fprintf( acFp, " (NW)" );
				}
			}
			else {
				fprintf( acFp, "***" );
			}

			// element score
			if( i >= 0 && j >= 0 ) {
				fprintf( elFp, "\t" );
				if( info->elScore < FLT_MAX ) {
					fprintf( elFp, "%.3f", info->elScore );
				}
				else {
					fprintf( elFp, "***" );
				}
			}
		}

		// new line
		fprintf( acFp, "\n" );
		if( i >= 0 ) {
			fprintf( elFp, "\n" );
		}
	}

	// close
	fflush( acFp );
	fclose( acFp );

	fflush( elFp );
	fclose( elFp );
}

// get score
double DynamicPrograming::getAccumulatedScore( const int row, const int col ) {
	// index
	int index = getIndex( row, col );
	if( index < 0 ) {
		return 0.0;
	}

	// score
	return m_scoreArray[ index ].acScore;
}

// get element score
double DynamicPrograming::getElementScore( const int row, const int col ) {
	// index
	int index = getIndex( row, col );
	if( index < 0 || row < 0 || col < 0 ) {
		return 0.0;
	}

	// score
	return m_scoreArray[ index ].elScore;
}

// valid or not
bool DynamicPrograming::isValid( const int row, const int col ) {
	// index
	int index = getIndex( row, col );
	if( index < 0 ) {
		return false;
	}

	// valid
	return m_scoreArray[ index ].valid;
}

// calculate
bool DynamicPrograming::calculate(
		boost::function< double( int, int ) > scoreFun,
		const double gapPenalty,
		const double invalidScore,
		kome::core::Progress* progress
) {
	// check the member
	if( m_scoreArray == NULL ) {
		return false;
	}

	// set progress
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// dynamic programing
	int roopCount = m_row + m_col - 2;

	// set length
	progress->setRange( 0, roopCount + 3 );
	progress->setPosition( 0 );

	for( int i = -2; i <= roopCount && !progress->isStopped(); i++ ) {
		int startJ = std::max( -1, i - m_col + 1 );
		int endJ = std::min( i + 1, m_row - 1 );

		for( int j = startJ; j <= endJ; j++ ) {
			// row, col
			int r = j;
			int c = i - j;

			// get information
			int index = getIndex( r, c );
			ScoreInfo* info = m_scoreArray + index;

			// get score
			if( r == -1 && c == -1 ) {		// initialize value
				info->acScore = 0.0;
				info->elScore = 0.0;
				info->valid = true;
			}
			else {
				// get neighbors
				int index11 = getIndex( r - 1, c - 1 );
				ScoreInfo* info11 = index11 < 0 ? NULL : ( m_scoreArray + index11 );

				int index10 = getIndex( r - 1, c );
				ScoreInfo* info10 = index10 < 0 ? NULL : ( m_scoreArray + index10 );

				int index01 = getIndex( r, c - 1 );
				ScoreInfo* info01 = index01 < 0 ? NULL : ( m_scoreArray + index01 );

				// get score
				double score = 0.0;
				bool valid = false;
				TraceBackDir dir = DIR_NONE;

				if( info11 != NULL && info11->valid ) {
					info->elScore = scoreFun( r, c );
					double tmp = info11->acScore + info->elScore;
					if( !valid || tmp > score ) {
						valid = true;
						score = tmp;
						dir = DIR_UPLEFT;
					}
				}
				if( info10 != NULL && info10->valid ) {
					double tmp = info10->acScore - gapPenalty;
					if( !valid || tmp > score ) {
						valid = true;
						score = tmp;
						dir = DIR_UP;
					}
				}
				if( info01 != NULL && info01->valid ) {
					double tmp = info01->acScore - gapPenalty;
					if( !valid || tmp > score ) {
						valid = true;
						score = tmp;
						dir = DIR_LEFT;
					}
				}

				if( score < invalidScore ) {
					valid = false;
					score = 0.0;
					dir = DIR_NONE;
				}

				// set value
				info->acScore = score;
				info->valid = valid;
				info->dir = dir;
			}
		}

		progress->setPosition( i + 3 );
	}

	// check progress
	if( progress->isStopped() ) {
		return false;
	}

	int index = getIndex( m_row - 1, m_col - 1 );
	return m_scoreArray[ index ].valid;
}

// traceback
void DynamicPrograming::traceback( boost::function< void( int, int ) > pushFun ) {
	// traceback
	int row = m_row - 1;
	int col = m_col - 1;
	int index = getIndex( row, col );
	ScoreInfo* info = index < 0 ? NULL : ( m_scoreArray + index );

	std::list< std::pair< int, int > > posList;

	while( info != NULL ) {
		// store position
		posList.push_front( std::make_pair( row, col ) );

		// traceback
		if( info->dir == DIR_NONE ) {
			info = NULL;
		}
		else {
			if( info->dir == DIR_UPLEFT ) {
				row--;
				col--;
			}
			else if( info->dir == DIR_LEFT ) {
				col--;
			}
			else if( info->dir == DIR_UP ) {
				row--;
			}

			index = getIndex( row, col );
			info = index < 0 ? NULL : ( m_scoreArray + index );
		}
	}

	// push
	for( std::list< std::pair< int, int > >::iterator it = posList.begin(); it != posList.end(); it++ ) {
		pushFun( (*it).first, (*it).second );
	}
}

// get default row title
std::string DynamicPrograming::getDefaultRowTitle( const int row ) {
	std::string title;

	if( row < 0 ) {
		title = "Row.";
	}
	else {
		title = FMT( "Row %d", row );
	}

	return title;
}

// get default column title
std::string DynamicPrograming::getDefaultColTitle( const int col ) {
	std::string title;

	if( col < 0 ) {
		title = "Col.";
	}
	else {
		title = FMT( "Col %d", col );
	}

	return title;
}
