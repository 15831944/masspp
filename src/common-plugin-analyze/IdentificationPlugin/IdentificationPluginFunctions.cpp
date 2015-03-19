/**
 * @file IdentificationPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author H. Parry
 * @date 2012.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumGetter.h"
#include "IdentificationPluginFunctions.h"   
#include "IdentificationDialogBase.h"
#include "ComparativeIdentificationSpectrumGetter.h"
#include "IdentificationDialog.h"
#include "SearchResultListCtrl.h"
#include "IdentificationPanel.h"
#include "SearchResultDialog.h"
#include "SubstanceDialog.h"
#include "SubstanceListCtrl.h"
#include "IdentificationPluginManager.h"
#include "MgfSampleSet.h"
#include "IdentificationPluginManager.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

             
using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define COMMENT_PARAM_NAME						"com"

#define MGF_SECTION								"Mascot Generic Format"
#define RT_TOL_KEY								"RT_TOLERANCE"
#define MZ_TOL_KEY								"MZ_TOLERANCE"

#define IDENTIFICATION_TYPE						"identification"

#define PEAK_DETECTOR_PARAM_NAME				"peakdetector"
#define CHARGE_DETECTOR_PARAM_NAME				"chargedetector"
#define TITLE_PARAM_NAME						"title"
#define SEARCH_COMMENT_PARAM_NAME				"comment"
#define SEARCHENGINE_PARAM_NAME                 "searchengine"

#define SPECTRUM_PEAK_PICKING_FUNC_TYPE			"SPEC_PEAK_PICKING"
#define CHARGE_DETECT_FUNC_TYPE					"CHARGE_DETECT"
#define IDENTIFICATION_FUNC_TYPE				"IDENTIFICATION"


// executes identification for batch processing
kome::objects::Variant identificationOnBatch( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::objects::ActiveObjectsManager& aoMgr
		= kome::objects::ActiveObjectsManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// sample
	kome::objects::Sample* sample = aoMgr.getActiveSample();

	// parameters object
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get arguments information." ), ERR_NULL_POINTER );
		return ret;
	}

	// title
	const char* title = settings->getParameterValue( TITLE_PARAM_NAME );
	if( title == NULL )
	{
		LOG_ERROR_CODE( FMT( "Failed to get the title parameter." ), ERR_OTHER );
	}
	if (strlen(title) == 0)
	{
		LOG_ERROR_CODE( FMT( "Title must be specified." ), ERR_OTHER );
		return ret;
	}

	// comment
	const char* comment = settings->getParameterValue( SEARCH_COMMENT_PARAM_NAME );

	// search engine
	const char* searchEngineName = settings->getParameterValue( SEARCHENGINE_PARAM_NAME );
	if( searchEngineName == NULL )
	{
		LOG_ERROR_CODE( FMT( "Failed to get the search engine parameter." ), ERR_OTHER );
	}

	kome::plugin::PluginFunctionItem* searchFun = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, NVL( searchEngineName, "" ) );
	if ( searchFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid search engine specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* searchSettings = settings->getSubParameterValues(
			SEARCHENGINE_PARAM_NAME, searchFun->getShortName());
	kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)searchFun->getCall()->invoke( NULL ).prim.pt;

	// peak detector
	const char* specPeakDetector = settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	kome::plugin::PluginFunctionItem* peakFun = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, NVL( specPeakDetector, "" ) );
	if ( peakFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid peak detection algorithm specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* peakSettings = settings->getSubParameterValues(
		PEAK_DETECTOR_PARAM_NAME,
		peakFun->getShortName()
	);

	// charge detector
	const char* chargeDetector = settings->getParameterValue( CHARGE_DETECTOR_PARAM_NAME );
	kome::plugin::PluginFunctionItem* chargeFun = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, NVL( chargeDetector, "" ) );

	if ( chargeFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid charge detection algorithm specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* chargeSettings = settings->getSubParameterValues(
			CHARGE_DETECTOR_PARAM_NAME,
			chargeFun->getShortName()
	);

	// spectra
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( sample );
	std::string sampleName;
	IdentificationSpectrumGetter* spectra = new IdentificationSpectrumGetter( sample );
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
		spectra->addSpectrum( spec );
		if( i == 0 ) {
			sampleName = spec->getSample()->getName();
		}
		else if( !sampleName.empty() && sampleName.compare( spec->getSample()->getName() ) != 0 ) {
			sampleName = "";
		}
	}

	std::string titleStr = title;
	if( !sampleName.empty() ) {
		titleStr.append( " - " );
		titleStr.append( sampleName );
	}

	// preparation
	if( !engine->prepareSearch( kome::core::Progress::getIgnoringProgress() ) ) {
		return ret;
	}

	// setting
	std::string msg = engine->validateSettings( searchSettings );
	if( !msg.empty() ) {
		LOG_ERROR_CODE( msg, ERR_OTHER );
		return ret;
	}

	// identification
	kome::core::DisplayProgress* prgs = new kome::core::DisplayProgress( "Identification" );

	// @date 2013.10.29 <Add> M.Izumi
	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settingsVal;

	int searchId = IdentificationPluginManager::identify(
		titleStr.c_str(),
		comment,
		searchFun,
		searchSettings,
		peakFun,
		peakSettings,
		chargeFun,
		chargeSettings,
		spectra,
		"",
		prgs,
		items,
		settingsVal
	);
	identMgr.commit();

	// register
	kome::ident::SearchEngine* searchEngine = (kome::ident::SearchEngine*)searchFun->getCall()->invoke( NULL ).prim.pt;
	if( engine == NULL ) {
		return ret;
	}

	prgs = new kome::core::DisplayProgress( "Registering" );
	prgs->createSubProgresses( 2 );
	kome::ident::SearchInfo searchInfo;
	identMgr.getSearch( searchId, searchInfo );

	kome::ident::SearchResult result;

	searchEngine->getResult( result, searchInfo.searchId.c_str(), *( prgs->getSubProgress( 0 ) ) );
	kome::ident::SearchEngine::SearchStatus status = searchEngine->getSearchStatus( searchInfo.searchId.c_str() );
	identMgr.registerResult( searchId, result, *( prgs->getSubProgress( 1 ) ) );
	identMgr.setSearchStatus( searchId, status );
	identMgr.commit();

	ret.prim.boolVal = ( searchId >= 0 );

	return ret;
}

// open mgf file
kome::objects::Variant openMgf( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::SAMPLE_SET;
	ret.prim.pt = NULL;
	
	// get path
	const char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// open
	MgfSampleSet* sampleSet = new MgfSampleSet();

	// set file
	ret.prim.pt = sampleSet;

	return ret;
}

// save mgf
kome::objects::Variant saveMisMgf( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	MgfManager& mgfMgr = MgfManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get path
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// get data set
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	// get progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// peak detection
	if( kome::window::WindowTool::getMainWindow() != NULL ) {
		kome::objects::DataSet spectra;

		for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
			kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
			if( !pkMgr.hasPeaks( spec ) ) {
				spectra.addSpectrum( spec );
			}
		}

		if( spectra.getNumberOfSpectra() > 0 ) {
			progress->createSubProgresses( 2 );

			kome::plugin::PluginCallTool::detectSpecPeaks( spectra, progress->getSubProgress( 0 ) );
			progress = progress->getSubProgress( 1 );
		}
	}
	if( progress->isStopped() ) {
		ret.prim.boolVal = true;
		return ret;
	}

	// write mgf
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return ret;
	}

	mgfMgr.writeMisHeader( fp );
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra() && !progress->isStopped(); i++ ) {
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
		kome::ident::SpecInfo specInfo;
		identMgr.getSpecInfo( *spec, specInfo );

		kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
		mgfMgr.writeMisPeaks( fp, &specInfo, peaks );
	}

	fflush( fp );
	fclose( fp );

	ret.prim.boolVal = true;
	return ret;
}

// save pmf
kome::objects::Variant savePmfText( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get path
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// get data set
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	// @date 2013/03/21 <Add> OKADA	// SPEC 90980
	if( 0 < dataSet->getNumberOfSpectra() ){
		;
	}else{
		return savePmfTextChrom( params );
	}

	// get progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	kome::core::Progress* prgs = progress;

	// peak detection
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataSet spectra;
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
		if( !pkMgr.hasPeaks( spec ) ) {
			spectra.addSpectrum( spec );
		}
	}

	if( spectra.getNumberOfSpectra() > 0 ) {
		progress->createSubProgresses( 2 );
		kome::core::Progress* prgs = progress->getSubProgress( 0 );

		kome::plugin::PluginCallTool::detectSpecPeaks( spectra, prgs );

		if( !progress->isStopped() ) {
			prgs->fill();
		}
		prgs = progress->getSubProgress( 1 );
	}
	if( progress->isStopped() ) {
		ret.prim.boolVal = true;
		return ret;
	}

	// peaks
	std::vector< std::pair< kome::objects::Spectrum*, kome::objects::Peaks* > > peakList;
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
		kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
		if( pkMgr.hasPeaks( spec ) ) {
			kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
			if( peaks->getLength() > 0 ) {
				peakList.push_back( std::make_pair( spec, peaks ) );
			}
		}
	}

	if( peakList.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "There are no peaks to be exported." ), ERR_OTHER );
		return ret;
	}

	// open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return ret;
	}

	prgs->setRange( 0, peakList.size() );
	prgs->setPosition( 0 );
	for( unsigned int i = 0; i < peakList.size() && !progress->isStopped(); i++ ) {
		kome::objects::Spectrum* spec = peakList[ i ].first;
		kome::objects::Peaks* peaks = peakList[ i ].second;

		if( i > 0 ) {
			fprintf( fp, "\n" );
		}

		fprintf( fp, "# %s\n", spec->getName() );

		for( unsigned int j = 0; j < peaks->getLength(); j++ ) {
			fprintf( fp, "%f\t%f\n", peaks->getX( j ), peaks->getY( j ) );
		}		
		prgs->setPosition( i + 1 );
	}

	fflush( fp );
	fclose( fp );

	if( !progress->isStopped() ) {
		progress->fill();
	}

	ret.prim.boolVal = true;
	return ret;
}

// save pmf for Chromatgram
kome::objects::Variant savePmfTextChrom( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get path
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// get data set
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	// get progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	kome::core::Progress* prgs = progress;

	// peak detection
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataSet chromatograms;
	for( unsigned int i = 0; i < dataSet->getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = dataSet->getChromatogram( i );
		if( !pkMgr.hasPeaks( chrom ) ) {
			chromatograms.addChromatogram( chrom );
		}
	}

	if( chromatograms.getNumberOfChromatograms() > 0 ) {
		progress->createSubProgresses( 2 );
		kome::core::Progress* prgs = progress->getSubProgress( 0 );

		kome::plugin::PluginCallTool::detectChromPeaks( chromatograms, prgs );

		if( !progress->isStopped() ) {
			prgs->fill();
		}
		prgs = progress->getSubProgress( 1 );
	}
	if( progress->isStopped() ) {
		ret.prim.boolVal = true;
		return ret;
	}

	// peaks
	std::vector< std::pair< kome::objects::Chromatogram*, kome::objects::Peaks* > > peakList;

	for( unsigned int i = 0; i < dataSet->getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = dataSet->getChromatogram( i );
		if( pkMgr.hasPeaks( chrom ) ) {
			kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
			if( peaks->getLength() > 0 ) {
				peakList.push_back( std::make_pair( chrom, peaks ) );
			}
		}
	}

	if( peakList.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "There are no peaks to be exported." ), ERR_OTHER );
		return ret;
	}

	// open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return ret;
	}

	prgs->setRange( 0, peakList.size() );
	prgs->setPosition( 0 );
	for( unsigned int i = 0; i < peakList.size() && !progress->isStopped(); i++ ) {
		kome::objects::Chromatogram* chrom = peakList[ i ].first;
		kome::objects::Peaks* peaks = peakList[ i ].second;

		if( i > 0 ) {
			fprintf( fp, "\n" );
		}

		fprintf( fp, "# %s\n", chrom->getName() );

		for( unsigned int j = 0; j < peaks->getLength(); j++ ) {
			fprintf( fp, "%f\t%f\n", peaks->getX( j ), peaks->getY( j ) );
		}		
		prgs->setPosition( i + 1 );
	}

	fflush( fp );
	fclose( fp );

	if( !progress->isStopped() ) {
		progress->fill();
	}

	ret.prim.boolVal = true;
	return ret;
}

// toggle the visibility of the search result window
kome::objects::Variant toggleSearchResultWindow( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle the visibility
	bool visible = ( SearchResultListCtrl::m_dlg != NULL && SearchResultListCtrl::m_dlg->IsShown() );

	// delete
	if( SearchResultListCtrl::m_dlg != NULL ) {
		delete SearchResultListCtrl::m_dlg;
		SearchResultListCtrl::m_dlg = NULL;
	}

	// create
	if( !visible ) {
		kome::ident::SearchResultDialog* dlg = new kome::ident::SearchResultDialog(
			kome::window::WindowTool::getMainWindow(),
			"Search Engine Results",
			true
		);
		
		SearchResultListCtrl* listCtrl = new SearchResultListCtrl( dlg );
		if( dlg != NULL ){
			dlg->setGridListCtrl( listCtrl );
			dlg->clearStatus();

			dlg->Show();
			SearchResultListCtrl::m_dlg = dlg;
		}
	}

	return ret;
}

// check the visibility of the search result window
kome::objects::Variant isVisibleSearchResult( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	ret.prim.boolVal = ( SearchResultListCtrl::m_dlg != NULL && SearchResultListCtrl::m_dlg->IsShown() );	// @date 2012/09/06 <Add> OKADA
	// ↑m_dlgがnullの場合、:m_dlg->IsShown()は評価されません。&&演算子は左->右の順に評価されます // @date 2012/09/06 <Comment> OKADA

	return ret;
}

// open the Identification dialog
kome::objects::Variant openIdentificationDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// delete
	if( SearchResultListCtrl::m_dlg != NULL ) {
		delete SearchResultListCtrl::m_dlg;
		SearchResultListCtrl::m_dlg = NULL;
	}

	// active sample
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();

	// preparation
	if( !IdentificationDialogBase::prepare() ) {
		return ret;
	}

	// dialog
	IdentificationDialog dlg( kome::window::WindowTool::getMainWindow(), sample );
	if( dlg.ShowModal() == wxID_OK ) {
		ret.prim.boolVal = IdentificationDialogBase::identify( &dlg, sample );
	}

	return ret;
}         

// enumeration: gets the database list
kome::objects::Variant getDatabaseList( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the enumeration item for the database list" ), ERR_OTHER );
		return ret;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// enumerations
	const unsigned int num = identMgr.getNumberOfLocalDbs();
	if( num > 0 ) {
		enumeration->resize( num );

		for( unsigned int i = 0; i < num; i++ ) {
			( *enumeration )[ i ].name = identMgr.getLocalDb( i );
			( *enumeration )[ i ].value = identMgr.getLocalDb( i );
			( *enumeration )[ i ].desc = identMgr.getLocalDbDesc( i );
		}
	}
	else {
		LOG_ERROR_CODE( FMT( "The database list is empty." ), ERR_OTHER );
	}

	return ret;
}

// enumeration: gets the db file list
kome::objects::Variant getDbFileList( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR( FMT( "Failed to get the enumeration item for the db file list" ) );
		return ret;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// enumerations
	for( int i = 0; i < identMgr.getNumberOfLocalDbs(); i++ ) {
		std::string db = identMgr.getLocalDb( i );

		for( int j = 0; j < identMgr.getNumberOfLocalDbFiles( i ); j++ ) {
			std::string dbFile = identMgr.getLocalDbFileName( i, j );

			enumeration->resize( enumeration->size() + 1 );
			enumeration->back().section = db;
			enumeration->back().name = dbFile;
			enumeration->back().value = dbFile;
		}
	}

	return ret;
}

// enumeration: gets the enzyme list
kome::objects::Variant getEnzymeList( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the enumeration item for the enzymes list" ), ERR_OTHER );
		return ret;
	}

	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	int nEnzymeCount = identMgr.getNumberOfEnzymes();

	for( int i = 0; i < nEnzymeCount; i++ ) {
		kome::ident::Enzyme* enz = identMgr.getEnzyme( i );

		if( !enz->isHidden() ) {
			enumeration->resize( enumeration->size() + 1 );
			enumeration->back().name = enz->getName();
			enumeration->back().value = enz->getName();
		}
	}

	if( enumeration->size() == 0 ) {
		LOG_ERROR_CODE( FMT( "The enzyme list is empty." ), ERR_OTHER );
	}

	return ret;
}

// enumeration: gets the modification list
kome::objects::Variant getModificationList( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the enumeration item for the modifications list" ), ERR_OTHER );
		return ret;
	}

	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	int num = identMgr.getNumberOfModifications();

	if( num > 0 ) {
		for( int i = 0; i < num; i++ ) {
			kome::ident::Modification* modification = identMgr.getModification( i );

			enumeration->resize( enumeration->size() + 1 );
			enumeration->back().section = "true";
			enumeration->back().name = modification->getName();
			enumeration->back().value = modification->getName();

			if( !modification->isHidden() ) {
				enumeration->resize( enumeration->size() + 1 );
				enumeration->back().section = "false";
				enumeration->back().name = modification->getName();
				enumeration->back().value = modification->getName();
			}
		}
	}
	else
	{
		LOG_ERROR_CODE( FMT( "The modification list is empty." ), ERR_OTHER );
	}

	return ret;
}

// prepare database thread
void	prepareDatabaseThread( void )
{
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// search engines
	const unsigned int num = plgMgr.getNumberOfFunctionItems( IDENTIFICATION_FUNC_TYPE );
	if  ( 0 != num )
	{
		// dialog progress
		kome::core::Progress *pProgress = &kome::core::Progress::getIgnoringProgress();

		bool flg = true;
		for ( unsigned int i = 0 ; i < num; i++ )
		{
			kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, i );

			if  ( item != NULL )
			{
				kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)( item->getCall()->invoke( NULL ).prim.pt );
				flg = flg && engine->prepareSearch( *pProgress );
			}
		}

		if  ( !flg )
		{
			kome::window::WindowTool::showError( getlasterrormessage() );
		}

		// 成功
		kome::ident::IdentificationManager&	insManager = kome::ident::IdentificationManager::getInstance( );
		insManager.setPrepareDBThreadCondition( true );
	}
	else
	{
		kome::window::WindowTool::showError( "No search engine plug-ins are not installed.\nExist Mass++ and install at least one search engine plug-in." );
	}

	return;
}


// get Identification panel
kome::objects::Variant getIdentificationPanel( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	wxWindow* parent = kome::plugin::PluginCallTool::getWindow( params );
	if( parent == NULL ){
		return ret;
	}

	// @date 2014.07.08 <Mod> M.Izumi ->
	// preparation
	kome::ident::IdentificationManager&	insManager = kome::ident::IdentificationManager::getInstance( );
	insManager.setPrepareDBThreadCondition( false );
	boost::thread	insThread( prepareDatabaseThread );
	
//	if( !IdentificationDialogBase::prepare() ) {
//		return ret;
//	}
	// @date 2014.07.08 <Mod> M.Izumi <-
	
	std::string strName;
	strName = kome::plugin::PluginCallTool::getString( params, "panelName", "" );

	// panel
	wxWizard* pParent = dynamic_cast<wxWizard*>( parent );

	IdentificationPanel* pane = new IdentificationPanel( pParent, strName.c_str() );

	ret.prim.pt = (kome::window::PeakMatrixSettingsPageBase*)pane;

	return ret;
}

// show the visibility of the search result window
kome::objects::Variant showResultViewFromSearchEngine( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// search ID
	int nSearchID = kome::plugin::PluginCallTool::getSearchID( params );

	// wxWindow*
	wxWindow* ptrWindow = kome::plugin::PluginCallTool::getCanvas( params );

	// ダブルクリックの処理をここに移動する
	// 従来のダブルクリック処理は、ButtonがONになっているやつを呼び出す
	SearchResultDialog* ptrDlg = (SearchResultDialog*)ptrWindow;
	if( ptrDlg->getList() != NULL ){
		ptrDlg->getList()->onActivateDataOther( -1 );
	}

	return ret;
}

// show the visibility of the search result window
kome::objects::Variant showSubstances( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// search ID
	int nSearchID = kome::plugin::PluginCallTool::getSearchID( params );

	// wxWindow*
	wxWindow* ptrWindow = kome::plugin::PluginCallTool::getCanvas( params );

	// ダイアログを作ってみせるだけの処理

	// dlg は SubstanceDialog にstaticメンバを持たせて、そこに入れる

	// Spec No.92224:ダイアログが複数表示される @date 2013.06.24 <Add> M.Izumi
	if( SubstanceDialog::m_dlgSubstance != NULL ){
		delete SubstanceDialog::m_dlgSubstance;
	}
	SubstanceDialog::m_dlgSubstance = NULL;
	
	// 同定結果から生データ表示改造	// @date 2013/01/23 <Mod> OKADA
	kome::ident::SubstanceDialog* dlg = new kome::ident::SubstanceDialog(
		ptrWindow,
		"Substances",
		true
	);

	if( dlg != NULL ){						// @date 2013/01/23 <Add> OKADA
		dlg->clearStatus();					// @date 2013/01/29 <Add> OKADA
		dlg->setSearchID( nSearchID );		// @date 2013/01/30 <Add> OKADA

		SubstanceDialog::m_dlgSubstance = dlg;

		dlg->Show();
	}

	return ret;
}

// Test Ident Substance
kome::objects::Variant showIdentSpectrum( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// search Spectrum ID
	const int nSearchSpectrumID = kome::plugin::PluginCallTool::getSearchSpectrumID( params );

	// wxWindow*
	wxWindow* ptrWindow = kome::plugin::PluginCallTool::getCanvas( params );

	// IdentificationManager
	kome::ident::IdentificationManager& identificationMgr = kome::ident::IdentificationManager::getInstance();
             
	// spec id
	int nSpectrumID;
	int searchId;
	int peaksId;
	double precursor;

	identificationMgr.getSearchSpectrum( nSearchSpectrumID, &nSpectrumID, &searchId, &peaksId, &precursor );

	// Spectrum
	SpecInfo specInfo;
	identificationMgr.getSpectrum( nSpectrumID, specInfo );

	/** sample file path */
	std::string	strPath = specInfo.path;
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();
		
	kome::objects::SampleSet* sampleSet = factory.searchFile( strPath.c_str() );
	if( sampleSet == NULL ) {
		sampleSet = factory.openFile( strPath.c_str() );
	}

	// Spec No.93271:Substance View からスペクトルを表示した時 Sample Tree が現れない。 @date 2013.06.24 <Add> M.Izumi ->
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	if( sampleSet == NULL ){
		aoMgr.removeHistoryFile( strPath.c_str() );
	}else{
		kome::plugin::PluginCallTool::onOpenSampleSet( *sampleSet ); 

		aoMgr.addHistoryFile( strPath.c_str() );
	}
	// @date 2013.06.24 <Add> M.Izumi <-

	if( sampleSet != NULL ) {

		for( unsigned int i = 0; i < sampleSet->getNumberOfSamples(); i++ ) {
			kome::objects::Sample* sample = sampleSet->getSample( i );
			sample->openSample();

			kome::objects::Spectrum* spec = sample->getSpectrumById( specInfo.specId );	// @date 2013/02/07 <Add> OKADA

			// open
			if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
				kome::plugin::PluginCallTool::openSpectrum( *spec );
			}		
		}
	}

	return ret;
}

// Test Ident Substance
kome::objects::Variant testIdentSubstance( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// search ID
	int nSubstanceID = kome::plugin::PluginCallTool::getSubstanceID( params );

	// wxWindow*
	wxWindow* ptrWindow = kome::plugin::PluginCallTool::getCanvas( params );

	return ret;
}

// export pepXML file
kome::objects::Variant exportPepXML( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// path
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		LOG_ERROR( FMT( "Failed to get the path." ) );
		return ret;
	}

	// sample
	kome::objects::DataSet* ds = kome::plugin::PluginCallTool::getDataSet( params );
	kome::objects::Sample* sample = ( ds == NULL ? NULL : ds->getSample() );
	if( sample == NULL ) {
		LOG_ERROR( FMT( "Failed to get the active sample" ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );

	// searches
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	std::vector< int > searches;
	identMgr.getSearchesFromSample( searches, sample );
	// spec No.97493 @date 2014.03.11 <Add> M.Izumi ->
	if( searches.empty() ){
		kome::window::WindowTool::showError( "There are no identification results to export." );
		return ret;
	}
	// spec No.97493 @date 2014.03.11 <Add> M.Izumi <-
	int search = searches.back();

	// result
	kome::ident::SearchResult result;
	identMgr.getSearch( search, result );

	identMgr.writePepXML( path, result, false, progress );
	ret.prim.boolVal = true;

	return ret;
}


kome::objects::Variant execComparativeIdent( kome::objects::Parameters* params ){;	// @date 2013/07/26 Add OKADA task:#22 (17)-comparative_ident
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// manager
	kome::objects::ActiveObjectsManager& aoMgr
		= kome::objects::ActiveObjectsManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	// title
	const char* n = settings->getParameterValue( TITLE_PARAM_NAME ); // @date 2013/09/05 <Mod> OKADA
	std::string strTitle = NVL( n, "" );	
	if( strTitle.empty() ) {
		LOG_ERROR( FMT( "Failed to get the title." ) );
		return ret;
	}

	// comment
	n = settings->getParameterValue( SEARCH_COMMENT_PARAM_NAME ); // @date 2013/09/05 <Mod> OKADA
	std::string strComment = NVL( n, "" );	
	if( strComment.empty() ) {
		LOG_ERROR( FMT( "Failed to get the comment." ) );
//		return ret;	// 必須ではない
	}

	// search engine
	n = settings->getParameterValue( SEARCHENGINE_PARAM_NAME );	// @date 2013/09/05 <Mod> OKADA
	std::string str_engine = NVL( n, "" );	
	if( str_engine.empty() ) {
		LOG_ERROR( FMT( "Failed to get the engine." ) );
		return ret;
	}

	kome::plugin::PluginFunctionItem* searchFun = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, NVL( str_engine.c_str(), "" ) );
	if ( searchFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid search engine specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* searchSettings = settings->getSubParameterValues(
			SEARCHENGINE_PARAM_NAME, searchFun->getShortName());
	if ( searchSettings == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid search engine settings." ), ERR_OTHER );
		return ret;
	}

	kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)searchFun->getCall()->invoke( NULL ).prim.pt;

	if ( engine == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid search engine pointer." ), ERR_OTHER );
		return ret;
	}

	// RT tolerance
	n = settings->getParameterValue( "rt_tol" ); 
	std::string rt_tol = NVL( n, "" );	
	if( rt_tol.empty() ) {
		LOG_ERROR( FMT( "Failed to get the rt_tol." ) );
		return ret;
	}

	// m/z tolerance
	n = settings->getParameterValue( "mz_tol" ); 
	std::string mz_tol = NVL( n, "" );	
	if( mz_tol.empty() ) {
		LOG_ERROR( FMT( "Failed to get the mz_tol." ) );
		return ret;
	}

	// peak detector
	n = settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME ); // @date 2013/09/05 <Mod> OKADA
	std::string peak_detector = NVL( n, "" );	
	if( peak_detector.empty() ) {
		LOG_ERROR( FMT( "Failed to get the peak_detector." ) );
		return ret;
	}
	kome::plugin::PluginFunctionItem* peakFun = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, NVL( peak_detector.c_str(), "" ) );
	if ( peakFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid peak detection algorithm specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* peakSettings = settings->getSubParameterValues(
		PEAK_DETECTOR_PARAM_NAME,
		peakFun->getShortName()
	);

	// charge detector
	n = settings->getParameterValue( "charge_calculator" ); 
	std::string charge_calculator = NVL( n, "" );	
	if( charge_calculator.empty() ) {
		LOG_ERROR( FMT( "Failed to get the charge_calculator." ) );
		return ret;
	}

	kome::plugin::PluginFunctionItem* chargeFun = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, NVL( charge_calculator.c_str(), "" ) );
	if ( chargeFun == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid charge detection algorithm specified." ), ERR_OTHER );
		return ret;
	}

	kome::objects::SettingParameterValues* chargeSettings = settings->getSubParameterValues(
			CHARGE_DETECTOR_PARAM_NAME,
			chargeFun->getShortName()
	);

	if ( chargeSettings == NULL ) {
		LOG_ERROR_CODE( FMT( "Invalid charge settings." ), ERR_OTHER );
		return ret;
	}

	// Filter
	n = settings->getParameterValue( "filters" ); 
	std::string filters = NVL( n, "" );	
	if( filters.empty() ) {
		LOG_ERROR( FMT( "Failed to get the filters." ) );
//		return ret;
	}

	// 参考元：identificationOnBatch()

	printf( "Comparative Identification...\n" );

	// sample
	kome::objects::Sample* sample = aoMgr.getActiveSample();


	kome::objects::Sample* m_sample2 = kome::plugin::PluginCallTool::getSample( params );//

	/** spectrum getter */
	double dRtTol = atof( rt_tol.c_str() );
	double dMzTol = atof( mz_tol.c_str() );

	SpectrumGetter* spectra = createSpectrumGetter( engine, dRtTol, dMzTol );
/*
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		spectra->addSpectrum( dataSet->getSpectrum( i ) );
	}
*/
	// Identificationは、開いたサンプルのスペクトル↑を使う
	// Comparative Identificationはピークマトリクスを使う。createSpectrumGetter()内で追加

	// preparation
	if( !engine->prepareSearch( kome::core::Progress::getIgnoringProgress() ) ) {
		return ret;
	}

	// setting
	std::string msg = engine->validateSettings( searchSettings );
	if( !msg.empty() ) {
		LOG_ERROR_CODE( msg, ERR_OTHER );
		return ret;
	}

	// identification
	IdentificationPluginManager& mgr = IdentificationPluginManager::getInstance();
	kome::core::DisplayProgress* prgs = new kome::core::DisplayProgress( "Identification" );

	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settingsVal;

	int searchId = mgr.identify(
		strTitle.c_str(),
		strComment.c_str(),
		searchFun,
		searchSettings,
		peakFun,
		peakSettings,
		chargeFun,
		chargeSettings,
		spectra,
		filters.c_str(),
		prgs,
		items,
		settingsVal
	);

	bool result = ( searchId >= 0 );

/*
    コマンドラインなら結果を開く処理は必要なし。 (2013.08.20 S.Tanaka)

	if( result ) {
		// @date 2013/08/07 <Add> OKADA ------->
		if( op->getSearchEngine() != NULL ){
			const char* szResultLocation = op->getResultLocation();
			if( szResultLocation != NULL ){
				bRet = op->getSearchEngine()->openResult( szResultLocation );
			}

//			こんな感じの処理をしたい
//			m_engine->openResult( result->getResultLocation().c_str() );
//
//			getResultLocation()は、result内の std::string m_resultLocation を返す。

		}// @date 2013/08/07 <Add> OKADA <-------


	}
	else {
		delete op;
	}
*/

	//
	//////////////////////////////////////////////////////////////////////

	
#ifdef _DEBUG
	printf( "execComparativeIdent() [IdentificationPluginFunctions.cpp]" );
#endif


	ret.prim.boolVal = result;	// @date 2013/09/05 <Mod> OKADA

	return ret;
}

#if 1

// create Spectrum Getter
SpectrumGetter* createSpectrumGetter( SearchEngine* engine, double rt_tol, double mz_tol ) {

	ComparativeIdentificationSpectrumGetter* spectra = new ComparativeIdentificationSpectrumGetter( engine );

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// files
	int project = identMgr.getSelectedMatrix();
	std::vector< int > groups;
	std::vector< int > samples;
	std::string paths;
	identMgr.getGroups( groups, project );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		std::vector< int > tmp;
		identMgr.getMatrixSamplesFromGroup( tmp, groups[ i ] );
		for( unsigned int j = 0; j < tmp.size(); j++ ) {
			int id = identMgr.getMatrixSampleSample( tmp[ j ] );
			std::string path = identMgr.getSampleFilePath( id );
			path = kome::plugin::SettingsValue::convertToDisplayingString( path.c_str() );

			if( !path.empty() ) {
				spectra->addFile( path.c_str() );
			}
		}
	}
		
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		spectra->addPeak( peaks[i] );
	}

	// tolerance
	spectra->setRtTol( rt_tol );
	spectra->setMzTol( mz_tol );

	return spectra;
}


#endif

