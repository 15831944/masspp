/**
 * @file BackgroundOperation.cpp
 * @brief Background Subtraction Operation class
 *
 * @author M.Izumi
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BackgroundOperation.h"
#include "BackgroundSubtractManager.h"
#include "BackgroundSubtraction.h"

using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
BackgroundOperation::BackgroundOperation(){
	// èâä˙âª
	m_spec = NULL;
	m_bs = NULL;
	m_bgSpec = NULL;
}

// destructor
BackgroundOperation::~BackgroundOperation(){
}

// on execute
bool BackgroundOperation::onExecute(){
	
	if( m_bs == NULL ){
		LOG_ERROR( FMT("The background spectrum is not specified yet.") );
		return false;
	}
	if( m_spec == NULL ){
		LOG_ERROR( FMT("This operation requires an active spectrum window.") );
		return false;
	}

	// managers
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	
	// -----------------------------------------------------
	// Background Subtraction
	// -----------------------------------------------------
	ptMgr.addOperation( m_spec, m_bs );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *m_spec );
	mgr.setBackgroundFlag();

	return true;
}


// on load condition
void BackgroundOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	int id = 0;
	readFun( &id, sizeof( id ) );

	kome::objects::Sample* sample = getTargetSample();
	kome::objects::Spectrum* spec = NULL;
	if( sample != NULL ){
		spec = sample->getSpectrumById( id );
	}
	
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	
	// Redo
	if( m_bs == NULL ){
		mgr.setBackground( m_bgSpec );
		m_bs = mgr.getCurrentBackgroundSubtraction();
		ptMgr.addOperation( spec, m_bs );
		mgr.setBackgroundFlag();
		
	// Undo
	}else{
		int inum = ptMgr.getNumberOfOperations( spec );
		for( int i=0; i < inum; i++ ){
			kome::objects::XYDataOperation* opt = ptMgr.getOperation( spec, i );
			if( m_bs == opt ){
				ptMgr.removeOperation( spec, opt );
			
				m_bs = NULL;
				
				break;
			}
		}
	}
	mgr.setBackground( NULL );

	kome::core::XYData* xyData = NULL;
	
	// spectrum
	if( spec != NULL ){
		xyData = spec->getXYData();
		if( xyData != NULL ){
			xyData->clearPoints();
			xyData->importData( readFun );
		}
		
		// update
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}

	kome::window::WindowTool::refresh();
}

// on save condition
void BackgroundOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
	
	kome::core::XYData* xyData = NULL;

	if( m_spec != NULL ){
		int id = (m_spec->getOrgSpectrum() == NULL ? m_spec->getId() : m_spec->getOrgSpectrum()->getId() );
		writeFun( &id, sizeof( id ) );
		xyData = m_spec->getXYData();
		if( xyData != NULL ){
			// export 
			xyData->exportData( writeFun );
		}
	}
}

// get the description
std::string BackgroundOperation::onGetDescription(){
	
	std::string s;
	if( m_spec != NULL ){
		s.append( FMT("[%s] Background Subtraction", m_spec->getName() ) );
	}
	return s;
}

// gets the parameter string
std::string BackgroundOperation::onGetParametersString(){
	// sample Id, spec Id, background sample Id, background specId
	std::string s;
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	if( m_spec != NULL && m_bgSpec != NULL ){
		s.append( FMT( "%d,%d,%d,%d",
			m_spec->getSample()->getSampleId(),
			m_spec->getOrgSpectrum()->getId(),
			m_bgSpec->getOrgSpectrum()->getSample()->getSampleId(),
			m_bgSpec->getOrgSpectrum()->getId()).c_str());
	}
	return s;
}

// sets the parameter string
void BackgroundOperation::onSetParametersString( const char* strParam ){
	// sample Id, spec Id, background sample Id, background specId
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	if( tokens.size() < 3 ){
		return;
	}
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}

	m_spec = sample->getSpectrumById( toint( tokens[1].c_str(), 10, -1 ) );
	kome::objects::Sample* bcSample = kome::objects::Sample::getSampleById( toint( tokens[2].c_str(), 10, -1 ));
	if( bcSample == NULL ){
		return;
	}
	kome::objects::Spectrum* bsSpec = bcSample->getSpectrumById( toint( tokens[3].c_str(), 10, -1 ) );
	mgr.setBackground( bsSpec );

	m_bs = mgr.getCurrentBackgroundSubtraction();
}

// set spectrum
void BackgroundOperation::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// get spectrum
kome::objects::Spectrum* BackgroundOperation::getSpec(){
	return m_spec;
}

// set current background subtraction
void BackgroundOperation::setBackgroundSubtraction( BackgroundSubtraction* bs ){
	m_bs = bs;
}

// get current background subtraction
BackgroundSubtraction* BackgroundOperation::getBackgroundSubtraction(){
	return m_bs;
}

// get background spectrum
void BackgroundOperation::setBackgroundSpec( kome::objects::Spectrum* spec ){
	m_bgSpec = spec;
}

// set background spectrum
kome::objects::Spectrum* BackgroundOperation::getBackgroundSpec(){
	return m_bgSpec;
}
