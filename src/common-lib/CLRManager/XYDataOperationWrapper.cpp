/**
 * @file XYDataOperationWrapper.cpp
 * @brief implements of XYDataOperationWrapper class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "XYDataOperationWrapper.h"

#include "XYDataWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "ClrObjectTool.h"
#include "SampleWrapper.h"

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// cosntructor
XYDataOperationWrapper::XYDataOperationWrapper( kome::objects::XYDataOperation& operation ) 
		: m_operation( operation ) {
}

// destructor
XYDataOperationWrapper::~XYDataOperationWrapper() {
}

// get operaion object
kome::objects::XYDataOperation& XYDataOperationWrapper::getXYDataOperation() {
	return m_operation;
}

// set name
void XYDataOperationWrapper::setName( System::String^ name ) {
	m_operation.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ XYDataOperationWrapper::getName() {
	return ClrObjectTool::convertString( m_operation.getName(), NULL );
}

// update chromatogram
void XYDataOperationWrapper::update( XYDataWrapper^ src, XYDataWrapper^ dst, ChromatogramWrapper^ chrom ) {
	if( src == nullptr || dst == nullptr || chrom == nullptr ) {
		return;
	}
	m_operation.update( src->getXYData(), dst->getXYData(), chrom->getChromatogram() );
}

// update spectrum
void XYDataOperationWrapper::update( XYDataWrapper^ src, XYDataWrapper^ dst, SpectrumWrapper^ spec ) {
	if( src == nullptr || dst == nullptr || spec == nullptr ) {
		return;
	}
	m_operation.update( src->getXYData(), dst->getXYData(), spec->getSpectrum() );
}

// update sample
void XYDataOperationWrapper::update( XYDataWrapper^ src, XYDataWrapper^ dst, SampleWrapper^ sample ) {
	if( src == nullptr || dst == nullptr || sample == nullptr ) {
		return;
	}
	m_operation.update( src->getXYData(), dst->getXYData(), sample->getSample() );
}
