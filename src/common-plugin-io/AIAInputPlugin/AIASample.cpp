/**
 * @file AIASample.cpp
 * @brief implementation of AIASample class
 *
 * @author fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "AIASample.h"
#include "AIASampleSet.h"
#include "AIASpectrum.h"

#include <netcdf.h>
#include <string.h>


using namespace kome::io::aia;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//#define INSTRUMENT_NAME	 "GCMS"
//#define SOFTWARE_NAME	   "GCMS solution"


// constructor
AIASample::AIASample( AIASampleSet* sampleSet ) : kome::objects::Sample( sampleSet ) {
	m_ncid = -1;
	m_sampleSet = sampleSet;
}

// destructor
AIASample::~AIASample() {
}

// get GCMS sample set
AIASampleSet* AIASample::getAIASampleSet() {
	return m_sampleSet;
}

// get NetCDF ID
int AIASample::getCdfId() {
	return m_ncid;
}

// set properties
void AIASample::setProperties() {
	// buffer
	char buff[ 2048 ];

	// properties
	kome::core::Properties& props = getRootDataGroupNode()->getProperties();

	// get properties
	const char* attrs[] = {
		"dataset_completeness",
		"ms_template_revision",
		"netcdf_revision",
		"languages",
		"administrative_comments",
		"netcdf_file_date_time_stamp",
		"experiment_title",
		"experiment_date_time_stamp",
		"operator_name",
		"source_file_reference",
		"source_file_format",
		"source_file_date_time_stamp",
		"experiment_type",
		"sample_state",
		"test_separation_type",
		"test_ms_inlet",
		"test_ionization_mode",
		"test_ionization_polarity",
		"test_electron_energy",
		"test_detector_type",
		"test_resolution_type",
		"test_scan_function",
		"test_scan_direction",
		"test_scan_law",
		"test_scan_time",
		"raw_data_mass_format",
		"raw_data_time_format",
		"raw_data_intensity_format",
		"units",
		"scale_factor",
		"long_name",
		"starting_scan_number"
		"actual_run_time_length",
		"actual_delay_time",
		"raw_data_uniform_sampling_flag"
	};

	const char* keys[] = {
		"Dataset Completeness",
		"MS Template Revision",
		"NetCDF Revision",
		"Languages",
		"Administrative Comments",
		"NetCDF File Date Time Stamp",
		"Experiment Title",
		"Experiment Date Time Stamp",
		"Operator Name",
		"Source File Reference",
		"Source File Format",
		"Source File Date Time Stamp",
		"Experiment Type",
		"Sample State",
		"Test Separation Type",
		"Test MS Inlet",
		"Test Ionization Mode",
		"Test Ionization Polarity",
		"Test Electron Energy",
		"Test Detector Type",
		"Test Resolution Type",
		"Test Scan Function",
		"Test Scan Direction",
		"Test Scan Law",
		"Test Scan Time",
		"Raw Data Mass Format",
		"Raw Data Time Format",
		"Raw Data Intensity Format",
		"Units",
		"Scale Factor",
		"Long Name",
		"Starting Scan Number"
		"Actual Run Time Length",
		"Actual Delay Time",
		"Raw Data Uniform Sampling Flag"
	};

	// get attributes
	int num = sizeof( attrs ) / sizeof( const char* );
	for( int i = 0; i < num; i++ ) {
		int status = nc_get_att_text( m_ncid, NC_GLOBAL, attrs[ i ], buff );
		if( status == NC_NOERR ) {
			props.setValue( keys[ i ], buff );
		}
	}

	// log error
	int varId = 0;
	int status = nc_inq_varid( m_ncid, "error_log", &varId );
	if( status == NC_NOERR ) {
		status = nc_get_var_text( m_ncid, varId, buff );
	}
	if( status == NC_NOERR ) {
		props.setValue( "Error Log", buff );
	}
}

// set instrument properties
void AIASample::setInstrumentProps() {
	// buffer
	char buff[ 32 ];

	// properties
	const char* names[] = {
		"instrument_name",
		"instrument_id",
		"instrument_mfr",
		"instrument_model",
		"instrument_serial_no",
		"instrument_sw_version",
		"instrument_fw_version",
		"instrument_os_version",
		"instrument_app_version",
		"instrument_comments"
	};

	const char* keys[] = {
		"Instrument Name",
		"Instrument ID",
		"Instrument MFR",
		"Instrument Model",
		"Instrument Serial Number",
		"Instrument SW Version",
		"Instrument FW Version",
		"Instrument OS Version",
		"Instrument App Version",
		"Instrument Comments"
	};

	// get properties
	kome::core::Properties& props = getRootDataGroupNode()->getProperties();
	int num = sizeof( names ) / sizeof( const char* );
	for( int i = 0; i < num; i++ ) {
		// ID
		int varId = 0;
		int status = nc_inq_varid( m_ncid, names[ i ], &varId );

		// get value
		if( status == NC_NOERR ) {
			size_t start[] = { 0, 0 };
			size_t count[] = { 1, 32 };

			status = nc_get_vara_text( m_ncid, varId, start, count, buff );

			// set value
			if( status == NC_NOERR ) {
				buff[ 31 ] = '\0';
				std::string val = trimstring( buff );

				props.setValue( keys[ i ], val.c_str() );
				}
		}
	}

	// instrument name
//	setInstrument( INSTRUMENT_NAME );

	// software
//	setSoftwareName( SOFTWARE_NAME );
//	setSoftwareVersion( props.getStringValue( "Instrument SW Version", "" ) );
}

// open file
bool AIASample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// open
	int status = nc_open( m_sampleSet->getCdfFilePath(), NC_NOWRITE, &m_ncid );
	if( status != NC_NOERR ) {
		AIASampleSet::error( status );
		return false;
	}

	// set properties
	setProperties();
	setInstrumentProps();

	// scan number
	size_t scans = 0;
	int scanId = 0;
	status = nc_inq_dimid( m_ncid, "scan_number", &scanId );
	if( status != NC_NOERR ) {
		AIASampleSet::error( status );
		return false;
	}
	status = nc_inq_dimlen( m_ncid, scanId, &scans );
	if( status != NC_NOERR ) {
		AIASampleSet::error( status );
		return false;
	}
	LOG_DEBUG( FMT( "%d scans.", scans ) );
	if( scans == 0 ) {
		return true;
	}

	// definitions
	union Array {   // array
		double* dbl;
		short* sh;
		long* l;
	};

	typedef enum {  // data type
		TYPE_DOUBLE,
		TYPE_SHORT,
		TYPE_LONG
	} DataType;

	struct Value {  // value
		std::string name;
		DataType type;
		Array arr;
	};

	// get values
	size_t start[] = { 0 };
	size_t count[] = { scans };

	Value vals[] = {
		{ "a_d_sampling_rate", TYPE_DOUBLE, NULL },			//  0: sampling rate
		{ "a_d_coaddition_factor", TYPE_SHORT, NULL },		//  1: coaddition factor
		{ "scan_acquisition_time", TYPE_DOUBLE, NULL },		//  2: acquisition time
		{ "scan_duration", TYPE_DOUBLE, NULL },				//  3: duration
		{ "inter_scan_time", TYPE_DOUBLE, NULL },			//  4: inter scan time
		{ "resolution", TYPE_DOUBLE, NULL },				//  5: resolution
		{ "actual_scan_number", TYPE_LONG, NULL },			//  6: scan number
		{ "total_intensity", TYPE_DOUBLE, NULL },			//  7: total intensity
		{ "mass_range_min", TYPE_DOUBLE, NULL },			//  8: min mass
		{ "mass_range_max", TYPE_DOUBLE, NULL },			//  9: max mass
		{ "time_range_min", TYPE_DOUBLE, NULL },			// 10: min time
		{ "time_range_max", TYPE_DOUBLE, NULL },			// 11: max time
		{ "scan_index", TYPE_LONG, NULL },					// 12: scan index
		{ "point_count", TYPE_LONG, NULL },				    // 13: point count
		{ "flag_count", TYPE_LONG, NULL },					// 14: flag count
	};

	int num = sizeof( vals ) / sizeof( Value );

	for( int i = 0; i < num; i++ ) {
		// id
		int varId = 0;
		int status = nc_inq_varid( m_ncid, vals[ i ].name.c_str(), &varId );

		// get values
		if( status == NC_NOERR ) {
			if( vals[ i ].type == TYPE_DOUBLE ) {		// double
				vals[ i ].arr.dbl = new double[ scans ];
				int status_dbl = nc_get_vara_double( m_ncid, varId, start, count, vals[ i ].arr.dbl );
				int j = 1;
			}
			else if( vals[ i ].type == TYPE_LONG ) {		// long
				vals[ i ].arr.l = new long[ scans ];
				int status_l = nc_get_vara_long( m_ncid, varId, start, count, vals[ i ].arr.l );
				long jjjj = *vals[ i ].arr.l;
				int jj = 1;
			}
			else if( vals[ i ].type == TYPE_SHORT ) {		// short
				vals[ i ].arr.sh = new short[ scans ];
				int status_sh = nc_get_vara_short( m_ncid, varId, start, count, vals[ i ].arr.sh );
				int jjj = 1;
			}
		}
		else {
			fillzero( &( vals[ i ].arr ), sizeof( vals[ i ].arr ) );
		}
	}

	// create spectra
	kome::objects::DataGroupNode* group = getRootDataGroupNode();
	// buffer
	char buff[ 2048 ];
	char* attribute = "experiment_type";
	for( unsigned int i = 0; i < scans; i++ ) {
		// spectrum
		AIASpectrum* spec;
//		AIASpectrum* spec = new AIASpectrum( this, ( vals[ 6 ].arr.l == NULL ? i : ( vals[ 6 ].arr.l )[ i ] ) );  // @date 2013/03/21 <Del> FUJITA  
		if ( vals[ 6 ].arr.l == NULL || (vals[ 6 ].arr.l)[ i ] < 0) {		                                      // @date 2013/03/21 <Add> FUJITA  
			spec = new AIASpectrum( this, i );   
		} else {
			spec = new AIASpectrum( this, ( vals[ 6 ].arr.l )[ i ] );
		}

		// properties
		spec->setHasChromatogram( vals[ 2 ].arr.dbl != NULL );
		if( vals[ 2 ].arr.dbl != NULL ) {
			spec->setRt( vals[ 2 ].arr.dbl[ i ] / 60.0 );
		}
		if( vals[ 8 ].arr.dbl != NULL && vals[ 9 ].arr.dbl != NULL ) {
			spec->setXRange( vals[ 8 ].arr.dbl[ i ], vals[ 9 ].arr.dbl[ i ] );
		}
		if( vals[ 7 ].arr.dbl != NULL ) {
			spec->setTotalIntensity( vals[ 7 ].arr.dbl[ i ] );
		}
		if( vals[ 12 ].arr.l != NULL && vals[ 13 ].arr.l != NULL ) {
			spec->setXYInfo( vals[ 12 ].arr.l[ i ], vals[ 13 ].arr.l[ i ] );
		}
		// set Experiment Type														 // @date 2012/12/20 <Add> FUJITA            
		int status = nc_get_att_text( m_ncid, NC_GLOBAL, attribute, buff );
		if( status == NC_NOERR && 0 == strcmp(buff, "Centroided Mass Spectrum")) {
			spec->setCentroidMode( true );
		}
		else{
			spec->setCentroidMode( false );
		}

		spec->setMsStage( 1 );

		// properties
		kome::core::Properties& props = spec->getProperties();

		if( vals[ 0 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Sampling Rate", vals[ 0 ].arr.dbl[ i ] );
		}
		if( vals[ 1 ].arr.sh != NULL ) {
			props.setIntValue( "Coaddition Factor", vals[ 1 ].arr.sh[ i ] );
		}
		if( vals[ 3 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Scan Duration", vals[ 3 ].arr.dbl[ i ] );
		}
		if( vals[ 4 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Inter Scan Time", vals[ 4 ].arr.dbl[ i ] );
		}
		if( vals[ 5 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Scan Resolution", vals[ 5 ].arr.dbl[ i ] );
		}
		if( vals[ 10 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Min Time", vals[ 10 ].arr.dbl[ i ] );
		}
		if( vals[ 11 ].arr.dbl != NULL ) {
			props.setDoubleValue( "Max Time", vals[ 11 ].arr.dbl[ i ] );
		}
		if( vals[ 13 ].arr.l != NULL ) {
			props.setIntValue( "Flag Count", vals[ 13 ].arr.l[ i ] );
		}

		// title
		int j = spec->getScanNumber();    
		std::string title = FMT( "scan %d  rt=%.2f", spec->getScanNumber(), spec->getRt() );
		spec->setTitle( title.c_str() );

		// add
		group->addSpectrum( spec );
	}

	// delete
	for( int i = 0; i < num; i++ ) {
		if( vals[ i ].type == TYPE_DOUBLE && vals[ i ].arr.dbl != NULL ) {		// double
			delete[] vals[ i ].arr.dbl;
		}
		else if( vals[ i ].type == TYPE_LONG && vals[ i ].arr.l != NULL ) {		// long
			delete[] vals[ i ].arr.l;
		}
		else if( vals[ i ].type == TYPE_SHORT && vals[ i ].arr.sh != NULL ) {		// short
			delete[] vals[ i ].arr.sh;
		}
	}

	return true;
}

// close file
bool AIASample::onCloseSample() {
	// close file
	if( m_ncid >= 0 ) {
		nc_close( m_ncid );
	}
	return true;
}
