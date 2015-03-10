/**
 * @file MzmlHandler.cpp
 * @brief implelements of MzmlHandler class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlHandler.h"

#include "MzmlSample.h"
#include "MzmlSpectrum.h"
#include "MzmlChromatogram.h"

#include "AccessionManager.h"
#include "Accession.h"


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MACHINE_ACC				   "MS:1000031"
#define ARRAY_ACC					 "MS:1000513"
#define MZ_ARRAY_ACC				  "MS:1000514"
#define RT_ARRAY_ACC				  "MS:1000595"
#define INT_ARRAY_ACC				 "MS:1000515"
#define SPEC_TYPE_ACC				 "MS:1000559"
#define SOFTWARE_ACC				  "MS:1000531"

#define CVPARAM_TAG_NAME			  "cvParam"
#define SRC_FILE_TAG_NAME			 "sourceFile"
#define SOFTWARE_TAG_NAME			 "software"
#define PARAM_GROUP_TAG_NAME		 "referenceableParamGroup"
#define SPEC_TAG_NAME				 "spectrum"
#define PRECURSOR_TAG_NAME			"precursor"
#define PRODUCT_TAG_NAME            "product"
#define CHROM_TAG_NAME				"chromatogram"
#define ION_SELECTION_TAG_NAME		"ionSelection"
#define USR_PARAM_TAG_NAME			"userParam"
#define OFFSET_TAG_NAME			   "offset"
#define REF_TAG_NAME               "referenceableParamGroupRef"


#define CV_ACC_ATTR_NAME			  "accession"
#define CV_NAME_ATTR_NAME			 "name"
#define CV_VALUE_ATTR_NAME			"value"
#define CV_UNIT_ATTR_NAME			 "unitAccession"
#define SRC_FILE_ATTR_NAME			"name"
#define SRC_LOC_ATTR_NAME			 "location"
#define SOFTWARE_ID_ATTR_NAME		 "id"
#define SOFTWARE_VER_ATTR_NAME		"version"
#define PARAM_GROUP_ID_ATTR_NAME	  "id"
#define SPEC_ID_ATTR_NAME			 "id"
#define SPEC_SCAN_NUM_ATTR_NAME	   "scanNumber"
#define PREC_SPEC_ATTR_NAME		   "spectrumRef"
#define CHROM_ID_ATTR_NAME			"id"
#define SPEC_MS_LEVEL_ATTR_NAME	   "msLevel"
#define USR_PARAM_NAME_ATTR_NAME	  "name"
#define USR_PARAM_VAL_ATTR_NAME	   "value"
#define OFFSET_SCAN_ATTR_NAME		 "scanNumber"
#define OFFSET_IDREF_ATTR_NAME		"idRef"
#define REF_REF_ATTR_NAME           "ref"


// constructor
MzmlHandler::MzmlHandler( MzmlSample& file ) : m_file( file ) {
	// initialize
	onStartDocument();

	// accessions
	AccessionManager& accManager = AccessionManager::getInstance();

	m_machineAcc = accManager.getAccession( MACHINE_ACC );
	m_arrAcc = accManager.getAccession( ARRAY_ACC );
	m_specAcc = accManager.getAccession( SPEC_TYPE_ACC );
	m_softwareAcc = accManager.getAccession( SOFTWARE_ACC );
}

// destructor
MzmlHandler::~MzmlHandler() {
}

// get SpecInfo
MzmlHandler::SpecInfo* MzmlHandler::getSpecInfo( const char* name ) {
	// name
	std::string n = trimstring( name );

	// spec info
	bool iniFlg = ( m_specMap.find( n ) == m_specMap.end() );
	SpecInfo* s = &( m_specMap[ n ] );

	// initialize
	if( iniFlg ) {
		s->stage = -1;
		s->centroid = -1;
		s->polarity = kome::objects::Spectrum::POLARITY_UNKNOWN;
		s->mzLowerLimit = -1.0;
		s->mzUpperLimit = -1.0;
		s->tic = -1.0;
		s->bpm = -1.0;
		s->bpi = -1.0;
		s->rt = -1.0;
		s->prec = -1.0;
		s->props.clear();
		s->collisionEnergy.clear();
	}

	return s;
}

// set spec info
void MzmlHandler::setSpecInfo( kome::objects::Spectrum* spec, SpecInfo* info ) {
	if( info->stage > 0 ) {
		spec->setMsStage( info->stage );
	}

	if( info->centroid == 0 || info->centroid == 1 ) {
		spec->setCentroidMode( info->centroid == 1 );
	}

	if( info->polarity != kome::objects::Spectrum::POLARITY_UNKNOWN ) {
		spec->setPolarity( info->polarity );
	}

	if( info->mzLowerLimit >= 0.0 ) {
		spec->setMinX( info->mzLowerLimit );
	}

	if( info->mzUpperLimit >= 0.0 ) {
		spec->setMaxX( info->mzUpperLimit );
	}

	if( info->tic >= 0.0 ) {
		spec->setTotalIntensity( info->tic );
	}

	if( info->bpi >= 0.0 ) {
		spec->setMaxIntensity( info->bpi );
	}

	if( info->prec >= 0.0 ) {
		spec->setPrecursor( info->prec );
	}

	if( !info->collisionEnergy.empty() ) {
		spec->setCollisionEnergy( info->collisionEnergy.c_str() );
	}

	for( unsigned int i = 0; i < info->props.size(); i++ ) {
		spec->getProperties().setValue( info->props.at( i ).first.c_str(), info->props.at( i ).second.c_str() );
	}
}

// start document
void MzmlHandler::onStartDocument() {
	// clear maps 
	m_specIdMap.clear();
	m_specScanMap.clear();
	m_chromIdMap.clear();

	// spectrum values
	m_currSpec = NULL;
	m_scanNum = 0;
	m_currSpecInfo = getSpecInfo( NULL );

	// chromatogram values
	m_currChrom = NULL;

	// offset
	m_offsetScan = -1;

	// flag
	m_inIonTag = false;
	m_inPrecursor = false;
	m_inProduct = false;

	// position
	m_pos = -1;
}

// end document
void MzmlHandler::onEndDocument() {
}

// start element
void MzmlHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, CVPARAM_TAG_NAME ) == 0 ) {	// <cvParam>
		const char* a = attrs.getStringValue( CV_ACC_ATTR_NAME, "" );
		const char* n = attrs.getStringValue( CV_NAME_ATTR_NAME, "" );
		const char* v = attrs.getStringValue( CV_VALUE_ATTR_NAME, "" );

		if( strcmp( a, "MS:1000511" ) == 0 ) {	// ms level
			int stage = toint( v, 10, 1 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->stage = stage;
			}
			else {
				m_currSpec->setMsStage( stage );
			}
		}
		else if( strcmp( a, "MS:1000521" ) == 0 ) {	// 32-bit float
			m_arrayInfo.bits = 32;
		}
		else if( strcmp( a, "MS:1000523" ) == 0 ) {	// 64-bit float
			m_arrayInfo.bits = 64;
		}
		else if( strcmp( a, "MS:1000576" ) == 0 ) {	// no compression
			m_arrayInfo.compressed = false;
		}
		else if( strcmp( a, "MS:1000574" ) == 0 ) {	// zlib compression
			m_arrayInfo.compressed = true;
		}
		else if( strcmp( a, "MS:1000127" ) == 0 ) {	// centroid spectrum
			if( m_currSpec == NULL ) {
				m_currSpecInfo->centroid = 1;
			}
			else {
				m_currSpec->setCentroidMode( true );
			}
		}
		else if( strcmp( a, "MS:1000128" ) == 0 ) {	// profile spectrum
			if( m_currSpec == NULL ) {
				m_currSpecInfo->centroid = 0;
			}
			else {
				m_currSpec->setCentroidMode( false );
			}
		}
		else if( strcmp( a, "MS:1000129" ) == 0 ) {	// negative scan
			if( m_currSpec == NULL ) {
				m_currSpecInfo->polarity = kome::objects::Spectrum::POLARITY_NEGATIVE;
			}
			else {
				m_currSpec->setPolarity( kome::objects::Spectrum::POLARITY_NEGATIVE );
			}
		}
		else if( strcmp( a, "MS:1000130" ) == 0 ) {	// positive scan
			if( m_currSpec == NULL ) {
				m_currSpecInfo->polarity = kome::objects::Spectrum::POLARITY_POSITIVE;
			}
			else {
				m_currSpec->setPolarity( kome::objects::Spectrum::POLARITY_POSITIVE );
			}
		}
		else if( strcmp( a, "MS:1000501" ) == 0 ) {	// scan m/z lower limit
			double lowMz = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->mzLowerLimit = lowMz;
			}
			else {
				m_currSpec->setMinX( lowMz );
			}
		}
		else if( strcmp( a, "MS:1000500" ) == 0 ) {	// scan m/z upper limit
			double upMz = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->mzUpperLimit = upMz;
			}
			else {
				m_currSpec->setMaxX( upMz );
			}
		}
		else if( strcmp( a, "MS:1000285" ) == 0 ) {	// total ion current
			double tic = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->tic = tic;
			}
			else {
				m_currSpec->setTotalIntensity( tic );
			}
		}
		else if( strcmp( a, "MS:1000504" ) == 0 ) {	// base peak m/z
			double bpm = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->bpm = bpm;
			}
			else {
				m_currSpec->setBasePeakMass( bpm );
			}
		}
		else if( strcmp( a, "MS:1000505" ) == 0 ) {	// base peak intensity
			double bpi = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->bpi = bpi;
			}
			else {
				m_currSpec->setMaxIntensity( bpi );
			}
		}
		else if( strcmp( a, "MS:1000016" ) == 0 ) {	// scan start time
			double rt = todouble( v, -1.0 );
			const char* unit = attrs.getStringValue( CV_UNIT_ATTR_NAME, "" );
			if( strcmp( unit, "UO:0000031" ) != 0 ) {	// second
				rt /= 60.0;
			}

			if( m_currSpec == NULL ) {
				m_currSpecInfo->rt = rt;
			}
			else {
				m_currSpec->setRt( rt );
				m_currSpec->setHasChromatogram( true );
			}
		}
		else if( strcmp( a, "MS:1000744" ) == 0 ) {	// selected ion m/z
			double prec = todouble( v, -1.0 );
			if( m_currSpec == NULL ) {
				m_currSpecInfo->prec = prec;
			}
			else {
				m_currSpec->setPrecursor( prec );
			}
		}
		else if( strcmp( a, "MS:1000512" ) == 0 ) {	// filter string
			if( m_currSpec == NULL ) {
				m_currSpecInfo->props.push_back( std::make_pair( "Filter", v ) );
			}
			else {
				m_currSpec->getProperties().setValue( "Filter", v );
			}
		}
		else if( strcmp( a, "MS:1000040" ) == 0 ) {	// m/z
			double mz = todouble( v, -1.0 );
			if( m_inIonTag && m_currSpec != NULL ) {
				m_currSpec->setPrecursor( mz );
			}
		}
		else if( strcmp( a, "MS:1000827" ) == 0 ) {    // isolation window target m/z
			if( m_currSpec != NULL || m_currSpecInfo != NULL ) {    // spectrum
				std::string name = "Isolation Window Target m/z";
				if( m_inPrecursor ) {
					name = "Precursor Target";
				}
				if( m_inProduct ) {
					name = "Product Target";
				}

				if( m_currSpec == NULL ) {
					m_currSpecInfo->props.push_back( std::make_pair( name.c_str(), v ) );
				}
				else {
					m_currSpec->getProperties().setValue( name.c_str(), v );
				}
			}

			if( m_currChrom != NULL ) {    // chromatogram
				double mz = todouble( v, -1.0 );
				if( mz >= 0.0 ) {
					if( m_inPrecursor ) {
						m_currChrom->setQ1( mz );
					}
					else if( m_inProduct ) {
						m_currChrom->setQ3( mz );
					}
				}
			}
		}
		else if( strcmp( a, "MS:1000045" ) == 0 ) {
			const char* unitId = attrs.getStringValue( "unitAccession", "" );
			std::string ce = v;
			if( strcmp( unitId, "UO:0000266" ) == 0 ) {
				ce = FMT( "%s eV", v );
			}
			if( m_currSpec == NULL ) {
				m_currSpecInfo->collisionEnergy = ce;
			}
			else {
				m_currSpec->setCollisionEnergy( ce.c_str() );
			}
		}
		else {
			// accession
			AccessionManager& accManager = AccessionManager::getInstance();
			Accession* acc = accManager.getAccession( a );

			// parent
			Accession* parent = ( acc == NULL ? NULL : acc->getOrigin() );

			// each accessions
			if( parent == m_machineAcc ) {	// machine name
				m_file.setInstrument( n );
			}
			else if( parent == m_arrAcc ) {	// array type
				// unit scale size
				const char* u = attrs.getStringValue( CV_UNIT_ATTR_NAME, "" );
				m_arrayInfo.isY = false;

				// set type
				if( strcmp( a, MZ_ARRAY_ACC ) == 0 ) {
					m_currType = TYPE_MZ;
				}
				else if( strcmp( a, RT_ARRAY_ACC ) == 0 ) {
					m_currType = TYPE_RT;
					if( strcmp( u, "UO:0000031" ) != 0 ) {	// second
						m_arrayInfo.scale = 1.0 / 60.0;
					}
				}
				else if( strcmp( a, INT_ARRAY_ACC ) == 0 ) {
					m_currType = TYPE_INT;
					m_arrayInfo.isY = true;
				}
				else {
					m_currType = TYPE_OTHER;
				}
			}
			else if( parent == m_specAcc ) {	// spectrum type
				if( m_currSpec == NULL ) {
					m_currSpecInfo->props.push_back( std::make_pair( "Spectrum Type", n ) );
				}
				else {
					m_currSpec->getProperties().setValue( "Spectrum Type", n );
				}
			}
			else if( parent == m_softwareAcc ) {	// software
				m_currSoftwareName = acc->getName();
			}
		}
	}
	else if( strcmp( name, SRC_FILE_TAG_NAME ) == 0 ) {	// <sourceFile>
		// add source file
		std::string file = attrs.getStringValue( SRC_FILE_ATTR_NAME, "" );
		std::string location = attrs.getStringValue( SRC_LOC_ATTR_NAME, "" );

		if( !location.empty() ) {
			if( location[ (int)location.length() - 1 ] != '/' ) {
				location.append( "/" );
			}
		}
		location.append( file );

		m_file.addParentFile( location.c_str() );
	}
	else if( strcmp( name, SOFTWARE_TAG_NAME ) == 0 ) {	// <software>		
		m_currSoftwareName = attrs.getStringValue( SOFTWARE_ID_ATTR_NAME, "" );
		m_currSoftwareVersion = attrs.getStringValue( SOFTWARE_VER_ATTR_NAME, "" );
	}
	else if( strcmp( name, PARAM_GROUP_TAG_NAME ) == 0 ) {	// <referenceableParamGroup>
		m_arrayInfo.name = attrs.getStringValue( PARAM_GROUP_ID_ATTR_NAME, "" );
		m_arrayInfo.bits = 32;
		m_arrayInfo.compressed = false;
		m_arrayInfo.scale = 1.0;
		m_arrayInfo.isY = false;

		m_currType = TYPE_OTHER;

		m_currSpecInfo = getSpecInfo( m_arrayInfo.name.c_str() );
	}
	else if( strcmp( name, SPEC_TAG_NAME ) == 0 ) {	// <spectrum>
		// spectrum
		const char* id = attrs.getStringValue( SPEC_ID_ATTR_NAME, "Scan" );
		m_currSpec = new MzmlSpectrum( &m_file, id );
		m_specIdMap[ id ] = m_currSpec;
		long long pos = m_pos;
		if( pos > 0 ) {
			m_currSpec->setOffset( pos );
		}

		m_file.getRootDataGroupNode()->addSpectrum( m_currSpec );

		// default attributes
		SpecInfo* dfSpec = getSpecInfo( NULL );
		setSpecInfo( m_currSpec, dfSpec );

		// set attributes
		int scanNum = attrs.getIntValue( SPEC_SCAN_NUM_ATTR_NAME, m_scanNum );
		m_scanNum++;
		m_currSpec->setScanNumber( scanNum );
		m_specScanMap[ scanNum ] = m_currSpec;

		int stage = attrs.getIntValue( SPEC_MS_LEVEL_ATTR_NAME, dfSpec->stage );
		if( stage >= 1 ) {
			m_currSpec->setMsStage( stage );
		}

		std::string	strSpotID = attrs.getStringValue( "spotID", "" );
		if  ( !strSpotID.empty( ) )
		{
			m_currSpec->setSpotId( strSpotID.c_str( ) );
		}
	}
	else if( strcmp( name, PRECURSOR_TAG_NAME ) == 0 ) {	// <precursor>
		// parent
		const char* id = attrs.getStringValue( PREC_SPEC_ATTR_NAME, NULL );
		kome::objects::Spectrum* parent = NULL;

		if( id != NULL && m_currSpec != NULL ) {
			if( m_specIdMap.find( id ) != m_specIdMap.end() ) {
				parent = m_specIdMap[ id ];
			}

			m_currSpec->setParentSpectrum( parent );
		}

		m_inPrecursor = true;
	}
	else if( strcmp( name, PRODUCT_TAG_NAME ) == 0 ) {    // <product>
		m_inProduct = true;
	}
	else if( strcmp( name, CHROM_TAG_NAME ) == 0 ) {	// <chromatogram>
		// chromatogram
		const char* id = attrs.getStringValue( CHROM_ID_ATTR_NAME, "Chromatogram" );
		m_currChrom = new MzmlChromatogram( &m_file );
		m_currChrom->setName( id );
		m_chromIdMap[ id ] = m_currChrom;

		m_file.getRootDataGroupNode()->addChromatogram( m_currChrom );
	}
	else if( strcmp( name, ION_SELECTION_TAG_NAME ) == 0 ) {	// <ionSelection>
		m_inIonTag = true;
	}
	else if( strcmp( name, USR_PARAM_TAG_NAME ) == 0 ) {	// <userParam>
		// name & value
		std::string n = attrs.getStringValue( USR_PARAM_NAME_ATTR_NAME, "" );
		std::string v = attrs.getStringValue( USR_PARAM_VAL_ATTR_NAME, "" );

		// SPEC No.86058 File Name is "Err!" Å@@date 2012.06.11 <Mod> M.Izumi ->
		// properties
		kome::core::Properties* props = &m_file.getRootDataGroupNode()->getProperties();
		if( m_currSpec != NULL ) {
			props = &m_currSpec->getProperties();
		}
		if( m_currChrom != NULL ) {
			props = &m_currChrom->getProperties();
		}

		// add
		if( !n.empty() ) {
			props->setValue( n.c_str(), v.c_str() );
		}
		// <- @date 2012.06.11 <Mod> M.Izum
	}
	else if( strcmp( name, OFFSET_TAG_NAME ) == 0 ) {	// <offset>
		m_offsetScan = attrs.getIntValue( OFFSET_SCAN_ATTR_NAME, -1 );
		m_offsetId = attrs.getStringValue( OFFSET_IDREF_ATTR_NAME, "" );
	}
	else if( strcmp( name, REF_TAG_NAME ) == 0 ) {    // <referenceableParamGroupRef>
		std::string id = attrs.getStringValue( REF_REF_ATTR_NAME, "" );

		if( m_currSpec != NULL ) {
			SpecInfo* info = getSpecInfo( id.c_str() );

			setSpecInfo( m_currSpec, info );
		}
	}
}

// end element
void MzmlHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, SOFTWARE_TAG_NAME ) == 0 ) {	// </software>
		if( strlen( m_file.getSoftwareName() ) == 0 ) {
			m_file.setSoftwareName( m_currSoftwareName.c_str() );
			m_file.setSoftwareVersion( m_currSoftwareVersion.c_str() );
		}
		m_currSoftwareName.clear();
		m_currSoftwareVersion.clear();
	}
	else if( strcmp( name, SPEC_TAG_NAME ) == 0 ) {	// </spectrum>
		m_currSpec = NULL;
	}
	else if( strcmp( name, CHROM_TAG_NAME ) == 0 ) {	// </chromatogram>
		if( m_currChrom != NULL ) {
			double q1 = m_currChrom->getQ1();
			double q3 = m_currChrom->getQ3();

			if( q1 >= 0.0 && q3 >= 0.0 ) {
				m_currChrom->setIcon( "MRM_chrom" );
			}
		}
		m_currChrom = NULL;
	}
	else if( strcmp( name, PARAM_GROUP_TAG_NAME ) == 0 ) {	// </referenceableParamGroup>
		if( m_currType != TYPE_OTHER ) {
			m_file.addArrayInfo(
				m_arrayInfo.name.c_str(),
				m_arrayInfo.bits,
				m_arrayInfo.compressed,
				m_arrayInfo.scale, m_arrayInfo.isY
			);
		}

		m_currSpecInfo = getSpecInfo( NULL );
	}
	else if( strcmp( name, ION_SELECTION_TAG_NAME ) == 0 ) {	// </ionSelection>
		m_inIonTag = false;
	}
	else if( strcmp( name, PRECURSOR_TAG_NAME ) == 0 ) {    // </precursor>
		m_inPrecursor = false;
	}
	else if( strcmp( name, PRODUCT_TAG_NAME ) == 0 ) {    // </product>
		m_inProduct = false;
	}
	else if( strcmp( name, OFFSET_TAG_NAME ) == 0 ) {	// </offset>
		// offset
		long long offset = toint64( text, 10, -1 );

		// get object
		MzmlSpectrum* spec = NULL;
		MzmlChromatogram* chrom = NULL;

		if( m_offsetScan >= 0 && m_specScanMap.find( m_offsetScan ) != m_specScanMap.end() ) {
			spec = m_specScanMap[ m_offsetScan ];
		}
		if( m_specIdMap.find( m_offsetId ) != m_specIdMap.end() ) {
			spec = m_specIdMap[ m_offsetId ];
		}
		if( m_chromIdMap.find( m_offsetId ) != m_chromIdMap.end() ) {
			chrom = m_chromIdMap[ m_offsetId ];
		}

		// set offset
		if( spec != NULL ) {
			spec->setOffset( offset );
		}
		if( chrom != NULL ) {
			chrom->setOffset( offset );
		}

		// initialize offset values
		m_offsetScan = -1;
		m_offsetId.clear();
	}

	// position
	m_pos = getPosition();
}
