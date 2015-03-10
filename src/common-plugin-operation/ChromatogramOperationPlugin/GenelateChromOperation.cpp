/**
 * @file GenelateChormOperaton.cpp
 * @brief Genelate Chorm Operation class
 *
 * @author M.Izumi
 * @date 2013.07.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "GenelateChromOperation.h"
#include "ChromatogramOperationManager.h"

#define GCD_TYPE_MC				"MC"
#define GCD_TYPE_BPC			"BPC"
#define GCD_TYPE_PROCES			"Processed MC"


using namespace kome::operation::sel_range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::operation::chrom;

// constructor
GenelateChormOperaton::GenelateChormOperaton(){
	m_group = NULL;
	m_startPos = -1.0;
	m_endPos = -1.0;
	m_paramName = "";

	m_chrom = NULL;
}

// destructor
GenelateChormOperaton::~GenelateChormOperaton(){
}

// on execute
bool GenelateChormOperaton::onExecute(){
	bool ret = false;
	
	// manager
	ChromatogramOperationManager& mgr = ChromatogramOperationManager::getInstance();
	
	std::string errMsg;

	if( m_paramName.compare( GCD_TYPE_MC ) == 0 ){
		m_chrom = mgr.createMC( m_group, m_startPos, m_endPos );
		errMsg.append( "Cannot create the mass chromatogram because at least 2 spectra are needed.\nOpen raw data containing at least 2 spectra." );
	}else if( m_paramName.compare( GCD_TYPE_BPC ) == 0 ){
		m_chrom = mgr.createBPC( m_group, m_startPos, m_endPos );
		errMsg.append( "Cannot create the base peak chromatogram because at least 2 spectra are needed.\nOpen raw data containing at least 2 spectra." );
	} // Processed MC @date 2012.10.03 <Add> M.Izumi 
	else if( m_paramName.compare( GCD_TYPE_PROCES ) == 0 ){
		m_chrom = mgr.createProcessedMC( m_group, m_startPos, m_endPos );
		errMsg.append( "Cannot create the mass chromatogram because at least 2 spectra are needed.\nOpen raw data containing at least 2 spectra." );
	}

	m_chrom->setTitle( m_title.c_str() );
	
	if( m_chrom == NULL ){
		kome::window::WindowTool::showError( errMsg.c_str() );
	}else if( kome::plugin::PluginCallTool::onOpenChromatogram( *m_chrom ) ){
		wxBeginBusyCursor();
		// open chromatogram
		kome::plugin::PluginCallTool::openChromatogram( *m_chrom );
		wxEndBusyCursor();

		ret = true;
	}

	return ret;
}

// on save condition
void GenelateChormOperaton::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}

// on load condition
void GenelateChormOperaton::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	if( m_chrom == NULL ){	
		onExecute();
	}	
}

// on get description
std::string GenelateChormOperaton::onGetDescription(){
	std::string s;
	s.append( FMT("Open %s", m_paramName.c_str() ));
	return s;
}

// on get parameters string
std::string GenelateChormOperaton::onGetParametersString(){
	std::string s;
	// sample id, spec id, star m/z : end m/z, param name
	
	s.append( FMT("%d,%d,%f:%f,%s,%s",
		getTargetSample()->getSampleId(),
		m_group->getId(),
		m_startPos,
		m_endPos,
		m_paramName.c_str(),
		m_title.c_str() ) );
	s.append( m_paramName.c_str() );

	return s;
}

// on set parameters string
void GenelateChormOperaton::onSetParametersString( const char* strParam ){
	// sample id, spec id, star m/z : end m/z, param name
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 3 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	m_group = sample->getGroupById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
	
	std::vector< std::string > range;
	stringtoken( tokens[ 2 ].c_str(), ":", range );

	m_startPos = todouble( range[ 0 ].c_str(), -1.0 );
	m_endPos = todouble( range[ 1 ].c_str(), -1.0 );

	m_paramName = tokens[ 3 ].c_str();

	m_title = tokens[ 4 ].c_str();
}
