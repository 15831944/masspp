/**
 * @sample MzmlSpectrum.cpp
 * @brief implements of MzmlSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlSpectrum.h"

#include "MzmlSample.h"
#include "MzmlSampleSet.h"
#include "MzmlDataHandler.h"

#include <list>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzmlSpectrum::MzmlSpectrum( MzmlSample* file, const char* name )
		: kome::objects::Spectrum( file, name ) {
	m_mzmlSample = file;
	m_offset = -1;
}

// destructor
MzmlSpectrum::~MzmlSpectrum() {
}

// set data offset
void MzmlSpectrum::setOffset( const long long offset ) {
	m_offset = offset;
}

// get data offset
long long MzmlSpectrum::getOffset() {
	return m_offset;
}

// parse
void MzmlSpectrum::parse( xercesc::SAX2XMLReader* parser ) {
	// get file
	MzmlSample* sample = (MzmlSample*)getSample();
	MzmlSampleSet* sampleSet = sample->getMzmlSampleSet();

	// parse
	if( m_offset > 0 ) {
		// seek
		FILE* fp = sampleSet->getFile();
		fileseek( fp, m_offset, SEEK_SET );

		// read file
		std::string tag;
// >>>>>>	@Date:2013/06/03	<Modify>	A.Ozaki
// 64bit環境で動作させる場合、ポインタアドレスを正しく処理することができなくなります
// x86/x64ともに動作させるために、size_tで処理することが勧められているのでいるので、
// size_tで処理するように修正します
		size_t	szPos = 0;
// <<<<<<	@Date:2013/06/03	<Modify>	A.Ozaki
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
// >>>>>>	@Date:2013/06/03	<Modify>	A.Ozaki
			szPos = (size_t)tag.find( "</spectrum>" );
			if( szPos != (size_t)tag.npos ) {
				endTag = true;
				loop = false;

				tag = tag.substr( 0, szPos );
				tag.append( "</spectrum>" );
			}

			// check the size
			if( readSize < 1023 ) {
				loop = false;
			}
		}

		szPos = (size_t)tag.find( "<spectrum" );
		if( szPos != (size_t)tag.npos ) {
			tag = tag.substr( szPos );
		}
// <<<<<<	@Date:2013/06/03	<Modify>	A.Ozaki

		// parse
		if( endTag ) {
			xercesc::MemBufInputSource source( (const XMLByte*)tag.c_str(), tag.size(), "memory_buffer", false );
			parser->parse( source );
		}
		else {
			LOG_WARN( FMT( "Failed to get the spectrum tag." ) );
		}
	}
	else {
		// parse file
		parser->parse( sampleSet->getFilePath() );
	}
}

// get data points
void MzmlSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
#pragma omp critical ( getMzmlSpectrumData )
	{
		// data points
		kome::core::DataPoints pt;

		// parser
		xercesc::SAX2XMLReader* parser = kome::xml::XercesTool::getParser( NULL );
		parser->setFeature( xercesc::XMLUni::fgXercesSchema, false );
		parser->setFeature( xercesc::XMLUni::fgXercesSchemaFullChecking, false );

		// create handler
		MzmlDataHandler handler( *m_mzmlSample, getName(), pt );

		parser->setContentHandler( &handler );
		parser->setErrorHandler( &handler );

		// parse
		try {
			parse( parser );
		}
		catch( const xercesc::XMLException& e ) {
			LOG_ERROR( FMT( "XML Exception: %s", kome::xml::XercesTool::transcode( e.getMessage() ).c_str() ) );
			handler.setError( true );
		}
		catch( const xercesc::SAXParseException& e ) {
			LOG_ERROR( FMT( "SAX Parse Exception: %s", kome::xml::XercesTool::transcode( e.getMessage() ).c_str() ) );
			handler.setError( true );
		}
		catch( ... ) {
			LOG_ERROR( FMT( "Unexpected Exception" ) );
			handler.setError( true );
		}

		// get data
		int startIdx = 0;
		if( minX >= 0.0 ) {
			startIdx = pt.searchIndex( minX );
			if( startIdx < 0 ) {
				startIdx = - startIdx - 1;
			}
		}

		int endIdx = (int)pt.getLength() - 1;
		if( maxX >= 0.0 ) {
			endIdx = pt.searchIndex( maxX );
			if( endIdx < 0 ) {
				endIdx = - endIdx - 2;
			}
		}

		int num = endIdx - startIdx + 1;
		if( num > 0 ) {
			xyData->reserve( num );
			for( int i = startIdx; i <= endIdx; i++ ) {
				xyData->addPoint( pt.getX( i ), pt.getY( i ) );
			}
		}
	}
}

// get x range
void MzmlSpectrum::onGetXRange( double* minX, double* maxX ) {
	// get
	kome::core::DataPoints dps;
	onGetXYData( &dps, -1.0, -1.0 );

	// set
	if( minX != NULL ) {
		*minX = dps.getMinX();
	}
	if( maxX != NULL ) {
		*maxX = dps.getMaxX();
	}
}

// get total intensity
double MzmlSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
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
double MzmlSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
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

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	MzmlSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	MzmlSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	MzmlSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	MzmlSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	MzmlSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	MzmlSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	MzmlSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

