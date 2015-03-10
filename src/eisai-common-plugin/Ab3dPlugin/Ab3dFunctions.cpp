/**
 * @file Ab3dAlignmentTICFunctions.cpp
 * @brief implements of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * @data 2011.03.02 <Add> M.Aihara
 *       add TBB parallel code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dFunctions.h"

#include "Ab3dAlignmentTIC.h"
#include "Ab3dAlignmentMC.h"
#include "Ab3dComposition.h"
#include "Ab3dManager.h"
#include "Ab3dLabeling.h"
#include "Ab3dClustering.h"


// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#include "tbb/task_scheduler_init.h"
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DRAWING_STATUS_KEY				 "DRAWING_AB3D_PEAKS"
#define DRAWING_STATUS_VALUE			   "on"

#define START_RT_PARAM_NAME				"start_rt"
#define END_RT_PARAM_NAME				  "end_rt"
#define START_MZ_PARAM_NAME				"start_mz"
#define END_MZ_PARAM_NAME				  "end_mz"


// execute alignment (TIC)
kome::objects::Variant executeAlignmentByTIC( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;
	ret.prim.pt = NULL;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// create alignment object
	Ab3dAlignmentTIC* ab3d = new Ab3dAlignmentTIC( settings );

	ret.prim.pt = ab3d;
	return ret;
}

// execute alignment (XIC)
kome::objects::Variant executeAlignmentByMC( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;
	ret.prim.pt = NULL;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// create alignment object
	Ab3dAlignmentMC* ab3d = new Ab3dAlignmentMC( settings );

	ret.prim.pt = ab3d;
	return ret;
}

// execute composition
kome::objects::Variant executeComposition( kome::objects::Parameters* params ) {
	// return vale
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::SAMPLE;
	ret.prim.pt = NULL;

	// group
	kome::objects::DataGroupNode* ctrlGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( ctrlGroup == NULL ) {
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );
	if( trmtGroup == NULL ) {
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	kome::objects::SettingParameterValues* vals = kome::plugin::PluginCallTool::getSettingValues( params );

	// alignment
	kome::operation::Alignment* alignment = NULL;
	if( params != NULL ) {
		kome::objects::Variant& var = params->getValue( "alignment" );
		alignment = (kome::operation::Alignment*)var.prim.pt;
	}

	// composition object
	Ab3dComposition composition( *ctrlGroup, *trmtGroup );

	// treatment
	ret.prim.pt = composition.createCompositionData( vals, alignment, progress );

	return ret;
}

// detect 2D peaks
kome::objects::Variant detect2dPeaks( kome::objects::Parameters* params ) {
	// return valeu
	kome::objects::Variant ret;

	// objects
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	kome::objects::Peaks2D* peaks = kome::plugin::PluginCallTool::getPeaks2D( params );
	if( peaks == NULL ) {
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
	task_scheduler_init init;
#endif
// <Add> M.Aihara

	// labeling object
	Ab3dLabeling labeling( settings );
	labeling.setRange(
		kome::plugin::PluginCallTool::getDouble( params, START_RT_PARAM_NAME, -1.0 ),
		kome::plugin::PluginCallTool::getDouble( params, END_RT_PARAM_NAME, -1.0 ),
		kome::plugin::PluginCallTool::getDouble( params, START_MZ_PARAM_NAME, -1.0 ),
		kome::plugin::PluginCallTool::getDouble( params, END_MZ_PARAM_NAME, -1.0 )
	);

	// detect peaks
	labeling.executeLabeling( *dataSet, *peaks, *progress );
	if( !progress->isStopped() ){
		progress->fill();
	}

	return ret;
}

// detect 2D clusters
kome::objects::Variant detect2dClusters( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// objects
	kome::objects::Peaks2D* peaks = kome::plugin::PluginCallTool::getPeaks2D( params );
	if( peaks == NULL ) {
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// clustering object
	Ab3dClustering clustering( settings );

	// clustering
	clustering.detectClusters( *peaks, *progress );
	if( !progress->isStopped() ){
		progress->fill();
	}

	return ret;
}
