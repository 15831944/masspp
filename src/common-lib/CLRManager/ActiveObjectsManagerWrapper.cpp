/**
 * @file ActiveObjectsManagerWrapper.cpp
 * @brief implements of ActiveObjectsManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ActiveObjectsManagerWrapper.h"

#include "SampleSetWrapper.h"
#include "SampleWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "DataSetWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "DataMapInfoWrapper.h"
#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ActiveObjectsManagerWrapper::ActiveObjectsManagerWrapper( kome::objects::ActiveObjectsManager& mgr )
		: DataManagerWrapper( m_mgr ), m_mgr( mgr ) {
}

// destructor
ActiveObjectsManagerWrapper::~ActiveObjectsManagerWrapper() {
}

// get manager object
kome::objects::ActiveObjectsManager& ActiveObjectsManagerWrapper::getActiveObjectsManager() {
	return m_mgr;
}

// get number of opened sample
unsigned int ActiveObjectsManagerWrapper::getNumberOfOpenedSamples() {
	return m_mgr.getNumberOfOpenedSamples();
}

// get sample
SampleWrapper^ ActiveObjectsManagerWrapper::getOpenedSample( unsigned int idx ) {
	// sample
	kome::objects::Sample* s = m_mgr.getOpenedSample( idx );

	// get wrapper object
	SampleWrapper^ sample = ClrObjectTool::createSampleWrapper( s );
	return sample;
}

// set the active sample
void ActiveObjectsManagerWrapper::setActiveSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	m_mgr.setActiveSample( s );
}

// get the active sample
SampleWrapper^ ActiveObjectsManagerWrapper::getActiveSample() {
	// get wrapper object
	SampleWrapper^ sample = ClrObjectTool::createSampleWrapper( m_mgr.getActiveSample() );
	return sample;
}

// get filtered spectra
DataSetWrapper^ ActiveObjectsManagerWrapper::getFilteredDataSet( SampleWrapper^ sample ) {
	// get group
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );

	// get filtered spectra
	kome::objects::DataSet* ds = m_mgr.getFilteredDataSet( s );
	DataSetWrapper^ dataSet = ClrObjectTool::createDataSetWrapper( ds );

	return dataSet;
}

// clear history
void ActiveObjectsManagerWrapper::clearFileHistory() {
	m_mgr.clearFileHistory();
}

// add history
void ActiveObjectsManagerWrapper::addHistoryFile( System::String^ path ) {
	m_mgr.addHistoryFile(
		ClrObjectTool::convertString( path ).c_str()
	);
}

// remove history
void ActiveObjectsManagerWrapper::removeHistoryFile( System::String^ path ) {
	m_mgr.removeHistoryFile(
		ClrObjectTool::convertString( path ).c_str()
	);
}

// get the number of history files
unsigned int ActiveObjectsManagerWrapper::getNumberOfHistoryFiles() {
	return m_mgr.getNumberOfHistoryFiles();
}

// get history file path
System::String^ ActiveObjectsManagerWrapper::getHistoryFile( unsigned int idx ) {
	return ClrObjectTool::convertString(
		m_mgr.getHistoryFile( idx ),
		NULL
	);
}

// set sample tag name
void ActiveObjectsManagerWrapper::setSampleTagName( SampleWrapper^ sample, System::String^ name ) {
	m_mgr.setSampleTagName(
		ClrObjectTool::getSample( sample ),
		ClrObjectTool::convertString( name ).c_str()
	);
}

// get sample tag name
System::String^ ActiveObjectsManagerWrapper::getSampleTagName( SampleWrapper^ sample ) {
	return ClrObjectTool::convertString(
		m_mgr.getSampleTagName( ClrObjectTool::getSample( sample ) ),
		NULL
	);
}

// get instance
ActiveObjectsManagerWrapper^ ActiveObjectsManagerWrapper::getInstance() {
	kome::objects::ActiveObjectsManager& mgr = kome::objects::ActiveObjectsManager::getInstance();
	return gcnew ActiveObjectsManagerWrapper( mgr );
}
