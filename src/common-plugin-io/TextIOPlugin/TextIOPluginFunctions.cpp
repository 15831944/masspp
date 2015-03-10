/**
 * @file TextIOPluginFunctions.cpp
 * @brief implements of TextIO plug-in function
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

  
#include "stdafx.h"
#include "TextIOPluginFunctions.h"
#include "TextSampleSet.h"

#define DATA_STATUS		 "data_status"

  
using namespace kome::io::text;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// save file
kome::objects::Variant openText( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::SAMPLE_SET;
	ret.prim.pt = NULL;
	  
	// gets parameters
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	ret.prim.pt = new TextSampleSet();

	return ret;
}

// save text file
kome::objects::Variant saveText( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// gets parameters
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file for writing. Check file / folder permissions. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return ret;
	}

	// progress
	unsigned int num = dataSet->getNumberOfSpectra();
	if( num == 0 ) {
		LOG_ERROR_CODE( FMT( "The file cannot be saved because there are no spectra. Open a data file containing spectra." ), ERR_OTHER );
		return ret;
	}
	progress->setRange( 0, num );
	progress->setPosition( 0 );
	   
	// save
	for( unsigned int i = 0; i < num && !progress->isStopped(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );

		std::string msg = FMT( "Writing Spectrum: %s [%d/%d]", spec->getName(), ( i + 1 ), num );
		progress->setStatus( msg.c_str() );

		kome::core::DataPoints dps;

		// get values on child dialog
		kome::objects::SettingParameterValues* saveSettings = kome::plugin::PluginCallTool::getSettingValues( params );  // @date 2012/08/20 <Add> FUJITA
        // get export data status
		bool op = saveSettings->getBoolValue( DATA_STATUS,false );			// @date 2012/08/20 <Add> FUJITA

		//false == op ...raw data  true == op ...processed data    
		spec->getXYData( &dps, op );										// @date 2012/08/21 <Add> FUJITA

		// write
		fprintf( fp, "# %s", spec->getName() );
		if( spec->getRt() >= 0.0 ) {
			fprintf( fp, " (rt=%f)", spec->getRt() );
		}
		fprintf( fp, "\n" );
		for( unsigned int j = 0; j < dps.getLength(); j++ ) {
			const double x = dps.getX( j );
			const double y = dps.getY( j );
			if( y > 0.0 ) {
				fprintf( fp, "%f\t%f\n", x, y );
			}
		}
		fprintf( fp, "\n" );

		// flush
		fflush( fp );
		progress->setPosition( i + 1 );
	}

	// close
	fclose( fp );

	ret.prim.boolVal = true;
	return ret;
}

