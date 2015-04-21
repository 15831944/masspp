/**
 * @file DefaultDataManager.cpp
 * @brief implements of DefaultDataManager class
 *
 * @author S.Tanaka
 * @date 2008.10.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DefaultDataManager.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DefaultDataManager::DefaultDataManager() {
}

// destructor
DefaultDataManager::~DefaultDataManager() {
}

// on open sample
void DefaultDataManager::onOpenSample( Sample* sample ) {
}

// on close sample
void DefaultDataManager::onCloseSample( Sample* sample, const bool deleting ) {
}

// on open spectrum
void DefaultDataManager::onOpenSpectrum( Spectrum* spec ) {
}

// on close spectrum
void DefaultDataManager::onCloseSpectrum( Spectrum* spec, const bool deleting ) {
}

// on open chromatogram
void DefaultDataManager::onOpenChromatogram( Chromatogram* chrom ) {
}

// on close chromatogram
void DefaultDataManager::onCloseChromatogram( Chromatogram* chrom, const bool deleting ) {
}

// on open data map
void DefaultDataManager::onOpenDataMap( DataMapInfo* dataMap ) {
}

// on close data map
void DefaultDataManager::onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) {
}
