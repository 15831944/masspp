/**
 * @file MzXMLHandler.cpp
 * @brief implelements of MzXMLHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzXMLHandler.h"

#include "MzXmlSample.h"
#include "MzXmlSpectrum.h"


using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PARENT_FILE_TAG				"parentFile"
#define SCAN_TAG					   "scan"
#define PRECURSOR_MZ_TAG			   "precursorMz"
#define OFFSET_TAG					 "offset"
#define PEAKS_TAG                    "peaks"

#define FILE_NAME_ATTR				 "fileName"
#define SCAN_NUMBER_ATTR			   "num"
#define PEAKS_COUNT_ATTR              "peaksCount"
#define PRECISION_ATTR                "precision"
#define MS_STAGE_ATTR				  "msLevel"
#define POLARITY_ATTR				  "polarity"
#define RT_ATTR						"retentionTime"
#define CENTROIDED_ATTR				"centroided"
#define LOW_MZ_ATTR					"lowMz"
#define HIGH_MZ_ATTR				   "highMz"
#define BPM_ATTR					   "basePeakMz"
#define BPI_ATTR					   "basePeakIntensity"
#define TIC_ATTR					   "totIonCurrent"
#define PRECURSOR_SCAN_NUM_ATTR		"precursorScanNum"
#define PRECURSOR_INTENSITY_ATTR	   "precursorIntensity"
#define OFFSET_ID_ATTR				 "id"

// >>>>>>	@Date:2013/08/20	<Add>	A.Ozaki
#define	MALDI_TAG					"maldi"
#define	PLATE_ID_ATTR					"plateID"
#define	SPOT_ID_ATTR					"spotID"
// <<<<<<	@Date;2013/08/20	<Add>	A.Ozaki

// constructor
MzXMLHandler::MzXMLHandler( MzXmlSample& file ) : m_file( file ) {
}

// destructor
MzXMLHandler::~MzXMLHandler() {
}

// start document
void MzXMLHandler::onStartDocument() {
	// initialize
	m_scanNumberMap.clear();
	m_currentSpec = NULL;
	m_pos = -1;
}

// end document
void MzXMLHandler::onEndDocument() {
	FILE* fp = m_file.getFile();
	if( fp != NULL ) {
		m_file.closeFile();
	}
}

// start element
void MzXMLHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tag
	if( strcmp( name, PARENT_FILE_TAG ) == 0 ) {	// <parentFile>
		m_file.addParentFile( attrs.getStringValue( FILE_NAME_ATTR, "" ) );
	}
	else if( strcmp( name, SCAN_TAG ) == 0 ) {		// <scan>
		// get scan number
		unsigned int num = attrs.getIntValue( SCAN_NUMBER_ATTR, 0 );

		// get name
		std::string name = FMT( "Scan %d", num );
		std::string title = name;

		// create spectrum
		m_currentSpec = new MzXmlSpectrum( &m_file, name.c_str() );
		m_file.getRootDataGroupNode()->addSpectrum( m_currentSpec );
		m_currentSpec->setScanNumber( num );
		m_scanNumberMap[ num ] = m_currentSpec;

		long long pos = m_pos;
		if( pos > 0 ) {
			m_currentSpec->setOffset( pos );
		}

		// stage
		m_currentSpec->setMsStage( attrs.getIntValue( MS_STAGE_ATTR, 0 ) );

		// polarity
		const char* polarity = attrs.getStringValue( POLARITY_ATTR, "" );
		if( strstr( polarity, "+" ) != NULL ) {
			m_currentSpec->setPolarity( kome::objects::Spectrum::POLARITY_POSITIVE );
		}
		else if( strstr( polarity, "-" ) != NULL ) {
			m_currentSpec->setPolarity( kome::objects::Spectrum::POLARITY_NEGATIVE );
		}

		// RT
		const char* rtStr = attrs.getStringValue( RT_ATTR, "" );
		const char* tPos = strstr( rtStr, "T" );
		if( tPos != NULL ) {
			tPos++;
			double rt = double();
			// Spec No.92266 :ExportÇµÇΩÇçÇöÇwÇlÇkÉtÉ@ÉCÉãÇ…RTÇ™Ç»Ç¢ @date 2013.06.25 <Mod> M.Izumi
			rt = todouble( tPos, -1.0 );
			if( rt >= 0.0 ){
				rt /= 60.0;
				m_currentSpec->setRt( rt );
				m_currentSpec->setHasChromatogram( true );

				title += FMT( " rt=%f", rt );
			}
		}

		// centroided
		bool centroided = attrs.getBoolValue( CENTROIDED_ATTR, false );
		m_currentSpec->setCentroidMode( centroided );

		// low mz
		double lowMz = attrs.getDoubleValue( LOW_MZ_ATTR, -1.0 );
		double highMz = attrs.getDoubleValue( HIGH_MZ_ATTR, -1.0 );
		if( lowMz < highMz ) {
			m_currentSpec->setXRange( lowMz, highMz );
		}

		// base peak m/z
		double bpm = attrs.getDoubleValue( BPM_ATTR, -1.0 );
		if( bpm >= 0.0 ) {
			m_currentSpec->setBasePeakMass( bpm );
		}

		// base peak intensity
		double bpi = attrs.getDoubleValue( BPI_ATTR, -1.0 );
		if( bpi >= 0.0 ) {
			m_currentSpec->setMaxIntensity( bpi );
		}

		// total ion chromatogram
		double tic = attrs.getDoubleValue( TIC_ATTR, -1.0 );
		if( tic >= 0.0 ) {
			m_currentSpec->setTotalIntensity( tic );
		}

		// title
		m_currentSpec->setTitle( title.c_str() );

		// current data
		m_currentData.num = attrs.getIntValue( PEAKS_COUNT_ATTR, -1 );
	}
	else if( strcmp( name, MALDI_TAG ) == 0 ) {		// <maldi>
		// get plate ID
		unsigned int num = attrs.getIntValue( PLATE_ID_ATTR, 0 );

		// get 
		const char	*pcSpotID = attrs.getStringValue( SPOT_ID_ATTR, "" );

		if  ( (char *)NULL != pcSpotID && (MzXmlSpectrum *)NULL != m_currentSpec )
		{
			m_currentSpec->setSpotId( pcSpotID );
		}
	}
	else if( strcmp( name, PRECURSOR_MZ_TAG ) == 0 ) {		// <precursorMz>
		// parent spectrum
		int parent = attrs.getIntValue( PRECURSOR_SCAN_NUM_ATTR, -1 );
		if( m_scanNumberMap.find( parent ) != m_scanNumberMap.end() ) {
			m_currentSpec->setParentSpectrum( m_scanNumberMap[ parent ] );
		}

		// precursor intensity
		double intensity = attrs.getDoubleValue( PRECURSOR_INTENSITY_ATTR, 0.0 );
		m_currentSpec->setPrecursorIntensity( intensity );
	}
	else if( strcmp( name, OFFSET_TAG ) == 0 ) {	// <offset>
		int id = attrs.getIntValue( OFFSET_ID_ATTR, -1 );
		if( m_scanNumberMap.find( id ) != m_scanNumberMap.end() ) {
			m_currentSpec = m_scanNumberMap[ id ];
		}
	}
	else if( strcmp( name, PEAKS_TAG ) == 0 ) {    // <peaks>
		m_currentData.precision = attrs.getIntValue( PRECISION_ATTR, 32 );
	}
}

// end element
void MzXMLHandler::onEndElement( const char* name, const char* text ) {
	// each tag
	if( strcmp( name, SCAN_TAG ) == 0 ) {   // </scan>
		m_currentSpec = NULL;
		m_currentData.num = 0;
		m_currentData.precision = 32;
	}
	else if( strcmp( name, PRECURSOR_MZ_TAG ) == 0 ) {		// </precursorMz>
		m_currentSpec->setPrecursor( todouble( text, 0.0 ) );

		std::string title = m_currentSpec->getTitle();
		title += FMT( " precursor=%f", m_currentSpec->getPrecursor() );
		m_currentSpec->setTitle( title.c_str() );
	}
	else if( strcmp( name, OFFSET_TAG ) == 0 ) {	// </offset>
		if( m_currentSpec != NULL ) {
			long long offset = toint64( text, 10, -1 );
			m_currentSpec->setOffset( offset );
		}
		m_currentSpec = NULL;
	}
	else if( strcmp( name, PEAKS_TAG ) == 0 ) {    // </peaks>
		if( m_currentSpec != NULL ) {
			FILE* fp = m_file.getFile();
			if( fp == NULL ) {
				fp = m_file.openWriteFile();
			}

			long long align = filetell( fp );
			m_currentSpec->setAlign( align );

			fwrite( &( m_currentData.num ), sizeof( m_currentData.num ), 1, fp );

			if( m_currentData.num > 0 ) {
				unsigned int arraySize = m_currentData.num * 2;
				float* arr = new float[ arraySize ];

				if( m_currentData.precision == 64 ) {
					double* tmpArr = new double[ arraySize ];
					kome::core::Base64::decode(
						(char*)text,
						strlen( text ),
						tmpArr,
						arraySize * sizeof( double )
					);

					if( islittleendian() ) {
						for( unsigned int i = 0; i < arraySize; i++ ) {
							memreverse( tmpArr + i, sizeof( double ) );
						}
					}

					for( unsigned int i = 0; i < arraySize; i++ ) {
						arr[ i ] = (float)tmpArr[ i ];
					}
					delete[] tmpArr;
				}
				else {
					kome::core::Base64::decode(
						(char*)text,
						strlen( text ),
						arr,
						arraySize * sizeof( float )
					);

					if( islittleendian() ) {
						for( unsigned int i = 0; i < arraySize; i++ ) {
							memreverse( arr + i, sizeof( float ) );
						}
					}
				}

				fwrite( arr, sizeof( float ), arraySize, fp );
				delete[] arr;
			}
			fflush( fp );
		}	
	}

	m_pos = getPosition();
}
