/**
 * @file IdentificationPluginManager.cpp
 * @brief implementsation of IdentificationPluginManager class
 *
 * @author S.Tanaka
 * @date 2013.07.26
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "IdentificationPluginManager.h"

#include "SpectrumGetter.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IdentificationPluginManager::IdentificationPluginManager() {
}

// destructor
IdentificationPluginManager::~IdentificationPluginManager() {
}

// identify
int IdentificationPluginManager::identify(
		const char* title,
		const char* comment,
		kome::plugin::PluginFunctionItem* searchEngine,
		kome::objects::SettingParameterValues* searchSettings,
		kome::plugin::PluginFunctionItem* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		kome::plugin::PluginFunctionItem* chargeCalculator,
		kome::objects::SettingParameterValues* chargeSettings,
		SpectrumGetter* specGetter,
		const char* peakFilter,
		kome::core::Progress* progress,
		std::vector< kome::plugin::PluginFunctionItem* > items,
		std::vector< kome::objects::SettingParameterValues > settings	// @date 2013.10.29 <Add> M.Izumi
) {
	// return value
	int searchId = -1;

	// check required objects
	kome::ident::SearchEngine* engine = NULL;
	if( searchEngine != NULL ) {
		engine = (kome::ident::SearchEngine*)searchEngine->getCall()->invoke( NULL ).prim.pt;
	}
	if( engine == NULL ) {
		LOG_ERROR( FMT( "Failed to get the search engine." ) );
		return searchId;
	}

	if( peakDetector == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector." ) );
		return searchId;
	}

	if( chargeCalculator == NULL ) {
		LOG_ERROR( FMT( "Failed to get the charge calculator." ) );
		return searchId;
	}

	if( specGetter == NULL ) {
		LOG_ERROR( FMT( "Failed to get the spectrum getter." ) );
		return searchId;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	identMgr.commit();

	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}
	progress->createSubProgresses( 2 );

	// peak detection
	progress->setStatus( "Detecting Peaks..." );

	// >>>>>>	@Date:2013/09/19	<Modify>	A.Ozaki
	// fixed memory leaks
	//
	std::vector< kome::ident::SpecInfo* > spectra;
	spectra.clear( );

	while ( 1 )
	{
		kome::core::Progress* prgs = progress->getSubProgress( 0 );

		if  ( !detectPeaks( *specGetter,
							engine,
							searchSettings,
							peakDetector,
							peakSettings,
							chargeCalculator,
							chargeSettings,
							peakFilter,
							*prgs,
							spectra,
							items,
							settings ) )
		{
			break;
		}

		if( prgs->isStopped() )
		{
			break;
		}
		prgs->fill( );

		// search
		progress->setStatus( "Searching..." );
		prgs = progress->getSubProgress( 1 );

		searchId = engine->search( title,
								comment,
								( spectra.size( ) == 0 ? NULL : &( spectra[ 0 ] ) ),
								spectra.size( ),
								searchSettings,
								prgs );
		identMgr.commit();
		
		if( prgs->isStopped() )
		{
			break;
		}
		prgs->fill( );

		break;
	}
	progress->fill( );

	kome::ident::SpecInfo	*pSpecInfo = (kome::ident::SpecInfo *)NULL;

	while ( false == spectra.empty( ) )
	{
		pSpecInfo = spectra.back( );
		spectra.pop_back( );
		if  ( (SpecInfo *)NULL != pSpecInfo )
		{
			delete	pSpecInfo;
			pSpecInfo = (SpecInfo *)NULL;
		}
	}

	// commit
	identMgr.commit();

	return	searchId;

	//
	// <<<<<<	@Date:2013/09/19	<Modified>	A.Ozaki
}

// detect peaks
bool IdentificationPluginManager::detectPeaks(
		SpectrumGetter& specGetter,
		kome::ident::SearchEngine* engine,
		kome::objects::SettingParameterValues* searchSettings,
		kome::plugin::PluginFunctionItem* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		kome::plugin::PluginFunctionItem* chargeCalculator,
		kome::objects::SettingParameterValues* chargeSettings,
		const char* peakFilter,
		kome::core::Progress& progress,
		std::vector< kome::ident::SpecInfo* >& spectra,
		std::vector< kome::plugin::PluginFunctionItem* > items,
		std::vector< kome::objects::SettingParameterValues > settings	// @date 2013.10.29 <Add> M.Izumi
) {
	// check parameters
	if( engine == NULL || peakDetector == NULL || chargeCalculator == NULL ) {
		return false;
	}

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// preparation
	int peakDetection = engine->preparePeakDetection(
		searchSettings,
		peakDetector->getShortName(),
		peakSettings,
		chargeCalculator->getShortName(),
		chargeSettings,
		peakFilter
	);

	if( peakDetection < 0 ) {
		LOG_ERROR( FMT( "Preparation for peak detection is failed." ) );
		return false;
	}
		
	// peak detection
	while( specGetter.next( progress ) && !progress.isStopped() ) {
		// spectrum
		kome::objects::Spectrum* spec = specGetter.getSpectrum();
				
		kome::core::DataPoints dps;
	
		// @date 2013.10.29 <Add> M.Izumi
		std::vector< kome::objects::XYDataOperation* > operations;
		kome::objects::XYDataOperation* opt = NULL;
		for( unsigned i=0; i < items.size(); i++ ){
			kome::plugin::PluginFunctionItem* item = items[ i ];
			
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setSettingValues( params, settings[i] );
			
			if( item != NULL ){
				
				kome::plugin::PluginCallTool::setSpectrum( params, *spec );

				opt = (kome::objects::XYDataOperation*)item->getCall()->invoke( &params ).prim.pt;
				if( opt != NULL ){
					spec->getXYData( &dps, -1.0, -1.0, true );
					opt->update( dps, dps, *spec ); // xyDataOperation update
										
					operations.push_back( opt );
				}
			}
		}
				
		// peak detection
		//kome::core::DataPoints dps;
		kome::objects::Peaks peaks;
		if( items.empty() ){
			spec->getXYData( &dps, -1.0, -1.0, true );
		}
	
		peaks.setPrecursor( spec->getPrecursor() );
		peaks.setDataPoints( dps );

		kome::objects::Parameters peakParams;
		kome::plugin::PluginCallTool::setSpectrum( peakParams, *spec );
		kome::plugin::PluginCallTool::setXYData( peakParams, dps );
		kome::plugin::PluginCallTool::setPeaks( peakParams, peaks );
		if( peakSettings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( peakParams, *peakSettings );
		}

		peakDetector->getCall()->invoke( &peakParams );

		// charge
		kome::objects::Parameters chargeParams;
		kome::plugin::PluginCallTool::setSpectrum( chargeParams, *spec );
		kome::plugin::PluginCallTool::setXYData( chargeParams, dps );
		kome::plugin::PluginCallTool::setPeaks( chargeParams, peaks );
		if( chargeSettings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( chargeParams, *chargeSettings );
		}

		chargeCalculator->getCall()->invoke( &chargeParams );

		// filter
		std::string pkFilter = NVL( peakFilter, "" );
		if( !pkFilter.empty() ) {
			kome::objects::Peaks* pks = &peaks;
			kome::plugin::PluginCallTool::filterSpecPeaks( &spec, &pks, 1, pkFilter.c_str() );
		}

		// Spectrum
		kome::ident::SpecInfo* info = new kome::ident::SpecInfo();
		identMgr.getSpecInfo( *spec, *info );

		for( unsigned int i = 0; i < peaks.getNumberOfCharges(); i++ ) {
			info->charges.push_back( peaks.getCharge( i ) );
		}

		const unsigned int peakNum = specGetter.getNumberOfPeakIds();
		for( unsigned int i = 0; i < peakNum; i++ ) {
			info->peakIds.push_back( specGetter.getPeakId( i ) );
		}

		info->peaks = identMgr.setPeaks( &peaks, peakDetection );

		spectra.push_back( info );

		// peaks
		engine->onSpectrumPeaks(
			spec,
			info,
			&peaks,
			( info->peakIds.size() == 0 ? NULL : &( info->peakIds[ 0 ] ) ),
			info->peakIds.size()
		);
		
		// @date 2013.10.29 <Add> M.Izumi
		for( unsigned int i=0; i < operations.size(); i++ ){
			delete operations[i];
		}
		operations.clear();
	
	}
	progress.fill();

	// finish
	if( !engine->onFinishPeakDetection() ) {
		LOG_ERROR( FMT( "Post event of peak detection occurs an error." ) );
		return false;
	}
	
	return true;
}

// get instance
IdentificationPluginManager& IdentificationPluginManager::getInstance() {
	// create object (This is the only object.)
	static IdentificationPluginManager mgr;

	return mgr;
}

