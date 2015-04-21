/**
 * @file RangeList.cpp
 * @brief implements of RangeList class
 *
 * @author S.Tanaka
 * @date 2009.01.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "RangeList.h"

#include <float.h>
#include <limits.h>
#include <boost/bind.hpp>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
RangeList::RangeList( const DefaultType type ) : m_type( type ) {
}

// destructor
RangeList::~RangeList() {
}

// get default type
RangeList::DefaultType RangeList::getType() {
	return m_type;
}

// add int range
void RangeList::addIntRange( const int start, const int end ) {
	m_ranges.resize( m_ranges.size() + 1 );
	m_ranges.back().first.intVal = start;
	m_ranges.back().second.intVal = end;
}

// add double range
void RangeList::addDoubleRange( const double start, const double end ) {
	m_ranges.resize( m_ranges.size() + 1 );
	m_ranges.back().first.dblVal = start;
	m_ranges.back().second.dblVal = end;
}

// clear ranges
void RangeList::clearRanges() {
	m_ranges.clear();
}

// get the number of ranges
unsigned int RangeList::getNumberOfRanges() {
	return m_ranges.size();
}

// get int range start
int RangeList::getStartInt( const unsigned int idx ) {
	if( idx >= m_ranges.size() ) {
		return INT_MAX;
	}
	return m_ranges[ idx ].first.intVal;
}

// get int range end
int RangeList::getEndInt( const unsigned int idx ) {
	if( idx >= m_ranges.size() ) {
		return INT_MIN;
	}
	return m_ranges[ idx ].second.intVal;
}
// get double range start
double RangeList::getStartDouble( const unsigned int idx ) {
	if( idx >= m_ranges.size() ) {
		return FLT_MAX;
	}
	return m_ranges[ idx ].first.dblVal;
}

// get double range end
double RangeList::getEndDouble( const unsigned int idx ) {
	if( idx >= m_ranges.size() ) {
		return - FLT_MAX;
	}
	return m_ranges[ idx ].second.dblVal;
}

// check int vlaue
bool RangeList::checkIntValue( const int val ) {
	// empty
	if( m_ranges.size() == 0 ) {
		return ( m_type == DEFAULT_ALL );
	}

	// check
	for( unsigned int i = 0; i < m_ranges.size(); i++ ) {
		if( val >= m_ranges[ i ].first.intVal
				&& val <= m_ranges[ i ].second.intVal ) {	// included
			return true;
		}
	}
	return false;
}

// check double value
bool RangeList::checkDoubleValue( const double val ) {
	// empty
	if( m_ranges.size() == 0 ) {
		return ( m_type == DEFAULT_ALL );
	}

	// check
	for( unsigned int i = 0; i < m_ranges.size(); i++ ) {
		if( val >= m_ranges[ i ].first.dblVal
				&& val <= m_ranges[ i ].second.dblVal ) {	// included
			return true;
		}
	}
	return false;
}

// import integer ranges
void RangeList::importIntRanges( const char* str ) {
	// clear 
	clearRanges();

	// token by ","
	std::vector< std::string > tokens;

	stringseparate(str,",",tokens);

	for( unsigned int i = 0; i < tokens.size(); i++ ) {
		// token by ":"
		std::vector< std::string > range;

		stringseparate(tokens[ i ].c_str(),":",range);

		if( range.size() == 1 ) {	// single value
			std::string val = trimstring( range[ 0 ].c_str() );
			int v = int();
			if( isint( val.c_str(), 10, &v ) ) {
				m_ranges.resize( m_ranges.size() + 1 );
				m_ranges.back().first.intVal = v;
				m_ranges.back().second.intVal = v;
			}
		}
		else if( range.size() > 1 ) {	// range value
			std::string val0 = trimstring( range[ 0 ].c_str() );
			std::string val1 = trimstring( range[ 1 ].c_str() );

			int v0 = int();
			int v1 = int();

			bool ret0 = isint( val0.c_str(), 10, &v0 );
			bool ret1 = isint( val1.c_str(), 10, &v1 );

			if( ret0 || ret1 ) {
				m_ranges.resize( m_ranges.size() + 1 );

				m_ranges.back().first.intVal = ( ret0 ? v0 : INT_MIN );
				m_ranges.back().second.intVal = ( ret1 ? v1 : INT_MAX );
			}
		}
	}
}

// import double ranges
void RangeList::importDoubleRanges( const char* str ) {
	// clear
	clearRanges();

	// token by ","
	std::vector< std::string > tokens;

	stringseparate(str, ",", tokens);

	for( unsigned int i = 0; i < tokens.size(); i++ ) {
		// token by ":"
		std::vector< std::string > range;

		stringseparate(tokens[ i ].c_str(),":",range);

		if( range.size() == 1 ) {	// single value
			std::string val = trimstring( range[ 0 ].c_str() );
			double v = double();
			if( isdouble( val.c_str(), &v ) ) {
				m_ranges.resize( m_ranges.size() + 1 );
				m_ranges.back().first.dblVal = v;
				m_ranges.back().second.dblVal = v;
			}
		}
		else if( range.size() > 1 ) {	// range value
			std::string val0 = trimstring( range[ 0 ].c_str() );
			std::string val1 = trimstring( range[ 1 ].c_str() );

			double v0 = double();
			double v1 = double();

			bool ret0 = isdouble( val0.c_str(), &v0 );
			bool ret1 = isdouble( val1.c_str(), &v1 );

			if( ret0 || ret1 ) {
				m_ranges.resize( m_ranges.size() + 1 );

				m_ranges.back().first.dblVal = ( ret0 ? v0 : - FLT_MAX );
				m_ranges.back().second.dblVal = ( ret1 ? v1 : FLT_MAX );
			}
		}
	}
}

// export integer ranges
std::string RangeList::exportIntRanges() {
	// string
	std::string str;

	// create character string
	for( unsigned int i = 0; i < m_ranges.size(); i++ ) {
		// create range string
		std::string range;

		int v0 = m_ranges[ i ].first.intVal;
		int v1 = m_ranges[ i ].second.intVal;

		if( v0 == v1 ) {
			range = FMT( "%d", v0 );
		}
		else {
			if( v0 != INT_MIN ) {
				range = FMT( "%d", v0 );
			}

			range.append( ":" );

			if( v1 != INT_MAX ) {
				range = FMT( "%d", v1 );
			}
		}

		// append
		if( !str.empty() ) {
			str.append( "," );
		}
		str.append( range );
	}

	return str;
}

// export double ranges
std::string RangeList::exportDoubleRanges() {
	// string
	std::string str;

	// create character string
	for( unsigned int i = 0; i < m_ranges.size(); i++ ) {
		// create range string
		std::string range;

		double v0 = m_ranges[ i ].first.dblVal;
		double v1 = m_ranges[ i ].second.dblVal;

		if( v0 == v1 ) {
			range = FMT( "%f", v0 );
		}
		else {
			if( v0 != - FLT_MAX ) {
				range = FMT( "%f", v0 );
			}

			range.append( ":" );

			if( v1 != FLT_MAX ) {
				range = FMT( "%f", v1 );
			}
		}

		// append
		if( !str.empty() ) {
			str.append( "," );
		}
		str.append( range );
	}

	return str;
}
