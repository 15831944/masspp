/**
 * @file Ab3dCompositionCluster.cpp
 * @brief implements of Ab3dCompositionCluster class
 *
 * @author S.Tanaka
 * @date 2007.10.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dCompositionCluster.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Ab3dCompositionCluster::Ab3dCompositionCluster() {
	m_cluster = NULL;

	m_intensity = double();
	m_ctrlInt = 0.0;
	m_trmtInt = 0.0;
}

// destructor
Ab3dCompositionCluster::~Ab3dCompositionCluster() {
}

// set clusters
void Ab3dCompositionCluster::setCluster( kome::objects::PeaksCluster2D* cluster ) {
	m_cluster = cluster;
}

// get cluster
kome::objects::PeaksCluster2D* Ab3dCompositionCluster::getCluster() {
	return m_cluster;
}

// set intensity
void Ab3dCompositionCluster::setIntensity( const double intensity ) {
	m_intensity = intensity;
}

// get intensity
double Ab3dCompositionCluster::getIntensity() {
	return m_intensity;
}

// set control intensity
void Ab3dCompositionCluster::setCtrlIntensity( const double intensity ) {
	m_ctrlInt = intensity;
}

// get control intensity
double Ab3dCompositionCluster::getCtrlIntensity() {
	return m_ctrlInt;
}

// set treatment intensity
void Ab3dCompositionCluster::setSmplIntensity( const double intensity ) {
	m_trmtInt = intensity;
}

// get treatment intensity
double Ab3dCompositionCluster::getSmplIntensity() {
	return m_trmtInt;
}

// add control cluster
void Ab3dCompositionCluster::addCtrlCluster( kome::objects::PeaksCluster2D* cluster ) {
	m_ctrlClusters.push_back( cluster );
}

// get the number of control clusters
unsigned int Ab3dCompositionCluster::getNumberOfCtrlClusters() {
	return m_ctrlClusters.size();
}

// get control cluster
kome::objects::PeaksCluster2D* Ab3dCompositionCluster::getCtrlCluster( const unsigned int idx ) {
	if( idx >= m_ctrlClusters.size() ) {
		return NULL;
	}
	return m_ctrlClusters[ idx ];
}

// add treatment cluster
void Ab3dCompositionCluster::addSmplCluster( kome::objects::PeaksCluster2D* cluster ) {
	m_trmtClusters.push_back( cluster );
}

// get the number of treatment clusters
unsigned int Ab3dCompositionCluster::getNumberOfSmplClusters() {
	return m_trmtClusters.size();
}

// get treatment cluster
kome::objects::PeaksCluster2D* Ab3dCompositionCluster::getSmplCluster( const unsigned int idx ) {
	if( idx >= m_trmtClusters.size() ) {
		return NULL;
	}
	return m_trmtClusters[ idx ];
}
