/**
 * @file BatchGrid.cpp
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchGrid.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( BatchGrid,  kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, BatchGrid::onColClick )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, BatchGrid::onItemActivated )	// @date 2012/10/17 <Add> OKADA
END_EVENT_TABLE()


// コンストラクタ
BatchGrid::BatchGrid( wxWindow* parent )
    : kome::window::GridListCtrl( parent, wxID_ANY, 400, 300, false ) {

	m_pParentDlg = (kome::batchservice::BatchProcessingMethodConfigurationDialog*)parent;	// @date 2012/10/17 <Add> OKADA

	init();
}

// デストラクタ
BatchGrid::~BatchGrid() { 
}


// メンバ変数の初期化
void BatchGrid::init( void ) {

	/** セルの行数 */
	int m_nRows = 0;

	/** セルの列数 */
	m_nCols =2;

	/** アクションリスト初期化 */
	m_vectActions.clear();
}


// テーブル作成
void BatchGrid::onCreate() {
    // 列
    addColumn( "Stage", TYPE_STRING );
    addColumn( "Action", TYPE_STRING );

	// Dummy
	Action tempAct;

	// 行
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectActions.size(); i++ ) {
		long long llTempVal = (long long)plgMgr.getPlugin( i );
		addData( (long long)( i ), false );	// TEST
    }
}


int BatchGrid::addAction( Action act ){
	m_vectActions.push_back( act );

	clearData( true );

	RefreshBatchGrid();

	return 0;
}


int BatchGrid::updateAction(  int index, Action act ){

	if( index < 0 ){
		return -1;
	}

	if( m_vectActions.size() <= (unsigned int)index ){
		return -1;
	}

	m_vectActions[index] = act;

	return 0;

}


int BatchGrid::deleteAction( int index ){

	if( index < 0 ){
		return -1;
	}

	if( m_vectActions.size() <= (unsigned int)index ){
		return -1;
	}

	std::vector<Action>::iterator it = m_vectActions.begin();
	it += index;

	m_vectActions.erase( it );      // itの位置の要素を削除
	
	return 0;
}


int BatchGrid::RefreshBatchGrid( void ){

    // 行
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectActions.size(); i++ ) {
		long long llTempVal = (long long)plgMgr.getPlugin( i );
		addData( (long long)( i ), false );	// TEST
    }

	return 0;
}


Action& BatchGrid::getAction( int index ){
	return m_vectActions[index];
}


// 文字列取得
std::string BatchGrid::getString( const long long data, const int col ) const {
    // 文字列
    std::string s;

    // プラグイン情報
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;

	if( data < m_vectActions.size() ){
		;	// OK
	}else{
		// error
		return "";
	}
	if( data < 0 ){
		// error
		return "";
	}

	Action tempAct;
	tempAct = m_vectActions[data];

    // 値取得
    if( col == 0 ) {    // plugin name
		if( SN_SENTINEL <= tempAct.nStage ){
			s = "";	// error
		}else{
			
			int nFlgAddStr = 0;
			if( data == 0 ){
				// 先頭行は必ず追加
				nFlgAddStr = 1;
			}else if( tempAct.nStage == m_vectActions[data-1].nStage ){
				// Stageが1行前の物と同じなら空白を追加
				nFlgAddStr = 0;
			}else{
				nFlgAddStr = 1;
			}

			if( nFlgAddStr == 1 ){
				s = g_aszStageString[tempAct.nStage];
			}else{
				s = "";
			}
		}
    }
    else if( col == 1 ) {    // link
		s = tempAct.strName;
    }

    return s;
}


// 整数取得
int BatchGrid::getInt( const long long data, const int col ) const {
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


// 行数設定
int BatchGrid::setRows( int nRow ){
	if( 0 <= nRow ){
		m_nRows = nRow;
	}

	return m_nRows;
}

// タイトルをクリックしても何も起きないようにする
void BatchGrid::onColClick( wxListEvent& evt ) {
	int ddd=0;
}

// セルをダブルクリック
void BatchGrid::onItemActivated( wxListEvent& evt ) 
{
	int nIndex = evt.m_itemIndex;	// 選択行を表す。一番上の行が0で返る

	int nLineNo=0;
	for( int i=0; i<=nIndex; i++ ){
		if( m_vectActions[nIndex].nStage == m_vectActions[i].nStage ){
			nLineNo++;
		}
	}
	if( nLineNo <= 0 ){
		return;
	}
	int nLineNoBaseZero = nLineNo-1;	// 行番号を0～にする

	// nStage タブの nLineNo項目目（項目は1から始まる）の項目を編集

	int nTabNo = m_vectActions[nIndex].nStage;

	switch( nTabNo ){
	case SN_INITIALIZE:		// ①Initialize	// fall-through
	case SN_FILTER:			// ②
	case SN_MANIPULATE:		// ③
	case SN_ANALYZE:		// ④
	case SN_IDENTIFY:		// ⑤
	case SN_FILTER_RESULTS:	// ⑥Filter Results
		m_pParentDlg->editBatchActionSelectPanel( nTabNo, nLineNoBaseZero );
		break;

	default:
		break;
	}

	return;
}
