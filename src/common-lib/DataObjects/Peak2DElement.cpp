/**
 * @file Peak2DElement.cpp
 * @brief implements of Peak2DElement class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peak2DElement.h"

#include "Peaks2D.h"
#include "PeaksCluster2D.h"
#include "PeakElement.h"

#include <boost/bind.hpp>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peak2DElement::Peak2DElement( Peaks2D* peaks ) {
	// initialize
	m_peaks = peaks;

	m_rt = double();
	m_startRt = double();
	m_endRt = double();

	m_mz = double();
	m_startMz = double();
	m_endMz = double();

	m_intensity = -1.0;

	m_cluster = NULL;
	m_precursor = -1.0;
	m_charge = -1;
	m_fwhm = -1.0;
	m_area = -1.0;

	m_elmId = -1;
	
	// issue peak2d id
	if( m_peaks != NULL ){
		m_peak2dId = m_peaks->issueId( this );
	}
}

// destructor
Peak2DElement::~Peak2DElement() {
}

// set RT
void Peak2DElement::setRt( const double rt, const double startRt, const double endRt ) {
	m_rt = rt;
	m_startRt = startRt;
	m_endRt = endRt;
}

// get RT
double Peak2DElement::getRt() {
	return m_rt;
}

// get start RT
double Peak2DElement::getStartRt() {
	return m_startRt;
}

// get end RT
double Peak2DElement::getEndRt() {
	return m_endRt;
}

// set m/z
void Peak2DElement::setMz( const double mz, const double startMz, const double endMz ) {
	m_mz = mz;
	m_startMz = startMz;
	m_endMz = endMz;
}

// get m/z
double Peak2DElement::getMz() {
	return m_mz;
}

// get start m/z
double Peak2DElement::getStartMz() {
	return m_startMz;
}

// get end m/z
double Peak2DElement::getEndMz() {
	return m_endMz;
}

// set intensity
void Peak2DElement::setIntensity( const double intensity ) {
	m_intensity = intensity;
}

// get intensity
double Peak2DElement::getIntensity() {
	return m_intensity;
}

// set cluster
void Peak2DElement::setCluster( PeaksCluster2D* cluster ) {
	// check
	if( m_cluster == cluster ) {
		return;
	}

	// remove
	if( m_cluster != NULL ) {
		m_cluster->removePeak( this );
	}

	// set
	m_cluster = cluster;

	// add
	if( m_cluster != NULL ) {
		m_cluster->addPeak( this );
	}
}

// get cluster
PeaksCluster2D* Peak2DElement::getCluster() {
	return m_cluster;
}

// set precursor mass
void Peak2DElement::setPrecursorMass( const double precursor ) {
	m_precursor = precursor;
}

// get precursor mass
double Peak2DElement::getPrecursorMass() {
	return m_precursor;
}

// set charge
void Peak2DElement::setCharge( const int charge ) {
	m_charge = charge;
}

// get charge
int Peak2DElement::getCharge() {
	return m_charge;
}

// set FWHM
void Peak2DElement::setFwhm( const double fwhm ) {
	m_fwhm = fwhm;
}

// get FWHM
double Peak2DElement::getFwhm() {
	return m_fwhm;
}

// set area
void Peak2DElement::setArea( const double area ) {
	m_area = area;
}

// get area
double Peak2DElement::getArea() {
	return m_area;
}

// get peak2d id
int Peak2DElement::getId(){
	return m_peak2dId;
}

// set peak2d id
void Peak2DElement::setId( int id ){
	m_peak2dId = id;
}

// clear data
void Peak2DElement::clearData() {
	m_dataList.clear();
}

// get the data list size
unsigned int Peak2DElement::getDataListSize() {
	return m_dataList.size();
}

// add data
void Peak2DElement::addData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak ) {
	m_dataList.resize( m_dataList.size() + 1 );
	m_dataList.back().peakVal = peakVal;
	if( peak != NULL ) {
		m_dataList.back().peak = *peak;
	}
	m_dataList.back().sampleId = sampleId;
	m_dataList.back().profile = profileId;
}

// set data
void Peak2DElement::setData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak ) {
	clearData();
	addData( sampleId, profileId, peakVal, peak );

	// set values
	setIntensity( peakVal );
	setArea( peak == NULL ? -1.0 : peak->getArea() );
	setFwhm( peak == NULL ? -1.0 : peak->getFwhm() );
}

// get data peak
PeakElement* Peak2DElement::getDataPeak( const unsigned int idx ) {
	if( idx >= m_dataList.size() ) {
		return NULL;
	}

	return &( m_dataList[ idx ].peak );
}

// get profile ID
int Peak2DElement::getDataProfileId( const unsigned int idx ) {
	if( idx >= m_dataList.size() ) {
		return -1;
	}

	return m_dataList[ idx ].profile;
}

// get sample ID
int Peak2DElement::getDataSampleId( const unsigned int idx ) {
	if( idx >= m_dataList.size() ) {
		return -1;
	}

	return m_dataList[ idx ].sampleId;
}

// get peak value
double Peak2DElement::getDataPeakValue( const unsigned int idx ) {
	if( idx >= m_dataList.size() ) {
		return -1.0;
	}

	return m_dataList[ idx ].peakVal;
}

// clear parameters
void Peak2DElement::clearParameters() {
	m_params.clear();
}

// set parameter
void Peak2DElement::setParameter( const char* name, const char* value, const int type ) {
	// search index
	int idx = searchParameterIndex( name );

	// set parametr
	if( idx < 0 ) {    // add
		m_params.resize( m_params.size() + 1 );
		m_params.back().name = NVL( name, "" );
		m_params.back().value = NVL( value, "" );
		m_params.back().type = type;
	}
	else {    // update
		m_params[ idx ].value = NVL( value, "" );
		if( type > 0 ) {
			m_params[ idx ].type = type;
		}
	}
}

// get the number of parameters
unsigned int Peak2DElement::getNumberOfParameters() {
	return m_params.size();
}

// get parameter name
const char* Peak2DElement::getParameterName( const int idx ) {
	if( idx < 0 || idx >= (int)m_params.size() ) {
		return NULL;
	}

	return m_params[ idx ].name.c_str();
}

// get parameter value
const char* Peak2DElement::getParameterValue( const char* name ) {
	// search index
	int idx = searchParameterIndex( name );

	// value
	return getParameterValue( idx );
}

// get parameter value
const char* Peak2DElement::getParameterValue( const int idx ) {
	if( idx < 0 || idx >= (int)m_params.size() ) {
		return NULL;
	}

	return m_params[ idx ].value.c_str();
}

// get parameter type
int Peak2DElement::getParameterType( const char* name ) {
	// search index
	int idx = searchParameterIndex( name );

	return getParameterType( idx );
}

// get parameter type
int Peak2DElement::getParameterType( const int idx ) {
	if( idx < 0 || idx >= (int)m_params.size() ) {
		return NULL;
	}

	return m_params[ idx ].type;
}

// search parameter index
int Peak2DElement::searchParameterIndex( const char* name ) {
	std::string n = NVL( name, "" );

	int idx = -1;
	for( int i = 0; i < (int)m_params.size() && idx < 0; i++ ) {
		if( n.compare( m_params[ i ].name ) == 0 ) {
			idx = i;
		}
	}

	return idx;
}
