/**
 * @file Chromatogram.cpp
 * @brief implements of Chromatogram class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Chromatogram.h"

#include "PointsManager.h"
#include "PeaksManager.h"
#include "Spectrum.h"
#include "DataSet.h"
#include "DataGroupNode.h"
#include "XYDataOperation.h"
#include "Spectrum.h"
#include "DataSet.h"
#include "Sample.h"

#include <math.h>
#include <set>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
std::vector< std::string > Chromatogram::m_commonProps;


// constructor
Chromatogram::Chromatogram( Sample* sample ) {
	// initialize
	m_mz = -1.0;
	m_sample = sample;
	m_group = NULL;
	m_icon = "chromatogram";

	m_autoCreated = false;
	m_visible = true;

	m_orgChrom = NULL;
	m_op = false;

	m_q1 = -1.0;
	m_q3 = -1.0;
	
	// issue chrom id
	if( sample != NULL ){
		m_chromId = sample->issueChromId( sample );
	}
}

// destructor
Chromatogram::~Chromatogram() {
	// manager
	PointsManager& ptMgr = PointsManager::getInstance();
	PeaksManager& pkMgr = PeaksManager::getInstance();

	// close
	DataManager::closeChromatogram( this, true );

	// delete
	deleteXYData();
	pkMgr.deletePeaks( this );
	ptMgr.clearOperations( this );
}

// get sample
Sample* Chromatogram::getSample() {
	return m_sample;
}

// set name
void Chromatogram::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get name
const char* Chromatogram::getName() {
	return m_name.c_str();
}

// set m/z
void Chromatogram::setMz( const double mz ) {
	m_mz = mz;
}

// get m/z
double Chromatogram::getMz() {
	return m_mz;
}

// set group
void Chromatogram::setGroup( DataGroupNode* group ) {
	m_group = group;
	m_sample = ( group == NULL ) ? NULL : group->getSample();
}

// get group
DataGroupNode* Chromatogram::getGroup() {
	return m_group;
}

// set icon
void Chromatogram::setIcon( const char* icon ) {
	m_icon = NVL( icon, "" );
}

// get icon
const char* Chromatogram::getIcon() {
	return m_icon.c_str();
}

// set title
void Chromatogram::setTitle( const char* title ) {
	m_title = NVL( title, "" );
}

// get title
const char* Chromatogram::getTitle() {
	return m_title.c_str();
}

// get properties
kome::core::Properties& Chromatogram::getProperties() {
	return m_props;
}

// get user properties
kome::core::Properties& Chromatogram::getUserProperties() {
	return m_userProps;
}

// set auto created flag
void Chromatogram::setAutoCreated( const bool autoCreated ) {
	m_autoCreated = autoCreated;
}

// get auto created flag
bool Chromatogram::isAutoCreated() {
	return m_autoCreated;
}

// set visible flag
void Chromatogram::setVisible( const bool visible ) {
	m_visible = visible;
}

// get visible flag
bool Chromatogram::isVisible() {
	return m_visible;
}

// set original chromatogram
void Chromatogram::setOrgChromatogram( Chromatogram* chrom ) {
	m_orgChrom = chrom;
}

// get original chromatogram
Chromatogram* Chromatogram::getOrgChromatogram() {
	if( m_orgChrom == NULL ) {
		m_orgChrom = this;
	}

	return m_orgChrom;	
}

// set Q1 Mass
void Chromatogram::setQ1( const double q1 ) {
	m_q1 = q1;

	if( q1 < 0.0 ) {
		m_props.setValue( "Q1 Mass", "" );
	}
	else {
		m_props.setDoubleValue( "Q1 Mass", q1 );
	}
}

// get Q1 Mass
double Chromatogram::getQ1() {
	if( m_q1 < 0.0 ) {
		m_q1 = m_props.getDoubleValue( "Q1 Mass", -1.0 );
	}

	return m_q1;
}

// set Q3 Mass
void Chromatogram::setQ3( const double q3 ) {
	m_q3 = q3;

	if( q3 < 0.0 ) {
		m_props.setValue( "Q3 Mass", "" );
	}
	else {
		m_props.setDoubleValue( "Q3 Mass", q3 );
	}
}

// get Q3 Mass
double Chromatogram::getQ3() {
	if( m_q3 < 0.0 ) {
		m_q3 = m_props.getDoubleValue( "Q3 Mass", -1.0 );
	}

	return m_q3;
}

// get xy data from data manager
kome::core::XYData* Chromatogram::getXYData() {	
	PointsManager& ptMgr = PointsManager::getInstance();

	return ptMgr.getXYData( this );
}

// delete xy data of data manager
void Chromatogram::deleteXYData() {
	PointsManager& ptMgr = PointsManager::getInstance();

	ptMgr.deleteXYData( this );
}

// get data points
void Chromatogram::getXYData( kome::core::XYData* const xyData, const bool op ) {
	// check parameter
	if( xyData == NULL ) {
		return;
	}

	// get xy data
	kome::core::Timer tm;
	tm.start();

	onGetXYData( xyData );

	tm.stop();
	LOG_DEBUG(
		FMT(
			"Getting Chromatogram [%s] ... %.2f sec",
			m_title.c_str(),
			tm.getTotalTime()
		)
	);

	// operation
	if( op ) {
		// get operations
		PointsManager& ptMgr = PointsManager::getInstance();

		// update
		for( unsigned int i = 0; i < ptMgr.getNumberOfOperations( this ); i++ ) {
			XYDataOperation* operation = ptMgr.getOperation( this, i );
			operation->update( *xyData, *xyData, *this );
		}
	}
}

// search spectrum
Spectrum* Chromatogram::searchSpectrum( const double rt, const SearchType search ) {
	// spectra
	DataSet dataSet( m_group );
	getSpectra( dataSet, rt, rt, search, search );

	// spectrum
	if( dataSet.getNumberOfSpectra() == 0 ) {
		return NULL;
	}
	return dataSet.getSpectrum( 0 );
}

// get spectra
void Chromatogram::getSpectra(
		DataSet& dataSet,
		const double startRt,
		const double endRt,
		const SearchType startSearch,
		const SearchType endSearch
) {
	onGetSpectra( dataSet, startRt, endRt, startSearch, endSearch );
}

// get mass
double Chromatogram::getMass( const unsigned int index ) {
	return onGetMass( index );
}

// get MS stage
int Chromatogram::getMsStage( const unsigned int index ) {
	return onGetMsStage( index );
}

// get precursor
double Chromatogram::getPrecursor( const unsigned int index ) {
	return onGetPrecursor( index );
}

// get properties
void Chromatogram::getProperties( kome::core::Properties& properties ) {
	// get spectrum properties
	for( unsigned int i = 0; i < m_props.getNumberOfProperties(); i++ ) {
		properties.setValue( m_props.getKey( i ), m_props.getValue( i ) );
	}

	// get peak information
	PeaksManager& pkMgr = PeaksManager::getInstance();
	Peaks* peaks = pkMgr.getPeaks( this );
	if( peaks != NULL ) {
		int peakNum = peaks->getLength();
		properties.setIntValue( "Peaks Count", peakNum );
	}

	// get group properties
	DataGroupNode* group = m_group;

	while( group != NULL ) {
		for( unsigned int i = 0; i < group->getProperties().getNumberOfProperties(); i++ ) {
			properties.setValue(
				group->getProperties().getKey( i ),
				group->getProperties().getValue( i )
			);
		}
		group = group->getParentGroup();
	}
}

// get user properties
void Chromatogram::getUserProperties( kome::core::Properties& userProperties ) {
	// get spectrum user properties
	for( unsigned int i = 0; i < m_userProps.getNumberOfProperties(); i++ ) {
		userProperties.setValue( m_userProps.getKey( i ), m_userProps.getValue( i ) );
	}

	// get group user properties
	DataGroupNode* group = m_group;

	while( group != NULL ) {
		for( unsigned int i = 0; i < group->getUserProperties().getNumberOfProperties(); i++ ) {
			userProperties.setValue(
				group->getUserProperties().getKey( i ),
				group->getUserProperties().getValue( i )
			);
		}
		group = group->getParentGroup();
	}
}

// check the property key
bool Chromatogram::isCommonProperty( const char* key ) {
	// create array
	if( m_commonProps.size() == 0 ) {
		return false;
	}

	// check the key
	if( key == NULL ) {
		return false;
	}

	for( unsigned int i = 0; i < m_commonProps.size(); i++ ) {
		if( m_commonProps[ i ].compare( key ) == 0 ) {
			return true;
		}
	}

	return false;
}

// set chromatogram id
void Chromatogram::setId( int id ){
	m_chromId = id;
}

// get chromatogram id
int Chromatogram::getId(){
	return m_chromId;
}
