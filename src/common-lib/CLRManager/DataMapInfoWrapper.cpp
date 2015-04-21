/**
 * @file DataMapInfoWrapper.cpp
 * @brief implements of DataMapInfoWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataMapInfoWrapper.h"

#include "DataGroupNodeWrapper.h"
#include "DataSetWrapper.h"
#include "SpectrumWrapper.h"
#include "ProgressWrapper.h"
#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataMapInfoWrapper::DataMapInfoWrapper( kome::objects::DataMapInfo& dataMap ) 
		: m_dataMap( dataMap ) {
}

// destructor
DataMapInfoWrapper::~DataMapInfoWrapper() {
}

// gets data map object
kome::objects::DataMapInfo& DataMapInfoWrapper::getDataMapInfo() {
	return m_dataMap;
}

// get row size
unsigned int DataMapInfoWrapper::getRowSize() {
	return m_dataMap.getRowSize();
}

// get column size
unsigned int DataMapInfoWrapper::getColSize() {
	return m_dataMap.getColSize();
}

// get raw data
DataGroupNodeWrapper^ DataMapInfoWrapper::getGroup() {
	kome::objects::DataGroupNode& group = m_dataMap.getGroup();
	return ClrObjectTool::createDataGroupNodeWrapper( &group );
}

// get min m/z
double DataMapInfoWrapper::getMinMz() {
	return m_dataMap.getMinMz();
}

// get max m/z
double DataMapInfoWrapper::getMaxMz() {
	return m_dataMap.getMaxMz();
}

// get min RT
double DataMapInfoWrapper::getMinRt() {
	return m_dataMap.getMinRt();
}

// get max RT
double DataMapInfoWrapper::getMaxRt() {
	return m_dataMap.getMaxRt();
}

// get min intensity
double DataMapInfoWrapper::getMinIntensity() {
	return m_dataMap.getMinIntensity();
}

// get max intensity
double DataMapInfoWrapper::getMaxIntensity() {
	return m_dataMap.getMaxIntensity();
}

// get intensity
double DataMapInfoWrapper::getIntensity( unsigned int row, unsigned int col ) {
	return m_dataMap.getIntensity( row, col );
}

// set completed flag
void DataMapInfoWrapper::setCompleted( bool completed ) {
	m_dataMap.setCompleted( completed );
}

// get completed flag
bool DataMapInfoWrapper::isCompleted() {
	return m_dataMap.isCompleted();
}

// set intensity range
void DataMapInfoWrapper::setIntensityRange( double minInt, double maxInt ) {
	m_dataMap.setIntensityRange( minInt, maxInt );
}

// set data map range
bool DataMapInfoWrapper::setRange(
		double minMz,
		double maxMz,
		double minRt,
		double maxRt,
		const char* filterName, 
		ProgressWrapper^ progress
) {
	// get object
	kome::core::Progress& p = kome::core::Progress::getIgnoringProgress();
	if( progress != nullptr ) {
		p = progress->getProgress();
	}

	// set spectra
	return m_dataMap.setRange( minMz, maxMz, minRt, maxRt, filterName, p ); // add parameter [filterName] @date 2012.09.07 <Mod> M.Izumi
}

// set auto commit flag
void DataMapInfoWrapper::setAutoCommit( bool autoCommit ) {
	m_dataMap.setAutoCommit( autoCommit );
}

// get auto commit flag
bool DataMapInfoWrapper::isAutoCommit() {
	return m_dataMap.isAutoCommit();
}

// commit
void DataMapInfoWrapper::commit() {
	m_dataMap.commit();
}

// rollback
void DataMapInfoWrapper::rollback() {
	m_dataMap.rollback();
}
