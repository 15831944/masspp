/**
 * @file SpectraSearchFunctions.cpp
 * @brief implement of Spectra Search plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.01.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraSearchFunctions.h"     
#include "SpectraSearchManager.h"
#include "SpectraSearchFilter.h"
#include "FilterSpectraOperation.h"

using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define RT_PARAM_NAME					"rts"
#define PRECURSOR_PARAM_NAME			"precs"
#define SCAN_PARAM_NAME					"scans"
#define STAGE_PARAM_NAME				"stages"
#define FILTER_KEY_PARAM_NAME			"key"
#define FILTER_RESTRICTION_PARAM_NAME	"restriction"
#define FILTER_STR_PARAM_NAME			"str"
#define TYPE_PARAM_NAME                 "type"


// search spectra
kome::objects::Variant openSpectraSearchDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// show dialog
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();
	mgr.openDialog( obj.getGroup() );

	return ret;
}

// set retention time range
kome::objects::Variant setSpectrumRtRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* rtStr = settings->getParameterValue( RT_PARAM_NAME );
	if( rtStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get retention time ranges." ) );
		return ret;
	}

	const char* typeStr = settings->getParameterValue( TYPE_PARAM_NAME );
	if( typeStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter type." ) );
		return ret;
	}
	SpectraSearchManager::SearchType type = SpectraSearchManager::SEARCH_NARROW;
	if( strcmp( typeStr, "or" ) == 0 ) {
		type = SpectraSearchManager::SEARCH_APPEND;
	}

	SpectraSearchFilter filter;
	filter.setRtRange( rtStr );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		mgr.searchSpectra( sample, type, filter );
	}

	ret.prim.boolVal = true;
	return ret;
}

// set precursor range
kome::objects::Variant setSpectrumPrecursorRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* precStr = settings->getParameterValue( PRECURSOR_PARAM_NAME );
	if( precStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get retention time ranges." ) );
		return ret;
	}

	const char* typeStr = settings->getParameterValue( TYPE_PARAM_NAME );
	if( typeStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter type." ) );
		return ret;
	}

	SpectraSearchManager::SearchType type = SpectraSearchManager::SEARCH_NARROW;
	if( strcmp( typeStr, "or" ) == 0 ) {
		type = SpectraSearchManager::SEARCH_APPEND;
	}

	SpectraSearchFilter filter;
	filter.setPrecursorRange( precStr );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		mgr.searchSpectra( sample, type, filter );
	}

	ret.prim.boolVal = true;
	return ret;
}

// set scan number range
kome::objects::Variant setSpectrumScanNumRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* scanStr = settings->getParameterValue( SCAN_PARAM_NAME );
	if( scanStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get scan number ranges." ) );
		return ret;
	}

	const char* typeStr = settings->getParameterValue( TYPE_PARAM_NAME );
	if( typeStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter type." ) );
		return ret;
	}

	SpectraSearchManager::SearchType type = SpectraSearchManager::SEARCH_NARROW;
	if( strcmp( typeStr, "or" ) == 0 ) {
		type = SpectraSearchManager::SEARCH_APPEND;
	}

	SpectraSearchFilter filter;
	filter.setScanRange( scanStr );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		mgr.searchSpectra( sample, type, filter );
	}

	ret.prim.boolVal = true;
	return ret;
}

// set ms stage range
kome::objects::Variant setSpectrumMsStageRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* stageStr = settings->getParameterValue( STAGE_PARAM_NAME );
	if( stageStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get MS stage ranges." ) );
		return ret;
	}

	const char* typeStr = settings->getParameterValue( TYPE_PARAM_NAME );
	if( typeStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter type." ) );
		return ret;
	}

	SpectraSearchManager::SearchType type = SpectraSearchManager::SEARCH_NARROW;
	if( strcmp( typeStr, "or" ) == 0 ) {
		type = SpectraSearchManager::SEARCH_APPEND;
	}

	SpectraSearchFilter filter;
	filter.setStageRange( stageStr );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		mgr.searchSpectra( sample, type, filter );
	}

	ret.prim.boolVal = true;
	return ret;
}

// set property filter
kome::objects::Variant setSpectrumPropFilter( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get arguments
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* key = settings->getParameterValue( FILTER_KEY_PARAM_NAME );
	if( key == NULL ) {
		LOG_ERROR( FMT( "Failed to get the property filter key." ) );
		return ret;
	}
	const char* str = settings->getParameterValue( FILTER_STR_PARAM_NAME );
	if( str == NULL ) {
		LOG_ERROR( FMT( "Failed to get the restriction string." ) );
	}

	static kome::core::ConversionElement< std::string, SpectraSearchFilter::PropertyFilterType > restrictions[] = {
        { "IS", SpectraSearchFilter::PROP_FILTER_IS },
		{ "IS_NOT", SpectraSearchFilter::PROP_FILTER_IS_NOT },
		{ "CONTAINS", SpectraSearchFilter::PROP_FILTER_CONTAINS },
		{ "DOES_NOT_CONTAIN", SpectraSearchFilter::PROP_FILTER_DOES_NOT_CONTAIN },
        { "IS_MORE_THAN", SpectraSearchFilter::PROP_FILTER_IS_MORE_THAN },
		{ "IS_NOT_MORE_THAN", SpectraSearchFilter::PROP_FILTER_IS_NOT_MORE_THAN },
		{ "IS_LESS_THAN", SpectraSearchFilter::PROP_FILTER_IS_LESS_THAN},
		{ "IS_NOT_LESS_THAN", SpectraSearchFilter::PROP_FILTER_IS_NOT_LESS_THAN }
    };

	std::string restrictionStr = touppercase( settings->getParameterValue( FILTER_RESTRICTION_PARAM_NAME ) );
	SpectraSearchFilter::PropertyFilterType restriction
		= CONVERT_FROM_ARRAY( restrictionStr, SpectraSearchFilter::PROP_FILTER_UNKNOWN, restrictions );
	if( restriction == SpectraSearchFilter::PROP_FILTER_UNKNOWN ) {
		LOG_ERROR( FMT( "Failed to get the property restriction." ) );
		return ret;
	}

	const char* typeStr = settings->getParameterValue( TYPE_PARAM_NAME );
	if( typeStr == NULL ) {
		LOG_ERROR( FMT( "Failed to get the filter type." ) );
		return ret;
	}

	SpectraSearchManager::SearchType type = SpectraSearchManager::SEARCH_NARROW;
	if( strcmp( typeStr, "or" ) == 0 ) {
		type = SpectraSearchManager::SEARCH_APPEND;
	}

	SpectraSearchFilter filter;
	filter.addPropertyFilter( key, str, restriction );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		mgr.searchSpectra( sample, type, filter );
	}

	ret.prim.boolVal = true;
	return ret;
}

// get filter spectra operation
kome::objects::Variant getFilterSpectraOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new FilterSpectraOperation();
	
	return ret;
}

// filter spectra function
kome::objects::Variant filterSpectraFunc( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	kome::objects::DataSet* spectra = kome::plugin::PluginCallTool::getDataSet( params );
	if( spectra == NULL ){
		return ret;
	}

	kome::objects::DataSet* dsp = kome::plugin::PluginCallTool::getSpectraFilter( params );
	
	// show dialog
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// >>>>>>	@Date:2014/02/26	<Modified>	A.Ozaki
	//
	bool	bRet = mgr.openDialog( spectra, *dsp );

	kome::plugin::PluginCallTool::setBool( *params, "RETURN_VALUE", bRet );
	//
	// <<<<<<	@Date:2014/02/26	<Modified>	A.Ozaki

	return ret;
}
