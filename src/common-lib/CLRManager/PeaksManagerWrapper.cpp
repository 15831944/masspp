/**
 * @file PeaksManagerWrapper.cpp
 * @brief implements of PeaksManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksManagerWrapper.h"


#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "PeaksWrapper.h"
#include "Peaks2DWrapper.h"
#include "PeakElementWrapper.h"
#include "CLRObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeaksManagerWrapper::PeaksManagerWrapper( kome::objects::PeaksManager& mgr )
		: DataManagerWrapper( m_mgr ), m_mgr( mgr ) {
}

// destrucotr
PeaksManagerWrapper::~PeaksManagerWrapper() {
}

// get peaks manager
kome::objects::PeaksManager& PeaksManagerWrapper::getPeaksManager() {
	return m_mgr;
}

// get spectrum peaks
PeaksWrapper^ PeaksManagerWrapper::getPeaks( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// peaks
	kome::objects::Peaks* peaks = m_mgr.getPeaks( s );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew PeaksWrapper( *peaks );
}

// get chromatogram peaks
PeaksWrapper^ PeaksManagerWrapper::getPeaks( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// peaks
	kome::objects::Peaks* peaks = m_mgr.getPeaks( c );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew PeaksWrapper( *peaks );
}

// get 2D peaks
Peaks2DWrapper^ PeaksManagerWrapper::getPeaks( DataGroupNodeWrapper^ group ) {
	// spectrum group
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &group->getDataGroupNode();
	}

	// peaks
	kome::objects::Peaks2D* peaks = m_mgr.getPeaks( g );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew Peaks2DWrapper( *peaks );
}

// create spectrum peaks
PeaksWrapper^ PeaksManagerWrapper::createPeaks( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// peaks
	kome::objects::Peaks* peaks = m_mgr.createPeaks( s );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew PeaksWrapper( *peaks );
}

// create chromatogram peaks
PeaksWrapper^ PeaksManagerWrapper::createPeaks( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// peaks
	kome::objects::Peaks* peaks = m_mgr.createPeaks( c );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew PeaksWrapper( *peaks );
}

// create 2D peaks
Peaks2DWrapper^ PeaksManagerWrapper::createPeaks( DataGroupNodeWrapper^ group ) {
	// spectrum group
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &group->getDataGroupNode();
	}

	// peaks
	kome::objects::Peaks2D* peaks = m_mgr.createPeaks( g );
	if( peaks == NULL ) {
		return nullptr;
	}
	return gcnew Peaks2DWrapper( *peaks );
}

// judge whether spectrum has peaks
bool PeaksManagerWrapper::hasPeaks( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}
	return m_mgr.hasPeaks( s );
}

// judge whether chromatogram has peaks
bool PeaksManagerWrapper::hasPeaks( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}
	return m_mgr.hasPeaks( c );
}

// judge whether spectrum group has peaks
bool PeaksManagerWrapper::hasPeaks( DataGroupNodeWrapper^ group ) {
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &group->getDataGroupNode();
	}
	return m_mgr.hasPeaks( g );
}

// delete spectrum peaks
void PeaksManagerWrapper::deletePeaks( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}
	m_mgr.deletePeaks( s );
}

// delete chromatogram peaks
void PeaksManagerWrapper::deletePeaks( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}
	m_mgr.deletePeaks( c );
}

// delete 2D peaks
void PeaksManagerWrapper::deletePeaks( DataGroupNodeWrapper^ group ) {
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &group->getDataGroupNode();
	}
	m_mgr.deletePeaks( g );
}

// update peaks
void PeaksManagerWrapper::updatePeaks( SpectrumWrapper^ spec, PeaksWrapper^ peaks, cli::array< PeakElementWrapper^ >^ addPeaks ) {
	// spectrum
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::objects::Peaks* p = ClrObjectTool::getPeaks( peaks );

	// array
	std::vector< kome::objects::PeakElement > arr;
	int num = addPeaks->Length;
	for( int i = 0; i < num; i++ ) {
		kome::objects::PeakElement elm( NULL );
		elm.setX( addPeaks[ i ]->getX() );
		elm.setY( addPeaks[ i ]->getY() );
		elm.setApex( addPeaks[ i ]->getApexX(), addPeaks[ i ]->getApexY() );
		elm.setLeft( addPeaks[ i ]->getLeftX(), addPeaks[ i ]->getLeftY() );
		elm.setRight( addPeaks[ i ]->getRightX(), addPeaks[ i ]->getRightY() );

		arr.push_back( elm );
	}

	// update peaks
	m_mgr.updatePeaks( *s, p, arr );
}

// get instance
PeaksManagerWrapper^ PeaksManagerWrapper::getInstance() {
	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	return gcnew PeaksManagerWrapper( mgr );
}
