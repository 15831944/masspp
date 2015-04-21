/**
 * @file DataSet.cpp
 * @brief implements of DataSet class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataSet.h"

#include "Spectrum.h"
#include "Chromatogram.h"
#include "DataGroupNode.h"
#include "Sample.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataSet::DataSet() {
	m_group = NULL;
	m_sample = NULL;
}

// constructor
DataSet::DataSet( DataGroupNode* group ) {
	m_group = group;
	m_sample = ( m_group == NULL ) ? NULL : group->getSample();
}

// destructor
DataSet::~DataSet() {
}

// get sample
Sample* DataSet::getSample() {
	// check the member
	if( m_sample != NULL ) {
		return m_sample;
	}

	// group
	DataGroupNode* group = getGroup();
	if( group == NULL ) {
		return NULL;
	}

	return group->getSample();
}

// get spectrum group
DataGroupNode* DataSet::getGroup() {
	// check the member
	if( m_group != NULL ) {
		return m_group;
	}

	// groups
	std::set< DataGroupNode* > groupSet;

	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		Spectrum* spec = m_spectra[ i ];
		DataGroupNode* g = spec->getGroup();
		if( g == NULL ) {
			return NULL;
		}
		else {
			groupSet.insert( g );
		}
	}

	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		Chromatogram* chrom = m_chroms[ i ];
		DataGroupNode* g = chrom->getGroup();
		if( g == NULL ) {
			return NULL;
		}
		else {
			groupSet.insert( g );
		}
	}

	// common group
	DataGroupNode* group = NULL;
	for( std::set< DataGroupNode* >::iterator it = groupSet.begin();
			it != groupSet.end(); it++ ) {
		DataGroupNode* g = *it;

		if( group == NULL ) {
			group = g;
		}
		else {
			while( group != NULL && group->getLevel() > g->getLevel() ) {
				group = group->getParentGroup();
			}
			while( g != NULL && g->getLevel() > group->getLevel() ) {
				g = g->getParentGroup();
			}

			while( group != NULL && g != NULL && g != group ) {
				group = group->getParentGroup();
				g = g->getParentGroup();
			}

			if( group == NULL || g == NULL ) {
				return NULL;
			}
		}
	}

	return group;
}

// get the number of spectra
unsigned int DataSet::getNumberOfSpectra() {
	return m_spectra.size();
}

// get spectrum
Spectrum* DataSet::getSpectrum( const unsigned int index ) {
	if( index >= m_spectra.size() ) {
		return NULL;
	}
	return m_spectra[ index ];
}

// get the number of chromatograms
unsigned int DataSet::getNumberOfChromatograms() {
	return m_chroms.size();
}

// get chromatogram
Chromatogram* DataSet::getChromatogram( const unsigned int index ) {
	// check the index
	if( index >= m_chroms.size() ) {
		return NULL;
	}
	return m_chroms[ index ];
}

// sort spectra
void DataSet::sortSpectra() {
	if( m_spectra.size() > 0 ) {
		std::sort( m_spectra.begin(), m_spectra.end(), lessSpectrum );
	}
}

// clear spectra
void DataSet::clearSpectra() {
	m_spectra.clear();
}

// add spectrum
void DataSet::addSpectrum( Spectrum* spec ) {
	insertSpectrum( spec, (int)m_spectra.size() );
}

// remove spectrum
void DataSet::removeSpectrum( Spectrum* spec ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_spectra.size() && idx < 0; i++ ) {
		Spectrum* tmp = m_spectra[ i ];
		if( tmp == spec ) {
			idx = (int)i;
		}
	}

	// remove
	if( idx >= 0 ) {
		m_spectra.erase( m_spectra.begin() + idx );
	}
}

// insert spectrum
void DataSet::insertSpectrum( Spectrum* spec, const int idx ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// check group
	bool flg = false;
	DataGroupNode* group = spec->getGroup();
	while( group != NULL && !flg ) {
		if( m_group == group ) {
			flg = true;
		}
		group = group->getParentGroup();
	}

	// add
	if( m_group == NULL || flg ) {
		m_spectra.insert( m_spectra.begin() + idx, spec );
	}
}

// clear chromatograms
void DataSet::clearChromatograms() {
	m_chroms.clear();
}

// add chromatogram
void DataSet::addChromatogram( Chromatogram* chrom ) {
	insertChromatogram( chrom, (int)m_chroms.size() );
}

// remove chromatogram
void DataSet::removeChromatogram( Chromatogram* chrom ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_chroms.size() && idx < 0; i++ ) {
		kome::objects::Chromatogram* tmp = m_chroms[ i ];
		if( tmp == chrom ) {
			idx = (int)i;
		}
	}

	// remove
	if( idx >= 0 ) {
		m_chroms.erase( m_chroms.begin() + idx );
	}
}

// insert chromatogram
void DataSet::insertChromatogram( Chromatogram* chrom, const int idx ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// check group
	bool flg = false;
	DataGroupNode* group = chrom->getGroup();
	while( group != NULL && !flg ) {
		if( m_group == group ) {
			flg = true;
		}
		group = group->getParentGroup();
	}

	// insert
	if( m_group == NULL || flg ) {
		m_chroms.insert( m_chroms.begin() + idx, chrom );
	}
}

// compare spectra
bool DataSet::lessSpectrum( Spectrum* spec0, Spectrum* spec1 ) {
	// @date 2013.11.11 <Del> M.Izumi ->
	//// scan number
	//const int s0 = spec0->getScanNumber();
	//const int s1 = spec1->getScanNumber();
	//if( s0 >= 0 && s1 >= 0 ) {
	//	if( s0 < s1 ) {
	//		return true;
	//	}
	//	return false;
	//}
	// @date 2013.11.11 <Del> M.Izumi <-

	// RT
	const double rt0 = spec0->getRt();
	const double rt1 = spec1->getRt();

	if( rt0 < rt1 ) {
		return true;
	}
	else if( rt0 > rt1 ) {
		return false;
	}

	return false;
}
