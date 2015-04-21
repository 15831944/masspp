/**
 * @file Operation.cpp
 * @brief implements of GridEmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.12.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "Operation.h"
#include "OperationManager.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Operation::Operation(){
	m_prevPath = "";
	m_updatePath = "";
	m_type = -1;
	m_targetSample = NULL;
	m_descript = "";
	m_paramString = "";

	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	mgr.addOperation( this );
	
	// キャンセルされたオペレーションを削除
	mgr.clearCanceledOperation();
}

// destructor
Operation::~Operation(){
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	mgr.removeOperation( this );
}

// set operation name
void Operation::setOperationName( const char* operationName ){
	m_operatName = operationName;
}

// get operation name
const char* Operation::getOperationName(){
	return m_operatName.c_str();
}
			
// set short name
void Operation::setShortName( const char* shortName ){
	m_shortName = shortName;
}

// get short name
const char* Operation::getShortName(){
	return m_shortName.c_str();
}
		
// set operation type
void Operation::setOperationType( int type ){
	m_type = type;
}

// get operation type
int Operation::getOperationType(){
	return m_type;
}
		
// set description
void Operation::setDescription( const char* description ){
	m_descript = description;
}

// get description
const char* Operation::getDescription(){
	if( m_descript.empty() ){	// @date 2013.09.17 <Mod> M.Izumi
		m_descript = onGetDescription();
	}
	return m_descript.c_str();	
}

//  set target sample
void Operation::setTargetSample( kome::objects::Sample* sample ){
	m_targetSample = sample;
}

// get target sample
kome::objects::Sample* Operation::getTargetSample(){
	return m_targetSample;
}

// set parameter string 
void Operation::setParametersString( const char* parameter ){
	onSetParametersString( parameter );
}

// get parameter string 
const char* Operation::getParametersString(){
	m_paramString = "";
	m_paramString = "\"";	
	m_paramString += onGetParametersString().c_str();
	m_paramString +="\"";

	return m_paramString.c_str();
}

// load condition
void Operation::loadCondition(boost::function< int ( void*, int ) > readFun ){
	onLoadCondition( readFun );
}

// save condition
void Operation::saveCondition( boost::function< int ( void*, int ) > writeFun ){
	onSaveCondition( writeFun );
}

// execute		
bool Operation::execute(){
	bool ret = false;

	OperationManager& optMgr = OperationManager::getInstance();
	if( getOperationType() == Operation::TYPE_INPUT ||
		getOperationType() == Operation::TYPE_OUTPUT ){

		// execute
		ret = onExecute();
	}else{
		// -----
		// Undo
		// -----
		m_prevPath = optMgr.getPreviousDataFilePath( this );	
		FILE* prevfp = fileopen( m_prevPath.c_str(), "wb" );

		kome::core::FileAccessor prev_acc( prevfp );
		saveCondition( boost::bind( &kome::core::DataAccessor::write, &prev_acc, _1, _2 ) );
		
		fclose( prevfp );				
		
		// Previous Data File Path 
		setPrevFilePath( m_prevPath.c_str() );

		// execute
		ret = onExecute();

		// -----
		// Redo
		// -----
		m_updatePath = optMgr.getUpdateDataFilePath( this );
		FILE* updatefp = fileopen( m_updatePath.c_str(), "wb" );

		kome::core::FileAccessor update_acc( updatefp );
		saveCondition( boost::bind( &kome::core::DataAccessor::write, &update_acc, _1, _2 ) );

		fclose( updatefp );

		// Update Data File Path
		setUpdateFilePath( m_updatePath.c_str() );
	}
	if( ret ){
		// Finished
		optMgr.moveToFinished( this );
	}
	return ret;
}

// get previous data file path
std::string Operation::getPrevFilePath(){
	return m_prevPath;
}

// set previous data file path
void Operation::setPrevFilePath( const char* strPath ){
	m_prevPath = strPath;
}

// get update data file path
std::string Operation::getUpdateFilePath(){
	return m_updatePath;
}

// set update data file path
void Operation::setUpdateFilePath( const char* strPath ){
	m_updatePath = strPath;
}
