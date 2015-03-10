/**
 * @file MsbIO0100.cpp
 * @brief implements of MsbIO100 class
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsbIO0100.h"

#include "MsbSample.h"
#include "MsbSampleSet.h"
#include "MsbSpectrum.h"

#include <time.h>
#include <math.h>
#include <zlib.h>


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static members
MsbManager::DataValue MsbIO0100::m_fileHeader[] = {
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  0: File Header Size
	DataValue( TYPE_UCHAR,  (unsigned char)0 ),		 //  1: Endian
	DataValue( TYPE_USHORT, (unsigned short)1 ),		//  2: Major Version
	DataValue( TYPE_USHORT, (unsigned short)0 ),		//  3: Minor Version
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  4: Source File Path
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  5: Instrument
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  6: Spectrum Group Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  7: Spectrum Data Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  8: Spectrum Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  9: Spectrum Part Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 // 10: Spectrum Property Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 // 11: File Property Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 // 12: String Table Align
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 // 13: Compressed String Table Size
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 // 14: Uncompressed String Table Size
	DataValue( TYPE_TIME,   (time_t)0 )				 // 15: Time
};

MsbManager::DataValue MsbIO0100::m_dataHeader[] = {
	DataValue( TYPE_ULONG, (unsigned int)0 ),		  //  0: header size
	DataValue( TYPE_ULONG, (unsigned int)0 ),		  //  1: data size
	DataValue( TYPE_ULONG, (unsigned int)0 )		   //  2: number of data
};

MsbManager::DataValue MsbIO0100::m_groupInfo[] = {
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  0: spectrum group ID
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  1: spectrum group name
	DataValue( TYPE_ULONG,  (unsigned int)0 )		  //  2: parent group ID
};

MsbManager::DataValue MsbIO0100::m_spectrumInfo[] = {
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  0: Spectrum ID
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  1: Spectrum Name
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  2: Parent Spectrum
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  3: Spectrum Group
	DataValue( TYPE_USHORT, (unsigned short)0 ),		//  4: MS Stage
	DataValue( TYPE_FLOAT,  (float)0.0f),			   //  5: Precursor
	DataValue( TYPE_FLOAT,  (float)0.0f),			   //  6: Retention Time
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  7: Scan Number
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  8: Title
	DataValue( TYPE_BOOL,   (bool)false ),			  //  9: Chromatogram Flag
};

MsbManager::DataValue MsbIO0100::m_partInfo[] = {
	DataValue( TYPE_ULONG, (unsigned int)0 ),		  //  0: Spectrum
	DataValue( TYPE_ULONG, (unsigned int)0 ),		  //  1: Data Length
	DataValue( TYPE_FLOAT, (float)0.0f ),			   //  2: Start m/z
	DataValue( TYPE_FLOAT, (float)0.0f ),			   //  3: End m/z
	DataValue( TYPE_FLOAT, (float)0.0f ),			   //  4: Total Intensity
	DataValue( TYPE_FLOAT, (float)0.0f ),			   //  5: Max Intensity
	DataValue( TYPE_ULONG, (unsigned int)0 ),		  //  6: m/z Align
	DataValue( TYPE_ULONG, (unsigned int)0 )		   //  7: Intensity Align
};

MsbManager::DataValue MsbIO0100::m_specProp[] = {
	DataValue( TYPE_ULONG,  (unsigned int)0 ),		 //  0: Spectrum
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  1: Property Key
	DataValue( TYPE_STRING, (MsbString)0 )			  //  2: Property Value
};

MsbManager::DataValue MsbIO0100::m_fileProp[] = {
	DataValue( TYPE_STRING, (MsbString)0 ),			 //  0: Property Key
	DataValue( TYPE_STRING, (MsbString)0 )			  //  1: Property Value
};

// constructor
MsbIO0100::MsbIO0100() {
}

// destructor
MsbIO0100::~MsbIO0100() {
}

// create string table
void MsbIO0100::createStringTable( char** stringTable ) {
	// get size
	unsigned long stringTableSize = m_fileHeader[ 14 ].val.ul;
	if( stringTableSize == 0 ) {
		return;
	}

	// keep position
	unsigned int pos = ftell( m_fp );

	// create string table array
	char* st = new char[ stringTableSize ];
	*stringTable = st;

	// create buff
	unsigned int buffSize = m_fileHeader[ 13 ].val.ul;
	unsigned char* buff = new unsigned char[ buffSize ];

	// read string table
	fseek( m_fp, m_fileHeader[ 12 ].val.ul, SEEK_SET );
	fread( buff, 1, buffSize, m_fp );

	// uncompress
	uncompress( (Bytef*)st, &stringTableSize, buff, buffSize );

	// delete buffer
	delete[] buff;

	// restore position
	fseek( m_fp, pos, SEEK_SET );
}

// get file header info
void MsbIO0100::getFileHeaderInfo( MsbSample& sample, char* stringTable ) {
	// add file info
	sample.getMsbSampleSet()->setSourceFilePath( stringTable + m_fileHeader[ 4 ].val.ul );
	sample.setInstrument( stringTable + m_fileHeader[ 5 ].val.ul );
}

// read spectrum group
void MsbIO0100::readDataGroupNodes(
		kome::objects::DataGroupNode* parentGroup,
		std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
		char* stringTable 
) {
	// read header
	READ_FROM_FILE( m_dataHeader );

	// check map
	if( groupMap.find( 0 ) == groupMap.end() ) {
		groupMap[ 0 ] = parentGroup;
	}

	// get size
	unsigned int size   = m_dataHeader[ 1 ].val.ul;
	unsigned int length = m_dataHeader[ 2 ].val.ul;
	if( size == 0 || length == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ size * length ];

	// read
	fread( buff, size, length, m_fp );

	// get spectrum group
	unsigned char* pos = buff;
	for( unsigned int i = 0; i < length; i++ ) {
		// get data from buffer
		READ_FROM_BUFFER( m_groupInfo, pos );

		// parent group
		kome::objects::DataGroupNode* parent = groupMap[ m_groupInfo[ 2 ].val.ul ];

		// create group
		kome::objects::DataGroupNode* group
			= parent->createChildGroup( stringTable + m_groupInfo[ 1 ].val.ul );

		// set to map
		groupMap[ m_groupInfo[ 0 ].val.ul ] = group;

		// move read position
		pos += size;
	}

	// delete buffer
	delete[] buff;
}

// read spectra
void MsbIO0100::readSpectra(
		MsbSample& sample,
		std::map< unsigned int, MsbSpectrum* >& specMap,
		std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
		char* stringTable
) {
	// read header
	READ_FROM_FILE( m_dataHeader );

	// check map
	if( specMap.find( 0 ) == specMap.end() ) {
		specMap[ 0 ] = NULL;
	}

	// get size
	unsigned int size   = m_dataHeader[ 1 ].val.ul;
	unsigned int length = m_dataHeader[ 2 ].val.ul;
	if( size == 0 || length == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ size * length ];

	// read
	fread( buff, size, length, m_fp );

	// get spectrra
	unsigned char* pos = buff;
	for( unsigned int i = 0; i < length; i++ ) {
		// get data from buffer
		READ_FROM_BUFFER( m_spectrumInfo, pos );

		// create spectrum
		MsbSpectrum* spec = new MsbSpectrum(
			&sample,
			stringTable + m_spectrumInfo[ 1 ].val.ms,
			m_fp
		);

		// add to group
		groupMap[ m_spectrumInfo[ 3 ].val.ul ]->addSpectrum( spec );

		// ID
		specMap[ m_spectrumInfo[ 0 ].val.ul ] = spec;

		// set info
		spec->setParentSpectrum( specMap[ m_spectrumInfo[ 2 ].val.ul ] );
		spec->setMsStage( m_spectrumInfo[ 4 ].val.us );
		spec->setPrecursor( (double)m_spectrumInfo[ 5 ].val.f );
		spec->setRt( (double)m_spectrumInfo[ 6 ].val.f );
		spec->setScanNumber( m_spectrumInfo[ 7 ].val.ul );
		spec->setTitle( stringTable + m_spectrumInfo[ 8 ].val.ms );
		spec->setHasChromatogram( m_spectrumInfo[ 9 ].val.b );

		// move read position
		pos += size;
	}

	// delete buffer
	delete[] buff;
}

// read spectrum part
void MsbIO0100::readSpectrumParts(
		std::map< unsigned int, MsbSpectrum* >& specMap
){
	// read header
	READ_FROM_FILE( m_dataHeader );

	// get size
	unsigned int size   = m_dataHeader[ 1 ].val.ul;
	unsigned int length = m_dataHeader[ 2 ].val.ul;
	if( size == 0 || length == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ size * length ];

	// read
	fread( buff, size, length, m_fp );

	// get spectrra
	unsigned char* pos = buff;
	for( unsigned int i = 0; i < length; i++ ) {
		// get data from buffer
		READ_FROM_BUFFER( m_partInfo, pos );

		// get spectrum
		MsbSpectrum* spec = specMap[ m_partInfo[ 0 ].val.ul ];

		// add part
		spec->addPart(
			m_partInfo[ 1 ].val.ul,
			m_partInfo[ 2 ].val.f,
			m_partInfo[ 3 ].val.f,
			m_partInfo[ 4 ].val.f,
			0.0f,
			m_partInfo[ 5 ].val.f,
			m_partInfo[ 6 ].val.ul,
			m_partInfo[ 7 ].val.ul
		);

		// move read position
		pos += size;
	}

	// delete buffer
	delete[] buff;
}

// read spectrum property
void MsbIO0100::readSpectrumProperties(
		std::map< unsigned int, MsbSpectrum* >& specMap,
		char* stringTable
		
){
	// read header
	READ_FROM_FILE( m_dataHeader );

	// get size
	unsigned int size   = m_dataHeader[ 1 ].val.ul;
	unsigned int length = m_dataHeader[ 2 ].val.ul;
	if( size == 0 || length == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ size * length ];

	// read
	fread( buff, size, length, m_fp );

	// get spectrra
	unsigned char* pos = buff;
	for( unsigned int i = 0; i < length; i++ ) {
		// get data from buffer
		READ_FROM_BUFFER( m_specProp, pos );

		// get spectrum
		MsbSpectrum* spec = specMap[ m_specProp[ 0 ].val.ul ];

		// add property
		spec->getProperties().setValue(
			stringTable + m_specProp[ 1 ].val.ms,
			stringTable + m_specProp[ 2 ].val.ms
		);

		// move read position
		pos += size;
	}

	// delete buffer
	delete[] buff;
}

// read file property
void MsbIO0100::readFileProperties( MsbSample& sample, char* stringTable ) {
	// read header
	READ_FROM_FILE( m_dataHeader );

	// get size
	unsigned int size   = m_dataHeader[ 1 ].val.ul;
	unsigned int length = m_dataHeader[ 2 ].val.ul;
	if( size == 0 || length == 0 ) {
		return;
	}

	// create buffer
	unsigned char* buff = new unsigned char[ size * length ];

	// read
	fread( buff, size, length, m_fp );

	// get spectrra
	unsigned char* pos = buff;
	for( unsigned int i = 0; i < length; i++ ) {
		// get data from buffer
		READ_FROM_BUFFER( m_fileProp, pos );

		// add property
		sample.getRootDataGroupNode()->getProperties().setValue(
			stringTable + m_fileProp[ 0 ].val.ms,
			stringTable + m_fileProp[ 1 ].val.ms
		);

		// move read position
		pos += size;
	}

	// delete buffer
	delete[] buff;
}


// write spectrum groups
void MsbIO0100::writeDataGroupNodes(
		std::vector< kome::objects::DataGroupNode* >& groups,
		std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
) {
	// spectrum group data size
	unsigned int dataSize = GET_DATA_SIZE( m_groupInfo );

	// header
	m_dataHeader[ 0 ].val.ul = GET_DATA_SIZE( m_dataHeader );	// header size
	m_dataHeader[ 1 ].val.ul = dataSize;						 // spectrum group size
	m_dataHeader[ 2 ].val.ul = groups.size();					// number of spectrum groups

	// write header
	WRITE_TO_FILE( m_dataHeader );
	if( groups.size() == 0 ) {
		return;
	}

	// buffer
	unsigned int size = dataSize * groups.size();
	unsigned char* buff = new unsigned char[ size ];
	unsigned char* pos = buff;

	// write group data
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		// group
		kome::objects::DataGroupNode* group = groups[ i ];

		// ID		
		unsigned id =  i + 1;
		groupMap[ group ] = id;
		m_groupInfo[ 0 ].val.ul = id;						  // Spectrum Group ID

		// name
		m_groupInfo[ 1 ].val.ms = st( group->getName() );	  // Spectrum Group Name

		// parent
		unsigned int parent = 0;
		if( group->getParentGroup() != NULL ) {
			parent = groupMap[ group->getParentGroup() ];
		}
		m_groupInfo[ 2 ].val.ul = parent;					  // Parent Spectrum Group

		// write to buffer
		WRITE_TO_BUFFER( m_groupInfo, pos );
		pos += dataSize;
	}

	// write to file
	fwrite( buff, 1, size, m_fp );

	// delete buffer
	delete[] buff;
}

// write spectrum data
void MsbIO0100::writeSpectrumData(
		unsigned int id,
		kome::core::DataPoints& points,
		std::vector< SpectrumPartInfo >& parts
) {
	// number of points
	unsigned int length = points.getLength();
	if( length == 0 ) {
		return;
	}

	// number of parts
	unsigned int num = (int)ceil( sqrt( (double)length * 0.1 ) );

	// save data
	unsigned int xPos = ftell( m_fp );
	fwrite( points.getXData(), sizeof( float ), length, m_fp );

	unsigned int yPos = ftell( m_fp );
	fwrite( points.getYData(), sizeof( float ), length, m_fp );

	// add parts
	unsigned int partIdx = parts.size();
	parts.resize( partIdx + num );
	for( unsigned int i = 0; i < num; i++ ) {
		// index
		int startIdx = i * length / num;
		int endIdx = ( i + 1 ) * length / num;

		// part
		SpectrumPartInfo* part = &parts[ partIdx + i ];

		// get data
		float minX = -1.0;
		float maxX = -1.0;
		float totalY = 0.0;
		float maxY = 0.0;

		for( int j = startIdx; j < endIdx; j++ ) {
			float x = points.getX( j );
			float y = points.getY( j );

			if( minX < 0.0 || x < minX ) {
				minX = x;
			}
			if( x > maxX ) {
				maxX = x;
			}
			if( y > maxY ) {
				maxY = y;
			}
			totalY += y;
		}

		part->spectrum	   = id;
		part->length		 = endIdx - startIdx;
		part->start		  = minX;
		part->end			= maxX;
		part->totalIntensity = totalY;
		part->maxIntensity   = maxY;
		part->msAlign		= xPos + sizeof( float ) * startIdx;
		part->intAlign	   = yPos + sizeof( float ) * startIdx;
	}
}

// write spectra
void MsbIO0100::writeSpectra(
		kome::objects::DataSet& dataSet,
		std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap,
		std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
) {
	// spectrum data size
	unsigned int dataSize = GET_DATA_SIZE( m_spectrumInfo );

	// header
	m_dataHeader[ 0 ].val.ul = GET_DATA_SIZE( m_dataHeader );	// header size
	m_dataHeader[ 1 ].val.ul = dataSize;						 // spectrum size
	m_dataHeader[ 2 ].val.ul = dataSet.getNumberOfSpectra();	 // number of spectra

	// write header
	WRITE_TO_FILE( m_dataHeader );
	if( dataSet.getNumberOfSpectra() == 0 ) {
		return;
	}

	// buffer
	unsigned int size = dataSize * dataSet.getNumberOfSpectra();
	unsigned char* buff = new unsigned char[ size ];
	unsigned char* pos = buff;

	// write to buffer
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );

		// set data
		m_spectrumInfo[ 0 ].val.ul = spectrumMap[ spec ];					// Spectrum ID
		m_spectrumInfo[ 1 ].val.ms = st( spec->getName() );				  // Spectrum Name

		int parent = 0;
		if( spec->getParentSpectrum() != NULL ) {
			parent = spectrumMap[ spec->getParentSpectrum() ];
		}
		m_spectrumInfo[ 2 ].val.ul = parent;								 // Parent Spectrum

		m_spectrumInfo[ 3 ].val.ul = groupMap[ spec->getGroup() ];   // Spectrum Group
		m_spectrumInfo[ 4 ].val.us = spec->getMsStage();					 // MS Stage
		m_spectrumInfo[ 5 ].val.f  = spec->getPrecursor();				   // Precursor
		m_spectrumInfo[ 6 ].val.f  = spec->getRt();						  // Retention Time
		m_spectrumInfo[ 7 ].val.ul = spec->getScanNumber();				  // Scan Number
		m_spectrumInfo[ 8 ].val.ms = st( spec->getTitle() );				 // Title
		m_spectrumInfo[ 9 ].val.b  = spec->hasChromatogram();				// Chromatogram Flag

		// write to header
		WRITE_TO_BUFFER( m_spectrumInfo, pos );
		pos += dataSize;
	}

	// write to file
	fwrite( buff, 1, size, m_fp );

	// delete buffer
	delete[] buff;
}

// write spectrum parts
void MsbIO0100::writeSpectrumParts( std::vector< SpectrumPartInfo >& parts ) {
	// spectrum part data size
	unsigned int dataSize = GET_DATA_SIZE( m_partInfo );

	// header
	m_dataHeader[ 0 ].val.ul = GET_DATA_SIZE( m_dataHeader );	// header size
	m_dataHeader[ 1 ].val.ul = dataSize;						 // spectrum part size
	m_dataHeader[ 2 ].val.ul = parts.size();					 // number of spectrum parts

	// write header
	WRITE_TO_FILE( m_dataHeader );
	if( parts.size() == 0 ) {
		return;
	}

	// buffer
	unsigned int size = dataSize * parts.size();
	unsigned char* buff = new unsigned char[ size ];
	unsigned char* pos = buff;

	// write to buffer
	for( unsigned int i = 0; i < parts.size(); i++ ) {
		// spectrum
		SpectrumPartInfo& part = parts[ i ];

		// set data
		m_partInfo[ 0 ].val.ul = part.spectrum;				 // Spectrum ID
		m_partInfo[ 1 ].val.ul = part.length;				 // Data Length
		m_partInfo[ 2 ].val.f  = part.start;				 // Start m/z
		m_partInfo[ 3 ].val.f  = part.end;				 // End m/z
		m_partInfo[ 4 ].val.f  = part.totalIntensity;	// Total Intensity
		m_partInfo[ 5 ].val.f  = part.maxIntensity;	  // Max Intensity
		m_partInfo[ 6 ].val.ul = part.msAlign;				 // m/z Data Align
		m_partInfo[ 7 ].val.ul = part.intAlign;				 // Intensity Data Align

		// write to header
		WRITE_TO_BUFFER( m_partInfo, pos );
		pos += dataSize;
	}

	// write to file
	fwrite( buff, 1, size, m_fp );

	// delete buffer
	delete[] buff;
}

// write specrum property
void MsbIO0100::writeSpectrumProperties(
		kome::objects::DataSet& dataSet,
		std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap
) {
	// get properties
	std::vector< Prop > properties;

	for( unsigned i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		unsigned int id = spectrumMap[ spec ];

		// property
		kome::core::Properties props;
		spec->getProperties( props );

		// add to array
		unsigned int idx = properties.size();
		unsigned int num = props.getNumberOfProperties();
		if( num > 0 ) {
			// resize
			properties.resize( idx + num );

			// set value
			for( unsigned int j = 0; j < num; j++ ) {
				Prop& prop = properties[ idx + j ];
				prop.id = id;
				prop.key = st( props.getKey( j ) );
				prop.value = st( props.getValue( j ) );
			}
		}
	}

	// spectrum property data size
	unsigned int dataSize = GET_DATA_SIZE( m_specProp );

	// header
	m_dataHeader[ 0 ].val.ul = GET_DATA_SIZE( m_dataHeader );	// header size
	m_dataHeader[ 1 ].val.ul = dataSize;						 // spectrum property size
	m_dataHeader[ 2 ].val.ul = properties.size();				// number of spectrum properties

	// write header
	WRITE_TO_FILE( m_dataHeader );
	if( properties.size() == 0 ) {
		return;
	}

	// buffer
	unsigned int size = dataSize * properties.size();
	unsigned char* buff = new unsigned char[ size ];
	unsigned char* pos = buff;

	// write to buffer
	for( unsigned int i = 0; i < properties.size(); i++ ) {
		// property
		Prop& prop = properties[ i ];

		// set data
		m_specProp[ 0 ].val.ul = prop.id;
		m_specProp[ 1 ].val.ms = prop.key;				// Property Key
		m_specProp[ 2 ].val.ms = prop.value;			  // Property Value

		// write to header
		WRITE_TO_BUFFER( m_specProp, pos );
		pos += dataSize;
	}

	// write to file
	fwrite( buff, 1, size, m_fp );

	// delete buffer
	delete[] buff;
}

// write file property
void MsbIO0100::writeFileProperties( kome::objects::Sample& sample ) {
	// get properties
	kome::core::Properties& props = sample.getRootDataGroupNode()->getProperties();

	// spectrum property data size
	unsigned int dataSize = GET_DATA_SIZE( m_fileProp );

	// header
	m_dataHeader[ 0 ].val.ul = GET_DATA_SIZE( m_dataHeader );	// header size
	m_dataHeader[ 1 ].val.ul = dataSize;						 // file property size
	m_dataHeader[ 2 ].val.ul = props.getNumberOfProperties();	// number of file properties

	// write header
	WRITE_TO_FILE( m_dataHeader );
	if( props.getNumberOfProperties() == 0 ) {
		return;
	}

	// buffer
	unsigned int size = dataSize * props.getNumberOfProperties();
	unsigned char* buff = new unsigned char[ size ];
	unsigned char* pos = buff;

	// write to buffer
	for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
		// set data
		m_fileProp[ 0 ].val.ms = st( props.getKey( i ) );	  // Property Key
		m_fileProp[ 1 ].val.ms = st( props.getValue( i ) );	// Property Value

		// write to header
		WRITE_TO_BUFFER( m_fileProp, pos );
		pos += dataSize;
	}

	// write to file
	fwrite( buff, 1, size, m_fp );

	// delete buffer
	delete[] buff;
}

// get sample information
void MsbIO0100::onReadSampleInfo( MsbSample& sample ) {
	// read file header
	READ_FROM_FILE( m_fileHeader );

	// create string table
	char* stringTable = NULL;
	createStringTable( &stringTable );

	// get file Header Info
	getFileHeaderInfo( sample, stringTable );

	// delete string table
	if( stringTable != NULL ) {
		delete[] stringTable;
	}
}

// open sample
void MsbIO0100::onReadMsb( MsbSample& sample ) {
	// create string table
	char* stringTable = NULL;
	createStringTable( &stringTable );

	// keep align
	unsigned int specGroupAlign = m_fileHeader[ 6 ].val.ul;
	unsigned int spectrumAlign  = m_fileHeader[ 8 ].val.ul;
	unsigned int specPartAlign  = m_fileHeader[ 9 ].val.ul;
	unsigned int specPropAlign  = m_fileHeader[ 10 ].val.ul;
	unsigned int filePropAlign  = m_fileHeader[ 11 ].val.ul;

	// read spectrum group
	std::map< unsigned int, kome::objects::DataGroupNode* > groupMap;
	fseek( m_fp, specGroupAlign, SEEK_SET );
	readDataGroupNodes( sample.getRootDataGroupNode(), groupMap, stringTable );

	// read spectrum
	std::map< unsigned int, MsbSpectrum* > specMap;
	fseek( m_fp, spectrumAlign, SEEK_SET );
	readSpectra( sample, specMap, groupMap, stringTable );
	groupMap.clear();

	// read spectrum part
	fseek( m_fp, specPartAlign, SEEK_SET );
	readSpectrumParts( specMap );

	// read spectrum property
	fseek( m_fp, specPropAlign, SEEK_SET );
	readSpectrumProperties( specMap, stringTable );
	specMap.clear();

	// read file property
	fseek( m_fp, filePropAlign, SEEK_SET );
	readFileProperties( sample, stringTable );

	// delete string table
	if( stringTable != NULL ) {
		delete[] stringTable;
	}
}


// write msb
bool MsbIO0100::onWriteMsb(
		kome::objects::Sample& sample,
		kome::objects::DataSet& dataSet,
		bool op,
		kome::core::Progress& progress
) {
	// log
	LOG_INFO( FMT( "Writing MSB format sample. [ver 1.0]" ) );

	// get spectra and spectrum groups
	if( progress.isStopped() ) {
		return true;
	}
	std::vector< kome::objects::DataGroupNode* > groups;
	getGroups( dataSet, groups );

	// progress
	unsigned int pos = 0;
	unsigned int length = dataSet.getNumberOfSpectra() + 8;
	progress.setRange( 0, length );
	progress.setPosition( pos );

	// file header
	if( progress.isStopped() ) {
		return true;
	}
	progress.setStatus( "Writing file header" );


	m_fileHeader[ 0 ].val.ul = GET_DATA_SIZE( m_fileHeader );   // Header Size
	m_fileHeader[ 1 ].val.uc = m_bigendian ? 0 : 1;			 // Endian
	m_fileHeader[ 4 ].val.ms = st( sample.getSampleSet()->getFilePath() );	 // File Path
	m_fileHeader[ 5 ].val.ms = st( sample.getInstrument() );   // Instrument Name
	m_fileHeader[ 15 ].val.t = time( NULL );					// Time

	WRITE_TO_FILE( m_fileHeader );
	progress.setPosition( ++pos );	// pos == 1

	// spectrum group
	if( progress.isStopped() ) {
		return true;
	}
	progress.setStatus( "Writing spectrum group" );
	m_fileHeader[ 6 ].val.ul = ftell( m_fp );				   // Spectrum Group Align

	std::map< kome::objects::DataGroupNode*, unsigned int > groupMap;
	groupMap[ sample.getRootDataGroupNode() ] = 0;

	writeDataGroupNodes( groups, groupMap );
	progress.setPosition( ++pos );	// pos == 2;
	groups.clear();

	// spectrum data
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 7 ].val.ul = ftell( m_fp );				   // Spectrum Data Align

	kome::core::DataPoints points( kome::core::DataPoints::FLOAT );
	std::map< kome::objects::Spectrum*, unsigned int > spectrumMap;
	std::vector< SpectrumPartInfo > parts;
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spectrum = dataSet.getSpectrum( i );
		if( progress.isStopped() ) {
			return true;
		}
		progress.setStatus(
			FMT(
				"Writing Spectrum Data [%d/%d] (%s)",
				( i + 1 ),
				dataSet.getNumberOfSpectra(),
				spectrum->getName()
			).c_str()
		);

		// ID
		unsigned int id = ( i + 1 );
		spectrumMap[ spectrum ] = id;

		// gets data points
		spectrum->getXYData( &points, op );

		// write spectrum data
		writeSpectrumData( id, points, parts );
		progress.setPosition( ++pos );	// pos == 3 + i

		// clear points
		points.clearPoints();
	}

	// spectra
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 8 ].val.ul = ftell( m_fp );					  // Spectrum Align
	progress.setStatus( "Writing spectrum information." );

	writeSpectra( dataSet, spectrumMap, groupMap );
	progress.setPosition( ++pos );	// pos == 3 + spectra

	// spectrum part
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 9 ].val.ul = ftell( m_fp );					  // Spectrum Part Align
	progress.setStatus( "Writing spectrum parts." );

	writeSpectrumParts( parts );
	progress.setPosition( ++pos );	// pos == 4 + spectra
	parts.clear();

	// spectrum property
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 10 ].val.ul = ftell( m_fp );					 // Spectrum Property Align
	progress.setStatus( "Writing spectrum properties." );

	writeSpectrumProperties( dataSet, spectrumMap );
	progress.setPosition( ++pos );	// pos == 5 + spectra

	// file property
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 11 ].val.ul = ftell( m_fp );					  // File Property Align
	progress.setStatus( "Writing sample properties." );
	writeFileProperties( sample );
	progress.setPosition( ++pos );	// pos == 6 + spectra

	// string table
	if( progress.isStopped() ) {
		return true;
	}
	m_fileHeader[ 12 ].val.ul = ftell( m_fp );					   // String Table Align

	progress.setStatus( "Writing string table." );
	m_fileHeader[ 13 ].val.ul = writeStringTable();				  // Compressed string table seize

	progress.setPosition( ++pos );	// pos == 7 + spectra

	m_fileHeader[ 14 ].val.ul = m_stringTable.size();				// Uncompressed String Table Size
	m_stringTable.clear();

	// rewrite file header
	if( progress.isStopped() ) {
		return true;
	}
	progress.setStatus( "Rewriting file header." );
	fseek( m_fp, 0, SEEK_SET );
	WRITE_TO_FILE( m_fileHeader );
	progress.setPosition( ++pos );	// pos == 8 + spectra


	return true;
}
