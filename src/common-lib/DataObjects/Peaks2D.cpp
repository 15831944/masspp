/**
 * @file Peaks2D.cpp
 * @brief implements of Peaks2D class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * @date 2011.03.04 <Add> M.Aihara
 *       add createPeak TBB parallel code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks2D.h"

#include "Peak2DElement.h"
#include "PeaksCluster2D.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peaks2D::Peaks2D() {
	m_dataId = -1;
}

// copy constructor
Peaks2D::Peaks2D( const Peaks2D& peaks ) {
	// create clusters
	std::map< PeaksCluster2D*, PeaksCluster2D* > clusterMap;
	clusterMap[ NULL ] = NULL;

	for( unsigned int i = 0; i < peaks.m_clusters.size(); i++ ) {
		// source cluster
		PeaksCluster2D* src = peaks.m_clusters[ i ];

		// new cluster
		PeaksCluster2D* cluster = new PeaksCluster2D();
		m_clusters.push_back( cluster );
		clusterMap[ src ] = cluster;

		cluster->setRt( src->getRt() );
		cluster->setRtRange( src->getStartRt(), src->getEndRt() );		
		cluster->setMz( src->getMz() );
		cluster->setMzRange( src->getStartMz(), src->getEndMz() );
		cluster->setIntensity( src->getIntensity() );
	}

	// create peaks
	m_peaks.clear();
	for( unsigned int i = 0; i < peaks.m_peaks.size(); i++ ) {
		// source peak
		Peak2DElement* src = peaks.m_peaks[ i ];

		// new peak
		Peak2DElement* peak = new Peak2DElement( this );
		m_peaks.push_back( peak );

		peak->setRt( src->getRt(), src->getStartRt(), src->getEndRt() );
		peak->setMz( src->getMz(), src->getStartMz(), src->getEndMz() );
		peak->setIntensity( src->getIntensity() );

		PeaksCluster2D* cluster = clusterMap[ src->getCluster() ];
		if( cluster != NULL ) {
			cluster->addPeak( peak );
		}
	}

	// data ID
	m_dataId = peaks.m_dataId;
}

// destructor
Peaks2D::~Peaks2D() {
	clearClusters();
	clearPeaks();
}

// substitution operator
Peaks2D& Peaks2D::operator=( const Peaks2D& other ) {
	// create clusters
	std::map< PeaksCluster2D*, PeaksCluster2D* > clusterMap;
	clusterMap[ NULL ] = NULL;

	for( unsigned int i = 0; i < other.m_clusters.size(); i++ ) {
		// source cluster
		PeaksCluster2D* src = other.m_clusters[ i ];

		// new cluster
		PeaksCluster2D* cluster = new PeaksCluster2D();
		m_clusters.push_back( cluster );
		clusterMap[ src ] = cluster;

		cluster->setRt( src->getRt() );
		cluster->setRtRange( src->getStartRt(), src->getEndRt() );		
		cluster->setMz( src->getMz() );
		cluster->setMzRange( src->getStartMz(), src->getEndMz() );
		cluster->setIntensity( src->getIntensity() );
	}

	// create peaks
	m_peaks.clear();
	for( unsigned int i = 0; i < other.m_peaks.size(); i++ ) {
		// source peak
		Peak2DElement* src = other.m_peaks[ i ];

		// new peak
		Peak2DElement* peak = new Peak2DElement( this );
		m_peaks.push_back( peak );

		peak->setRt( src->getRt(), src->getStartRt(), src->getEndRt() );
		peak->setMz( src->getMz(), src->getStartMz(), src->getEndMz() );
		peak->setIntensity( src->getIntensity() );

		PeaksCluster2D* cluster = clusterMap[ src->getCluster() ];
		if( cluster != NULL ) {
			cluster->addPeak( peak );
		}
	}

	// data ID
	m_dataId = other.m_dataId;

	return *this;
}

// create peak
Peak2DElement* Peaks2D::createPeak(
		const double rt,
		const double startRt,
		const double endRt,
		const double mz,
		const double startMz,
		const double endMz,
		const double intensity
) {
	// create peak
	Peak2DElement* peak = new Peak2DElement( this );
	m_peaks.push_back( peak );

	// set
	peak->setRt( rt, startRt, endRt );
	peak->setMz( mz, startMz, endMz );
	peak->setIntensity( intensity );

	return peak;
}
// @date 2011.03.04 <Add> M.Aihara ->
// add createPeak TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
Peak2DElement* Peaks2D::createPeak_TBB(
		const double rt,
		const double startRt,
		const double endRt,
		const double mz,
		const double startMz,
		const double endMz,
		const double intensity
) {
	// create peak
	Peak2DElement* peak = new Peak2DElement( this );
	// m_peaks.push_back( peak );

	// set
	peak->setRt( rt, startRt, endRt );
	peak->setMz( mz, startMz, endMz );
	peak->setIntensity( intensity );

	return peak;
}
// push_back peak element objects
void Peaks2D::push_back_TBB(
		std::vector<kome::objects::Peak2DElement*>& peaks
) {
	for(std::vector<kome::objects::Peak2DElement*>::iterator it= peaks.begin();
		it != peaks.end();
		++it ){
		m_peaks.push_back( *it );
	}
	return;
}
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

// create peak
Peak2DElement* Peaks2D::createPeak(
		const double rt,
		const double mz,
		const double intensity
) {
	// create peak
	Peak2DElement* peak = createPeak( rt, rt, rt, mz, mz, mz, intensity );
	return peak;
}

// clear peaks
void Peaks2D::clearPeaks() {
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		delete m_peaks[ i ];
	}
	m_peaks.clear();
}

// remove peak
void Peaks2D::removePeak( Peak2DElement* peak ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_peaks.size() && idx < 0 ; i++ ) {
		if( peak == m_peaks[ i ] ) {
			idx = (int)i;
		}
	}

	// remove
	if( idx >= 0 ) {
		removePeak( idx );
	}
}

// remove peak
void Peaks2D::removePeak( const unsigned int idx ) {
	// check
	if( idx >= m_peaks.size() ) {
		return;
	}

	// delete
	kome::objects::Peak2DElement* peak = m_peaks[ idx ];
	kome::objects::PeaksCluster2D* cluster = peak->getCluster();
	if( cluster != NULL ) {
		cluster->removePeak( peak );
	}
	delete peak;
	m_peaks.erase( m_peaks.begin() + idx );
}

// create cluster
PeaksCluster2D* Peaks2D::createCluster() {
	PeaksCluster2D* cluster = new PeaksCluster2D();
	m_clusters.push_back( cluster );

	return cluster;
}

// get the number of clusters
unsigned int Peaks2D::getNumberOfClusters() {
	return m_clusters.size();
}

// get cluster
PeaksCluster2D* Peaks2D::getCluster( const unsigned int idx ) {
	if( idx >= m_clusters.size() ) {
		return NULL;
	}
	return m_clusters[ idx ];
}

// clear clusters
void Peaks2D::clearClusters() {
	for( unsigned int i = 0; i < m_clusters.size(); i++ ) {
		delete m_clusters[ i ];
	}
	m_clusters.clear();
}

// remove cluster
void Peaks2D::removeCluster( PeaksCluster2D* cluster ) {
	// search 
	int idx = -1;
	for( unsigned int i = 0; i < m_clusters.size() && idx < 0; i++ ) {
		if( cluster == m_clusters[ i ] ) {
			idx = (int)i;
		}
	}

	// remove
	if( idx >= 0 ) {
		removeCluster( idx );
	}
}

// remove cluster
void Peaks2D::removeCluster( const unsigned int idx ) {
	// check
	if( idx >= m_clusters.size() ) {
		return;
	}

	// remove
	delete m_clusters[ idx ];
	m_clusters.erase( m_clusters.begin() + idx );
}

// set data ID
void Peaks2D::setDataId( const int id ) {
	m_dataId = id;
}

// get data ID
int Peaks2D::getDataId() {
	return m_dataId;
}
