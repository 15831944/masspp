/**
 * @file PeakPositions.cpp
 * @brief Peak Positions class
 *
 * @author M.Izumi
 * @date 2013.12.09
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakPositions.h"
#include "IdentificationManager.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// profile map
std::map< long, int > PeakPositions::m_profileMap;

// constructor
PeakPositions::PeakPositions(){
	m_peakInfos.clear();
	m_propertys.clear();
	m_peakValueInfo.clear();
	m_matrixSampleId.clear();
}

// destructor
PeakPositions::~PeakPositions(){
	int tmp=0;
	for( std::map< std::pair< int, int >, PeakValueInfo >::iterator it = m_peakValueInfo.begin();
			it != m_peakValueInfo.end(); it++ ) {
		kome::objects::PeakElement* elm = (*it).second.elt;
		if( elm != NULL ) {
			delete elm;
			tmp++;
		}
	}
	tmp;
}

// add Peak Position
int PeakPositions::addPeakPosition( double rt, double mz, int charge ){	
	int idx = m_peakInfos.size();

	PeakInfo info;

	info.rt = rt;
	info.mz = mz;
	info.charge = charge;

	m_peakInfos.push_back( info );
		
	return idx;
}

// get peak positon m/z
double PeakPositions::getPeakPositionMz( int peakIndex ){
	if( peakIndex < 0 || peakIndex >= (int)m_peakInfos.size() ) {
		return -1.0;
	}

	PeakInfo info = m_peakInfos[ peakIndex ];
	return info.mz;
}

// get peak positon rt
double PeakPositions::getPeakPositionRt( int peakIndex ){
	if( peakIndex < 0 || peakIndex >= (int)m_peakInfos.size() ) {
		return -1.0;
	}

	PeakInfo info = m_peakInfos[ peakIndex ];
	return info.rt;
}

// get peak positon charge
int PeakPositions::getPeakPositonCharge( int peakIndex ){
	if( peakIndex < 0 || peakIndex >= (int)m_peakInfos.size() ) {
		return -1;
	}

	PeakInfo info = m_peakInfos[ peakIndex ];
	return info.charge;
}

// set peak value
void PeakPositions::setPeakValueInfo( 
	int peakIndex, 
	int sampleId,
	double peak, 
	kome::core::XYData& xyData,
	kome::objects::PeakElement* element,
	bool chromFlg,
	int profileId
){
	
	PeakValueInfo tmp;
	tmp.peakVal = peak;
	if( element == NULL ) {
		tmp.elt = NULL;
	}
	else {
		
		tmp.elt = new kome::objects::PeakElement();
		*( tmp.elt ) = *element;
	}
	
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	//int profileId = -1;
	if( profileId == -1 ){
		long ptsVersion = xyData.getVersion();
	
		if( m_profileMap.find( ptsVersion ) == m_profileMap.end() ) {
			profileId = identMgr.setProfile( xyData, chromFlg );
			m_profileMap[ ptsVersion ] = profileId;
		}
		else {
			profileId = m_profileMap[ ptsVersion ];
		}
	}
		
	tmp.profileId = profileId;

	std::pair< int , int > key;
	key.first = sampleId;
	key.second = peakIndex;
	
	// ÉÅÉÇÉäÉäÅ[ÉNÇ™ëŒçÙ @date 2014.04.18 <Mod> M.Izumi
	if( m_peakValueInfo.find( key ) == m_peakValueInfo.end() ){
		m_peakValueInfo[ key ] = tmp;
	}else{
		kome::objects::PeakElement* elt = m_peakValueInfo[ key ].elt;
		delete elt;
		m_peakValueInfo[ key ].elt = tmp.elt;
		m_peakValueInfo[ key ].peakVal = tmp.peakVal;
	}
	
	std::vector< int >::iterator id = std::find( m_matrixSampleId.begin(), m_matrixSampleId.end(), sampleId );
	if( id == m_matrixSampleId.end() ){
		m_matrixSampleId.push_back( sampleId );
	}	
}

// get peak value
double PeakPositions::getPeakValue( int peakIndex, int sampleId ){	
	std::pair< int, int > key;
	key.first = sampleId;
	key.second = peakIndex;

	std::map< std::pair< int, int >, PeakValueInfo >::iterator it = m_peakValueInfo.find( key );

	if( it != m_peakValueInfo.end() ){
		return m_peakValueInfo[ key ].peakVal;
	}
		
	return -1.0;
}

// get profile
int PeakPositions::getProfile( int peakIndex, int sampleId ){
	std::pair< int, int > key;
	key.first = sampleId;
	key.second = peakIndex;

	std::map< std::pair< int, int >, PeakValueInfo >::iterator it = m_peakValueInfo.find( key );

	if( it != m_peakValueInfo.end() ){
		return m_peakValueInfo[ key ].profileId;
	}

	return -1;
}

// get peak
kome::objects::PeakElement* PeakPositions::getPeak( int peakIndex, int sampleId ){
	std::pair< int, int > key;
	key.first = sampleId;
	key.second = peakIndex;

	std::map< std::pair< int, int >, PeakValueInfo >::iterator it = m_peakValueInfo.find( key );

	if( it != m_peakValueInfo.end() ){
		return m_peakValueInfo[ key ].elt;
	}

	return NULL;
}

// get number of peaks
int PeakPositions::getNumberOfPeaks(){
	return m_peakInfos.size();
}

// get number of matrix samples
int PeakPositions::getNumberOfMatrixSamples(){	
	return m_matrixSampleId.size();
}

// get matrix sample id
int PeakPositions::getMatrixSampleId( int sampleIndex ){
	
	if( sampleIndex < 0 || sampleIndex >= (int)m_matrixSampleId.size() ) {
		return -1;
	}
	return m_matrixSampleId[ sampleIndex ];
}

// set peak property
void PeakPositions::setPeakProperty( int peakIndex, const char* propName, const char* prop, int type ){
	bool bflg  = false;
	for( unsigned int i=0; i < m_propertys.size(); i++ ){
		
		if( m_propertys[i].peakIndex == peakIndex &&
			m_propertys[i].name.compare( propName ) == 0 && 
			m_propertys[i].value.compare( prop ) == 0 ){
		
			bflg = true;
		}
	}

	if( !bflg ){
		PropertyInfo info;
		info.peakIndex = peakIndex;
		info.name = propName;
		info.value = prop;
		info.type = type;

		m_propertys.push_back( info );
	}
}

// get number of properties
int PeakPositions::getNumberOfProperties(){
	return m_propertys.size();
}

// get property name
const char* PeakPositions::getPropertyName( int index ){
	
	if( index < 0 || index >= (int)m_propertys.size() ) {
		return "";
	}
		
	return m_propertys[index].name.c_str();
}

// get property value
const char* PeakPositions::getPropertyValue( int peakIndex, const char* propName ){
	
	for( unsigned int i=0; i < m_propertys.size(); i++ ){
		
		if( m_propertys[i].peakIndex == peakIndex && m_propertys[i].name.compare( propName ) == 0 ){
		
			return m_propertys[i].value.c_str();
		}
	}

	return "";
}

// get property index
int PeakPositions::getPropertyType( int index ){
	if( index < 0 || index >= (int)m_propertys.size() ) {
		return -1;
	}
		
	return m_propertys[index].type;
}
