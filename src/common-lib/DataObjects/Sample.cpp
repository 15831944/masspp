/**
 * @file Sample.cpp
 * @brief implements of Sample class
 *
 * @author S.Tanaka
 * @date 2007.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Sample.h"

#include "SampleSet.h"		   
#include "DataGroupNode.h"
#include "DataManager.h"
#include "ActiveObjectsManager.h"
#include "Chromatogram.h"
#include "Spectrum.h"
#include "Peaks.h"

using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// static member
std::vector< std::string > Sample::m_commonProps;
int Sample::m_optSampleId = 0;

// constructor
Sample::Sample( SampleSet* sampleSet ) {
	// initialize
	m_type = "MS";
	m_sampleSet = sampleSet;
	m_sampleIdx = -1;
	m_specId = 0;
	m_opened = false;
	m_edited = false;
	m_root = NULL;
	m_parallelReadable = false;
	m_supportedApiPeaks = false;

	// issue sample id
	m_optSampleId = issueSampleId();
}

// destructor
Sample::~Sample() {
	// close
	DataManager::closeSample( this, true );

	// delete spectra
	if( m_root != NULL ) {
		delete m_root;
		m_root = NULL;
		m_opened = false;
		m_edited = false;	
	}

	// delete hidden objects
	for( unsigned int i = 0; i < m_hiddenSet.getNumberOfSpectra(); i++ ) {
		delete m_hiddenSet.getSpectrum( i );
	}
	for( unsigned int i = 0; i < m_hiddenSet.getNumberOfChromatograms(); i++ ) {
		delete m_hiddenSet.getChromatogram( i );
	}
}

// get sample set
SampleSet* Sample::getSampleSet() {
	return m_sampleSet;
}

// set sample index
void Sample::setSampleIndex( const int index ) {
	m_sampleIdx = index;
}

// get sample index
int Sample::getSampleIndex() {
	return m_sampleIdx;
}

// set sample name
void Sample::setName( const char* name ) {
	m_name = NVL( name, "" );

	// property
	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Sample Name", m_name.c_str() );
	}
}

// get sample name
const char* Sample::getName() {
	return m_name.c_str();
}

// set sample type
void Sample::setType( const char* type ) {
	m_type = NVL( type, "" );

	// property
	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Sample Type", m_type.c_str() );
	}
}

// get sample type
const char* Sample::getType() {
	return m_type.c_str();
}

// set instrument name
void Sample::setInstrument( const char* instrument ) {
	// clear
	m_instrument.clear();
	if( instrument == NULL ) {
		return;
	}

	// instrument name
	std::string instStr = trimstring( instrument );
	bool prevSpace = false;
	for( unsigned int i = 0; i < instStr.size(); i++ ) {
		char c = instStr[ i ];
		if( isalpha( c ) || isdigit( c ) ) {
			prevSpace = false;
			c = (char)toupper( c );
			m_instrument.push_back( c );
		}
		else if( !prevSpace ) {
			prevSpace = true;
			m_instrument.push_back( '-' );
		}
	}

	// set property
	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Instrument", m_instrument.c_str() );
	}
}

// get instrument name
const char* Sample::getInstrument() {
	return m_instrument.c_str();
}

// set MS company
void Sample::setMsCompany( const char* company ) {
	m_company = NVL( company, "" );

	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Manufacturer", m_company.c_str() );
	}
}

// get MS company
const char* Sample::getMsCompany() {
	return m_company.c_str();
}

// set software name
void Sample::setSoftwareName( const char* name ) {
	m_softwareName = NVL( name, "" );

	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Software Name", m_softwareName.c_str() );
	}
}

// get software namee
const char* Sample::getSoftwareName() {
	return m_softwareName.c_str();
}

// set software version
void Sample::setSoftwareVersion( const char* version ) {
	m_softwareVersion = NVL( version, "" );

	if( m_root != NULL ) {
		m_root->getProperties().setValue( "Software Version", m_softwareVersion.c_str() );
	}
}

// get software version
const char* Sample::getSoftwareVersion() {
	return m_softwareVersion.c_str();
}

// set opened flag value
void Sample::setOpened( const bool opened ) {
	m_opened = opened;
}

// judges whether this sample is opened or not
bool Sample::isOpened() {
	return m_opened;
}

// set edited flag value
void Sample::setEdited( const bool edited ) {
	m_edited = edited;
}

// judges whether this sample is edited or not
bool Sample::isEdited() {
	return m_edited;
}

// spectrum group
DataGroupNode* Sample::getRootDataGroupNode() {
	return m_root;
}

// hiddent data set
DataSet* Sample::getHiddenDataSet() {
	return &m_hiddenSet;
}

// get the number of spectrum groups
unsigned int Sample::getNumberOfGroups() {
	return m_groups.size();
}

// get spectrum group
DataGroupNode* Sample::getGroup( const unsigned int index ) {
	if( index >= m_groups.size() ) {
		return NULL;
	}
	return m_groups[ index ];
}

// get groups
void Sample::getGroups( DataGroupNode* parentGroup ) {
	if( parentGroup == NULL ) {			// all groups
		// initialize
		m_groups.clear();

		if( m_root != NULL ) {
			getGroups( m_root );
		}
	}
	else {
		// add
		if( parentGroup->getNumberOfChildren() == 0 ) {	// leaf
			// set index
			parentGroup->setGroupIndex( (int)m_groups.size() );
			m_groups.push_back( parentGroup );
		}

		// add children
		for( unsigned int i = 0; i < parentGroup->getNumberOfChildren(); i++ ) {
			// child
			DataGroupNode* child = parentGroup->getChild( i );
			if( child != NULL ) {
				getGroups( child );
			}
		}
	}
}

// set parallel readable
void Sample::setParallelReadable( const bool readable ) {
	m_parallelReadable = readable;
}

// get parallel readable
bool Sample::isParallelReadable() {
	return m_parallelReadable;
}

// open sample
bool Sample::openSample( kome::core::Progress* progress ) {
	if( m_opened ) {		// already opened
		return true;
	}

	// active objects manager
	ActiveObjectsManager& activeMgr = ActiveObjectsManager::getInstance();

	// create root group
	if( m_root != NULL ) {
		delete m_root;
		m_groups.clear();
	}
	std::string groupName = m_name;

	std::string fileName;
	if( m_sampleSet != NULL ) {
		fileName = m_sampleSet->getFileName();
	}

	if( !fileName.empty() ) {
		if( !groupName.empty() ) {
			groupName.append( " : " );
		}
		groupName.append( fileName );
	}

	if( m_sampleSet != NULL && m_sampleSet->getNumberOfSamples() > 1 ) {
		groupName.append( FMT( "(%d)", this->m_sampleIdx ) );
	}

	m_root = new DataGroupNode( this, groupName.c_str() );
	
	// sample set
	SampleSet::startLoadingTimer();
	if( m_sampleSet != NULL ) {
		m_sampleSet->onOpenSample( this, progress ); // add param @date 2014.07.08 <Mod> M.Izumi
	}

	// open
	LOG_INFO( FMT( "Open Sample [%s]", m_name.c_str() ) );
	bool ret = onOpenSample( m_root, progress );
	
	// add sort @date 2011.10.31 <Add> M.Izumi
	m_root->sortSpectra();
		
	SampleSet::stopLoadingTimer();

	// properties
	if( m_sampleSet != NULL ) {
		m_root->getProperties().setValue( "File Path", m_sampleSet->getFilePath() );
		m_root->getProperties().setValue( "File Name", m_sampleSet->getFileName() );
	}
	m_root->getProperties().setValue( "Instrument", m_instrument.c_str() );
	m_root->getProperties().setValue( "Manufacturer", m_instrument.c_str() );
	m_root->getProperties().setValue( "Sample Name", m_name.c_str() );
	m_root->getProperties().setValue( "Sample Type", m_type.c_str() );
	m_root->getProperties().setValue( "Software Name", m_softwareName.c_str() );
	m_root->getProperties().setValue( "Software Version", m_softwareVersion.c_str() );
		 
	if( ret ) {		// success
		m_opened = true;
		getGroups( NULL );

		DataManager::openSample( this );
	}
	else {  // failed
		LOG_ERROR( FMT( "Failed to open the sample.[%s]", m_name.c_str() ) );
		closeSample();
	}


	return ret;
}

// close sample
bool Sample::closeSample() {
	if( !m_opened ) {
		return true;
	}

	// close
	bool ret = onCloseSample();
	DataManager::closeSample( this, false );
	m_opened = false;

	// delete spectra
	if( m_root != NULL ) {
		delete m_root;
		m_root = NULL;
		m_groups.clear();
	}

	// sample set
	if( m_sampleSet != NULL ) {
		m_sampleSet->onCloseSample( this );
	}
	m_specId = 0;	// @date 2013.09.05 <Add> M.Izumi

	return ret;
}

// detect peaks by API
void Sample::detectPeaksByAPI( Spectrum* spec, Peaks* peaks ) {
	onDetectPeaksByAPI( spec, peaks );
}

// check the property key
bool Sample::isCommonProperty( const char* key ) {
	// keys
	const char* keys[] = {
		"File Path",
		"File Name",
		"Instrument",
		"Manufacturer",
		"Sample Name",
		"Sample Type",
		"Software Name",
		"Software Version"
	};

	// create array
	if( m_commonProps.size() == 0 ) {
		const unsigned int num = sizeof( keys ) / sizeof( const char* );

		for( unsigned int i = 0; i < num; i++ ) {
			m_commonProps.push_back( keys[ i ] );
		}
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

// issue sample id
int Sample::issueSampleId(){
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	int inum = aoMgr.getNumberOfOpenedSamples();
	int id = m_optSampleId;
	if( inum > 0 ){
		id++;
	}
	return id;
}

// set sample id
void Sample::setSampleId( int id ){
	m_optSampleId = id;
}

// get sample id
int Sample::getSampleId(){
	return m_optSampleId;
}

// get sample by id
Sample* Sample::getSampleById( int id ){
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	
	int inum = aoMgr.getNumberOfOpenedSamples();
	for( int i=0; i < inum; i++ ){
		Sample* sample = aoMgr.getOpenedSample( i );
		if( m_optSampleId == id ){
			return sample;
		}
	}
	
	return NULL;
}

// issue spectrum id
int Sample::issueSpecId() {
	int specId = m_specId;
	m_specId++;

	return specId;
}

// get spectrum by id
Spectrum* Sample::getSpectrumById( int id ){
	// gets all spectra
	kome::objects::DataSet spectra;

	kome::objects::DataGroupNode* root = getRootDataGroupNode();
	if( root == NULL ) {
		return NULL;
	}
	root->getDataSet( &spectra );

	// search spectrum
	kome::objects::Spectrum* spec = NULL;
	for( unsigned int i = 0; i < spectra.getNumberOfSpectra() && spec == NULL; i++ ) {
		kome::objects::Spectrum* tmp = spectra.getSpectrum( i );
		if( tmp->getId() == id ) {
			spec = tmp;
		}
	}

	return spec;
}

// issue chromatogram id
int Sample::issueChromId( Sample* sample ){
	static int chromId = 0;
	chromId++;

	return chromId;
}

// get chromatogram by id
Chromatogram* Sample::getChromatogramById( int id ){
	// @date 2013.09.03 <Mod> M.Izumi ->
	// gets all spectra
	kome::objects::DataSet chroms;

	kome::objects::DataGroupNode* root = getRootDataGroupNode();
	if( root == NULL ) {
		return NULL;
	}
	root->getDataSet( &chroms );

	// search spectrum
	kome::objects::Chromatogram* chrom = NULL;
	for( unsigned int i = 0; i < chroms.getNumberOfChromatograms() && chrom == NULL; i++ ) {
		kome::objects::Chromatogram* tmp = chroms.getChromatogram( i );
		if( tmp->getId() == id ) {
			chrom = tmp;
		}
	}

	return chrom;
	// @date 2013.09.03 <Mod> M.Izumi <-
}

// issue group id
int Sample::issueGroupId( Sample* sample ){
	static int groupId = 0;

	groupId++;
	return groupId;
}

// get group by id
DataGroupNode* Sample::getGroupById( int id ){
	int inum = getNumberOfGroups();
	for( int i=0; i < inum; i++ ){
		DataGroupNode* group = getGroup(i);
		if( group->getId() == id ){
			return group;
		}
	}
	return NULL;
}

// get API peak detection is supported.
bool Sample::isSupportedAPIPeaks() {
	return m_supportedApiPeaks;
}

// set API peak detection is supported
void Sample::setSupportedAPIPeaks( const bool supported ) {
	m_supportedApiPeaks = supported;
}

// detect peaks using API
void Sample::onDetectPeaksByAPI( kome::objects::Spectrum* spec, kome::objects::Peaks* peaks ) {
	if( !m_supportedApiPeaks ) {
		LOG_WARN( FMT( "This file type does not support API peak detection." ) );
	}
}

// reset id
void Sample::resetId(){
	m_optSampleId = 0;
}
