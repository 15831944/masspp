/**
 * @file PeakSearch.cpp
 * @brief implements of PeakSearch class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakSearch.h"   
#include "PeakListCtrl.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION					   "Peak Search"
#define SPEC_VIEW_RANGE_PARAM_NAME	"SPECTRUM_VIEW_RANGE"


// constructor
PeakSearch::PeakSearch() {
	reset();

	setSpecViewRange();
}

// destructor
PeakSearch::~PeakSearch() {
}

// reset parameters
void PeakSearch::resetParameters() {
	m_minStage = -1;
	m_maxStage = -1;
	m_minRt = -1.0;
	m_maxRt = -1.0;
	m_minMz = -1.0;
	m_maxMz = -1.0;
	m_minInt = -1.0;
	m_maxInt = -1.0;

//	m_peaksDlg = NULL;	// @date 2011/05/30 <Del> OKADA
	closeDialog();		// @date 2011/05/30 <Add> OKADA

}

// clear result
void PeakSearch::clearResult() {
	// close dialog
	if( m_peaksDlg != NULL ) {
		delete m_peaksDlg;
		m_peaksDlg = NULL;
	}

	// clear result
	m_hitPeaks.clear();

	// initialize parameters
	m_group = NULL;
	m_lblSpecCnt = 0;
	m_notLblSpecCnt = 0;
}

// reset
void PeakSearch::reset() {
	resetParameters();
	clearResult();
}

// set stage range
void PeakSearch::setStageRange( const int minStage, const int maxStage ) {
	m_minStage = minStage;
	m_maxStage = maxStage;
}

// get min stage
int PeakSearch::getMinStage() {
	return m_minStage;
}

// get max stage
int PeakSearch::getMaxStage() {
	return m_maxStage;
}

// set RT range
void PeakSearch::setRtRange( const double minRt, const double maxRt ) {
	m_minRt = minRt;
	m_maxRt = maxRt;
}

// gets min RT
double PeakSearch::getMinRt() {
	return m_minRt;
}

// get max RT
double PeakSearch::getMaxRt() {
	return m_maxRt;
}

// set m/z range
void PeakSearch::setMzRange( const double minMz, const double maxMz ) {
	m_minMz = minMz;
	m_maxMz = maxMz;
}

// get min m/z
double PeakSearch::getMinMz() {
	return m_minMz;
}

// get max m/z
double PeakSearch::getMaxMz() {
	return m_maxMz;
}

// set intensity range
void PeakSearch::setIntRange( const double minInt, const double maxInt ) {
	m_minInt = minInt;
	m_maxInt = maxInt;
}

// get min intenisty
double PeakSearch::getMinIntensity() {
	return m_minInt;
}

// get max intensity
double PeakSearch::getMaxIntensity() {
	return m_maxInt;
}

// set spectrum view range
void PeakSearch::setSpecViewRange() {
	// default
	m_specViewRange = 3.0;

	// get range from ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	m_specViewRange = ini->getDouble( SECTION, SPEC_VIEW_RANGE_PARAM_NAME, m_specViewRange );
}

// get spectrum view range
double PeakSearch::getSpecViewRange() {
	return m_specViewRange;
}

// get the number of peaks
unsigned int PeakSearch::getNumberOfPeaks() {
	return m_hitPeaks.size();
}

// get peak spectrum
kome::objects::Spectrum* PeakSearch::getPeakSpectrum( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return NULL;
	}
	return m_hitPeaks[ index ].spec;
}

// get peak RT
double PeakSearch::getPeakRt( const unsigned int index ) {
	kome::objects::Spectrum* spec = getPeakSpectrum( index );

	return ( spec == NULL ? -1.0 : spec->getRt() );
}

// get peak m/z
double PeakSearch::getPeakMz( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].mz;
}

// get peak intensity
double PeakSearch::getPeakIntensity( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].intensity;
}

// get the target group
kome::objects::DataGroupNode* PeakSearch::getTargetGroup() {
	return m_group;
}

// get the number of labeled spectra
int PeakSearch::getNumberOfLabeledSpectra() {
	return m_lblSpecCnt;
}

// get the number of not labeled spectra
int PeakSearch::getNumberOfNotLabeledSpectra() {
	return m_notLblSpecCnt;
}

// search peaks
bool PeakSearch::search( kome::objects::DataGroupNode& group, kome::core::Progress* progress ) {
	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// set group
	m_group = &group;

	// spectra
	std::vector< kome::objects::Spectrum* > spectra;
	for( unsigned int i = 0; i < group.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = group.getSpectrum( i );

		if( ( m_minStage < 0 || spec->getMsStage() >= m_minStage )
				&& ( m_maxStage < 0 || spec->getMsStage() <= m_maxStage )
				&& ( m_minRt < 0.0 || spec->getRt() >= m_minRt )
				&& ( m_maxRt < 0.0 || spec->getRt() <= m_maxRt )
		) {
			spectra.push_back( spec );
		}
	}
	if( spectra.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "Filter Peaks cannot be performed because there are no spectra. Open a data file containing spectra." ), ERR_OTHER );
		return false;
	}
	if( progress->isStopped() ) {
		return true;
	}

	// peaks
	std::vector< std::pair< kome::objects::Spectrum*, kome::objects::Peaks* > > peaksArray;
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = spectra[ i ];
		kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );

		if( peaks != NULL ) {
			peaksArray.push_back( std::make_pair( spec, peaks ) );
		}
	}
	if( peaksArray.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "No peaks could be filtered because no spectra have been peak-detected.\nPerform peak detection on at least one spectrum." ), ERR_OTHER );
		return false;
	}

	m_lblSpecCnt = peaksArray.size();
	m_notLblSpecCnt = (int)spectra.size() - m_lblSpecCnt;
	if( m_notLblSpecCnt > 0 ) {
		LOG_WARN( FMT( "%d spectra have not been peak-detected.", m_notLblSpecCnt ) );
	}
	if( progress->isStopped() ) {
		return true;
	}

	// search
	progress->setRange( 0, peaksArray.size() );
	for( unsigned int i = 0; i < peaksArray.size() && !progress->isStopped(); i++ ) {
		// peaks
		kome::objects::Peaks* peaks = peaksArray[ i ].second;

		// index
		int startIdx = 0;
		int endIdx = (int)peaks->getLength() - 1;

		if( m_minMz >= 0.0 ) {
			startIdx = peaks->searchIndex( m_minMz );
			if( startIdx < 0 ) {
				startIdx = - startIdx - 1;
			}
		}

		if( m_maxMz >= 0.0 ) {
			endIdx = peaks->searchIndex( m_maxMz );
			if( endIdx < 0 ) {
				endIdx = - endIdx - 2;
			}
		}

		// search
		for( int j = startIdx; j <= endIdx; j++ ) {
			// peak
			kome::objects::PeakElement* peak = peaks->getPeak( j );

			if( ( m_minInt < 0.0 || peak->getY() >= m_minInt )
					&& ( m_maxInt < 0.0 || peak->getY() <= m_maxInt ) ) {
				m_hitPeaks.resize( m_hitPeaks.size() + 1 );

				m_hitPeaks.back().spec = peaksArray[ i ].first;
				m_hitPeaks.back().mz = peak->getX();
				m_hitPeaks.back().intensity = peak->getY();
			}
		}

		progress->setPosition( i + 1 );
	}
	progress->fill();

	return true;
}

// release dialog
void PeakSearch::releaseDialog() {
	m_peaksDlg = NULL;
}

// toogle visibility of the dialog
void PeakSearch::toggleDialog() {
	// show flag
	bool showing = !isShownDialog();

	// delete old dialog
	if( m_peaksDlg != NULL ) {
		delete m_peaksDlg;
		m_peaksDlg = NULL;
	}

	// show
	if( showing ) {
//		m_peaksDlg = new kome::window::GridListDialog( kome::window::WindowTool::getMainWindow(), "Peaks" );
		m_peaksDlg = new kome::window::GridListDialog( kome::window::WindowTool::getMainWindow(), "Filtered Peak List" );
		PeakListCtrl* peaksList = new PeakListCtrl( m_peaksDlg, 400, 300 );

		m_peaksDlg->Show( true );
	}
}

// judge whether matrix dialog is opened
bool PeakSearch::isShownDialog() {
	if( m_peaksDlg == NULL ) {
		return false;
	}
	return m_peaksDlg->IsShown();
}

// update dialog
void PeakSearch::updateDialog() {
	// delete old dialog
	if( m_peaksDlg != NULL ) {
		delete m_peaksDlg;
		m_peaksDlg = NULL;
	}

	// show dialog
	toggleDialog();
}

// close dialog
void PeakSearch::closeDialog() {
	// delete old dialog
	if( m_peaksDlg != NULL ) {
		delete m_peaksDlg;
		m_peaksDlg = NULL;
	}
}

// on close sample
void PeakSearch::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	if( m_group == NULL || sample == NULL ) {
		return;
	}

	if( m_group->getSample() == sample ) {
		reset();
	}
}

// gets instance
PeakSearch& PeakSearch::getInstance() {
	// create object (This is the only object.)
	static PeakSearch s;

	return s;
}
