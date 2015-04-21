/**
 * @file ChromatogramWrapper.cpp
 * @brief implements of ChromatogramWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramWrapper.h"

#include "DataGroupNodeWrapper.h"
#include "XYDataWrapper.h"
#include "SampleWrapper.h"
#include "PropertiesWrapper.h"

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
ChromatogramWrapper::ChromatogramWrapper( kome::objects::Chromatogram& chromatogram )
		: m_chromatogram( chromatogram ) {
}

// destructor
ChromatogramWrapper::~ChromatogramWrapper() {
}

// get chromatogram object
kome::objects::Chromatogram& ChromatogramWrapper::getChromatogram() {
	return m_chromatogram;
}

// get sample
SampleWrapper^ ChromatogramWrapper::getSample() {
	// get sample
	kome::objects::Sample* sample = m_chromatogram.getSample();
	if( sample == NULL ) {
		return nullptr;
	}

	// cast
	ClrSample* clrSample = dynamic_cast< ClrSample* >( sample );
	if( clrSample != NULL ) {
		return clrSample->getBaseSample();
	}

	return gcnew SampleWrapper( *sample );
}

// set name
void ChromatogramWrapper::setName( System::String^ name ) {
	m_chromatogram.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ ChromatogramWrapper::getName() {
	return ClrObjectTool::convertString( m_chromatogram.getName(), NULL );
}

// set m/z
void ChromatogramWrapper::setMz( double mz ) {
	m_chromatogram.setMz( mz );
}

// get m/z
double ChromatogramWrapper::getMz() {
	return m_chromatogram.getMz();
}

// set spectrum group
void ChromatogramWrapper::setGroup( DataGroupNodeWrapper^ group ) {
	// spectrum group
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &( group->getDataGroupNode() );
	}

	// set
	m_chromatogram.setGroup( g );
}

// get spectrum group
DataGroupNodeWrapper^ ChromatogramWrapper::getGroup() {
	kome::objects::DataGroupNode* group = m_chromatogram.getGroup();
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew DataGroupNodeWrapper( *group );
}

// set icon
void ChromatogramWrapper::setIcon( System::String^ icon ) {
	m_chromatogram.setIcon( ClrObjectTool::convertString( icon ).c_str() );
}

// get icon
System::String^ ChromatogramWrapper::getIcon() {
	return ClrObjectTool::convertString( m_chromatogram.getIcon(), NULL );
}

// set title
void ChromatogramWrapper::setTitle( System::String^ title ) {
	m_chromatogram.setTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get title
System::String^ ChromatogramWrapper::getTitle() {
	return ClrObjectTool::convertString( m_chromatogram.getTitle(), NULL );
}

// get properties
PropertiesWrapper^ ChromatogramWrapper::getProperties() {
	kome::core::Properties& props = m_chromatogram.getProperties();

	return gcnew PropertiesWrapper( props );
}

// set auto created flag
void ChromatogramWrapper::setAutoCreated( bool autoCreated ) {
	m_chromatogram.setAutoCreated( autoCreated );
}

// get auto created flag
bool ChromatogramWrapper::isAutoCreated() {
	return m_chromatogram.isAutoCreated();
}

// set visible
void ChromatogramWrapper::setVisible( bool visible ) {
	m_chromatogram.setVisible( visible );
}

// get visible
bool ChromatogramWrapper::isVisible() {
	return m_chromatogram.isVisible();
}

// set original chromatogram
void ChromatogramWrapper::setOrgChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	m_chromatogram.setOrgChromatogram( c );
}

// get original chromatogram
ChromatogramWrapper^ ChromatogramWrapper::getOrgChromatogram() {
	kome::objects::Chromatogram* c = m_chromatogram.getOrgChromatogram();
	ChromatogramWrapper^ chrom = ClrObjectTool::createChromatogramWrapper( c );

	return chrom;
}

// set Q1 Mass
void ChromatogramWrapper::setQ1( double q1 ) {
	m_chromatogram.setQ1( q1 );
}

// get Q1 Mass
double ChromatogramWrapper::getQ1() {
	return m_chromatogram.getQ1();
}

// set Q3 Mass
void ChromatogramWrapper::setQ3( double q3 ) {
	m_chromatogram.setQ3( q3 );
}

// get Q3 Mass
double ChromatogramWrapper::getQ3() {
	return m_chromatogram.getQ3();
}

// get xy data from data manager
XYDataWrapper^ ChromatogramWrapper::getXYData() {
	kome::core::XYData* xyData = m_chromatogram.getXYData();
	if( xyData == NULL ) {
		return nullptr;
	}

	return gcnew XYDataWrapper( *xyData );
}

// delete xy data of data manager
void ChromatogramWrapper::deleteXYData() {
	m_chromatogram.deleteXYData();
}

// get data points
void ChromatogramWrapper::getXYData( XYDataWrapper^ xyData, bool op ) {
	m_chromatogram.getXYData( &xyData->getXYData(), op );
}

// search spectrum
SpectrumWrapper^ ChromatogramWrapper::searchSpectrum( double rt, SearchType search ) {
	// convert
	kome::objects::SearchType t = INVERT_FROM_ARRAY( search, kome::objects::SEARCH_NEAR, g_searchTypes );

	// spectrum
	kome::objects::Spectrum* spec = m_chromatogram.searchSpectrum( rt, t );
	if( spec == NULL ) {
		return nullptr;
	}

	// cast 
	ClrSpectrum* clrSpec = dynamic_cast< ClrSpectrum* >( spec );
	if( clrSpec != NULL ) {
		return clrSpec->getBaseSpectrum();
	}

	return gcnew SpectrumWrapper( *spec );
}

// get spectra
void ChromatogramWrapper::getSpectra(
		DataSetWrapper^ dataSet,
		double startRt,
		double endRt,
		SearchType startSearch,
		SearchType endSearch
) {
	// check parameter
	if( dataSet == nullptr ) {
		return;
	}

	// convert
	kome::objects::SearchType st = INVERT_FROM_ARRAY( startSearch, kome::objects::SEARCH_NEAR, g_searchTypes );
	kome::objects::SearchType et = INVERT_FROM_ARRAY( endSearch, kome::objects::SEARCH_NEAR, g_searchTypes );

	// spectra
	m_chromatogram.getSpectra( dataSet->getDataSet(), startRt, endRt, st, et );
}

// get mass
double ChromatogramWrapper::getMass( unsigned int index ) {
	return m_chromatogram.getMass( index );
}

// get MS stage
int ChromatogramWrapper::getMsStage( unsigned int index ) {
	return m_chromatogram.getMsStage( index );
}

// get precursor
double ChromatogramWrapper::getPrecursor( unsigned int index ) {
	return m_chromatogram.getPrecursor( index );
}

// get properties
void ChromatogramWrapper::getProperties( PropertiesWrapper^ properties ) {
	if( properties != nullptr ) {
		m_chromatogram.getProperties( properties->getProperties() );
	}
}

// check whether the specified key is common or not
bool ChromatogramWrapper::isCommonProperty( System::String^ key ) {
	return kome::objects::Chromatogram::isCommonProperty( ClrObjectTool::convertString( key ).c_str() );
}
