/**
 * @file DataManagerWrapper.cpp
 * @brief implements of DataManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataManagerWrapper.h"

#include "SampleWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "DataMapInfoWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataManagerWrapper::DataManagerWrapper( kome::objects::DataManager& dataMgr ) 
		: m_dataMgr( dataMgr ) {
}

// destrucotr
DataManagerWrapper::~DataManagerWrapper() {
}

// get data manager object
kome::objects::DataManager& DataManagerWrapper::getDataManager() {
	return m_dataMgr;
}

// open sample
void DataManagerWrapper::openSample( SampleWrapper^ sample ) {
	// get sample
	kome::objects::Sample* s = NULL;
	if( sample != nullptr ) {
		s = &( sample->getSample() );
	}

	// open
	kome::objects::DataManager::openSample( s );
}

// close sample
void DataManagerWrapper::closeSample( SampleWrapper^ sample, bool deleting ) {
	// get sample
	kome::objects::Sample* s = NULL;
	if( sample != nullptr ) {
		s = &( sample->getSample() );
	}

	// close
	kome::objects::DataManager::closeSample( s, deleting );
}

// open spectrum
void DataManagerWrapper::openSpectrum( SpectrumWrapper^ spec ) {
	// get spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &( spec->getSpectrum() );
	}

	// open
	kome::objects::DataManager::openSpectrum( s );
}

// close spectrum
void DataManagerWrapper::closeSpectrum( SpectrumWrapper^ spec, bool deleting ) {
	// get spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &( spec->getSpectrum() );
	}

	// close
	kome::objects::DataManager::closeSpectrum( s, deleting );
}

// open chromatogram
void DataManagerWrapper::openChromatogram( ChromatogramWrapper^ chrom ) {
	// get chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &( chrom->getChromatogram() );
	}

	// open
	kome::objects::DataManager::openChromatogram( c );
}

// close chromatogram
void DataManagerWrapper::closeChromatogram( ChromatogramWrapper^ chrom, bool deleting ) {
	// get chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &( chrom->getChromatogram() );
	}

	// close
	kome::objects::DataManager::closeChromatogram( c, deleting );
}

// open data map
void DataManagerWrapper::openDataMap( DataMapInfoWrapper^ dataMap ) {
	// get data map
	kome::objects::DataMapInfo* dp = NULL;
	if( dataMap != nullptr ) {
		dp = &( dataMap->getDataMapInfo() );
	}

	// open
	kome::objects::DataManager::openDataMap( dp );
}

// close data map
void DataManagerWrapper::closeDataMap( DataMapInfoWrapper^ dataMap, bool deleting ) {
	// get data map
	kome::objects::DataMapInfo* dp = NULL;
	if( dataMap != nullptr ) {
		dp = &( dataMap->getDataMapInfo() );
	}

	// close
	kome::objects::DataManager::closeDataMap( dp, deleting );
}
