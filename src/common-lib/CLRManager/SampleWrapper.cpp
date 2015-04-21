/**
 * @file SampleWrapper.cpp
 * @brief implements of SampleWrapper class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleWrapper.h"

#include "SampleSetWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "ClrObjectTool.h"
#include "PeaksWrapper.h"
#include "SpectrumWrapper.h"
#include "DataSetWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SampleWrapper::SampleWrapper( kome::objects::Sample& sample ) : m_sample( sample ) {
}

// destructor
SampleWrapper::~SampleWrapper() {
}

// get sample object to be wrapped
kome::objects::Sample& SampleWrapper::getSample() {
	return m_sample;
}

// get sample set
SampleSetWrapper^ SampleWrapper::getSampleSet() {
	kome::objects::SampleSet* sampleSet = m_sample.getSampleSet();
	if( sampleSet == NULL ) {
		return nullptr;
	}
	return gcnew SampleSetWrapper( *sampleSet );
}

// set sample index
void SampleWrapper::setSampleIndex( int index ) {
	m_sample.setSampleIndex( index );
}

// get sample index
int SampleWrapper::getSampleIndex() {
	return m_sample.getSampleIndex();
}

// set sample name
void SampleWrapper::setName( System::String^ name ) {
	m_sample.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get sample name
System::String^ SampleWrapper::getName() {
	return ClrObjectTool::convertString( m_sample.getName(), NULL );
}

// set sample type
void SampleWrapper::setType( System::String^ type ) {
	m_sample.setType( ClrObjectTool::convertString( type ).c_str() );
}

// get sample type
System::String^ SampleWrapper::getType() {
	return ClrObjectTool::convertString( m_sample.getType(), NULL );
}

// set instrument
void SampleWrapper::setInstrument( System::String^ instrument ) {
	m_sample.setInstrument( ClrObjectTool::convertString( instrument ).c_str() );
}

// get instrument
System::String^ SampleWrapper::getInstrument() {
	return ClrObjectTool::convertString( m_sample.getInstrument(), NULL );
}

// set MS company
void SampleWrapper::setMsCompany( System::String^ company ) {
	m_sample.setMsCompany( ClrObjectTool::convertString( company ).c_str() );
}

// get MS company
System::String^ SampleWrapper::getMsCompany() {
	return ClrObjectTool::convertString( m_sample.getMsCompany(), NULL );
}

// set software name
void SampleWrapper::setSoftwareName( System::String^ name ) {
	m_sample.setSoftwareName( ClrObjectTool::convertString( name ).c_str() );
}

// get software name
System::String^ SampleWrapper::getSoftwareName() {
	return ClrObjectTool::convertString( m_sample.getSoftwareName(), NULL );
}

// set software version
void SampleWrapper::setSoftwareVersion( System::String^ version ) {
	m_sample.setSoftwareVersion( ClrObjectTool::convertString( version ).c_str() );
}

// get software version
System::String^ SampleWrapper::getSoftwareVersion() {
	return ClrObjectTool::convertString( m_sample.getSoftwareVersion(), NULL );
}

// set opened flag
void SampleWrapper::setOpened( bool opened ) {
	m_sample.setOpened( opened );
}

// judges whether this sample is opened
bool SampleWrapper::isOpened() {
	return m_sample.isOpened();
}

// get root spectrum group
DataGroupNodeWrapper^ SampleWrapper::getRootDataGroupNode() {
	kome::objects::DataGroupNode* group = m_sample.getRootDataGroupNode();
	if( group == NULL ) {
		return nullptr;
	}
	return gcnew DataGroupNodeWrapper( *group );
}

// get hidden data set
DataSetWrapper^ SampleWrapper::getHiddenDataSet() {
	return ClrObjectTool::createDataSetWrapper( m_sample.getHiddenDataSet() );
}

// get the number of spectrum groups
unsigned int SampleWrapper::getNumberOfGroups() {
	return m_sample.getNumberOfGroups();
}

// get spectrum group
DataGroupNodeWrapper^ SampleWrapper::getGroup( unsigned int index ) {
	kome::objects::DataGroupNode* group = m_sample.getGroup( index );
	if( group == NULL ) {
		return nullptr;
	}
	return gcnew DataGroupNodeWrapper( *group );
}

// set parallel readable
void SampleWrapper::setParallelReadable( bool readable ) {
	m_sample.setParallelReadable( readable );
}

// get parallel readable
bool SampleWrapper::isParallelReadable() {
	return m_sample.isParallelReadable();
}

// open sample
bool SampleWrapper::openSample() {
	return m_sample.openSample();
}

// close sample
bool SampleWrapper::closeSample() {
	return m_sample.closeSample();
}

// API peak detection is supported.
bool SampleWrapper::isSupportedAPIPeaks() {
	return m_sample.isSupportedAPIPeaks();
}

// detect peaks by API
void SampleWrapper::detectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks ) {
	m_sample.detectPeaksByAPI(
		ClrObjectTool::getSpectrum( spec ),
		ClrObjectTool::getPeaks( peaks )
	);
}

// set ID
void SampleWrapper::setSampleId( int id ) {
	m_sample.setSampleId( id );
}

// get ID
int SampleWrapper::getSampleId() {
	return m_sample.getSampleId();
}

// check whether the specified key is common or not
bool SampleWrapper::isCommonProperty( System::String^ key ) {
	return kome::objects::Sample::isCommonProperty( ClrObjectTool::convertString( key ).c_str() );
}
