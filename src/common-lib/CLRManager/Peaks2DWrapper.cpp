/**
 * @file Peaks2DWrapper.h
 * @brief interfaces of Peaks2DWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks2DWrapper.h"

#include "Peak2DElementWrapper.h"
#include "PeaksCluster2DWrapper.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peaks2DWrapper::Peaks2DWrapper( kome::objects::Peaks2D& peaks )
		: Peaks2DArrayWrapper( peaks ), m_peaks( peaks ) {
}

// destructor
Peaks2DWrapper::~Peaks2DWrapper() {
}

// get peaks objec to be warpped
kome::objects::Peaks2D& Peaks2DWrapper::getPeaks() {
	return m_peaks;
}

// create peak
Peak2DElementWrapper^ Peaks2DWrapper::createPeak(
		double rt,
		double startRt,
		double endRt,
		double mz,
		double startMz,
		double endMz,
		double intensity
) {
	// create
	kome::objects::Peak2DElement* p = m_peaks.createPeak( rt, startRt, endRt, mz, startMz, endMz, intensity );
	if( p == NULL ) {
		return nullptr;
	}
	return gcnew Peak2DElementWrapper( *p );
}

// create peak
Peak2DElementWrapper^ Peaks2DWrapper::createPeak(
		double rt,
		double mz,
		double intensity
) {
	// create
	kome::objects::Peak2DElement* p = m_peaks.createPeak( rt, mz, intensity );
	if( p == NULL ) {
		return nullptr;
	}
	return gcnew Peak2DElementWrapper( *p );
}

// clear peaks
void Peaks2DWrapper::clearPeaks() {
	m_peaks.clearPeaks();
}

// remove peak
void Peaks2DWrapper::removePeak( Peak2DElementWrapper^ peak ) {
	kome::objects::Peak2DElement* p = NULL;
	if( peak != nullptr ) {
		p = &peak->getPeakElement();
	}

	m_peaks.removePeak( p );
}

// remove peak
void Peaks2DWrapper::removePeak( unsigned int idx ) {
	m_peaks.removePeak( idx );
}

// create cluster
PeaksCluster2DWrapper^ Peaks2DWrapper::createCluster() {
	// create
	kome::objects::PeaksCluster2D* c = m_peaks.createCluster();
	if( c == NULL ) {
		return nullptr;
	}
	return gcnew PeaksCluster2DWrapper( *c );
}

// get the number of clusters
unsigned int Peaks2DWrapper::getNumberOfClusters() {
	return m_peaks.getNumberOfClusters();
}

// get cluster
PeaksCluster2DWrapper^ Peaks2DWrapper::getCluster( unsigned int idx ) {
	kome::objects::PeaksCluster2D* cluster = m_peaks.getCluster( idx );
	if( cluster == NULL ) {
		return nullptr;
	}
	return gcnew PeaksCluster2DWrapper( *cluster );
}

// clear clusters
void Peaks2DWrapper::clearClusters() {
	m_peaks.clearClusters();
}

// remove cluster
void Peaks2DWrapper::removeCluster( PeaksCluster2DWrapper^ cluster ) {
	// get cluster
	kome::objects::PeaksCluster2D* c = NULL;
	if( cluster != nullptr ) {
		c = &cluster->getCluster();
	}

	// remove
	m_peaks.removeCluster( c );
}

// remove cluster
void Peaks2DWrapper::removeCluster( unsigned int idx ) {
	m_peaks.removeCluster( idx );
}

// set data ID
void Peaks2DWrapper::setDataId( int id ) {
	m_peaks.setDataId( id );
}

// get data ID
int Peaks2DWrapper::getDataId() {
	return m_peaks.getDataId();
}
