/**
 * @file Peak2DElementWrapper.cpp
 * @brief implements of Peak2DElementWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peak2DElementWrapper.h"

#include "PeaksCluster2DWrapper.h"
#include "XYDataWrapper.h"
#include "ClrObjectTool.h"
#include "PeakElementWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peak2DElementWrapper::Peak2DElementWrapper( kome::objects::Peak2DElement& peak )
		: m_peak( peak ) {
}

// destructor
Peak2DElementWrapper::~Peak2DElementWrapper() {
}

// get peak element to be wrapped
kome::objects::Peak2DElement& Peak2DElementWrapper::getPeakElement() {
	return m_peak;
}

// set RT
void Peak2DElementWrapper::setRt( double rt, double startRt, double endRt ) {
	m_peak.setRt( rt, startRt, endRt );
}

// get RT
double Peak2DElementWrapper::getRt() {
	return m_peak.getRt();
}

// get start RT
double Peak2DElementWrapper::getStartRt() {
	return m_peak.getStartRt();
}

// get end RT
double Peak2DElementWrapper::getEndRt() {
	return m_peak.getEndRt();
}

// set m/z
void Peak2DElementWrapper::setMz( double mz, double startMz, double endMz ) {
	m_peak.setMz( mz, startMz, endMz );
}

// get m/z
double Peak2DElementWrapper::getMz() {
	return m_peak.getMz();
}

// get start m/z
double Peak2DElementWrapper::getStartMz() {
	return m_peak.getStartMz();
}

// get end m/z
double Peak2DElementWrapper::getEndMz() {
	return m_peak.getEndMz();
}

// set intensity
void Peak2DElementWrapper::setIntensity( double intensity ) {
	m_peak.setIntensity( intensity );
}

// get intensity
double Peak2DElementWrapper::getIntensity() {
	return m_peak.getIntensity();
}

// set cluster
void Peak2DElementWrapper::setCluster( PeaksCluster2DWrapper^ cluster ) {
	// cluster object
	kome::objects::PeaksCluster2D* c = NULL;
	if( cluster != nullptr ) {
		c = &cluster->getCluster();
	}

	// set
	m_peak.setCluster( c );
}

// set precursor
void Peak2DElementWrapper::setPrecursorMass( double precursor ) {
	m_peak.setPrecursorMass( precursor );
}

// get perecursor
double Peak2DElementWrapper::getPrecursorMass() {
	return m_peak.getPrecursorMass();
}

// set charge
void Peak2DElementWrapper::setCharge( int charge ) {
	m_peak.setCharge( charge );
}

// get charge
int Peak2DElementWrapper::getCharge() {
	return m_peak.getCharge();
}

// get cluster
PeaksCluster2DWrapper^ Peak2DElementWrapper::getCluster() {
	kome::objects::PeaksCluster2D* c = m_peak.getCluster();
	if( c == NULL ) {
		return nullptr;
	}
	return gcnew PeaksCluster2DWrapper( *c );
}

// set FWHM
void Peak2DElementWrapper::setFwhm( double fwhm ) {
	m_peak.setFwhm( fwhm );
}

// get FWHM
double Peak2DElementWrapper::getFwhm() {
	return m_peak.getFwhm();
}

// set area
void Peak2DElementWrapper::setArea( double area ) {
	m_peak.setArea( area );
}

// get area
double Peak2DElementWrapper::getArea() {
	return m_peak.getArea();
}

// clear data
void Peak2DElementWrapper::clearData() {
	m_peak.clearData();
}

// get data list size
unsigned int Peak2DElementWrapper::getDataListSize() {
	return m_peak.getDataListSize();
}

// add data
void Peak2DElementWrapper::addData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak ) {
	m_peak.addData( sampleId, profileId, peakVal, ( peak == nullptr ? NULL : &( peak->getPeakElement() ) ) );
}

// set data
void Peak2DElementWrapper::setData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak ) {
	m_peak.setData( sampleId, profileId, peakVal, ( peak == nullptr ? NULL : &( peak->getPeakElement() ) ) );
}

// get data peak
PeakElementWrapper^ Peak2DElementWrapper::getDataPeak( unsigned int idx ) {
	kome::objects::PeakElement* peak = m_peak.getDataPeak( idx );
	if( peak == NULL ) {
		return nullptr;
	}

	return gcnew PeakElementWrapper( *peak );
}

// get data profile ID
int Peak2DElementWrapper::getDataProfileId( unsigned int idx ) {
	return m_peak.getDataProfileId( idx );
}

// get data sample ID
int Peak2DElementWrapper::getDataSampleId( unsigned int idx ) {
	return m_peak.getDataSampleId( idx );
}

// get data peak value
double Peak2DElementWrapper::getDataPeakValue( unsigned int idx ) {
	return m_peak.getDataPeakValue( idx );
}

// clear parameters
void Peak2DElementWrapper::clearParameters() {
	m_peak.clearParameters();
}

// set parameter
void Peak2DElementWrapper::setParameter( System::String^ name, System::String^ value, int type ) {
	m_peak.setParameter(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str(),
		type
	);
}

// get number of parameters
unsigned int Peak2DElementWrapper::getNumberOfParameters() {
	return m_peak.getNumberOfParameters();
}

// get parameter name
System::String^ Peak2DElementWrapper::getParameterName( int idx ) {
	return ClrObjectTool::convertString( m_peak.getParameterName( idx ), NULL );
}

// get parameter value
System::String^ Peak2DElementWrapper::getParameterValue( System::String^ name ) {
	return ClrObjectTool::convertString(
		m_peak.getParameterValue(
			ClrObjectTool::convertString( name ).c_str()
		),
		NULL
	);
}

// get parameter value
System::String^ Peak2DElementWrapper::getParameterValue( int idx ) {
	return ClrObjectTool::convertString(
		m_peak.getParameterValue( idx ), NULL
	);
}

// get parameter type
int Peak2DElementWrapper::getParameterType( System::String^ name ) {
	return m_peak.getParameterType( ClrObjectTool::convertString( name ).c_str() );
}

// get parameter type
int Peak2DElementWrapper::getParameterType( int idx ) {
	return m_peak.getParameterType( idx );
}
