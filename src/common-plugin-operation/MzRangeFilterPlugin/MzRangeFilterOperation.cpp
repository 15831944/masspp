/**
 * @file MzRangeFilterOperation.cpp
 * @brief MzRangeFilter Operation class
 *
 * @author M.Izumi
 * @date 2013.05.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "MzRangeFilterManager.h"
#include "MzRangeFilterOperation.h"

#include <sstream>

using namespace kome::mzfilter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MzRangeFilterOperation::MzRangeFilterOperation(){
	m_startMz = -1.0;
	m_endMz = -1.0;
	m_spectra.clear();
	m_filters.clear();
}

// destructor
MzRangeFilterOperation::~MzRangeFilterOperation(){
}

// execute
bool MzRangeFilterOperation::onExecute(){
	bool ret = false;
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	
	// progress
	kome::core::Progress* pprogress = NULL;		
	kome::window::DialogProgress* progress = NULL;
	if( kome::window::WindowTool::getMainWindow() != NULL ){
		// progress
		progress = new kome::window::DialogProgress( kome::window::WindowTool::getMainWindow(),	"Mz Range Filter" );
		pprogress = progress;
	}

	if( progress == NULL ){
		pprogress = &kome::core::Progress::getIgnoringProgress();
	}
	
	pprogress->setRange( 0, m_spectra.size() + 1 );
	pprogress->setPosition( 0 );

	for( unsigned int i=0; i < m_spectra.size() && !pprogress->isStopped(); i++ ){
		kome::objects::Spectrum* spec = m_spectra[i];
		if( spec != NULL ){
			// progress
			pprogress->setStatus( FMT( "Mz Range Filter... [%d/%d]", ( i + 1 ), ( m_spectra.size() ) ).c_str() );
			
			addOperation( spec );
			kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		}	

		// progress
		pprogress->setPosition( i + 1 );
	}
	
	if( !pprogress->isStopped() ){
		// progress
		pprogress->fill();
	}

	if( progress != NULL ){
		delete progress;
	}

	ret = true;
	return ret;
}

// save condition
void MzRangeFilterOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
	int inum = m_spectra.size();
	writeFun( &inum, sizeof( inum ) );

	for( unsigned int i=0; i < m_spectra.size(); i++ ){
		kome::core::XYData* xyData = NULL;
		kome::objects::Spectrum* spec = m_spectra[i];
				
		if( spec != NULL && kome::window::SpectrumCanvas::getCanvas( spec ) != NULL ){
			int id = spec->getId();
			writeFun( &id, sizeof(id) );
			xyData = spec->getXYData();
		}
		
		if( xyData != NULL ){
			// export
			xyData->exportData( writeFun );
		}				
	}
}

// load condition
void MzRangeFilterOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	
	int inum = 0;
	readFun( &inum, sizeof( inum ) );

	kome::objects::Sample* sample = getTargetSample();
	for( int i=0; i < inum; i++ ){
		kome::objects::Spectrum* spec = NULL;

		int id = int();
		readFun( &id, sizeof( id ) );

		if( sample != NULL ){
			spec = sample->getSpectrumById( id );
		}

		if( spec != NULL ){
		
			std::vector< kome::operation::MzRangeFilter* > filter = m_filters[ spec ];
			int opts = ptMgr.getNumberOfOperations( spec );
				
			bool bflg = false;
			// Undo
			for( int i=0; i < opts; i++ ){
				kome::objects::XYDataOperation* opt = ptMgr.getOperation( spec, i );
				
				for( unsigned int j=0; j < filter.size(); j++ ){
					if( (kome::objects::XYDataOperation*)filter[j] == opt ){
						ptMgr.removeOperation( spec, opt );
						filter.erase( filter.begin() + j );
						bflg = true;
					}
				}
					
			}
			// Redo
			if( !bflg ){
				addOperation( spec );			
			}
								
			kome::core::XYData* xyData = NULL;
	
			xyData = spec->getXYData();
			if( xyData != NULL ){
				xyData->clearPoints();
				xyData->importData( readFun );
			}
			kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		}
	}

	kome::window::WindowTool::refresh();
}

// get description
std::string MzRangeFilterOperation::onGetDescription(){
	std::string s;

	s.append( FMT("[m/z Range: %.2f-%.2f]", m_startMz, m_endMz ) );
	
	return s;
}

// set parameter string
void MzRangeFilterOperation::onSetParametersString( const char* strParam ){
	std::vector<std::string> tokens;
	stringseparate( strParam, ",", tokens );
 
	if( tokens.size() < 3 ){
		return;
	}

	// sample
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// spectra
	kome::objects::DataGroupNode* group = sample->getGroupById( toint( tokens[1].c_str(), 10, -1 ) );
	if( group == NULL ){
		return;
	}

	m_startMz = todouble( tokens[2].c_str(), -1.0 );
	m_endMz = todouble( tokens[3].c_str(), -1.0 );
	
	for( unsigned int i=4; i < tokens.size(); i++ ){
		int id = toint( tokens[ i ].c_str(), 10, -1 );
		kome::objects::Spectrum* spec = sample->getSpectrumById( id );
		if( spec != NULL ){
			m_spectra.push_back( spec );
			addOperation( spec );	
		}
	}
}

// get parameters string
std::string MzRangeFilterOperation::onGetParametersString(){
	std::string s;
		
	kome::objects::Sample* sample = getTargetSample();
	if( sample != NULL ){
		s.append( FMT( "%d,", sample->getSampleId() ) );
		kome::objects::DataGroupNode* group = sample->getRootDataGroupNode();
		if( group != NULL ){
			s.append( FMT("%d", group->getId() ) );
		}
	}

	s.append( FMT(",%f,%f", m_startMz, m_endMz ) );
		
	for( unsigned int i=0; i < m_spectra.size(); i++ ){
		kome::objects::Spectrum* spec = m_spectra[i];
		if( spec != NULL ){
			
			std::stringstream ss;
			std::string tmp;
			ss << ( spec->getOrgSpectrum() == NULL ? spec->getId() :spec->getOrgSpectrum()->getId() );
			ss >> tmp;
			
			s.append( "," );
			s.append( tmp );
		}
	}

	return s;
}

// set start m/z
void MzRangeFilterOperation::setStartMz( double startMz ){
	m_startMz = startMz;
}

// get start m/z
double MzRangeFilterOperation::getStartMz(){
	return m_startMz;
}

// set end m/z
void MzRangeFilterOperation::setEndMz( double endMz ){
	m_endMz = endMz;
}

// get end m/z
double MzRangeFilterOperation::getEndMz(){
	return m_endMz;
}

// set spectra
void MzRangeFilterOperation::setSpectra( std::vector< kome::objects::Spectrum* > spectra ){
	m_spectra = spectra;
}

// get spectra
std::vector< kome::objects::Spectrum* > MzRangeFilterOperation::getSpectra(){
	return m_spectra;
}

void MzRangeFilterOperation::addOperation( kome::objects::Spectrum* spec ){
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	if( spec != NULL ){
		kome::operation::MzRangeFilter* filter = new kome::operation::MzRangeFilter( m_startMz, m_endMz );
		filter->setName( FMT( "m/z Range Filter [ Scan: %d ]", spec->getScanNumber() ).c_str() );
			
		ptMgr.addOperation( spec, filter );
			
		std::vector<kome::operation::MzRangeFilter*> filter_opt = m_filters[ spec ];
		filter_opt.push_back( filter );

		m_filters[ spec ] = filter_opt;
	}
}
