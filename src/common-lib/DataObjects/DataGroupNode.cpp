/**
 * @file DataGroupNode.cpp
 * @brief implements of DataGroupNode class
 *
 * @author S.Tanaka
 * @date 2006.08.31
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataGroupNode.h"

#include "Spectrum.h"
#include "Sample.h"
#include "Chromatogram.h"

#include "SpectraChromatogram.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_SPEC_X_TITLE	 "m/z"
#define DEFAULT_SPEC_Y_TITLE	 "intensity"
#define DEFAULT_CHROM_X_TITLE	 "RT"
#define DEFAULT_CHROM_Y_TITLE	 "intensity"


// cosntructor
DataGroupNode::DataGroupNode( Sample* sample, const char* name ) : DataSet( NULL ) {
	// log
	LOG_TRACE( FMT( "Creating spectrum group... (%s)", NVL( name, "" ) ) );

	// initialize
	m_group = this;
	m_sample = sample;
	setName( name );
	m_index = -1;
	m_parent = NULL;
	m_autoScanNumber = true;

	// axis title
	m_specXTitle  = DEFAULT_SPEC_X_TITLE;
	m_specYTitle  = DEFAULT_SPEC_Y_TITLE;
	m_chromXTitle = DEFAULT_CHROM_X_TITLE;
	m_chromYTitle = DEFAULT_CHROM_Y_TITLE;

	// level
	m_level = 0;
	
	// issue group id
	if( sample != NULL ){
		m_groupId = sample->issueGroupId( sample );
	}
}

// destructor
DataGroupNode::~DataGroupNode() {
	// delete childrent
	for( unsigned int i = 0; i < m_children.size(); i++ ) {
		delete m_children[ i ];
	}
	m_children.clear();

	// delete spectra
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		delete m_spectra[ i ];
	}
	m_spectra.clear();

	// delete chromatograms
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		delete m_chroms[ i ];
	}
	m_chroms.clear();
}

// set name
void DataGroupNode::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get name
const char* DataGroupNode::getName() {
	return m_name.c_str();
}

// set group index
void DataGroupNode::setGroupIndex( const int index ) {
	m_index = index;
}

// get group index
int DataGroupNode::getGroupIndex() {
	return m_index;
}

// get parent spectrum
DataGroupNode* DataGroupNode::getParentGroup() {
	return m_parent;
}

// create new child group
DataGroupNode* DataGroupNode::createChildGroup( const char* name ){
	// create child group object
	DataGroupNode* child = new DataGroupNode( m_sample, name );
	child->m_level = m_level + 1;
	child->setParentDataGroupNode( this );

	// add object to the array
	m_children.push_back( child );

	return child;
}

// remove child group
void DataGroupNode::removeChildGroup( DataGroupNode* child ) {
	// index
	int idx = -1;
	for( int i = 0; i < (int)m_children.size() && idx < 0; i++ ) {
		DataGroupNode* tmp = m_children[ i ];
		if( tmp == child ) {
			idx = i;
		}
	}

	// removee
	if( idx >= 0 ) {
		m_children.erase( m_children.begin() + idx );
	}
}

// get the number of child groups
unsigned int DataGroupNode::getNumberOfChildren() {
	return m_children.size();
}

// get child group object
DataGroupNode* DataGroupNode::getChild( const unsigned int index ) {
	// check the index
	if( index >= m_children.size() ) {
		return NULL;
	}
	return m_children[ index ];
}

// get spectra
void DataGroupNode::getDataSet( DataSet* dataSet ) {
	// check parameter
	if( dataSet == NULL ) {
		return;
	}

	// get chromatograms
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		dataSet->addChromatogram( m_chroms[ i ] );
	}

	// get spectra
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		dataSet->addSpectrum( m_spectra[ i ] );
	}

	// gets spectra that belong to child group
	for( unsigned int i = 0; i < m_children.size(); i++ ) {
		m_children[ i ]->getDataSet( dataSet );
	}
}

// set auto scan number flag
void DataGroupNode::setAutoScanNumber( const bool autoScanNumber ) {
	m_autoScanNumber = autoScanNumber;
}

// get auto scan number flag
bool DataGroupNode::isAutoScanNumber() {
	return m_autoScanNumber;
}

// set spectrum x axis title
void DataGroupNode::setSpecXTitle( const char* title ) {
	m_specXTitle = std::string( NVL( title, "" ) );
}

// get spectrum x axis title
const char* DataGroupNode::getSpecXTitle() {
	return m_specXTitle.c_str();
}

// set spectrum y axis title
void DataGroupNode::setSpecYTitle( const char* title ) {
	m_specYTitle = std::string( NVL( title, "" ) );
}

// get spectrum y axis title
const char* DataGroupNode::getSpecYTitle() {
	return m_specYTitle.c_str();
}

// set chromatogram x axis title
void DataGroupNode::setChromXTitle( const char* title ) {
	m_chromXTitle = std::string( NVL( title, "" ) );
}

// get chromatogram x axis title
const char* DataGroupNode::getChromXTitle() {
	return m_chromXTitle.c_str();
}

// set chromatogram y axis title
void DataGroupNode::setChromYTitle( const char* title ) {
	m_chromYTitle = std::string( NVL( title, "" ) );
}

// get chromatogram y axis title
const char* DataGroupNode::getChromYTitle() {
	return m_chromYTitle.c_str();
}

// get group level
unsigned int DataGroupNode::getLevel() {
	return m_level;
}

// get properties
kome::core::Properties& DataGroupNode::getProperties() {
	return m_properties;
}

// get user properties
kome::core::Properties& DataGroupNode::getUserProperties() {
	return m_userProperties;
}

// set parent spectrum group
void DataGroupNode::setParentDataGroupNode( DataGroupNode* parent ) {
	m_parent = parent;
}

// create default chromatogram
void DataGroupNode::createDefaultChromatogram() {
	// check the member
	if( m_spectra.size() == 0 || m_chroms.size() > 0 ) {
		return;
	}

	// check spectra
	int cnt = 0;
	for( unsigned int i = 0; i < m_spectra.size() && cnt < 2; i++ ) {
		kome::objects::Spectrum* spec = m_spectra[ i ];
		if( spec->hasChromatogram() ) {
			cnt++;
		}
	}
	if( cnt < 2 ) {
		return;
	}

	// TIC
	kome::objects::SpectraChromatogram* tic = new kome::objects::SpectraChromatogram( *this );
	tic->setName( "TIC" );
	tic->setIcon( "TIC" );
	tic->setAutoCreated( true );
	m_chroms.push_back( tic );
}

// sort spectra
void DataGroupNode::sortSpectra() {
	// sort
	DataSet::sortSpectra();
}

// add spectrum
void DataGroupNode::insertSpectrum( Spectrum* spectrum, const int idx ) {
	// check the parameter
	if( spectrum == NULL ) {
		return;
	}

	// set spectrum properties
	int scanNum = (int)m_spectra.size();

	if( spectrum->getScanNumber() < 0 && m_autoScanNumber ) {
		spectrum->setScanNumber( scanNum );
	}
	spectrum->setGroup( this );

	// insert spectrum
	m_spectra.insert( m_spectra.begin() + idx, spectrum );

	// default chromatogram
	createDefaultChromatogram();
}

// add chromatogram
void DataGroupNode::insertChromatogram( kome::objects::Chromatogram* chrom, const int idx ) {
	// check the object
	if( chrom == NULL ) {
		return;
	}

	// insert
	m_chroms.insert( m_chroms.begin() + idx, chrom );
	chrom->setGroup( this );

	// delete
	for( int i = (int)m_chroms.size() - 1; i >= 0; i-- ) {
		Chromatogram* tmp = m_chroms[ i ];
		if( tmp->isAutoCreated() ) {
			m_chroms.erase( m_chroms.begin() + i );
			delete tmp;
		}
	}
}

// set group id
void DataGroupNode::setId( int id ){
	m_groupId = id;
}

// get group id
int DataGroupNode::getId(){
	return m_groupId;
}
