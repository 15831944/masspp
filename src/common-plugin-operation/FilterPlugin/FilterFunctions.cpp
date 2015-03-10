/**
 * @file FilterFunctions.cpp
 * @brief implements of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FilterFunctions.h"
#include "FilterManager.h"
#include "FilterDialog.h"
#include "FilterOperation.h"


using namespace kome::filter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FILTER_TYPE_NAME			 "XYDATA_FILTER"
#define SPEC_PARAM_SHORT_NAME		 "spec_waveform_filter"
#define CHROM_PARAM_SHORT_NAME		 "chrom_waveform_filter"

// subtract filter
kome::objects::Variant openFilterDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	if( spec == NULL && chrom == NULL ) {
		LOG_ERROR( FMT( "A data object is not active" ) );
		return ret;
	}

	// check plug-in
	if( plgMgr.getNumberOfFunctionItems( FILTER_TYPE_NAME ) == 0 ) {
		LOG_ERROR( FMT( "There are no xy data filter plug-ins." ) );
		return ret;
	}

	// open dialog
	FilterDialog dlg( kome::window::WindowTool::getMainWindow() );
	if( dlg.ShowModal() == wxID_OK ) {
		// manager
		FilterManager& mgr = FilterManager::getInstance();

		// filter item
		kome::plugin::PluginFunctionItem* item = dlg.getFilter();
		if( item == NULL ) {
			LOG_ERROR( FMT( "Failed to get the filter." ) );
			return ret;
		}
		
		// filter settings
		kome::objects::SettingParameterValues* filterSettings = dlg.getFilterSettings();
	
		// ----------------
		// Operation ì¬
		// ----------------
		FilterOperation* opt = new FilterOperation();
		if( spec != NULL ){
			opt->setSpec( spec );
			opt->setTargetSample( spec->getSample() );
			opt->setShortName( SPEC_PARAM_SHORT_NAME );
			opt->setOperationName( "Filter Spectrum Waveform" );
		}

		if( chrom != NULL ){
			opt->setChrom( chrom );
			opt->setTargetSample( chrom->getSample() );
			opt->setShortName( CHROM_PARAM_SHORT_NAME );
			opt->setOperationName( "Filter Chromatogram Waveform" );
		}

		opt->setFuncItem( item );
		opt->setWaveformFilterSettings( filterSettings );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
			ret.prim.boolVal = true;
		}else{
			delete opt;
		}
	}

	ret.prim.boolVal = true;
	return ret;
}

// get spectrum filter operation
kome::objects::Variant getSpecFilterOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new FilterOperation();
	
	return ret;

}

// get chromatogram filter operation
kome::objects::Variant getChromFilterOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new FilterOperation();
	
	return ret;
}

// filter
kome::objects::Variant filterWaveformOnBatch( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Getting parameters is failed." ) );
		return ret;
	}

	// filter
	const char* filter = settings->getParameterValue( "filter" );
	if( filter == NULL ) {
		return ret;
	}

	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "XYDATA_FILTER", filter );
	if( item == NULL ) {
		LOG_ERROR( FMT( "Getting filter function is failed." ) );
		return ret;
	}
	
	// data set
	kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( aoMgr.getActiveSample() );
	if( ds == NULL ) {
		LOG_ERROR( FMT( "Getting active spectra is failed." ) );
		return ret;
	}

	// filter
	kome::operation::Filter* op = new kome::operation::Filter();
	op->setFilterInfo( item->getCall(), settings->getSubParameterValues( "filter", filter ) );

	for( unsigned int i = 0; i < ds->getNumberOfSpectra(); i++ ) {
		ptMgr.addOperation( ds->getSpectrum( i ), op );
	}

	ret.prim.boolVal = true;
	return ret;
}

// get spec xydata operation ( Filter )
kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
	
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	std::string strName = settings->getParameterName( 0 );
	std::string val = settings->getParameterValue( strName.c_str() );

	kome::objects::SettingParameterValues* sub = settings->getSubParameterValues( strName.c_str() );
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "xydata_filter", val.c_str(), sub );

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun != NULL ){

		// create filter subtract object
		kome::operation::Filter* filter = new kome::operation::Filter();
		filter->setFilterInfo( fun, sub );

		// get xyDataOperation
		ret.prim.pt = filter;

	}
	return ret;
}

// get chrom xydata operation ( Filter )
kome::objects::Variant getChromXYDataOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ){
		return ret;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		
	std::string strName = settings->getParameterName( 0 );
	std::string val = settings->getParameterValue( strName.c_str() );

	kome::objects::SettingParameterValues* sub = settings->getSubParameterValues( strName.c_str() );
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "xydata_filter", val.c_str(), sub );
		
	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun != NULL ) {
		// create filter subtract object
		kome::operation::Filter* filter = new kome::operation::Filter();

		filter->setFilterInfo( fun, sub );

		// get xyDataOperation
		ret.prim.pt = filter;
	}
	
	return ret;
}
