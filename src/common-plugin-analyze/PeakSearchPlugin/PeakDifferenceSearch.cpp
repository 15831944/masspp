/**
 * @file PeakDifferenceSearch.cpp
 * @brief implements of PeakDifferenceSearch class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakDifferenceSearch.h"                 
#include "PeakDifferenceListCtrl.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeakDifferenceSearch::PeakDifferenceSearch() {
	reset();
}

// destructor
PeakDifferenceSearch::~PeakDifferenceSearch() {
}

// reset parameters
void PeakDifferenceSearch::resetParameters() {
	m_mzDiff = -1.0;
	m_mzTol = -1.0;
	m_minStage = -1;
	m_maxStage = -1;
	m_minRt = -1.0;
	m_maxRt = -1.0;
	m_minMz = -1.0;
	m_maxMz = -1.0;
	m_minInt = -1.0;
	m_maxInt = -1.0;

	closeDialog();			// @date 2011/05/30 <Add> OKADA
}

// clear result
void PeakDifferenceSearch::clearResult() {
	// close dialog
	if( m_peakDiffDlg != NULL ) {
		delete m_peakDiffDlg;
		m_peakDiffDlg = NULL;
	}

	// clear result
	m_hitPeaks.clear();

	// initialize parameters
	m_group = NULL;
	m_lblSpecCnt = 0;
	m_notLblSpecCnt = 0;
}

// reset
void PeakDifferenceSearch::reset() {
	resetParameters();
	clearResult();
}

// set m/z difference
void PeakDifferenceSearch::setMzDifference( const double diff ) {
	m_mzDiff = diff;
}

// get m/z difference
double PeakDifferenceSearch::getMzDifference() {
	return m_mzDiff;
}

// set m/z tolerance
void PeakDifferenceSearch::setMzTolerance( const double tol ) {
	m_mzTol = tol;
}

// get m/z tolerance
double PeakDifferenceSearch::getMzTolerance() {
	return m_mzTol;
}

// set stage range
void PeakDifferenceSearch::setStageRange( const int minStage, const int maxStage ) {
	m_minStage = minStage;
	m_maxStage = maxStage;
}

// get min stage
int PeakDifferenceSearch::getMinStage() {
	return m_minStage;
}

// get max stage
int PeakDifferenceSearch::getMaxStage() {
	return m_maxStage;
}

// set RT range
void PeakDifferenceSearch::setRtRange( const double minRt, const double maxRt ) {
	m_minRt = minRt;
	m_maxRt = maxRt;
}

// gets min RT
double PeakDifferenceSearch::getMinRt() {
	return m_minRt;
}

// get max RT
double PeakDifferenceSearch::getMaxRt() {
	return m_maxRt;
}

// set m/z range
void PeakDifferenceSearch::setMzRange( const double minMz, const double maxMz ) {
	m_minMz = minMz;
	m_maxMz = maxMz;
}

// get min m/z
double PeakDifferenceSearch::getMinMz() {
	return m_minMz;
}

// get max m/z
double PeakDifferenceSearch::getMaxMz() {
	return m_maxMz;
}

// set intensity range
void PeakDifferenceSearch::setIntRange( const double minInt, const double maxInt ) {
	m_minInt = minInt;
	m_maxInt = maxInt;
}

// get min intenisty
double PeakDifferenceSearch::getMinIntensity() {
	return m_minInt;
}

// get max intensity
double PeakDifferenceSearch::getMaxIntensity() {
	return m_maxInt;
}

// get the number of peaks
unsigned int PeakDifferenceSearch::getNumberOfPeaks() {
	return m_hitPeaks.size();
}

// get peak spectrum
kome::objects::Spectrum* PeakDifferenceSearch::getPeakSpectrum( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return NULL;
	}
	return m_hitPeaks[ index ].spec;
}

// get peak RT
double PeakDifferenceSearch::getPeakRt( const unsigned int index ) {
	kome::objects::Spectrum* spec = getPeakSpectrum( index );

	return ( spec == NULL ? -1.0 : spec->getRt() );
}

// get low peak m/z
double PeakDifferenceSearch::getLowPeakMz( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].lowMz;
}

// get low peak intensity
double PeakDifferenceSearch::getLowPeakIntensity( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].lowInt;
}

// get high peak m/z
double PeakDifferenceSearch::getHighPeakMz( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].highMz;
}

// get high peak intensity
double PeakDifferenceSearch::getHighPeakIntensity( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].highInt;
}

// get peak difference
double PeakDifferenceSearch::getPeakDiff( const unsigned int index ) {
	if( index >= m_hitPeaks.size() ) {
		return -1.0;
	}
	return m_hitPeaks[ index ].diff;
}

// get the target group
kome::objects::DataGroupNode* PeakDifferenceSearch::getTargetGroup() {
	return m_group;
}

// get the number of labeled spectra
int PeakDifferenceSearch::getNumberOfLabeledSpectra() {
	return m_lblSpecCnt;
}

// get the number of not labeled spectra
int PeakDifferenceSearch::getNumberOfNotLabeledSpectra() {
	return m_notLblSpecCnt;
}

// search peaks
bool PeakDifferenceSearch::search( kome::objects::DataGroupNode& group, kome::core::Progress* progress ) {
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
		LOG_ERROR_CODE( FMT( "Filter Peaks cannot be used because there are no spectra in the active sample. Open a data file containing spectra." ), ERR_OTHER );
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
			startIdx = peaks->searchIndex( std::max( 0.0, m_minMz - m_mzDiff - m_mzTol ) );
			if( startIdx < 0 ) {
				startIdx = - startIdx - 1;
			}
		}

		if( m_maxMz >= 0.0 ) {
			endIdx = peaks->searchIndex( m_maxMz + m_mzDiff + m_mzTol );
			if( endIdx < 0 ) {
				endIdx = - endIdx - 2;
			}
		}

		// search
		for( int j = startIdx; j <= endIdx; j++ ) {
			// low peak
			kome::objects::PeakElement* lowPeak = peaks->getPeak( j );

			// high peak index
			int highStartIdx = peaks->searchIndex( lowPeak->getX() + m_mzDiff - m_mzTol );
			if( highStartIdx < 0 ) {
				highStartIdx = - highStartIdx - 1;
			}
			highStartIdx = std::max( highStartIdx, startIdx + 1 );

			int highEndIdx = peaks->searchIndex( lowPeak->getX() + m_mzDiff + m_mzTol );
			if( highEndIdx < 0 ) {
				highEndIdx = - highEndIdx - 2;
			}

			// high peak
			for( int k = highStartIdx; k <= highEndIdx; k++ ) {
				kome::objects::PeakElement* highPeak = peaks->getPeak( k );

				// add
				if( ( m_minMz < 0.0 || ( lowPeak->getX() >= m_minMz && highPeak->getX() >= m_minMz ) )
						&& ( m_maxMz < 0.0 || ( lowPeak->getX() <= m_maxMz && highPeak->getX() <= m_maxMz ) )
						&& ( m_minInt < 0.0 || ( lowPeak->getY() >= m_minInt && highPeak->getY() >= m_minInt ) )
						&& ( m_maxInt < 0.0 || ( lowPeak->getY() <= m_maxInt && highPeak->getY() <= m_maxInt ) )
				) {
					m_hitPeaks.resize( m_hitPeaks.size() + 1 );
					m_hitPeaks.back().spec = peaksArray[ i ].first;
					m_hitPeaks.back().lowMz = lowPeak->getX();
					m_hitPeaks.back().lowInt = lowPeak->getY();
					m_hitPeaks.back().highMz = highPeak->getX();
					m_hitPeaks.back().highInt = highPeak->getY();
					m_hitPeaks.back().diff = highPeak->getX() - lowPeak->getX();
				}
			}
		}

		progress->setPosition( i + 1 );
	}
	progress->fill();

	return true;
}

// release dialog
void PeakDifferenceSearch::releaseDialog() {
	m_peakDiffDlg = NULL;
}

// toogle visibility of the dialog
void PeakDifferenceSearch::toggleDialog() {
	// show flag
	bool showing = !isShownDialog();

	// delete old dialog
	if( m_peakDiffDlg != NULL ) {
		delete m_peakDiffDlg;
		m_peakDiffDlg = NULL;
	}

	// show
	if( showing ) {
		m_peakDiffDlg = new kome::window::GridListDialog( kome::window::WindowTool::getMainWindow(), "Filtered Peak List" );
		PeakDifferenceListCtrl* peaksList = new PeakDifferenceListCtrl( m_peakDiffDlg, 400, 300 );

		m_peakDiffDlg->Show( true );
	}
}

// judge whether matrix dialog is opened
bool PeakDifferenceSearch::isShownDialog() {
	if( m_peakDiffDlg == NULL ) {
		return false;
	}
	return m_peakDiffDlg->IsShown();
}

// update dialog
void PeakDifferenceSearch::updateDialog() {
	// delete old dialog
	if( m_peakDiffDlg != NULL ) {
		delete m_peakDiffDlg;
		m_peakDiffDlg = NULL;
	}

	// show dialog
	toggleDialog();
}

// close dialog
void PeakDifferenceSearch::closeDialog() {
	// delete old dialog
	if( m_peakDiffDlg != NULL ) {
		delete m_peakDiffDlg;
		m_peakDiffDlg = NULL;
	}
}

// on close sample
void PeakDifferenceSearch::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	if( m_group == NULL || sample == NULL ) {
		return;
	}

	if( m_group->getSample() == sample ) {
		reset();
	}
}

// gets instance
PeakDifferenceSearch& PeakDifferenceSearch::getInstance() {
	// create object (This is the only object.)
	static PeakDifferenceSearch s;

	return s;
}
