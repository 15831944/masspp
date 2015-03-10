/**
 * @file OperationXmlHandler.cpp
 * @brief interfaces of Operation Xml Handler class
 *
 * @author OKADA, H
 * @date 2013/08/15
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationXmlHandler.h"
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
OperationXmlHandler::OperationXmlHandler(){
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
	m_xmlcommand.tmpPath = getpath( operatFolder.c_str(), datFile.c_str() );
}

// destructor
OperationXmlHandler::~OperationXmlHandler(){
	deleteBinaryData();
}

// initialize
void OperationXmlHandler::Initialize(){
	// clear
	clearCommand();
	m_xmlcommands.clear();
}

// on start document
void OperationXmlHandler::onStartDocument(){
}

// on end document
void OperationXmlHandler::onEndDocument(){
}

// on start element
void OperationXmlHandler::onStartElement( const char* name, kome::core::Properties& attrs ){
	
	if( strcmp( name, OPERATIONS_TAG ) == 0 ){				// <operations>
		// initialize
		Initialize();
	}else if( strcmp( name, OPERATION_TAG ) == 0 ){			// <operation>
		
		std::string type = attrs.getStringValue( OPERATION_TYPE, "" );
		kome::operation::OperationLOGManager& mgr = kome::operation::OperationLOGManager::getInstance();
		
		// get operation type
		m_xmlcommand.optType = mgr.getOperationType( type.c_str() );

		// get state
		m_xmlcommand.optState = attrs.getIntValue( OPERATION_STATE, -1 );
	}
}

// on end element
void OperationXmlHandler::onEndElement( const char* name, const char* text ){
//	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	if( strcmp( name, OPERATION_TAG ) == 0 ){			// </operation>

		// manager
	/*	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
				
		kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( OPERATION, m_xmlcommand.shortName.c_str() );		
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
				
				opt->setPrevFilePath( m_xmlcommand.prevDatapath.c_str() );
				opt->setUpdateFilePath( m_xmlcommand.upDatepath.c_str() );
				mgr.readOperation( opt, m_xmlcommand.optState, m_xmlcommand.prevDatapath, m_xmlcommand.upDatepath );
			
				deleteBinaryData();
			}
		}*/

		// 保存
		m_xmlcommands.push_back( m_xmlcommand );

		// clear
		clearCommand();

	}else if( strcmp( name, NAME_TAG ) == 0 ){			// </name>
		m_xmlcommand.name = text;
		
	}else if( strcmp( name, SHORT_NAME_TAG ) == 0 ){	// </short-name>
		m_xmlcommand.shortName = text;
	}else if( strcmp( name, DESC_TAG ) == 0 ){			// </desc>
		m_xmlcommand.desc = text;
	}else if( strcmp( name, PARAM_TAG ) == 0 ){			// </param>
		m_xmlcommand.param = text;
	}else if( strcmp( name, BEFORE_TAG ) == 0 ||  strcmp( name, AFTER_TAG ) == 0 ){		// </before>, </after>
		
		if( strcmp( "", text ) == 0 ){
			return;
		}		

		getBinaryData( name, text );	
	}	
}

// バイナリデータ取得
void OperationXmlHandler::getBinaryData( const char* name, const char* text ){	
	FILE* fp = fopen( m_xmlcommand.tmpPath.c_str(), "wb" );
	if( fp == NULL ){
		return;
	}

	// write
	fwrite( text, strlen( text ), 1, fp );
	fclose( fp );

	fp = fopen( m_xmlcommand.tmpPath.c_str(), "rb" );
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

		m_xmlcommand.beforeData = base64;
		m_xmlcommand.beforeSize = base64Size;		
			
	}else if( strcmp( name, AFTER_TAG ) == 0 ){ // after
		m_xmlcommand.afterData = base64;
		m_xmlcommand.afterSize = base64Size;
	}

	delete[] pucData;	

	DeleteFile( m_xmlcommand.tmpPath.c_str() );
}

// set operation info
void OperationXmlHandler::setOperation( kome::operation::Operation* opt ){
	// manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	opt->setOperationName( m_xmlcommand.name.c_str() );
	opt->setOperationType( m_xmlcommand.optType );
	opt->setShortName( m_xmlcommand.shortName.c_str() );
	opt->setDescription( m_xmlcommand.desc.c_str() );
	opt->setParametersString( m_xmlcommand.param.c_str() );

	int inum = optMgr.getNumberOfOperations();
	int index = 0;
	for( int i=0; i < inum; i++ ){
		if( opt == optMgr.getOperation(i) ){
			index = i;
			break;
		}
	}

	optMgr.setState( index, m_xmlcommand.optState );	
}

// write data
void OperationXmlHandler::writeData( kome::operation::Operation* opt ){
	// manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();

	m_xmlcommand.upDatepath = optMgr.getUpdateDataFilePath( opt );
	FILE* fp = fileopen( m_xmlcommand.upDatepath.c_str(), "wb" );
	if( fp != NULL ){
		if( m_xmlcommand.afterSize > 0 && m_xmlcommand.afterData != NULL ){ 
			fwrite( m_xmlcommand.afterData, m_xmlcommand.afterSize, 1, fp );
		}
		fclose( fp );
	}									

	m_xmlcommand.prevDatapath = optMgr.getPreviousDataFilePath( opt );
	fp = fileopen( m_xmlcommand.prevDatapath.c_str(), "wb" );
	if( fp != NULL ){
		if( m_xmlcommand.beforeSize > 0 && m_xmlcommand.beforeData != NULL ){
			fwrite( m_xmlcommand.beforeData, m_xmlcommand.beforeSize, 1, fp );
		}
		fclose( fp );
	}
}

// delete Binary data
void OperationXmlHandler::deleteBinaryData(){
	// delete
	if( m_xmlcommand.afterData != NULL ){
		delete[] m_xmlcommand.afterData;
	}
	m_xmlcommand.afterData = NULL;
	if( m_xmlcommand.beforeData != NULL ){
		delete[] m_xmlcommand.beforeData;
	}
	m_xmlcommand.beforeData = NULL;				
}


// initialize
void OperationXmlHandler::clearCommand(){
	m_xmlcommand.optType = -1;
	m_xmlcommand.optState = -1;
	m_xmlcommand.name = "";
	m_xmlcommand.shortName = "";
	m_xmlcommand.desc = "";
	m_xmlcommand.param = "";
		
	m_xmlcommand.beforeData = NULL;
	m_xmlcommand.beforeSize = -1;
	
	m_xmlcommand.afterData = NULL;
	m_xmlcommand.afterSize = -1;

	m_xmlcommand.upDatepath = "";
	m_xmlcommand.prevDatapath = "";
}


// get m_xmlcommands
void OperationXmlHandler::getXmlCommands( std::vector< kome::operation::OperationXmlHandler::XmlCommand_t >& xmlcommands )
{
	xmlcommands = m_xmlcommands;
}
