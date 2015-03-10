/**
 * @file FilterOperation.cpp
 * @brief Filter Operation class
 *
 * @author M.Izumi
 * @date 2012.01.25
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "FilterOperation.h"
#include "FilterManager.h"

using namespace kome::filter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SPEC_PARAM_SHORT_NAME		"spec_waveform_filter"
#define CHROM_PARAM_SHORT_NAME		"chrom_waveform_filter"

// constructor
FilterOperation::FilterOperation(){	
	
	// èâä˙âª
	m_chrom = NULL;
	m_spec = NULL;
	m_item = NULL;
	m_settings.clear();
	m_xyOpt = NULL;
	m_index = -1;
}

// destructor
FilterOperation::~FilterOperation(){
}

// on execute
bool FilterOperation::onExecute(){
	bool ret = false;
	FilterManager& mgr = FilterManager::getInstance();
	
	if( m_spec == NULL && m_chrom == NULL ) {
		LOG_ERROR( FMT( "A data object is not active" ) );
		return ret;
	}

	if( m_item == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter." ) );
		return ret;
	}

	// filter
	if( m_spec != NULL ) {
		mgr.executeFilter( m_spec, m_item, &m_settings );
	}
	if( m_chrom != NULL ) {
		mgr.executeFilter( m_chrom, m_item, &m_settings );
	}
	
	ret = true;
	return ret;
}

// on save condition
void FilterOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
	kome::objects::Chromatogram* chrom = getChrom();
	kome::objects::Spectrum* spec = getSpec();
	
	kome::core::XYData* xyData = NULL;
	
	if( chrom != NULL ){
		int id = chrom->getOrgChromatogram()->getId();
		writeFun( &id, sizeof(id) );
		xyData = chrom->getXYData();			
	}
	if( spec != NULL ){
		int id = spec->getOrgSpectrum()->getId();
		writeFun( &id, sizeof(id) );
		xyData = spec->getXYData();
	}
	if( xyData == NULL ){
		return;
	}
		
	// export
	xyData->exportData( writeFun );
	
}

// on load condition
void FilterOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	int id = 0;
	readFun( &id, sizeof(id) );
		
	kome::objects::Chromatogram* chrom = NULL;
	kome::objects::Spectrum* spec = NULL;
	kome::objects::Sample* sample = getTargetSample();
	if( sample != NULL ){
		chrom = sample->getChromatogramById( id );
	}

	// chromatogram
	if( chrom != NULL && m_chrom != NULL ){		
		if( chrom->getOrgChromatogram() == m_chrom->getOrgChromatogram() ){
			updateChrom( m_chrom, readFun );
		}
	}

	// spectrum
	spec = sample->getSpectrumById( id );
	if( spec != NULL && m_spec != NULL ){
		if( spec->getOrgSpectrum() == m_spec->getOrgSpectrum() ){
			updateSpec( m_spec, readFun );
		}
	}
	
	kome::window::WindowTool::refresh();
}

// on get description
std::string FilterOperation::onGetDescription(){
	std::string s;

	if( m_item != NULL ){
		s.append( FMT("%s ", m_item->getLongName() ) );
	}

	if( m_spec != NULL ){
		s.append( FMT("[%s](", m_spec->getName() ) );
	}

	if( m_chrom != NULL ){
		s.append( FMT("[%s](", m_chrom->getName() ) );
	}

	if( &m_settings != NULL ){
		int inum = m_settings.getNumberOfParameters();
		for( int i=0; i < inum; i++ ){
			s.append( FMT("[%s=%s]", m_settings.getParameterName(i), m_settings.getParameterValue(i)) );
			// sub
			kome::objects::SettingParameterValues* sub = m_settings.getSubParameterValues( m_settings.getParameterName(i), m_settings.getParameterValue(i) );
			int subnum = sub->getNumberOfParameters();
			for( int j=0; j < subnum; j++ ){
				s.append( FMT("[%s=%s]", sub->getParameterName(j), sub->getParameterValue(j)) );
			}
		}
		s.append(")");
	}
	return s;
}

// on get parameters string
std::string FilterOperation::onGetParametersString(){
	std::string s;
	// get function
	kome::plugin::PluginCall* fun = ( m_item == NULL ) ? NULL : m_item->getCall();
	if( fun == NULL ) {
		return s;
	}
		
	if( m_spec != NULL ){
		
		// sample Id, spec Id, item type, item name, param value...
		s.append( FMT("%d,%d",
			m_spec->getSample()->getSampleId(),
			( m_spec->getOrgSpectrum() != NULL ?  m_spec->getOrgSpectrum()->getId() : m_spec->getId() ))
		);
	}

	if( m_chrom != NULL ){
		// sample Id, chrom Id, item type, item name, param value...
		s.append( FMT("%d,%d",
			m_chrom->getSample()->getSampleId(),
			( m_chrom->getOrgChromatogram() != NULL ? m_chrom->getOrgChromatogram()->getId() : m_chrom->getId() ))
		);
	}

	s.append( FMT(",%s,%s",
		fun->getType(),
		fun->getProperties().getStringValue("short name",""))
	);

	kome::plugin::SettingsPage* page = m_item->getSettingsPage();
	if( page != NULL ){
		s.append( FMT(",%s", page->getParameterString( m_settings ).c_str()) );
	}
	return s;
}

// on set parameters string
void FilterOperation::onSetParametersString( const char* strParam ){
	// sample Id, chrom Id, funitem type, funcitem name, [param value]....
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 3 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ));
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	std::string shortName = getShortName();
	// spectrum
	if( shortName.compare( SPEC_PARAM_SHORT_NAME ) == 0 ){
		m_spec = sample->getSpectrumById( toint( tokens[1].c_str(), 10, -1 ) );

	// chromatogram
	}else if( shortName.compare( CHROM_PARAM_SHORT_NAME ) == 0 ){
		m_chrom = sample->getChromatogramById( toint( tokens[1].c_str(), 10, -1 ) );
	}

	// function item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_item = plgMgr.getFunctionItem( tokens[2].c_str(), tokens[3].c_str() );

	std::string param;
	param.append( strParam );
	int index = param.find_first_of( "[" );
	std::string strSettings = param.substr(index);
	
	// setting parameter values
	kome::plugin::SettingsPage* page = m_item->getSettingsPage();
	if( page != NULL ){
		page->setParameterString( m_settings, strSettings.c_str() );
	}
}

// update chromatogram
void FilterOperation::updateChrom( kome::objects::Chromatogram* chrom, boost::function< int ( void*, int ) > readFun ){
	editXYDataOperation( chrom );

	kome::core::XYData* xyData = NULL;

	xyData = chrom->getXYData();
	if( xyData != NULL ){
		xyData->clearPoints();
		xyData->importData( readFun );
	}
		
	// update
	kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
}

// update spectrum
void FilterOperation::updateSpec( kome::objects::Spectrum* spec, boost::function< int ( void*, int ) > readFun ){
	editXYDataOperation( spec );
	
	kome::core::XYData* xyData = NULL;
	
	xyData = spec->getXYData();
	if( xyData != NULL ){
		xyData->clearPoints();
		xyData->importData( readFun );
	}
		
	// update
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
}

// Edit XYDataOperation ( chromatogram )
void FilterOperation::editXYDataOperation( kome::objects::Chromatogram* chrom ){
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	// Redo
	if( m_xyOpt == NULL ){
		FilterManager& mgr = FilterManager::getInstance();
		// new Filter 
		mgr.executeFilter( chrom, m_item, &m_settings, m_index );
	// Undo
	}else{
		int inum = ptMgr.getNumberOfOperations( chrom );
		for( int i=0; i < inum; i++ ){
			kome::objects::XYDataOperation* opt = ptMgr.getOperation( chrom, i );
			if( opt == m_xyOpt ){
				m_index = i;
				// delete Filter
				ptMgr.removeOperation( chrom, opt );
				m_xyOpt = NULL;
				return;
			}
		}
	}
}

// Edit XYDataOperation ( spectrum )
void FilterOperation::editXYDataOperation( kome::objects::Spectrum* spec ){
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	// Redo
	if( m_xyOpt == NULL ){
		FilterManager& mgr = FilterManager::getInstance();
		mgr.executeFilter( spec, m_item, &m_settings, m_index );
	// Undo
	}else{
		int inum = ptMgr.getNumberOfOperations( spec );
		for( int i=0; i < inum; i++ ){
			kome::objects::XYDataOperation* opt = ptMgr.getOperation( spec, i );
			if( opt == m_xyOpt ){
				m_index = i;
				// delete Filter
				ptMgr.removeOperation( spec, opt );
				m_xyOpt = NULL;
				return;
			}
		}
	}
}

// set chromatogram
void FilterOperation::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get chromatogram
kome::objects::Chromatogram* FilterOperation::getChrom(){
	return m_chrom;
}

// set spectrum
void FilterOperation::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// get spectrum
kome::objects::Spectrum* FilterOperation::getSpec(){
	return m_spec;
}

// set waveform filter function item
void FilterOperation::setFuncItem( kome::plugin::PluginFunctionItem* item ){
	m_item = item;
}

// get waveform filter function item
kome::plugin::PluginFunctionItem* FilterOperation::getFuncItem(){
	return m_item;
}

// set waveform filter setting parameter values
void FilterOperation::setWaveformFilterSettings( kome::objects::SettingParameterValues* paramValue ){
	m_settings = *paramValue;
}

// get waveform filter setting parameter values
kome::objects::SettingParameterValues FilterOperation::getWaveformFilterSettings(){
	return m_settings;
}

// set xyDataOperation
void FilterOperation::setxyDataOperation(kome::objects::XYDataOperation* opt){
	m_xyOpt = opt;
}

// get xyDataOperation
kome::objects::XYDataOperation* FilterOperation::getxyDataOperation(){
	return m_xyOpt;
}
