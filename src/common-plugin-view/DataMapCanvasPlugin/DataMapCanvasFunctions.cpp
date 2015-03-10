/**
 * @file DataMapCanvasFunctions.cpp
 * @brief implements of functions of DataMapCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataMapCanvasFunctions.h"
#include "DataMapCanvas.h"
#include "DataMapCanvasOperation.h"

#include <boost/format.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION						"Data Map Filter"
#define KEY							"FILTER"
#define DEFAULT						"ms"

#define SPEC_STATUS_PARAM_NAME	    "DATA_MAP_WINDOW"
#define SPEC_STATUS_PARAM_VALUE	    "Canvas"

bool m_btn = false;


// create data map canvas
kome::objects::Variant createDataMapCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );

	// add operation
	DataMapCanvasOperation* opt = new DataMapCanvasOperation();
	opt->setShortName("datamap_canvas");
	opt->setOperationName("HeatMap Canvas");
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setTargetSample( dataMap->getSample() );
	opt->setParentWindow( parent );
	opt->setDataMapInfo( dataMap );
	
	if( opt->execute() ){
		ret.prim.pt = opt->getCanvas();
	}else{
		delete opt;
	}	

	return ret;
}

// set data map status
kome::objects::Variant setDataMapStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( SPEC_STATUS_PARAM_NAME, SPEC_STATUS_PARAM_VALUE );

	return ret;
}

// set data map status and open data map info
kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* params) {
	// @date 2011.08.30 <Mod> M.Izumi
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	setDataMapStatus( params );
	openDataMapFrame( params );
	
	return ret;
}


// on mouse move (DataMap)
kome::objects::Variant onDataMapMouseMoveStatusBar( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	int pixelX = mouseEvent->x();	// マウスカーソル現在位置 x (pixel)
	int pixelY = mouseEvent->y();	// マウスカーソル現在位置 y (pixel)
	double dMZ;	// マウスカーソルが現在示している位置の m/z 値
	double dRT; // マウスカーソルが現在示している位置の RT 値
	bool bInGraph=0;	// （マウスカ・[ソル現在位置が）heatmap表示領域内であれば1 外なら0

	double dMZWidth;	// heatmap表示領域 m/z の最大値最小値の差
	double dRTHeight;	// heatmap表示領域 RT の最大値最小値の差

	std::string strHisentaku;	// 非選択時、ステータスバーに表示する文字列のフォーマット ex."(%.9f, %.9f)"
	std::string strSentaku;		// 選択時、ステータスバーに表示する文字列のフォーマット ex."(%.9f, %.9f)-(%.9f, %.9f)"

	do{
		// get objects
		kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
		if( dataMap == NULL ) {
			return ret;
		}
		
		// create canvas
		DataMapCanvas* canvas = DataMapCanvas::getCanvas( dataMap );

		canvas->convertPos2Data( pixelX, pixelY, &dMZ, &dRT );

		// m/zやRTの範囲、ウインドウの表示領域の大きさから、ステータスバーの表示桁数を求める ////////

		int GraphAreaTop;	// heatmap表示領域 top の位置(pixel)
		int GraphAreaBottom;// heatmap表示領域 bottom の位置(pixel)
		int GraphAreaLeft;	// heatmap表示領域 left の位置(pixel)
		int GraphAreaRight;	// heatmap表示領域 right の位置(pixel)

		int GraphAreaWidth;	// heatmap表示領域 水平方向 の・ﾅ大値最小値の差(pixel)
		int GraphAreaHeight;// heatmap表示領域 垂直方向 の最大値最小値の差(pixel)

		// グラフエリア(heatmap表示領域)を取得
		canvas->getGraphPosition( &GraphAreaTop, &GraphAreaBottom, &GraphAreaLeft, &GraphAreaRight );
		GraphAreaWidth = GraphAreaRight - GraphAreaLeft;
		GraphAreaHeight = GraphAreaBottom - GraphAreaTop;

		// 現在のマウスカーソル位置がグラフエリアheatmap表示領域)内か否か判定する
		if(	( GraphAreaLeft <= pixelX ) && ( pixelX <= GraphAreaRight ) && 
			( GraphAreaTop <= pixelY ) && ( pixelY <= GraphAreaBottom )  ){
			bInGraph = 1;	// 領域内
		}

		double dRTTop;
		double dRTBottom;
		double dMZLeft;
		double dMZRight;

		// グラフエリア(heatmap表示領域)の左下求める
		canvas->convertPos2Data( GraphAreaLeft, GraphAreaBottom, &dMZLeft, &dRTBottom );

		// グラフエリア(heatmap表示領域)の右上求める
		canvas->convertPos2Data( GraphAreaRight, GraphAreaTop, &dMZRight, &dRTTop );

		// m/z, RT の幅を求める
		dMZWidth = dMZRight - dMZLeft;
		dRTHeight = dRTTop - dRTBottom;

		if( canvas->isTooSmall() ) {	// "too small"表示が出る場合はステータスバーに表示しない
			bInGraph = 0;
			break;
		}

		if( dMZWidth == 0 || dRTHeight == 0 ){
			bInGraph = 0;
			break;
		}
		double dMZPerPixelInv = GraphAreaWidth / dMZWidth;
		double dRTPerPixelInv = GraphAreaHeight / dRTHeight;

		int iMZKeta = ceil( log10( dMZPerPixelInv ) + 1.0 );	// +1.0 を足しているのは、最小変化桁の1桁下まで表示させているため
		int iRTKeta = ceil( log10( dRTPerPixelInv ) + 1.0 );

		if( iMZKeta < 0 ){	iMZKeta = 0; }
		if( iRTKeta < 0 ){	iRTKeta = 0; }

		std::string strFormatX;
		std::string strFormatY;

		strFormatX = (boost::format("%%.%df") % iMZKeta ).str();
		strFormatY = (boost::format("%%.%df") % iRTKeta ).str();

		strHisentaku = "(" + strFormatX + ", " + strFormatY + ")";	// "(%.9f, %.9f)"
		strSentaku = strHisentaku + "-" + strHisentaku;				// "(%.9f, %.9f)-(%.9f, %.9f)"
	}while( false );

	// 範囲選択時用に値を保存
	static double old_dMZ;
	static double old_dRT;

	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp( "HeatMap", 1 );	// ステータスバーを有効化
	std::string strDisp;

	if(	bInGraph == 0 ){
		// 領域外なので表示しない
		strDisp = "";
		statusBar.setFlgDisp( "HeatMap", 0 );	// ステータスバーを無効化
	}else{
		// 表示する場合
		if( mouseEvent->lbutton() == false ){
			// 非選択
			strDisp = (boost::format(strHisentaku) % dMZ % dRT).str();

			old_dMZ = dMZ;
			old_dRT = dRT;
		}else{
			// 選択中
			strDisp = (boost::format(strSentaku) % old_dMZ % old_dRT % dMZ % dRT).str();
		}
	}

	statusBar.setStatusMsg( "HeatMap", strDisp.c_str() );


	// @date 2011/06/29 <Add> OKADA ------->
	// マウスカーソルの設定
	{
		// graphics
		kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
		if( g == NULL ) {
			return ret;
		}

		setMouseCursor( *g, *mouseEvent, PLUGIN_NAME_DATA_MAP, ICON_NAME_HEATMAP_SELECT );
	}
	// @date 2011/06/29 <Add> OKADA <-------


	return ret;
}


// on mouse leave (DataMap)
kome::objects::Variant onDataMapMouseLeaveStatusBar( kome::objects::Parameters* ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp( "HeatMap", 0 );	// ステータスバーを無効化

	return ret;
}

// open data map frame
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	
	// @date 2011/06/20 <Add> OKADA datamapの取得方法 を 3DView から移植 ------->

	// active group 
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();
	kome::objects::DataGroupNode* group = ( sample == NULL ? NULL : sample->getRootDataGroupNode() );
		
	if( group == NULL ) {
		LOG_ERROR( FMT( "Failed to get the active spectrum group." ) );
		return ret;
	}

	// data map
	kome::objects::DataMapManager& mgr = kome::objects::DataMapManager::getInstance();
	std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMapLocal = mgr.getDataMap();
	kome::objects::DataMapInfo* dataMap = NULL;
	if( dataMapLocal.find( group ) == dataMapLocal.end() ) {	// create
		// create data map
		dataMap = new kome::objects::DataMapInfo( *group, MAP_ROW_SIZE, MAP_COL_SIZE );

		// on open
		if( !kome::plugin::PluginCallTool::onOpenDataMap( *dataMap ) ) {
			delete dataMap;
			return ret;
		}

		// create data map
		if( !dataMap->isCompleted() ) {
			// progress
			kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );
			// @date 2012.09.07 <Mod> M.Izumi ->
			// get ini file 
			kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
			kome::core::IniFile* ini = msppMgr.getIniFile();

			std::string filterName = ini->getString( SECTION, KEY, DEFAULT );

			// create
			dataMap->setRange( -1.0, -1.0, -1.0, -1.0, filterName.c_str(), progress );
			// @date 2012.09.07 <Mod> M.Izumi <-
			if( progress.isStopped() ) {
				delete dataMap;
				return ret;
			}
		}

		// set to map
		dataMapLocal[ group ] = dataMap;
		mgr.setDataMap2( dataMapLocal );
	}
	else {
		dataMap = dataMapLocal[ group ];
	}

	const char * pszName = dataMap->getGroup().getSample()->getName();
	std::string strOrgName = pszName;

	std::string strWindowName = "Heatmap - " + strOrgName;

	dataMap->getGroup().getSample()->setName( strWindowName.c_str() );

	// open
	kome::plugin::PluginCallTool::openDataMap( *dataMap );

	dataMap->getGroup().getSample()->setName( strOrgName.c_str() );

	return ret;
}


int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName )
{
	
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* acCanvas = ( frame == NULL ? NULL : frame->getCanvas() );
	// This check is necessary to prevent a crash when no canvas is active.
	if( acCanvas == NULL ){
		return 0;
	}

	int px = evt.x();
	int py = evt.y();
	
	do{
		if( px >= graphics.getGraphPositionLeft()
				&& px <= graphics.getGraphPositionRight()
				&& py >= graphics.getGraphPositionTop()
				&& py <= graphics.getGraphPositionBottom()
		) {
			// グラフ表示内
		
			//マウスカーソル変更
			kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();
			kome::plugin::PluginInfo* info;
			kome::plugin::PluginIcon* icon;

			if( pluginName == NULL ){
				break;	// while()文の後ろに脱出
			}
			info = pluginMgr.getPlugin( pluginName );
			if( info == NULL ){
				break;	// while()文の後ろに脱出
			}
			icon = info->getIcon( iconName );
			if( icon == NULL ){
				break;	// while()文の後ろに脱出
			}
			
			// @date 2012.01.10 <mod> M.Izumi
			if( m_btn && (evt.lbutton() || evt.rbutton() )){
				wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
				wxImage img( bitmap.ConvertToImage() );
		
				// hotspot(アイコン中心)の設定
				if( strcmp( iconName, ICON_NAME_HEATMAP_SELECT ) == 0 ){
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 5 );
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 5 );
				}else{
					break;
				}
		
				acCanvas->SetCursor(wxCursor(img));
				
			}else{
				acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW)); //矢印
			}
			return 0;
		}else{
			;
		}
	}while(false);
	
	if (acCanvas != NULL)
	{
		// This check is necessary to prevent a crash when no canvas is active.
		acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
	}

	return 0;
}

// @date 2012.01.10 <Add> M.Izumi ->
// mouse button down
kome::objects::Variant onDataMapMouseButtonDown( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}
	
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ){
		return ret;
	}

	m_btn = true;

	setMouseCursor( *g, *mouseEvent, PLUGIN_NAME_DATA_MAP, ICON_NAME_HEATMAP_SELECT );

	return ret;
}

// mouse button up
kome::objects::Variant onDataMapMouseButtonUp( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}
	
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ){
		return ret;
	}
	m_btn = false;
	
	setMouseCursor( *g, *mouseEvent, PLUGIN_NAME_DATA_MAP, ICON_NAME_HEATMAP_SELECT );
	
	return ret;
}
// @date 2012.01.10 <Add> M.Izumi <-

// @date 2013/02/14 <Add> OKADA ------->
// mouse button up
kome::objects::Variant onDataMapMouseDblClick( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}
	
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ){
		return ret;
	}
	m_btn = false;
	
	setMouseCursor( *g, *mouseEvent, PLUGIN_NAME_DATA_MAP, ICON_NAME_HEATMAP_SELECT );
	
	return ret;
}
// @date 2013/02/14 <Add> OKADA <-------

kome::objects::Variant getHeatMapCanvasOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new DataMapCanvasOperation();
	
	return ret;
}
