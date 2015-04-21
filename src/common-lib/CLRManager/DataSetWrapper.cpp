/**
 * @file DataSetWrapper.cpp
 * @brief implements of DataSetWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataSetWrapper.h"

#include "SampleWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "XYDataWrapper.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"



using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataSetWrapper::DataSetWrapper( DataGroupNodeWrapper^ group )
		: m_dataSet( ClrObjectPool::getInstance().createDataSet( group ) ) {
	m_destroyFlg = true;
}

// constructor
DataSetWrapper::DataSetWrapper( kome::objects::DataSet& dataSet )
		: m_dataSet( dataSet ) {
	m_destroyFlg = false;
}

// destructor
DataSetWrapper::~DataSetWrapper() {
	if( m_destroyFlg ) {
		ClrObjectPool::getInstance().destroyDataSet( m_dataSet );
	}
}

// get DataSet object
kome::objects::DataSet& DataSetWrapper::getDataSet() {
	return m_dataSet;
}

// get sample
SampleWrapper^ DataSetWrapper::getSample() {
	kome::objects::Sample* sample = m_dataSet.getSample();
	if( sample == NULL ) {
		return nullptr;
	}

	return gcnew SampleWrapper( *sample );
}

// get spectrum group
DataGroupNodeWrapper^ DataSetWrapper::getGroup() {
	kome::objects::DataGroupNode* group = m_dataSet.getGroup();
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew DataGroupNodeWrapper( *group );
}

// get the number of spectra
unsigned int DataSetWrapper::getNumberOfSpectra() {
	return m_dataSet.getNumberOfSpectra();
}

// get spectrum
SpectrumWrapper^ DataSetWrapper::getSpectrum( unsigned int index ) {
	kome::objects::Spectrum* spec = m_dataSet.getSpectrum( index );
	return ClrObjectTool::createSpectrumWrapper( spec );
}

// get the number of chromatograms
unsigned int DataSetWrapper::getNumberOfChromatograms() {
	return m_dataSet.getNumberOfChromatograms();
}

// get chromatograms
ChromatogramWrapper^ DataSetWrapper::getChromatogram( unsigned int index ) {
	kome::objects::Chromatogram* chrom = m_dataSet.getChromatogram( index );
	return ClrObjectTool::createChromatogramWrapper( chrom );
}

// sort spectra
void DataSetWrapper::sortSpectra() {
	m_dataSet.sortSpectra();
}

// clear spectra
void DataSetWrapper::clearSpectra() {
	m_dataSet.clearSpectra();
}

// add spectrum
void DataSetWrapper::addSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	m_dataSet.addSpectrum( s );
}

// remove spectrum
void DataSetWrapper::removeSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	m_dataSet.removeSpectrum( s );
}

// insert spectrum
void DataSetWrapper::insertSpectrum( SpectrumWrapper^ spec, int idx ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	m_dataSet.insertSpectrum( s, idx );
}

// clear chroamtograms
void DataSetWrapper::clearChromatograms() {
	m_dataSet.clearChromatograms();
}

// add chromatogram
void DataSetWrapper::addChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	m_dataSet.addChromatogram( c );
}

// remove chromatogram
void DataSetWrapper::removeChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	m_dataSet.removeChromatogram( c );
}

// insert chromatogram
void DataSetWrapper::insertChromatogram( ChromatogramWrapper^ chrom, int idx ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	m_dataSet.insertChromatogram( c, idx );
}
