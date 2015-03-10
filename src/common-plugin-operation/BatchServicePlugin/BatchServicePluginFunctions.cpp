/**
 * @file BatchServicePluginFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author okada_h
 * @date 2012.02.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BatchServicePluginFunctions.h"
#include "BatchProcessingMethodConfigurationDialog.h"
#include "JobListCtrl.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// Batch Processing - Method Configuration
kome::objects::Variant batchProcessingMethodConfiguration( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// open dialog
	BatchProcessingMethodConfigurationDialog dlg(
		kome::window::WindowTool::getMainWindow()
	);

	if( dlg.ShowModal() == wxID_OK ) {
		// 処理
		int iii = 0;
	}
	
	return ret;
}


// toggle the visibility of the batch job window
kome::objects::Variant toggleBatchJobWindow( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle the visibility
	bool visible = ( JobListCtrl::m_dlg != NULL && JobListCtrl::m_dlg->IsShown() );

	// delete
	if( JobListCtrl::m_dlg != NULL ) {
		delete JobListCtrl::m_dlg;
		JobListCtrl::m_dlg = NULL;
	}

	// create
	if( !visible ) {
		kome::window::GridListDialog* dlg = new kome::window::GridListDialog(
			kome::window::WindowTool::getMainWindow(),
			"Batch Job List",
			true
		);
		
		JobListCtrl* listCtrl = new JobListCtrl( dlg );
		dlg->Show();
		JobListCtrl::m_dlg = dlg;
	}

	return ret;
}


// the batch job window is shown or not
kome::objects::Variant isVisibleBatchJob( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = ( JobListCtrl::m_dlg != NULL && JobListCtrl::m_dlg->IsShown() );

	return ret;
}




// Template Test


// batch_set BatchInfo*へのポインタを返す
kome::objects::Variant batchSetFunctionTest( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.prim.pt = NULL;	// @date 2013/08/21 <Add> OKADA
		
	// オペレーション取得
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	int ifinished = mgr.getNumberOfFinishedOperations();
	
	// active sample 取得
/*	kome::objects::ActiveObjectsManager& actMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::Sample* acSample = actMgr.getActiveSample();
	if( acSample == NULL ){
		return ret;
	}
*/
	kome::objects::Sample* acSample = kome::plugin::PluginCallTool::getSample( params );
	if( acSample == NULL ){
		return ret;
	}

	// Dummy data
	kome::plugin::BatchInfo* batchInfo = new kome::plugin::BatchInfo();
//	batchInfo->addCall( "db", "[sqlite[[C:\\Users\\okada_h\\.mspp\common5.db]]]" );
	
	batchInfo->addCall( "matrix_standard", "[C:\\dev\\test\\Q0\\yeast5_bsa0_rt15-30.msb]" );
	batchInfo->addCall( "rt", "[1:10000]" );
	batchInfo->addCall( "baseline_subtract", "[linear[2.0]]" );
	
//	batchInfo->addCall( "ident", "[Test,Comment,mascot-pmf-lcms[okada_h,okada_h@shimadzusd.co.jp,0.25,da,999,SwissProt,All entries,Trypsin,1,Carbamidomethyl (C),Oxidation (M),false,1.0,1.2,Da,1+,Monoisotopic,false,AUTO],gion[5,10],intrate]" );
//	batchInfo->addCall( "ident", "[Test20130820_1643,Comment,mascot-mis[Test20130820_1643,okada_h@shimadzusd.co.jp,Vertebrates_EST,All entries,Trypsin,1,,Carbamidomethyl (C),Oxidation (M),false,1.2,Da,0,0.6,Da,2+,Monoisotopic,Default,,AUTO],gion[5,10],intrate]" );

	ret.prim.pt = batchInfo;
	return ret;
}
