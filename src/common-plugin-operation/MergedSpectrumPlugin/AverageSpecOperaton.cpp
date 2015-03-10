/**
 * @file AverageSpecOperaton.cpp
 * @brief averaged spectru Operation class
 *
 * @author M.Izumi
 * @date 2013.08.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "AverageSpecOperaton.h"
#include "MergedSpectrumManager.h"


using namespace kome::operation::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
AverageSpecOperaton::AverageSpecOperaton(){
	m_chrom = NULL;
	m_startRt = -1.0;
	m_endRt = -1.0;

	m_spec = NULL;
}

// destructor
AverageSpecOperaton::~AverageSpecOperaton(){
}

// on execute
bool AverageSpecOperaton::onExecute(){
	bool ret = false;

	// manager
	MergedSpectrumManager&	mgr = MergedSpectrumManager::getInstance();

	// open merged spectrum
	ret = mgr.openMergedSpectrum( m_chrom, m_startRt, m_endRt );

	// refresh
	kome::window::WindowTool::refresh();

	m_spec = mgr.getSpec();
	return ret;
}

// on save condition
void AverageSpecOperaton::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}

// on load condition
void AverageSpecOperaton::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	MergedSpectrumManager& mgr = MergedSpectrumManager::getInstance();

	if( m_spec != NULL ){
		mgr.removeMergedSpectrum( (kome::operation::merged::MergedSpectrum*) m_spec );
		
		delete m_spec;
		m_spec = NULL;
	}else{
		m_spec = mgr.getSpec();
	}

	onExecute();
	
}

// on get description
std::string AverageSpecOperaton::onGetDescription(){
	std::string s;

	s.append( "Merging Spectra" );

	return s;
}

// on get parameters string
std::string AverageSpecOperaton::onGetParametersString(){
	std::string s;
	// sample id, chrom id, start Rt: end Rt
	s.append( FMT( "%d,%d,%f:%f",
		getTargetSample()->getSampleId(),
		m_chrom->getOrgChromatogram()->getId(),
		m_startRt,
		m_endRt) );
	

	return s;
}

// on set parameters string
void AverageSpecOperaton::onSetParametersString( const char* strParam ){
	// sample id, chrom id, start Rt: end Rt
	std::vector< std::string > tokens;
	stringtoken( strParam, ",", tokens );

	if( tokens.size() < 3 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[ 0 ].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}

	setTargetSample( sample );

	m_chrom = sample->getChromatogramById( toint( tokens[ 1 ].c_str(), 10, -1 ) );

	std::vector< std::string > range;
	stringtoken( tokens[ 2 ].c_str(), ":", range );

	m_startRt = todouble( range[ 0 ].c_str(), -1.0 );
	m_endRt = todouble( range[ 1 ].c_str(), -1.0 );
	
}
