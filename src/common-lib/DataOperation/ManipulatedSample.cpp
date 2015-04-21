/**
 * @file ManipulatedSample.cpp
 * @brief Manipulated Sample class
 *
 * @author M.Izumi
 * @date 2012.11.12
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "ManipulatedSample.h"
#include "ManipulatedSpectrum.h"
#include "Normalization.h"
#include "TransformedChromatogram.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ManipulatedSample::ManipulatedSample( kome::objects::Sample& org )
		: kome::objects::Sample( NULL ), m_org( org ){

	m_opts.clear();

	setType( org.getType() );
	setSampleIndex( org.getSampleIndex() );
	setSampleId( org.getSampleId() );
	setName( org.getName() );
	setOpened( org.isOpened() );
	setEdited( org.isEdited() );
	
	if( m_root != NULL ){
		delete m_root;
	}
	m_root = NULL;

	m_root = new kome::objects::DataGroupNode( this, org.getRootDataGroupNode()->getName() );
	if( m_root != NULL ){
		m_root->clearSpectra();
		m_root->clearChromatograms();
	}
	addSpectrum( org.getRootDataGroupNode(), m_root );	
	getGroups( NULL );
}

// destructor
ManipulatedSample::~ManipulatedSample(){	
	if( m_root != NULL ){
		delete m_root;
	}
	m_root = NULL;
	
	clearOperation();
}

// add spectrum
void ManipulatedSample::addSpectrum( kome::objects::DataGroupNode* orgGroup, kome::objects::DataGroupNode* dstGroup ){	
	if( orgGroup != NULL && dstGroup != NULL ){
		// creating spectra
		for( unsigned int i=0; i < orgGroup->getNumberOfSpectra(); i++ ){			
			kome::operation::ManipulatedSpectrum* spec = new kome::operation::ManipulatedSpectrum( this, *orgGroup->getSpectrum(i) );
			dstGroup->addSpectrum( spec );
			spec->setSample( this );
		}

		// creating chromatograms
		for( unsigned int i = 0; i < orgGroup->getNumberOfChromatograms(); i++ ) {
			kome::objects::Chromatogram* orgChrom = orgGroup->getChromatogram( i );
			TransformedChromatogram* chrom = new TransformedChromatogram( *orgChrom, false );
			dstGroup->addChromatogram( chrom );
		}

		// child groups
		for( unsigned int i = 0; i < orgGroup->getNumberOfChildren(); i++ ) {
			kome::objects::DataGroupNode* orgChild = orgGroup->getChild( i );
			kome::objects::DataGroupNode* dstChild = dstGroup->createChildGroup( orgChild->getName() );

			addSpectrum( orgChild, dstChild );
		}
	}	
}

// add normaliz operation
void ManipulatedSample::addOperation(kome::objects::XYDataOperation* opt){
	bool bflg = false;
	for( unsigned int i=0; i < m_opts.size(); i++ ){
		if( opt == m_opts[i] ){
			bflg = true;
			break;
		}
	}
	if( bflg ){
		m_opts.push_back( opt );
	}
}

// remove operation
void ManipulatedSample::removeOperation( kome::objects::XYDataOperation* opt ){
	
	for( unsigned int i=0; i < m_opts.size(); i++ ){
		if( opt == m_opts[i] ){
			delete m_opts[i];
			m_opts.erase( m_opts.begin() + i );
			return;
		}
	}
}

// clear Operation
void ManipulatedSample::clearOperation(){
	for( unsigned int i=0; i < m_opts.size(); i++ ){
		delete m_opts[i];
	}
	m_opts.clear();
}

// set normaliz operation info
void ManipulatedSample::setOperationInfo( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings, kome::objects::DataGroupNode* stdGroup ){
	// create filter subtract object
	Normalization* normaliz = new Normalization();
	
	normaliz->setNormalizInfo( call, settings, stdGroup );
	m_opts.push_back( (kome::objects::XYDataOperation*)normaliz );
}

// on open sample
bool ManipulatedSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ){
	return true;
}

// on close sample
bool ManipulatedSample::onCloseSample(){
	if( m_root != NULL ){
		delete m_root;
	}
	m_root = NULL;
	
	clearOperation();

	return true;
}

// get org sample
kome::objects::Sample* ManipulatedSample::getOrgSample(){
	return &m_org;
}

// get number of operation
unsigned int ManipulatedSample::getNumberOfOperation(){
	return m_opts.size();
}

// get operation
kome::objects::XYDataOperation* ManipulatedSample::getOperation( int index ){
	
	// check the index
	if( index >= (int)m_opts.size() ) {
		return NULL;
	}
	return m_opts[ index ];

}
