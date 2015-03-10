/**
 * @file BatchProcessingSubmitJobGrid.cpp
 * @brief implementsation of BatchProcessingSubmitJobGrid class
 *
 * @author OKADA, H
 * @date 2012-02-29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


// BatchProcessingSubmitJobGrid.cpp



#include "stdafx.h"
#include "BatchProcessingSubmitJobGrid.h"
#include "BatchServicePluginManager.h"

namespace
{
	enum col_no_tag{
		COL_TYPE,	// 0
		COL_INPUT,	// 1
		COL_OUTPUT	// 2-
	} col_no;
};


using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( BatchProcessingSubmitJobGrid,  kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, BatchProcessingSubmitJobGrid::onColClick )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, BatchProcessingSubmitJobGrid::onDoubleClick )
END_EVENT_TABLE()

// コンストラクタ
BatchProcessingSubmitJobGrid::BatchProcessingSubmitJobGrid( wxWindow* parent )
    : kome::window::GridListCtrl( parent, wxID_ANY, 400, 300, false ) {

	m_pParent = parent;
	m_selectFormatExt = "";
}

// デストラクタ
BatchProcessingSubmitJobGrid::~BatchProcessingSubmitJobGrid() { 
}

void BatchProcessingSubmitJobGrid::onCreate() {	 

	doCreate();
}


void BatchProcessingSubmitJobGrid::doCreate( void ){

	ClearAll();

    // 列

    addColumn( "Type", TYPE_STRING );	// Type 列		// @date 2012/08/07 <Add> OKADA
	
	addColumn( "Input", TYPE_STRING );	// Input 列

	addColumn( "Output File", TYPE_STRING );	

	m_vecIOSettingsList.clear();
	IOSettingsList tempIOSettingsList;

	/** 入力ファイル */
	for( unsigned int i=0; i<m_vectstrPaths.size(); i++ ){
	
		std::string strNowTime = m_vectstrOutputFilenameInfo[i];

		tempIOSettingsList.strTypeName.clear();	// @date 2012/08/07 <Add> OKADA
		tempIOSettingsList.strInput.clear();
		tempIOSettingsList.strOutput.clear();
		
		tempIOSettingsList.strTypeName = m_vectstrTypes[i];	// @date 2012/08/07 <Add> OKADA

		tempIOSettingsList.strInput = getfilename( m_vectstrPaths[i].c_str() );	// filename
		
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
		
		// @date 2013.09.30 <Mod> M.Izumi
		std::string strOutput = mgr.getOutputFineName( 
		m_vectstrPaths[i], 
		m_vectstrOutputFilenameInfo[i], 
		m_selectFormatExt.c_str(), 
		i,	// @date 2012/08/09 <Add> OKADA
		0 );

		std::string strFinename = getfilename( strOutput.c_str() );
			
		tempIOSettingsList.strOutput.push_back( strFinename );

		m_vecIOSettingsList.push_back( tempIOSettingsList );	// getString()から参照します
	}

	clearData( false );
	
    // 行
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vecIOSettingsList.size(); i++ ) {
		addData( (long long)( i ), false );
    }
}

// get string
std::string BatchProcessingSubmitJobGrid::getString( const long long data, const int col ) const {
    // 文字列
    std::string s;

    // プラグイン情報
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;

	if( data < m_vecIOSettingsList.size() ){
		;	// OK
	}else{
		// error
		return "";
	}

	IOSettingsList tempIOSettingsList;
	tempIOSettingsList = m_vecIOSettingsList[data];

	// columnが列数を越えていなければOK
	if( (unsigned long)col < (tempIOSettingsList.strOutput.size()+COL_OUTPUT) ){	// @date 2012/08/08 <Add> OKADA
		;	// OK
	}else{
		// error
		return "";	// colの値が大きすぎる
	}

    // 値取得
	if( col == COL_TYPE ){	// Type		// @date 2012/08/07 <Add> OKADA
		s = tempIOSettingsList.strTypeName;	// @date 2012/08/07 <Add> OKADA
	}										// @date 2012/08/07 <Add> OKADA

    if( col == COL_INPUT ) {    // Input
		s = tempIOSettingsList.strInput;
    }
    else if( COL_OUTPUT <= col ) {    // Output
		s = tempIOSettingsList.strOutput[col-COL_OUTPUT];	// @date 2012/08/08 <Add> OKADA
    }

    return s;
}

// 整数取得
int BatchProcessingSubmitJobGrid::getInt( const long long data, const int col ) const {
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

// タイトルをクリックしても何も起きないようにする
void BatchProcessingSubmitJobGrid::onColClick( wxListEvent& evt ) {
}

// ダブルクリック
void BatchProcessingSubmitJobGrid::onDoubleClick( wxListEvent& evt ) {
}

void BatchProcessingSubmitJobGrid::setVectStrPaths( stIOInf stIoInf )
{
	m_vectstrTypes.clear();	//　@date 2012/08/07 <Add> OKADA
	m_vectstrPaths.clear();
	m_vectstrOutputFilenameInfo.clear();

	for( unsigned int i=0; i<stIoInf.vectstrPaths.size(); i++ ){	// @date 2013/06/10 <Mod> OKADA
		m_vectstrTypes.push_back( stIoInf.vectstrTypeName[i] );	//　@date 2012/08/08 <Add> OKADA
		m_vectstrPaths.push_back( stIoInf.vectstrPaths[i] );
		m_vectstrOutputFilenameInfo.push_back( stIoInf.vectstrOutputFilenameInfo[i] );
	}
}


