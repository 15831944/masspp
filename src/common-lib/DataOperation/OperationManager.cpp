/**
 * @file OperationManager.cpp
 * @brief implements of GridEmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.12.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationManager.h"

#define FUNC_TYPE "update_list" 
#define FUNC_NAME "update_log_list"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
OperationManager::OperationManager(){
	m_saveCount = -1;
}

// destructor
OperationManager::~OperationManager(){
	// delete all operation
	int i = m_operations.size()-1;
	while( i > -1 ){
		delete m_operations[i].operation;
	
		i--;
	}
	m_operations.clear();
}

// Add Operation
void OperationManager::addOperation( Operation* operation ){
	if( operation == NULL ){
		return;
	}

	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].operation == operation ){
			return;
		}
	}
	// operation info
	OperationInfo optInf;
	optInf.operation = operation;
	optInf.iState = TYPE_NONE;

	m_operations.push_back( optInf );
}
			
// Remove Operation
void OperationManager::removeOperation( Operation* operation ){
	int i = m_operations.size()-1;
	while( i > -1 ){
		Operation* opt =  m_operations[i].operation;
		if( opt == operation ){		
			m_operations.erase( m_operations.begin() + i );
			// リストの更新処理
			SendStatusToList();
			return;
		}
		i--;
	}
}
			
// get number of operations
unsigned int OperationManager::getNumberOfOperations(){
	return m_operations.size();
}
		
// get operation
Operation* OperationManager::getOperation( const unsigned int i ){
	if( i >= m_operations.size() ){
		return NULL;
	}	
	return m_operations[i].operation;
}

// get number of finished operations
int OperationManager::getNumberOfFinishedOperations(){
	int inum = 0;
	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].iState == TYPE_FINISH ){ // finish
			inum++;
		}
	}
	return inum;
}

// get finished operation
Operation* OperationManager::getFinishedOperation( int index ){
	std::vector<Operation*> opt;
	for( unsigned i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].iState == TYPE_FINISH ){ // finish
			opt.push_back( m_operations[i].operation );
		}
	}
	
	if( index > (int)opt.size() || index <= -1 ){
		return NULL;
	}
	return opt[index];
}

// get number of canseled operations	
int OperationManager::getNumberOfCanceledOperations(){
	int inum = 0;
	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].iState == TYPE_CANCEL ){ // cansel
			inum++;
		}
	}
	return inum;
}
	
// get canseled operation
Operation* OperationManager::getCanceledOperation( int index ){
	std::vector<Operation*> opt;
	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].iState == TYPE_CANCEL ){ // cansel
			opt.push_back( m_operations[i].operation );
		}
	}
	
	if( index > (int)opt.size() || index <= -1 ){
		return NULL;
	}
	return opt[index];
}

// get Update Data File Path
const char* OperationManager::getUpdateDataFilePath( Operation* operation ){
	// Manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	
	// Operation Folder
	std::string folderName = "Operation";
	std::string operatFolder = getpath( msppMgr.getTmpDir(), folderName.c_str() );
	
	if( !fileexists( operatFolder.c_str() ) ){
		makedirectory( operatFolder.c_str() );
	}

	m_updateFilePath = operation->getUpdateFilePath();
	if( !fileexists( m_updateFilePath.c_str() ) ){
		// file name ( not path )
		std::string strName =  operation->getOperationName();
		strName = "UpdatePath" + strName ;

		std::string datFile = msppMgr.getTmpFileName( strName.c_str(), ".dat", folderName.c_str() );
		m_updateFilePath = getpath( operatFolder.c_str(), datFile.c_str() );
	}

	return m_updateFilePath.c_str();
}
		
// get Previouse Data File Path
const char* OperationManager::getPreviousDataFilePath( Operation* operation ){
	// Manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	
	// Operation Folder
	std::string folderName = "Operation";
	std::string operatFolder = getpath( msppMgr.getTmpDir(), folderName.c_str() );
	
	if( !fileexists( operatFolder.c_str() ) ){
		makedirectory( operatFolder.c_str() );
	}
	
	m_prevFilePath = operation->getPrevFilePath();
	// 一致するファイルがない場合は新規作成
	if( !fileexists( m_prevFilePath.c_str()) ){
		// file name ( not path )
		std::string strName =  operation->getOperationName();
		strName = "PrevPath" + strName ;

		std::string datFile = msppMgr.getTmpFileName( strName.c_str(), ".dat", folderName.c_str() );
		m_prevFilePath = getpath( operatFolder.c_str(), datFile.c_str() );
	}

	return m_prevFilePath.c_str();
}
			
// move to finished
void OperationManager::moveToFinished( Operation* operation ){
	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].operation == operation ){
			m_operations[i].iState = TYPE_FINISH;

			SendStatusToList();

			return;
		}
	}

	// operation info
	OperationInfo optInf;
	optInf.iState = TYPE_FINISH;
	optInf.operation = operation;
	
	m_operations.push_back( optInf );
	
	// リストの更新処理
	SendStatusToList();
}
			
// move to canseled
void OperationManager::movetoCanseled( Operation* operation ){
	for( unsigned int i=0; i < m_operations.size(); i++ ){
		if( m_operations[i].operation == operation ){
			m_operations[i].iState = TYPE_CANCEL;
			
			SendStatusToList();
			
			return;
		}
	}

	// operation inofo
	OperationInfo optInf;
	optInf.iState = TYPE_CANCEL;
	optInf.operation = operation;
	
	m_operations.push_back( optInf );
	
	// リストの更新処理
	SendStatusToList();
}

// clearCanseledOperation
void OperationManager::clearCanceledOperation(){
	int i = m_operations.size()-1;
		
	// @date 2013.09.17 <Mod> ->
	while( i > -1 ){
		if( m_operations[i].iState == TYPE_CANCEL ){
			delete m_operations[i].operation;
			m_operations[i].operation = NULL;
		}
		i--;
	}
	// @date 2013.09.17 <Mod> <-
}

// Send a state to list
void OperationManager::SendStatusToList(){
	
	//リストの更新処理
	kome::plugin::PluginFunctionItem* item;

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int iFuncItem = plgMgr.getNumberOfFunctionItems( FUNC_TYPE );
	item =  plgMgr.getFunctionItem( FUNC_TYPE, FUNC_NAME );
	if( item != NULL ){
		item->getCall()->invoke( NULL );
	}
}

// set status
void OperationManager::setState( const unsigned int index, int state ){
	if( index >= m_operations.size() ){
		return;
	}

	m_operations[index].iState = state;
}

// get status
int OperationManager::getState( const unsigned int index ){
	if( index >= m_operations.size() ){
		return -1;
	}
	return m_operations[index].iState;
}

// get refer to OperationManager object
OperationManager& OperationManager::getInstance(){
	// create the only object
	static OperationManager plgMgr;

	return plgMgr;
}
