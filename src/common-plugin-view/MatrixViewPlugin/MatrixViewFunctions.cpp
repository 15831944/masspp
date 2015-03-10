/**
 * @file MatrixViewFunctions.cpp
 * @brief implements of MatrixView plug-in functions
 *
 * @author H. Parry
 * @date 2011.04.27
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixViewFunctions.h"

#include "MatrixViewManager.h"
#include "PeakMatrixManager.h"
#include "MatrixListCtrl.h"
#include "kome/IdentificationTools.h"

using namespace kome::matrix;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::matrix::view;


// toggle visibility of the dialog
kome::objects::Variant togglePeakMatrixOpened( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// toggle visibility
	mgr.toggleDialog();

	return ret;
}

// judges whether the dialog is opened
kome::objects::Variant isPeakMatrixOpened( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// judge
	ret.prim.boolVal = mgr.isShownDialog();

	return ret;
}

// update matrix dialog
kome::objects::Variant updateMatrixDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// update dialog
	mgr.updateDialog();

	return ret;
}

// open Create Peak Matrix Dialog
kome::objects::Variant openCreatePeakMatrixDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	mgr.openPeakMatrixDialog();
	
	return ret;
}


kome::objects::Variant onExitPeakMatrixDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	mgr.releasePeakMatrixDialog();

	return ret;
}

// on exit GUI
kome::objects::Variant onExitGUI( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// release dialog
	mgr.releaseDialog();

	return ret;
}

// open matrix
kome::objects::Variant openMatrixDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// open matrix
	if( !mgr.isShownDialog() ) {
		mgr.toggleDialog();
	}

	return ret;
}

// close matrix
kome::objects::Variant closeMatrixDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// close matrix
	if( mgr.isShownDialog() ) {
		mgr.toggleDialog();
	}

	return ret;
}

// checks whether can exit the Quantitation dialog
kome::objects::Variant canExitQuantitationDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	ret.prim.boolVal = mgr.canCloseDialog();

	return ret;
}

// select matrix	// #22 [1]-matrix_name
kome::objects::Variant selectPeakMatrix( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "name" );
	std::string name = NVL( n, "" );
	if( name.empty() ) {
		LOG_ERROR( FMT( "Failed to get the matrix name." ) );
		return ret;
	}

	// matrix numbers
	std::vector< int > nums;
	identMgr.getMatrices( nums );

	// check names
	int id = -1;
	for( unsigned int i = 0; i < nums.size() && id < 0; i++ ) {
		std::string tmp = identMgr.getMatrixName( nums[ i ] );
		if( name.compare( tmp ) == 0 ) {
			id = nums[ i ];
		}
	}

	// add matrix
	if( id < 0 ) {
		id = identMgr.addMatrix( name.c_str(), "", true, true, false );
	}

	if( id < 0 ) {
		LOG_ERROR_CODE( FMT( "There is a problem with the Mass++ results database configuration and the matrix could not be added.\nCheck the database configuration (File -> Database…)." ), ERR_OTHER );
		return ret;
	}

	// select matrix
	identMgr.selectMatrix( id );

	ret.prim.boolVal = true;
	return ret;
}

// initialize matrix
kome::objects::Variant initializeMatrix( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix cannot be initialized because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// initialize matrix
	mgr.initMatrix( prj );

	ret.prim.boolVal = true;
	return ret;
}

// initialize matrix row
kome::objects::Variant initializeMatrixRow( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix rows cannot be initialized because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// initialize matrix row
	mgr.initRow( prj );

	ret.prim.boolVal = true;
	return ret;
}

// initialze matrix column
kome::objects::Variant initializeMatrixCol( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix columns cannot be initialized because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// initialize matrix column
	mgr.initCol( prj );

	ret.prim.boolVal = true;
	return ret;
}

// add matrix sample with group
kome::objects::Variant addMatrixSampleWithGroup( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix sample with group cannot be added because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* s = settings->getParameterValue( "raw_data" );
	std::string path = NVL( s, "" );
	if( path.empty() ) {
		LOG_ERROR( FMT( "Failed to get the sample raw data." ) );
		return ret;
	}

	s = settings->getParameterValue( "color" );
	COLOR color = kome::img::ColorTool::getColor( s );

	s = settings->getParameterValue( "group" );
	std::string group = NVL( s, "" );
	if( group.empty() ) {
		LOG_ERROR( FMT( "Failed to get the group name." ) );
		return ret;
	}

	// add sample
	mgr.addSampleSet( path.c_str(), prj, color, group.c_str() );

	ret.prim.boolVal = true;
	return ret;
}

// read sample list file
kome::objects::Variant readSampleListFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix sample list file cannot be used because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// file path
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* s = settings->getParameterValue( "path" );
	if( s == NULL || strlen( s ) == 0 ) {
		LOG_ERROR( FMT( "Failed to get the matrix sample list file path." ) );
		return ret;
	}

	// progress
	printf( "Reading Matrix Sample List Data...\n" );
	kome::core::DisplayProgress progress( "Matrix Sample List Data" );

	// read file
	ret.prim.boolVal = mgr.readSampleList( s, prj, &progress );

	return ret;
}

// set standard file path
kome::objects::Variant setStandardFilePath( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	
	// file path
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* s = settings->getParameterValue( "path" );
	if( s == NULL || strlen( s ) == 0 ) {
		LOG_ERROR( FMT( "Failed to get the standard file path." ) );
		return ret;
	}

	// set standard 
	mgr.setStandard( s );

	ret.prim.boolVal = true;
	return ret;
}

// create matrix row
kome::objects::Variant createMatrixRow( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	ret = setMatrixPeakPositions( params );

	//// managers
	//kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	//PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	//kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	//// selected matrix
	//int prj = identMgr.getSelectedMatrix();
	//if( prj < 0 ) {
	//	LOG_ERROR_CODE( FMT( "Matrix row cannot be created because no matrix exists." ), ERR_OTHER );
	//	return ret;
	//}

	//// detector
	//kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	//if( settings == NULL ) {
	//	LOG_ERROR( FMT( "Failed to get arguments information." ) );
	//	return ret;
	//}

	//const char* s = settings->getParameterValue( "detector" );

	//kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "QUANT_PEAK_POS", s );
	//if( item == NULL ) {
	//	LOG_ERROR( FMT( "Failed to get the peak position detector." ) );
	//	return ret;
	//}

	//// settings
	//kome::objects::SettingParameterValues* peakSettings = settings->getSubParameterValues( "detector", s );
	//if( peakSettings->getNumberOfParameters() == 0 ) {
	//	kome::plugin::SettingsPage* page = item->getSettingsPage();
	//	if( page != NULL ) {
	//		page->setParameters( *peakSettings );
	//	}
	//}

	//// standard group
	//int stdSample = mgr.getStandardGroupId();
	//if( stdSample < 0 ) {
	//	std::vector< int > matrixGroups;
	//	std::vector< int > samples;

	//	identMgr.getGroups( matrixGroups, prj );
	//	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
	//		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	//	}

	//	if( samples.size() > 0 ) {
	//		stdSample = samples[ 0 ];
	//	}
	//}

	//kome::ident::SampleReadInfo readInfo;
	//int sampleId = identMgr.getMatrixSampleSample( stdSample );

	//kome::objects::Sample* sample = NULL;
	//if( stdSample >= 0 ) {
	//	sample = identMgr.openSample( sampleId, readInfo );
	//}

	//// progress
	//printf( "Matrix Peak Positions...\n" );
	//kome::core::DisplayProgress progress( "Matrix Peak Positions" );

	//// call function
	//kome::objects::Parameters posParams;
	//kome::plugin::PluginCallTool::setSettingValues( posParams, *peakSettings );
	//if( sample != NULL ) {
	//	kome::plugin::PluginCallTool::setControlGroup( posParams, *( sample->getRootDataGroupNode() ) );
	//}
	//kome::plugin::PluginCallTool::setProgress( posParams, progress );

	//item->getCall()->invoke( &posParams );
	//progress.fill();


	//// release
	//identMgr.releaseSample( sample, readInfo );

	//ret.prim.boolVal = true;
	return ret;
}

// calculate peak values   // 「Peak Values Caluculation」パネル
kome::objects::Variant calculatePeakValues( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

//// ###### Warning !!! ###################
//// 検証の関係でここでリターンをかけています。
//// 問題解決後、本ブロックを削除して下さい。
//// 7.18 コミット時に交じっていたのでコメントを添えて、
////	コメントアウトしました(M.F)
//
//	ret.prim.boolVal = true;
//	return ret;
//
//// ######################### ここまで ####
//
//	// managers
//	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
//	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
//	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
//
//	// selected matrix
//	int prj = identMgr.getSelectedMatrix();
//	if( prj < 0 ) {
//		LOG_ERROR_CODE( FMT( "Fill Matrix processing cannot be performed because no matrix exists." ), ERR_OTHER );
//		return ret;
//	}
//
//	// detector
//	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params ); // page上に設定されたパラメータを読んでくる
//	if( settings == NULL ) {
//		LOG_ERROR( FMT( "Failed to get arguments information." ) );
//		return ret;
//	}
//
//	const char* s = settings->getParameterValue( "alg" );
//
//	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "QUANT_PEAK_VAL", s );  // "searchSpectrumPeak""searchChromtgrampeak"のどちらかを読んでくる
//	if( item == NULL ) {
//		LOG_ERROR( FMT( "Failed to get the filling algorithm." ) );
//		return ret;
//	}
//
//	// settings 
//	kome::objects::SettingParameterValues* peakSettings = settings->getSubParameterValues( "alg", s );
//	if( peakSettings->getNumberOfParameters() == 0 ) {
//		kome::plugin::SettingsPage* page = item->getSettingsPage();
//		if( page != NULL ) {
//			page->setParameters( *peakSettings );
//		}
//	}
//
//	// tolerance
//	double rtTol = settings->getDoubleValue( "rt_tol", -1.0 );
//	if( rtTol < 0.0 ) {
//		LOG_ERROR( FMT( "Failed to get the RT tolerance." ) );
//		return ret;
//	}
//
//	double mzTol = settings->getDoubleValue( "mz_tol", -1.0 );
//	if( mzTol < 0.0 ) {
//		LOG_ERROR( FMT( "Failed to get the m/z tolerance." ) );
//		return ret;
//	}
//
//	// progress
//	printf( "Fill Matrix...\n" );
//	kome::core::DisplayProgress progress( "Fill Matrix" );
//
//	// groups
//	std::vector< int > matrixGroups;
//	std::vector< int > samples;
//	identMgr.getGroups( matrixGroups, prj );
//	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
//		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );   // 各グループに属するサンプルを登録
//	}
//
//	int stdSample = mgr.getStandardGroupId();
//	if( stdSample < 0 ) {
//		if( samples.size() > 0 ) {
//			stdSample = samples[ 0 ];
//		}
//	}
//
//	const char* supplement_method= settings->getParameterValue( "supplement"); // @date 2013/08/28 <Add> FUJITA
//
//	// PeakPositionクラスの引数追加
//	// fill   // @date 2013/08/28 <Add> FUJITA supplement_method引数追加
//	ret.prim.boolVal = mgr.fillMatrix( item, peakSettings, rtTol, mzTol, true, samples, stdSample, &progress, supplement_method, NULL,false );   // このメソッド内でfill matrixする

	ret = setMatrixPeakValues( params );
			
	return ret;
}

// output peak matrix
kome::objects::Variant outputPeakMatrix( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		LOG_ERROR_CODE( FMT( "Matrix cannot be exported because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// file path
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* s = settings->getParameterValue( "path" );
	if( s == NULL || strlen( s ) == 0 ) {
		LOG_ERROR( FMT( "Failed to get the output file path." ) );
		return ret;
	}

	// output
	ret.prim.boolVal = mgr.outputMatrix( prj, s );

	return ret;
}

// refresh window
kome::objects::Variant refreshWindow( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	mgr.refreshDialog();

	return ret;
}

// activate matrix peak
kome::objects::Variant activateMatrixPeak( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// peak ID
	int peak = kome::plugin::PluginCallTool::getInt( params, "peak", -1 );
	mgr.activatePeak( peak );

	return ret;
}

// add matrix group	// #22 (4)-matrix_group
kome::objects::Variant addMatrixGroup( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "group" );
	std::string group = NVL( n, "" );
	if( group.empty() ) {
		LOG_ERROR( FMT( "Failed to get the matrix group." ) );
		return ret;
	}
//	std::string name = group;	// group nameなので

	n = settings->getParameterValue( "color" );
	std::string color = NVL( n, "" );
	if( color.empty() ) {
		LOG_ERROR( FMT( "Failed to get the sample color." ) );
		return ret;
	}

	/* ピークマトリクスのグループを追加する。既に同じ名前のグループが存在する場合は色を更新する。 */

	// 既に同じ名前のグループが存在する?

	// IdentificationManager::addGroup()
	// 新規の場合は（）内はfalse
	// 既存の場合は（）内はtrue

	return addMatrixGroupString( group, color );
}


kome::objects::Variant addMatrixGroupString( std::string strGroup, std::string strColor ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	int selected_matrix_id = identMgr.getSelectedMatrix();

	if( selected_matrix_id < 0 ) {
		LOG_ERROR_CODE( FMT( "Selected matrix is nothing" ), ERR_OTHER );
		return ret;
	}

	COLOR colorCode = kome::img::ColorTool::getColor( strColor.c_str() ) ;	// ex. "#0000FF" = blue, "silver"等も指定可

	int nGroupId = -1;
	GroupIdTable& ref_map_group_id = mgr.getMapGroupId();

	bool flgExistGroup;
	GroupIdTable::iterator retGroup = ref_map_group_id.find( strGroup );
	if( retGroup != ref_map_group_id.end() ){
		flgExistGroup = true;	// groupが存在する場合
	}else{
		flgExistGroup = false;	// groupが存在しない場合
	}

	if( flgExistGroup ){
		// groupが存在する場合
		nGroupId = ref_map_group_id[ strGroup.c_str() ];
		identMgr.updateGroup( nGroupId, strGroup.c_str(), colorCode );
	}else{
		// groupが存在しない場合
		nGroupId = identMgr.addGroup( strGroup.c_str(), selected_matrix_id, colorCode );
		ref_map_group_id[ strGroup.c_str() ] = nGroupId;
	}

	if( nGroupId < 0 ) {
		LOG_ERROR( FMT( "Failed to add group." ) );
		return ret;
	}

	ret.prim.boolVal = true;
	return ret;
}

// add matrix view sample // #22 (5)-matrix_sample
kome::objects::Variant addMatrixViewSample( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// file factory
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "group" );
	std::string group = NVL( n, "" );
	if( group.empty() ) {
		LOG_ERROR( FMT( "Failed to get the matrix group." ) );
		return ret;
	}

	n = settings->getParameterValue( "path" );	// 存在するファイルのパスが入る
	std::string path = NVL( n, "" );
	if( path.empty() ) {
		LOG_ERROR( FMT( "Failed to get the sample file path." ) );
		return ret;
	}

	n = settings->getParameterValue( "color" );
	std::string color = NVL( n, "" );
	if( color.empty() ) {
		LOG_ERROR( FMT( "Failed to get the sample color." ) );
		return ret;
	}

	addMatrixViewSampleString( group, path, color );

	ret.prim.boolVal = true;
	return ret;
}


// add matrix view sample string
kome::objects::Variant addMatrixViewSampleString( std::string strGroup, std::string strPath, std::string strColor )
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// file factory
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	COLOR colorCode = kome::img::ColorTool::getColor( strColor.c_str() ) ;	// ex. "#0000FF" = blue, "silver"等も指定可

/* ピークマトリクスのサンプルを追加する。
指定したグループが存在しない場合は新たにグループも追加する。この時のグループの色はサンプルと同じものとする。既に同じサンプルが存在する場合は無効とする。
Standard Sample が指定されていない場合はここで指定されたサンプルをStandard Sample とする。
 */

	GroupIdTable& ref_map_group_id = mgr.getMapGroupId();

	size_t size = ref_map_group_id.size();
	GroupIdTable::iterator retGroup = ref_map_group_id.find( strGroup );

	bool flgExistGroup;
	if(size <= 0){
		flgExistGroup = false;	// 指定したグループが存在しない場合
	}else if( retGroup != ref_map_group_id.end() ){
		flgExistGroup = true;
	}else{
		flgExistGroup = false;	// 指定したグループが存在しない場合
	}

	if( flgExistGroup == false ){
		addMatrixGroupString( strGroup.c_str(), strColor.c_str() );
		flgExistGroup = true;
	}

	// 要素を出力する
	GroupIdTable::iterator it = ref_map_group_id.begin();
	while( it != ref_map_group_id.end() )
	{
		if( it->first == strGroup ){
			// OK
		}else{
			// NG
			++it;
			continue;
		}

		// @date 2013/07/22 <Add> OKADA
		// 参考元：GroupSettingsPanel::SavePanelSettings()
		bool opened0 = false;
		kome::objects::SampleSet* ss = factory.searchFile( strPath.c_str() );
		if( ss == NULL ) {
			ss = factory.openFile( strPath.c_str() );
		}
		else {
			opened0 = true;
		}
		
		if( ss != NULL ) {

			//
			for( unsigned int i = 0; i < ss->getNumberOfSamples(); i++ ) {
				kome::objects::Sample* s = ss->getSample( i );
				int sampleId = identMgr.addSample( s );
				std::string sampleName = s->getName();
				if( !sampleName.empty() ) {
					sampleName.append( " : " );
				}
				sampleName.append( getfilename( strPath.c_str() ) );
				if( sampleId >= 0 ) {


					// 既に同じサンプルが存在する場合は無効とする。
					//////////
					{	// sample id check
						int nGroupId = -1;
						nGroupId = ref_map_group_id[ strGroup.c_str() ];

						// delete matrix samples
						std::vector< int > matrixSampleIDs;
						identMgr.getMatrixSamplesFromGroup( matrixSampleIDs, nGroupId );
						bool flgSame = false;
						for( unsigned int i = 0; i < matrixSampleIDs.size(); i++ ) {
							int registeredSampleId = identMgr.getMatrixSampleSample( matrixSampleIDs[ i ] );
							if( registeredSampleId == sampleId ){
								// same id
								flgSame = true;// 既に同じサンプルが存在する……
							}
						}
						if( flgSame ){
							continue;	// 既に同じサンプルが存在する場合は無効とする。
						}
					}

					int nMatrixSampleID = identMgr.addMatrixSample( (*it).second, sampleId, sampleName.c_str(), colorCode );

					{	// Standard Sample が指定されていない場合はここで指定されたサンプルをStandard Sample とする。
						int selected_matrix_id = identMgr.getSelectedMatrix();
						int nStandardSample = identMgr.getStandardSample( selected_matrix_id );

						if( nStandardSample < 0 ){
							// standard Sample が設定されていない
							identMgr.setStandardSample( selected_matrix_id, nMatrixSampleID );	// @date 2013/08/06 <Add> OKADA
						}

						int nStandardSampleDummy = identMgr.getStandardSample( selected_matrix_id );
					}
				}
			}
		}

		if( !opened0 ) {
			factory.closeFile( ss );
		}

		++it;
	}

	ret.prim.boolVal = true;
	return ret;
}

// add matrix view samples	// #22 (6)-matrix_samples
kome::objects::Variant addMatrixViewSamples( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "path" );	// 存在するファイルのパスが入る
	std::string path = NVL( n, "" );
	if( path.empty() ) {
		LOG_ERROR( FMT( "Failed to get the sample(csv) file path." ) );
		return ret;
	}

	//////////
	FILE *fp;
	const int nBufferSize = 1024;
    char buf[nBufferSize];    

	fp = fopen( path.c_str(), "r");
	if( fp == NULL ){
		LOG_ERROR( FMT( "Failed to open the sample(csv) file." ) );
		return ret;
	}
    // １行ずつ読み込む
	while (fgets(buf, nBufferSize-1, fp) != NULL) {        
		/* コンマ、改行で分割 */
		std::string strGroup;
		std::string strPath;
		std::string strColor;

		const char* SEPARATOR = "\t\n";

		strGroup = strtok(buf,  SEPARATOR);
		strPath  = strtok(NULL, SEPARATOR);       
		strColor  = strtok(NULL, SEPARATOR);       

		addMatrixViewSampleString( strGroup, strPath, strColor );

	}    
	fclose(fp);
	//////////

	ret.prim.boolVal = true;
	return ret;
}

// add matrix normalization filter intensity	// #22 (7)-matrix_norm_filter_int
kome::objects::Variant setMatrixNormFilterInt( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "type" );
	std::string type = NVL( n, "" );
	if( type.empty() ) {
		LOG_ERROR( FMT( "Failed to get the type." ) );
		return ret;
	}
	n = settings->getParameterValue( "threshold" );	// "int_range"->"threshold"
	std::string int_range = NVL( n, "" );
	if( int_range.empty() ) {
		LOG_ERROR( FMT( "Failed to get the range." ) );
		return ret;
	}

	peak_filter_t& peak_filter_info = mgr.getPeakFilterInfo();

	mgr.setPeakFilterInfoIntValType( type.c_str() );
	mgr.setPeakFilterInfoIntensityRange( int_range.c_str() );

	ret.prim.boolVal = true;
	return ret;
}

// add matrix group	// #22 (8)-matrix_norm_filter_rt
kome::objects::Variant setMatrixNormFilterRt( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "rt_range" );
	std::string rt_range = NVL( n, "" );
	if( rt_range.empty() ) {
		LOG_ERROR( FMT( "Failed to get the RT range." ) );
		return ret;
	}

	mgr.setPeakFilterInfoRTRange( rt_range.c_str() );

	ret.prim.boolVal = true;
	return ret;
}

// add matrix group	// #22 (9)-matrix_norm_filter_mz
kome::objects::Variant setMatrixNormFilterMz( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "mz_range" );
	std::string mz_range = NVL( n, "" );
	if( mz_range.empty() ) {
		LOG_ERROR( FMT( "Failed to get the m/z range." ) );
		return ret;
	}

	mgr.setPeakFilterInfoMzRange( mz_range.c_str() );

	ret.prim.boolVal = true;
	return ret;
}


#define NORMALIZ_PEAK_DETECT			"normaliz_peak_detect"
#define NORMALIZ_FILTER					"normalization_filter"

// add matrix group	// #22 (10)-matrix_norm_spec
kome::objects::Variant execMatrixNormSpec( kome::objects::Parameters* params ) {
	const char* detect = "peak_spec";
	return execMatrixNormSpec2D( params, detect );
}

// add matrix group	// #22 (11)-matrix_norm_2d
kome::objects::Variant execMatrixNorm2D( kome::objects::Parameters* params ) {
	const char* detect = "peak_2d";
	return execMatrixNormSpec2D( params, detect );
}

// exec matrix norm spec 2D
kome::objects::Variant execMatrixNormSpec2D( kome::objects::Parameters* params, const char* detect )
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "peak_detector" );
	std::string peak_detector = NVL( n, "" );
	if( peak_detector.empty() ) {
		LOG_ERROR( FMT( "Failed to get the peak_detector." ) );
		return ret;
	}

	n = settings->getParameterValue( "methods" );
	std::string methods = NVL( n, "" );
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	// 参考元：NormalizationPanel::SavePanelSettings()

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected project
	int selected_matrix_id = identMgr.getSelectedMatrix();

	// ア
	kome::plugin::PluginFunctionItem* m_peakDetectFunc = plgMgr.getFunctionItem( NORMALIZ_PEAK_DETECT, detect );

	if( m_peakDetectFunc == NULL ){
		LOG_ERROR( FMT( "Failed to get the function of PluginManager." ) );
		return ret;
	}

	kome::plugin::SettingsPage* page = m_peakDetectFunc->getSettingsPage();
	kome::objects::SettingParameterValues peakDetectSettings;

	// イ
	// peak detect settings 
	kome::plugin::SettingsPage* pPage = NULL;
	
	if( m_peakDetectFunc == NULL ){
		pPage = NULL;
	}else{
		pPage = m_peakDetectFunc->getSettingsPage();
	}

	std::string strMethods;
	std::string strActStrSub;

	// サブParameterがあるか確認
	if( pPage != NULL ){
		bool flgSubParam = false;


		unsigned int uiNum = settings->getNumberOfParameters();
		for( unsigned int i=0; i<uiNum; i++ ){
			flgSubParam = false;

			const char* n = settings->getParameterName( i );	// 例：n = "peak_detector"
			const char* v = settings->getParameterValue( i );	// 例：v = "gion"

			std::string strActStr = "";

			if(	( pPage->getForm( i )->getParam()->getValue()->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) )
			{
				// サブParameterがある可能性がある
				const char* szType = pPage->getForm( i )->getParam()->getValue()->getEnumKey();	// szType =  "SPEC_PEAK_PICKING"

				kome::plugin::PluginFunctionItem* pItemSub = plgMgr.getFunctionItem( szType, v );

				if( pItemSub == NULL ){
					// サブParameterは無い
				}else{
					kome::plugin::SettingsPage * sSubPage = pItemSub->getSettingsPage();

					if( sSubPage != NULL ){
						kome::objects::SettingParameterValues* pSettingsSub = settings->getSubParameterValues( n, v );
						if( pSettingsSub != NULL ){
							// サブParameterがある
							flgSubParam =true;
							std::string strSettings = sSubPage->getParameterString( *pSettingsSub );

							strActStr = v;
							strActStr.append( strSettings );

							strActStrSub = strSettings;

							if( i == 0 ){
//								pPage->setParameterString( peakDetectSettings, "gion[5,15]" );	// 例
								pPage->setParameterString( peakDetectSettings, strActStr.c_str() );	// 例
							}
						}
					}
				}
			}

			// normalization method
			if( 0 < i ){

				if( 2 <= i ){
					strMethods.append( "," );
				}

				if( flgSubParam == true ){
					// サブParameterあり;
					strMethods.append( v );
					strMethods.append( strActStrSub );
				}else{
					// サブParameterなし
					//strActStr
					strMethods.append( v );	// 例 v = "int_std_multi_sample[]"
				}
			}
			strActStrSub = "";
		}
	}

	kome::plugin::PluginFunctionItem* m_filterThreshold = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_threshold" );
	kome::plugin::PluginFunctionItem* m_filterRt = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_rt_range" );
	kome::plugin::PluginFunctionItem* m_filterMz = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_mz_range" );

	// manager
	MatrixViewManager& mgrMatrixView = MatrixViewManager::getInstance();

	peak_filter_t& peak_filter_info = mgrMatrixView.getPeakFilterInfo();

	kome::objects::SettingParameterValues settingsInt;
	const char* nameInt = "int_range";
	settingsInt.setValue( (const char*)nameInt, peak_filter_info.strIntensityRange.c_str() );

	const char* nameThreshold = "threshold";	// Threshold	// @date 2013/08/06 <Add> OKADA
	settingsInt.setValue( (const char*)nameThreshold, peak_filter_info.strIntensityRange.c_str() );

	const char* nameType = "type";	// Threshold	// @date 2013/08/06 <Add> OKADA
	settingsInt.setValue( (const char*)nameType, peak_filter_info.strIntensityValueType.c_str() );

	kome::objects::SettingParameterValues settingsRT;
	const char* nameRT = "rt_range";
	settingsRT.setValue( nameRT, peak_filter_info.strRTRange.c_str() );

	kome::objects::SettingParameterValues settingsMz;
	const char* nameMz = "mz_range";
	settingsMz.setValue( nameMz, peak_filter_info.strMzRange.c_str() );

	// normalization method
	const char* normalization = strMethods.c_str();

	std::vector< std::string > normalizArry;
	kome::plugin::SettingsValue::separateListValue( normalization, normalizArry );

	if( normalizArry.empty() ){
		LOG_ERROR( FMT( "Failed to get the normalization methods." ) );
		return ret;
	}

	/** nomalization function item */
	std::vector< kome::plugin::PluginFunctionItem* > m_normaliz;
	/** nomalization settings */
	std::vector< kome::objects::SettingParameterValues > m_normalizSettings;

	for( unsigned int i=0; i < normalizArry.size(); i++ ){
		std::string name = ( normalizArry[i].c_str() );
				
		int ifind = name.find_first_of("[");
		std::string strItem =  name.substr( 0, ifind );
		std::string strParam = name.substr( ifind );
	
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "normalization_method", strItem.c_str() );

		if( item != NULL ){
			kome::objects::SettingParameterValues settings;
			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ){
				page->setParameterString( settings, strParam.c_str() );
			}

			m_normaliz.push_back( item );
			m_normalizSettings.push_back( settings );

		}
	}

	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, selected_matrix_id );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[i] );
	}

	// standard group ID	
	int m_stdId = -1;
	m_stdId = identMgr.getStandardSample( selected_matrix_id );

	// clear peaks
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, selected_matrix_id );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		identMgr.deleteMatrixPeak( peaks[ i ] );
	}

	if( ( m_normaliz.size() <= 0 ) || ( m_normalizSettings.size() <= 0 ) ){
		LOG_ERROR( FMT( "Failed to get the normalize method(size 0)." ) );
		return ret;
	}

	int nStandardSample = identMgr.getStandardSample( selected_matrix_id );

	bool bSuccess = mgr.normalizations( 
		m_peakDetectFunc, &peakDetectSettings,
		m_filterThreshold, &settingsInt,
		m_filterRt, &settingsRT,
		m_filterMz, &settingsMz,
		&(m_normaliz[0]), &( m_normalizSettings[0] ),
		samples,
		m_normaliz.size(),
		nStandardSample,
		NULL );

	ret.prim.boolVal = bSuccess;
	return ret;
}


#define SCALING_NAME				"scaling"
#define SCALING_TYPE_NAME		    "spectrum_scaling_for_rt_alignment"
#define ALIGNMENT_LONG_NAME	  "AB3D Alignment (TIC Pattern)"  // RT アライメントとしてこれが選択されている場合のみスペクトルのスケーリングを行う。

// add matrix group	// #22 (12)-matrix_rt_alignment
kome::objects::Variant execMatrixRtAlignment( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "scaling" );
	std::string strScaling = NVL( n, "" );
	if( strScaling.empty() ) {
		LOG_ERROR( FMT( "Failed to get the scaling." ) );
//		return ret;		// @Date:2013/09/09	<Changed>	A.Ozaki
	}

	n = settings->getParameterValue( "methods" );
	std::string strMethods = NVL( n, "" );
	if( strMethods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}
	
	/** settings of scaling **/
	kome::plugin::PluginFunctionItem* scaling_item;

	/** scaling setting parameter */
	kome::objects::SettingParameterValues* scaling_settings;

	/** settings of alignment **/
	kome::plugin::PluginFunctionItem* alignment_item;

	/** alignment setting parameter */
	kome::objects::SettingParameterValues* alignment_settings;

	const char* scaling = strScaling.c_str();

	scaling_item = plgMgr.getFunctionItem( SCALING_TYPE_NAME, scaling );
	scaling_settings = settings->getSubParameterValues( SCALING_NAME, scaling );

	// アライメントの設定
	const char* alignment = strMethods.c_str();

	alignment_item = plgMgr.getFunctionItem( "alignment", alignment );
	alignment_settings = settings->getSubParameterValues( "methods", alignment );
	
	if( scaling_item != NULL ){	// (None)が選択されていれば、スケーリングを行わない。

		int selected_matrix_id = identMgr.getSelectedMatrix();

		// groups   NormalizationPanel::SavePanelSettings のl248からコピー
		std::vector< int > matrixGroups;
		std::vector< int > samples;
		identMgr.getGroups( matrixGroups, selected_matrix_id );
		for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
			identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
		}

		// PeakMatrixManager::Normalizations　からコピー(l777付近) したものを拡張
		for( unsigned int i = 0; i < samples.size(); i++ ) {
			// sample
			int matrixSampleId = samples[ i ];
			int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );

			kome::ident::SampleReadInfo readInfo;
			kome::objects::Sample* sample = identMgr.openSample( sampleId, readInfo );
			// 下記を追加
			
			if ( sample != NULL ) {   // エラー処理

				kome::objects::DataGroupNode* sampleGroup = sample->getGroup(0);           // 0で決め打ち
		
				kome::objects::Parameters params;
	
				kome::plugin::PluginCallTool::setSample( params, *sample );           // sampleの情報をparamsに入れる  
				kome::plugin::PluginCallTool::setSettingValues( params, *scaling_settings ); 
	
				// TICアライメントならば、スケーリングを行う
				const char* alignment_name = alignment_item->getLongName();

				kome::plugin::PluginCallTool::setGroup( params, *sampleGroup );
				if ( 0 == strcmp( alignment_name, ALIGNMENT_LONG_NAME) ){         // TICアライメント       
					kome::objects::Variant var = scaling_item->getCall()->invoke( &params );	     // スケーリング実行
					sample = (kome::objects::Sample*)var.prim.pt;
				}
			}   // エラー処理用
		}
	}
		
	// アライメント実行 
	kome::plugin::PluginFunctionItem* item = alignment_item;

	int selected_matrix_id = identMgr.getSelectedMatrix();

	const char* name = (item != NULL ? item->getShortName() : "" );
	kome::plugin::SettingsPage* page = (item != NULL ? item->getSettingsPage() : NULL );
	std::string strParam;
	if( page != NULL ){
		strParam = page->getParameterString( *alignment_settings );
	}

	// set Alignment	
	identMgr.setAlignment( selected_matrix_id, name, strParam.c_str() );
	

	ret.prim.boolVal = true;
	return ret;
}

// paring kakko ( parameter value no sotogawa no kakko[] wo kezuri masu. ex; ("MzRange","[400:1200]")->("MzRange","400:1200") )
void paringKakko( kome::objects::SettingParameterValues* settings ){

	static int nDepthDebu;
	nDepthDebu++;

	if( settings != NULL ){
		unsigned int uNumberOfSubParam = settings->getNumberOfParameters();
		for( unsigned int index=0; index<uNumberOfSubParam; index++ ){
			const char * pszName = settings->getParameterName( index );
			const char * pszValue= settings->getParameterValue(index );

			bool bRet = settings->hasSubParameterValues( pszName, pszValue );
			if( bRet ){			
				kome::objects::SettingParameterValues* settingsTempSub = settings->getSubParameterValues( pszName, pszValue );

				paringKakko( settingsTempSub );
				int ddd = 0;

			}else{
				// sub parameter ga nai
			}
			
//			if( !bRet )	// この行を有効にすれば、サブパラメータだけを書き換えるようになります
			{
				// pszValueの先頭と末尾に[]があれば、セットで1組削除
				bool hasKakko = false;
				size_t uLength = strlen( pszValue );
				if( 2 <= uLength ){
					if(	( pszValue[0] == '[' ) && ( pszValue[uLength-1] == ']' ) ){
						hasKakko = true;	// []があった
					}
				}
				
				std::string strNewVal = pszValue;
				if( hasKakko ){
					std::string strOrg = pszValue;
					std::string strWithoutKakko;
					strWithoutKakko = strOrg.substr( 1, uLength-2 );	// []内を取り出す

					strNewVal = strWithoutKakko;
					settings->setValue( pszName, strNewVal.c_str() );	// 更新	
				}

				int ddd = 0;
			}
		}
	}

	nDepthDebu--;

	// kono function wo tukau mae ni,
	// kome::objects::SettingParameterValues settingsCopy( *settings );
	// no youni sureba, motono settings wo henkou sezuni tukae masu.
}

// Batch
#define PEAK_POSITION_PLUGIN_TYPE		"QUANT_PEAK_POS"
#define PEAK_POSITION_PARAM_NAME		"peak position"
#define INIT_FLG_PARAM_NAME				"initialize"

// add matrix group	// #22 (13)-matrix_peak_positions
kome::objects::Variant setMatrixPeakPositions( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
//	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "methods" );
	std::string methods = NVL( n, "" );
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	puts("Matrix Peak Positions...");

	/** peak position function item */
	kome::plugin::PluginFunctionItem* m_peakPosItem;
										
	/** peak position settings */
	kome::objects::SettingParameterValues* m_peakPosSettings;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int selected_matrix_id = identMgr.getSelectedMatrix();

	// getting peak position method
	m_peakPosItem = NULL;

	const char* peakPos = methods.c_str();	// 例：peakPos = "merge"

	m_peakPosItem = plgMgr.getFunctionItem( PEAK_POSITION_PLUGIN_TYPE, peakPos );
	if( m_peakPosItem == NULL ) {
		LOG_ERROR( FMT( "Select a peak position method." ) );
		return ret;
	}

	// peak position settings
	m_peakPosSettings = NULL;
	m_peakPosSettings = settings->getSubParameterValues( PEAK_POSITION_PARAM_NAME, peakPos );

	paringKakko( settings );	// @date 2013/09/10 <Add>  OKADA

	// initialize flag
	bool m_initFlg = settings->getBoolValue( INIT_FLG_PARAM_NAME, true );

	// standard group ID
	int m_stdId = -1;

	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, selected_matrix_id );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}
		
	m_stdId = identMgr.getStandardSample( selected_matrix_id );

	// clear peaks
	if( m_initFlg ) {
		// peaks
		std::vector< int > peaks;
		identMgr.getMatrixPeaks( peaks, selected_matrix_id );
		for( unsigned int i = 0; i < peaks.size(); i++ ) {
			identMgr.deleteMatrixPeak( peaks[ i ] );
		}
	}
	
	// clear peaks
	if( m_initFlg ) {
		// peaks
		std::vector< int > peaks;
		identMgr.getMatrixPeaks( peaks, selected_matrix_id );
		for( unsigned int i = 0; i < peaks.size(); i++ ) {
			identMgr.deleteMatrixPeak( peaks[ i ] );
		}
	}

	// Always delete the added columns, so that previous Peak Analysis results are deleted.
	std::vector< int > columns;
	identMgr.getMatrixColumns( columns, selected_matrix_id );
	for( unsigned int i = 0; i < columns.size(); i++ ) {
		identMgr.deleteMatrixColumn( columns[ i ] );
	}

	// function
	kome::plugin::PluginFunctionItem* item = m_peakPosItem;

	// standard group
	kome::ident::SampleReadInfo readInfo;
	kome::objects::Sample* stdSample = NULL;
	int stdSampleId = identMgr.getMatrixSampleSample( m_stdId );
	if( stdSampleId >= 0 ) {
		stdSample = identMgr.openSample( stdSampleId, readInfo );
	}

	// create sample getter
	kome::ident::SampleGetter* getter = new kome::ident::SampleGetter();

	getter->setMatrix( selected_matrix_id );

	kome::operation::Alignment* alignment = identMgr.getAlignment( selected_matrix_id );
	getter->setAlignment( alignment );
	getter->setStdSample( stdSample );
	
	// Peak Position Detector object
	kome::ident::PeakPositions* pPosObj = new kome::ident::PeakPositions();
			
	// progress
	kome::core::DisplayProgress progress( "matrix peak positions" );	// @date 2013/08/08 <Add> OKADA
	
	// parameters
	kome::objects::Parameters params2;
	
	kome::objects::SettingParameterValues* peakDetectorSettings = NULL;	// @date 2013/08/06 <Add> OKADA
	peakDetectorSettings = settings->getSubParameterValues( methods.c_str() );	// @date 2013/08/06 <Add> OKADA

	kome::objects::SettingParameterValues* peakDetectorSettings2 = NULL;	// @date 2013/08/06 <Add> OKADA
	peakDetectorSettings2 = settings->getSubParameterValues( "methods" );	// @date 2013/08/06 <Add> OKADA

	kome::plugin::PluginCallTool::setSettingValues( params2, *peakDetectorSettings2 );	// @date 2013/08/06 <Add> OKADA

	if( peakDetectorSettings2 != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params2, *peakDetectorSettings2 );	// @date 2013/08/06 <Add> OKADA
	}

	// Peak Postion Detector object
	kome::ident::PeakPositionsDetector* peakPosDetect = NULL;
	peakPosDetect = (kome::ident::PeakPositionsDetector*)( m_peakPosItem->getCall()->invoke( &params2 ).prim.pt );

	if( peakPosDetect == NULL ){ // NULL チェック
		return ret;
	}
		
	int peak = -1;
	peak = peakPosDetect->detectPeakPositions( *getter, *pPosObj, progress );

	// エラーメッセージ表示
	std::string errMsg = peakPosDetect->getLastError();
	if( !errMsg.empty() ){
		kome::window::WindowTool::showError( errMsg.c_str() );
		return ret;
	}
		
	bool bSuccess;
	bSuccess = true;
				
	if( m_initFlg ) {
		std::vector< int > peaks;
		identMgr.getMatrixPeaks( peaks, selected_matrix_id );
		for( unsigned int i = 0; i < peaks.size(); i++ ) {
			identMgr.deleteMatrixPeak( peaks[ i ] );
		}
	}

	bool brt = false;
	bool bmz = false;
	bool bch = false;

	std::map< std::string, int > colMap;
				
	int numPeak = pPosObj->getNumberOfPeaks();
	for( int i=0; i < numPeak; i++ ){

		double rt = pPosObj->getPeakPositionRt( i );
		double mz = pPosObj->getPeakPositionMz( i );
		int charge = pPosObj->getPeakPositonCharge( i );

		if( rt > 0.0 ){
			brt = true;
		}
		if( mz > 0.0 ){
			bmz = true;
		}
		if( charge > 0.0 ){
			bch = true;
		}

		// add peak
		int matrixPeakId = identMgr.addMatrixPeak( rt, mz, charge, selected_matrix_id );

		int numProp = pPosObj->getNumberOfProperties();
		for( int j=0; j < numProp; j++ ){
			std::string colName = pPosObj->getPropertyName( j );
			int colType = pPosObj->getPropertyType( j );
			std::string colVal = pPosObj->getPropertyValue( j, colName.c_str() );
			
			// column ID
			int colId = -1;
			if( colMap.find( colName ) == colMap.end() ) {    // new column
				colId = identMgr.addMatrixColumn( colName.c_str(), (kome::ident::ColumnDataType)colType, selected_matrix_id );
				if( colId >= 0 ) {
					colMap[ colName ] = colId;
				}
			}
			else {
				colId = colMap[ colName ];
			}
			// set value
			if( colId >= 0 ) {
				identMgr.setAppendedValue( matrixPeakId, colId, colVal.c_str() );
			}
		}

		// peak values
		int numSample = pPosObj->getNumberOfMatrixSamples();
		for( int j=0; j < numSample; j++ ){
			int matrixSampleId = pPosObj->getMatrixSampleId( j );

			kome::objects::PeakElement* pk = pPosObj->getPeak( i, matrixSampleId );
			int profile = pPosObj->getProfile( i, matrixSampleId );
			double peakVal = pPosObj->getPeakValue( i, matrixSampleId );

			identMgr.setPeakValue( matrixPeakId, matrixSampleId, peakVal, pk, profile );
		}
	}
		
	delete pPosObj;
	
	if( peak > 0 ){ // @date 2014.01.15 <Add> M.Izumi
		identMgr.updateMatrixFlags( selected_matrix_id, brt, bmz, bch );
	}

	MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
	viewMgr.updateMatrixList();


	ret.prim.boolVal = true;
	return ret;
}

// Batch
#define SEARCH_PARAM_NAME				"search"
#define PEAK_SEARCH_PLUGIN_TYPE			"QUANT_PEAK_VAL"
#define RT_TOLERANCE_PARAM_NAME			"rt tol"
#define MZ_TOLERANCE_PARAM_NAME			"mz tol"

// add matrix group	// #22 (14)-matrix_peak_values
kome::objects::Variant setMatrixPeakValues( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "methods" );
	std::string methods = NVL( n, "" );
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	// 参考元：FillMatrixPanel::SavePanelSettings()

	puts("Matrix Peak Values...");

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// selected project
	int project = identMgr.getSelectedMatrix();

	// peak search
	const char* search = methods.c_str();

	/** peak search */
	kome::plugin::PluginFunctionItem* m_search;
	/** peak search settings */
	kome::objects::SettingParameterValues* m_searchSettings;

	m_search = plgMgr.getFunctionItem( PEAK_SEARCH_PLUGIN_TYPE, search );

	if (m_search != NULL)
	{
		m_searchSettings = settings->getSubParameterValues( "methods", search );
	}

	/** RT tolerance */
	double m_rtTol;

	// RT tolerance
	m_rtTol = settings->getDoubleValue( RT_TOLERANCE_PARAM_NAME, 0.5 );

	/** m/z tolerance */
	double m_mzTol;

	// m/z tolerance
	m_mzTol = settings->getDoubleValue( MZ_TOLERANCE_PARAM_NAME, 0.5 );

	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	/** selected groups */
	std::vector< int > m_samples;

	// selected groups
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		m_samples.push_back( samples[ i ] );
	}

	// standard group ID
	int m_stdId = -1;

	// manager
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	m_stdId = identMgr.getStandardSample( project );
	
	bool initFlg = true;
		
	kome::ident::PeakPositions* peakPos = new kome::ident::PeakPositions();
	
	// peaks
	std::vector< int > peaks;
	std::map< int, int > peakIdMap;
	identMgr.getMatrixPeaks( peaks, project );

	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		double rt = identMgr.getMatrixPeakRt( peaks[i] );
		double mz = identMgr.getMatrixPeakMz( peaks[i] );
		int charge = identMgr.getMatrixPeakCharge( peaks[i] );

		peakPos->addPeakPosition( rt, mz, charge );

		peakIdMap[ i ] = peaks[ i ];
	}

	// Always delete the added columns, so that previous Peak Analysis results are deleted.
	std::vector< int > columns;
	identMgr.getMatrixColumns( columns, project );
	for( unsigned int i = 0; i < columns.size(); i++ ) {
		identMgr.deleteMatrixColumn( columns[ i ] );
	}

	// groups
	samples.clear();
	for( unsigned int i = 0; i < m_samples.size(); i++ ) {
		samples.push_back( m_samples[i] );
	}

	// progress
	kome::core::DisplayProgress progress( "Fill Matrix" );

	const char* supplement_method= settings->getParameterValue( "supplement" ); // @date 2013/08/28 <Add> FUJITA
	if( supplement_method == NULL ){
		supplement_method = "(None)";
	}



	// search
	bSuccess = mgr.fillMatrix(
		m_search,
		m_searchSettings,
		m_rtTol,
		m_mzTol,
		initFlg,
		samples,
		m_stdId,
		&progress,
		supplement_method,		// @date 2013/08/28 <Add> FUJITA  supplement_method引数追加
		peakPos,
		getPeakValStatus( search ),
		peakIdMap
	);

	delete peakPos;

	if( bSuccess ) {
		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.updateMatrixList();
	}

	if (!bSuccess )
	{
		LOG_ERROR( FMT( "An error occurred while filling the matrix." ) );
		return ret;
	}

	ret.prim.boolVal = true;
	return ret;
}

// peak valueの状態を確認
bool getPeakValStatus( const char* name ){
	std::string strName = name;
	if( !strName.empty() ){
		if( strName.compare( "spec_peaks" ) == 0 || strName.compare( "ab3dpos" ) == 0 ||
			strName.compare( "mrm" ) == 0 )
		{
			return true;
		}
	}
	return false;
}


#define SCALING_PLUGIN_TYPE				"MATRIX_PEAK_SCALING"

// add matrix group	// #22 (15)-matrix_peak_scaling
kome::objects::Variant setMatrixPeakScaling( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "methods" ); //n="pareto,Log,Ln"といった形式で入ってくる
	std::string methods = NVL( n, "" );	
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	// 参考元：PeakAnalysisPanel::SavePanelSettings()

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = true;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();	
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	
	// selected project
	int project = identMgr.getSelectedMatrix();

	/** peak analysis */
	std::vector< kome::plugin::PluginFunctionItem* > m_scaling;

	/** settings */
	std::vector< kome::objects::SettingParameterValues* > m_scalingSettings;

	/** scaling */
	std::string m_helpscaling;

	const char* scaling = methods.c_str();	// m_helpPanelのm_scalingの文字を返す

	std::vector< std::string > scalingArr;
	stringtoken( scaling, LIST_VALUES_DELIMITER, scalingArr );

	for( unsigned int i=0; i < scalingArr.size(); i++ ){
		std::string scalingName = scalingArr[ i ];
		kome::objects::SettingParameterValues* subSettings = new kome::objects::SettingParameterValues();
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( SCALING_PLUGIN_TYPE, scalingName.c_str(), subSettings );
		if( item != NULL ){
			m_scaling.push_back( item );
			m_scalingSettings.push_back( subSettings );
		}
	}
	
	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	/** selected groups */
	std::vector< int > m_samples;

	// selected groups
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		m_samples.push_back( samples[ i ] );
	}

	// standard group ID	
	int m_stdId = -1;
	m_stdId = identMgr.getStandardSample( project );

	// progress
	kome::core::DisplayProgress progress( "Scaling Peaks" );
	puts( "Matrix Peak Scaling" );

	// scaling search
	if( m_scaling.size() > 0 && m_scalingSettings.size() > 0 ){
		bSuccess = mgr.scalingPeaks(
			&( m_scaling[ 0 ] ),
			&( m_scalingSettings[ 0 ] ),
			m_scaling.size(),
			m_stdId,
			&progress
		);
	}

	if (!bSuccess )
	{
		LOG_ERROR( FMT( "An error occurred while analyzing matrix peaks(peak scaling)." ) );
		return ret;
	}

	ret.prim.boolVal = bSuccess;
	
	return ret;
}


#define ANALYSIS_PLUGIN_TYPE            "MATRIX_PEAK_ANALYSIS"

// Matrix Peak Analysis	// #22 (16)-matrix_peak_analysis
kome::objects::Variant setMatrixPeakAnalysis( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "methods" ); //n="pareto,Log,Ln"といった形式で入ってくる
	std::string methods = NVL( n, "" );	
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	// 参考元：PeakAnalysisPanel::SavePanelSettings()

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = true;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();	
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	
	// selected project
	int project = identMgr.getSelectedMatrix();

	/** peak analysis */
	std::vector< kome::plugin::PluginFunctionItem* > m_analysis;

	/** settings */
	std::vector< kome::objects::SettingParameterValues* > m_analysisSettings;

	// analysis
	const char* analysis = methods.c_str();	// m_helpPanelのm_scalingの文字を返す
	std::vector< std::string > analysisArr;
	stringtoken( analysis, LIST_VALUES_DELIMITER, analysisArr );
	
	for( unsigned int i = 0; i < analysisArr.size(); i++ ) {
		std::string analysisName = analysisArr[ i ];
		kome::objects::SettingParameterValues* subSettings = new kome::objects::SettingParameterValues();
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( ANALYSIS_PLUGIN_TYPE, analysisName.c_str(), subSettings );
		if( item != NULL ) {
			m_analysis.push_back( item );
			m_analysisSettings.push_back( subSettings );
		}			
	}
		
	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	/** selected groups */
	std::vector< int > m_samples;

	// selected groups
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		m_samples.push_back( samples[ i ] );
	}

	// standard group ID	
	int m_stdId = -1;
	m_stdId = identMgr.getStandardSample( project );

	// progress
	kome::core::DisplayProgress progress( "Analysis Peaks" );
	puts( "Matrix Peak Analysis" );

	// analysis search
	if( m_analysis.size() > 0 && m_analysisSettings.size() > 0 ) {
		bSuccess = mgr.analyzePeaks(
			&( m_analysis[ 0 ] ),
			&( m_analysisSettings[ 0 ] ),
			m_analysis.size(),
			m_stdId,
			&progress
		);	
	}

	if (!bSuccess )
	{
		LOG_ERROR( FMT( "An error occurred while analyzing matrix peaks." ) );
		return ret;
	}
		
	ret.prim.boolVal = true;
	return ret;
}

// Matrix Peak Analysis	// #22 (18)-matrix_del_dup_peaks
kome::objects::Variant delMatrixDupPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	puts( "Delete Duplicate Peaks..." );	// @date 2013/08/08 <Add> OKADA

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "rt_tol" ); 
	std::string rt_tol = NVL( n, "" );	
	if( rt_tol.empty() ) {
		LOG_ERROR( FMT( "Failed to get the rt_tol." ) );
		return ret;
	}

	n = settings->getParameterValue( "mz_tol" ); 
	std::string mz_tol = NVL( n, "" );	
	if( mz_tol.empty() ) {
		LOG_ERROR( FMT( "Failed to get the mz_tol." ) );
		return ret;
	}

	// 参考元：PeakMagePanel::SavePanelSettings()

	// rt tol
	double m_rtTol;
			
	// m/z to
	double m_mzTol;

	m_rtTol = atof( rt_tol.c_str() );
	m_mzTol = atof( mz_tol.c_str() );

	// progress
	kome::core::DisplayProgress progress( "Delete Duplicate Peaks" );	// @date 2013/08/08 <Add> OKADA

	// 保存処理
	int matrixId = identMgr.getSelectedMatrix();
		
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrixId );
	if( peaks.size() == 0 ) {
		LOG_INFO( FMT( "Peak size is zero." ) );
		ret.prim.boolVal = true;
		return ret;
	}
	
	// search 
	std::vector< int > searches;
	identMgr.getSearchesFromMatrix( searches, matrixId );
	int searchId = -1;
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		searchId = std::max( searchId, searches[ i ] );
	}

	// get substance
	std::vector< int >substance;
	getSubstance( searches, searchId, peaks, substance );
	
	std::vector< int > resPeakId;
	
	double rt = identMgr.getMatrixPeakRt( peaks[0] );
	double mz = identMgr.getMatrixPeakMz( peaks[0] );

	progress.setRange( 1, peaks.size() );	// @date 2013/08/08 <Add> OKADA

	for( unsigned int i=1; i < peaks.size(); i++ )
	{

		// progress
		progress.setPosition( i );	// @date 2013/08/08 <Add> OKADA

		double srt = rt-m_rtTol;
		double ert = rt+m_rtTol;
		double smz = mz-m_mzTol;
		double emz = mz+m_mzTol;
	
		resPeakId.clear();
		for( unsigned int j=i; j < peaks.size(); j++ ){
			
			rt = identMgr.getMatrixPeakRt( peaks[j] );
			mz = identMgr.getMatrixPeakMz( peaks[j] );
	
			if(( srt < rt && rt <= ert) &&(smz < mz && mz <= emz) ){
				resPeakId.push_back( peaks[j] );
			}
					
		}

		DeleteMatrixPeak( matrixId, resPeakId, substance );

		rt = identMgr.getMatrixPeakRt( peaks[i] );
		mz = identMgr.getMatrixPeakMz( peaks[i] );

	}

	progress.fill();	// @date 2013/08/08 <Add> OKADA

	ret.prim.boolVal = true;
	return ret;
}


// PeakMagePanel::getSubstance()
void getSubstance( 
	std::vector< int > searches, 
	int searchId,
	std::vector< int > peaks, 
	std::vector< int >& subtrance 
){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	bool bflg = false;

	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		// add peak
		const int peakId = peaks[ i ];
		
		// hit spectra
		searches.clear();
		double score = -FLT_MAX;
		identMgr.getMatrixPeakSpectraFromSearchAndPeak( searches, searchId, peakId );
		for( unsigned int j = 0; j < searches.size(); j++ ) {
			const int specId = identMgr.getMatrixPeakSpectrumSpectrum( searches[ j ] );
			std::vector< int > hits;
			int substance = -1;
			identMgr.getHitsFromSpectrum( hits, specId );
			for( unsigned int k = 0; k < hits.size(); k++ ) {
				const int hitId = hits[ k ];
				kome::ident::HitInfo hit;
				identMgr.getHit( hitId, hit );
				bool loop = true;
				for( unsigned int l = 0; l < hit.props.size() && loop; l++ ) {
					if( hit.props.at( l ).type == kome::ident::TYPE_SCORE ) {
						double val = double();
						if( isdouble( hit.props.at( l ).value.c_str(), & val ) ) {
							if( val > score ) {
								substance = hit.substance;
								loop = false;
							}
						}
					}
				}
				if( substance < 0 ) {
					substance = hit.substance;
				}
			}

			kome::ident::SubstanceInfo subInfo;
			subInfo.parentId = substance;
			while( subInfo.parentId >= 0 ) {
				identMgr.getSubstance( subInfo.parentId, subInfo );
			}
			if( !subInfo.accession.empty() || !subInfo.name.empty() ) {
				
				bflg = true;
			}
		}
		if( bflg ){
			subtrance.push_back( peakId );
		}
	}
}


// delete matrix peak
void DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance ){
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
		
	double max=0.0;
	int peakId = 0;
	
	std::vector< Info > subInfo;
	
	for( unsigned int i=0; i < resPeaks.size(); i++ ){
		int peak = resPeaks[i];
		
		double sum = 0.0;
		if( substance.empty() ){
			getTotalValueOfPeak( matrixId, peak, sum );
			if( max < sum ){
				max = sum;
				peakId = peak;
			}
		}else{
			for( unsigned int j=0; j < substance.size(); j++ ){
				if( peak == substance[i] ){
					getTotalValueOfPeak( matrixId, peak, sum );
					if( max < sum ){
						max = sum;
						peakId = peak;
					}
				}
			}
		}
	}
	
	for( unsigned int i=0; i < resPeaks.size(); i++ ){
		if( peakId != resPeaks[i] ){
			identMgr.deleteMatrixPeak( resPeaks[i] );
		}
	}
}

void getTotalValueOfPeak( int matrixId, int peak , double& sum ){

	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrixId );

	// samples
	std::vector< int > samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}

	for( unsigned int j=0; j < samples.size(); j++ ){
		int sample = samples[j];

		sum += identMgr.getPeakValue( peak, sample );
	}
}

// Peak Matrix
kome::objects::Variant savePeakMatrixDataFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2013/09/11 <Add> OKADA ------->
	// get progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	// @date 2013/09/11 <Add> OKADA <-------

	// manager
	MatrixViewManager& mgr = MatrixViewManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* szPath = kome::plugin::PluginCallTool::getPath( params );

	ret =  mgr.saveTextPeakMatrixDataFile( szPath, params );	

	progress->fill();	// @date 2013/09/11

	ret.prim.boolVal = true;
	
	return ret;
}
