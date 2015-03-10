/**
 * @file BatchJobListGrid.cpp
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchJobListGrid.h"


using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// コンストラクタ
BatchJobListGrid::BatchJobListGrid( wxWindow* parent )
    : kome::window::GridListCtrl( parent, wxID_ANY, 400, 300, false ) {
}

// デストラクタ
BatchJobListGrid::~BatchJobListGrid() { 
}

// テーブル作成
void BatchJobListGrid::onCreate() {	 //file name of input file (full path)

    // 列
    addColumn( "Status", TYPE_STRING );
    addColumn( "Date", TYPE_STRING );
    addColumn( "Name", TYPE_STRING );
    addColumn( "Server", TYPE_STRING );
    addColumn( "Comments", TYPE_STRING );

	//	vectActions
	BatchJobList tempBatchJob;

	tempBatchJob.strStatus = "Error";
	tempBatchJob.strDate = "2012/02/28 19:47:38";
	tempBatchJob.strName = "Test_1";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Comment";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 1, 0xEAB8B8 );

	tempBatchJob.strStatus = "Complete";
	tempBatchJob.strDate = "2012/02/28 19:47:39";
	tempBatchJob.strName = "Test_2";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Koment";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 1, 0xB8EAB8 );

	tempBatchJob.strStatus = "Running";
	tempBatchJob.strDate = "2012/02/28 19:47:40";
	tempBatchJob.strName = "Test_3";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Hoge";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 2, 0xFFFF99 );

	tempBatchJob.strStatus = "Waiting";
	tempBatchJob.strDate = "2012/02/28 19:47:41";
	tempBatchJob.strName = "Test_4";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Huga";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 3, kome::img::ColorTool::WHITE );


	tempBatchJob.strStatus = "Waiting";
	tempBatchJob.strDate = "2012/02/28 19:47:42";
	tempBatchJob.strName = "Test_5";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Piyo";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 4, kome::img::ColorTool::WHITE );

	tempBatchJob.strStatus = "Waiting";
	tempBatchJob.strDate = "2012/02/28 19:47:43";
	tempBatchJob.strName = "Test_6";
	tempBatchJob.strServer = "JobRequest";
	tempBatchJob.strComment = "Foo";
	m_vectBatchJobLists.push_back( tempBatchJob );
	(this)->setBackgroundColor( 5, kome::img::ColorTool::WHITE );

	m_vectBatchJobLists.push_back( tempBatchJob );

    // 行
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectBatchJobLists.size(); i++ ) {
		addData( (long long)( i ), false );	// TEST
    }
}

// 文字列取得
std::string BatchJobListGrid::getString( const long long data, const int col ) const {
    // 文字列
    std::string s;

    // プラグイン情報
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;

	if( data < m_vectBatchJobLists.size() ){
		;	// OK
	}else{
		// error
		return "";
	}

	BatchJobList tempBatchJob;
	tempBatchJob = m_vectBatchJobLists[data];

    // 値取得
    if( col == 0 ) {    // Status
		s = tempBatchJob.strStatus;
    }
    else if( col == 1 ) {    // Date
		s = tempBatchJob.strDate;
    }
    else if( col == 2 ) {    // Name
		s = tempBatchJob.strName;
    }
    else if( col == 3 ) {    // Server
		s = tempBatchJob.strServer;
    }
    else if( col == 4 ) {    // Comment
		s = tempBatchJob.strComment;
    }
    return s;
}

// 整数取得
int BatchJobListGrid::getInt( const long long data, const int col ) const {
    // 整数
    int v = -1;

    // プラグイン情報
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;
    if( data == NULL ) {
        return v;
    }

    // 値取得
    if( col == 2 ) {    // call count
        v = plugin->getNumberOfCalls();
    }

    return v;
}

