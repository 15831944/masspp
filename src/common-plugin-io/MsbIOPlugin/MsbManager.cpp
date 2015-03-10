/**
 * @file MsbManager.cpp
 * @brief implements of MabManager class
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MsbManager.h"

#include "MsbIO0100.h"
#include "MsbIO0101.h"
#include "MsbIO0102.h"
#include "MsbIO0103.h"
#include "MsbIO0104.h"
#include "MsbIO0105.h"
#include "MsbIO0106.h"
#include "MsbIO0107.h"
#include "MsbIO0108.h"
#include "MsbIO0200.h"
#include "MsbIO0201.h"
#include "MsbIO0202.h"

#include "SpectrumPart.h"
#include "SpectrumPartArray.h"


#include <zlib.h>
#include <math.h>

#include <list>

using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// static member
const bool MsbManager::m_bigendian = isbigendian();
const float MsbManager::m_partRange = 8.0f;

// constructor
MsbManager::MsbManager() {
	m_fp = NULL;
	m_reverse = false;
}

// destructor
MsbManager::~MsbManager() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
}

// set file descriptor
void MsbManager::setFile( FILE* file ) {
	// close
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}

	// set
	m_fp = file;
}

// get file descriptor
FILE* MsbManager::getFile() {
	return m_fp;
}

// set endian
void MsbManager::setEndian( unsigned char endian ) {
	if( m_bigendian ) {	// big endian
		m_reverse = ( endian != 0 );
	}
	else {	// little endian
		m_reverse = ( endian != 1 );
	}
}

// add to string table
MsbManager::MsbString MsbManager::st( const char* s ) {
	// create string object
	std::string str = NVL( s, "" );

	// check position
	unsigned int pos = m_stringTable.size();
	if( m_stringMap.find( str ) != m_stringMap.end() ) {
		pos = m_stringMap[ str ];
		return pos;
	}

	/** add to table */
	m_stringTable.reserve( str.length() + 1 );
	if( s != NULL ) {
		for( unsigned int i = 0; i < str.length(); i++ ) {
			m_stringTable.push_back( s[ i ] );
		}
	}
	m_stringTable.push_back( '\0' );

	// insert to map
	m_stringMap[ str ] = pos;

	return pos;
}

// write string table
unsigned int MsbManager::writeStringTable() {
	// check table
	if( m_stringTable.size() == 0 ) {
		return 0;
	}

	// create buffer
	unsigned long size = m_stringTable.size() * 11 / 10 + 13;
	unsigned char* buffer = new unsigned char[ size ];

	// compress
	compress( (Bytef*)buffer, &size, (Bytef*)&m_stringTable[ 0 ], m_stringTable.size() );

	// write
	fwrite( buffer, sizeof( unsigned char ), size, m_fp );

	// delete buffer
	delete[] buffer;

	return size;
}

// read sample
void MsbManager::readSampleInfo( MsbSample& sample ) {
	onReadSampleInfo( sample );
}

// read msb
void MsbManager::readMsb( MsbSample& sample ) {
	onReadMsb( sample );
}

// write msb
bool MsbManager::writeMsb(
		const char* path,
		kome::objects::DataSet& dataSet,
		bool op,
		kome::core::Progress& progress

) {
	// initialize
	m_stringTable.clear();
	m_stringMap.clear();

	// check spectra
	if( dataSet.getNumberOfSpectra() == 0 && dataSet.getNumberOfChromatograms() == 0 ) {
		LOG_WARN( FMT( "There is no data to save." ) );
		return false;
	}

	// open
	m_fp = fileopen( path, "wb" );
	if( m_fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the sample for writing. Check file / folder permissions. [%s]", NVL( path, "" ) ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	bool ret = onWriteMsb( *( dataSet.getSample() ), dataSet, op, progress );

	// close
	fflush( m_fp );
	fclose( m_fp );
	m_fp = NULL;

	return ret;
}

// close file
void MsbManager::closeFile() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
}

// get spectrum groups
void MsbManager::getGroups(
		kome::objects::DataSet& dataSet,
		std::vector< kome::objects::DataGroupNode* >& groups
) {
	// sample
	kome::objects::Sample* sample = dataSet.getSample();
	if( sample == NULL ) {
		return;
	}

	// get group array
	std::vector< kome::objects::DataGroupNode* > groupArray;
	getGroupArray( groupArray, sample->getRootDataGroupNode() );

	// get group set
	std::set< kome::objects::DataGroupNode* > groupSet;
	getGroupSet( groupSet, dataSet );

	// get groups
	for( unsigned int i = 0; i < groupArray.size(); i++ ) {
		kome::objects::DataGroupNode* group = groupArray[ i ];

		if( groupSet.find( group ) != groupSet.end() ) {
			groups.push_back( group );
		}
	}
}

// get spectrum groups array
void MsbManager::getGroupArray(
		std::vector< kome::objects::DataGroupNode* >& groupArray,
		kome::objects::DataGroupNode* parentGroup
) {
	// add child groups
	for( unsigned int i = 0; i < parentGroup->getNumberOfChildren(); i++ ) {
		// child group
		kome::objects::DataGroupNode* child = parentGroup->getChild( i );

		// add
		groupArray.push_back( child );
		getGroupArray( groupArray, child );
	}
}

// get spectrum groups set
void MsbManager::getGroupSet(
		std::set< kome::objects::DataGroupNode* >& groupSet,
		kome::objects::DataSet& dataSet
) {
	// get groups from chromatograms
	for( unsigned int i = 0; i < dataSet.getNumberOfChromatograms(); i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = dataSet.getChromatogram( i );

		// group
		kome::objects::DataGroupNode* group = chrom->getGroup();

		// search parent groups
		while( group != NULL && group->getParentGroup() != NULL ) {
			if( groupSet.find( group ) == groupSet.end() ) {
				// add
				groupSet.insert( group );
				group = group->getParentGroup();
			}
			else {
				group = NULL;
			}
		}
	}

	// get groups from spectra
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );

		// group
		kome::objects::DataGroupNode* group = spec->getGroup();

		// search parent groups
		while( group != NULL && group->getParentGroup() != NULL ) {
			if( groupSet.find( group ) == groupSet.end() ) {
				// add
				groupSet.insert( group );
				group = group->getParentGroup();
			}
			else {
				group = NULL;
			}			
		}
	}
}

// get total size
unsigned int MsbManager::getSize( DataValue* values, const unsigned int size ) {
	// check parameters
	if( values == NULL || size == 0 ) {
		return 0;
	}

	// get size
	int dataSize = 0;
	for( unsigned int i = 0; i < size; i++ ) {
		void* p = NULL;
		unsigned int size = 0;

		getPosition( values[ i ], &p, &size );

		dataSize += size;
	}

	return dataSize;
}

// read data
void MsbManager::readFromFile( DataValue* values, const unsigned int size ) {
	// get size
	unsigned dataSize = getSize( values, size );
	if( dataSize == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ dataSize ];

	// read
	fread( buff, 1, dataSize, m_fp );

	// get data from buffer
	readFromBuffer( values, size, buff );

	// delete buffer
	delete[] buff;
}

// read data from buffer
void MsbManager::readFromBuffer( DataValue* values, const unsigned int size, void* buffer ) {
	// cast
	unsigned char* bf = (unsigned char*)buffer;

	// get data
	for( unsigned int i = 0; i < size; i++ ) {
		// data value
		DataValue& v = values[ i ];

		// get pointer and size
		void* p = NULL;
		unsigned int s = 0;

		getPosition( v, &p, &s );

		// copy
		if( p != NULL && s > 0 ) {
			memcpy( p, bf, s );
			if( m_reverse ) {
				memreverse( p, s );
			}
			bf += s;
		}
	}
}

// write
void MsbManager::writeToFile( DataValue* values, const unsigned int size ) {
	// get data size
	unsigned dataSize = getSize( values, size );
	if( dataSize == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ dataSize ];

	// write to bufefr
	writeToBuffer( values, size, buff );

	// write
	fwrite( buff, 1, dataSize, m_fp );

	// delete buffer
	delete[] buff;
}

// write to buffer
void MsbManager::writeToBuffer( DataValue* values, const unsigned int size, void* buffer ) {
	// cast
	unsigned char* bf = (unsigned char*)buffer;

	// write
	for( unsigned int i = 0; i < size; i++ ) {
		// data value
		DataValue& v = values[ i ];

		// get pointer and size
		void* p = NULL;
		unsigned int s = 0;

		getPosition( v, &p, &s );

		if( p != NULL && s > 0 ) {
			memcpy( bf, p, s );
			bf += s;
		}
	}
}

// get position
void MsbManager::getPosition( DataValue& value, void** p, unsigned int* size ) {
	switch( value.type ) {
	case TYPE_BOOL:
		*p = &value.val.b;
		*size = sizeof( value.val.b );
		break;
	case TYPE_CHAR:
		*p = &value.val.c;
		*size = sizeof( value.val.c );
		break;
	case TYPE_UCHAR:
		*p = &value.val.uc;
		*size = sizeof( value.val.uc );
		break;
	case TYPE_SHORT:
		*p = &value.val.s;
		*size = sizeof( value.val.s );
		break;
	case TYPE_USHORT:
		*p = &value.val.us;
		*size = sizeof( value.val.us );
		break;
	case TYPE_LONG:
		*p = &value.val.l;
		*size = sizeof( value.val.l );
		break;
	case TYPE_ULONG:
		*p = &value.val.ul;
		*size = sizeof( value.val.ul );
		break;
	case TYPE_LONGLONG:
		*p = &value.val.ll;
		*size = sizeof( value.val.ll );
		break;
	case TYPE_ULONGLONG:
		*p = &value.val.ull;
		*size = sizeof( value.val.ull );
		break;
	case TYPE_FLOAT:
		*p = &value.val.f;
		*size = sizeof( value.val.f );
		break;
	case TYPE_DOUBLE:
		*p = &value.val.d;
		*size = sizeof( value.val.d );
		break;
	case TYPE_TIME:
		*p = &value.val.t;
		*size = sizeof( value.val.t );
		break;
	case TYPE_STRING:
		*p = &value.val.ms;
		*size = sizeof( value.val.ms );
		break;
	default:
		break;
	}
}

// get parts
void MsbManager::getParts( kome::core::XYData& dps, SpectrumPartArray& arr ) {
	// variables
	float maxMz = float();
	SpectrumPart* lastPart = NULL;

	// create parts
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		float x = (float)dps.getX( i );
		float y = (float)dps.getY( i );

		if( fabs( y ) > 0.0 ) {
			if( x > maxMz || lastPart == NULL ) {	// create new parts
				int num = roundnum( x / m_partRange );
				float mz = (float)num * m_partRange;
				maxMz = mz + m_partRange / 2.0f;

				lastPart = &( arr.addPart() );

				lastPart->setMz( mz );
				lastPart->setMinMz( x );
				lastPart->setMaxMz( x );
				lastPart->setTotalIntensity( y );
				lastPart->setMinIntensity( y );
				lastPart->setMaxIntensity( y );
				lastPart->setLength( 1 );
			}
			else {
				float totalInt = lastPart->getMaxIntensity() + y;
				float minInt = MIN( lastPart->getMinIntensity(), y );
				float maxInt = MAX( lastPart->getMaxIntensity(), y );
				unsigned int length = lastPart->getLength() + 1;
		 
				lastPart->setMaxMz( x );
				lastPart->setTotalIntensity( totalInt );
				lastPart->setMinIntensity( minInt );
				lastPart->setMaxIntensity( maxInt );
				lastPart->setLength( length );
			}
		}
	}
}

// get file format version
void MsbManager::getFormatVersion(
		unsigned short* major,
		unsigned short* minor,
		unsigned char* endian,
		FILE* fp
) {
	// skip size
	unsigned int skip = sizeof( unsigned int );	// File Header Size

	// seek
	fseek( fp, skip, SEEK_SET );

	// endian
	unsigned char e;
	fread( &e, sizeof( unsigned char ), 1, fp );
	bool rv = m_bigendian ? ( e != 0 ) : ( e != 1 );
	*endian = e;

	// major version
	fread( major, sizeof( unsigned short ), 1, fp );
	if( rv ) {
		memreverse( major, sizeof( unsigned short ) );
	}

	// minor version
	fread( minor, sizeof( unsigned short ), 1, fp );
	if( rv ) {
		memreverse( minor, sizeof( unsigned short ) );
	}
}

// gets msb manager
MsbManager* MsbManager::getMsbManager(
		const unsigned short majorVersion,
		const unsigned short minorVersion,
		const unsigned char endian,
		FILE* fp
) {
	MsbManager* mgr = NULL;

	// check version
	if( majorVersion == 1 ) {
		if( minorVersion == 0 ) {
			mgr = new MsbIO0100();
		}
		else if( minorVersion == 1 ) {
			mgr = new MsbIO0101();
		}
		else if( minorVersion == 2 ) {
			mgr = new MsbIO0102();
		}
		else if( minorVersion == 3 ) {
			mgr = new MsbIO0103();
		}
		else if( minorVersion == 4 ) {
			mgr = new MsbIO0104();
		}
		else if( minorVersion == 5 ) {
			mgr = new MsbIO0105();
		}
		else if( minorVersion == 6 ) {
			mgr = new MsbIO0106();
		}
		else if( minorVersion == 7 ) {
			mgr = new MsbIO0107();
		}
		else if( minorVersion == 8 ) {
			mgr = new MsbIO0108();
		}
	}
	else if( majorVersion == 2 ) {
		if( minorVersion == 0 ) {
			mgr = new MsbIO0200();
		}
		else if( minorVersion == 1 ) {
			mgr = new MsbIO0201();
		}
		else if( minorVersion == 2 ) {
			mgr = new MsbIO0202();
		}
	}

	// set
	if( mgr != NULL ) {
		fseek( fp, 0, SEEK_SET );

		mgr->setFile( fp );
		mgr->setEndian( endian );
	}

	return mgr;
}

// get part range
float MsbManager::getPartRange() {
	return m_partRange;
}


//////////////////////////

// DataValue
MsbManager::DataValue::DataValue( MsbManager::ValueType t, boost::any v ) : type( t ) {
	// set value
	switch( t ) {
	case TYPE_BOOL:
		val.b = boost::any_cast< bool >( v );
		break;
	case TYPE_CHAR:
		val.c = boost::any_cast< char >( v );
		break;
	case TYPE_UCHAR:
		val.uc = boost::any_cast< unsigned char >( v );
		break;
	case TYPE_SHORT:
		val.s = boost::any_cast< short >( v );
		break;
	case TYPE_USHORT:
		val.us = boost::any_cast< unsigned short >( v );
		break;
	case TYPE_LONG:
		val.l = boost::any_cast< int >( v );
		break;
	case TYPE_ULONG:
		val.ul = boost::any_cast< unsigned int >( v );
		break;
	case TYPE_LONGLONG:
		val.ll = boost::any_cast< long long >( v );
		break;
	case TYPE_ULONGLONG:
		val.ull = boost::any_cast< unsigned long long >( v );
		break;
	case TYPE_FLOAT:
		val.f = boost::any_cast< float >( v );
		break;
	case TYPE_DOUBLE:
		val.d = boost::any_cast< double >( v );
		break;
	case TYPE_TIME:
		val.t = boost::any_cast< time_t >( v );
		break;
	case TYPE_STRING:
		val.ms = boost::any_cast< MsbString >( v );
		break;
	default:
		break;
	}
}

// destructor
MsbManager::DataValue::~DataValue() {
}
