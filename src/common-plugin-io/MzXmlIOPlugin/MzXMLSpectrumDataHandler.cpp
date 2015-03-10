/**
 * @file MzXMLSpectrumDataHandler.cpp
 * @brief implements of MzXMLSpectrumDataHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzXMLSpectrumDataHandler.h"

#include "MzXmlSpectrum.h"


using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SCAN_TAG					   "scan"
#define PEAKS_TAG					  "peaks"

#define SCAN_NUMBER_ATTR			   "num"
#define PEAKS_COUNT_ATTR			   "peaksCount"
#define PRECISION_ATTR				 "precision"


// constructor
MzXMLSpectrumDataHandler::MzXMLSpectrumDataHandler( MzXmlSpectrum& spec, kome::core::XYData& xyData )
		: m_spec( spec ), m_xyData( xyData ) {
	m_peaksCount = 0;
	m_precision = 32;
}

// destructor
MzXMLSpectrumDataHandler::~MzXMLSpectrumDataHandler() {
	// clear data
	m_xyData.clearPoints();
}

// start document
void MzXMLSpectrumDataHandler::onStartDocument() {
}

// end document
void MzXMLSpectrumDataHandler::onEndDocument() {
}

// start element
void MzXMLSpectrumDataHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	if( strcmp( name, SCAN_TAG ) == 0 ) {   // <scan>
		// get scan number
		int num = attrs.getIntValue( SCAN_NUMBER_ATTR, -1 );
		if( num == m_spec.getScanNumber() ) {
			m_peaksCount = attrs.getIntValue( PEAKS_COUNT_ATTR, 0 );
		}else{	// @date 2011.02.28 <Add> M.Izumi 
			m_peaksCount = 0;
		}
	}
	if( m_peaksCount > 0 ) {
		if( strcmp( name, PEAKS_TAG ) == 0 ) {
			// precision
			m_precision = attrs.getIntValue( PRECISION_ATTR, 32 );

			// create array
			unsigned arraySize = m_peaksCount * 2;
			if( m_precision == 64 ) {
				m_dataArray.dbl = new double[ arraySize ];
				fillzero( m_dataArray.dbl, arraySize * sizeof( double ) );
			}
			else {
				m_dataArray.flt = new float[ arraySize ];
				fillzero( m_dataArray.flt, arraySize * sizeof( float ) );
			}
		}		
	}
}

// end element
void MzXMLSpectrumDataHandler::onEndElement( const char* name, const char* text ) {
	if( strcmp( name, SCAN_TAG ) == 0 ) {   // </scan>
		m_peaksCount = 0;
	}
	if( m_peaksCount > 0 ) {
		if( strcmp( name, PEAKS_TAG ) == 0 ) {		// </peaks>
			// decode
			if( m_precision == 64 ) {		// double
				kome::core::Base64::decode(
					(char*)text,
					strlen( text ),
					m_dataArray.dbl,
					m_peaksCount * 2 * sizeof( double )
				);
			}
			else {  // float
				kome::core::Base64::decode(
					(char*)text,
					strlen( text ),
					m_dataArray.flt,
					m_peaksCount * 2 * sizeof( float )
				);
			}

			// get data
			m_xyData.reserve( m_peaksCount );
			if( m_precision == 64 ) {		// double data
				// get array
				double* arr = m_dataArray.dbl;

				// reverse
				if( islittleendian() ) {
					unsigned arraySize = m_peaksCount * 2;
					for( unsigned i = 0; i < arraySize; i++ ) {
						memreverse( arr + i, sizeof( double ) );
					}
				}

				// add points
				for( unsigned int i = 0; i < m_peaksCount; i++ ) {
					m_xyData.addPoint( arr[ i * 2 ], arr[ i * 2 + 1 ] );
				}
			}
			else {  // float data
				// get array
				float* arr = m_dataArray.flt;

				// reverse
				if( islittleendian() ) {
					unsigned arraySize = m_peaksCount * 2;
					for( unsigned int i = 0; i < arraySize; i++ ) {
						memreverse( arr + i, sizeof( float ) );
					}
				}

				// add points
				for( unsigned int i = 0; i < m_peaksCount; i++ ) {
					m_xyData.addPoint( (double)arr[ i * 2 ], (double)arr[ i * 2 + 1 ] );
				}
			}

			// delete array
			if( m_precision == 64 ) {
				delete[] m_dataArray.dbl;
			}
			else {
				delete[] m_dataArray.flt;
			}
		}
	}
}
