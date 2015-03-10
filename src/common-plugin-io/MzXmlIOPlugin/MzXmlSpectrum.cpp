/**
 * @sample MzXmlSpectrum.cpp
 * @brief implements of MzXmlSpectrum class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "MzXmlSpectrum.h"

#include "MzXmlSample.h"
#include "MzXmlSampleSet.h"
#include "MzXMLSpectrumDataHandler.h"

#include <list>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <xercesc/framework/MemBufInputSource.hpp>

using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzXmlSpectrum::MzXmlSpectrum( MzXmlSample* file, const char* name )
		: kome::objects::Spectrum( file, name ) {
	m_offset = -1;
}

// destructor
MzXmlSpectrum::~MzXmlSpectrum() {
}

// set align
void MzXmlSpectrum::setAlign( const long long align ) {
	m_align = align;
}

// get align
long long MzXmlSpectrum::getAlign() {
	return m_align;
}

// set data offset
void MzXmlSpectrum::setOffset( const long long offset ) {
	m_offset = offset;
}

// get data offset
long long MzXmlSpectrum::getOffset() {
	return m_offset;
}

// parse
void MzXmlSpectrum::parse( xercesc::SAX2XMLReader* parser ) {
	// get file
	MzXmlSample* sample = (MzXmlSample*)getSample();
	MzXmlSampleSet* sampleSet = sample->getMzXmlSampleSet();

	// parse
	if( m_offset > 0 ) {
		// seek
		FILE* fp = sampleSet->getFile();
		fileseek( fp, m_offset, SEEK_SET );

		// read file
		std::string tag;
		char buff[ 1024 ];
		bool loop = true;
		bool endTag = false;
		while( loop ) {
			// read
			int readSize = fread( buff, 1, 1023, fp );

			// add to tag
			if( readSize > 0 ) {
				buff[ readSize ] = '\0';
				tag.append( buff );
			}

			// check the tag
			std::string::size_type pos = tag.find( "</peaks" );
			if( pos != tag.npos ) {
				pos = tag.find( ">", pos );
				if( pos != tag.npos ) {
					tag = tag.substr( 0, pos + 1 );
					loop = false;
					endTag = true;
				}
			}

			// check the size
			if( readSize < 1023 ) {
				loop = false;
			}
		}

		std::string::size_type startPos = tag.find( "<scan" );
		int startCount = 0;
		while( startPos != tag.npos ) {
			startCount++;
			startPos = tag.find( "<scan", startPos + 1 );
		}
		for( int i = 0; i < startCount; i++ ) {
			tag.append( "</scan>" );
		}

		// parse
		if( endTag ) {
			xercesc::MemBufInputSource source( (const XMLByte*)tag.c_str(), tag.size(), "memory_buffer", false );
			parser->parse( source );
		}
		else {
			LOG_WARN( FMT( "Failed to get the scan tag." ) );
		}
	}
	else {
		// parse file
		parser->parse( sampleSet->getFilePath() );
	}
}

// get data points
void MzXmlSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// file
	MzXmlSample* sample = (MzXmlSample*)getSample();
	FILE* fp = sample->getFile();
	if( fp == NULL ) {
		fp = sample->openReadFile();
	}

	// read
	fileseek( fp, m_align, SEEK_SET );
	int num = 0;
	fread( &num, sizeof( num ), 1, fp );
	
	if( num > 0 ) {
		int arraySize = num * 2;
		float* arr = new float[ arraySize ];
		fread( arr, sizeof( float ), arraySize, fp );
		
		// points
		for( int i = 0; i < num; i++ ) {
			double x = (double)arr[ 2 * i ];
			double y = (double)arr[ 2 * i + 1 ];
			
			if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
				xyData->addPoint( x, y );
			}
		}
	}
}

// get x range
void MzXmlSpectrum::onGetXRange( double* minX, double* maxX ) {
	// data points
	kome::core::DataPoints dps;
	onGetXYData( &dps, -1.0, -1.0 );

	// min x
	if( minX != NULL ) {
		*minX = ( dps.getLength() == 0 ? 0.0 : dps.getMinX() );
	}
	if( maxX != NULL ) {
		*maxX = ( dps.getLength() == 0 ? 0.0 : dps.getMaxX() );
	}
}

// get total intensity
double MzXmlSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints pt;
	onGetXYData( &pt, minX, maxX );

	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < pt.getLength(); i++ ) {
		intensity += pt.getY( i );
	}

	return intensity;
}

// get total intensity
double MzXmlSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints pt;
	onGetXYData( &pt, minX, maxX );

	// get main intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < pt.getLength(); i++ ) {
		double y = pt.getY( i );
		intensity = MAX( y, intensity );
	}

	return intensity;
}

void	MzXmlSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	MzXmlSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	MzXmlSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	MzXmlSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	MzXmlSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	MzXmlSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	MzXmlSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}
