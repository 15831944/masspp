/**
 * @file PluginCallTool.cpp
 * @brief implements of PluginCallTool class
 *
 * @author S.Tanaka
 * @date 2007.03.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginCallTool.h"

#include "PluginManager.h"
#include "PluginCall.h"
#include "SettingsValue.h"
#include "PluginFunctionItem.h"
#include "SettingsPage.h"

#include <boost/bind.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SAMPLE_SET_PARAM_NAME				  "sample set"
#define SAMPLE_PARAM_NAME					  "sample"
#define SPECTRUM_PARAM_NAME					  "spectrum"
#define CHROMATOGRAM_PARAM_NAME				  "chromatogram"
#define START_RANGE_PARAM_NAME				  "start"
#define END_RANGE_PARAM_NAME				  "end"
#define XY_DATA_PARAM_NAME					  "xy data"
#define PEAKS_PARAM_NAME					  "peaks"
#define PEAKS_2D_PARAM_NAME					  "peaks"
#define DATA_SET_PARAM_NAME					  "data set"
#define DATA_GROUP_NODE_PARAM_NAME			  "group"
#define PARENT_PEAKS_PARAM_NAME				  "parent peaks"
#define BASELINE_PARAM_NAME					  "baseline"
#define UPDATED_XY_DATA_PARAM_NAME			  "updated"
#define UPDATED_PEAKS_PARAM_NAME			  "updated"
#define UPDATED_PEAKS_2D_PARAM_NAME			  "updated"
#define CONTROL_SPEC_PARAM_NAME				  "control spectrum"
#define CONTROL_CHROM_PARAM_NAME			  "control chromatogram"
#define CONTROL_GROUP_PARAM_NAME			  "control group"
#define CONTROL_XY_DATA_PARAM_NAME			  "control xy data"
#define PATH_PARAM_NAME						  "path"
#define PROGRESS_PARAM_NAME					  "progress"
#define MOUSE_EVENT_PARAM_NAME				  "event"
#define KEY_EVENT_PARAM_NAME				  "event"
#define SIZE_EVENT_PARAM_NAME				  "event"
#define MOVE_EVENT_PARAM_NAME                 "event"
#define GRAPHICS_PARAM_NAME					  "graphics"
#define DATA_MAP_PARAM_NAME					  "data map"
#define WINDOW_PARAM_NAME					  "window"
#define PARENT_WINDOW_PARAM_NAME			  "parent window"
#define CANVAS_ID_PARAM_NAME				  "canvas"
#define SETTINGS_PARAM_NAME					  "settings"
#define ACTIVE_OBJECT_PARAM_NAME			  "active"
#define SAMPLE_GETTER_PARAM_NAME			  "sample getter"
#define FILTER_SPECTR_PARAM_NAME			  "filtered"

// IDENT_RESULT
#define	SEARCH_ID_PARAM_NAME			      "Search ID"				// Search ID	
// IDENT_SPECTRUM
#define	SEARCH_SPECTRUM_ID_PARAM_NAME	      "search spectrum ID"	    // search spectrum ID
// IDENT_SUBSTANCE
#define	SUBSTANCE_ID_PARAM_NAME			      "Substance ID"			// Substance ID


#define FUNC_TYPE_OPEN_FILE                  "OPEN_RAWDATA"
#define FUNC_TYPE_ON_SAVE_SAMPLE             "ON_SAVE_SAMPLE"
#define FUNC_TYPE_OPEN_SPECTRUM				 "SPECTRUM"
#define FUNC_TYPE_OPEN_CHROM				 "CHROMATOGRAM"
#define FUNC_TYPE_OPEN_DATA_MAP				 "DATA_MAP"
#define FUNC_TYPE_OPEN_SPECTRUM_WINDOW		 "SPECTRUM_WINDOW"
#define FUNC_TYPE_OPEN_CHROM_WINDOW			 "CHROMATOGRAM_WINDOW"
#define FUNC_TYPE_OPEN_DATA_MAP_WINDOW		 "DATA_MAP_WINDOW"
#define FUNC_TYPE_SPECTRUM_RANGE			 "SPECTRUM_RANGE"
#define FUNC_TYPE_CHROMATOGRAM_RANGE		 "CHROMATOGRAM_RANGE"
#define FUNC_TYPE_SPECTRUM_RANGE_ACTION		 "SPECTRUM_RANGE_ACTION"
#define FUNC_TYPE_CHROMATOGRAM_RANGE_ACTION	 "CHROMATOGRAM_RANGE_ACTION"
#define FUNC_TYPE_ON_OPEN_SAMPLE_SET		 "ON_OPEN_SAMPLE_SET"
#define FUNC_TYPE_ON_OPEN_SAMPLE			 "ON_OPEN_SAMPLE"
#define FUNC_TYPE_ON_CLOSE_SAMPLE			 "ON_CLOSE_SAMPLE"
#define FUNC_TYPE_CAN_EXIT					 "CAN_EXIT"
#define FUNC_TYPE_ON_UPDATE_SPECTRUM		 "ON_UPDATE_SPEC"
#define FUNC_TYPE_ON_UPDATE_CHROMATOGRAM	 "ON_UPDATE_CHROM"
#define FUNC_TYPE_ON_UPDATE_DATA_MAP		 "ON_UPDATE_DATA_MAP"
#define FUNC_TYPE_ON_UPDATE_GROUP			 "ON_UPDATE_GROUP"
#define FUNC_TYPE_ON_UPDATE_DATA_SET		 "ON_UPDATE_DATA_SET"
#define FUNC_TYPE_ON_UPDATE_SAMPLE			 "ON_UPDATE_SAMPLE"
#define FUNC_TYPE_DETECT_SPEC_PEAKS          "DETECT_SPEC_PEAKS"
#define FUNC_TYPE_SPEC_PEAK_FILTERING        "SPEC_PEAK_FILTER"
#define FUNC_TYPE_DETECT_CHROM_PEAKS         "DETECT_CHROM_PEAKS"
#define FUNC_TYPE_ON_UPDATE_SPEC_PEAKS		 "ON_UPDATE_SPEC_PEAKS"
#define FUNC_TYPE_ON_UPDATE_CHROM_PEAKS		 "ON_UPDATE_CHROM_PEAKS"
#define FUNC_TYPE_DETECT_2D_PEAKS		     "DETECT_2D_PEAKS"
#define FUNC_TYPE_ON_UPDATE_2D_PEAKS		 "ON_UPDATE_2D_PEAKS"
#define FUNC_TYPE_ON_UPDATE_PEAKS_MATRIX	 "ON_UPDATE_PEAK_MATRIX"
#define FUNC_TYPE_ON_MATRIX_PEAK			 "ON_MATRIX_PEAK"
#define FUNC_TYPE_OPEN_MATRIX				 "OPEN_MATRIX"
#define FUNC_TYPE_CLOSE_MATRIX				 "CLOSE_MATRIX"
#define FUNC_TYPE_ON_COMMIT_SPEC_PEAKS		 "ON_COMMIT_SPEC_PEAKS"
#define FUNC_TYPE_ON_COMMIT_CHROM_PEAKS		 "ON_COMMIT_CHROM_PEAKS"
#define FUNC_TYPE_ON_ROLLBACK_SPEC_PEAKS	 "ON_ROLLBACK_SPEC_PEAKS"
#define FUNC_TYPE_ON_ROLLBACK_CHROM_PEAKS	 "ON_ROLLBACK_CHROM_PEAKS"
#define FUNC_TYPE_COMMIT_SPEC_PEAKS			 "COMMIT_SPEC_PEAKS"
#define FUNC_TYPE_ROLLBACK_SPEC_PEAKS		 "ROLLBACK_SPEC_PEAKS"
#define FUNC_TYPE_COMMIT_CHROM_PEAKS		 "COMMIT_CHROM_PEAKS"
#define FUNC_TYPE_ROLLBACK_CHROM_PEAKS		 "ROLLBACK_CHROM_PEAKS" 
#define FUNC_TYPE_ON_OPEN_SPECTRUM			 "ON_OPEN_SPEC"
#define FUNC_TYPE_ON_OPEN_CHROMATOGRAM		 "ON_OPEN_CHROM"
#define FUNC_TYPE_ON_OPEN_DATA_MAP			 "ON_OPEN_DATA_MAP"
#define FUNC_TYPE_ON_CLOSE_SPECTRUM			 "ON_CLOSE_SPEC"
#define FUNC_TYPE_ON_CLOSE_CHROMATOGRAM		 "ON_CLOSE_CHROM"
#define FUNC_TYPE_ON_CLOSE_DATA_MAP			 "ON_CLOSE_DATA_MAP"
#define FUNC_TYPE_ON_ACTIVATE				 "ON_ACTIVATE"
#define FUNC_TYPE_ON_SETTINGS				 "ON_SETTINGS"
#define FUNC_TYPE_REFRESH                    "REFRESH"
#define FUNC_TYPE_CLOSE_ALL_SPEC			 "CLOSE_ALL_SPEC"
#define FUNC_TYPE_CLOSE_ALL_CHROM			 "CLOSE_ALL_CHROM"
#define FUNC_TYPE_CLOSE_ALL_DATA_MAP		 "CLOSE_ALL_DATA_MAP"
#define FUNC_TYPE_DRAW_SPEC					 "DRAW_SPEC"
#define FUNC_TYPE_PREPARE_DRAW_SPEC			 "PREPARE_SPEC_WINDOW"
#define FUNC_TYPE_DRAW_SPEC_FG				 "DRAW_SPEC_FG"
#define FUNC_TYPE_DRAW_SPEC_BG				 "DRAW_SPEC_BG"
#define FUNC_TYPE_DRAW_SPEC_WND_FG			 "DRAW_SPEC_WINDOW_FG"
#define FUNC_TYPE_DRAW_SPEC_WND_BG			 "DRAW_SPEC_WINDOW_BG"
#define FUNC_TYPE_DRAW_CHROM				 "DRAW_CHROM"
#define FUNC_TYPE_PREPARE_DRAW_CHROM		 "PREPARE_CHROM_WINDOW"
#define FUNC_TYPE_DRAW_CHROM_FG				 "DRAW_CHROM_FG"
#define FUNC_TYPE_DRAW_CHROM_BG				 "DRAW_CHROM_BG"
#define FUNC_TYPE_DRAW_CHROM_WND_FG			 "DRAW_CHROM_WINDOW_FG"
#define FUNC_TYPE_DRAW_CHROM_WND_BG			 "DRAW_CHROM_WINDOW_BG"
#define FUNC_TYPE_DRAW_DATA_MAP				 "DRAW_DATA_MAP"
#define FUNC_TYPE_PREPARE_DRAW_DATA_MAP		 "PREPARE_DATA_MAP_WINDOW"
#define FUNC_TYPE_DRAW_DATA_MAP_FG			 "DRAW_DATA_MAP_FG"
#define FUNC_TYPE_DRAW_DATA_MAP_BG			 "DRAW_DATA_MAP_BG"
#define FUNC_TYPE_DRAW_DATA_MAP_WND_FG		 "DRAW_DATA_MAP_WINDOW_FG"
#define FUNC_TYPE_DRAW_DATA_MAP_WND_BG		 "DRAW_DATA_MAP_WINDOW_BG"
#define FUNC_TYPE_ON_SPEC_BUTTON_DOWN		 "ON_SPEC_BUTTON_DOWN"
#define FUNC_TYPE_ON_SPEC_BUTTON_UP			 "ON_SPEC_BUTTON_UP"
#define FUNC_TYPE_ON_SPEC_DBL_CLK			 "ON_SPEC_DBL_CLK"
#define FUNC_TYPE_ON_SPEC_MOUSE_MOVE		 "ON_SPEC_MOUSE_MOVE"
#define FUNC_TYPE_ON_SPEC_MOUSE_LEAVE		 "ON_SPEC_MOUSE_LEAVE"
#define FUNC_TYPE_ON_SPEC_MOUSE_ENTER		 "ON_SPEC_MOUSE_ENTER"
#define FUNC_TYPE_ON_CHROM_BUTTON_DOWN		 "ON_CHROM_BUTTON_DOWN"
#define FUNC_TYPE_ON_CHROM_BUTTON_UP		 "ON_CHROM_BUTTON_UP"
#define FUNC_TYPE_ON_CHROM_DBL_CLK			 "ON_CHROM_DBL_CLK"
#define FUNC_TYPE_ON_CHROM_MOUSE_MOVE		 "ON_CHROM_MOUSE_MOVE"
#define FUNC_TYPE_ON_CHROM_MOUSE_LEAVE		 "ON_CHROM_MOUSE_LEAVE"
#define FUNC_TYPE_ON_CHROM_MOUSE_ENTER		 "ON_CHROM_MOUSE_ENTER"
#define FUNC_TYPE_ON_DATA_MAP_BUTTON_DOWN	 "ON_DATA_MAP_BUTTON_DOWN"
#define FUNC_TYPE_ON_DATA_MAP_BUTTON_UP		 "ON_DATA_MAP_BUTTON_UP"
#define FUNC_TYPE_ON_DATA_MAP_DBL_CLK		 "ON_DATA_MAP_DBL_CLK"
#define FUNC_TYPE_ON_DATA_MAP_MOUSE_MOVE	 "ON_DATA_MAP_MOUSE_MOVE"
#define FUNC_TYPE_ON_DATA_MAP_MOUSE_LEAVE	 "ON_DATA_MAP_MOUSE_LEAVE"
#define FUNC_TYPE_ON_DATA_MAP_MOUSE_ENTER	 "ON_DATA_MAP_MOUSE_ENTER"
#define FUNC_TYPE_ON_SPEC_KEY_DOWN			 "ON_SPEC_KEY_DOWN"
#define FUNC_TYPE_ON_SPEC_KEY_UP			 "ON_SPEC_KEY_UP"
#define FUNC_TYPE_ON_CHROM_KEY_DOWN			 "ON_CHROM_KEY_DOWN"
#define FUNC_TYPE_ON_CHROM_KEY_UP			 "ON_CHROM_KEY_UP"
#define FUNC_TYPE_ON_DATA_MAP_KEY_DOWN		 "ON_DATA_MAP_KEY_DOWN"
#define FUNC_TYPE_ON_DATA_MAP_KEY_UP		 "ON_DATA_MAP_KEY_UP"
#define FUNC_TYPE_QUANTITATION_WINDOW		 "QUANTITATION_WINDOW" 

// 同定結果から生データ表示	
#define FUNC_TYPE_IDENT_RESULT		         "IDENT_RESULT"
#define FUNC_TYPE_IDENT_SPECTRUM	         "IDENT_SPECTRUM"
#define FUNC_TYPE_IDENT_SUBSTANCE	         "IDENT_SUBSTANCE"


// open raw data
void PluginCallTool::openFile( const char* path ) {
	// log
	LOG_INFO( FMT( "Open file (%s)", path ) );

	// create parameters
	kome::objects::Parameters params;
	setPath( params, path );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_OPEN_FILE );
}

// on save sample
void PluginCallTool::onSaveSample( kome::objects::Sample& sample ) {
	// create parameters
	kome::objects::Parameters params;
	setSample( params, sample );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_SAVE_SAMPLE );
}

// open spectrum
void PluginCallTool::openSpectrum( kome::objects::Spectrum& spec ) {
	// log
	LOG_INFO( FMT( "Open spectrum (%s)", spec.getName() ) );

	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_OPEN_SPECTRUM );

	// add opened data
	kome::objects::DataManager::openSpectrum( &spec );
}

// open chromatogram
void PluginCallTool::openChromatogram( kome::objects::Chromatogram& chrom ) {
	// log
	LOG_INFO( FMT( "Open chromatogram (%s)", chrom.getName() ) );

	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_OPEN_CHROM );

	// add opened data
	kome::objects::DataManager::openChromatogram( &chrom );
}

// open data map
void PluginCallTool::openDataMap( kome::objects::DataMapInfo& dataMap ) {
	// log
	kome::objects::DataGroupNode& group = dataMap.getGroup();
	LOG_INFO( FMT( "Open data map (%s)", group.getName() ) );

	// creata parameters
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_OPEN_DATA_MAP );

	// add opened data
	kome::objects::DataMapManager::openDataMap( &dataMap );
}

// create spectrum window
wxWindow* PluginCallTool::createSpectrumWindow( kome::objects::Spectrum& spec, wxWindow* parent ) {
	// create paramters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setParentWindow( params, parent );

	// call functions
	return callOpenWindowFunctions( params, FUNC_TYPE_OPEN_SPECTRUM_WINDOW );
}

// create chromatogram window
wxWindow* PluginCallTool::createChromatogramWindow( kome::objects::Chromatogram& chrom, wxWindow* parent ) {
	// create paramters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setParentWindow( params, parent );

	// call functions
	return callOpenWindowFunctions( params, FUNC_TYPE_OPEN_CHROM_WINDOW );
}

// create data map window
wxWindow* PluginCallTool::createDataMapWindow( kome::objects::DataMapInfo& dataMap, wxWindow* parent ) {
	// create paramters
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setParentWindow( params, parent );

	// call functions
	return callOpenWindowFunctions( params, FUNC_TYPE_OPEN_DATA_MAP_WINDOW );
}

// broadcast a sample set is opened
void PluginCallTool::onOpenSampleSet( kome::objects::SampleSet& sampleSet, kome::core::Progress* progress ) {
	// create paremters
	kome::objects::Parameters params;
	setSampleSet( params, sampleSet );
	setProgress( params, *progress );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_OPEN_SAMPLE_SET );
}

// broadcast a sample is opened
void PluginCallTool::onOpenSample( kome::objects::Sample& sample ) {
	// create parameters
	kome::objects::Parameters params;
	setSample( params, sample );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_OPEN_SAMPLE );
}

// broadcast a sample is closed
void PluginCallTool::onCloseSample( kome::objects::Sample& sample ) {
	// create parameters
	kome::objects::Parameters params;
	setSample( params, sample );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_CLOSE_SAMPLE );
}

// can exit
bool PluginCallTool::canExit() {
	kome::objects::Parameters params;
	// call functions
	return callOpenFunctions( params, FUNC_TYPE_CAN_EXIT );
}

// broadcast a spectrum is updated
void PluginCallTool::onUpdateSpectrum( kome::objects::Spectrum& spec ) {
	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_SPECTRUM );
}

// broadcast a chromatogram is updated
void PluginCallTool::onUpdateChromatogram( kome::objects::Chromatogram& chrom ) {
	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_CHROMATOGRAM );
}

// broadcast a data map is updated
void PluginCallTool::onUpdateDataMap( kome::objects::DataMapInfo& dataMap ) {
	// create parameters
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_DATA_MAP );
}

// broadcast group is updated
void PluginCallTool::onUpdateGroup( kome::objects::DataGroupNode& group ) {
	// create parameters
	kome::objects::Parameters params;
	setGroup( params, group );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_GROUP );
}

// broadcast sample is updated
void PluginCallTool::onUpdateSample( kome::objects::Sample& sample ) {
	// create parameters
	kome::objects::Parameters params;
	setSample( params, sample );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_SAMPLE );
}

// broadcast data set is updated
void PluginCallTool::onUpdateDataSet( kome::objects::DataSet& dataSet ) {
	// create parameters
	kome::objects::Parameters params;
	setDataSet( params, dataSet );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_DATA_SET );
}

// broadcast a spectrum peaks are detected
void PluginCallTool::detectSpecPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress ) {
	// create parameters
	kome::objects::Parameters params;
	setDataSet( params, dataSet );

	if( progress != NULL ) {
		setProgress( params, *progress );
	}

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DETECT_SPEC_PEAKS );
}

// filter spectrum peaks
void PluginCallTool::filterSpecPeaks(
		kome::objects::Spectrum** spectra,
		kome::objects::Peaks** peaks,
		const unsigned int num,
		const char* filter
) {
	// check parametrs
	if( spectra == NULL || peaks == NULL || filter == NULL || num == 0 ) {
		return;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// filters
	std::vector< std::string > filters;
	SettingsValue::separateListValue( filter, filters );

	for( unsigned int i = 0; i < filters.size(); i++ ) {
		std::string s = filters[ i ];

		LOG_DEBUG( FMT( "Peak Filter: %s", s.c_str() ) );

		// item & parameters
		int ifind = s.find_first_of( "[" );
		std::string strItem = s.substr( 0, ifind );
		std::string strParams = s.substr( ifind );

		// item
		PluginFunctionItem* item = plgMgr.getFunctionItem( FUNC_TYPE_SPEC_PEAK_FILTERING, strItem.c_str() );
		if( item == NULL ) {
			return;
		}

		// parameters
		kome::objects::SettingParameterValues settings;
		SettingsPage* page = ( item == NULL ? NULL: item->getSettingsPage() );
		if( page != NULL ) {
			page->setParameterString( settings, strParams.c_str() );
		}

		// update peaks
		for( unsigned int j = 0; j < num; j++ ) {
			kome::objects::Spectrum* s = spectra[ j ];
			kome::objects::Peaks* p = peaks[ j ];

			// data points
			kome::core::DataPoints dps;
			s->getXYData( &dps, false );

			// filter
			PluginCall* call = item->getCall();

			// @date 2013/04/17 <Add> OKADA ------->
			if( call == NULL ){
				continue;
			}else if( call->isActive() ){
				// OK
			}else{
				LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
				continue;	// activeでない関数は呼び出さない
			}
			// @date 2013/04/17 <Add> OKADA <-------

			kome::objects::Parameters params;
			setSpectrum( params, *s );
			setPeaks( params, *p );
			setSettingValues( params, settings );
			setXYData( params, dps );

			call->invoke( &params );
		}
	}
}

// broadcast a spectrum peaks are updated
void PluginCallTool::onUpdateSpecPeaks( kome::objects::Spectrum& spec ) {
	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_SPEC_PEAKS );
}

// broadcast a chromatogram peaks are detected
void PluginCallTool::detectChromPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress ) {
	// create parameters
	kome::objects::Parameters params;
	setDataSet( params, dataSet );

	if( progress != NULL ) {
		setProgress( params, *progress );
	}

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DETECT_CHROM_PEAKS );
}

// broadcast a chromatogram peaks are updated
void PluginCallTool::onUpdateChromPeaks( kome::objects::Chromatogram& chrom ) {
	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_CHROM_PEAKS );
}

// broadcast 2D peaks are detected
void PluginCallTool::detect2DPeaks( kome::objects::DataGroupNode& group, kome::core::Progress* progress ) {
	// create parameters
	kome::objects::Parameters params;
	setGroup( params, group );

	if( progress != NULL ) {
		setProgress( params, *progress );
	}

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DETECT_2D_PEAKS );
}

// broadcast 2D peaks are updated
void PluginCallTool::onUpdate2DPeaks( kome::objects::DataGroupNode& group ) {
	// create parameters
	kome::objects::Parameters params;
	setGroup( params, group );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_UPDATE_2D_PEAKS );
}

// broadcast peaks matrix is updated
void PluginCallTool::onUpdatePeaksMatrix() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_ON_UPDATE_PEAKS_MATRIX );
}

// matrix peak action
void PluginCallTool::onMatrixPeak( const int peakId, wxWindow* parent ) {
	// parameters
	kome::objects::Parameters params;
	setInt( params, "peak", peakId );
	setParentWindow( params, parent );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_MATRIX_PEAK );
}

// open matrix
void PluginCallTool::openMatrix() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_OPEN_MATRIX );
}

// close matrix
void PluginCallTool::closeMatrix() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_CLOSE_MATRIX );
}

// broadcast spectrum peaks are commited
void PluginCallTool::onCommitSpecPeaks(
		kome::objects::Spectrum& spec,
		kome::core::XYData& xyData,
		kome::objects::Peaks& src,
		kome::objects::Peaks& dst
) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setXYData( params, xyData );
	setPeaks( params, src );
	setUpdatedPeaks( params, dst );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_COMMIT_SPEC_PEAKS );
}

// broadcast chromatogram peaks are commited
void PluginCallTool::onCommitChromPeaks(
		kome::objects::Chromatogram& chrom,
		kome::core::XYData& xyData,
		kome::objects::Peaks& src,
		kome::objects::Peaks& dst
) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setXYData( params, xyData );
	setPeaks( params, src );
	setUpdatedPeaks( params, dst );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_COMMIT_CHROM_PEAKS );
}

// broadcast spectrum peaks are rollbacked
void PluginCallTool::onRollbackSpecPeaks(
		kome::objects::Spectrum& spec,
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::objects::Peaks& canceledPeaks
) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setXYData( params, xyData );
	setPeaks( params, peaks );
	setUpdatedPeaks( params, canceledPeaks );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_ROLLBACK_SPEC_PEAKS );
}

// broadcast chromatogram peaks are rollbacked
void PluginCallTool::onRollbackChromPeaks(
		kome::objects::Chromatogram& chrom,
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::objects::Peaks& canceledPeaks
) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setXYData( params, xyData );
	setPeaks( params, peaks );
	setUpdatedPeaks( params, canceledPeaks );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_ROLLBACK_CHROM_PEAKS );
}

// commit spectrum peaks
void PluginCallTool::commitSpecPeaks( kome::objects::Spectrum& spec ) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_COMMIT_SPEC_PEAKS );
}

// commit chromatogram peaks
void PluginCallTool::commitChromPeaks( kome::objects::Chromatogram& chrom ) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_COMMIT_CHROM_PEAKS );
}

// rollback spectrum peaks
void PluginCallTool::rollbackSpecPeaks( kome::objects::Spectrum& spec ) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ROLLBACK_SPEC_PEAKS );
}

// rollback chromatogram peaks
void PluginCallTool::rollbackChromPeaks( kome::objects::Chromatogram& chrom ) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ROLLBACK_CHROM_PEAKS );
} 

// broadcast a spectrum is opened
bool PluginCallTool::onOpenSpectrum( kome::objects::Spectrum& spec ) {
	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );

	// call functions
	return callOpenFunctions( params, FUNC_TYPE_ON_OPEN_SPECTRUM );
}

// broadcast a chromatogram is opened
bool PluginCallTool::onOpenChromatogram( kome::objects::Chromatogram& chrom ) {
	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );

	// call functions
	return callOpenFunctions( params, FUNC_TYPE_ON_OPEN_CHROMATOGRAM );
}

// broadcast a data map is opened
bool PluginCallTool::onOpenDataMap( kome::objects::DataMapInfo& dataMap ) {
	// create parameters
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );

	// call functions
	return callOpenFunctions( params, FUNC_TYPE_ON_OPEN_DATA_MAP );
}

// broadcast a spectrum is closed
void PluginCallTool::onCloseSpectrum( kome::objects::Spectrum& spec, wxWindow* canvas ) {
	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_CLOSE_SPECTRUM );

	// delete opened data
	kome::objects::DataManager::closeSpectrum( &spec, false );
}

// broadcast a chromatogram is closed
void PluginCallTool::onCloseChromatogram( kome::objects::Chromatogram& chrom, wxWindow* canvas ) {
	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_CLOSE_CHROMATOGRAM );


	// delete opened data
	kome::objects::DataManager::closeChromatogram( &chrom, false );
}

// broadcast a data map is closed
void PluginCallTool::onCloseDataMap( kome::objects::DataMapInfo& dataMap, wxWindow* canvas ) {
	// create parameters
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_CLOSE_DATA_MAP );

	// delete opened data
	kome::objects::DataManager::closeDataMap( &dataMap, false );
}

// broadcast active object is changed
void PluginCallTool::onActivate( kome::objects::Variant& obj ) {
	// parameters
	kome::objects::Parameters params;
	setActiveObject( params, obj );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_ON_ACTIVATE );
}

// broadcast settings parameter is updated
void PluginCallTool::onSettings() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_ON_SETTINGS );
}

// refreshes windows
void PluginCallTool::refresh() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_REFRESH );
}

// change spectrum range
void PluginCallTool::changeSpectrumRange( kome::objects::Spectrum& spec, const double start, const double end, const long long canvas ) {
	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setRange( params, start, end );
	setCanvas( params, (wxWindow*)canvas );

	// call function
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_SPECTRUM_RANGE );
}

// change chromatogram range
void PluginCallTool::changeChromatogramRange( kome::objects::Chromatogram& chrom, const double start, const double end, const long long canvas ) {
	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setRange( params, start, end );
	setCanvas( params, (wxWindow*)canvas );

	// call function
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_CHROMATOGRAM_RANGE );
}

// spectrum range action
void PluginCallTool::actSpectrumRange( kome::objects::Spectrum& spec, const double start, const double end ) {
	// log
	LOG_INFO( FMT( "Spectrum range (%s) [%.2f-%.2f]", spec.getName(), start, end ) );

	// create parameters
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setRange( params, start, end );

	// call function
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_SPECTRUM_RANGE_ACTION );
}

// chromatogram range
void PluginCallTool::actChromatogramRange( kome::objects::Chromatogram& chrom, const double start, const double end, kome::core::Progress* progress ) {
	// log
	LOG_INFO( FMT( "Chromatogram range (%s) [%.2f-%.2f]", chrom.getName(), start, end ) );

	// create parameters
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setRange( params, start, end );

	if( progress != NULL ) {
		setProgress( params, *progress );
	}

	// call function
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_CHROMATOGRAM_RANGE_ACTION );
}

// close all spectrum
void PluginCallTool::closeAllSpectrum() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_CLOSE_ALL_SPEC );
}

// close all chromatogram
void PluginCallTool::closeAllChromatogram() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_CLOSE_ALL_CHROM );
}

// close all data map
void PluginCallTool::closeAllDataMap() {
	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, FUNC_TYPE_CLOSE_ALL_DATA_MAP );
}

// draw spectrum
void PluginCallTool::drawSpectrum(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.forEachCalls(
		FUNC_TYPE_DRAW_SPEC,
		boost::bind( onDrawProfile, _1, &params, &g ),
		true
	);
}

// prepare to draw spectrum
void PluginCallTool::prepareDrawSpectrum(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_PREPARE_DRAW_SPEC );
}

// draw spectrum foreground
void PluginCallTool::drawSpectrumFg(
		kome::objects::Spectrum&
		spec, kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_SPEC_FG );
}

// draw spectrum background
void PluginCallTool::drawSpectrumBg(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_SPEC_BG );
}

// draw spectrum window foreground
void PluginCallTool::drawSpectrumWindowFg(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_SPEC_WND_FG );
}

// draw spectrum window background
void PluginCallTool::drawSpectrumWindowBg(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_SPEC_WND_BG );
}

// draw chromatogram
void PluginCallTool::drawChromatogram(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.forEachCalls(
		FUNC_TYPE_DRAW_CHROM,
		boost::bind( onDrawProfile, _1, &params, &g ),
		true
	);
}

// prepare to draw chromatogram
void PluginCallTool::prepareDrawChromatogram(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_PREPARE_DRAW_CHROM );
}

// draw chromatogram foreground
void PluginCallTool::drawChromatogramFg(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_CHROM_FG );
}

// draw chromatogram background
void PluginCallTool::drawChromatogramBg(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_CHROM_BG );
}

// draw chromatogram window foreground
void PluginCallTool::drawChromatogramWindowFg(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_CHROM_WND_FG );
}

// draw chroamtogram window background
void PluginCallTool::drawChromatogramWindowBg(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_CHROM_WND_BG );
}

// draw data map
void PluginCallTool::drawDataMap(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.forEachCalls(
		FUNC_TYPE_DRAW_DATA_MAP,
		boost::bind( onDrawProfile, _1, &params, &g ),
		true
	);
}

// prepare to draw data map
void PluginCallTool::prepareDrawDataMap(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_PREPARE_DRAW_DATA_MAP );
}

// draw data map foreground
void PluginCallTool::drawDataMapFg(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_DATA_MAP_FG );
}

// draw data map background
void PluginCallTool::drawDataMapBg(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_DATA_MAP_BG );
}

// draw data map window foreground
void PluginCallTool::drawDataMapWindowFg(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_DATA_MAP_WND_FG );
}

// draw spectrum window background
void PluginCallTool::drawDataMapWindowBg(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& g,
		wxWindow* canvas
) {
	// check the graphics
	if( g.getGraphWidth() <= 0 || g.getGraphHeight() <= 0 ) {
		return;
	}

	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	setGraphics( params, g );
	setCanvas( params, canvas );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_DRAW_DATA_MAP_WND_BG );
}

// broadcast mouse button is down on spectrum window
bool PluginCallTool::onSpectrumButtonDown(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_BUTTON_DOWN );
}

// broadcast mouse button is up on spectrum window
bool PluginCallTool::onSpectrumButtonUp(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_BUTTON_UP );
}

// broadcast mouse button is double clicked on spectrum window
bool PluginCallTool::onSpectrumDoubleClick(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_DBL_CLK );
}

// broadcast mouse cursor moves on spectrum window
bool PluginCallTool::onSpectrumMouseMove(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_MOUSE_MOVE );
}

// broadcast mouse cursor leaves from spectrum window
bool PluginCallTool::onSpectrumMouseLeave(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_MOUSE_LEAVE );
}

// broadcast mouse cursor enters into spectrum window
bool PluginCallTool::onSpectrumMouseEnter(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_MOUSE_ENTER );
}

// broadcast mouse button is down on chromatogram window
bool PluginCallTool::onChromatogramButtonDown(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_BUTTON_DOWN );
}

// broadcast mouse button is up on chromatogram window
bool PluginCallTool::onChromatogramButtonUp(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_BUTTON_UP );
}

// broadcast mouse button is double clicked on chromatogram window
bool PluginCallTool::onChromatogramDoubleClick(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_DBL_CLK );
}

// broadcast mouse cursor moves on chromatogram window
bool PluginCallTool::onChromatogramMouseMove(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_MOUSE_MOVE );
}

// broadcast mouse cursor leaves from chromatogram window
bool PluginCallTool::onChromatogramMouseLeave(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;

	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_MOUSE_LEAVE );
}

// broadcast mouse cursor enters into chromatogram window
bool PluginCallTool::onChromatogramMouseEnter(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_MOUSE_ENTER );
}

// broadcast mouse button is down on data map window
bool PluginCallTool::onDataMapButtonDown(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_BUTTON_DOWN );
}

// broadcast mouse button is up on data map window
bool PluginCallTool::onDataMapButtonUp(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_BUTTON_UP );
}

// broadcast mouse button is double clicked on data map window
bool PluginCallTool::onDataMapDoubleClick(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_DBL_CLK );
}

// broadcast mouse cursor moves on data map window
bool PluginCallTool::onDataMapMouseMove(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_MOUSE_MOVE );
}

// broadcast mouse cursor leaves from data map window
bool PluginCallTool::onDataMapMouseLeave(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_MOUSE_LEAVE );
}

// broadcast mouse cursor enters into data map window
bool PluginCallTool::onDataMapMouseEnter(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::MouseEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setMouseEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_MOUSE_ENTER );
}

// broadcast key is down on spectrum window
bool PluginCallTool::onSpectrumKeyDown(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_KEY_DOWN );
}

// broadcast key is up on spectrum window
bool PluginCallTool::onSpectrumKeyUp(
		kome::objects::Spectrum& spec,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setSpectrum( params, spec );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_SPEC_KEY_UP );
}

// broadcast key is down on chromatogram window
bool PluginCallTool::onChromatogramKeyDown(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_KEY_DOWN );
}

// broadcast key is up on chromatogram window
bool PluginCallTool::onChromatogramKeyUp(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setChromatogram( params, chrom );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_CHROM_KEY_UP );
}

// broadcast key is down on data map window
bool PluginCallTool::onDataMapKeyDown(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_KEY_DOWN );
}

// broadcast key is up on data map window
bool PluginCallTool::onDataMapKeyUp(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics* g,
		kome::evt::KeyEvent& evt,
		wxWindow* canvas
) {
	// create parameter
	kome::objects::Parameters params;
	setDataMapInfo( params, dataMap );
	if( g != NULL ) {
		setGraphics( params, *g );
	}
	setKeyEvent( params, evt );
	setCanvas( params, canvas );

	// call functions
	return callInputFunctions( params, FUNC_TYPE_ON_DATA_MAP_KEY_UP );
}

// call open event function
void PluginCallTool::onOpenEvent(
		PluginCall* call,
		kome::objects::Parameters* params,
		bool* enable
) {
	// check parameters
	if( call == NULL ) {
		return;
	}

	// check refresh flag
	if( !( *enable ) ) {
		return;
	}

	// @date 2013/04/17 <Add> OKADA	------->
	if( call->isActive() ){
		// OK
	}else{
		LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
		return;	// activeでない関数は呼び出さない
	}
	// @date 2013/04/17 <Add> OKADA <-------

	// call function
	*enable = call->invoke( params ).prim.boolVal;
}

// call open event functions
bool PluginCallTool::callOpenFunctions( kome::objects::Parameters& params, const char* type ) {
	// check parameters
	if( type == NULL ) {
		return false;
	}

	// plug-in manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// create array
	bool enable = true;
	plgMgr.forEachCalls(
		type,
		boost::bind(
			onOpenEvent,
			_1,
			&params,
			&enable
		),
		true
	);

	return enable;
}

// call drawing profile function
void PluginCallTool::onDrawProfile(
		PluginCall* call,
		kome::objects::Parameters* params,
		kome::img::Graphics* graphics
) {
	// @date 2013/04/17 <Add> OKADA ------->
	if( call == NULL ){
		return;
	}else if( call->isActive() ){
		// OK
	}else{
		LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
		return;	// activeでない関数は呼び出さない
	}
	// @date 2013/04/17 <Add> OKADA <-------

	// call function
	call->invoke( params );

	// flush
	graphics->flush();
}

// call open window function
void PluginCallTool::onOpenWindow(
		PluginCall* call,
		kome::objects::Parameters* params,
		wxWindow** wnd
) {
	// check parameters
	if( call == NULL ) {
		return;
	}

	// check window object
	if( *wnd != NULL ) {
		return;
	}

	// call function
	if( call->checkStatus() ) {
		
		// @date 2013/04/17 <Add> OKADA	------->
		if( call->isActive() ){
			// OK
		}else{
			LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
			return;	// activeでない関数は呼び出さない
		}
		// @date 2013/04/17 <Add> OKADA <-------
		// @date 2013/04/19 <Comment> OKADA 処理位置変更
		
		kome::objects::Variant ret = call->invoke( params );
		if( ret.type == kome::objects::Variant::WINDOW ) {
			*wnd = (wxWindow*)ret.prim.pt;
		}
	}
}

// call open window functions
wxWindow* PluginCallTool::callOpenWindowFunctions( kome::objects::Parameters& params, const char* type ) {
	// check parameters
	if( type == NULL ) {
		return false;
	}

	// plug-in manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// create array
	wxWindow* wnd = NULL;
	plgMgr.forEachCalls(
		type,
		boost::bind(
			onOpenWindow,
			_1,
			&params,
			&wnd
		),
		true
	);

	return wnd;
}

// call input event function
void PluginCallTool::onInputEvent(
		PluginCall* call,
		kome::objects::Parameters* params,
		bool* refresh
) {
	// check parameters
	if( call == NULL ) {
		return;
	}

	// check refresh flag
	if( *refresh ) {
		return;
	}

	// @date 2013/04/17 <Add> OKADA	------->
	if( call->isActive() ){
		// OK
	}else{
		LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
		return;	// activeでない関数は呼び出さない
	}
	// @date 2013/04/17 <Add> OKADA <-------

	// call function
	*refresh = call->invoke( params ).prim.boolVal;
}

// call input event functions
bool PluginCallTool::callInputFunctions( kome::objects::Parameters& params, const char* type ) {
	// check parameters
	if( type == NULL ) {
		return false;
	}

	// plug-in manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// create array
	bool refresh = false;
	plgMgr.forEachCalls(
		type,
		boost::bind(
			onInputEvent,
			_1,
			&params,
			&refresh
		),
		true
	);

	return refresh;
}

// set sample set object
void PluginCallTool::setSampleSet( kome::objects::Parameters& params, kome::objects::SampleSet& sampleSet ) {
	kome::objects::Variant& sampleSetParam = params.getValue( SAMPLE_SET_PARAM_NAME );
	sampleSetParam.type = kome::objects::Variant::SAMPLE_SET;
	sampleSetParam.prim.pt = &sampleSet;
}

// set sample object
void PluginCallTool::setSample( kome::objects::Parameters& params, kome::objects::Sample& sample ) {
	kome::objects::Variant& sampleParam = params.getValue( SAMPLE_PARAM_NAME );
	sampleParam.type = kome::objects::Variant::SAMPLE;
	sampleParam.prim.pt = &sample;
}

// set spectrum object
void PluginCallTool::setSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec ) {
	kome::objects::Variant& specParam = params.getValue( SPECTRUM_PARAM_NAME );
	specParam.type = kome::objects::Variant::SPECTRUM;
	specParam.prim.pt = &spec;
}

// set chromatogram object
void PluginCallTool::setChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom ){
	kome::objects::Variant& chromParam = params.getValue( CHROMATOGRAM_PARAM_NAME );
	chromParam.type = kome::objects::Variant::CHROMATOGRAM;
	chromParam.prim.pt = &chrom;
}

// set xy data
void PluginCallTool::setXYData( kome::objects::Parameters& params, kome::core::XYData& xyData ) {
	kome::objects::Variant& xyParam = params.getValue( XY_DATA_PARAM_NAME );
	xyParam.type = kome::objects::Variant::XY_DATA;
	xyParam.prim.pt = &xyData;
}

// set data set
void PluginCallTool::setDataSet( kome::objects::Parameters& params, kome::objects::DataSet& dataSet ) {
	kome::objects::Variant& dataSetParam = params.getValue( DATA_SET_PARAM_NAME );
	dataSetParam.type = kome::objects::Variant::DATA_SET;
	dataSetParam.prim.pt = &dataSet;
}

// set data set
void PluginCallTool::setSpectraFilter( kome::objects::Parameters& params, kome::objects::DataSet& dataSet ) {
	kome::objects::Variant& dataSetParam = params.getValue( FILTER_SPECTR_PARAM_NAME );
	dataSetParam.type = kome::objects::Variant::DATA_SET;
	dataSetParam.prim.pt = &dataSet;
}

// set spectrum group
void PluginCallTool::setGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group ) {
	kome::objects::Variant& groupParam = params.getValue( DATA_GROUP_NODE_PARAM_NAME );
	groupParam.type = kome::objects::Variant::DATA_GROUP_NODE;
	groupParam.prim.pt = &group;
}

// set peaks
void PluginCallTool::setPeaks( kome::objects::Parameters& params, kome::objects::Peaks& peaks ) {
	kome::objects::Variant& peaksParam = params.getValue( PEAKS_PARAM_NAME );
	peaksParam.type = kome::objects::Variant::PEAKS;
	peaksParam.prim.pt = &peaks;
}

// set parent peaks
void PluginCallTool::setParentPeaks( kome::objects::Parameters& params, kome::objects::Peaks& parentPeaks ) {
	kome::objects::Variant& ppeaksParam = params.getValue( PARENT_PEAKS_PARAM_NAME );
	ppeaksParam.type = kome::objects::Variant::PEAKS;
	ppeaksParam.prim.pt = &parentPeaks;
}

// set 2D peaks
void PluginCallTool::setPeaks2D( kome::objects::Parameters& params, kome::objects::Peaks2D& peaks ) {
	kome::objects::Variant& peaksParam = params.getValue( PEAKS_2D_PARAM_NAME );
	peaksParam.type = kome::objects::Variant::PEAKS_2D;
	peaksParam.prim.pt = &peaks;
}

// set baseline
void PluginCallTool::setBaseline( kome::objects::Parameters& params, kome::core::XYData& baseline ) {
	kome::objects::Variant& baselineParam = params.getValue( BASELINE_PARAM_NAME );
	baselineParam.type = kome::objects::Variant::XY_DATA;
	baselineParam.prim.pt = &baseline;
}

// set updated xy data
void PluginCallTool::setUpdatedXYData( kome::objects::Parameters& params, kome::core::XYData& updated ) {
	kome::objects::Variant& updatedParam = params.getValue( UPDATED_XY_DATA_PARAM_NAME );
	updatedParam.type = kome::objects::Variant::XY_DATA;
	updatedParam.prim.pt = &updated;
}

// set updated peaks
void PluginCallTool::setUpdatedPeaks( kome::objects::Parameters& params, kome::objects::Peaks& updated ) {
	kome::objects::Variant& updatedParam = params.getValue( UPDATED_PEAKS_PARAM_NAME );
	updatedParam.type = kome::objects::Variant::PEAKS;
	updatedParam.prim.pt = &updated;
}

// set updated peaks 2d
void PluginCallTool::setUpdatedPeaks2d( kome::objects::Parameters& params, kome::objects::Peaks2D& updated ){
	kome::objects::Variant& updatedParam = params.getValue( UPDATED_PEAKS_2D_PARAM_NAME );
	updatedParam.type = kome::objects::Variant::PEAKS_2D;
	updatedParam.prim.pt = &updated;
}

// set control spectrum
void PluginCallTool::setControlSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec ) {
	kome::objects::Variant& controlParam = params.getValue( CONTROL_SPEC_PARAM_NAME );
	controlParam.type = kome::objects::Variant::SPECTRUM;
	controlParam.prim.pt = &spec;
}

// set control chromatogram
void PluginCallTool::setControlChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom ) {
	kome::objects::Variant& controlParam = params.getValue( CONTROL_CHROM_PARAM_NAME );
	controlParam.type = kome::objects::Variant::CHROMATOGRAM;
	controlParam.prim.pt = &chrom;
}

// set control group
void PluginCallTool::setControlGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group ) {
	kome::objects::Variant& controlParam = params.getValue( CONTROL_GROUP_PARAM_NAME );
	controlParam.type = kome::objects::Variant::DATA_GROUP_NODE;
	controlParam.prim.pt = &group;
}

// set control spectrum
void PluginCallTool::setControlXYData( kome::objects::Parameters& params, kome::core::XYData& xyData ) {
	kome::objects::Variant& controlParam = params.getValue( CONTROL_XY_DATA_PARAM_NAME );
	controlParam.type = kome::objects::Variant::XY_DATA;
	controlParam.prim.pt = &xyData;
}

// set file path
void PluginCallTool::setPath( kome::objects::Parameters& params, const char* path ) {
	kome::objects::Variant& pathParam = params.getValue( PATH_PARAM_NAME );
	pathParam.setString( path );
}

// set start of range
void PluginCallTool::setStartOfRange( kome::objects::Parameters& params, const double start ) {
	kome::objects::Variant& startVal = params.getValue( START_RANGE_PARAM_NAME );
	startVal.type = kome::objects::Variant::DOUBLE;
	startVal.prim.dblVal = start;
}

// set end of range
void PluginCallTool::setEndOfRange( kome::objects::Parameters& params, const double end ) {
	kome::objects::Variant& endVal = params.getValue( END_RANGE_PARAM_NAME );
	endVal.type = kome::objects::Variant::DOUBLE;
	endVal.prim.dblVal = end;
}

// set range
void PluginCallTool::setRange( kome::objects::Parameters& params, const double start, const double end ) {
	setStartOfRange( params, start );
	setEndOfRange( params, end );
}

// set progress
void PluginCallTool::setProgress( kome::objects::Parameters& params, kome::core::Progress& progress ) {
	kome::objects::Variant& pgrVal = params.getValue( PROGRESS_PARAM_NAME );
	pgrVal.type = kome::objects::Variant::PROGRESS;
	pgrVal.prim.pt = &progress;
}

// set mouse event
void PluginCallTool::setMouseEvent( kome::objects::Parameters& params, kome::evt::MouseEvent& evt ) {
	kome::objects::Variant& meVal = params.getValue( MOUSE_EVENT_PARAM_NAME );
	meVal.type = kome::objects::Variant::MOUSE_EVT;
	meVal.prim.pt = &evt;
}

// set key event
void PluginCallTool::setKeyEvent( kome::objects::Parameters& params, kome::evt::KeyEvent& evt ) {
	kome::objects::Variant& keVal = params.getValue( KEY_EVENT_PARAM_NAME );
	keVal.type = kome::objects::Variant::KEY_EVT;
	keVal.prim.pt = &evt;
}

// set size event
void PluginCallTool::setSizeEvent( kome::objects::Parameters& params, kome::evt::SizeEvent& evt ) {
	kome::objects::Variant& seVal = params.getValue( SIZE_EVENT_PARAM_NAME );
	seVal.type = kome::objects::Variant::SIZE_EVT;
	seVal.prim.pt = &evt;
}

// set move event
void PluginCallTool::setMoveEvent( kome::objects::Parameters& params, kome::evt::MoveEvent& evt ) {
	kome::objects::Variant& meVal = params.getValue( MOVE_EVENT_PARAM_NAME );
	meVal.type = kome::objects::Variant::MOVE_EVT;
	meVal.prim.pt = &evt;
}

// set graphics
void PluginCallTool::setGraphics( kome::objects::Parameters& params, kome::img::Graphics& g ) {
	kome::objects::Variant& gVal = params.getValue( GRAPHICS_PARAM_NAME );
	gVal.type = kome::objects::Variant::GRAPHICS;
	gVal.prim.pt = &g;
}

// set data map
void PluginCallTool::setDataMapInfo( kome::objects::Parameters& params, kome::objects::DataMapInfo& dataMap ) {
	kome::objects::Variant& dmVal = params.getValue( DATA_MAP_PARAM_NAME );
	dmVal.type = kome::objects::Variant::DATA_MAP;
	dmVal.prim.pt = &dataMap;
}

// set window
void PluginCallTool::setWindow( kome::objects::Parameters& params, wxWindow* wnd ) {
	kome::objects::Variant& wVal = params.getValue( WINDOW_PARAM_NAME );
	wVal.type = kome::objects::Variant::WINDOW;
	wVal.prim.pt = wnd;
}

// set parent window
void PluginCallTool::setParentWindow( kome::objects::Parameters& params, wxWindow* parent ) {
	kome::objects::Variant& wVal = params.getValue( PARENT_WINDOW_PARAM_NAME );
	wVal.type = kome::objects::Variant::WINDOW;
	wVal.prim.pt = parent;
}

// set canvas ID
void PluginCallTool::setCanvas( kome::objects::Parameters& params, wxWindow* canvas ) {
	kome::objects::Variant& cVal = params.getValue( CANVAS_ID_PARAM_NAME );
	cVal.type = kome::objects::Variant::WINDOW;
	cVal.prim.pt = canvas;
}

// set parameter values
void PluginCallTool::setSettingValues( kome::objects::Parameters& params, kome::objects::SettingParameterValues& vals ) {
	kome::objects::Variant& sVal = params.getValue( SETTINGS_PARAM_NAME );
	sVal.type = kome::objects::Variant::SETTING_VALUES;
	sVal.prim.pt = &vals;
}

// set active object
void PluginCallTool::setActiveObject( kome::objects::Parameters& params, kome::objects::Variant& obj ) {
	kome::objects::Variant o = obj;
	kome::objects::Variant& aVal = params.getValue( ACTIVE_OBJECT_PARAM_NAME );

	aVal.type = obj.type;
	aVal.prim.pt = obj.prim.pt;
}

// get sample set
kome::objects::SampleSet* PluginCallTool::getSampleSet( kome::objects::Parameters* params ) {
	// get sample
	kome::objects::SampleSet* sampleSet = NULL;

	if( params != NULL ) {
		kome::objects::Variant& sampleSetVal = params->getValue( SAMPLE_SET_PARAM_NAME );
		if( sampleSetVal.type == kome::objects::Variant::SAMPLE_SET ) {
			sampleSet = (kome::objects::SampleSet*)sampleSetVal.prim.pt;
		}
	}

	if( sampleSet == NULL ) {
		LOG_WARN( FMT( "Failed to get the sample set." ) );
	}

	return sampleSet;
}

// get sample
kome::objects::Sample* PluginCallTool::getSample( kome::objects::Parameters* params ) {
	// get sample
	kome::objects::Sample* sample = NULL;

	if( params != NULL ) {
		kome::objects::Variant& sampleVal = params->getValue( SAMPLE_PARAM_NAME );
		if( sampleVal.type == kome::objects::Variant::SAMPLE ) {
			sample = (kome::objects::Sample*)sampleVal.prim.pt;
		}
	}

	if( sample == NULL ) {
		LOG_WARN( FMT( "Failed to get the sample." ) );
	}

	return sample;
}

// get spectrum
kome::objects::Spectrum* PluginCallTool::getSpectrum( kome::objects::Parameters* params ) {
	// get spectrum
	kome::objects::Spectrum* spec = NULL;

	if( params != NULL ) {
		kome::objects::Variant& specParam = params->getValue( SPECTRUM_PARAM_NAME );
		if( specParam.type == kome::objects::Variant::SPECTRUM ) {
			spec = (kome::objects::Spectrum*)specParam.prim.pt;
		}
	}

	if( spec == NULL ) {
		LOG_WARN( FMT( "Failed to get the spectrum." ) );
	}

	return spec;
}

// get chromatogram
kome::objects::Chromatogram* PluginCallTool::getChromatogram( kome::objects::Parameters* params ) {
	// get chromatogram
	kome::objects::Chromatogram* chrom = NULL;

	if( params != NULL ) {
		kome::objects::Variant& chromParam = params->getValue( CHROMATOGRAM_PARAM_NAME );
		if( chromParam.type == kome::objects::Variant::CHROMATOGRAM ) {
			chrom = (kome::objects::Chromatogram*)chromParam.prim.pt;
		}
	}

	if( chrom == NULL ) {
		LOG_WARN( FMT( "Failed to get the chromatogram." ) );
	}

	return chrom;
}

// get xy data
kome::core::XYData* PluginCallTool::getXYData( kome::objects::Parameters* params ) {
	// get xy data
	kome::core::XYData* xyData = NULL;

	if( params != NULL ) {
		kome::objects::Variant& xyParam = params->getValue( XY_DATA_PARAM_NAME );
		if( xyParam.type == kome::objects::Variant::XY_DATA ) {
			xyData = (kome::core::XYData*)xyParam.prim.pt;
		}
	}

	if( xyData == NULL ) {
		LOG_WARN( FMT( "Failed to get the xy data." ) );
	}

	return xyData;
}

// get data set
kome::objects::DataSet* PluginCallTool::getDataSet( kome::objects::Parameters* params ) {
	// get data set
	kome::objects::DataSet* dataSet = NULL;

	if( params != NULL ) {
		kome::objects::Variant& dataSetParam = params->getValue( DATA_SET_PARAM_NAME );
		if( dataSetParam.type == kome::objects::Variant::DATA_SET ) {
			dataSet = (kome::objects::DataSet*)dataSetParam.prim.pt;
		}
	}

	if( dataSet == NULL ) {
		LOG_WARN( FMT( "Failed to get the data set." ) );
	}

	return dataSet;
}

// get spectrum filter
kome::objects::DataSet* PluginCallTool::getSpectraFilter( kome::objects::Parameters* params ){
	// get data set
	kome::objects::DataSet* dataSet = NULL;

	if( params != NULL ) {
		kome::objects::Variant& dataSetParam = params->getValue( FILTER_SPECTR_PARAM_NAME );
		if( dataSetParam.type == kome::objects::Variant::DATA_SET ) {
			dataSet = (kome::objects::DataSet*)dataSetParam.prim.pt;
		}
	}

	if( dataSet == NULL ) {
		LOG_WARN( FMT( "Failed to get the filter spectra." ) );
	}

	return dataSet;
}

// get spectrum group
kome::objects::DataGroupNode* PluginCallTool::getGroup( kome::objects::Parameters* params ) {
	// get group
	kome::objects::DataGroupNode* group = NULL;

	if( params != NULL ) {
		kome::objects::Variant& groupParam = params->getValue( DATA_GROUP_NODE_PARAM_NAME );
		if( groupParam.type == kome::objects::Variant::DATA_GROUP_NODE ) {
			group = (kome::objects::DataGroupNode*)groupParam.prim.pt;
		}
	}

	if( group == NULL ) {
		LOG_WARN( FMT( "Failed to get the spectrum group." ) );
	}

	return group;
}

// get peaks
kome::objects::Peaks* PluginCallTool::getPeaks( kome::objects::Parameters* params ) {
	// get peaks
	kome::objects::Peaks* peaks = NULL;

	if( params != NULL ) {
		kome::objects::Variant& peaksParam = params->getValue( PEAKS_PARAM_NAME );
		if( peaksParam.type == kome::objects::Variant::PEAKS ) {
			peaks = (kome::objects::Peaks*)peaksParam.prim.pt;
		}
	}

	if( peaks == NULL ) {
		LOG_WARN( FMT( "Failed to get the peaks data." ) );
	}

	return peaks;
}

// get parent peaks
kome::objects::Peaks* PluginCallTool::getParentPeaks( kome::objects::Parameters* params ) {
	// get peaks
	kome::objects::Peaks* ppeaks = NULL;

	if( params != NULL ) {
		kome::objects::Variant& ppeaksParam = params->getValue( PARENT_PEAKS_PARAM_NAME );
		if( ppeaksParam.type == kome::objects::Variant::PEAKS ) {
			ppeaks = (kome::objects::Peaks*)ppeaksParam.prim.pt;
		}
	}

	if( ppeaks == NULL ) {
		LOG_WARN( FMT( "Failed to get the parent peaks data." ) );
	}

	return ppeaks;
}

// get 2D peaks
kome::objects::Peaks2D* PluginCallTool::getPeaks2D( kome::objects::Parameters* params ) {
	// get peaks
	kome::objects::Peaks2D* peaks = NULL;

	if( params != NULL ) {
		kome::objects::Variant& peaksParam = params->getValue( PEAKS_2D_PARAM_NAME );
		if( peaksParam.type == kome::objects::Variant::PEAKS_2D ) {
			peaks = (kome::objects::Peaks2D*)peaksParam.prim.pt;
		}
	}

	if( peaks == NULL ) {
		LOG_WARN( FMT( "Failed to get the peaks data." ) );
	}

	return peaks;
}

// get baseline
kome::core::XYData* PluginCallTool::getBaseline( kome::objects::Parameters* params ) {
	// get baseline
	kome::core::XYData* baseline = NULL;
	if( params != NULL ) {
		kome::objects::Variant& baselineParam = params->getValue( BASELINE_PARAM_NAME );
		if( baselineParam.type == kome::objects::Variant::XY_DATA ) {
			baseline = (kome::core::XYData*)baselineParam.prim.pt;
		}
	}

	if( baseline == NULL ) {
		LOG_WARN( FMT( "Failed to get the baseline object." ) );
	}

	return baseline;
}

// get updated xy data
kome::core::XYData* PluginCallTool::getUpdatedXYData( kome::objects::Parameters* params ) {
	// get updated data
	kome::core::XYData* updated = NULL;
	if( params != NULL ) {
		kome::objects::Variant& updatedParam = params->getValue( UPDATED_XY_DATA_PARAM_NAME );
		if( updatedParam.type == kome::objects::Variant::XY_DATA ) {
			updated = (kome::core::XYData*)updatedParam.prim.pt;
		}
	}

	if( updated == NULL ) {
		LOG_WARN( FMT( "Failed to get the updated xy data object." ) );
	}

	return updated;
}

// get updated peaks
kome::objects::Peaks* PluginCallTool::getUpdatedPeaks( kome::objects::Parameters* params ) {
	// get updated data
	kome::objects::Peaks* updated = NULL;
	if( params != NULL ) {
		kome::objects::Variant& updatedParam = params->getValue( UPDATED_PEAKS_PARAM_NAME );
		if( updatedParam.type == kome::objects::Variant::PEAKS ) {
			updated = (kome::objects::Peaks*)updatedParam.prim.pt;
		}
	}

	if( updated == NULL ) {
		LOG_WARN( FMT( "Failed to get the updated peaks object." ) );
	}

	return updated;
}

// get update 2d peaks
kome::objects::Peaks2D* PluginCallTool::getUpdatedPeaks2D( kome::objects::Parameters* params ){
	// get updated data
	kome::objects::Peaks2D* updated = NULL;
	if( params != NULL ){
		kome::objects::Variant& updatedParam = params->getValue( UPDATED_PEAKS_2D_PARAM_NAME );
		if( updatedParam.type == kome::objects::Variant::PEAKS_2D ){
			updated = (kome::objects::Peaks2D*)updatedParam.prim.pt;
		}
	}

	if( updated == NULL ){
		LOG_WARN( FMT( "Failed to get the updated 2d peaks object." ) ); 
	}

	return updated;
}

// get control spectrum
kome::objects::Spectrum* PluginCallTool::getControlSpectrum( kome::objects::Parameters* params ) {
	// get control spectrum
	kome::objects::Spectrum* spec = NULL;
	if( params != NULL ) {
		kome::objects::Variant& controlParam = params->getValue( CONTROL_SPEC_PARAM_NAME );
		if( controlParam.type == kome::objects::Variant::SPECTRUM ) {
			spec = (kome::objects::Spectrum*)controlParam.prim.pt;
		}
	}

	if( spec == NULL ) {
		LOG_WARN( FMT( "Failed to get the control spectrum object." ) );
	}

	return spec;
}

// get control chromatogram
kome::objects::Chromatogram* PluginCallTool::getControlChromatogram( kome::objects::Parameters* params ) {
	// get control chromatogram
	kome::objects::Chromatogram* chrom = NULL;
	if( params != NULL ) {
		kome::objects::Variant& controlParam = params->getValue( CONTROL_CHROM_PARAM_NAME );
		if( controlParam.type == kome::objects::Variant::CHROMATOGRAM ) {
			chrom = (kome::objects::Chromatogram*)controlParam.prim.pt;
		}
	}

	if( chrom == NULL ) {
		LOG_WARN( FMT( "Failed to get the control chromatogram object." ) );
	}

	return chrom;
}

// get control data set
kome::objects::DataGroupNode* PluginCallTool::getControlGroup( kome::objects::Parameters* params ) {
	// get control chromatogram
	kome::objects::DataGroupNode* group = NULL;
	if( params != NULL ) {
		kome::objects::Variant& controlParam = params->getValue( CONTROL_GROUP_PARAM_NAME );
		if( controlParam.type == kome::objects::Variant::DATA_GROUP_NODE ) {
			group = (kome::objects::DataGroupNode*)controlParam.prim.pt;
		}
	}

	if( group == NULL ) {
		LOG_WARN( FMT( "Failed to get the control group object." ) );
	}

	return group;
}

// get control xy data
kome::core::XYData* PluginCallTool::getControlXYData( kome::objects::Parameters* params ) {
	// get control xy data
	kome::core::XYData* xyData = NULL;
	if( params != NULL ) {
		kome::objects::Variant& controlParam = params->getValue( CONTROL_XY_DATA_PARAM_NAME );
		if( controlParam.type == kome::objects::Variant::XY_DATA) {
			xyData = (kome::core::XYData*)controlParam.prim.pt;
		}
	}

	if( xyData == NULL ) {
		LOG_WARN( FMT( "Failed to get the control xy data object." ) );
	}

	return xyData;
}

// get file path
char* PluginCallTool::getPath( kome::objects::Parameters* params ) {
	// get path
	char* path = NULL;

	if( params != NULL ) {
		kome::objects::Variant& pathParam = params->getValue( PATH_PARAM_NAME );
		if( pathParam.type == kome::objects::Variant::STRING ) {
			path = pathParam.getString();
		}
	}

	if( path == NULL ) {
		LOG_WARN( FMT( "Failed to get the file path." ) );
	}

	return path;
}

// get the start of range
bool PluginCallTool::getStartOfRange( kome::objects::Parameters* params, double* start ) {
	// check the parameter
	if( start == NULL ) {
		LOG_ERROR( FMT( "Null pointer." ) );
		return false;
	}

	// get range
	if( params != NULL ) {
		kome::objects::Variant& startVal = params->getValue( START_RANGE_PARAM_NAME );
		if( startVal.type == kome::objects::Variant::DOUBLE ) {
			*start = startVal.prim.dblVal;
			return true;
		}
	}

	LOG_ERROR( FMT( "Failed to get the start of the range." ) );
	return false;
}

// get the end of range
bool PluginCallTool::getEndOfRange( kome::objects::Parameters* params, double* end ) {
	// check the parameter
	if( end == NULL ) {
		LOG_ERROR( FMT( "Null pointer." ) );
		return false;
	}

	// get range
	if( params != NULL ) {
		kome::objects::Variant& endVal = params->getValue( END_RANGE_PARAM_NAME );
		if( endVal.type == kome::objects::Variant::DOUBLE ) {
			*end = endVal.prim.dblVal;
			return true;
		}
	}

	LOG_ERROR( FMT( "Failed to get the end of the range." ) );
	return false;
}

// get range
bool PluginCallTool::getRange( kome::objects::Parameters* params, double* start, double* end ) {
	// get the start of range
	bool s = getStartOfRange( params, start );

	// get the end of range
	bool e = getEndOfRange( params, end );

	return ( s && e );
}

// get progress
kome::core::Progress* PluginCallTool::getProgress( kome::objects::Parameters* params ) {
	// get progress
	kome::core::Progress* progress = &kome::core::Progress::getIgnoringProgress();

	if( params != NULL ) {
		kome::objects::Variant& pgrVal = params->getValue( PROGRESS_PARAM_NAME );
		if( pgrVal.type == kome::objects::Variant::PROGRESS ) {
			progress = (kome::core::Progress*)pgrVal.prim.pt;
		}
	}

	return progress;
}

// get mouse event
kome::evt::MouseEvent* PluginCallTool::getMouseEvent( kome::objects::Parameters* params ) {
	// get mouse event
	kome::evt::MouseEvent* evt = NULL;
	if( params != NULL ) {
		kome::objects::Variant& meVal = params->getValue( MOUSE_EVENT_PARAM_NAME );
		if( meVal.type == kome::objects::Variant::MOUSE_EVT ) {
			evt = (kome::evt::MouseEvent*)meVal.prim.pt;
		}
	}

	if( evt == NULL ) {
		LOG_ERROR( FMT( "Failed to get the mouse event." ) );
	}
	return evt;
}

// get key event
kome::evt::KeyEvent* PluginCallTool::getKeyEvent( kome::objects::Parameters* params ) {
	// get key event
	kome::evt::KeyEvent* evt = NULL;
	if( params != NULL ) {
		kome::objects::Variant& keVal = params->getValue( KEY_EVENT_PARAM_NAME );
		if( keVal.type == kome::objects::Variant::KEY_EVT ) {
			evt = (kome::evt::KeyEvent*)keVal.prim.pt;
		}
	}

	if( evt == NULL ) {
		LOG_ERROR( FMT( "Failed to get the key event." ) );
	}
	return evt;
}

// get size event
kome::evt::SizeEvent* PluginCallTool::getSizeEvent( kome::objects::Parameters* params ) {
	// get size event
	kome::evt::SizeEvent* evt = NULL;
	if( params != NULL ) {
		kome::objects::Variant& seVal = params->getValue( SIZE_EVENT_PARAM_NAME );
		if( seVal.type == kome::objects::Variant::SIZE_EVT ) {
			evt = (kome::evt::SizeEvent*)seVal.prim.pt;
		}
	}

	if( evt == NULL ) {
		LOG_ERROR( FMT( "Failed to get the size event." ) );
	}
	return evt;
}

// get move event
kome::evt::MoveEvent* PluginCallTool::getMoveEvent( kome::objects::Parameters* params ) {
	// get move event
	kome::evt::MoveEvent* evt = NULL;
	if( params != NULL ) {
		kome::objects::Variant& meVal = params->getValue( MOVE_EVENT_PARAM_NAME );
		if( meVal.type == kome::objects::Variant::MOVE_EVT ) {
			evt = (kome::evt::MoveEvent*)meVal.prim.pt;
		}
	}

	if( evt == NULL ) {
		LOG_ERROR( FMT( "Failed to get the move event." ) );
	}

	return evt;
}

// get graphics
kome::img::Graphics* PluginCallTool::getGraphics( kome::objects::Parameters* params ) {
	// get graphics
	kome::img::Graphics* g = NULL;
	if( params != NULL ) {
		kome::objects::Variant& gVal = params->getValue( GRAPHICS_PARAM_NAME );
		if( gVal.type == kome::objects::Variant::GRAPHICS ) {
			g = (kome::img::Graphics*)gVal.prim.pt;
		}
	}

	if( g == NULL ) {
		LOG_ERROR( FMT( "Failed to get the graphics object." ) );
	}

	return g;
}

// get data map
kome::objects::DataMapInfo* PluginCallTool::getDataMapInfo( kome::objects::Parameters* params ) {
	// get data map
	kome::objects::DataMapInfo* dm = NULL;
	if( params != NULL ) {
		kome::objects::Variant& dmVal = params->getValue( DATA_MAP_PARAM_NAME );
		if( dmVal.type == kome::objects::Variant::DATA_MAP ) {
			dm = (kome::objects::DataMapInfo*)dmVal.prim.pt;
		}
	}

	if( dm == NULL ) {
		LOG_ERROR( FMT( "Failed to get the data map object." ) );
	}

	return dm;
}

// get window
wxWindow* PluginCallTool::getWindow( kome::objects::Parameters* params ) {
	// get window
	wxWindow* wnd = NULL;
	if( params != NULL ) {
		kome::objects::Variant& wVal = params->getValue( WINDOW_PARAM_NAME );
		if( wVal.type != kome::objects::Variant::WINDOW ) {
			LOG_ERROR( FMT( "Failed to get the window object." ) );
		}
		else {
			wnd = (wxWindow*)wVal.prim.pt;
		}
	}

	return wnd;
}

// get parent window
wxWindow* PluginCallTool::getParentWindow( kome::objects::Parameters* params ) {
	// get window
	wxWindow* wnd = NULL;
	if( params != NULL ) {
		kome::objects::Variant& wVal = params->getValue( PARENT_WINDOW_PARAM_NAME );
		if( wVal.type == kome::objects::Variant::WINDOW ) {
			wnd = (wxWindow*)wVal.prim.pt;
		}
		else {
			LOG_ERROR( FMT( "Failed to get the parent window object." ) );
		}
	}

	return wnd;
}

// get canvas ID
wxWindow* PluginCallTool::getCanvas( kome::objects::Parameters* params ) {
	// get canvas
	wxWindow* canvas = NULL;
	if( params != NULL ) {
		kome::objects::Variant& cVal = params->getValue( CANVAS_ID_PARAM_NAME );
		if( cVal.type == kome::objects::Variant::WINDOW ) {
			canvas = (wxWindow*)cVal.prim.pt;
		}
		else {
			LOG_ERROR( FMT( "Failed to get the canvas object." ) );
		}
	}

	return canvas;
}

// get parameter values
kome::objects::SettingParameterValues* PluginCallTool::getSettingValues( kome::objects::Parameters* params ) {
	// get setting values
	kome::objects::SettingParameterValues* vals = NULL;
	if( params != NULL ) {
		kome::objects::Variant& sVal = params->getValue( SETTINGS_PARAM_NAME );
		if( sVal.type == kome::objects::Variant::SETTING_VALUES ) {
			vals = (kome::objects::SettingParameterValues*)sVal.prim.pt;
		}
	}

	return vals;
}

// get active object
kome::objects::Variant PluginCallTool::getActiveObject( kome::objects::Parameters* params ) {
	kome::objects::Variant ret;

	if( params != NULL ) {
		ret = params->getValue( ACTIVE_OBJECT_PARAM_NAME );
	}

	return ret;
}

// set boolean value
void PluginCallTool::setBool( kome::objects::Parameters& params, const char* name, const bool val ) {
	kome::objects::Variant& v = params.getValue( name );
	v.type = kome::objects::Variant::BOOL;
	v.prim.boolVal = val;
}

// get boolean value
bool PluginCallTool::getBool( kome::objects::Parameters* params, const char* name, const bool df ) {
	// check parameters
	if( params == NULL || name == NULL || !params->hasParameter( name ) ) {
		return df;
	}

	// get value
	kome::objects::Variant& v = params->getValue( name );
	if( v.type != kome::objects::Variant::BOOL ) {
		return df;
	}
	return v.prim.boolVal;
}

// set integer value
void PluginCallTool::setInt( kome::objects::Parameters& params, const char* name, const int val ) {
	kome::objects::Variant& v = params.getValue( name );
	v.type = kome::objects::Variant::INTEGER;
	v.prim.intVal = val;
}

// get integer value
int PluginCallTool::getInt( kome::objects::Parameters* params, const char* name, const int df ) {
	// check parameters
	if( params == NULL || name == NULL || !params->hasParameter( name ) ) {
		return df;
	}

	// get value
	kome::objects::Variant& v = params->getValue( name );
	if( v.type != kome::objects::Variant::INTEGER ) {
		return df;
	}
	return v.prim.intVal;
}

// set double value
void PluginCallTool::setDouble( kome::objects::Parameters& params, const char* name, const double val ) {
	kome::objects::Variant& v = params.getValue( name );
	v.type = kome::objects::Variant::DOUBLE;
	v.prim.dblVal = val;
}

// get double value
double PluginCallTool::getDouble( kome::objects::Parameters* params, const char* name, const double df ) {
	// check parameters
	if( params == NULL || name == NULL || !params->hasParameter( name ) ) {
		return df;
	}

	// get value
	kome::objects::Variant& v = params->getValue( name );
	if( v.type != kome::objects::Variant::DOUBLE ) {
		return df;
	}
	return v.prim.dblVal;
}

// set string
void PluginCallTool::setString( kome::objects::Parameters& params, const char* name, const char* val ) {
	kome::objects::Variant& v = params.getValue( name );
	v.setString( val );
}

// get string value
const char* PluginCallTool::getString( kome::objects::Parameters* params, const char* name, const char* df ) {
	// check parameters
	if( params == NULL || name == NULL || !params->hasParameter( name ) ) {
		return df;
	}

	// get value
	kome::objects::Variant& v = params->getValue( name );
	if( v.type != kome::objects::Variant::STRING ) {
		return df;
	}

	return v.getString();
}

// set sample getter
void PluginCallTool::setSampleGetter( kome::objects::Parameters& params, kome::ident::SampleGetter& getter ){
	kome::objects::Variant& sampleGetterParam = params.getValue( SAMPLE_GETTER_PARAM_NAME );
	sampleGetterParam.type = kome::objects::Variant::SAMPLE_GETTER;
	sampleGetterParam.prim.pt = &getter;
}

// get sample getter
kome::ident::SampleGetter* PluginCallTool::getSampleGetter( kome::objects::Parameters* params ){
	// get sample getter
	kome::ident::SampleGetter* getter = NULL;

	if( params != NULL ){
		kome::objects::Variant& getterVal = params->getValue( SAMPLE_GETTER_PARAM_NAME );
		if( getterVal.type == kome::objects::Variant::SAMPLE_GETTER ){
			getter = (kome::ident::SampleGetter*)getterVal.prim.pt;
		}
	}

	if( getter == NULL ){
		LOG_WARN( FMT( "Failed to get the sample getter." ) );
	}

	return getter;
}

// Quantitation Window
void PluginCallTool::onQuantitationWindow( wxWindow* parent, const char* name ){
	// parameters
	kome::objects::Parameters params;
	setWindow( params, parent );
	setString( params, name, "" );

	// call functions
	PluginManager& plgMgr = PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, FUNC_TYPE_QUANTITATION_WINDOW );	
}


// set Search ID
void PluginCallTool::setSearchID( kome::objects::Parameters& params, int nSearchID ) {
	kome::objects::Variant& cVal = params.getValue( SEARCH_ID_PARAM_NAME );
	
	cVal.type = kome::objects::Variant::INTEGER;
	cVal.prim.intVal = nSearchID;
}

// get Search ID
int PluginCallTool::getSearchID( kome::objects::Parameters* params ) {
	// get Search ID
	int nSearchID = -1;
	if( params != NULL ) {
		kome::objects::Variant& cVal = params->getValue( SEARCH_ID_PARAM_NAME );
		if( cVal.type == kome::objects::Variant::INTEGER ) {
			nSearchID = cVal.prim.intVal;
		}
		else {
			LOG_ERROR( FMT( "Failed to get the Search ID." ) );
		}
	}

	return nSearchID;
}

// set search spectrum ID
void PluginCallTool::setSearchSpectrumID( kome::objects::Parameters& params, int nSearchSpectrumID ) {
	kome::objects::Variant& cVal = params.getValue( SEARCH_SPECTRUM_ID_PARAM_NAME );
	
	cVal.type = kome::objects::Variant::INTEGER;
	cVal.prim.intVal = nSearchSpectrumID;
}

// get search spectrum ID
int PluginCallTool::getSearchSpectrumID( kome::objects::Parameters* params ) {
	// get search spectrum ID
	int nSearchSpectrumID = -1;
	if( params != NULL ) {
		kome::objects::Variant& cVal = params->getValue( SEARCH_SPECTRUM_ID_PARAM_NAME );
		if( cVal.type == kome::objects::Variant::INTEGER ) {
			nSearchSpectrumID = cVal.prim.intVal;
		}
		else {
			LOG_ERROR( FMT( "Failed to get the Search spectrum ID." ) );
		}
	}

	return nSearchSpectrumID;
}

// set Substance ID
void PluginCallTool::setSubstanceID( kome::objects::Parameters& params, int nSubstanceID ) {
	kome::objects::Variant& cVal = params.getValue( SUBSTANCE_ID_PARAM_NAME );
	
	cVal.type = kome::objects::Variant::INTEGER;
	cVal.prim.intVal = nSubstanceID;
}

// get Substance ID
int PluginCallTool::getSubstanceID( kome::objects::Parameters* params ) {
	// get Substance ID
	int nSubstanceID = -1;
	if( params != NULL ) {
		kome::objects::Variant& cVal = params->getValue( SUBSTANCE_ID_PARAM_NAME );
		if( cVal.type == kome::objects::Variant::INTEGER ) {
			nSubstanceID = cVal.prim.intVal;
		}
		else {
			LOG_ERROR( FMT( "Failed to get the Substance ID." ) );
		}
	}

	return nSubstanceID;
}
