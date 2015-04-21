/**
 * @file SpectraChromatogramWrapper.cpp
 * @brief implements of SpectraChromatogramWrapper class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraChromatogramWrapper.h"

#include "SpectrumWrapper.h"
#include "DataSetWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "XYDataWrapper.h"
#include "SampleWrapper.h"

#include "ClrSample.h"
#include "ClrSpectrum.h"
#include "ClrObjectTool.h"
#include "ConversionArrays.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectraChromatogramWrapper::SpectraChromatogramWrapper( kome::objects::SpectraChromatogram& chromatogram )
		: ChromatogramWrapper( chromatogram ), m_groupChrom( chromatogram ) {
}

// destructor
SpectraChromatogramWrapper::~SpectraChromatogramWrapper() {
}

// get chromatogram object
kome::objects::SpectraChromatogram& SpectraChromatogramWrapper::getSpectraChromatogram() {
	return m_groupChrom;
}

// get spectrum group
DataSetWrapper^ SpectraChromatogramWrapper::getMemberSpectra() {
	return gcnew DataSetWrapper( m_groupChrom.getMemberSpectra() );
}

// set m/z range
void SpectraChromatogramWrapper::setMzRange( double minMz, double maxMz ) {
	m_groupChrom.setMzRange( minMz, maxMz );
}

// set min m/z
void SpectraChromatogramWrapper::setMinMz( double mz ) {
	m_groupChrom.setMinMz( mz );
}

// set max m/z
void SpectraChromatogramWrapper::setMaxMz( double mz ) {
	m_groupChrom.setMaxMz( mz );
}

// get min m/z
double SpectraChromatogramWrapper::getMinMz() {
	return m_groupChrom.getMinMz();
}

// get max m/z
double SpectraChromatogramWrapper::getMaxMz() {
	return m_groupChrom.getMaxMz();
}

// set RT range
void SpectraChromatogramWrapper::setRtRange( double minRt, double maxRt ) {
	m_groupChrom.setRtRange( minRt, maxRt );
}

// set min RT
void SpectraChromatogramWrapper::setMinRt( double rt ) {
	m_groupChrom.setMinRt( rt );
}

// set max RT
void SpectraChromatogramWrapper::setMaxRt( double rt ) {
	m_groupChrom.setMaxRt( rt );
}

// get min RT
double SpectraChromatogramWrapper::getMinRt() {
	return m_groupChrom.getMinRt();
}

// get max RT
double SpectraChromatogramWrapper::getMaxRt() {
	return m_groupChrom.getMaxRt();
}

// set filter
void SpectraChromatogramWrapper::setFilter( unsigned char filter ) {
	m_groupChrom.setFilter( filter );
}

// get filter
unsigned char SpectraChromatogramWrapper::getFilter() {
	return m_groupChrom.getFilter();
}

// set TIC
void SpectraChromatogramWrapper::setTIC() {
	m_groupChrom.setTIC();
}

// set BPC
void SpectraChromatogramWrapper::setBPC() {
	m_groupChrom.setBPC();
}

// is TIC
bool SpectraChromatogramWrapper::isTIC() {
	return m_groupChrom.isTIC();
}

// is BPC
bool SpectraChromatogramWrapper::isBPC() {
	return m_groupChrom.isBPC();
}
