/**
 * @file PeaksManager.cpp
 * @brief implements of PeaksManager class
 *
 * @author S.Tanaka
 * @date 2007.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksManager.h"

#include "Sample.h"
#include "SampleSet.h"
#include "Spectrum.h"
#include "Chromatogram.h"
#include "DataGroupNode.h"
#include "DataMapInfo.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeaksManager::PeaksManager() {
}

// destructor
PeaksManager::~PeaksManager() {
}

// get peaks of specified spectrum
Peaks* PeaksManager::getPeaks( Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return NULL;
	}

	// get peaks
	if( m_specPeaksMap.find( spec ) == m_specPeaksMap.end() ) {
		return NULL;
	}
	return &( m_specPeaksMap[ spec ] );
}

// get peaks of specified chromatogram
Peaks* PeaksManager::getPeaks( Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return NULL;
	}

	// get peaks
	if( m_chromPeaksMap.find( chrom ) == m_chromPeaksMap.end() ) {
		return NULL;
	}
	return &( m_chromPeaksMap[ chrom ] );
}

// get peaks of specified spectrum group
Peaks2D* PeaksManager::getPeaks( DataGroupNode* group ) {
	// check the parameter
	if( group == NULL ) {
		return NULL;
	}

	// get peaks
	if( m_2dPeaksMap.find( group ) == m_2dPeaksMap.end() ) {
		return NULL;
	}
	return &( m_2dPeaksMap[ group ] );
}

// create peaks of specified spectrum
Peaks* PeaksManager::createPeaks( Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return NULL;
	}

	// create peaks
	Peaks* peaks = getPeaks( spec );
	if( peaks == NULL ) {
		peaks = &( m_specPeaksMap[ spec ] );
	}
	else {
		peaks->clearPoints();
	}

	// set precursor
	peaks->setPrecursor( spec->getPrecursor() );

	return peaks;
}

// create peaks of specified chromatogram
Peaks* PeaksManager::createPeaks( Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return NULL;
	}

	// create peaks
	Peaks* peaks = getPeaks( chrom );
	if( peaks == NULL ) {
		peaks = &( m_chromPeaksMap[ chrom ] );
	}
	else {
		peaks->clearPoints();
	}

	return peaks;
}

// create peaks of specified spectrum group
Peaks2D* PeaksManager::createPeaks( DataGroupNode* group ) {
	// check the parameter
	if( group == NULL ) {
		return NULL;
	}

	// create peaks
	Peaks2D* peaks = getPeaks( group );
	if( peaks == NULL ) {
		peaks = &( m_2dPeaksMap[ group ] );
	}
	else {
		peaks->clearClusters();
		peaks->clearPeaks();
	}

	return peaks;
}

// judge whether peaks of specified spectrum exists
bool PeaksManager::hasPeaks( Spectrum* spec ) {
	return ( m_specPeaksMap.find( spec ) != m_specPeaksMap.end() );
}

// judge whether peaks of specified chromatogram exists
bool PeaksManager::hasPeaks( Chromatogram* chrom ) {
	return ( m_chromPeaksMap.find( chrom ) != m_chromPeaksMap.end() );
}

// judge whether peaks of specified spectrum group exists
bool PeaksManager::hasPeaks( DataGroupNode* group ) {
	return ( m_2dPeaksMap.find( group ) != m_2dPeaksMap.end() );
}

// delete peaks of specified spectrum
void PeaksManager::deletePeaks( Spectrum* spec ) {
	if( m_specPeaksMap.find( spec ) != m_specPeaksMap.end() ) {
		m_specPeaksMap.erase( spec );
	}
}

// delete peaks of specified chromatogram
void PeaksManager::deletePeaks( Chromatogram* chrom ) {
	if( m_chromPeaksMap.find( chrom ) != m_chromPeaksMap.end() ) {
		m_chromPeaksMap.erase( chrom );
	}
}

// delete peaks of specified spectrum group
void PeaksManager::deletePeaks( DataGroupNode* group ) {
	if( m_2dPeaksMap.find( group ) != m_2dPeaksMap.end() ) {
		m_2dPeaksMap.erase( group );
	}
}

// This method is called when a sample is closed
void PeaksManager::onCloseSample( Sample* sample, const bool deleting ) {
	// get spectra
	std::vector< Spectrum* > spectra;
	for( std::map< Spectrum*, Peaks >::iterator it = m_specPeaksMap.begin();
			it != m_specPeaksMap.end(); it++ ) {
		// spectrum
		Spectrum* spec = (*it).first;
		if( spec->getSample() == sample ) {
			spectra.push_back( spec );
		}
	}

	// erase spectrum peaks
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		Spectrum* spec = spectra[ i ];
		m_specPeaksMap[ spec ].clearPoints();
		m_specPeaksMap.erase( spec );
	}

	// get chromatograms
	std::vector< Chromatogram* > chroms;
	for( std::map< Chromatogram*, Peaks >::iterator it = m_chromPeaksMap.begin();
			it != m_chromPeaksMap.end(); it++ ) {
		// chromatogram
		Chromatogram* chrom = (*it).first;
		if( chrom->getSample() == sample ) {
			chroms.push_back( chrom );
		}
	}

	// erase chromatogram peaks
	for( unsigned int i = 0; i < chroms.size(); i++ ) {
		Chromatogram* chrom = chroms[ i ];
		m_chromPeaksMap[ chrom ].clearPoints();
		m_chromPeaksMap.erase( chrom );
	}

	// get spectrum groups
	std::vector< DataGroupNode* > groups;
	for( std::map< DataGroupNode*, Peaks2D >::iterator it = m_2dPeaksMap.begin();
			it != m_2dPeaksMap.end(); it++ ) {
		// spectrum group
		DataGroupNode* group = (*it).first;
		if( group->getSample() == sample ) {
			groups.push_back( group );
		}
	}

	// erase 2d peaks
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		DataGroupNode* group = groups[ i ];

		Peaks2D& peaks = m_2dPeaksMap[ group ];
		peaks.clearClusters();
		peaks.clearPeaks();

		m_2dPeaksMap.erase( group );
	}
}

// update peaks
void PeaksManager::updatePeaks( Spectrum& spec, Peaks* peaks, std::vector<PeakElement>& addPeaks ){
	if( peaks == NULL ) {
		peaks = createPeaks( &spec );
	}
	else {
		peaks->clearPoints();
	}

	for( unsigned int i=0; i < addPeaks.size(); i++ ){
		PeakElement* peakElement = peaks->createPeak( addPeaks[i].getX(), addPeaks[i].getY() );
		if( peakElement != NULL ){
			peakElement->setLeft( addPeaks[i].getLeftX(), addPeaks[i].getLeftY() );
			peakElement->setRight( addPeaks[i].getRightX(), addPeaks[i].getRightY() );
			peakElement->setApex( addPeaks[i].getApexX(), addPeaks[i].getApexY() );
			peakElement->setId( addPeaks[i].getId() );
		}
	}
}

// get peaks manager object
PeaksManager& PeaksManager::getInstance() {
	// create object (This is the only object.)
	static PeaksManager mgr;

	return mgr;
}
