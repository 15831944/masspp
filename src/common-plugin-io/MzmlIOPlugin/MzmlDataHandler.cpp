/**
 * @file MzmlDataHandler.cpp
 * @brief implements of MzmlDataHandler class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlDataHandler.h"
#include "MzmlSample.h"

#include <zlib.h>


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SPEC_TAG_NAME							  "spectrum"
#define CHROM_TAG_NAME							  "chromatogram"
#define ARRAY_TAG_NAME							  "binaryDataArray"
#define PARAM_GROUP_TAG_NAME					  "referenceableParamGroupRef"
#define CV_TAG_NAME								  "cvParam"
#define BINARY_TAG_NAME							  "binary"

#define SPEC_ID_ATTR_NAME						  "id"
#define SPEC_LENGTH_ATTR_NAME					  "defaultArrayLength"
#define CHROM_ID_ATTR_NAME						  "id"
#define CHROM_LENGTH_ATTR_NAME					  "defaultArrayLength"
#define ARRAY_LENGTH_ATTR_NAME					  "arrayLength"
#define PARAM_GROUP_ID_ATTR_NAME				  "ref"
#define CV_ACCESSION_ATTR_NAME					  "accession"
#define CV_UNIT_ATTR_NAME						  "unitAccession"


// constructor
MzmlDataHandler::MzmlDataHandler( MzmlSample& sample, const char* id, kome::core::XYData& xyData )
		: m_sample( sample ), m_id( NVL( id, "" ) ), m_xyData( xyData ) {
	m_reverseFlg = isbigendian();
	onStartDocument();
}

// destructor
MzmlDataHandler::~MzmlDataHandler() {
}

// set points
void MzmlDataHandler::setPoints() {
	// points
	if( m_dataLength > 0 ) {
		m_xyData.reserve( m_dataLength );

		for( int i = 0; i < m_dataLength; i++ ) {
			m_xyData.addPoint( m_xArray[ i ], m_yArray[ i ] );
		}
	}

	// delete arrays
	if( m_xArray != NULL ) {
		delete[] m_xArray;
		m_xArray = NULL;
	}

	if( m_yArray != NULL ) {
		delete m_yArray;
		m_yArray = NULL;
	}

	m_reading = false;
}

// start document
void MzmlDataHandler::onStartDocument() {
	// initialize
	m_reading = false;
	m_dataLength = 0;
	m_available = false;

	m_xArray = NULL;
	m_yArray = NULL;
}

// end document
void MzmlDataHandler::onEndDocument() {
	// delete arrays
	if( m_xArray != NULL ) {
		delete[] m_xArray;
		m_xArray = NULL;
	}

	if( m_yArray != NULL ) {
		delete m_yArray;
		m_yArray = NULL;
	}
}

// start element
void MzmlDataHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, SPEC_TAG_NAME ) == 0 ) {	// <spectrum>
		// ID
		const char* id = attrs.getStringValue( SPEC_ID_ATTR_NAME, "" );
		if( m_id.compare( id ) == 0 ) {
			m_reading = true;
			m_dataLength = attrs.getIntValue( SPEC_LENGTH_ATTR_NAME, 0 );
		}
	}
	else if( strcmp( name, CHROM_TAG_NAME ) == 0 ) {	// <chromatogram>
		// ID
		const char* id = attrs.getStringValue( CHROM_ID_ATTR_NAME, "" );
		if( m_id.compare( id ) == 0 ) {
			m_reading = true;
			m_dataLength = attrs.getIntValue( CHROM_LENGTH_ATTR_NAME, 0 );
		}
	}
	else if( strcmp( name, ARRAY_TAG_NAME ) == 0 ) {	// <binaryDataArray>
		if( m_reading ) {
			m_dataLength = attrs.getIntValue( ARRAY_LENGTH_ATTR_NAME, m_dataLength );

			m_arrayInfo.name.clear();
			m_arrayInfo.bits = 32;
			m_arrayInfo.compressed = false;
			m_arrayInfo.scale = 1.0;
			m_arrayInfo.isY = false;

			m_available = false;
		}
	}
	else if( strcmp( name, PARAM_GROUP_TAG_NAME ) == 0 ) {	// <referenceableParamGroupRef>
		if( m_reading ) {
			const char* id = attrs.getStringValue( PARAM_GROUP_ID_ATTR_NAME, "" );
			MzmlSample::ArrayInfo* info = m_sample.getArrayInfo( id );
			if( info != NULL ) {
				m_arrayInfo = *info;
				m_available = true;
			}
		}
	}
	else if( strcmp( name, CV_TAG_NAME ) == 0 ) {	// <cvParam>
		if( m_reading ) {
			// accession
			const char* accession = attrs.getStringValue( CV_ACCESSION_ATTR_NAME, "" );

			// each accessions
			if( strcmp( accession, "MS:1000514" ) == 0 ) {	// m/z array
				m_arrayInfo.isY = false;
				m_arrayInfo.scale = 1.0;

				m_available = true;
			}
			else if( strcmp( accession, "MS:1000595" ) == 0 ) {	// RT array
				m_arrayInfo.isY = false;
				m_arrayInfo.scale = 1.0;

				m_available = true;

				const char* unit = attrs.getStringValue( CV_UNIT_ATTR_NAME, "" );
				if( strcmp( unit, "UO:0000031" ) != 0 ) {	// second
					m_arrayInfo.scale = 1.0 / 60.0;
				}
			}
			else if( strcmp( accession, "MS:1000515" ) == 0 ) {	// intensity array
				m_arrayInfo.isY = true;
				m_available = true;
			}
			else if( strcmp( accession, "MS:1000521" ) == 0 ) {	// 32-bit float
				m_arrayInfo.bits = 32;
			}
			else if( strcmp( accession, "MS:1000523" ) == 0 ) {	// 64-bit float
				m_arrayInfo.bits = 64;
			}
			else if( strcmp( accession, "MS:1000576" ) == 0 ) {	// no compression
				m_arrayInfo.compressed = false;
			}
			else if( strcmp( accession, "MS:1000574" ) == 0 ) {	// zlib compression
				m_arrayInfo.compressed = true;
			}
		}
	}
}

// end element
void MzmlDataHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, SPEC_TAG_NAME ) == 0 ) {	// </spectrum>
		if( m_reading ) {
			setPoints();
		}
	}
	else if( strcmp( name, CHROM_TAG_NAME ) == 0 ) {	// </chromatogram>
		if( m_reading ) {
			setPoints();
		}
	}
	else if( strcmp( name, BINARY_TAG_NAME ) == 0 ) {	// </binary>
		if( m_reading && m_available && m_dataLength > 0 ) {
			// create array
			double* arr = NULL;
			if( m_arrayInfo.isY ) {
				if( m_yArray == NULL ) {
					m_yArray = new double[ m_dataLength ];
				}
				arr = m_yArray;
			}
			else {
				if( m_xArray == NULL ) {
					m_xArray = new double[ m_dataLength ];
				}
				arr = m_xArray;
			}

			// get data
			char* data0 = new char[ strlen( text ) ];
			unsigned long size0 = kome::core::Base64::decode( (char*)text, strlen( text ), data0, strlen( text ) );

			unsigned long size1 = m_dataLength * m_arrayInfo.bits / 8;
			char* data1 = new char[ size1 ];

			if( m_arrayInfo.compressed ) {
				uncompress( (Bytef*)data1, &size1, (Bytef*)data0, size0 );
			}
			else {
				memcpy( data1, data0, size1 );
			}

			// add to array
			if( m_arrayInfo.bits == 64 ) {	// double
				double* dArr = (double*)data1;
				for( int i = 0; i < m_dataLength; i++ ) {
					arr[ i ] = dArr[ i ] * m_arrayInfo.scale;
					if( m_reverseFlg ) {
						memreverse( arr + i, sizeof( double ) );
					}
				}
			}
			else {	// float
				float* fArr = (float*)data1;
				for( int i = 0; i < m_dataLength; i++ ) {
					arr[ i ] = (double)fArr[ i ] * m_arrayInfo.scale;
					if( m_reverseFlg ) {
						memreverse( arr + i, sizeof( double ) );
					}
				}
			}

			// delete
			delete[] data0;
			delete[] data1;
		}
		m_available = false;
	}
}
