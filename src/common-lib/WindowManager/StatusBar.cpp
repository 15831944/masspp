/**
* @file StatusBar.cpp
* @brief Status Bar / ステータスバーを表示し、任意の文字列を表示します
* @author OKADA, H.
* @date 2011/01/07
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <補足説明>
*   使い方；
*     0. StatusBar.h をincludeします。
*     1. ステータスバーのインスタンスを取得します。
*        kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
*     2. ステータスバーのメンバ関数、 setStatusMsg()を用いて表示させたい文字列を指定します
*        statusBar.setStatusMsg( "common", "TEST" ); （例）
*     3. ステータスバーの一番左側の領域の名前は"common"と決まっているので、左側の領域に"TEST"と表示されます
*     4. 後述するようにxmlファイルでstatus-barタグを指定した場合、左側の領域以外に独自の領域を確保して文字列を表示できます
*     5. ステータスバーの領域を無効にすることも出来ます。以下のようにすると、"VerticalBox"で定義した領域が無くなります。
*        statusBar.setFlgDisp( "VerticalBox", 0 );
*        再度有効にしたい場合は、上記例の0を1に変更します。※なお、"common"を無効にすることはできません。
*   
*       "common"以外の領域を設定するには、プラグインのxmlファイルplugin.xmlに以下のように記述します。
*   
*	  <gui>
*		<status-bar target="spectrum"  order="127">
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*	  </gui>
*   
*	    targetには "none", "spectrum", "chromatogram", "datamap" のうちいずれか一つを指定します。たとえば
*     targetに"spectrum"を指定した場合、アクティブなウインドウがスペクトルの場合にそのステータスバーが表示されます。
*     スペクトルとクロマトグラム両方で表示させたいといった場合は、後述するようにstatus-barタグを複数書きます。
*       orderには、表示の優先度を指定します。ステータスバーが複数ある場合、左から数字が少ない順に表示されます。
*     1～255を目安に設定して下さい。優先度の値が他と重なった場合の表示順序は未定義です。また、"common"の優先度は0です。
*       status-bar-nameタグには、ステータスバーの名前を指定します。複数のプラグインで同じ名前を指定すると、
*     同じ領域に表示を行うことが出来ます。
*       status-barタグは、複数記述できます。status-bar-nameタグが同じタグを複数記述した場合、2つ目以降のorderは無視されます。
*     以下の例では、「VerticalBox」という名前のステータスバーを設定し、spectrumかchromatogramがアクティブなときに、
*     setStatusMsg()関数で指定した文字列が表示されるようになります。
*	  <gui>
*		<status-bar target="spectrum"  order="127">
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*		<status-bar target="chromatogram"  order="100">   ※「order="100"」は2つ目なので無視されます
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*	  </gui>
*   
*   
*   
*/

#include "Stdafx.h"
#include "StatusBar.h"

#include "ChildFrame.h"

using kome::window::StatusBar;


#define MAX_SIZE 10			// @date 2011.10.19 <Add> M.Izumi

// 配列の要素数を得る_countof()マクロの定義(ポインタを指定してはならない)
#ifndef _countof
	#define _countof(array)	sizeof(array)/sizeof((array)[0])
#else
#endif

// constructor
StatusBar::StatusBar() {
	m_pStatbar = NULL;
	m_pFrame = NULL;
	getMainWindow();	// @date 2011/01/26 <Add> OKADA
	getStatusBarInfo();
}

// destructor
StatusBar::~StatusBar() {
}

// get refer to PluginManager object
StatusBar& StatusBar::getInstance( void ) {
	// create the only object
	static StatusBar statusBar;

	return statusBar;
}

// xmlファイルから読み込まれたステータスバー情報を全て取得する
void StatusBar::getStatusBarInfo( void )
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int iCountOfPlugin = plgMgr.getNumberOfPlugins();
	int iCountOfSourceStatusBarInfos;
	kome::plugin::StatusBarInfo* p_tempStatusBarInfo; 

	for( int i=0; i<iCountOfPlugin; i++ ){
		iCountOfSourceStatusBarInfos = plgMgr.getPlugin( i )->getNumberOfStatusBarInfos();
		if( iCountOfSourceStatusBarInfos != 0 ){
			for( int j=0; j<iCountOfSourceStatusBarInfos; j++ ){
				p_tempStatusBarInfo = plgMgr.getPlugin( i )->getStatusBarInfo( j );

				// StatusBarInfo１件取得

				int iCountOfTotal = m_totalStatusBarInfo.size();
				int flgIcchi = 0;	// 一致したら(既に登録されていれば)「１」

				for( int k=0; k<iCountOfTotal; k++ ){
					if (strcmp( m_totalStatusBarInfo[k].getName(), p_tempStatusBarInfo->getName() ) ){
						// 不一致
					}else{
						// 一致
						flgIcchi = 1;
						m_totalStatusBarInfo[k].setTargetVal( p_tempStatusBarInfo->getTargetVal() );
						m_totalStatusBarInfo[k].setPlugin( p_tempStatusBarInfo->getPlugin() );
						m_totalStatusBarInfo[k].setWidth( p_tempStatusBarInfo->getWidth() ); // @date 2011.10.19 <Add> M.Izuimi
						break;	// 検索終了
					}
				}
	
				if( flgIcchi == 0 ){
					// 一度も一致しなかった->新たに追加
					m_totalStatusBarInfo.push_back( *p_tempStatusBarInfo );
				}
			}
		}
	}

	// sort statusbar vector / 取得したステータスバー情報をorderの順に並び替える
	// m_totalStatusBarInfoをorderの昇順にsortします
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();

	// sort処理をライブラリ関数で行う場合
	std::stable_sort( m_totalStatusBarInfo.begin(), m_totalStatusBarInfo.end() );

	int iiii = 0;
}


void StatusBar::getMainWindow( void )
{
	m_pFrame = (MainFrame*)kome::window::WindowTool::getMainWindow();

	if( m_pFrame != NULL ){
		if( m_pStatbar == NULL ){
			m_pStatbar = m_pFrame->CreateStatusBar();
		}
	}
}

// "hoge" といったキーを指定して、そこに表示させる処理
int StatusBar::setStatusMsg( const char* szPos, const char* szMessage )
{
	// キーを検索
	int nPos = getPositionByName( szPos );
	if( nPos < 0 || m_pStatbar == NULL ){	// 不一致
		return -1;	// err
	}else{
		// 表示文字列を指定
		m_totalStatusBarInfo[nPos].setMessage( szMessage );
		// 表示
		dispAuto();

		// @date 2011.10.19 <Add> M.Izumi
		int aryWidth[MAX_SIZE];
		for (int i=0; i < _countof( aryWidth ); i++) {
			aryWidth[i] = 0;
		}

		for( unsigned int i=0; i < m_totalStatusBarInfo.size(); i++ ){
			if( strcmp( m_totalStatusBarInfo[i].getName() , "common" ) == 0 ){
				aryWidth[i] = 540;
			}else{
				aryWidth[i] = m_totalStatusBarInfo[i].getWidth();
			}
		}
		
		m_pStatbar->SetStatusWidths( m_totalStatusBarInfo.size(), aryWidth );
		
	}

	return 0;
}

// その ステータスバーは、現在表示すべきか否か判定する関数 返値：0=表示すべきでない 1=表示すべき
int StatusBar::isDispNow( kome::plugin::StatusBarInfo& statusBarInfo )
{
	kome::plugin::StatusBarInfo::StatusBarTargetValue targetVal;

	if( statusBarInfo.getFlgDisp() == 0 ){
		return 0;
	}

	targetVal = statusBarInfo.getTargetVal();
	if( targetVal == kome::plugin::StatusBarInfo::VAL_SB_TGT_NONE ){
		return 1;
	}else{
		kome::plugin::StatusBarInfo::StatusBarTargetValue activeWindowKind;
		activeWindowKind = getActiveWindowKind();

		if( activeWindowKind & targetVal ){
			return 1;
		}
	}

	return 0;
}

// 現在アクティブなウインドウの種類を取得する
// 返値；
//   VAL_SB_TGT_OTHER			アクティブなウインドウが取得できない
//   VAL_SB_TGT_NONE			アクティブなウインドウは、spectrum, chromatogram, datamapのどれでもない
//   VAL_SB_TGT_SPECTRUM		アクティブなウインドウは、spectrum
//   VAL_SB_TGT_CHROMATOGRAM	アクティブなウインドウは、chromatogram
//   VAL_SB_TGT_DATAMAP			アクティブなウインドウは、datamap
kome::plugin::StatusBarInfo::StatusBarTargetValue StatusBar::getActiveWindowKind( void )
{
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	unsigned int specNum = 0;
	unsigned int chromNum = 0;
	unsigned int mapNum = 0;

	if( frame != NULL ) {
		specNum = frame->getNumberOfSpectra();
		chromNum = frame->getNumberOfChromatograms();
		mapNum = frame->getNumberOfDataMaps();
	}else{
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_OTHER;
	}

	if( (0 < specNum) && (chromNum == 0) && (mapNum == 0) ) {
		// Spectrum アクティブ
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_SPECTRUM;
	}
	else if( (specNum == 0) && (0 < chromNum) && (mapNum == 0) ) {
		// Chromatogram アクティブ
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_CHROMATOGRAM;
	}
	else if( (specNum == 0) && (chromNum == 0) && (0 < mapNum) ) {
		// Data Map アクティブ
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_DATAMAP;
	}
	else {
		// None
	}

	return kome::plugin::StatusBarInfo::VAL_SB_TGT_NONE;
}

// 全てのステータスバーをチェックして、表示すべきものを表示
void StatusBar::dispAuto( void )
{
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();	// 実際には、表示すべきステータスバーの件数を数えて iCountOfTotalStatusBarInfo に代入する
	int iFieldsCount = 0;

	if( m_pStatbar == NULL ){
		return;
	}
	
	// 表示すべきフィールドの数を設定する
	m_pStatbar->SetFieldsCount( iCountOfTotalStatusBarInfo );	

	// 表示すべきフィールドのみ表示する	
	// Spec No.86227 When dragging on an area in the Heatmap, the end position RT value is not shown in the status bar @date 2012.07.03 <Mod> M.Izumi
	for( int i=0; i<iCountOfTotalStatusBarInfo; i++ ){
		if( m_pStatbar != NULL ){
			if( 0 < isDispNow( m_totalStatusBarInfo[i] ) ){				
				m_pStatbar->SetStatusText( m_totalStatusBarInfo[i].getMessage(), i );				
			}
		}
	}

}


int StatusBar::getPositionByName( const char* szName )
{
	// キーを検索
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();

	for( int i=0; i<iCountOfTotalStatusBarInfo; i++ ){
		if( strcmp( m_totalStatusBarInfo[i].getName(), szName ) == 0 ){
			// 一致
			return i;
		}
	}

	return -1;	// err
}


int StatusBar::setFlgDisp(  const char* szPos, int flgDisp )
{
	// キーを検索
	int nPos = getPositionByName( szPos );
	if( nPos < 0 ){	// 不一致
		return -1;	// err
	}else{
		// 表示文字列を指定
		m_totalStatusBarInfo[nPos].setFlgDisp( flgDisp );
		// 表示
		dispAuto();
	}

	return 0;
}
