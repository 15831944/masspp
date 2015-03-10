/**
 * @file OperationLogHandler.cpp
 * @brief interfaces of Operation Log Handler class
 *
 * @author M.Izumi
 * @date 2013.07.24
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationLogHandler.h"
#include "OperationLogManager.h"

#define OPERATIONS_TAG		"operations"
#define OPERATION_TAG		"operation"
#define OPERATION_TYPE		"type"
#define OPERATION_STATE		"state"
#define NAME_TAG			"name"
#define SHORT_NAME_TAG		"short-name"
#define DESC_TAG			"desc"
#define PARAM_TAG			"param"
#define BEFORE_TAG			"before"
#define AFTER_TAG			"after"

#define OPERATION				"operation"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
OperationLogHandler::OperationLogHandler(){
	// initialize
	Initialize();

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	
	// Operation Folder
	std::string folderName = "Operation";
	std::string operatFolder = getpath( msppMgr.getTmpDir(), folderName.c_str() );
	
	if( !fileexists( operatFolder.c_str() ) ){
		makedirectory( operatFolder.c_str() );
	}

	std::string datFile = msppMgr.getTmpFileName( "tmpSave", ".dat", folderName.c_str() );
	m_tmpPath = getpath( operatFolder.c_str(), datFile.c_str() );
}

// destructor
OperationLogHandler::~OperationLogHandler(){
	deleteBinaryData();
}

// initialize
void OperationLogHandler::Initialize(){
	m_optType = -1;
	m_optState = -1;
	m_name = "";
	m_shortName = "";
	m_desc = "";
	m_param = "";
		
	m_beforeData = NULL;
	m_beforeSize = -1;
	
	m_afterData = NULL;
	m_afterSize = -1;

	m_upDatepath = "";
	m_prevDatapath = "";
}

// on start document
void OperationLogHandler::onStartDocument(){
}

// on end document
void OperationLogHandler::onEndDocument(){
}

// on start element
void OperationLogHandler::onStartElement( const char* name, kome::core::Properties& attrs ){
	
	if( strcmp( name, OPERATIONS_TAG ) == 0 ){				// <operations>
		// initialize
		Initialize();
	}else if( strcmp( name, OPERATION_TAG ) == 0 ){			// <operation>
		
		std::string type = attrs.getStringValue( OPERATION_TYPE, "" );
		kome::operation::OperationLOGManager& mgr = kome::operation::OperationLOGManager::getInstance();
		
		// get operation type
		m_optType = mgr.getOperationType( type.c_str() );

		// get state
		m_optState = attrs.getIntValue( OPERATION_STATE, -1 );
	}
}

// on end element
void OperationLogHandler::onEndElement( const char* name, const char* text ){
	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	if( strcmp( name, OPERATION_TAG ) == 0 ){			// </operation>

		// manager
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
				
		kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( OPERATION, m_shortName.c_str() );		
		if( fun != NULL ){
			// get operation
			kome::operation::Operation* opt = (kome::operation::Operation*)fun->getCall()->invoke( NULL ).prim.pt;

			if( opt != NULL ){
				setOperation( opt );

				if( opt->getOperationType() == kome::operation::Operation::TYPE_INPUT ||
					opt->getOperationType() == kome::operation::Operation::TYPE_OUTPUT ){
				}else{
					writeData( opt );
				}
				
				opt->setPrevFilePath( m_prevDatapath.c_str() );
				opt->setUpdateFilePath( m_upDatepath.c_str() );
				mgr.readOperation( opt, m_optState, m_prevDatapath, m_upDatepath );
			
				deleteBinaryData();
			}
		}

	}else if( strcmp( name, NAME_TAG ) == 0 ){			// </name>
		m_name = text;
		
	}else if( strcmp( name, SHORT_NAME_TAG ) == 0 ){	// </short-name>
		m_shortName = text;
	}else if( strcmp( name, DESC_TAG ) == 0 ){			// </desc>
		m_desc = text;
	}else if( strcmp( name, PARAM_TAG ) == 0 ){			// </param>
		m_param = text;
	}else if( strcmp( name, BEFORE_TAG ) == 0 ||  strcmp( name, AFTER_TAG ) == 0 ){		// </before>, </after>
		
		if( strcmp( "", text ) == 0 ){
			return;
		}		

		getBinaryData( name, text );	
	}	
}

// バイナリデータ取得
void OperationLogHandler::getBinaryData( const char* name, const char* text ){	
	FILE* fp = fopen( m_tmpPath.c_str(), "wb" );
	if( fp == NULL ){
		return;
	}

	// write
	fwrite( text, strlen( text ), 1, fp );
	fclose( fp );

	fp = fopen( m_tmpPath.c_str(), "rb" );
	if( fp == NULL ){
		return;
	}
	fpos_t fsize = 0;

	/* ファイルサイズを調査 */ 
	fseek( fp, 0, SEEK_END ); 
	fgetpos( fp, &fsize );
	fseek( fp, 0, SEEK_SET ); 

	unsigned char	*pucData;
	pucData = new unsigned char[fsize];

	fread( (void *)pucData, (size_t)1, (size_t)fsize, fp );

	fclose( fp );

	const unsigned int buffLen = fsize * 2 + 10;		
	char* base64 = new char[ buffLen ];
				
	int base64Size = kome::core::Base64::decode( (char*)pucData, fsize, base64, buffLen );
	base64[ base64Size ] = '\0';
		
	if( strcmp( name, BEFORE_TAG ) == 0  ){		// before

		m_beforeData = base64;
		m_beforeSize = base64Size;		
			
	}else if( strcmp( name, AFTER_TAG ) == 0 ){ // after
		m_afterData = base64;
		m_afterSize = base64Size;
	}

	delete[] pucData;	

	DeleteFile( m_tmpPath.c_str() );
}

// set operation info
void OperationLogHandler::setOperation( kome::operation::Operation* opt ){
	// manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	opt->setOperationName( m_name.c_str() );
	opt->setOperationType( m_optType );
	opt->setShortName( m_shortName.c_str() );
	opt->setDescription( m_desc.c_str() );
	opt->setParametersString( m_param.c_str() );

	int inum = optMgr.getNumberOfOperations();
	int index = 0;
	for( int i=0; i < inum; i++ ){
		if( opt == optMgr.getOperation(i) ){
			index = i;
			break;
		}
	}

	optMgr.setState( index, m_optState );	
}

// write data
void OperationLogHandler::writeData( kome::operation::Operation* opt ){
	// manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();

	m_upDatepath = optMgr.getUpdateDataFilePath( opt );
	FILE* fp = fileopen( m_upDatepath.c_str(), "wb" );
	if( fp != NULL ){
		if( m_afterSize > 0 && m_afterData != NULL ){ 
			fwrite( m_afterData, m_afterSize, 1, fp );
		}
		fclose( fp );
	}									

	m_prevDatapath = optMgr.getPreviousDataFilePath( opt );
	fp = fileopen( m_prevDatapath.c_str(), "wb" );
	if( fp != NULL ){
		if( m_beforeSize > 0 && m_beforeData != NULL ){
			fwrite( m_beforeData, m_beforeSize, 1, fp );
		}
		fclose( fp );
	}
}

// delete Binary data
void OperationLogHandler::deleteBinaryData(){
	// delete
	if( m_afterData != NULL ){
		delete[] m_afterData;
	}
	m_afterData = NULL;
	if( m_beforeData != NULL ){
		delete[] m_beforeData;
	}
	m_beforeData = NULL;				
}
