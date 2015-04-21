/**
 * @file PeaksCluster2DWrapper.cpp
 * @brief implements of PeaksCluster2DWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksCluster2DWrapper.h"

#include "Peak2DElementWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeaksCluster2DWrapper::PeaksCluster2DWrapper( kome::objects::PeaksCluster2D& cluster )
		: Peaks2DArrayWrapper( cluster ), m_cluster( cluster ) {
}

// destructor
PeaksCluster2DWrapper::~PeaksCluster2DWrapper() {
}

// get cluster object to be wrapped
kome::objects::PeaksCluster2D& PeaksCluster2DWrapper::getCluster() {
	return m_cluster;
}

// set RT
void PeaksCluster2DWrapper::setRt( double rt ) {
	m_cluster.setRt( rt );
}

// get RT
double PeaksCluster2DWrapper::getRt() {
	return m_cluster.getRt();
}

// set RT range
void PeaksCluster2DWrapper::setRtRange( double startRt, double endRt ) {
	m_cluster.setRtRange( startRt, endRt );
}

// get start RT
double PeaksCluster2DWrapper::getStartRt() {
	return m_cluster.getStartRt();
}

// get end RT
double PeaksCluster2DWrapper::getEndRt() {
	return m_cluster.getEndRt();
}

// set mz/
void PeaksCluster2DWrapper::setMz( double mz ) {
	m_cluster.setMz( mz );
}

// get m/z
double PeaksCluster2DWrapper::getMz() {
	return m_cluster.getMz();
}

// set m/z range
void PeaksCluster2DWrapper::setMzRange( double startMz, double endMz ) {
	m_cluster.setMzRange( startMz, endMz );
}

// get start m/z
double PeaksCluster2DWrapper::getStartMz() {
	return m_cluster.getStartMz();
}

// get end m/z
double PeaksCluster2DWrapper::getEndMz() {
	return m_cluster.getEndMz();
}

// set intensity
void PeaksCluster2DWrapper::setIntensity( double intensity ) {
	m_cluster.setIntensity( intensity );
}

// get intensity
double PeaksCluster2DWrapper::getIntensity() {
	return m_cluster.getIntensity();
}

// set charge
void PeaksCluster2DWrapper::setCharge( int charge ) {
	m_cluster.setCharge( charge );
}

// ge charge
int PeaksCluster2DWrapper::getCharge() {
	return m_cluster.getCharge();
}

// add peak
void PeaksCluster2DWrapper::addPeak( Peak2DElementWrapper^ peak ) {
	// peak object
	kome::objects::Peak2DElement* p = NULL;
	if( peak != nullptr ) {
		p = &peak->getPeakElement();
	}

	// add
	m_cluster.addPeak( p );
}

// remove peak
void PeaksCluster2DWrapper::removePeak( Peak2DElementWrapper^ peak ) {
	// peak object
	kome::objects::Peak2DElement* p = NULL;
	if( peak != nullptr ) {
		p = &peak->getPeakElement();
	}

	// remove
	m_cluster.removePeak( p );
}

// remove peak
void PeaksCluster2DWrapper::removePeak( unsigned int idx ) {
	m_cluster.removePeak( idx );
}

// clear peaks
void PeaksCluster2DWrapper::clearPeaks() {
	m_cluster.clearPeaks();
}
