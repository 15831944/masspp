/**
 * @file PeakPickingPluginFunctions.cpp
 * @brief implements of PeakPicking plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakPickingPluginFunctions.h"   
#include "PeakPickingManager.h"
#include "SpectrumLabelingDialog.h"
#include "ChromatogramLabelingDialog.h"
#include "Peaks2dManager.h"	// merge from Peaks2dPluginFunctions.cpp
#include "LabelingDialog.h"	// merge from Peaks2dPluginFunctions.cpp
#include "PeakDetectOperation.h"

#include <boost/progress.hpp>


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_DETECTOR_NAME                      "peakDetector"
#define CHARGE_DETECTOR_NAME                    "chargeDetector"
#define PARENT_RANGE_PARAM_NAME					"parent range"

#define SECTION									"Auto Labeling"
#define MS_SPEC_LABELING_KEY					"DEFAULT_MS_SPEC_LABELING"
#define MSMS_SPEC_LABELING_KEY					"DEFAULT_MSMS_SPEC_LABELING"
#define CHARGE_DETECTOR_KEY						"DEFAULT_CHARGE_DETECTOR"
#define PARENT_LABELING_KEY						"DEFAULT_PARENT_PEAK_DETECTOR"
#define CHROM_LABELING_KEY						"DEFAULT_CHROM_LABELING"
#define CHROM_PEAK_ALG_KEY						"CHROMATOGRAM_PEAK_ALGORITHM"

#define SPECTRUM_PEAK_PICKING_FUNC_TYPE			"SPEC_PEAK_PICKING"
#define CHARGE_DETECT_FUNC_TYPE					"CHARGE_DETECT"
#define CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE		"CHROM_PEAK_PICKING"

#define LABELING_FUNC_NAME						"2D_PEAK_PICKING"

#define DEFAULT_SPEC_PEAK_DETECT				"local"

// execute spectrum labeling
kome::objects::Variant executeSpectrumLabeling( kome::objects::Parameters* params ) {					   
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = true;
	ret.type = kome::objects::Variant::BOOL;

	// active spectrum
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* acSpec = obj.getSpectrum();

	// check peak picking plug-in
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	if( plgMgr.getNumberOfFunctionItems( SPECTRUM_PEAK_PICKING_FUNC_TYPE ) == 0 ) { // no peak picking
		LOG_ERROR_CODE( FMT( "No peak detection algorithms are installed.\nExit Mass++ and install at least one peak detection plug-in." ), ERR_OTHER );
		kome::window::WindowTool::showError( getlasterrormessage() );

		return ret;
	}

	// check charge detect plug-in
	if( plgMgr.getNumberOfFunctionItems( CHARGE_DETECT_FUNC_TYPE ) == 0 ) { // no charge detector
		LOG_WARN_CODE( FMT( "No charge detection algorithms are installed.\nOK - continue without using charge detection.\nCancel - exit Mass++ and install at least one charge detection plug-in." ), ERR_OTHER );

		if( wxMessageBox( wxT( getlasterrormessage() ), wxT( "Labeling" ), wxOK | wxCANCEL | wxICON_EXCLAMATION ) == wxCANCEL ) {
			return ret;
		}
	}

	// manager object
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// open dialog
	SpectrumLabelingDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		acSpec
	);

	if( dlg.ShowModal() == wxID_OK ) {
		// get spectra
		std::vector< kome::objects::Spectrum* > targetSpectra;
		kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
		mgr.getTargetSpectra( var, targetSpectra, dlg.getTarget() );
		if( targetSpectra.size() == 0 ) {
			wxMessageBox(
				wxT( "There are no spectra to be peak-detected in the specified target. Open a data file containing spectra." ),
				wxT( "Labeling" ),
				wxOK | wxICON_HAND
			);
			return ret;
		}			
		
		// function
		kome::plugin::PluginFunctionItem* peakDetector = dlg.getPeakPickingFunction();
		kome::plugin::PluginFunctionItem* chargeDetector = dlg.getChargeDetectFunction();
		
		// setttings
		kome::objects::SettingParameterValues* labelSettings = dlg.getPeakPickingSettings();
		kome::objects::SettingParameterValues* chargeSettings = dlg.getChargeDetectSettings();
		
		// -------------------
		// Operation 作成
		// -------------------
		PeakDetectOperation* opt = new PeakDetectOperation();
		opt->setTargetSample( acSpec->getSample() );
		if( dlg.getTarget() == mgr.TARGET_CURRENT_SPECTRUM  ){
			opt->setOperationName( "Detect Spectrum Peaks" );
		}else{
			opt->setOperationName( "Detect Spectra Peaks" );
		}
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "spec_peak_detect" );

		opt->setSpectra( targetSpectra );
		opt->setPeakDetect( peakDetector );
		opt->setLabelSettings( *labelSettings );
		opt->setChargeDetct( chargeDetector );
		opt->setChargeSettings( *chargeSettings );
		opt->setTarget( dlg.getTarget() );

		if( opt->execute() ){
			ret.prim.boolVal = true;
			const char* peak_filter = dlg.getPeakFilterParam();
			
			// Peak Filtering Function
			ret.prim.boolVal = mgr.executePeakFiltering( targetSpectra, FMT("%s",peak_filter ).c_str() );
		}else{
			delete opt;
		}
	}
		
	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// execute chromatogram labeling
kome::objects::Variant executeChromatogramLabeling( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// check peak picking plug-in
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int num = plgMgr.getNumberOfFunctionItems( CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE );
	if( num == 0 ) {	// no peak picking
		LOG_ERROR_CODE( FMT( "No chromatogram peak detection algorithms are installed.\nExit Mass++ and install at least one chromatogram peak detection plug-in." ), ERR_OTHER );
		return ret;
	}

	// chromatogram
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	if( chrom == NULL ) {
		LOG_ERROR( FMT( "A chromatogram is not active." ) );
		return ret;
	}

	// open dialog
	ChromatogramLabelingDialog dlg( kome::window::WindowTool::getMainWindow() );

	if( dlg.ShowModal() == wxID_OK ) {
		// peak manager
		PeakPickingManager& peakMgr = PeakPickingManager::getInstance();

		// function
		kome::plugin::PluginFunctionItem* item = dlg.getPeakPickingFunction();
		if( item == NULL ) {
			LOG_ERROR( FMT( "Failed to get the peak picking function." ) );
			return ret;
		}

		// settings 
		kome::objects::SettingParameterValues* labelSettings
			= dlg.getPeakPickingSettings();
				
		peakMgr.setPeakDetectFunction(*item);
		peakMgr.setPeakDetectSettings(*labelSettings);
				
		// -------------------
		// Operation 作成
		// -------------------
		PeakDetectOperation* opt = new PeakDetectOperation();
		opt->setTargetSample( chrom->getSample() );
		opt->setOperationName( "Detect Chromatogram Peaks" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "chrom_peak_detect" );
		opt->setChrom( chrom );
		opt->setPeakDetect( item );
		opt->setLabelSettings( *labelSettings );

		if( opt->execute() ){
			ret.prim.boolVal = true;
			// refresh
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}else{
	
		ret.prim.boolVal = true;
	}
	
	return ret;
}

// clear labels
kome::objects::Variant clearLabels( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// active objects
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();
	if( spec != NULL && chrom != NULL && group != NULL ) {
		LOG_ERROR( FMT( "A data object is not active." ) );
		return ret;
	}

	if (!kome::window::WindowTool::showYesNo( "Are you sure you want to clear all peaks?", "Clear all", "Don't clear" ))
	{
		ret.prim.boolVal = true;
		return ret;
	}

	// data set
	kome::objects::DataSet ds( group );
	if( spec != NULL ) {
		ds.addSpectrum( spec );
	}
	else if( chrom != NULL ) {
		ds.addChromatogram( chrom );
	}
	else {
		group->getDataSet( &ds );
	}

	// clear labels
	for( unsigned int i = 0; i < ds.getNumberOfSpectra(); i++ ) {	// each spectra
		// peaks
		kome::objects::Spectrum* spec = ds.getSpectrum( i );
		pkMgr.deletePeaks( spec );
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	}

	for( unsigned int i = 0; i < ds.getNumberOfChromatograms(); i++ ) { // each chromatograms
		// peaks
		kome::objects::Chromatogram* chrom = ds.getChromatogram( i );
		pkMgr.deletePeaks( chrom );
		kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );
	}

	// refres
	kome::window::WindowTool::refresh();

	ret.prim.boolVal = true;
	return ret;
}

// detect spectrum peaks on open
kome::objects::Variant detectSpectrumPeaksOnOpen( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// check peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if( pkMgr.hasPeaks( spec ) ) { // already labeled
		return ret;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// parameters
	kome::core::IniFile* ini = msppMgr.getParameters();
	if( ini == NULL ) {
		return ret;
	}

	// get functions
	std::string funName;
	std::string tmpfunName;

	// peak detect function
	kome::plugin::PluginFunctionItem* peakFun = NULL;
	if( spec->getMsStage() > 1 ) {
		// @date 2011.01.18 <Mod> M.Izumi ->
		// デフォルト設定の（None)を有効にするため 
		tmpfunName = ini->getString( SECTION, MSMS_SPEC_LABELING_KEY, DEFAULT_SPEC_PEAK_DETECT );
		if( strcmp(tmpfunName.c_str(), "none")==0 ){
			funName = "";
		}else{
			funName = tmpfunName;
		}
		// <-
	}
	else {
		// @date 2011.01.18 <Mod> M.Izumi ->
		// デフォルト設定の（None)を有効にするため 
		tmpfunName = ini->getString( SECTION, MS_SPEC_LABELING_KEY, DEFAULT_SPEC_PEAK_DETECT );
		if( strcmp(tmpfunName.c_str(), "none")==0 ){
			funName = "";
		}else{
			funName = tmpfunName;
		}		 
		// <
	}
	peakFun = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, funName.c_str() );

	// peak detect parameters
	kome::objects::SettingParameterValues labelSettings;

	kome::plugin::SettingsPage* peakPage = ( peakFun == NULL ) ? NULL : peakFun->getSettingsPage();
	if( peakPage != NULL ) {
		peakPage->setParameters( labelSettings );
	}

	// charge detect function
	kome::plugin::PluginFunctionItem* chargeFun = NULL;
	if( spec->getMsStage() > 1 ) {	
		// @date 2011.01.18 <Mod> M.Izumi ->
		// デフォルト設定の（None)・L効に・ｷる・ｽめ 
		tmpfunName = ini->getString( SECTION, CHARGE_DETECTOR_KEY, "simple" );
		if( strcmp( tmpfunName.c_str(), "none" ) == 0 ) {
			funName = "";
		}else{
			funName = tmpfunName;
		}
		// @date 2011.01.18 <Mod> M.Izumi <-
		chargeFun = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, funName.c_str() );
	}

	// charge detect parameters
	kome::objects::SettingParameterValues chargeSettings;

	kome::plugin::SettingsPage* chargePage = ( chargeFun == NULL ) ? NULL : chargeFun->getSettingsPage();
	if( chargePage != NULL ) {
		chargePage->setParameters( chargeSettings );
	}
	
	// Defaultの設定が"None"でない時
	if( peakFun != NULL ){
		// ---------------
		// Operation 作成
		// ---------------
		PeakDetectOperation* opt = new PeakDetectOperation();
		std::vector< kome::objects::Spectrum* > spectra;
		spectra.clear();
		spectra.push_back( spec );

		opt->setSpectra( spectra );
		opt->setTargetSample( spec->getSample() );
		opt->setOperationName( "Detect Spectrum Peaks" );
		opt->setOperationType( kome::operation::Operation::TYPE_AUTO );
		opt->setShortName( "spec_peak_detect" );

		opt->setPeakDetect( peakFun );
		opt->setLabelSettings( labelSettings );
		opt->setChargeDetct( chargeFun );
		opt->setChargeSettings( chargeSettings );
// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
// Current Spectrumを設定しておかないとダメ
		opt->setTarget( PeakPickingManager::TARGET_CURRENT_SPECTRUM );
// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki

		if( opt->execute() ){
			ret.prim.boolVal = true;
		}else{
			delete opt;
		}
	}
	
	return ret;
}

// detect chroamtogram peaks on open
kome::objects::Variant detectChromatogramPeaksOnOpen( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// check peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if( pkMgr.hasPeaks( chrom ) ) {
		return ret;
	}

	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();
	if( ini == NULL ) {
		return ret;
	}

	// peak manager
	PeakPickingManager& peakMgr = PeakPickingManager::getInstance();

	// @date 2011.01.18 <Mod> M.Izumi ->
	// デフォルト設定の（None)を有効にするため 
	// function
	std::string funName;
	std::string tmpFunName;
	tmpFunName = ini->getString( SECTION, CHROM_LABELING_KEY, "local" );
	if( strcmp(tmpFunName.c_str(), "none")==0 ){
		funName = "";
	}else{
		funName = tmpFunName;
	}
	//  @date 2011.01.18 <Mod> M.Izumi<-
	
	// get item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE, funName.c_str() );

	// settings
	kome::objects::SettingParameterValues settings;

	kome::plugin::SettingsPage* page = NULL;
	if( item == NULL ) {
		return ret;
	}
	page = item->getSettingsPage();

	if( page != NULL ) {
		page->setParameters( settings );
	}	

	peakMgr.setPeakDetectFunction(*item);
	peakMgr.setPeakDetectSettings(settings);
	
	// ---------------
	// Operation 作成
	// ---------------
	PeakDetectOperation* opt = new PeakDetectOperation();
	opt->setChrom( chrom );
	opt->setTargetSample( chrom->getSample() );
	opt->setOperationName( "Detect Chromatogram Peaks" );
	opt->setOperationType( kome::operation::Operation::TYPE_AUTO );
	opt->setShortName( "chrom_peak_detect" );

	opt->setPeakDetect( item );
	opt->setLabelSettings( settings );
	
	if( opt->execute() ){
		ret.prim.boolVal = true;
	}else{
		delete opt;
	}
	
	return ret;
}
   
// pick peaks
kome::objects::Variant pickPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeakPickingManager& mgr = PeakPickingManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// arguments
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	// peak picking algorithm
	const char* peakAlg = settings->getParameterValue( PEAK_DETECTOR_NAME );
	kome::plugin::PluginFunctionItem* peakFun = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, NVL( peakAlg, "" ) );
	if( peakFun == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector algorithm." ) );
		return ret;
	}

	kome::objects::SettingParameterValues* peakSettings = settings->getSubParameterValues( PEAK_DETECTOR_NAME, peakAlg );
	if( peakSettings->getNumberOfParameters() == 0 ) {
		kome::plugin::SettingsPage* page = peakFun->getSettingsPage();
		if( page != NULL ) {
			page->setParameters( *peakSettings );
		}
	}

	// charge detect algorithm
	const char* chargeAlg = settings->getParameterValue( CHARGE_DETECTOR_NAME );
	kome::plugin::PluginFunctionItem* chargeFun = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, NVL( chargeAlg, "" ) );
	if( chargeFun == NULL ) {
		LOG_ERROR( FMT( "Failed to get the charge detector algorithm." ) );
		return ret;
	}

	kome::objects::SettingParameterValues* chargeSettings = settings->getSubParameterValues( CHARGE_DETECTOR_NAME, chargeAlg );
	if( chargeSettings->getNumberOfParameters() == 0 ) {
		kome::plugin::SettingsPage* page = chargeFun->getSettingsPage();
		if( page != NULL ) {
			page->setParameters( *chargeSettings );
		}
	}

	// progress
	printf( "Detect Peaks...\n" );
	kome::core::DisplayProgress progress( "Detect Peaks" );

	const unsigned int smplNum = aoMgr.getNumberOfOpenedSamples();
	if( smplNum > 0 ) {
		progress.createSubProgresses( smplNum );
	}

	// samples
	for( unsigned int i = 0; i < smplNum && !progress.isStopped(); i++ ) {
		// sample
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );

		kome::core::Progress* smplProgress = progress.getSubProgress( i );

		const unsigned int groupNum = sample->getNumberOfGroups();
		if( groupNum > 0 ) {
			smplProgress->createSubProgresses( groupNum );
		}

		// data set
		kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );
		const unsigned int specNum = ds->getNumberOfSpectra();
		if( specNum > 0 ) {
			smplProgress->setRange( 0, specNum );
		}

		// spectra
		for( unsigned int k = 0; k < specNum; k++ ) {
			kome::objects::Spectrum* spec = ds->getSpectrum( k );
			// ------------------------------------------------------------
			// peaks
			// ------------------------------------------------------------
			mgr.detectPeaks(
				spec,
				NULL,
				peakFun,
				peakSettings,
				chargeFun,
				chargeSettings
			);

			smplProgress->setPosition( k + 1 );
		}
		smplProgress->fill();
	}
	progress.fill();

	ret.prim.boolVal = true;
	return ret;
}

// @date 2011.01.25 <Add> M.Izumi
kome::objects::Variant onChromatogramProcessing( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// managers
	PeakPickingManager& mgr = PeakPickingManager::getInstance();	
	kome::plugin::PluginFunctionItem item =	mgr.getPeakDetectFunction();
	kome::objects::SettingParameterValues settings =  mgr.getPeakDetectSettings();
	
	// ---------------------------------------------
	// detet peaks
	// ---------------------------------------------
	mgr.detectPeaks( chrom, NULL,  &item, &settings );

	ret.prim.boolVal = true;
	return ret;
}

// initialize peak list view
kome::objects::Variant initPeakListView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// create view
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.createPeaksView();

	return ret;
}

// change active object
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get active group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::DataGroupNode* group = obj.getGroup();

	// select
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.selectGroup( group );

	return ret;
}

// update peak view
kome::objects::Variant updatePeaksView( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get spectrum group
	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ) {
		return ret;
	}

	// update peaks view
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.updatePeakListView( group );

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// release view
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.releaseView();

	return ret;
}

// 2D labeling
kome::objects::Variant execute2dLabeling( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	Peaks2dManager& mgr = Peaks2dManager::getInstance();

	// active group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::DataGroupNode* group = obj.getGroup();
	if( group == NULL ) {
		LOG_ERROR( FMT( "A spectrum group is not active." ) );
		return ret;
	}

	// data set
	kome::objects::DataSet* dataSet = group;
	if( dataSet == NULL ) {
		LOG_ERROR( FMT( "Failed to get the active data set." ) );
		return ret;
	}

	// check labeling plug-in
	ret.prim.boolVal = true;

	if( plgMgr.getNumberOfFunctionItems( LABELING_FUNC_NAME ) == 0 ) { // no peak picking
		LOG_ERROR_CODE( FMT( "No peak detection algorithms are installed.\nExit Mass++ and install at least one peak detection plug-in." ), ERR_OTHER );
		kome::window::WindowTool::showError( getlasterrormessage() );

		return ret;
	}

	// dialog
	LabelingDialog dlg( kome::window::WindowTool::getMainWindow() );

	// labeling
	if( dlg.ShowModal() == wxID_OK ) {
		// peaks object
		kome::objects::Peaks2D* peaks = pkMgr.createPeaks( group );

		// peak detector
		kome::plugin::PluginFunctionItem* peakDetector = dlg.getPeakDetector();
		if( peakDetector == NULL && peaks->getNumberOfPeaks() == 0 ) {
			LOG_WARN( FMT( "There are no peaks. Select a peak detector." ) );
			ret.prim.boolVal = false;
			return ret;
		}

		// ---------------
		// Operation 作成
		// ---------------
		PeakDetectOperation* opt = new PeakDetectOperation();
		
		opt->setOperationName( "Detect 2D Peaks" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "2d_peak_detect" );
		opt->setTargetSample( dataSet->getSample() );
		opt->setGroup( group );
		opt->setPeakDetect( peakDetector );
		opt->setLabelSettings( *dlg.getPeakSettings() );

		if( opt->execute() ){
			ret.prim.boolVal = true;
		}else{
			delete opt;
		}
	}else{
	
		ret.prim.boolVal = true;
	}
	
	return ret;
}

// check peak list view visibility
kome::objects::Variant isVisiblePeaksView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility of the peak list view
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	ret.prim.boolVal = mgr.isVisiblePeakListView();

	return ret;
}

// Peak Detection
kome::objects::Variant peakDetection( kome::objects::Parameters* params) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );

	if( var.type == kome::objects::Variant::DATA_MAP ){
		// data map
		ret.prim.boolVal = execute2dLabeling( params ).prim.boolVal;
	}else if( var.type == kome::objects::Variant::CHROMATOGRAM ){
		// CHROMATOGRAM
		ret.prim.boolVal = executeChromatogramLabeling( params ).prim.boolVal;
	}else if( var.type == kome::objects::Variant::SPECTRUM ){
		// SPECTRUM
		ret.prim.boolVal = executeSpectrumLabeling( params ).prim.boolVal;
	}else{
		// other
	}

	return ret;
}

// save peaks file
kome::objects::Variant save2dPeaksFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// gets parameters
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		LOG_ERROR( FMT( "It failed to get the group." ) );
		return ret;
	}

	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		LOG_ERROR( FMT( "It failed to get the file path" ) );
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// peaks
	kome::objects::DataGroupNode* group = dataSet->getGroup();
	if( group == NULL ) {
		LOG_ERROR( FMT( "Failed to get the data group." ) );
		return ret;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL || peaks->getNumberOfPeaks() == 0 ) {
		LOG_ERROR_CODE( FMT( "There are no peaks to be exported." ), ERR_NULL_POINTER );
		return ret;
	}

	// save file
	ret.prim.boolVal = mgr.savePeaks( *peaks, path, *progress );

	// part of the way
	return ret;
}

// detect spectrum peaks
 kome::objects::Variant detectSpectrumPeaks( kome::objects::Parameters* params ) {
 	// return value
 	kome::objects::Variant ret;

	// check the window
	if( kome::window::WindowTool::getMainWindow() == NULL ) {
		return ret;
	}
 
 	// data set
 	kome::objects::DataSet* ds = kome::plugin::PluginCallTool::getDataSet( params );
 	if( ds == NULL ) {
 		return ret;
 	}
 	if( ds->getNumberOfSpectra() == 0 ) {
 		kome::window::WindowTool::showError( "Spectrum peak detection cannot be performed because there are no spectra.\nRaw data containing spectra should be used." );
 		return ret;
 	}
 
 	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
 	if( progress == NULL ) {
 		progress = &( kome::core::Progress::getIgnoringProgress() );
 	}
  
 	// check peak picking plug in
 	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
 	PeakPickingManager& mgr = PeakPickingManager::getInstance();
 
 	if( plgMgr.getNumberOfFunctionItems( SPECTRUM_PEAK_PICKING_FUNC_TYPE ) == 0 ) { // no peak picking
		LOG_ERROR_CODE( FMT( "No peak detection algorithms are installed.\nExit Mass++ and install at least one peak detection plug-in." ), ERR_OTHER );
 		kome::window::WindowTool::showError( getlasterrormessage() );
 		return ret;
 	}

 	// open dialog
 	SpectrumLabelingDialog dlg(	kome::window::WindowTool::getMainWindow() );
 
 	if( dlg.ShowModal() == wxID_OK ) {
 		// progress
 		progress->setRange( 0, ds->getNumberOfSpectra() );
 		progress->setPosition( 0 );
 
 		// peak picking
 		for( unsigned int i = 0; i < ds->getNumberOfSpectra() && !progress->isStopped(); i++ ) {
 			// spectrum
 			kome::objects::Spectrum* spec = ds->getSpectrum( i );
 
 			// status
 			std::string msg = FMT( "Peak Detection (%s) ...... [%d/%d]", spec->getName(), ( i + 1 ), ds->getNumberOfSpectra() );
 			progress->setStatus( msg.c_str() );
 
 			// function
 			kome::plugin::PluginFunctionItem* peakDetector = dlg.getPeakPickingFunction();
			kome::plugin::PluginFunctionItem* chargeDetector = dlg.getChargeDetectFunction();
 
 			// setttings
 			kome::objects::SettingParameterValues* labelSettings = dlg.getPeakPickingSettings();
			kome::objects::SettingParameterValues* chargeSettings = dlg.getChargeDetectSettings();
 
 			// peak
 			mgr.detectPeaks(
 				spec,
 				NULL,
 				peakDetector,
 				labelSettings, 				
 				chargeDetector,
 				chargeSettings
 			);
 
 			// position
 			progress->setPosition( i + 1 );
 		}
 	}
 
 	// refresh
 	kome::window::WindowTool::refresh();
 
 	return ret;
 }
 
 // detect chromatogram peaks
 kome::objects::Variant detectChromatogramPeaks( kome::objects::Parameters* params ) {
 	// return value
 	kome::objects::Variant ret;
 
 	// data set
 	kome::objects::DataSet* ds = kome::plugin::PluginCallTool::getDataSet( params );
 	if( ds == NULL ) {
 		return ret;
 	}
 	if( ds->getNumberOfChromatograms() == 0 ) {
 		LOG_ERROR( FMT( "There are no chromatograms." ) );
 		return ret;
 	}
 
 	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
 	if( progress == NULL ) {
 		progress = &( kome::core::Progress::getIgnoringProgress() );
 	}
 
 	// check peak picking plug in
 	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
 	unsigned int num = plgMgr.getNumberOfFunctionItems( CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE );
 	if( num == 0 ) {	// no peak picking
		LOG_ERROR_CODE( FMT( "No chromatogram peak detection algorithms are installed.\nExit Mass++ and install at least one chromatogram peak detection plug-in." ), ERR_OTHER );
 		return ret;
 	}
 
 	// open dialog
 	ChromatogramLabelingDialog dlg( kome::window::WindowTool::getMainWindow() );
 
 	if( dlg.ShowModal() == wxID_OK ) {
 		// peak manager
 		PeakPickingManager& peakMgr = PeakPickingManager::getInstance();
 
 		// function
 		kome::plugin::PluginFunctionItem* item = dlg.getPeakPickingFunction();
 		if( item == NULL ) {
 			LOG_ERROR( FMT( "Failed to get the peak picking function." ) );
 			return ret;
 		}
 
 		// settings 
 		kome::objects::SettingParameterValues* labelSettings
 			= dlg.getPeakPickingSettings();
 
 		// @date 2011.01.25 <Add> M.Izumi  >
 		peakMgr.setPeakDetectFunction(*item);
 		peakMgr.setPeakDetectSettings(*labelSettings);
 		// @date 2011.01.25 <Add> M.Izumi < 
 
 		// progress
 		progress->setRange( 0, ds->getNumberOfChromatograms() );
 		progress->setPosition( 0 );
 
 		for( unsigned int i = 0; i < ds->getNumberOfChromatograms() && !progress->isStopped(); i++ ) {
 			// chromatogram
 			kome::objects::Chromatogram* chrom = ds->getChromatogram( i );
 
 			// status
 			std::string msg = FMT( "Peak Detection (%s) ...... [%d/%d]", chrom->getName(), ( i + 1 ), ds->getNumberOfChromatograms() );
 			progress->setStatus( msg.c_str() );
 
 			// peaks
 			peakMgr.detectPeaks( ds->getChromatogram( i ), NULL, item, labelSettings );
 			progress->setPosition( i + 1 );
 		}
 		
 		// refresh
 		kome::window::WindowTool::refresh();
 	}
 
 	return ret;
 }
 
 // detect 2D peaks
 kome::objects::Variant detectGroup2DPeaks( kome::objects::Parameters* params ) {
 	// return value
 	kome::objects::Variant ret;
 
 	// manager
 	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
 	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
 	Peaks2dManager& mgr = Peaks2dManager::getInstance();
 
 	// group
 	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
 	if( group == NULL ) {
 		return ret;
 	}
 
 	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
 	if( progress == NULL ) {
 		progress = &( kome::core::Progress::getIgnoringProgress() );
 	}
 
 	// check peak detecting plug in
 	if( plgMgr.getNumberOfFunctionItems( LABELING_FUNC_NAME ) == 0 ) { // no peak picking
		LOG_ERROR_CODE( FMT( "No peak detection algorithms are installed.\nExit Mass++ and install at least one peak detection plug-in." ), ERR_OTHER );
 		kome::window::WindowTool::showError( getlasterrormessage() );
 		return ret;
 	}
 
 	// dialog
 	LabelingDialog dlg( kome::window::WindowTool::getMainWindow() );
 
 	// labeling
 	if( dlg.ShowModal() == wxID_OK ) {
 		// peaks object
 		kome::objects::Peaks2D* peaks = pkMgr.createPeaks( group );
 
 		// peak detector
 		kome::plugin::PluginFunctionItem* peakDetector = dlg.getPeakDetector();
 		if( peakDetector == NULL && peaks->getNumberOfPeaks() == 0 ) {
 			LOG_WARN( FMT( "There are no peaks. Select a peak detector." ) );
 			ret.prim.boolVal = false;
 			return ret;
 		}
 
 		// peak detect
 		mgr.detectPeaks(
 			*group,
 			*group,
 			*peaks,
 			peakDetector,
 			dlg.getPeakSettings(),
 			*progress
 		);
 	}
 
 	return ret;
 }

// draw 2D peaks
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
 
	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.drawPeaks( *dataMap, *graphics );

	return ret;
}

// draw 2D peaks
kome::objects::Variant draw2dClusters( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.drawClusters( *dataMap, *graphics );

	return ret;
}


// get spectrum peak detect operation
kome::objects::Variant getSpecPeakDetectOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new PeakDetectOperation();
	
	return ret;
}

// get chromatogram peak detect operation
kome::objects::Variant getChromPeakDetectOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new PeakDetectOperation();
	
	return ret;
}

// get 2d peak detect operation
kome::objects::Variant get2DPeakDetectOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new PeakDetectOperation();
	
	return ret;
}

// spec peak filtering height threshold
kome::objects::Variant SpecPeakFilterThreshold( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	// peaks 
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ){
		return ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	// xy data
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( params );
	if( xyData == NULL ){
		return ret;
	}

	// Threshold処理
	unsigned int inum = settings->getNumberOfParameters();
	std::string type;
	int iVal = 0;
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue(i);
		if( i == 0 ){
			type = val;
		}else{
			iVal = toint( val , 10, 0 );
		}
	}
	
	double value = 0.0;
	if( type.compare( "Relative" ) == 0 ){
		if( 0 <= iVal && iVal <= 100 ){
			double maxInt = xyData->getMaxY();
			value = (maxInt * iVal) / 100 ;
		}else{
			LOG_ERROR_CODE( FMT("Min. 0 must be less than or equal to Max. 100." ), ERR_OTHER );
			return ret;
		}

	}else{
		value = abs( (double)iVal );
	}
			
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	std::vector<kome::objects::PeakElement> addPeak;

	for( unsigned int i=0; i < peaks->getLength(); i++ ){
	// WMDなど、ピーク面積を内部的にピーク強度として扱っているものもあるので、ピーク強度(getY(nIndex))でなくピーク頂点(newPeak->getApexY())でフィルターを掛ける。
	//	if( value <= peaks->getY(i) ){               // @Date:2014/02/26	<Del>	Y.Fujita
		kome::objects::PeakElement* newPeak = NULL;
		newPeak = peaks->getPeak(i);
		if( newPeak != NULL ){
			if (newPeak->getApexY() > value ) {      // @Date:2014/02/26	<Add>	Y.Fujita
				newPeak->setLeft( newPeak->getLeftX(), newPeak->getLeftY() );
				newPeak->setRight( newPeak->getRightX(), newPeak->getRightY() );
				newPeak->setApex( newPeak->getApexX(), newPeak->getApexY() );
				newPeak->setId( newPeak->getId() );
				addPeak.push_back( *newPeak );
			}
		}
	}

	pkMgr.updatePeaks( *spec, peaks, addPeak );
	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );

	ret.prim.boolVal = true;

	return ret;
}

// spec peak filtering m/z range
kome::objects::Variant SpecPeakFilterMzRange( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = true;
	ret.type = kome::objects::Variant::BOOL;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}
	    
	// peaks 
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ){
		return ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	double start = settings->getDoubleValue( settings->getParameterName(0), -1.0 );
	double end = settings->getDoubleValue( settings->getParameterName(1), -1.0 );
	
	if( start < 0.0 && end < 0.0 ){
		return ret;
	}

	if( end > spec->getMaxX() || end < 0.0 ){
		end = spec->getMaxX();
	}

	// 0以上End未満
	if( start >= end || start < spec->getMinX() ){
		start = 0.0;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	std::vector<kome::objects::PeakElement> addPeak;
	
	for( unsigned int i=0; i < peaks->getLength(); i++ ){
		// Spec No.86451 Peaks in the specified range is removed. @date 2012.07.02 <Mod> M.Izumi
		if( start < peaks->getX(i) && peaks->getX(i) <= end ){
			kome::objects::PeakElement* newPeak = NULL;
			newPeak = peaks->getPeak(i);
			if( newPeak != NULL ){
				newPeak->setRight( newPeak->getRightX(), newPeak->getRightY() );
				newPeak->setApex( newPeak->getApexX(), newPeak->getApexY() );
				newPeak->setId( newPeak->getId() );
				addPeak.push_back( *newPeak );
			}
		}
	}

	pkMgr.updatePeaks( *spec, peaks, addPeak );
	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );

	ret.prim.boolVal = true;
	return ret;
}

// spec peak filtering elimination
kome::objects::Variant SpecPeakFilterElimination( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	// peaks 
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ){
		return ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	unsigned int inum = settings->getNumberOfParameters();
	double torerance = 0.0;
	std::vector< double > mz;
	for( unsigned int i=0; i < inum; i++ ){
		if( i == 0 ){
			torerance = settings->getDoubleValue( settings->getParameterName(i), 0.0 );
		}else{
			std::vector<std::string> s;
			kome::plugin::SettingsValue::separateListValue( settings->getParameterValue(i), s ); // stringtokenから変更 @date 2012.06.27 <Mod> M.Izumi
			for( unsigned int j=0; j < s.size(); j++ ){
				std::string tmp;
				int ifind = s[j].find_first_of( ":" );
				tmp = s[j].substr( 0, ifind );
				
				// 一番外側の[]が存在すれば削除
				std::string strDouble = kome::plugin::SettingsValue::removeOuterBracket( tmp.c_str() );
				
				mz.push_back( todouble( strDouble.c_str(), 0.0 ) );
			}
		}
	}
		
	std::vector<kome::objects::PeakElement> addPeak;
	
	bool bflg = false;
	for( unsigned int i=0; i < peaks->getLength(); i++ ){
		double px = peaks->getX(i);			
		
		for( unsigned int j=0; j < mz.size(); j++ ){
			if( !bflg ){
				double d0 = mz[j]-torerance;
				double d1 = mz[j]+torerance;
						
				if( d0 < px && px < d1 ){
					bflg = true;
					break;
				}
			}
							
		}
		if( !bflg ){    			
			kome::objects::PeakElement* newPeak = NULL;
			newPeak = peaks->getPeak(i);
			if( newPeak != NULL ){			
				newPeak->setLeft( newPeak->getLeftX(), newPeak->getLeftY() );
				newPeak->setRight( newPeak->getRightX(), newPeak->getRightY() );
				newPeak->setApex( newPeak->getApexX(), newPeak->getApexY() );
				newPeak->setId( newPeak->getId() );

				addPeak.push_back( *newPeak );
			
			}
		}
		bflg = false;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	pkMgr.updatePeaks( *spec, peaks, addPeak );

	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		
	ret.prim.boolVal = true;

	return ret;
}

// >>>>>>	@Dtae:2013/08/26	<Modify>	A.Ozaki
//
#define	PARAM_NAME_SMOOTHING_METHOD				"smoothing_method"
#define	PARAM_VALUE_MSLOWESS					"mslowess"

#define	CALL_TYPE_SN_PEAK_FILTERING				"sn_peak_filtering"

// S/N Filter
kome::objects::Variant SpecPeakFilterSn( kome::objects::Parameters* params )
{
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// spectrum
	kome::objects::Spectrum	*pSpec = kome::plugin::PluginCallTool::getSpectrum( params );
	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	ret;
	}

	// peaks 
	kome::objects::Peaks	*pPeaks = kome::plugin::PluginCallTool::getPeaks( params );
	if  ( (kome::objects::Peaks *)NULL == pPeaks)
	{
		return	ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues	*pSettings = kome::plugin::PluginCallTool::getSettingValues( params );
	if  ( (kome::objects::SettingParameterValues *)NULL == pSettings )
	{
		return	ret;
	}

	// xy data
	kome::core::XYData	*pXyData = kome::plugin::PluginCallTool::getXYData( params );
	if  ( (kome::core::XYData *)NULL == pXyData )
	{
		return	ret;
	}

	wxBeginBusyCursor( );

	// page上の値の読み込み
	//
	typedef enum {
		SMOOTHING_METHOD_UNKOWN = -1,
		SMOOTHING_METHOD_MSLOWESS,
	} eSmoothingMethod;
	eSmoothingMethod	eMethod = SMOOTHING_METHOD_UNKOWN;

	std::string	strTemp;
	const char	*pcMethod = pSettings->getParameterValue( PARAM_NAME_SMOOTHING_METHOD );
	if  ( (char *)NULL != pcMethod )
	{
		strTemp = tolowercase( pcMethod );
		if  ( 0 == strTemp.compare( PARAM_VALUE_MSLOWESS ) )
		{
			// type is Mslowess
			//
			eMethod = SMOOTHING_METHOD_MSLOWESS;

		}
	}

	switch ( eMethod )
	{
	case  SMOOTHING_METHOD_MSLOWESS:
		{
			kome::plugin::PluginManager	&plgMgr = kome::plugin::PluginManager::getInstance( );
			int	nItems = (int)plgMgr.getNumberOfFunctionItems( CALL_TYPE_SN_PEAK_FILTERING );
			for ( int i = 0 ; i < nItems ; i++ )
			{
				kome::plugin::PluginFunctionItem	*pFunction = plgMgr.getFunctionItem( CALL_TYPE_SN_PEAK_FILTERING, i );
				const char	*pName = pFunction->getLongName( );
				if  ( (char *)NULL != pName )
				{
					std::string	strName = tolowercase( pName );
					if  ( 0 == strName.compare( PARAM_VALUE_MSLOWESS ) )
					{
						ret = pFunction->getCall( )->invoke( params );
					}
				}
			}
		}
		break;
	default:
		break;
	}

	wxEndBusyCursor( );

	return ret;

}
//
// <<<<<<	@Date:2013/08/26	<Modify>	A.Ozaki

// spec peak filter difference
kome::objects::Variant SpecPeakFilterDifference( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;
	
	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	// peaks 
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ){
		return ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	std::vector<kome::objects::PeakElement> addPeak;

	// get difference	
	double tmpDiff = settings->getDoubleValue( "Difference", 0.0 );
	double tmpTol = settings->getDoubleValue( "diffTol", 0.0 );
	
	bool bflg = false;
	// +
	if( tmpDiff > 0.0 ){
		bflg = true;
	// -
	}else{
		tmpDiff = -( tmpDiff );
	}

	double start = tmpDiff - tmpTol;
	double end = tmpDiff + tmpTol;
	
	std::vector<int> element;
	// filter
	for( unsigned int j=0; j < peaks->getLength(); j++ ){
		double stdpx = peaks->getX(j);

		for( unsigned int k=j+1; k < peaks->getLength(); k++ ){
			double px = peaks->getX(k);
			double calc = px - stdpx;
			
			if( start < calc && calc < end ){
		
				// 正の数
				if( bflg ){
					element.push_back( j );					

				// 負の数
				}else{
					element.push_back( k );
				}
			}
		}
	}
		
	for( unsigned int i=0; i < peaks->getLength(); i++ ){
		bool bflg = false;

		std::vector< int >::iterator it = std::find( element.begin(),element.end() , i );

		if( it != element.end() ){
			bflg = true;
		}
			
		if( !bflg  ){
			kome::objects::PeakElement* newPeak = NULL;
			newPeak = peaks->getPeak(i);

			if( newPeak != NULL ){
				newPeak->setLeft( newPeak->getLeftX(), newPeak->getLeftY() );
				newPeak->setRight( newPeak->getRightX(), newPeak->getRightY() );
				newPeak->setApex( newPeak->getApexX(), newPeak->getApexY() );
				newPeak->setId( newPeak->getId() );
				addPeak.push_back( *newPeak );
			}
		}
	}
	
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	pkMgr.updatePeaks( *spec, peaks, addPeak );

	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );


	ret.prim.boolVal = true;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
// normalization spectra peak detect 
kome::objects::Variant normalizPeakDetectSpectra( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// standard sample
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataSet* ds = kome::plugin::PluginCallTool::getDataSet( params );
			
	kome::objects::SettingParameterValues* val = kome::plugin::PluginCallTool::getSettingValues( params );
	if( val == NULL ){
		return ret;
	}
	
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	PeakPickingManager& mgr = PeakPickingManager::getInstance();
	
	// function
	kome::plugin::PluginFunctionItem* peakDetect = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, val->getParameterValue(val->getParameterName(0)) );
	kome::plugin::PluginFunctionItem* chargeDetect = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, val->getParameterValue(val->getParameterName(1)) );

	// @date 2013/09/18 <Add> OKADA -------> 
	bool flgCmd=false;
	if( chargeDetect == NULL ){
		chargeDetect = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, 
			val->getParameterValue( "chargeDetector" )  );
		flgCmd = true;
	}
	// @date 2013/09/18 <Add> OKADA <-------

	// setttings
	kome::objects::SettingParameterValues* peakDetectSettings = val->getSubParameterValues( val->getParameterName(0), val->getParameterValue(val->getParameterName(0)) );
	kome::objects::SettingParameterValues* chargeDetectSettings = val->getSubParameterValues( val->getParameterName(1), val->getParameterValue(val->getParameterName(1)) );
		
	// @date 2013/09/18 <Add> OKADA -------> 
	if(flgCmd){
		chargeDetectSettings = val->getSubParameterValues( "chargeDetector", val->getParameterValue( "chargeDetector" ) );
	}
	// @date 2013/09/18 <Add> OKADA <-------

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ){
		progress = &kome::core::Progress::getIgnoringProgress();
	}
 	 
 	if( plgMgr.getNumberOfFunctionItems( SPECTRUM_PEAK_PICKING_FUNC_TYPE ) == 0 ) { // no peak picking
 		LOG_ERROR_CODE( FMT( "There is no peak detection plug in." ), ERR_OTHER );
 		kome::window::WindowTool::showError( getlasterrormessage() );
 		return ret;
 	}

	kome::objects::DataSet dataSet;
	// standard spectra
	stdGroup->getDataSet( &dataSet );

	// @date 2013/08/08 <Add> OKADA ------->
	// progress
	int nRangeMaxDs = 0;
	int nRangeMaxTotal = 0;
	if( ds != NULL ){
		nRangeMaxDs = ds->getNumberOfSpectra();
	}
	nRangeMaxTotal = nRangeMaxDs + dataSet.getNumberOfSpectra();

 	progress->setRange( 0, nRangeMaxTotal );
 	progress->setPosition( 0 );
	// @date 2013/08/08 <Add> OKADA <-------

	if( ds != NULL ){
		// spectra
		for( unsigned int i=0; i < ds->getNumberOfSpectra(); i++ ){

			// status
 			std::string msg = FMT( "set Spectra ...... [%d/%d]", ( i + 1 ), ds->getNumberOfSpectra() );
 			progress->setStatus( msg.c_str() );

			dataSet.addSpectrum( ds->getSpectrum(i) );

		}
	}
	// ↑のループ内で、dataSet.getNumberOfSpectra()の数が増える可能性あり	// @date 2013/08/08 <Comment> OKADA

	nRangeMaxTotal = nRangeMaxDs + dataSet.getNumberOfSpectra();	// @date 2013/08/08 <Add> OKADA
 	progress->setRange( 0, nRangeMaxTotal );						// @date 2013/08/08 <Add> OKADA
	progress->setPosition( nRangeMaxDs );							// @date 2013/08/08 <Add> OKADA

	// peak picking
 	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra() && !progress->isStopped(); i++ ) {
		// spectrum
 		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
 
 		// status
 		std::string msg = FMT( "Peak Detection (%s) ...... [%d/%d]", spec->getName(), ( i + 1 ), dataSet.getNumberOfSpectra() );
 		progress->setStatus( msg.c_str() );
 
 		// peak
 		mgr.detectPeaks(
 			spec,
 			NULL,
 			peakDetect,
 			peakDetectSettings, 				
 			chargeDetect,
 			chargeDetectSettings
 		);
 
 		// position
 		progress->setPosition( i + nRangeMaxDs );	// @date 2013/08/08 <Add> OKADA
 	}
			
	if( !progress->isStopped() ){
		progress->fill();	// @date 2013/08/08 <Add> OKADA
		ret.prim.boolVal = true;
	}
	
	return ret;
}

// normalization 2d peak detect 
kome::objects::Variant normalizPeakDetect2d( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	std::vector< kome::objects::DataGroupNode* > groups;
	kome::objects::DataGroupNode* g = kome::plugin::PluginCallTool::getControlGroup( params );
	groups.push_back( g );
	
	g = kome::plugin::PluginCallTool::getGroup( params );
	if( g != NULL ){
		groups.push_back( g );
	}
		
	kome::objects::SettingParameterValues* val = kome::plugin::PluginCallTool::getSettingValues( params );
	if( val == NULL ){
		return ret;
	}
	// manager
 	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
 	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
 	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	
	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ){
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	// check peak detecting plug in
 	if( plgMgr.getNumberOfFunctionItems( LABELING_FUNC_NAME ) == 0 ) { // no peak picking
 		LOG_WARN_CODE( FMT( "There is no peak detection plug in." ), ERR_OTHER );
 		kome::window::WindowTool::showError( getlasterrormessage() );
 		return ret;
 	}
 
	for( unsigned i=0; i < groups.size() && !progress->isStopped(); i++ ){
		kome::objects::DataGroupNode* group = groups[i];
		if( group != NULL ){
			// peaks object
 			kome::objects::Peaks2D* peaks = pkMgr.createPeaks( group );
	
			// peak detector
			kome::plugin::PluginFunctionItem* peakDetector = plgMgr.getFunctionItem( LABELING_FUNC_NAME, val->getParameterValue(val->getParameterName(0)) );
 			if( peakDetector == NULL && peaks->getNumberOfPeaks() == 0 ) {
 				LOG_WARN_CODE( FMT( "There are no peaks. Select a peak detector." ), ERR_OTHER );
 				ret.prim.boolVal = false;
 				return ret;
 			}
  
			// setttings
			kome::objects::SettingParameterValues* peakDetectSettings = val->getSubParameterValues( val->getParameterName(0), val->getParameterValue(val->getParameterName(0)) );
			kome::objects::SettingParameterValues* chargeDetectSettings = val->getSubParameterValues( val->getParameterName(1), val->getParameterValue(val->getParameterName(1)) );
		
 			// peak detect
 			mgr.detectPeaks(
 				*group,
 				*group,
 				*peaks,
 				peakDetector,
 				peakDetectSettings,
 				*progress
 			);
		}
	}

	if( !progress->isStopped() ){
		ret.prim.boolVal = true;
	}
	return ret;
}
