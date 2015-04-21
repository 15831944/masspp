/**
 * @file PointsManagerWrapper.cpp
 * @brief implements of PointsManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PointsManagerWrapper.h"

#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "XYDataWrapper.h"
#include "XYDataOperationWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PointsManagerWrapper::PointsManagerWrapper( kome::objects::PointsManager& mgr )
		: DataManagerWrapper( mgr ), m_mgr( mgr ) {

}

// destructor
PointsManagerWrapper::~PointsManagerWrapper() {
}

// get points manager
kome::objects::PointsManager& PointsManagerWrapper::getPointsManager() {
	return m_mgr;
}

// get xy data of spectrum
XYDataWrapper^ PointsManagerWrapper::getXYData( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// xy data
	kome::core::XYData* xyData = m_mgr.getXYData( s );
	if( xyData == NULL ) {
		return nullptr;
	}
	return gcnew XYDataWrapper( *xyData );
}

// get xy data of chromatogram
XYDataWrapper^ PointsManagerWrapper::getXYData( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// xy data
	kome::core::XYData* xyData = m_mgr.getXYData( c );
	if( xyData == NULL ) {
		return nullptr;
	}
	return gcnew XYDataWrapper( *xyData );
}

// delete xy data of spectrum
void PointsManagerWrapper::deleteXYData( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// delete
	m_mgr.deleteXYData( s );
}

// delete xy data of chromatogram
void PointsManagerWrapper::deleteXYData( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// delete
	m_mgr.deleteXYData( c );
}

// add spectrum operation
void PointsManagerWrapper::addOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// operation
	kome::objects::XYDataOperation* o = NULL;
	if( operation != nullptr ) {
		o = &operation->getXYDataOperation();
	}

	// add
	m_mgr.addOperation( s, o );
}

// add chromatogram operation
void PointsManagerWrapper::addOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// operation
	kome::objects::XYDataOperation* o = NULL;
	if( operation != nullptr ) {
		o = &operation->getXYDataOperation();
	}

	// add
	m_mgr.addOperation( c, o );
}

// remove spectrum operation
void PointsManagerWrapper::removeOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// operation
	kome::objects::XYDataOperation* o = NULL;
	if( operation != nullptr ) {
		o = &operation->getXYDataOperation();
	}

	// remove
	m_mgr.removeOperation( s, o );
}

// remove chromatogram operation
void PointsManagerWrapper::removeOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// operation
	kome::objects::XYDataOperation* o = NULL;
	if( operation != nullptr ) {
		o = &operation->getXYDataOperation();
	}

	// remove
	m_mgr.removeOperation( c, o );
}

// clear spectrum operations
void PointsManagerWrapper::clearOperations( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// clear
	m_mgr.clearOperations( s );
}

// clear chromatogram opertions
void PointsManagerWrapper::clearOperations( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// clear
	m_mgr.clearOperations( c );
}

// get the number of spectrum operations
unsigned int PointsManagerWrapper::getNumberOfOperations( SpectrumWrapper^ spec ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// get
	return m_mgr.getNumberOfOperations( s );
}

// get the number of chromatogram operations
unsigned int PointsManagerWrapper::getNumberOfOperations( ChromatogramWrapper^ chrom ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// get
	return m_mgr.getNumberOfOperations( c );
}

// get spectrum operation
XYDataOperationWrapper^ PointsManagerWrapper::getOperation( SpectrumWrapper^ spec, unsigned int index ) {
	// spectrum
	kome::objects::Spectrum* s = NULL;
	if( spec != nullptr ) {
		s = &spec->getSpectrum();
	}

	// operation
	kome::objects::XYDataOperation* operation = m_mgr.getOperation( s, index );
	if( operation == NULL ) {
		return nullptr;
	}
	return gcnew XYDataOperationWrapper( *operation );
}

// get chromatogram operation
XYDataOperationWrapper^ PointsManagerWrapper::getOperation( ChromatogramWrapper^ chrom, unsigned int index ) {
	// chromatogram
	kome::objects::Chromatogram* c = NULL;
	if( chrom != nullptr ) {
		c = &chrom->getChromatogram();
	}

	// operation
	kome::objects::XYDataOperation* operation = m_mgr.getOperation( c, index );
	if( operation == NULL ) {
		return nullptr;
	}
	return gcnew XYDataOperationWrapper( *operation );
}

// get instance
PointsManagerWrapper^ PointsManagerWrapper::getInstance() {
	kome::objects::PointsManager& mgr = kome::objects::PointsManager::getInstance();
	return gcnew PointsManagerWrapper( mgr );
}
