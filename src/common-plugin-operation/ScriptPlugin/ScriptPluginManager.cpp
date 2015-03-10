/**
 * @file ScriptPluginManager.cpp
 * @brief interfaces of ScriptPluginManager class
 *
 * @author M.Izumi
 * @date 2013.08.21 
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ScriptPluginManager.h"
#include "ScriptPluginCall.h"

using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define PLUGIN_INFO_NAME			"Script"
#define LONG_NAME					"long name"
#define SHORT_NAME					"short name"
#define SCRIPT_TYPE_NAME			 "SCRIPT"

// constructor
ScriptPluginManager::ScriptPluginManager(){
	m_strScript = "";	
}

// destructor
ScriptPluginManager::~ScriptPluginManager(){	
}

// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
// ãNìÆéûÇÃÉXÉNÉäÉvÉgìoò^éûÇ…ê≥ÇµÇ≠ìoò^Ç≈Ç´Ç»Ç©Ç¡ÇΩåèÇèCê≥
//
// add PluginInfo
void ScriptPluginManager::addPluginInfo( stScriptInfo stInfo, ExecutingType eType ){
//
// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki
	
	std::string strlongName = FMT( "Script:<%s>", stInfo.strName.c_str() );
	std::string strshortName = FMT( "script-<%s>", stInfo.strName.c_str() );
	
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	kome::plugin::PluginInfo* info = plgMgr.getHiddenPlugin( PLUGIN_INFO_NAME );
	
	deletePluginInfo( stInfo );

	ScriptPluginCall* call = new ScriptPluginCall( info );	
	
	// long name
	call->getProperties().setValue( LONG_NAME, strlongName.c_str() );
	// short name
	call->getProperties().setValue( SHORT_NAME, strshortName.c_str() );

	call->setType( stInfo.strEvent.c_str() );
	call->setDescription( stInfo.strPath.c_str() );	
	call->setActive( true );
	
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, stInfo.strFuncItemName.c_str() );
	
	call->getProperties().setValue( "item", ( item != NULL ? item->getShortName() : "" ) );

	if( info != NULL ){		
		info->addCall( call );
	}

	// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
	//
	if  ( TYPE_INITIALIZE )
	{
		plgMgr.addActiveLookupTable( info );
	}
	else
	{
		plgMgr.updateActiveLookupTable();	
	}
	//
	// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki
}

// delete PluginInfo
void ScriptPluginManager::deletePluginInfo( stScriptInfo stInfo ){
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	kome::plugin::PluginInfo* info = plgMgr.getHiddenPlugin( PLUGIN_INFO_NAME );

	ScriptPluginCall* call = getScriptPluginCall( stInfo );
	if( call != NULL ){
		info->deleteCall( call );
	}	

	// >>>>>>	SPEC94873	@Date:2013/09/11	<Add>	A.Ozaki
	//
	plgMgr.updateActiveLookupTable();	
	//
	// <<<<<<	SPEC94873	@Date:2013/09/11	<Add>	A.Ozaki
}

// get scriptPluginCall
ScriptPluginCall* ScriptPluginManager::getScriptPluginCall( stScriptInfo stInfo ){
	// manger
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	std::string strlongName = FMT( "Script:<%s>", stInfo.strName.c_str() );
	std::string strshortName = FMT( "script-<%s>", stInfo.strName.c_str() );

	ScriptPluginCall* call = NULL;

	kome::plugin::PluginInfo* info = plgMgr.getHiddenPlugin( PLUGIN_INFO_NAME );
	int inum = info->getNumberOfCalls();
	for( int i=0; i < inum; i++ ){
		kome::plugin::PluginCall* pCall = info->getCall( i );
		// short nameÇ∆path Ç™àÍív
		if( ( pCall != NULL ) && strshortName.compare( pCall->getProperties().getStringValue( SHORT_NAME, "" ) ) == 0 &&
			stInfo.strPath.compare( pCall->getDescription() ) == 0 ){

			return (ScriptPluginCall*)pCall;
		}
	}

	return NULL;
}

// get the script
kome::objects::Script* ScriptPluginManager::getScript( kome::plugin::PluginFunctionItem* item ) {	// @Date:2013/08/06	<Modify>	A.Ozaki
	// check the item
	if( item == NULL ) {
		return NULL;
	}

	// active object
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::window::ChildFrame* child = kome::window::ChildFrame::getActiveFrame();
	kome::objects::Variant obj;
	if( child == NULL ) {
		kome::objects::Sample* sample = aoMgr.getActiveSample();
		if( sample != NULL ) {
			obj.type = kome::objects::Variant::SAMPLE;
			obj.prim.pt = sample;
		}
	}
	else {
		obj = child->getActiveObject();
	}

	// script object
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setActiveObject( params, obj );
	kome::objects::Variant var = item->getCall()->invoke( &params );
	kome::objects::Script* sc = NULL;	// @Date:2013/08/06	<Modify>	A.Ozaki
	if( var.type == kome::objects::Variant::SCRIPT ) {
		sc = (kome::objects::Script*)var.prim.pt;	// @Date:2013/08/06	<Modify>	A.Ozaki
	}

	return sc;
}

// get script value
std::string ScriptPluginManager::getScriptVal( const char* path ){
	std::string s;

	FILE* fp = fileopen( path, "r" );

	if( fp == NULL ){
		std::string msg = FMT( "Failed to open the file. [%s]", path );
		kome::window::WindowTool::showError( msg.c_str() );
	}else{
		char line[ 4096 ];
		while( fgets( line, 4096, fp ) != NULL ){
			s.append( line );
		}
		fclose( fp );
	}
	
	return s;
}

// script run
std::string ScriptPluginManager::scriptRun( 
	kome::objects::Script* sc,
	const char* val,
	kome::objects::Parameters* params 
){
	// active object
	kome::objects::Variant var;
	var.type = kome::objects::Variant::SAMPLE;
	var.prim.pt = NULL;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		var = frame->getActiveObject();
	}

	std::string result = sc->run( val, var, params );

	return result;
}

// get instance
ScriptPluginManager& ScriptPluginManager::getInstance(){
	// create object (This is the only object.)
	static ScriptPluginManager mgr;

	return mgr;
}
