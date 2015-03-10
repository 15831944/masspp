/**
 * @file CompositionFunctions.cpp
 * @brief implements of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.02.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CompositionDialog.h"
#include "CompositionManager.h"	
#include "SampleFusionOperation.h"

#include <wx/wx.h>
#include <wx/utils.h>


using namespace kome::composition;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define COMPOSITION_NAME			"composition"
#define COMPOSITION_TYPE_NAME		"COMPOSITION"


// subtract composition
kome::objects::Variant openCompositionDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// dialog
	CompositionDialog dlg( kome::window::WindowTool::getMainWindow() );

	if( dlg.ShowModal() == wxID_OK ) {
		// groups
		kome::objects::DataGroupNode* ctrlGroup = dlg.getCtrlGroup();
		kome::objects::DataGroupNode* trmtGroup = dlg.getTrmtGroup();
		if( ctrlGroup == NULL || trmtGroup == NULL || ctrlGroup == trmtGroup ) {
			LOG_ERROR( FMT( "Failed to get groups." ) );
			return ret;
		}

		// check the composition
		kome::plugin::PluginFunctionItem* composition = dlg.getComposition();
		if( composition == NULL ) {
			LOG_ERROR( FMT( "Failed to get the composition object." ) );
			return ret;
		}
				   
		// ------------------------------
		// Operation’Ç‰Á
		// ------------------------------
		SampleFusionOperation* opt = new SampleFusionOperation();
		// operation name
		opt->setOperationName("Sample Fusion");
		// operation type
		opt->setOperationType( kome::operation::Operation::TYPE_INPUT );
		// short name
		opt->setShortName( "sample_fusion" );

		opt->setCtrlGroup( ctrlGroup );
		opt->setTrmtGroup( trmtGroup );
		opt->setCompositionItem( composition );
		opt->setCompositionSettings( dlg.getCompositionSettings() );
		opt->setAlignmentFuncItem( dlg.getAlignment() );
		opt->setAlignmentSettings( dlg.getAlignmentSettings() );

		if( opt->execute() ){
			ret.prim.boolVal = true;
		}else{
			delete opt;
		}
	}

	ret.prim.boolVal = true;
	return ret;
}

// get sample fusion operation
kome::objects::Variant getSampleFusionOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new SampleFusionOperation();
	
	return ret;
}

