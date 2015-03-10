/**
 * @file TextSample.cpp
 * @brief implements of TextSample class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TextSample.h"

#include "TextSampleSet.h"
#include "TextSpectrum.h"


using namespace kome::io::text;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TextSample::TextSample( TextSampleSet* sampleSet ) : kome::objects::Sample( sampleSet ) {
	m_sampleSet = sampleSet;
}

// destructor
TextSample::~TextSample() {
}

// get text sample set
TextSampleSet* TextSample::getTextSampleSet() {
	return m_sampleSet;
}

// on open sample
bool TextSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// get file
	FILE* fp = ( m_sampleSet == NULL ? NULL : m_sampleSet->getFile() );
	if( fp == NULL ) {
		// @date 2011.03.17 <Mod> M.Izumi ->
		const char* path = m_sampleSet->getFilePath();
		if( m_sampleSet->openFile(path) ){
			fp = m_sampleSet->getFile();
			if( fp == NULL ){
				LOG_ERROR_CODE( FMT( "Failed to open the sample. [%s]", path ), ERR_FILE_OPEN_FAILED );
				return false;
			}
		}
		// <-
	}

	// read
	char line[ 2048 ];
	TextSpectrum* spec = NULL;
	long long pos = 0;
	int cnt = 0;
	double rt = -1.0;
	double bpm = -1.0;
	double bpi = 0.0;
	double tic = 0.0;

	while( fgets( line, 2048, fp ) != NULL ) {
		// part of the way
		std::string l = trimstring( line );

		// x, y
		bool blank = ( l.empty() || l[ 0 ] == '#' );
		if( blank ) {	// comment line
			// try to get RT
			std::string rtStr = touppercase( l.c_str() );
			unsigned int rtPos = rtStr.find( "RT" );
			if( rtPos != (unsigned int)rtStr.npos ) {
				rtStr = rtStr.substr( rtPos + 2 );
				while( rtStr.length() > 0 
						&& ( rtStr[ 0 ] == ' ' || rtStr[ 0 ] == '=' || rtStr[ 0 ] == ':' ) ) {
					rtStr = rtStr.substr( 1 );
				}

				rt = todouble( rtStr.c_str(), -1.0 );
			}
		}
		else {	// data line
			double x = double();
			double y = double();

			if( sscanf( l.c_str(), "%lf %lf", &x, &y ) >= 2 ) {
				if( spec == NULL ) {	// new spectrum
					std::string name = FMT( "scan %d", cnt ); // @date 2011.10.11 <Mod> M.Izumi

					spec = new TextSpectrum( this, name.c_str() );
					spec->setHasChromatogram( rt >= 0.0 );
					spec->setDataPos( pos );
					
					spec->setTitle( name.c_str() );	// @date 2011.10.07 <Add> M.Izumi

					if( rt >= 0.0 ) {
						spec->setRt( rt );
						rt = -1.0;
					}
					cnt++;

					rootGroup->addSpectrum( spec );
				}

				spec->addPoint( x, y );
				if( y > bpi ) {
					bpi = y;
					bpm = x;
				}
				tic += y;
			}
		}

		// blank
		if( blank ) {
			if( spec != NULL ) {
				spec->setBasePeakMass( bpm );
				spec->setMaxIntensity( bpi );
				spec->setTotalIntensity( tic );
				bpm = -1.0;
				bpi = 0.0;
				tic = 0.0;
			}
			spec = NULL;
			pos = filetell( fp );
		}			
	}

	if( spec != NULL ) {
		spec->setBasePeakMass( bpm );
		spec->setMaxIntensity( bpi );
		spec->setTotalIntensity( tic );
	}

	if( cnt <= 0 ) {
		LOG_ERROR_CODE(			
			FMT(
				"There is no data in the file. [%s]",
				m_sampleSet->getFilePath()
			),
			ERR_NULL_POINTER
		);
	}
	
	m_sampleSet->closeFile();	// Spec No.93991 @date 2013.08.29 <Add> M.Izumi

	return ( cnt > 0 );
}

// on close sample
bool TextSample::onCloseSample() {
	return true;
}
