/**
 * @file BaselineSubtractFunctions.cpp
 * @brief implements of BaselineSubtract plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "BaselineSubtractFunctions.h"
#include "BaselineSubtractManager.h"
#include "BaselineSubtractDialog.h"
#include "BaselineOperation.h"

using namespace kome::baseline;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BASELINE_TYPE_NAME		    "XYDATA_BASELINE"
#define BASELINE_NAME				"baseline"
#define SPEC_PARAM_SHORT_NAME		"spec_baseline_subtract"
#define CHROM_PARAM_SHORT_NAME		"chrom_baseline_subtract"

// subtract baseline
kome::objects::Variant openBaselineDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	// check active objects
	if( spec == NULL && chrom == NULL ) {
		LOG_ERROR( FMT( "A data object is not active" ) );
		return ret;
	}

	// check plug-in
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int num = plgMgr.getNumberOfFunctionItems( BASELINE_TYPE_NAME );
	if( num == 0 ) {
		LOG_ERROR( FMT( "There is no baseline detection algorithm plug-in." ) );
		return ret;
	}

	// open dialog
	BaselineSubtractDialog dlg( kome::window::WindowTool::getMainWindow() );

	int nRet = dlg.ShowModal();
	if( nRet == wxID_OK ) {	// @date 2011/07/27 <Add> OKADA

		// ----------------
		// Operation 作成
		// ----------------
		BaselineOperation* opt = new BaselineOperation();
		if( spec != NULL ){
			opt->setSpec( spec ); 
			opt->setShortName( SPEC_PARAM_SHORT_NAME );
			opt->setTargetSample( spec->getSample() );
			opt->setOperationName( "Baseline Subtract Spectrum" );
		}
		if( chrom != NULL ){
			opt->setChrom( chrom );
			opt->setShortName( CHROM_PARAM_SHORT_NAME );
			opt->setTargetSample( chrom->getSample() );
			opt->setOperationName( "Baseline Subtract Chromatogram" );
		}
						
		opt->setFuncItem( dlg.getBaselineDetector() ) ;
		opt->setBaselineSettings( dlg.getBaselineSettings() );
		// operation type
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
			
		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	ret.prim.boolVal = true;
	return ret;
}

// draw chromatogram baseline
kome::objects::Variant drawChromBaseline( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// draw baseline
	BaselineSubtractManager& mgr = BaselineSubtractManager::getInstance();
	mgr.drawBaseline( chrom, graphics, canvas );

	return ret;
}

// draw spectrum baseline
kome::objects::Variant drawSpecBaseline( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// draw baseline
	BaselineSubtractManager& mgr = BaselineSubtractManager::getInstance();
	mgr.drawBaseline( spec, graphics, canvas );

	return ret;
}

// get spectrum base line operation
kome::objects::Variant getSpecBaselineOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new BaselineOperation();
	
	return ret;
}

// get chromatogram balse line operation
kome::objects::Variant getChromBaselineOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new BaselineOperation();
	
	return ret;
}

// Perform the baseline subtraction.	// #280-1 -baseline_subtract
kome::objects::Variant execBaselineSubtract( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	puts( "Baseline Subtract ..." );	// @date 2013/08/08 <Add> OKADA

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "baseline_subtract" ); 
	std::string baseline_detector = NVL( n, "" );	
	if( baseline_detector.empty() ) {
		LOG_ERROR( FMT( "Failed to get the baseline_detector." ) );
		return ret;
	}

	kome::plugin::PluginFunctionItem* baselineDetector
		= plgMgr.getFunctionItem( BASELINE_TYPE_NAME , baseline_detector.c_str() );
	kome::objects::SettingParameterValues* baselineSettings_1 = settings->getSubParameterValues( "baseline_subtract", baseline_detector.c_str() );

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// active sample0
//	kome::objects::Sample* acSample = aoMgr.getActiveSample();
	// aoMgr.getActiveSample()をそのまま使うと全てのスペクトルに対して処理を行ってしまう。
	// そこで、getFilteredDataSet()を用いて、フィルタされたデータに対して処理を行う。

	// data set
	kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( aoMgr.getActiveSample() );
	if( ds == NULL ) {
		LOG_ERROR( FMT( "Getting active spectra is failed." ) );
		return ret;
	}

	unsigned int uNumberOfSpectra = ds->getNumberOfSpectra();
	kome::objects::Spectrum* spec = NULL;

	// progress
	kome::core::DisplayProgress progress( "baseline Subtract" );
	progress.setRange( 0, uNumberOfSpectra );

	for( unsigned int index = 0; index < uNumberOfSpectra; index++ ){

		progress.setPosition( index );

		spec = ds->getSpectrum( index );
		if( spec == NULL ){
			continue;
		}else{
			BaselineOperation* opt = new BaselineOperation();
			opt->setSpec( spec ); 
			opt->setShortName( SPEC_PARAM_SHORT_NAME );
			opt->setTargetSample( spec->getSample() );
			opt->setOperationName( "Baseline Subtract Spectrum" );

			opt->setFuncItem( baselineDetector  ) ;
			opt->setBaselineSettings( baselineSettings_1  );
			// operation type
			opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );

			if( opt->execute() ){
			}else{
				delete opt;
			}
		}
	}

	progress.fill();

	ret.prim.boolVal = true;
	return ret;
}

// get spec xydata operation ( Baseline Subtraction )
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
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "xydata_baseline", val.c_str(), sub );

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun != NULL ){

		// create baseline subtract object
		kome::operation::BaselineSubtract* baseline = new kome::operation::BaselineSubtract();
		baseline->setBaselineInfo( fun, sub );

		// get xyDataOperation
		ret.prim.pt = baseline;
	}	

	return ret;
}

// get chrom xydata operation ( Baseline Subtraction )
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
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "xydata_baseline", val.c_str(), sub );

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun != NULL ){

		// create baseline subtract object
		kome::operation::BaselineSubtract* baseline = new kome::operation::BaselineSubtract();
		baseline->setBaselineInfo( fun, sub );

		// get xyDataOperation
		ret.prim.pt = baseline;
	}	

	return ret;
}
