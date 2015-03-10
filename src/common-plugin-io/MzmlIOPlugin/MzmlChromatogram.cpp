/**
 * @file MzmlChromatogram.cpp
 * @brief interfaces of MzmlChromatogram class
 *
 * @author S.Tanaka
 * @date 2009.04.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlChromatogram.h"
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
MzmlChromatogram::MzmlChromatogram( MzmlSample* file )
		: kome::objects::Chromatogram( file ) {
	m_offset = -1;
	m_mzmlSample = file;
}

// destructor
MzmlChromatogram::~MzmlChromatogram() {
}

// set offset
void MzmlChromatogram::setOffset( const long long offset ) {
	m_offset = offset;
}

// get offset
long long MzmlChromatogram::getOffset() {
	return m_offset;
}

// parse XML data
void MzmlChromatogram::parse( xercesc::SAX2XMLReader* parser ) {
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
			unsigned int pos = tag.find( "</chromatogram>" );
			if( pos != (unsigned int)tag.npos ) {
				endTag = true;
				loop = false;

				tag = tag.substr( 0, pos );
				tag.append( "</chromatogram>" );
			}

			// check the size
			if( readSize < 1023 ) {
				loop = false;
			}
		}

		unsigned int pos = tag.find( "<chromatogram" );
		if( pos != (unsigned int)tag.npos ) {
			tag = tag.substr( pos );
		}

		// parse
		if( endTag ) {
			xercesc::MemBufInputSource source( (const XMLByte*)tag.c_str(), tag.size(), "memory_buffer", false );
			parser->parse( source );
		}
		else {
			LOG_WARN( FMT( "Failed to get the chromatogram tag." ) );
		}
	}
	else {
		// parse file
		parser->parse( sampleSet->getFilePath() );
	}
}

// on get xy data
void MzmlChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check the parameter
	if( xyData == NULL ) {
		return;
	}

	// data points
	kome::core::DataPoints dps;

	// parser
	xercesc::SAX2XMLReader* parser = kome::xml::XercesTool::getParser( NULL );
	parser->setFeature( xercesc::XMLUni::fgXercesSchema, false );
	parser->setFeature( xercesc::XMLUni::fgXercesSchemaFullChecking, false );

	// create handler
	MzmlDataHandler handler( *m_mzmlSample, getName(), dps );

	parser->setContentHandler( &handler );
	parser->setErrorHandler( &handler );

	// parse
	try {
		parse( parser );
		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			xyData->addPoint( dps.getX( i ), dps.getY( i ) );
		}
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
}

// on get spectra
void MzmlChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double MzmlChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get ms stage
int MzmlChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double MzmlChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
