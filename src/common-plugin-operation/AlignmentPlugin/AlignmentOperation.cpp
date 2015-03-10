/**
 * @file AlignmentOperaton.cpp
 * @brief Alignment Operation class
 *
 * @author M.Izumi
 * @date 2012.03.12
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "AlignmentManager.h"
#include "AlignmentOperation.h"

using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
AlignmentOperation::AlignmentOperation(){
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	optMgr.addOperation( this );	
	
	// キャンセルされたオペレーションを削除
	optMgr.clearCanceledOperation();

	// 初期化
	m_trmtGroup = NULL;
}

// destructor
AlignmentOperation::~AlignmentOperation(){	
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	optMgr.removeOperation( this );
}

// on execute
bool AlignmentOperation::onExecute(){	
	bool ret = true;

	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	std::string strPrevPath = optMgr.getPreviousDataFilePath( this );
	
	if( !fileexists( m_oldPrevPath.c_str() ) ){	// previous data file path

		FILE* fp = fileopen( strPrevPath.c_str(), "wb" );

		kome::core::FileAccessor acc( fp );
		saveCondition( boost::bind( &kome::core::DataAccessor::write, &acc, _1, _2 ) );
		
		fclose( fp );
		
		optMgr.moveToFinished( this );	
		m_oldPrevPath = strPrevPath;

		setPrevFilePath( strPrevPath.c_str() );

	}else{									// update data file path
		std::string strUpdatePath = optMgr.getUpdateDataFilePath( this );
		FILE* fp = fileopen( strUpdatePath.c_str(), "wb" );

		kome::core::FileAccessor acc( fp );
		saveCondition( boost::bind( &kome::core::DataAccessor::write, &acc, _1, _2 ) );

		fclose( fp );
				
		setUpdateFilePath( strUpdatePath.c_str() );
	}	
		
	return ret;
}

// on load condition
void AlignmentOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){

	int trmtid = -1;
	int trmtsampleid = -1;

	readFun( &trmtid, sizeof( trmtid ) );
	readFun( &trmtsampleid, sizeof( trmtsampleid ) );
	kome::objects::Sample* trmtsample = kome::objects::Sample::getSampleById(trmtsampleid);
	
	// treatment group
	kome::objects::DataGroupNode* trmtGroup = NULL;
	if( trmtsample != NULL ){
		trmtGroup = trmtsample->getGroupById( trmtid );
		if( trmtGroup == NULL ){
			return;
		}
		
		unsigned int inum=0;
		readFun( &inum, sizeof( inum ) );

		for( unsigned int i=0; i < inum; i++ ){
			kome::objects::Spectrum* spec = trmtGroup->getSpectrum(i);
			double rt = 0.0;
			readFun( &rt, sizeof( rt ) );
			spec->setRt( rt );
		}

		// update group
		kome::plugin::PluginCallTool::onUpdateGroup( *trmtGroup );
		kome::window::WindowTool::refresh();
	}

}

// on save condition
void AlignmentOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){	
	kome::objects::DataGroupNode* trmtGroup = getGroup();

	if( trmtGroup != NULL ){
		
		// treatment
		int trmtid = trmtGroup->getId();
		int trmtsampleid = trmtGroup->getSample()->getSampleId();
		writeFun( &trmtsampleid, sizeof( trmtsampleid ) );
		writeFun( &trmtid, sizeof( trmtid ) );

		unsigned int inum = trmtGroup->getNumberOfSpectra();
		writeFun( &inum, sizeof( inum ) );

		for( unsigned int i=0; i < inum; i++ ){
			kome::objects::Spectrum* spec = trmtGroup->getSpectrum(i);
			if( spec != NULL ){
				double rt = spec->getRt();
				writeFun( &rt, sizeof( rt ) );
			}
		}
	}
}

std::string AlignmentOperation::onGetDescription(){
	return "";
}

std::string AlignmentOperation::onGetParametersString(){
	return "";
}

void AlignmentOperation::onSetParametersString( const char* strParam ){

}

// set group
void AlignmentOperation::setGroup( kome::objects::DataGroupNode* trmtGroup ){
	m_trmtGroup = trmtGroup;
}

// get group
kome::objects::DataGroupNode*  AlignmentOperation::getGroup(){
	return m_trmtGroup;
}
