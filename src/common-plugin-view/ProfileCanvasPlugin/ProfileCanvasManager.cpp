/**
 * @file ProfileCanvasManager.cpp
 * @brief interfaces of ProfileCanvasManager class
 *
 * @author M.Izumi
 * @date 2012.02.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "ProfileCanvasManager.h"
#include "ProfileCanvasOperation.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
ProfileCanvasManager::ProfileCanvasManager(){
	m_spec = NULL;
	m_chrom = NULL;
}

// destructor
ProfileCanvasManager::~ProfileCanvasManager(){
}

// on open spectrum
void ProfileCanvasManager::onOpenSpectrum( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// on open chromatogram
void ProfileCanvasManager::onOpenChromatogram( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get instance
ProfileCanvasManager& ProfileCanvasManager::getInstance() {
	// create object (This is the only object.)
	static ProfileCanvasManager mgr;

	return mgr;
}
