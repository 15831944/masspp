/**
 * @file ClrXYDataOperation.cpp
 * @brief implements of ClrXYDataOperation class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrXYDataOperation.h"

#include "ClrXYDataOperationBase.h"
#include "XYDataWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "ClrObjectPool.h"
#include "SampleWrapper.h"

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrXYDataOperation::ClrXYDataOperation() {
	m_baseOperation = nullptr;
}

// destructor
ClrXYDataOperation::~ClrXYDataOperation() {
	ClrObjectPool::getInstance().removeOperation( *this );
}

// set base operation
void ClrXYDataOperation::setBaseOperation( ClrXYDataOperationBase^ base ) {
	m_baseOperation = base;
}

// get base operation
ClrXYDataOperationBase^ ClrXYDataOperation::getBaseOperation() {
	ClrXYDataOperationBase^ base = m_baseOperation;
	return base;
}

// on update chromatogram
void ClrXYDataOperation::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Chromatogram& chrom
) {
	// get base object
	ClrXYDataOperationBase^ base = m_baseOperation;

	// update
	base->update(
		gcnew XYDataWrapper( src ),
		gcnew XYDataWrapper( dst ),
		gcnew ChromatogramWrapper( chrom )
	);
}

// on update spectrum
void ClrXYDataOperation::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Spectrum& spec
) {
	// get base object
	ClrXYDataOperationBase^ base = m_baseOperation;

	// update
	base->onUpdate(
		gcnew XYDataWrapper( src ),
		gcnew XYDataWrapper( dst ),
		gcnew SpectrumWrapper( spec )
	);
}

// on update sample
void ClrXYDataOperation::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Sample& sample
) {
	// get base object
	ClrXYDataOperationBase^ base = m_baseOperation;

	// update
	base->onUpdate(
		gcnew XYDataWrapper( src ),
		gcnew XYDataWrapper( dst ),
		gcnew SampleWrapper( sample )
	);
}
