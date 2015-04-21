/**
 * @file ClrDataManager.cpp
 * @brief implements of ClrDataManager class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrDataManager.h"

#include "ClrDataManagerBase.h"
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
ClrDataManager::ClrDataManager() {
}

// destructor
ClrDataManager::~ClrDataManager() {
}

// set base manager
void ClrDataManager::setBaseDataManager( ClrDataManagerBase^ baseDataManager ) {
	m_baseDataManager = baseDataManager;
}

// get base manager
ClrDataManagerBase^ ClrDataManager::getBaseDataManager() {
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	return baseDataManager;
}

// on open sample
void ClrDataManager::onOpenSample( kome::objects::Sample* sample ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// sample
	SampleWrapper^ s = nullptr;
	if( sample != NULL ) {
		s = gcnew SampleWrapper( *sample );
	}

	// open
	baseDataManager->onOpenSample( s );
}

// on close sample
void ClrDataManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// sample
	SampleWrapper^ s = nullptr;
	if( sample != NULL ) {
		s = gcnew SampleWrapper( *sample );
	}

	// close
	baseDataManager->onCloseSample( s, deleting );
}

// on open spectrum
void ClrDataManager::onOpenSpectrum( kome::objects::Spectrum* spec ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// spectrum
	SpectrumWrapper^ s = nullptr;
	if( spec != NULL ) {
		s = gcnew SpectrumWrapper( *spec );
	}

	// open
	baseDataManager->onOpenSpectrum( s );
}

// on close spectrum
void ClrDataManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// spectrum
	SpectrumWrapper^ s = nullptr;
	if( spec != NULL ) {
		s = gcnew SpectrumWrapper( *spec );
	}

	// close
	baseDataManager->onCloseSpectrum( s, deleting );
}

// on open chromatogram
void ClrDataManager::onOpenChromatogram( kome::objects::Chromatogram* chrom ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// chromatogram
	ChromatogramWrapper^ c = nullptr;
	if( chrom != NULL ) {
		c = gcnew ChromatogramWrapper( *chrom );
	}

	// open
	baseDataManager->onOpenChromatogram( c );
}

// on close chromatogram
void ClrDataManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// chromatogram
	ChromatogramWrapper^ c = nullptr;
	if( chrom != NULL ) {
		c = gcnew ChromatogramWrapper( *chrom );
	}

	// close
	baseDataManager->onCloseChromatogram( c, deleting );
}

// on open data map
void ClrDataManager::onOpenDataMap( kome::objects::DataMapInfo* dataMap ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// data map
	DataMapInfoWrapper^ dp = nullptr;
	if( dataMap != NULL ) {
		dp = gcnew DataMapInfoWrapper( *dataMap );
	}

	// open
	baseDataManager->onOpenDataMap( dp );
}

// on close data map
void ClrDataManager::onCloseDataMap( kome::objects::DataMapInfo* dataMap, const bool deleting ) {
	// base manager
	ClrDataManagerBase^ baseDataManager = m_baseDataManager;
	if( baseDataManager == nullptr ) {
		return;
	}

	// data map
	DataMapInfoWrapper^ dp = nullptr;
	if( dataMap != NULL ) {
		dp = gcnew DataMapInfoWrapper( *dataMap );
	}

	// close
	baseDataManager->onCloseDataMap( dp, deleting );
}
