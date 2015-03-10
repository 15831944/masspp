/**
 * @file PeakWaveformManager.cpp
 * @brief interfaces of PeakWaveform plug-in manager
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakWaveformManager.h"
#include "PeakWaveformSpectrum.h"
#include "PeakWaveformChromatogram.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakWaveformManager::PeakWaveformManager(){
	m_spectra.clear();
	m_chromatograms.clear();
}

//destructor
PeakWaveformManager::~PeakWaveformManager(){
	for( unsigned int i=0; i < m_spectra.size(); i++ ){
		delete m_spectra[i];
	}
	m_spectra.clear();

	for( unsigned int i=0; i < m_chromatograms.size(); i++ ){

		delete m_chromatograms[i];
	}
	m_chromatograms.clear();
}

// open peak waveform spectrum
bool PeakWaveformManager::openPeakWaveformSpectrum( kome::objects::Spectrum* spec ){
	if( spec == NULL ){
		LOG_ERROR( FMT("Failed to get a spectrum." ) );
		return false;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* p = pkMgr.getPeaks( spec );
	int len = ( p != NULL ? p->getLength() : 0 );
	if( !pkMgr.hasPeaks( spec ) || len == 0 ){
		kome::window::WindowTool::showInfo("Peaks of specified spectrum are not found."
			"If you want to display peaks, detect peaks by [Processing] > [Peaks] > [Detect Peaks].");
		return false;
	}

	PeakWaveformSpectrum* peakSpec = new PeakWaveformSpectrum( *spec );
	if( peakSpec != NULL ){
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *peakSpec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *peakSpec );
		}
	}
	m_spectra.push_back( peakSpec );
	return true;
}

// open peak waveform chromatogram
bool PeakWaveformManager::openPeakWaveformChromatogram( kome::objects::Chromatogram* chrom ){
	if( chrom == NULL ){
		LOG_ERROR( FMT("Failed to get a chromatogram.") );
		return false;
	}
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* p = pkMgr.getPeaks( chrom );
	int len = ( p != NULL ? p->getLength() : 0 );
	if( !pkMgr.hasPeaks( chrom ) || len == 0 ){
		kome::window::WindowTool::showInfo( "Peaks of specified chromatogram are not found."
			 "If you want to display peaks, detect peaks by [Processing] > [Peaks] > [Detect Peaks].");
		return false;
	}

	PeakWaveformChromatogram* peakChrom = new PeakWaveformChromatogram( *chrom );
	if( peakChrom != NULL ){
		if( kome::plugin::PluginCallTool::onOpenChromatogram( *peakChrom ) ){
			kome::plugin::PluginCallTool::openChromatogram( *peakChrom );
		}
	}

	m_chromatograms.push_back( peakChrom );
	return true;
}

// get instance
PeakWaveformManager& PeakWaveformManager::getInstance() {
	// create object (This is the only object.)
	static PeakWaveformManager mgr;

	return mgr;
}
