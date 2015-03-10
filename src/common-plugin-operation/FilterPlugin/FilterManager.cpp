/**
 * @file FilterManager.cpp
 * @brief implements of FilterManager class
 *
 * @author S.Tanaka
 * @date 2007.08.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "FilterManager.h"
#include "FilterOperation.h"

#include <boost/bind.hpp>


using namespace kome::filter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
FilterManager::FilterManager() {
}

// destructor
FilterManager::~FilterManager() {
}

// subtract chromatogram filter
void FilterManager::executeFilter(
		kome::objects::Chromatogram* chrom,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings,
		int index
) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// create filter subtract object
	kome::operation::Filter* filter = new kome::operation::Filter();
	filter->setFilterInfo( fun, settings );
	filter->setIndex( index );
	LOG_DEBUG( FMT( "Execute %s", filter->getName() ) );
	
	setValMatchOperationChrom( filter, item );

	// -----------------------------------------------------
	// update
	// -----------------------------------------------------
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	ptMgr.addOperation( chrom, filter );

	kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
}

// subtract spectrum filter
void FilterManager::executeFilter(
		kome::objects::Spectrum* spec,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings,
		int index
) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// create filter subtract object
	kome::operation::Filter* filter = new kome::operation::Filter();
	filter->setFilterInfo( fun, settings );
	filter->setIndex( index );
	LOG_DEBUG( FMT( "Execute %s", filter->getName() ) );
	
	setValMatchOperationSpec( filter, item );

	// -----------------------------------------------------
	// update
	// -----------------------------------------------------
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	ptMgr.addOperation( spec, filter );
	
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
}

// Set the value of the matching operation
void FilterManager::setValMatchOperationChrom( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item ){
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int	inum = optMgr.getNumberOfOperations();
	for( int i=0; i < inum; i++ ){
		FilterOperation* opt = (FilterOperation*)optMgr.getOperation( i );
		if( ( opt != NULL ) && strcmp( opt->getShortName(), "chrom_waveform_filter" ) == 0 ){
			// 一致したXYDataOperationにセット
			if( opt->getFuncItem() == item && opt->getxyDataOperation() != (kome::objects::XYDataOperation*)filter ){
				opt->setxyDataOperation( (kome::objects::XYDataOperation*)filter );
				return;
			}
		}		
	}
}

// Set the value of the matching operation
void FilterManager::setValMatchOperationSpec( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item ){
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int	inum = optMgr.getNumberOfOperations();
	for( int i=0; i < inum; i++ ){
		FilterOperation* opt = (FilterOperation*)optMgr.getOperation( i );
		if( ( opt != NULL ) && strcmp( opt->getShortName(), "spec_waveform_filter" ) == 0 ){
			// 一致したXYDataOperationにセット
			if( opt->getFuncItem() == item && opt->getxyDataOperation() != (kome::objects::XYDataOperation*)filter ){
				opt->setxyDataOperation( (kome::objects::XYDataOperation*)filter );
				return;
			}
		}		
	}
}

// get instance
FilterManager& FilterManager::getInstance() {
	// create object (This is the only object.)
	static FilterManager mgr;

	return mgr;
}

