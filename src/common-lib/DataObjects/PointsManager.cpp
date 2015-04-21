/**
 * @file PointsManager.cpp
 * @brief implement of PointsManager class
 *
 * @author S.Tanaka
 * @date 2007.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PointsManager.h"

#include "Spectrum.h"
#include "Chromatogram.h"
#include "Sample.h"
#include "XYDataOperation.h"
#include "DataMapInfo.h"

#include <set>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// construcotr
PointsManager::PointsManager() {
}

// destructor
PointsManager::~PointsManager() {
	// delete operation
	for( std::map< XYDataOperation*, int >::iterator it = m_countMap.begin();
			it != m_countMap.end(); it++ ) {
		delete (*it).first;
	}
	m_countMap.clear();
}

// get xy data of specified spectrum
kome::core::XYData* PointsManager::getXYData( Spectrum* spec ) {
	// points
	kome::core::DataPoints& dps = m_specPointsMap[ spec ];

	// get points
	if( dps.getLength() == 0 ) {
		spec->getXYData( &dps, true );
	}

	return &dps;
}

// get xy data of specified chromatogram
kome::core::XYData* PointsManager::getXYData( Chromatogram* chrom ) {
	// points
	kome::core::DataPoints& dps = m_chromPointsMap[ chrom ];

	// get points
	if( dps.getLength() == 0 ) {
		chrom->getXYData( &dps, true );
	}

	return &dps;
}


// delete xy data of specified spectrum
void PointsManager::deleteXYData( Spectrum* spec ) {
	// check the map
	if( m_specPointsMap.find( spec ) == m_specPointsMap.end() ) {
		return;
	}

	// clear
	kome::core::DataPoints& dps = m_specPointsMap[ spec ];
	dps.clearPoints();

	// erase
	m_specPointsMap.erase( spec );
}

// delete xy data of specified chromatogram
void PointsManager::deleteXYData( Chromatogram* chrom ) {
	// check the map
	if( m_chromPointsMap.find( chrom ) == m_chromPointsMap.end() ) {
		return;
	}

	// clear
	kome::core::DataPoints& dps = m_chromPointsMap[ chrom ];
	dps.clearPoints();

	// erase
	m_chromPointsMap.erase( chrom );
}


// adds operation
void PointsManager::addOperation( Spectrum* spec, XYDataOperation* operation ) {
	// check parameters
	if( spec == NULL || operation == NULL ) {
		return;
	}

	// add count
	if( m_countMap.find( operation ) == m_countMap.end() ) {
		m_countMap[ operation ] = 1;
	}
	else {
		int count = m_countMap[ operation ];
		count++;
		m_countMap[ operation ] = count;
	}
		
	// add operation
	m_specOperationMap[ spec ].push_back( operation );
	
	// sort
	if( operation->getIndex() != -1 ){
		std::sort( m_specOperationMap[spec].begin(), m_specOperationMap[spec].end(), lessOperation );
	}

	// clear points
	deleteXYData( spec );
}

// add operation
void PointsManager::addOperation( Chromatogram* chrom, XYDataOperation* operation ) {
	// check parameters
	if( chrom == NULL || operation == NULL ) {
		return;
	}

	// add count
	if( m_countMap.find( operation ) == m_countMap.end() ) {
		m_countMap[ operation ] = 1;
	}
	else {
		int count = m_countMap[ operation ];
		count++;
		m_countMap[ operation ] = count;
	}

	// add operation
	m_chromOperationMap[ chrom ].push_back( operation );

	// sort
	if( operation->getIndex() != -1 ){
		std::sort( m_chromOperationMap[ chrom ].begin(), m_chromOperationMap[ chrom ].end(), lessOperation );
	}
	// clear points
	deleteXYData( chrom );
}

// add operation
void PointsManager::addOperation( Sample* sample, XYDataOperation* operation ){
	// check parameters
	if( sample == NULL || operation == NULL ){
		return;
	}

	// add count
	if( m_countMap.find( operation ) == m_countMap.end() ){
		m_countMap[ operation] = 1;
	}else{
		int count = m_countMap[ operation ];
		count++;
		m_countMap[ operation ] = count;
	}
	
	// add operation
	m_sampleOperationMap[ sample ].push_back( operation );
	
	if( operation->getIndex() != -1 ){
		std::sort( m_sampleOperationMap[ sample ].begin(), m_sampleOperationMap[ sample ].end(), lessOperation );
	}
}

// remove operation of specified spectrum
void PointsManager::removeOperation( Spectrum* spec, XYDataOperation* operation ) {
	// check parameters
	if( spec == NULL || operation == NULL ) {
		return;
	}

	// search
	if( m_specOperationMap.find( spec ) == m_specOperationMap.end() ) {
		return;
	}
	std::vector< XYDataOperation* >& operations = m_specOperationMap[ spec ];
	int idx = -1;

	for( int i = 0; i < (int)operations.size() && idx < 0; i++ ) {
		if( operations[ i ] == operation ) {
			idx = i;
		}
	}
	if( idx < 0 ) {
		return;
	}

	// remove operation
	operations.erase( operations.begin() + idx );

	// sort
	if( operations.size() == 0 ) {
		m_specOperationMap.erase( spec );
	}

	deleteOperation( operation );
}

// remove operation of specified chromatogram
void PointsManager::removeOperation( Chromatogram* chrom, XYDataOperation* operation ) {
	// check parameters
	if( chrom == NULL || operation == NULL ) {
		return;
	}

	// search
	if( m_chromOperationMap.find( chrom ) == m_chromOperationMap.end() ) {
		return;
	}
	std::vector< XYDataOperation* >& operations = m_chromOperationMap[ chrom ];
	int idx = -1;

	for( int i = 0; i < (int)operations.size() && idx < 0; i++ ) {
		if( operations[ i ] == operation ) {
			idx = i;
		}
	}
	if( idx < 0 ) {
		return;
	}

	// remove operation
	operations.erase( operations.begin() + idx );
	if( operations.size() == 0 ) {
		m_chromOperationMap.erase( chrom );
	}

	deleteOperation( operation );
}

// remove operation of specified sample
void PointsManager::removeOperation( Sample* sample, XYDataOperation* operation ){
	// check parameters
	if( sample == NULL || operation == NULL ){
		return;
	}

	// search
	if( m_sampleOperationMap.find( sample) == m_sampleOperationMap.end() ){
		return;
	}
	std::vector< XYDataOperation* >& operations = m_sampleOperationMap[ sample ];
	int idx = -1;

	for( int i=0; i < (int)operations.size() && idx < 0; i++ ){
		if( operations[ i ] == operation ){
			idx = i;
		}
	}
	if( idx < 0 ){
		return;
	}

	// remove operation
	operations.erase( operations.begin() + idx );
	if( operations.size() == 0 ){
		m_sampleOperationMap.erase( sample );
	}

	deleteOperation( operation );
}


// clear operations of specified spectrum
void PointsManager::clearOperations( Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// operations
	if( m_specOperationMap.find( spec ) == m_specOperationMap.end() ) {
		return;
	}
	std::vector< XYDataOperation* >& operations = m_specOperationMap[ spec ];

	// delete operations
	for( unsigned int i = 0; i < operations.size(); i++ ) {
		deleteOperation( operations[ i ] );
	}
	
	// erase
	operations.clear();
	m_specOperationMap.erase( spec );
}

// clear operations of specified chromatogram
void PointsManager::clearOperations( Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// operations
	if( m_chromOperationMap.find( chrom ) == m_chromOperationMap.end() ) {
		return;
	}
	std::vector< XYDataOperation* >& operations = m_chromOperationMap[ chrom ];

	// delete oprations
	for( unsigned int i = 0; i < operations.size(); i++ ) {
		deleteOperation( operations[ i ] );
	}

	// erase
	operations.clear();
	m_chromOperationMap.erase( chrom );
}

// clear operations of specified sample
void PointsManager::clearOperations( Sample* sample ){
	// check the parameter
	if( sample == NULL ){
		return;
	}

	//operations
	if( m_sampleOperationMap.find( sample) == m_sampleOperationMap.end() ){
		return;
	}
	std::vector< XYDataOperation* >& operations = m_sampleOperationMap[ sample ];

	// delete operations
	for( unsigned int i=0; i < operations.size(); i++ ){
		deleteOperation( operations[ i ] );
	}

	// erase
	operations.clear();
	m_sampleOperationMap.erase( sample );
}

// get the number of operations of specified spectrum
unsigned int PointsManager::getNumberOfOperations( Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return 0;
	}

	// get size
	if( m_specOperationMap.find( spec ) == m_specOperationMap.end() ) {
		return 0;
	}
	return m_specOperationMap[ spec ].size();
}

// get the number of operations of specified chromatogram
unsigned int PointsManager::getNumberOfOperations( Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return 0;
	}

	// get size
	if( m_chromOperationMap.find( chrom ) == m_chromOperationMap.end() ) {
		return 0;
	}
	return m_chromOperationMap[ chrom ].size();
}

// get the number of operations of specified chromatogram
unsigned int PointsManager::getNumberOfOperations( Sample* sample ){
	// check the parameter
	if( sample == NULL ){
		return 0;
	}

	// get size
	if( m_sampleOperationMap.find( sample) == m_sampleOperationMap.end() ){
		return 0;
	}
	return m_sampleOperationMap[ sample ].size();
}

// get operation of specified spectrum
XYDataOperation* PointsManager::getOperation( Spectrum* spec, const unsigned int index ) {
	// check the parameter
	if( spec == NULL ) {
		return NULL;
	}

	// operations
	if( m_specOperationMap.find( spec ) == m_specOperationMap.end() ) {
		return NULL;
	}
	std::vector< XYDataOperation* >& operations = m_specOperationMap[ spec ];

	// check the index
	if( index >= operations.size() ) {
		return NULL;
	}
	return operations[ index ];
}

// get operation of specified chromatogram
XYDataOperation* PointsManager::getOperation( Chromatogram* chrom, const unsigned int index ) {
	// check the parameter
	if( chrom == NULL ) {
		return NULL;
	}

	// operations
	if( m_chromOperationMap.find( chrom ) == m_chromOperationMap.end() ) {
		return NULL;
	}
	std::vector< XYDataOperation* >& operations = m_chromOperationMap[ chrom ];

	// check the index
	if( index >= operations.size() ) {
		return NULL;
	}
	return operations[ index ];
}

// get operation of specified sample
XYDataOperation* PointsManager::getOperation( Sample* sample, const unsigned int index ){
	// check the parameter
	if( sample == NULL ){
		return NULL;
	}

	// operations
	if( m_sampleOperationMap.find( sample ) == m_sampleOperationMap.end() ){
		return NULL;
	}
	std::vector< XYDataOperation* >& operations = m_sampleOperationMap[ sample ];

	// check the index
	if( index >= operations.size() ){
		return NULL;
	}
	return operations[ index ];
}

// delete operation
void PointsManager::deleteOperation( XYDataOperation* operation ) {
	// check parameter
	if( operation == NULL || m_countMap.find( operation ) == m_countMap.end() ) {
		return;
	}

	// get count
	int count = m_countMap[ operation ];
	count--;
	if( count > 0 ) {
		m_countMap[ operation ] = count;
	}
	else {
		m_countMap.erase( operation );
		delete operation;
	}
}

// This method is called when a sample is closed
void PointsManager::onCloseSample( Sample* sample, const bool deleting ) {
	// get spectra
	std::set< Spectrum* > specSet;
	for( std::map< Spectrum*, kome::core::DataPoints >::iterator it = m_specPointsMap.begin();
			it != m_specPointsMap.end(); it++ ) {
		Spectrum* spec = (*it).first;
		if( spec->getSample() == sample ) {
			specSet.insert( spec );
		}
	}
	for( std::map< Spectrum*, std::vector< XYDataOperation* > >::iterator it = m_specOperationMap.begin();
			it != m_specOperationMap.end(); it++ ) {
		Spectrum* spec = (*it).first;
		if( spec->getSample() == sample ) {
			specSet.insert( spec );
		}
	}

	// delete spectra
	for( std::set< Spectrum* >::iterator it = specSet.begin(); it != specSet.end(); it++ ) {
		// spectrum		
		Spectrum* spec = *it;

		// delete operation
		if( m_specOperationMap.find( spec ) != m_specOperationMap.end() ) {
			std::vector< XYDataOperation* >& operations = m_specOperationMap[ spec ];
			for( unsigned int i = 0; i < operations.size(); i++ ) {
				deleteOperation( operations[ i ] );
			}

			m_specOperationMap.erase( spec );
		}

		// delete points
		if( m_specPointsMap.find( spec ) != m_specPointsMap.end() ) {
			m_specPointsMap.erase( spec );
		}
	}

	// get chromatograms
	std::set< Chromatogram* > chromSet;
	for( std::map< Chromatogram*, kome::core::DataPoints >::iterator it = m_chromPointsMap.begin();
			it != m_chromPointsMap.end(); it++ ) {
		Chromatogram* chrom = (*it).first;
		if( chrom->getSample() == sample ) {
			chromSet.insert( chrom );
		}
	}
	for( std::map< Chromatogram*, std::vector< XYDataOperation* > >::iterator it = m_chromOperationMap.begin();
			it != m_chromOperationMap.end(); it++ ) {
		Chromatogram* chrom = (*it).first;
		if( chrom->getSample() == sample ) {
			chromSet.insert( chrom );
		}
	}

	// delete chromatograms
	for( std::set< Chromatogram* >::iterator it = chromSet.begin(); it != chromSet.end(); it++ ) {
		// chromatogram
		Chromatogram* chrom = *it;

		// delete operation
		if( m_chromOperationMap.find( chrom ) != m_chromOperationMap.end() ) {
			std::vector< XYDataOperation* >& operations = m_chromOperationMap[ chrom ];
			for( unsigned int i = 0; i < operations.size(); i++ ) {
				deleteOperation( operations[ i ] );
			}

			m_chromOperationMap.erase( chrom );
		}

		// delete points
		if( m_chromPointsMap.find( chrom ) != m_chromPointsMap.end() ) {
			m_chromPointsMap.erase( chrom );
		}
	}
}

// on close spectrum
void PointsManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	deleteXYData( spec );
}

// on close Chromatogram
void PointsManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	deleteXYData( chrom );
}

// less operation
bool PointsManager::lessOperation( XYDataOperation* opt0, XYDataOperation* opt1 ){
	return ( opt0->getIndex() < opt1->getIndex() );
}


// get points manager object
PointsManager& PointsManager::getInstance() {
	// create object (This is the only object.)
	static PointsManager mgr;

	return mgr;
}
