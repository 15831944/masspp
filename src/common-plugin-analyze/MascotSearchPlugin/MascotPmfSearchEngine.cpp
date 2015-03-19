/**
 * @file MascotPmfSearchEngine.cpp
 * @brief implements of MascotPmfSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MascotPmfSearchEngine.h"   
#include "MascotManager.h"


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MascotPmfSearchEngine::MascotPmfSearchEngine() : MascotSearchEngineBase( "Mascot PMF", "PMF", false ) {
}

// destructor
MascotPmfSearchEngine::~MascotPmfSearchEngine() {
}

// judges the specified spectrum is valied or not
bool MascotPmfSearchEngine::isValidSpectrum( kome::objects::Spectrum* spec ) {
	if( spec == NULL ) {
		return false;
	}

	return ( spec->getMsStage() == 1 );
}

// on spectrum peaks
void MascotPmfSearchEngine::onSpectrumPeaks(
		kome::objects::Spectrum* spec,
		kome::ident::SpecInfo* specInfo,
		kome::objects::Peaks* peaks,
		int* peakIds,
		const unsigned int peakIdsSize
) {
	// manager
	kome::ident::MgfManager& mgfMgr = kome::ident::MgfManager::getInstance();

	// check the peak
	if( peaks == NULL || peaks->getLength() == 0 ) {
		return;
	}

	// file
	std::string peaksFile = FMT( "%lld.pmf", (long long)specInfo );
	std::string peaksPath = getpath( m_peaksDir.c_str(), peaksFile.c_str() );
	m_peaksMap[ specInfo ] = peaksPath;

	// save the peak list
	mgfMgr.writePmf( peaksPath.c_str(), peaks );
}

// on finish peak detection
bool MascotPmfSearchEngine::onFinishPeakDetection() {
	// nothing to do
	return true;
}


// on search
std::string MascotPmfSearchEngine::onSearch(
		const char* title,
		const char* comment,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// return value
	std::string ret;

	// manager
	MascotManager& mgr = MascotManager::getInstance();

	// progress
	if( specNum == 0 ) {
		LOG_WARN( FMT( "There are no spectra." ) );
		return ret;
	}

// >>>>>>	@Date:2013/09/07	<Deleted>	A.Ozaki
//  Process has been completed, the progress dialog does not close automatically
//
//	progress.createSubProgresses( 2 );
//
// <<<<<<	@Date:2013/09/07	<Deleted>	A.Ozaki

	progress.setStatus( "Identifying with PMF Search..." );

	// peak files
	std::vector< std::string > peakFiles;
	for( unsigned int i = 0; i < specNum; i++ ) {
		kome::ident::SpecInfo* spec = spectra[ i ];
		if( m_peaksMap.find( spec ) != m_peaksMap.end() ) {
			std::string path = m_peaksMap[ spec ];
			peakFiles.push_back( path );
		}
	}

	// search
	ret = performSearch(
			peakFiles,
			title,
			settings,
			progress
	);

	return ret;
}

// prepares peak detection
bool MascotPmfSearchEngine::onPreparePeakDetection(
		kome::objects::SettingParameterValues* searchSettings,
		const char* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		const char* chargeDetector,
		kome::objects::SettingParameterValues* chargeSettings,
		const char* peakFilter
) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// clear the map
	m_peaksMap.clear();

	// temporary directory
	std::string tmpDir = msppMgr.getTmpDir();
	std::string mascotDir = getpath( tmpDir.c_str(), "mascot" );
	if( !fileexists( mascotDir.c_str() ) ) {
		makedirectory( mascotDir.c_str() );
	}
	std::string pmfDir = getpath( mascotDir.c_str(), "pmf" );
	if( !fileexists( pmfDir.c_str() ) ) {
		makedirectory( pmfDir.c_str() );
	}

	std::string peaksDir = getpath( pmfDir.c_str(), FMT( "%lld", getcurrenttime() ).c_str() );
	if( !fileexists( peaksDir.c_str() ) ) {
		makedirectory( peaksDir.c_str() );
	}

	m_peaksDir = peaksDir;
	return true;
}

// on end search
void MascotPmfSearchEngine::onEndSearch() {
	m_peaksMap.clear();
	m_peaksDir.clear();
}
