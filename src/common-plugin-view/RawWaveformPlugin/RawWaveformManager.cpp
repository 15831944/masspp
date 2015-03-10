/**
 * @file RawWaveformManager.cpp
 * @brief interfaces of RawWaveform plug-in manager
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "RawWaveformManager.h"
#include "RawWaveformSpectrum.h"
#include "RawWaveformChromatogram.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
RawWaveformManager::RawWaveformManager(){
	m_spectra.clear();
	m_chromatograms.clear();
}

//destructor
RawWaveformManager::~RawWaveformManager(){
	for( unsigned int i=0; i < m_spectra.size(); i++ ){
		delete m_spectra[i];
	}
	m_spectra.clear();

	for( unsigned int i=0; i < m_chromatograms.size(); i++ ){

		delete m_chromatograms[i];
	}
	m_chromatograms.clear();
}

// open raw waveform spectrum
bool RawWaveformManager::openRawWaveformSpectrum( kome::objects::Spectrum* spec ){
	if( spec == NULL ){
		LOG_ERROR( FMT("Failed to get a spectrum." ) );
		return false;
	}

	RawWaveformSpectrum* rawSpec = new RawWaveformSpectrum( *spec );
	if( rawSpec != NULL ){
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *rawSpec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *rawSpec );
		}
	}
	m_spectra.push_back( rawSpec );
	return true;
}

// open raw waveform chromatogram
bool RawWaveformManager::openRawWaveformChromatogram( kome::objects::Chromatogram* chrom ){
	if( chrom == NULL ){
		LOG_ERROR( FMT("Failed to get a chromatogram." ) );
		return false;
	}

	RawWaveformChromatogram* rawChrom = new RawWaveformChromatogram( *chrom );
	if( rawChrom != NULL ){
		if( kome::plugin::PluginCallTool::onOpenChromatogram( *rawChrom ) ){
			kome::plugin::PluginCallTool::openChromatogram( *rawChrom );
		}
	}
	m_chromatograms.push_back( rawChrom );
	return true;	
}


// get instance
RawWaveformManager& RawWaveformManager::getInstance() {
	// create object (This is the only object.)
	static RawWaveformManager mgr;

	return mgr;
}
