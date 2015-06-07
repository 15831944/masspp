/**
 * @file MascotPmfForLCMSSearchEngine.cpp
 * @brief implements of MascotPmfForLCMSSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MascotPmfForLCMSSearchEngine.h"    
#include "MascotManager.h"


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MascotPmfForLCMSSearchEngine::MascotPmfForLCMSSearchEngine() : MascotSearchEngineBase( "Mascot PMF (for LC)", "PMF", false ) {
	m_peakTol = 0.25;
	m_peakNum = 999;
	m_peakTolUnit = TOL_DA;
	m_protein = NULL;
}

// destructor
MascotPmfForLCMSSearchEngine::~MascotPmfForLCMSSearchEngine() {
}

// on hit
void MascotPmfForLCMSSearchEngine::onHit( kome::ident::Hit* hit ) {
	// check the parameterr
	if( hit == NULL ) {
		return;
	}

	// protein
	kome::ident::Substance* peptide = hit->getSubstance();
	if( peptide == NULL ) {
		return;
	}

	kome::ident::Substance* protein = peptide->getParentSubstance();
	if( protein == NULL ) {
		return;
	}

	// spectra
	if( m_protein != protein ) {
		m_spectra.clear();

		// excluded peaks
		std::string unmatched = hit->getValueInfo( "Unmatched" )->value;
		std::vector< std::string > tokens;
		stringtoken( unmatched.c_str(), " ,\t\r\n:", tokens );
		std::vector< double > unmatchedPeaks;
		for( unsigned int i = 0; i < tokens.size(); i++ ) {
			double peakPos = -1.0;
			if( isdouble( tokens[ i ].c_str(), &peakPos ) ) {
				if( peakPos >= 0.0 ) {
					unmatchedPeaks.push_back( peakPos );
				}
			}
		}

		std::set< PeakInfo* > excluded;
		int startIdx = 0;
		for( unsigned int i = 0; i < unmatchedPeaks.size(); i++ ) {
			double diff = FLT_MAX;
			PeakInfo* currentPeak = NULL;
			bool loop = true;
			for( int j = startIdx; j < (int)m_pmfPeaks.size() && loop; j++ )  {
				const double tmpDiff = fabs( m_pmfPeaks[ j ].x - unmatchedPeaks[ i ] );
				if( tmpDiff < diff ) {
					diff = tmpDiff;
					currentPeak = &( m_pmfPeaks[ j ] );
					startIdx = j;
				}
				else {
					loop = false;
				}
			}

			excluded.insert( currentPeak );
		}

		// spectra
		std::set< kome::ident::SpecInfo* > specSet;
		for( unsigned int i = 0; i < m_pmfPeaks.size(); i++ ) {
			PeakInfo* pk = &( m_pmfPeaks[ i ] );

			if( excluded.find( pk ) == excluded.end() ) {
				kome::ident::SpecInfo* spec = pk->spec;

				if( specSet.find( spec ) == specSet.end() ) {
					specSet.insert( spec );
					m_spectra.push_back( spec );
				}
			}
		}
	}

	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		hit->addSpectrum( m_spectra[ i ] );
	}
}

// judges the specified spectrum is valied or not
bool MascotPmfForLCMSSearchEngine::isValidSpectrum( kome::objects::Spectrum* spec ) {
	if( spec == NULL ) {
		return false;
	}

	return ( spec->getMsStage() == 1 );
}

// on spectrum peaks
void MascotPmfForLCMSSearchEngine::onSpectrumPeaks(
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

	// add peaks
	for( unsigned int i = 0; i < peaks->getLength(); i++ ) {
		PeakInfo peak;
		peak.x = peaks->getX( i );
		peak.y = peaks->getY( i );
		peak.maxY = peak.y;
		peak.spec = specInfo;

		m_pmfPeaks.push_back( peak );
	}
}

// on finish peak detection
bool MascotPmfForLCMSSearchEngine::onFinishPeakDetection() {
	// check the peak
	if( m_pmfPeaks.size() == 0 ) {
		LOG_ERROR( FMT( "PMF Error: No peaks are detected." ) );
		return false;
	}

	// copy
	std::vector< PeakInfo > peaks;
	for( unsigned int i = 0; i < m_pmfPeaks.size(); i++ ) {
		peaks.push_back( m_pmfPeaks[ i ] );
	}

	// sort
	std::sort( peaks.begin(), peaks.end(), lessPeakX );

	// merge
	m_pmfPeaks.clear();
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		// peak
		PeakInfo& tmp = peaks[ i ];

		// merge
		bool flg = true;
		if( m_pmfPeaks.size() > 0 ) {
			PeakInfo& peak = m_pmfPeaks.back();

			double tol = m_peakTol;
			if( m_peakTolUnit == TOL_PPM ) {
				tol = peak.x * tol / 1000000.0;
			}

			if( tmp.x <= peak.x + tol ) {
				flg = false;

				if( peak.maxY > tmp.maxY ) {
					peak.maxY = tmp.maxY;
					peak.spec = tmp.spec;
				}

				double y = peak.y + tmp.y;
				double x = peak.x * peak.y + tmp.x * tmp.y;
				if( y > 0.0 ) {
					peak.x = x / y;
					peak.y = y;
				}
			}
		}

		// add
		if( flg ) {
			m_pmfPeaks.push_back( tmp );
		}		
	}

	// remove
	if( m_peakNum > 0 ) {
		std::sort( m_pmfPeaks.begin(), m_pmfPeaks.end(), lessPeakY );
		while( m_pmfPeaks.size() > (unsigned int)m_peakNum ) {
			m_pmfPeaks.pop_back();
		}

		std::sort( m_pmfPeaks.begin(), m_pmfPeaks.end(), lessPeakX );
	}

	return true;
}

// on search
std::string MascotPmfForLCMSSearchEngine::onSearch(
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

	if( m_pmfPeaks.size() == 0 ) {
		LOG_WARN( FMT( "There are no peaks" ) );
		return ret;
	}
	progress.createSubProgresses( 3 );

	// export peaks
	progress.setStatus( "Exporting Peaks..." );
	kome::core::Progress* prgs = progress.getSubProgress( 0 );

	std::string peaksPath = getpath( m_peaksDir.c_str(), "peaks.pmf" );
	FILE* fp = fileopen( peaksPath.c_str(), "w" );
	if( fp == NULL ) {
		return ret;
	}

	for( unsigned int i = 0; i < m_pmfPeaks.size(); i++ ) {
		fprintf( fp, "%f %f\n", m_pmfPeaks[ i ].x, m_pmfPeaks[ i ].y );
	}

	fflush( fp );
	fclose( fp );

	if( prgs->isStopped() ) {
		return ret;
	}
	prgs->fill();

	// search
	progress.setStatus( "Identifying with PMF Search..." );
	prgs = progress.getSubProgress( 1 );

	std::vector< std::string > peakFiles;
	peakFiles.push_back( peaksPath );

	ret = performSearch( peakFiles, title, settings, *prgs );

	if( prgs->isStopped() ) {
		return ret;
	}
	progress.fill();

	return ret;
}

// prepares peak detection
bool MascotPmfForLCMSSearchEngine::onPreparePeakDetection(
		kome::objects::SettingParameterValues* searchSettings,
		const char* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		const char* chargeDetector,
		kome::objects::SettingParameterValues* chargeSettings,
		const char* peakFilter
) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// clear peaks
	m_pmfPeaks.clear();

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

	// get parameters
	m_peakTol = searchSettings->getDoubleValue( "peak_tol", 0.25 );
	m_peakNum = searchSettings->getIntValue( "peak_num", 999 );
	m_peakTolUnit = TOL_DA;

	std::string u = tolowercase( searchSettings->getParameterValue( "peak_tol_unit" ) );
	if( u.compare( "ppm" ) == 0 ) {
		m_peakTolUnit = TOL_PPM;
	}

	return true;
}

// on end search
void MascotPmfForLCMSSearchEngine::onEndSearch() {
	m_pmfPeaks.clear();
	m_peaksDir.clear();
}

// comapre peaks by x
bool MascotPmfForLCMSSearchEngine::lessPeakX( PeakInfo& peak0, PeakInfo& peak1 ) {
	return ( peak0.x < peak1.x );
}

// compare peaks by y
bool MascotPmfForLCMSSearchEngine::lessPeakY( PeakInfo& peak0, PeakInfo& peak1 ) {
	return ( peak0.y > peak1.y );
}
