/**
 * @file PeaksCluster2D.cpp
 * @brief implements of PeaksCluster2D class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksCluster2D.h"

#include "Peak2DElement.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeaksCluster2D::PeaksCluster2D() {
	m_rt = 0.0;
	m_rtFlg = false;

	m_startRt = 0.0;
	m_endRt = 0.0;
	m_rtRangeFlg = false;

	m_mz = 0.0;
	m_mzFlg = false;

	m_startMz = 0.0;
	m_endMz = 0.0;
	m_mzRangeFlg = false;

	m_intensity = 0.0;
	m_intFlg = false;

	m_charge = -1;
}

// destructor
PeaksCluster2D::~PeaksCluster2D() {
	while( m_peaks.size() > 0 ) {
		m_peaks.front()->setCluster( NULL );
	}
}

// set RT
void PeaksCluster2D::setRt( const double rt ) {
	m_rt = rt;
	m_rtFlg = true;
}

// get RT
double PeaksCluster2D::getRt() {
	return m_rt;
}

// set RT range
void PeaksCluster2D::setRtRange( const double startRt, const double endRt ) {
	m_startRt = startRt;
	m_endRt = endRt;
	m_rtRangeFlg = true;
}

// get start RT
double PeaksCluster2D::getStartRt() {
	return m_startRt;
}

// get end RT
double PeaksCluster2D::getEndRt() {
	return m_endRt;
}

// set m/z
void PeaksCluster2D::setMz( const double mz ) {
	m_mz = mz;
	m_mzFlg = true;
}

// get m/z
double PeaksCluster2D::getMz() {
	return m_mz;
}

// set m/z range
void PeaksCluster2D::setMzRange( const double startMz, const double endMz ) {
	m_startMz = startMz;
	m_endMz = endMz;
	m_mzRangeFlg = true;
}

// get start m/z
double PeaksCluster2D::getStartMz() {
	return m_startMz;
}

// get end m/z
double PeaksCluster2D::getEndMz() {
	return m_endMz;
}

// set intensity
void PeaksCluster2D::setIntensity( const double intensity ) {
	m_intensity = intensity;
	m_intFlg = true;
}

// get intensity
double PeaksCluster2D::getIntensity() {
	return m_intensity;
}

// set charge
void PeaksCluster2D::setCharge( const int charge ) {
	m_charge = charge;
}

// get charge
int PeaksCluster2D::getCharge() {
	return m_charge;
}

// add peak
void PeaksCluster2D::addPeak( Peak2DElement* peak ) {
	// search peak
	int idx = searchPeak( peak );
	if( idx >= 0 || peak == NULL ) {
		return;
	}

	// add
	m_peaks.push_back( peak );
	peak->setCluster( this );

	// update
	update();
}

// remove peak
void PeaksCluster2D::removePeak( Peak2DElement* peak ) {
	// index
	int idx = searchPeak( peak );

	// remove
	if( idx >= 0 ) {
		removePeak( idx );
	}
}

// remove peak
void PeaksCluster2D::removePeak( const unsigned int idx ) {
	// check the index
	if( idx >= m_peaks.size() ) {
		return;
	}

	// peak
	kome::objects::Peak2DElement* peak = m_peaks[ idx ];

	// remove
	m_peaks.erase( m_peaks.begin() + idx );

	// set cluster
	peak->setCluster( NULL );

	// update
	update();
}

// clear peaks
void PeaksCluster2D::clearPeaks() {
	// clear
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		m_peaks[ i ]->setCluster( NULL );
	}
	m_peaks.clear();

	// update
	update();
}

// update member values
void PeaksCluster2D::update() {
	// initialize
	double rt = 0.0;
	double startRt = 0.0;
	double endRt = 0.0;
	double mz = 0.0;
	double startMz = 0.0;
	double endMz = 0.0;
	double intensity = 0.0;

	// peaks
	bool neg = true;
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		// peak
		Peak2DElement* peak = m_peaks[ i ];
		double peakInt = std::max( peak->getIntensity(), 0.0 );
		if( peak->getIntensity() >= 0.0 ) {
			neg = false;
		}

		// RT
		rt += peak->getRt() * peakInt;
		if( i == 0 || peak->getStartRt() < startRt ) {
			startRt = peak->getStartRt();
		}
		if( i == 0 || peak->getEndRt() > endRt ) {
			endRt = peak->getEndRt();
		}

		// m/z
		mz += peak->getMz() * peakInt;
		if( i == 0 || peak->getStartMz() < startMz ) {
			startMz = peak->getStartMz();
		}
		if( i == 0 || peak->getEndMz() > endMz ) {
			endMz = peak->getEndMz();
		}

		// intensity
		intensity += peakInt;
	}

	// set
	if( !m_rtFlg ) {
		if( intensity > 0.0 ) {
			m_rt = rt / intensity;
		}
		else {
			for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
				rt += m_peaks[ i ]->getRt();
			}
			if( m_peaks.size() > 0 ) {
				rt /= (double)m_peaks.size();
			}
			m_rt = rt;
		}
	}
	if( !m_rtRangeFlg ) {
		m_startRt = startRt;
		m_endRt = endRt;
	}
	if( !m_mzFlg ) {
		if( intensity > 0.0 ) {
			m_mz = mz / intensity;
		}
		else {
			for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
				mz += m_peaks[ i ]->getMz();
			}
			if( m_peaks.size() > 0 ) {
				mz /= (double)m_peaks.size();
			}
			m_mz = mz;
		}
	}
	if( !m_mzRangeFlg ) {
		m_startMz = startMz;
		m_endMz = endMz;
	}
	if( !m_intFlg ) {
		m_intensity = neg ? -1.0 : intensity;
	}
}
