/**
 * @file AverageSampleOperation.cpp
 * @brief Average Sample Operation class
 *
 * @author M.Izumi
 * @date 2012.01.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "AverageSampleOperation.h"
#include "SampleMergeManager.h"
#include "MergedSampleSet.h"

using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
AverageSampleOperation::AverageSampleOperation(){

	// 初期化
	m_rtTol = -1.0;
	m_mzTol = -1.0;
	m_samples.clear();
}

// destructor
AverageSampleOperation::~AverageSampleOperation(){	
}

// execute
bool AverageSampleOperation::onExecute(){
	MergedSampleSet* ss = new MergedSampleSet( m_rtTol, m_mzTol );
	// マージ用のサンプル追加 @date 2012.10.09 <Add> M.Izumi
	for( unsigned int i=0; i < m_samples.size(); i++ ){
		ss->addMergeSample( m_samples[i] );
	}
	
	ss->openSampleSet();

	int inum = ss->getNumberOfSamples()-1;
	// sample setを開いた後にTargetSampleセット
	kome::objects::Sample* mgrSample = ss->getSample( inum );
	setTargetSample( mgrSample );

	return true;
}

// on save condition
void AverageSampleOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}

// on load condition
void AverageSampleOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
}

// get the description
std::string AverageSampleOperation::onGetDescription(){
	std::string s;
	if( m_targetSample != NULL ){
		s.append( FMT("Averaged Sample %s:%s", 
			m_targetSample->getName(), m_targetSample->getSampleSet()->getFileName() ) );
	}
	return s;
}

// gets the parameter string
std::string AverageSampleOperation::onGetParametersString(){
	std::string s;
	
	//  rt tol, sample id : group id, .... 
	s.append( FMT( "%f,%f", m_rtTol, m_mzTol ) );
		
	for( unsigned int i=0; i < m_samples.size(); i++ ){
		kome::objects::Sample* sample = m_samples[i];
		if( sample != NULL ){
			s.append( FMT( ",%d", sample->getSampleId() ));
		}
	}
	
	return s;
}

// sets the parameter string
void AverageSampleOperation::onSetParametersString( const char* strParam ){
	std::vector< std::string > tokens;
	stringtoken( strParam, ",", tokens );
		
	if( tokens.size() < 1 ){
		return;
	}
	m_rtTol = todouble( tokens[0].c_str(),  -1.0 );
	m_mzTol = todouble( tokens[1].c_str(),  -1.0 );
	for( unsigned int i=2; i < tokens.size(); i++ ){
		int index = tokens[i].find_first_of(":");
		std::string strSampleId = tokens[i].substr(0,index);
		std::string strGroupId = tokens[i].substr(index);
		kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[i].c_str(), 10, -1 ));
		if( sample != NULL ){
			m_samples.push_back( sample );
		}			
	}
}

// set RT toleerance
void AverageSampleOperation::setRtTol( double rtTol ){
	m_rtTol = rtTol;
}

// get RT toleerance
double AverageSampleOperation::getRtTol(){
	return m_rtTol;
}

// set m/z tolerance
void AverageSampleOperation::setMzTol( double mzTol ) {
	m_mzTol = mzTol;
}

// get m/z tolerance
double AverageSampleOperation::getMzTol() {
	return m_mzTol;
}

// set samples
void AverageSampleOperation::setSamples( std::vector< kome::objects::Sample* > samples ){
	m_samples = samples;
}

// get samples
std::vector< kome::objects::Sample* > AverageSampleOperation::getSamples(){
	return m_samples;
}

