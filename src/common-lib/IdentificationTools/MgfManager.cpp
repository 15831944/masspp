/**
 * @file MgfManager.cpp
 * @brief implements of MgfManager class
 *
 * @author S.Tanaka
 * @date 2008.10.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"
#include "SearchResult.h"
#include "IdentificationManager.h"
#include "MgfManager.h"

#include <list>
#include <map>
#include <set>
#include <algorithm>

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MgfManager::MgfManager() {
}

// destructor
MgfManager::~MgfManager() {
} 

// write MIS Header
void MgfManager::writeMisHeader( FILE* fp ) {
	// header
	fprintf( fp, "SEARCH=MIS\n" );
	fprintf( fp, "REPTYPE=Peptide\n" );
}

// write MIS peaks
void MgfManager::writeMisPeaks( FILE* fp, SpecInfo* spec, kome::objects::Peaks* peaks ) {
	bool bOversizePeakListFound = false;

	// Check that no peak list contains > 10000 peaks.	
	if (peaks->getLength() > 10000)
	{
		bOversizePeakListFound = true;
	}
	
	if (bOversizePeakListFound) {
		LOG_ERROR_CODE( FMT( "Cannot create an MGF file if any spectrum contains > 10000 peaks."), ERR_FILE_WRITE_FAILED );
		return;
	}

	// MS spectra information must not be output to an MGF file.
	// Following assert macro always stop DEBUG running. We must remove upstream bug or error otherwise ... . Morimoto.
	// _ASSERT(spec->stage >= 2);

	// precursor
	peaks->setPrecursor( spec->precursor );

	// set title
	std::string title;
	
	// write
	for( unsigned int j = 0; j < peaks->getNumberOfCharges(); j++ ) {
		// charge
		int charge = peaks->getCharge( j );

		// title
		std::string title = getSpectrumTitle( spec, charge );
		
		// begin ions
		fprintf( fp, "\n" );
		fprintf( fp, "BEGIN IONS\n" );
		fprintf( fp, "PEPMASS=%f\n", peaks->getPrecursor() );
		fprintf( fp, "CHARGE=%d", charge );
		if( spec->polarity == kome::objects::Spectrum::POLARITY_POSITIVE ) {
			fprintf( fp, "+" );
		}
		else if( spec->polarity == kome::objects::Spectrum::POLARITY_NEGATIVE ) {
			fprintf( fp, "-" );
		}
		fprintf( fp, "\n" );
		fprintf( fp, "TITLE=%s\n", title.c_str() );
		
		// write xy data
		for( unsigned int k = 0; k < peaks->getLength(); k++ ) {
			fprintf( fp, "%lf %lf\n", peaks->getX( k ), peaks->getY( k ) );
		}

		// end ions
		fprintf( fp, "END IONS\n" );
	}
}

// get spectrum title
std::string MgfManager::getSpectrumTitle( SpecInfo* spec, const int charge ) {
	// polarity
	std::string polarity = "n/a";
	if( spec->polarity == kome::objects::Spectrum::POLARITY_POSITIVE ) {
		polarity = "+";
	}
	else if( spec->polarity == kome::objects::Spectrum::POLARITY_NEGATIVE ) {
		polarity = "-";
	}

	// title
	std::string title;
	if (spec->peakIds.size() > 0 ) {
		std::string peakIdStr;
		for( unsigned int i = 0; i < spec->peakIds.size(); i++ ) {
			if( !peakIdStr.empty() ) {
				peakIdStr.append( "+" );
			}
			peakIdStr.append( FMT( "%d", spec->peakIds.at( i ) ) );
		}
		
		title = FMT(
			"spec_id: %d, sample_index: %d, sample_name: %s, file_path: %s, spec_rt: %f, spec_prec: %lf, spec_stage: %d, polarity: %s, charge: %d, peak_id: %s --- %s",
			spec->specId,
			spec->sampleIndex,
			spec->sampleName.c_str(),
			spec->path.c_str(),
			spec->retentionTime,
			spec->precursor,
			spec->stage,
			polarity.c_str(),
			charge,
			peakIdStr.c_str(),
			spec->name.c_str()
		);
	}
	else {
		title = FMT(
			"spec_id: %d, sample_index: %d, sample_name: %s, file_path: %s, spec_rt: %f, spec_prec: %f, spec_stage: %d, polarity: %s, charge: %d --- %s",
			spec->specId,
			spec->sampleIndex,
			spec->sampleName.c_str(),
			spec->path.c_str(),
			spec->retentionTime,
			spec->precursor,
			spec->stage,
			polarity.c_str(),
			charge,
			spec->name.c_str()
		);
	}

	return title;
}

// write the specified peak list to a PMF file at the specified path
bool MgfManager::writePmf( const char* path, kome::objects::Peaks* const peakList ) {
	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector." ) );
		return false;
	}

	// write
	for( unsigned int i = 0; i < peakList->getLength(); i++ ) {
		fprintf( fp, "%f %f\n", peakList->getX( i ), peakList->getY( i ) );
	}

	fflush( fp );
	fclose( fp );

	return false;
}

// get instance
MgfManager& MgfManager::getInstance() {
	// create object (This is the only object.)
	static MgfManager mgr;

	return mgr;
}
