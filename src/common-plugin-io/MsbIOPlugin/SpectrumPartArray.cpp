/**
 * @file SpectrumPartArray.cpp
 * @brief implements of SpectrumPartArray class
 *
 * @author S.Tanaka
 * @date 2008.07.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumPartArray.h"

#include <boost/bind.hpp>


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectrumPartArray::SpectrumPartArray() {
	m_spec = NULL;
}

// destructor
SpectrumPartArray::~SpectrumPartArray() {
}

// set spectrum
void SpectrumPartArray::setSpectrum( kome::objects::Spectrum* spec ) {
	m_spec = spec;
}

// get spectrum
kome::objects::Spectrum* SpectrumPartArray::getSpectrum() {
	return m_spec;
}

// add part
SpectrumPart& SpectrumPartArray::addPart() {
	m_parts.resize( m_parts.size() + 1 );

	return m_parts.back();
}

// get the number of spectrum parts
unsigned int SpectrumPartArray::getNumberOfParts() {
	return m_parts.size();
}

// get part
SpectrumPart* SpectrumPartArray::getPart( const unsigned int index ) {
	if( index >= m_parts.size() ) {
		return NULL;
	}

	return &( m_parts[ index ] );
}

// get spectrum part index
int SpectrumPartArray::getPartIndex( const double mz ) {
	int index = kome::core::SearchTool::binarySearch< double, SpectrumPart* >(
		mz,
		boost::bind( &SpectrumPartArray::getPart, this, _1 ),
		comparePart,
		m_parts.size()
	);
	return index;	
}

// on load data
bool SpectrumPartArray::onLoadData( boost::function< int ( void*, int ) > readFun ) {
	// index
	int idx = (int)m_parts.size();

	// size
	unsigned int num = 0;
	readFun( &num, sizeof( num ) );
	if( num == 0 ) {
		return true;
	}
	m_parts.resize( idx + num );

	int elmSize = 6 * sizeof( float ) + 3 * sizeof( unsigned int );
	int size = elmSize * num;
	unsigned char* buff = new unsigned char[ size ];

	// read
	if( readFun( buff, size ) < size ) {
		return false;
	}

	// parts
	for( unsigned int i = 0; i < num; i++ ) {
		SpectrumPart& part = m_parts[ idx + i ];

		unsigned char* line = buff + ( i * elmSize );

		float mz = *( (float*)line );
		line += sizeof( float );

		float minMz = *( (float*)line );
		line += sizeof( float );

		float maxMz = *( ( float*)line );
		line += sizeof( float );

		float totInt = *( (float*)line );
		line += sizeof( float );

		float minInt = *( (float*)line );
		line += sizeof( float );

		float maxInt = *( (float*)line );
		line += sizeof( float );

		unsigned int len = *( (unsigned int*)line );
		line += sizeof( unsigned int );

		unsigned int msAli = *( (unsigned int*)line );
		line += sizeof( unsigned int );

		unsigned int intAli = *( (unsigned int*)line );

		part.setMz( mz );
		part.setMinMz( minMz );
		part.setMaxMz( maxMz );
		part.setTotalIntensity( totInt );
		part.setMinIntensity( minInt );
		part.setMaxIntensity( maxInt );
		part.setLength( len );
		part.setMsAlign( msAli );
		part.setIntAlign( intAli );
	}

	delete[] buff;

	return true;
}

// on write data
bool SpectrumPartArray::onSaveData( boost::function< int ( void*, int ) > writeFun ) {
	// size
	unsigned int num = m_parts.size();
	writeFun( &num, sizeof( num ) );
	if( num == 0 ) {
		return true;
	}

	int elmSize = 6 * sizeof( float ) + 3 * sizeof( unsigned int );
	int size = elmSize * num;
	unsigned char* buff = new unsigned char[ size ];

	// write to buffer
	for( unsigned int i = 0; i < num; i++ ) {
		SpectrumPart& part = m_parts[ i ];

		unsigned char* line = buff + ( i * elmSize );

		float* mz = (float*)line;
		*mz = part.getMz();
		line += sizeof( float );

		float* minMz = (float*)line;
		*minMz = part.getMinMz();
		line += sizeof( float );

		float* maxMz = (float*)line;
		*maxMz = part.getMaxMz();
		line += sizeof( float );

		float* totInt = (float*)line;
		*totInt = part.getTotalIntensity();
		line += sizeof( float );

		float* minInt = (float*)line;
		*minInt = part.getMinIntensity();
		line += sizeof( float );

		float* maxInt = (float*)line;
		*maxInt = part.getMaxIntensity();
		line += sizeof( float );

		unsigned int* len = (unsigned int*)line;
		*len = part.getLength();
		line += sizeof( unsigned int );

		unsigned int* msAli = (unsigned int*)line;
		*msAli = part.getMsAlign();
		line += sizeof( unsigned int );

		unsigned int* intAli = (unsigned int*)line;
		*intAli = part.getIntAlign();
	}

	// write
	bool ret = ( writeFun( buff, size ) == size );

	delete[] buff;

	return ret;
}

// on clear points
void SpectrumPartArray::onClearPoints() {
	m_parts.clear();
}

// on add point
void SpectrumPartArray::onAddPoint( const double x, const double y ) {
	LOG_WARN( FMT( "SpectrumPartArray::addPoint is not supported."  ) );
}

// on insert point
void SpectrumPartArray::onInsertPoint( const unsigned int index, const double x, const double y ) {
	LOG_WARN( FMT( "SpectrumPartArray::insertPoint is unsupported." ) );
}

// on delete point
void SpectrumPartArray::onDeletePoint( const unsigned int index ) {
	if( index >= m_parts.size() ) {
		return;
	}
	m_parts.erase( m_parts.begin() + index );
}

// on get length
unsigned int SpectrumPartArray::onGetLength() {
	return m_parts.size();
}

// on get x
double SpectrumPartArray::onGetX( const unsigned int index ) {
	return m_parts[ index ].getMz();
}

// on get y
double SpectrumPartArray::onGetY( const unsigned int index ) {
	return m_parts[ index ].getMaxIntensity();
}

// on reserve
void SpectrumPartArray::onReserve( const unsigned int num ) {
	LOG_WARN( FMT( "SpectrumPartArray::reserve is unsupported." ) );
}

// compare for binary search
int SpectrumPartArray::comparePart( SpectrumPart* part, double& x ) {
	if( (double)part->getMaxMz() < x ) {
		return -1;
	}
	if( (double)part->getMinMz() > x ) {
		return 1;
	}
	return 0;
}
