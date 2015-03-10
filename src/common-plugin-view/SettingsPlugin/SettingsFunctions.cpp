/**
 * @file SettingsFunctions.cpp
 * @brief implements of settings plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsFunctions.h"
#include "SettingsDialog.h"
#include "SettingsDialogPopup.h"	// Context Menu
#include "ParametersDialog.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PATH_PARAM_NAME		   "path"
#define SECTION				   "Parameters"
#define PATH_KEY			   "FILE_PATH"


// show settings dialog
kome::objects::Variant showParametersDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// dialog
	ParametersDialog* dlg = new ParametersDialog( kome::window::WindowTool::getMainWindow() );
	dlg->Show();

	return ret;
}


// show settings dialog
kome::objects::Variant showSettingsDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// dialog
	SettingsDialog* dlg = new SettingsDialog( kome::window::WindowTool::getMainWindow() );
	dlg->Show();

	return ret;
}

// set parameters file
kome::objects::Variant setParamsFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get settings
	kome::objects::SettingParameterValues* settings
		= kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get the parameters file path." ) );
		return ret;
	}

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	const char* fileName = settings->getParameterValue( PATH_PARAM_NAME );
	std::string path = getabsolutepath( msppMgr.getConfDir(), fileName );

	// create parameters file
	kome::core::IniFile* oldParams = msppMgr.getParameters();

	kome::core::IniFile* p = kome::core::IniFile::getIniFile( path.c_str() );
	if( p == NULL ) {
		LOG_ERROR_CODE(
			FMT(
				"Failed to load the parameters file. [%s]",
				path.c_str()
			),
			ERR_NULL_POINTER
		);
		return ret;
	}

	// set parameters file
	if( p != oldParams ) {
		oldParams->save();
		msppMgr.setParameters( p );
	}

	ret.prim.boolVal = true;

	return ret;
}

// update parameters file
kome::objects::Variant updateParamsFile( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// get ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return ret;
	}

	// parameters file path
	const char* fileName = ini->getString( SECTION, PATH_KEY, "parameters.ini" );
	std::string path = getabsolutepath( msppMgr.getConfDir(), fileName );
	kome::core::IniFile* oldParams = msppMgr.getParameters();
	kome::core::IniFile* p = kome::core::IniFile::getIniFile( path.c_str() );
	if( p == NULL ) {
		LOG_WARN(
			FMT(
				"Failed to load the parameters file. [%s]",
				path.c_str()
			)
		);
		return ret;
	}

	// set parameters file
	if( p != oldParams ) {
		oldParams->save();
		msppMgr.setParameters( p );
	}

	return ret;
}

// save parameters file
kome::objects::Variant saveParamsFile( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// save
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* p = msppMgr.getParameters();
	if( p != NULL ) {
		p->save();
	}

	return ret;
}

// show settings dialog
kome::objects::Variant showSettingsDialogFromContextMenu( PropertiesPageType pt ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	if(	( pt == PROP_NONE ) ||
		( pt == PROP_SPEC_VIEW ) ||
		( pt == PROP_CHROM_VIEW ) ||
		( pt == PROP_HEATMAP_VIEW ) ||
		( pt == PROP_3D_VIEW )
		){
		; // OK
	}else{
		ret.prim.boolVal = false;
		return ret;
	}

	// 3D View での右クリック時はPROP_HEATMAP_VIEWがくるので、この処理で区別する
	if( pt == PROP_HEATMAP_VIEW ){

		// data map
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
		if( frame != NULL ) {
			kome::objects::MsDataVariant obj( frame->getActiveObject() );
			kome::objects::DataMapInfo* dataMap = obj.getDataMap();

			// canvas
			wxWindow* canvas = frame->getCanvas();

			// 3D or Heatmap ?
			kome::window::Canvas* ptrCanvas = dynamic_cast< kome::window::Canvas* >( canvas );
			if( ptrCanvas == NULL ){
				// 3D View
				pt = PROP_3D_VIEW;
			}else{
				;	// Heatmap
			}
		}
	}

	// dialog
	SettingsDialogPopup* dlg = new SettingsDialogPopup( kome::window::WindowTool::getMainWindow() );
	
	dlg->setPropertiesPageType( pt ); // SPEC No.91674 : Optionsダイアログ @date 2013.03.18 <Add> M.Izumi
	dlg->Show();

	return ret;
}

// show settings dialog from context menu chrom view
kome::objects::Variant showSettingsDialogFromContextMenuChromView( kome::objects::Parameters* ){
	return showSettingsDialogFromContextMenu( PROP_CHROM_VIEW );
}

// show settings dialog from context menu spec view
kome::objects::Variant showSettingsDialogFromContextMenuSpecView( kome::objects::Parameters* ){
	return showSettingsDialogFromContextMenu( PROP_SPEC_VIEW );
}

// show settings dialog from context menu heatmap view
kome::objects::Variant showSettingsDialogFromContextMenuHeatmapView( kome::objects::Parameters* ){
	return showSettingsDialogFromContextMenu( PROP_HEATMAP_VIEW );
}
