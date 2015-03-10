/**
 * @file DataMapRangeManager.cpp
 * @brief implements of DataMapRangeManager class
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataMapRangeManager.h"
#include "DataMapChangeDispRangeDialog.h"
#include "DataMapRangeOperation.h"

#define SECTION					"Data Map Filter"
#define KEY						"FILTER"
#define DEFAULT					"ms"

#define X_MINIMUM_RANGE			1.0
#define Y_MINIMUM_RANGE			0.1

#define RANGE_STATUS_KEY		"SYNCHRONIZED_RANGE"


#define BASE_MENU_ID ((13579+10))	// MENU_MIN_ID(10001) ～ MENU_MAX_ID(50000)

// <call-name>タグに書いた名前で処理を区別
#define STR_NAME_MENU_GENERATE_CHROMATOGRAM	"show Generate Chromatogram from datamap" // @date 2103.09.17 <Mod> M.Izumi 
#define STR_NAME_MENU_AVERAGED_SPECTRUM		"datamap range"							  // @date 2103.09.17 <Mod> M.Izumi 
#define STR_NAME_CHANGE_DISPLAYED_RANGE		"Change Displayed Range Dialog"

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataMapRangeManager::DataMapRangeManager() {
	m_changing.dataMap	  = NULL;
	m_changing.startPos  = -1;
	m_changing.endPos	= -1;

	// flag
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	m_syncDataMap = tobool( statusMgr.getValue( RANGE_STATUS_KEY ), false );

	// temporary directory
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	m_tmpDir = getpath( msppMgr.getTmpDir(), "datamaps" );
	makedirs( m_tmpDir.c_str() );

	// 初期化
	m_rtRange.first = -1.0;
	m_rtRange.second = -1.0;
	m_mzRange.first = -1.0;
	m_mzRange.second = -1.0;

	m_flgClickRight = false;

	m_flgRt = false;	/** 左クリック開始がRT領域 */
	m_flgMz = false;	/** 左クリック開始がm/z領域 */

	m_datamapMenu = NULL;
}

// destructor
DataMapRangeManager::~DataMapRangeManager() {
	
	for( std::map< kome::objects::DataMapInfo*, RangeInfo >::iterator it = m_rangeMap.begin(); 
		it != m_rangeMap.end(); it++ ){
		RangeInfo& range = (*it).second;
		while( !range.xRange.empty() ){
			range.xRange.pop();
		}
		while( !range.yRange.empty() ){
			range.yRange.pop();
		}		
	}
	m_rangeMap.clear();

	// m_datamapMenuの後始末(getDatamapRangeMenu()内でメモリ確保)
	if( m_datamapMenu != NULL ) {
		delete m_datamapMenu;
		m_datamapMenu = NULL;
	}

}

// prepare data map
bool DataMapRangeManager::onOpen( kome::objects::DataMapInfo& dataMap ) {
	// set range
	if( m_syncDataMap ) {
		return setSynchronizedDataMapRange( &dataMap );
	}

	// range
	RangeInfo& range = m_rangeMap[ &dataMap ];

	// create data map
	if( dataMap.isCompleted() ) {
		return true;
	}

	kome::core::Timer tm;
	tm.start();

	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );

	double minRt = -1.0;
	double maxRt = -1.0;
	double minMz = -1.0;
	double maxMz = -1.0;

	if( !range.xRange.empty() && !range.yRange.empty() ) {
		minRt = range.yRange.top().first;
		maxRt = range.yRange.top().second;
		minMz = range.xRange.top().first;
		maxMz = range.xRange.top().second;
	}

	DataInfo* dataInfo = getData( &( dataMap.getGroup() ), minRt, maxRt, minMz, maxMz );
	if( dataInfo != NULL ) {
		if( !dataMap.loadDataMap( dataInfo->path.c_str() ) ) {
			dataInfo = NULL;
		}
	}

	if( dataInfo == NULL ) {
		// @date 2012.09.07 <Mod> M.Izumi ->
		// get ini file
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		std::string filterName = ini->getString( SECTION, KEY, DEFAULT );

		dataMap.setRange( minMz, maxMz, minRt, maxRt, filterName.c_str(), progress );
		// @date 2012.09.07 <Mod> M.Izumi <-
		dataMap.commit();
		saveData( dataMap );
	}
	progress.fill();

	tm.stop();
	LOG_INFO( FMT( "Getting Data Map : %f sec", tm.getTotalTime() ) );

	return !progress.isStopped();
}

// prepare to draw data map
void DataMapRangeManager::prepare( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& graphics, wxWindow* canvas ) {
	// set range
	if( dataMap.isCompleted() ) {
		// x range
		double minX = dataMap.getMinMz();
		double maxX = dataMap.getMaxMz();
		double midX = ( minX + maxX ) / 2.0;
		minX = std::min( midX - X_MINIMUM_RANGE / 2.0, minX );
		minX = std::max( 0.0, minX );
		maxX = std::max( maxX, minX + X_MINIMUM_RANGE );

		// y range
		double minY = dataMap.getMinRt();
		double maxY = dataMap.getMaxRt();
		double midY = ( minY + maxY ) / 2.0;
		minY = std::min( midY - Y_MINIMUM_RANGE / 2.0, minY );
		minY = std::max( 0.0, minY );
		maxY = std::max( maxY, minY + Y_MINIMUM_RANGE );

		// set range
		graphics.setDisplayedRange( minX, maxX, minY, maxY );
	}
}

// close spectrum
void DataMapRangeManager::close( kome::objects::DataMapInfo* dataMap ) {
}


// down button (data map)
bool DataMapRangeManager::downButton(
		kome::objects::DataMapInfo& dataMap,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	
	// RT軸領域
	bool flgRt = false;
	if( evt.x() < graphics.getGraphPositionLeft() ){
		if(	( graphics.getGraphPositionTop() <= evt.y() )&&
			( evt.y() <= graphics.getGraphPositionBottom() ) ){
			flgRt = true;
		}
	}
	m_flgRt = flgRt;

	// m/z軸領域
	bool flgMz = false;
	if(graphics.getGraphPositionBottom() < evt.y() ){
		if(	( graphics.getGraphPositionLeft() <= evt.x() )&&
			( evt.x() <= graphics.getGraphPositionRight() ) ){
			flgMz = true;
		}
	}
	m_flgMz = flgMz;

	if(	( flgRt == false ) && ( flgMz == false ) ){
		if( evt.x() < graphics.getGraphPositionLeft()
				|| evt.x() > graphics.getGraphPositionRight()
				|| evt.y() < graphics.getGraphPositionTop()
				|| evt.y() > graphics.getGraphPositionBottom()
		) {
			return false;
		}
	}

	// active data
	m_changing.dataMap = &dataMap;

	// flags
	bool refresh = false;
	bool setNull = false;

	// @date 2013/02/12 <Add> OKADA ------>
	if( evt.rbutton() ) {   // pop range
		m_flgClickRight = true;
	}
	// @date 2013/02/12 <Add> OKADA <------

	// position
	int xPos = evt.x();
	int yPos = evt.y();
	int pos = ( xPos << 16 ) + yPos;

	// set position
	m_changing.startPos = pos;
	m_changing.endPos = pos;
	// refresh
	refresh = true;

	return refresh;
}

// get range information
DataMapRangeManager::RangeInfo* DataMapRangeManager::getRange() {
	// get range
	RangeInfo* range = ( m_syncDataMap ? &m_syncDataMapRange : &( m_rangeMap[ m_changing.dataMap ] ) );

	return range;
}

// get range information
DataMapRangeManager::RangeInfo* DataMapRangeManager::getRange( kome::objects::DataMapInfo* dataMap ){
	// get range
	RangeInfo* range = ( m_syncDataMap ? &m_syncDataMapRange : &( m_rangeMap[ dataMap ] ) );

	return range;
}

// get data
DataMapRangeManager::DataInfo* DataMapRangeManager::getData(
		kome::objects::DataGroupNode* group,
		const double minRt,
		const double maxRt, 
		const double minMz, 
		const double maxMz
) {
	// tolerance
	const double rtTol = Y_MINIMUM_RANGE / 2.0;
	const double mzTol = X_MINIMUM_RANGE / 2.0;

	// search
	for( unsigned int i = 0; i < m_dataArray.size(); i++ ) {
		DataInfo* info = &( m_dataArray[ i ] );
		if( info->group == group
				&& fabs( info->minRt - minRt ) < rtTol
				&& fabs( info->maxRt - maxRt ) < rtTol
				&& fabs( info->minMz - minMz ) < mzTol
				&& fabs( info->maxMz - maxMz ) < mzTol ) {
			return info;
		}
	}

	return NULL;
}

// save data
void DataMapRangeManager::saveData( kome::objects::DataMapInfo& dataMap ) {
	// file name
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string fileName = FMT( "data%04x", ( 0xffff & (long long)this ) );
	fileName = msppMgr.getTmpFileName( fileName.c_str(), ".tmp", "datamaps" );
	std::string path = getpath( m_tmpDir.c_str(), fileName.c_str() );

	// save
	if( dataMap.saveDataMap( path.c_str() ) ) {
		// add to array
		m_dataArray.resize( m_dataArray.size() + 1 );

		m_dataArray.back().group = &( dataMap.getGroup() );
		m_dataArray.back().minRt = dataMap.getMinRt();
		m_dataArray.back().maxRt = dataMap.getMaxRt();
		m_dataArray.back().minMz = dataMap.getMinMz();
		m_dataArray.back().maxMz  = dataMap.getMaxMz();
		m_dataArray.back().path = path;
	}
}

// move mouse (data map)
bool DataMapRangeManager::moveMouse(
		kome::objects::DataMapInfo& dataMap,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// 右ドラッグ以外
	if( !evt.rbutton() ){								// @date 2013/02/12 <Add> OKADA
		// check the object
		if( m_changing.dataMap != &dataMap ) {
			return false;
		}
	}													

	// refresh flag
	bool refresh = false;

	// set position
	if( evt.lbutton() || evt.rbutton() ) {
		int xPos = CLAMP( evt.x(), graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
		int yPos = CLAMP( evt.y(), graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );
		int pos = ( xPos << 16 ) + yPos;

		if( m_changing.endPos != pos ) {
			m_changing.endPos = pos;
			refresh = true;
		}
	}

	return refresh;
}

// load actions
void DataMapRangeManager::loadActions() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// clear
	m_datamapRangeActions.clear();

	// get actions
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );
		for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
			kome::plugin::PluginCall* call = plugin->getCall( j );
			if( compareignorecase( call->getType(), "datamap_range_action" ) == 0 ) {
				m_datamapRangeActions.push_back( call );
			}
		}
	}

	// set function
	unsigned int num = m_datamapRangeActions.size();
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();
	for( unsigned int i = 0; i < num; i++ ) {
		mainFrame->setExecuteFunction( BASE_MENU_ID + i,  DataMapRangeManager::actionDatamap );
	}
}

// datamap range action
bool DataMapRangeManager::actionDatamap( kome::objects::Variant obj, int id ) {
	// index
	int idx = id - BASE_MENU_ID;
	if( idx < 0 ) {
		LOG_WARN( FMT( "Invalid ID." ) );
		return false;
	}

	// manager
	DataMapRangeManager& mgr = getInstance();

	RangeInfo* range = mgr.getRange();

	kome::objects::Parameters params;
	
	// active object
	kome::objects::MsDataVariant var( obj );
	kome::objects::Spectrum* spec = NULL;
	kome::objects::Chromatogram* chrom = NULL;
	kome::objects::DataMapInfo* dataMapInfo = var.getDataMap();

	const char* szName = mgr.m_datamapRangeActions[ idx ]->getName();
	if( compareignorecase( szName, STR_NAME_MENU_GENERATE_CHROMATOGRAM ) == 0 ){	// Generate Chromatogram...
		unsigned int uiNumSpec = dataMapInfo->getGroup().getNumberOfSpectra();
		if( 0 <= uiNumSpec ){
			// 例：1件目取得
			spec = dataMapInfo->getGroup().getSpectrum( 0 );
			kome::plugin::PluginCallTool::setSpectrum( params, *spec );

			double dFirst;
			double dSecond;
			mgr.getMzRange( &dFirst, &dSecond );
			kome::plugin::PluginCallTool::setRange( params, dFirst, dSecond );
		}
	}
	if( compareignorecase( szName, STR_NAME_MENU_AVERAGED_SPECTRUM ) == 0 ){	// Average Spectrum...
		unsigned int uiNumSpec = dataMapInfo->getGroup().getNumberOfChromatograms();
		if( 0 <= uiNumSpec ){
			chrom = dataMapInfo->getGroup().getChromatogram( 0 );
			kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
			double dFirst;
			double dSecond;
			mgr.getRtRange( &dFirst, &dSecond );
			kome::plugin::PluginCallTool::setRange( params, dFirst, dSecond );
		}
	}

	mgr.m_datamapRangeActions[ idx ]->invoke( &params );

	return true;
}

// create chromatogram range menu
void DataMapRangeManager::createDatamapRangeMenu( wxMenu& menu ) {
	// load
	if( m_datamapRangeActions.size() == 0 ) {
		loadActions();

		//sort
		unsigned int uiSize = m_datamapRangeActions.size();
		if( 2 <= uiSize ){
			bool flgSwapped;
			do {
				flgSwapped = false;
				for( unsigned int i=0; i<uiSize-1; i++ ){
					unsigned short wOrder = m_datamapRangeActions[i]->getOrder();
					unsigned short wOrderNext = m_datamapRangeActions[i+1]->getOrder();
					if( wOrder < wOrderNext ){
						// OK
					}else{
						// swap
						kome::plugin::PluginCall* callTemp;
						callTemp = m_datamapRangeActions[i];
						m_datamapRangeActions[i] = m_datamapRangeActions[i+1];
						m_datamapRangeActions[i+1] = callTemp;
						flgSwapped = true;
					}
				}
			}while( flgSwapped );
		}
		
	}
	
	// each actions
	for( unsigned int i = 0; i < m_datamapRangeActions.size(); i++ ) {
		kome::plugin::PluginCall* call = m_datamapRangeActions[ i ];
		std::string name = call->getProperties().getStringValue( "name", "(null)" );
		menu.Append( BASE_MENU_ID + i, wxT( name.c_str() ), wxT( call->getDescription() ) );
	}
}

// get datamap menu
wxMenu* DataMapRangeManager::getDatamapRangeMenu() {
	if( m_datamapMenu == NULL ) {
		m_datamapMenu = new wxMenu();
		createDatamapRangeMenu( *m_datamapMenu );
	}

	return m_datamapMenu;
}

// up button (data map)
bool DataMapRangeManager::upButton(
		kome::objects::DataMapInfo& dataMap,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {

	// マウスボタンdown開始地点
	bool flgRt = m_flgRt;
	bool flgMz = m_flgMz;

	m_flgRt = false;
	m_flgMz = false;

	// check the object
	if( m_changing.dataMap != &dataMap ) {
		m_changing.dataMap = NULL;
		m_flgClickRight = false;		// @date 2013/02/12 <Add> OKADA
		return false;
	}
		
	// range
	RangeInfo* range = getRange();
	if( range == NULL ){
		m_flgClickRight = false;		// @date 2013/02/12 <Add> OKADA
		return false;
	}

	// @date 2013.06.06 <Add> M.Izumi
	if( m_changing.startPos == m_changing.endPos ){
		return false;
	}
	
	// refresh flag
	bool refresh = false;
	
	// active object
	m_changing.dataMap = NULL;

	// set position
	int xPos0 = m_changing.startPos >> 16;
	int yPos0 = m_changing.startPos & 0xffff;
	int xPos1 = CLAMP( evt.x(), graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
	int yPos1 = CLAMP( evt.y(), graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );

	// 右クリックから復帰の場合、範囲が選択されていないならfalseを返す
	if( m_flgClickRight == true ){	// @date 2013/02/12 <Add> OKADA
		m_flgClickRight = false;		// @date 2013/02/12 <Add> OKADA
	}else{							// @date 2013/02/12 <Add> OKADA

		if( ( xPos0 == xPos1 && yPos0 == yPos1 )
				|| ( fabs( graphics.getMaxX() - graphics.getMinX() ) < 0.105
				&& fabs( graphics.getMaxY() - graphics.getMinY() ) < 0.105 ) ) {
			return true;
		}
	}								// @date 2013/02/12 <Add> OKADA

	// rect
	double l0 = (double)( MIN( xPos0, xPos1 ) );
	double l1 = l0 - 1.0;
	double r0 = (double)( MAX( xPos0, xPos1 ) );
	double r1 = r0 + 1.0;
	double t0 = (double)( MIN( yPos0, yPos1 ) );
	double t1 = t0 - 1.0;
	double b0 = (double)( MAX( yPos0, yPos1 ) );
	double b1 = b0 + 1.0;

	// get range
	double minX0 = double();
	double minX1 = double();
	double maxX0 = double();
	double maxX1 = double();
	double minY0 = double();
	double minY1 = double();
	double maxY0 = double();
	double maxY1 = double();

	graphics.transformPositionToData( l0, b0, &minX0, &minY0 );
	graphics.transformPositionToData( l1, b1, &minX1, &minY1 );
	graphics.transformPositionToData( r0, t0, &maxX0, &maxY0 );
	graphics.transformPositionToData( r1, t1, &maxX1, &maxY1 );

	double minX = ( minX0 + minX1 ) / 2.0;		
	double maxX = ( maxX0 + maxX1 ) / 2.0;
	double minY = ( minY0 + minY1 ) / 2.0;
	double maxY = ( maxY0 + maxY1 ) / 2.0;

	double midX = ( minX + maxX ) / 2.0;
	minX = std::min( midX - 0.05, minX );
	minX = std::max( graphics.getMinX(), minX );// graphics.getMinX()= X(横)軸方向の最小値。m/zの最小値。表示中の値
	maxX = std::max( minX + 0.1, maxX );
	maxX = std::min( graphics.getMaxX(), maxX );// graphics.getMaxX()= X(横)軸方向の最大値。m/zの最大値。表示中の値
	minX = std::min( minX, maxX - 0.1 );

	double midY = ( minY + maxY ) / 2.0;
	minY = std::min( midY - 0.05, minY );
	minY = std::max( graphics.getMinY(), minY );// graphics.getMinY()= Y(縦)軸方向の最小値。RTの最小値。表示中の値
	maxY = std::max( minY + 0.1, maxY );
	maxY = std::min( graphics.getMaxY(), maxY );// graphics.getMaxY()= Y(縦)軸方向の最大値。RTの最大値。表示中の値
	minY = std::min( minY, maxY - 0.1 );
	

	if( flgRt ){
		// RTは選択範囲、m/zは表示範囲
		minX = graphics.getMinX();
		maxX =  graphics.getMaxX();
	}
	if( flgMz ){
		// RTは表示範囲、m/zは選択範囲
		minY = graphics.getMinY();
		maxY = graphics.getMaxY();
	}

	if( evt.lbutton() ) {   // push range
		// Add Operation
		DataMapRangeOperation* opt = new DataMapRangeOperation();
		opt->setOperationName( "DataMap Range" );
		opt->setShortName( "datamp_range" );
		opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( dataMap.getSample() );
		opt->setCanvas( canvas );
		opt->setDataMapInfo( &dataMap );
		opt->setStartMz(minX );
		opt->setEndMz( maxX );
		opt->setStartRt( minY );
		opt->setEndRt( maxY );
		opt->setbReset( false );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
		
		refresh = true;
	}else if( evt.rbutton() ){

		m_mzRange.first = minX;
		m_mzRange.second = maxX;
		m_rtRange.first = minY;
		m_rtRange.second = maxY;

		// 範囲選択されているか判定
		bool flgXSelected = true;
		bool flgYSelected = true;

		if( flgRt ){
			if( xPos0 == evt.x() ){
				flgXSelected = false;
			}
		}else{
			if( xPos0 == xPos1 ){
				flgXSelected = false;
			}
		}

		if( flgMz ){
			if( yPos0 == evt.y() ){
				flgYSelected = false;
			}
		}else{
			if( yPos0 == yPos1 ){
				flgYSelected = false;
			}
		}

		// 範囲選択されているなら、右ドラッグ用メニューを表示
		if( ( !flgXSelected && !flgYSelected )
				|| ( fabs( graphics.getMaxX() - graphics.getMinX() ) < 0.105
				&& fabs( graphics.getMaxY() - graphics.getMinY() ) < 0.105 ) ) {
			// 範囲選択されていない
		}else{
			// 範囲選択されている 

			kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( canvas );
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
			kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );

			if( c != NULL && c == acCanvas ) {
				if( wxWindow::GetCapture() == c ) {
					c->ReleaseMouse();
				}
				wxMenu* menu = getDatamapRangeMenu();
				c->PopupMenu( menu );

				refresh = true;
			}
		}
	}

	return refresh;
}

// draw foreground ( data map )
void DataMapRangeManager::drawForeground(
		kome::objects::DataMapInfo& dataMap,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// check the object
	if( m_changing.dataMap != &dataMap ) {
		return;
	}

	// color
	COLOR oldColor = graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = graphics.getDrawStyle();
	graphics.setColor( kome::img::ColorTool::GRAY );
	graphics.setDrawStyle( kome::img::Graphics::STYLE_DOT );

	// rect
	int xPos0 = m_changing.startPos >> 16;
	int yPos0 = m_changing.startPos & 0xffff;
	int xPos1 = m_changing.endPos >> 16;
	int yPos1 = m_changing.endPos & 0xffff;

	xPos0 = CLAMP( xPos0, graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
	xPos1 = CLAMP( xPos1, graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
	yPos0 = CLAMP( yPos0, graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );
	yPos1 = CLAMP( yPos1, graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );

	// RT軸が選択されている場合は、RT範囲を囲む(x軸方向=m/zは、表示されている範囲が選択されているものとして扱う) // @date 2013/02/14 <Add> OKADA
	if( m_flgRt ){
		xPos0 = graphics.getGraphPositionLeft();
		xPos1 = graphics.getGraphPositionRight();
	}
	// m/z軸が選択されている場合は、m/z範囲を囲む(y軸方向=RTは、表示されている範囲が選択されているものとして扱う)	// @date 2013/02/14 <Add> OKADA
	if( m_flgMz ){
		yPos0 = graphics.getGraphPositionTop();
		yPos1 = graphics.getGraphPositionBottom();
	}

	// draw rect
	int rectPts[] = {
		xPos0, yPos0,
		xPos0, yPos1,
		xPos1, yPos1,
		xPos1, yPos0,
		xPos0, yPos0
	};

	graphics.drawLines( rectPts, 5 );

	// restore
	graphics.setColor( oldColor );
	graphics.setDrawStyle( oldStyle );
}

// toggle data map range status
void DataMapRangeManager::toggleDataMapRangeStatus() {
	
	// @date 2011.08.22 <Mod> M.Izumi ->
	// Spec No.82145 : Cannot use Synchronize Axes function against heatmaps 
	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string strStatus = statusMgr.getValue( RANGE_STATUS_KEY );
	// set flag
	if( strStatus.compare( "on" )  == 0 ){
		m_syncDataMap = true;
	}else{
		m_syncDataMap = false;
	}
	// <-

	// range stack
	if( m_syncDataMap ) {		// synchronized
		// clear stack
		// @date 2011.09.07 <Mod> M.Izumi
		while( !m_syncDataMapRange.xRange.empty() ){
			m_syncDataMapRange.xRange.pop();
		}
		while( !m_syncDataMapRange.yRange.empty() ){
			m_syncDataMapRange.yRange.pop();
		}

		// data map
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
		if( frame != NULL ) {
			kome::objects::MsDataVariant obj( frame->getActiveObject() );
			kome::objects::DataMapInfo* dm = obj.getDataMap();
			if( dm != NULL ) {
				m_syncDataMapRange = m_rangeMap[ dm ];
			}
		}

		// set range
		if( !setSynchronizedDataMapRange( NULL ) ) {
			return;
		}
		
		// clear map
		m_rangeMap.clear();
	}
	else {
		unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
		for( unsigned int i = 0; i < num; i++ ) {
			// canvas
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
			kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
			
			// data map
			kome::objects::MsDataVariant obj( frame->getActiveObject() );			
			kome::objects::DataMapInfo* dm = obj.getDataMap();

			if( dm != NULL ) {	
				// SPEC 82323 :Cannot use Synchronize Axes function against heatmaps (again)  @date 2011.09.07 <Mod> M.Izumi
				// copy
				if( m_syncDataMapRange.xRange.empty() || m_syncDataMapRange.yRange.empty() ) {
					RangeInfo& range = m_rangeMap[ dm ];
					m_rangeMap[ dm ] = m_syncDataMapRange;
					// intensity range
					double minInt = 0.0;
					double maxInt = 1.0;
					for( unsigned int j = 0; j < dm->getRowSize(); j++ ) {
						for( unsigned int k = 0; k < dm->getColSize(); k++ ) {
							double intensity = dm->getIntensity( j, k );
							minInt = std::min( intensity, minInt );
							maxInt = std::max( intensity, maxInt );
						}
					}
					dm->setIntensityRange( minInt, maxInt );
					
					setDataMapRange( dm, canvas, true );
				}else{
					m_rangeMap[ dm ] = m_syncDataMapRange;		// @date 2011/09/08 <Add> OKADA
					// update
					kome::plugin::PluginCallTool::onUpdateDataMap( *dm );
				}
			}
		}
	}

	// refresh
	kome::window::WindowTool::refresh();
}

// set data map range
bool DataMapRangeManager::setDataMapRange( kome::objects::DataMapInfo* dataMap, wxWindow* canvas, bool bflg ) {
	// auto commit
	bool autoCommit = dataMap->isAutoCommit();
	dataMap->setAutoCommit( false );

	// get range
	RangeInfo& range = m_rangeMap[ dataMap ];

	// progress
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );
	kome::core::Progress* pprogress = NULL;
	// @date 2011.11.30 <Add> M.Izumi
	if( !bflg ){
		pprogress = &progress;
	}

	if( pprogress == NULL ){
		pprogress = &kome::core::Progress::getIgnoringProgress();
	}

	// range
	double minRt = -1.0;
	double maxRt = -1.0;
	double minMz = -1.0;
	double maxMz = -1.0;
	if(  range.xRange.empty() || range.yRange.empty()  ) {  // get range
		// group
		kome::objects::DataGroupNode& group = dataMap->getGroup();

		for( unsigned int i = 0; i < group.getNumberOfSpectra() && !pprogress->isStopped(); i++ ) {
			// spectrum
			kome::objects::Spectrum* spec = group.getSpectrum( i );

			// >>>>>> @Date:2014/02/18	<Add>	A.Ozaki
			//  SPEC97099
			//   範囲を求めるのはMS1のみに限定
			//
			if  ( (kome::objects::Spectrum *)NULL == spec )
			{
				continue;
			}
			if  ( 1 != spec->getMsStage( ) )
			{
				continue;
			}
			//
			// <<<<<< @Date:2014/02/18	<Add>	A.Ozaki

			// RT
			double rt = spec->getRt();
			if( minRt < 0.0 || rt < minRt ) {
				minRt = rt;
			}
			if( maxRt < 0.0 || rt > maxRt ) {
				maxRt = rt;
			}

			// m/z
			if( minMz < 0.0 || spec->getMinX() < minMz ) {
				minMz = spec->getMinX();
			}
			if( maxMz < 0.0 || spec->getMaxX() > maxMz ) {
				maxMz = spec->getMaxX();
			}
		}
	}
	else {
		minRt = range.yRange.top().first;
		maxRt = range.yRange.top().second;
		minMz = range.xRange.top().first;
		maxMz = range.xRange.top().second;
	}

	DataInfo* dataInfo = getData( &( dataMap->getGroup() ), minRt, maxRt, minMz, maxMz );

	if( dataInfo != NULL ) {
		if( !dataMap->loadDataMap( dataInfo->path.c_str() ) ) {
			dataInfo = NULL;
		}
	}

	if( dataInfo == NULL ) {
		// @date 2012.09.07 <Mod> M.Izumi ->
		// get ini file
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		std::string filterName = ini->getString( SECTION, KEY, DEFAULT );
		bool bflg = dataMap->setRange( minMz, maxMz, minRt, maxRt, filterName.c_str(), *pprogress  );		
		// @date 2012.09.07 <Mod> M.Izumi <-

		// commit, rollback
		if( !bflg ) {
			dataMap->rollback();
			return false;
		}
		else {
			dataMap->commit();
			saveData( *dataMap );
			kome::plugin::PluginCallTool::onUpdateDataMap( *dataMap );
			//pprogress->fill();
		}
	}

	pprogress->fill();

	/*else {
		pprogress->fill();
	}*/

	return !( pprogress->isStopped() );
}

// set synchronized data map range
bool DataMapRangeManager::setSynchronizedDataMapRange( kome::objects::DataMapInfo* appendedMap ) {
	// data maps
	bool appendFlg = ( appendedMap != NULL );
	std::vector< std::pair< kome::window::Canvas*, kome::objects::DataMapInfo* > > canvases;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// canvas
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );

		// object
		kome::objects::MsDataVariant obj;
		if( canvas != NULL ) {
			obj.setVariant( canvas->getActiveObject() );
		}

		kome::objects::DataMapInfo* dataMap = obj.getDataMap();

		if( canvas != NULL && dataMap != NULL ) {
			canvases.push_back( std::make_pair( canvas, dataMap ) );
			if( dataMap == appendedMap ) {
				appendFlg = false;
			}
		}
	}

	if( appendFlg ) {
		canvases.push_back( std::make_pair( (kome::window::Canvas*)NULL, appendedMap ) );
	}

	// range
	double minRt = -1.0;
	double maxRt = -1.0;
	double minMz = -1.0;
	double maxMz = -1.0;
	if( m_syncDataMapRange.xRange.empty() || m_syncDataMapRange.yRange.empty() ) {  // get range
		for( unsigned int i = 0; i < canvases.size(); i++ ) {
			// data map
			kome::objects::DataMapInfo* dataMap = canvases[ i ].second;
			kome::objects::DataGroupNode& group = dataMap->getGroup();

			// spectra
			for( unsigned int j = 0; j < group.getNumberOfSpectra(); j++ ) {
				// spectrum
				kome::objects::Spectrum* spec = group.getSpectrum( j );

				// RT
				double rt = spec->getRt();
				if( minRt < 0.0 || rt < minRt ) {
					minRt = rt;
				}
				if( maxRt < 0.0 || rt > maxRt ) {
					maxRt = rt;
				}

				// m/z
				if( minMz < 0.0 || spec->getMinX() < minMz ) {
					minMz = spec->getMinX();
				}
				if( maxMz < 0.0 || spec->getMaxX() > maxMz ) {
					maxMz = spec->getMaxX();
				}
			}
		}
	}
	else {
		minRt = m_syncDataMapRange.yRange.top().first;
		maxRt = m_syncDataMapRange.yRange.top().second;
		minMz = m_syncDataMapRange.xRange.top().first;
		maxMz = m_syncDataMapRange.xRange.top().second;
	}

	double midRt = ( minRt + maxRt ) / 2.0;
	minRt = std::min( minRt, midRt - X_MINIMUM_RANGE );
	minRt = std::max( minRt, 0.0 );
	maxRt = std::max( minRt + X_MINIMUM_RANGE, maxRt );

	double midMz = ( minMz + maxMz ) / 2.0;
	minMz = std::min( minMz, midMz - X_MINIMUM_RANGE );
	minMz = std::max( minMz, 0.0 );
	maxMz = std::max( minMz + X_MINIMUM_RANGE, maxMz );

	// get data maps
	std::vector< std::pair< kome::objects::DataMapInfo*, bool > > dataMaps;
	for( unsigned int i = 0; i < canvases.size(); i++ ) {
		kome::objects::DataMapInfo* dataMap = canvases[ i ].second;
		if( !dataMap->isCompleted()
				|| fabs( minRt - dataMap->getMinRt() ) > 0.001 
				|| fabs( maxRt - dataMap->getMaxRt() ) > 0.001
				|| fabs( minMz - dataMap->getMinMz() ) > 0.001
				|| fabs( maxMz - dataMap->getMaxMz() ) > 0.001
		) {
			dataMaps.push_back( std::make_pair( dataMap, dataMap->isAutoCommit() ) );
		}
	}

	// update data maps
	std::vector< kome::objects::DataMapInfo* > saveDataMaps;
	if( dataMaps.size() > 0 ) {
		// progress
		kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );
		progress.createSubProgresses( dataMaps.size() );

		// get data
		for( unsigned int i = 0; i < dataMaps.size() && !progress.isStopped(); i++ ) {
			// data map
			kome::objects::DataMapInfo* dataMap = dataMaps[ i ].first;

			// get
			dataMap->setAutoCommit( false );

			DataInfo* dataInfo = getData( &( dataMap->getGroup() ), minRt, maxRt, minMz, maxMz );
			if( dataInfo != NULL ) {
				if( dataMap->loadDataMap( dataInfo->path.c_str() ) ) {
					progress.getSubProgress( i )->fill();
				}
				else {
					dataInfo = NULL;
				}
			}

			if( dataInfo == NULL ) {
				// @date 2012.09.07 <Mod> M.Izumi ->
				// get ini file
				kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
				kome::core::IniFile* ini = msppMgr.getIniFile();

				std::string filterName = ini->getString( SECTION, KEY, DEFAULT );

				dataMap->setRange( minMz, maxMz, minRt, maxRt, filterName.c_str(), *( progress.getSubProgress( i ) ) );
				// @date 2012.09.07 <Mod> M.Izumi <-
				saveDataMaps.push_back( dataMap );
			}
		}

		// commit, rollback
		for( unsigned int i = 0; i < dataMaps.size(); i++ ) {
			// data map
			kome::objects::DataMapInfo* dataMap = dataMaps[ i ].first;
			dataMap->setAutoCommit( dataMaps[ i ].second );

			// commit, rollback
			if( progress.isStopped() ) {
				dataMap->rollback();
			}
			else {
				dataMap->commit();
			}		
		}

		// save data
		if( !progress.isStopped() ) {
			for( unsigned int i = 0; i < saveDataMaps.size(); i++ ) {
				saveData( *( saveDataMaps[ i ] ) );
			}
		}

		// check progress
		if( progress.isStopped() ) {
			return false;
		}
	}

	// get intensity range
	double minInt = 0.0;
	double maxInt = 1.0;

	for( unsigned int i = 0; i < canvases.size(); i++ ) {
		kome::objects::DataMapInfo* dataMap = canvases[ i ].second;
		minInt = std::min( dataMap->getMinIntensity(), minInt );
		maxInt = std::max( dataMap->getMaxIntensity(), maxInt );
	}

	// set intensity range
	for( unsigned int i = 0; i < canvases.size(); i++ ) {
		kome::objects::DataMapInfo* dataMap = canvases[ i ].second;
		dataMap->setIntensityRange( minInt, maxInt );

		// update
		kome::plugin::PluginCallTool::onUpdateDataMap( *dataMap );
	}

	return true;
}

// on close sample
void DataMapRangeManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	for( int i = (int)m_dataArray.size() - 1; i >= 0; i-- ) {
		if( m_dataArray[ i ].group->getSample() == sample ) {
			removefile( m_dataArray[ i ].path.c_str() );
			m_dataArray.erase( m_dataArray.begin() + i );
		}
	}
}

// get object
DataMapRangeManager& DataMapRangeManager::getInstance() {
	// create object (This is the only object)
	static DataMapRangeManager mgr;

	return mgr;
}

// reset zoom datamap from context menu manager
bool DataMapRangeManager::resetZoomDatamapFromContextMenuManager( kome::objects::DataMapInfo& dataMap, wxWindow* canvas ){

	// flags
	bool refresh = false;

	// active data
	m_changing.dataMap = &dataMap;	// @date 2012/12/03 <Add> OKADA

	RangeInfo* ptrRange = getRange( &dataMap );

// >>>>>>	@Date:2012/05/30	<Add>	A.Ozaki
// OperationLog対応につき、リセット時にはrange内の情報が空でも
// 動作できるようにしないといけません
//   空での最初の動作だけは出来るようにするための処置
	bool	bFirst = true;


	while( ptrRange != NULL ){

		// check range
		if( true == bFirst || !ptrRange->xRange.empty() && !ptrRange->yRange.empty() ) {

			double minX = 0.0;
			double maxX = 0.0;
			double minY = 0.0;
			double maxY = 0.0;

			bFirst = false;		// @Date:2013/05/30	<Add>	A.Ozaki

			if( !ptrRange->xRange.empty() ){
				minX = ptrRange->xRange.top().first;
				maxX = ptrRange->xRange.top().second;
				ptrRange->xRange.pop();		// @Date:2013/05/30	<Move>	A.Ozaki
			}
			if( !ptrRange->yRange.empty() ){
				minY = ptrRange->yRange.top().first;
				maxY = ptrRange->yRange.top().second;
				ptrRange->yRange.pop();		// @Date:2013/05/30	<Move>	A.Ozaki
			}

			// update
			if( m_syncDataMap ) {
				refresh = setSynchronizedDataMapRange( NULL );
			}
			else {
				refresh = setDataMapRange( &dataMap, canvas, true );
			}

			// restore range
			if( !refresh ) {
				ptrRange->xRange.push( std::make_pair( minX, maxX ) );
				ptrRange->yRange.push( std::make_pair( minY, maxY ) );
			}
		}
		else{
			break;
		}
	}
	// >>>>>>	@Date:2012/05/30	<Add>	A.Ozaki
	// 3D Viewも更新(SPEC 89152)
	if( refresh ){
		kome::plugin::PluginCallTool::onUpdateDataMap( dataMap );
	}

	// set Null
	m_changing.dataMap = NULL;

	return refresh;
}

// open change displayed dialog
void DataMapRangeManager::openChangeDispRangeDlg( kome::objects::DataMapInfo& dataMap, wxWindow* canvas ){
	DataMapChangeDispRangeDialog dlg( kome::window::WindowTool::getMainWindow() );

	dlg.setStartMz( m_mzRange.first );
	dlg.setEndMz( m_mzRange.second );
	dlg.setStartRt( m_rtRange.first );
	dlg.setEndRt( m_rtRange.second );
	
	if( dlg.ShowModal() == wxID_OK ){
		RangeInfo* range = getRange();
		if( range == NULL ){
			return;
		}

		// Add Operation
		DataMapRangeOperation* opt = new DataMapRangeOperation();
		opt->setShortName( "datamp_range" );
		opt->setOperationName( "DataMap Range" );
		opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( dataMap.getSample() );
		opt->setCanvas( canvas );
		opt->setDataMapInfo( &dataMap );
		opt->setStartMz( dlg.getStartMz() );
		opt->setEndMz( dlg.getEndMz() );
		opt->setStartRt( dlg.getStartRt() );
		opt->setEndRt( dlg.getEndRt() );
		opt->setbReset( false );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

}

// set Range Info
bool DataMapRangeManager::setRange( 
	//RangeInfo range, 
	kome::objects::DataMapInfo* dataMap, 
	wxWindow* canvas, 
	double minX, 
	double maxX, 
	double minY, 
	double maxY 
){

	RangeInfo* range = getRange( dataMap );
	if( range == NULL ){
		return false;
	}
	// push range
	range->xRange.push( std::make_pair( minX, maxX) );
	range->yRange.push( std::make_pair( minY, maxY ) );

	// update data map
	bool updated = false;
	if( m_syncDataMap ) {
		updated = setSynchronizedDataMapRange( NULL );
	}
	else {
		updated = setDataMapRange( dataMap, canvas );
	}
		
	// restore
	if( !updated ) {
		range->xRange.pop();
		range->yRange.pop();
	}

	return updated;
}

// get Rt Range
void DataMapRangeManager::getRtRange( double* dFirst, double* dSecond )
{
	*dFirst = m_rtRange.first;
	*dSecond = m_rtRange.second;
}

// get Mz Range
void DataMapRangeManager::getMzRange( double* dFirst, double* dSecond )
{
	*dFirst = m_mzRange.first;
	*dSecond = m_mzRange.second;
}

// double click button (data map)
bool DataMapRangeManager::dblClick(
		kome::objects::DataMapInfo& dataMap,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// range
	RangeInfo* range = getRange();
	if( range == NULL ){
		m_flgClickRight = false;		// @date 2013/02/12 <Add> OKADA
		return false;
	}

	// refresh flag
	bool refresh = false;

	// active object
	m_changing.dataMap = NULL;

	// position
	int xPos = evt.x();
	int yPos = evt.y();

	// set position
	int xPos0 = evt.x();
	int yPos0 = evt.y();
	int xPos1 = evt.x();
	int yPos1 = evt.y();

	// rect
	double l0 = (double)( MIN( xPos0, xPos1 ) );
	double l1 = l0 - 1.0;
	double r0 = (double)( MAX( xPos0, xPos1 ) );
	double r1 = r0 + 1.0;
	double t0 = (double)( MIN( yPos0, yPos1 ) );
	double t1 = t0 - 1.0;
	double b0 = (double)( MAX( yPos0, yPos1 ) );
	double b1 = b0 + 1.0;

	// get range
	double minX0 = double();
	double minX1 = double();
	double maxX0 = double();
	double maxX1 = double();
	double minY0 = double();
	double minY1 = double();
	double maxY0 = double();
	double maxY1 = double();

	graphics.transformPositionToData( l0, b0, &minX0, &minY0 );
	graphics.transformPositionToData( l1, b1, &minX1, &minY1 );
	graphics.transformPositionToData( r0, t0, &maxX0, &maxY0 );
	graphics.transformPositionToData( r1, t1, &maxX1, &maxY1 );

	double dMzVal = dMzVal = minX0;	// ポイントされている箇所のMz値
	double dRtVal = dRtVal = minY0;	// ポイントされている箇所のRt値

	double minX = ( minX0 + minX1 ) / 2.0;		
	double maxX = ( maxX0 + maxX1 ) / 2.0;
	double minY = ( minY0 + minY1 ) / 2.0;
	double maxY = ( maxY0 + maxY1 ) / 2.0;

	double midX = ( minX + maxX ) / 2.0;
	minX = std::min( midX - 0.05, minX );
	minX = std::max( graphics.getMinX(), minX );	// graphics.getMinX()= X(横)軸方向の最小値。m/zの最小値。表示中の値
	maxX = std::max( minX + 0.1, maxX );
	maxX = std::min( graphics.getMaxX(), maxX );	// graphics.getMaxX()= X(横)軸方向の最大値。m/zの最大値。表示中の値
	minX = std::min( minX, maxX - 0.1 );

	double midY = ( minY + maxY ) / 2.0;
	minY = std::min( midY - 0.05, minY );
	minY = std::max( graphics.getMinY(), minY );	// graphics.getMinY()= Y(縦)軸方向の最小値。RTの最小値。表示中の値
	maxY = std::max( minY + 0.1, maxY );
	maxY = std::min( graphics.getMaxY(), maxY );	// graphics.getMaxY()= Y(縦)軸方向の最大値。RTの最大値。表示中の値
	minY = std::min( minY, maxY - 0.1 );
	
	if( evt.lbutton() ){
		// 左double click

		// Clickされた領域の判定
		// RT軸領域？
		bool flgInRt=false;
		if( dMzVal < graphics.getMinX() ){
			flgInRt = true;
		}

		// m/z軸領域？
		bool flgInMz=false;
		if( dRtVal < graphics.getMinY() ){
			flgInMz = true;
		}

		if(( flgInRt == true )&&( flgInMz == true )){
			// 原点左下領域
			refresh = false;	// なにもしない
		}else if( flgInRt == true ){
			// RT軸領域でダブルクリックされた
			// Rangeのx,y片方だけpopするのは問題があり、全部リセットすることになった

			DataMapRangeOperation* opt = new DataMapRangeOperation();
			opt->setShortName( "datamp_range" );
			opt->setOperationName( "DataMap Range" );
			opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
			opt->setTargetSample( dataMap.getSample() );
			opt->setCanvas( canvas );
			opt->setDataMapInfo( &dataMap );
			opt->setStartMz( dataMap.getMinMz() );
			opt->setEndMz( dataMap.getMaxMz() );
			opt->setStartRt( dataMap.getMinRt() );
			opt->setEndRt( dataMap.getMaxRt() );
			opt->setbReset( true );

			if( opt->execute() ){
				kome::window::WindowTool::refresh();
			}else{
				delete opt;
			}
			// 解決案（リセットしたい方のstackのbottomを取り出し、リセットしたくない方のtopと併せてpushする）も考えられたが、今回は見送り
			refresh = true;
		}else if( flgInMz == true ){
			// m/z軸領域でダブルクリックされた
			// Rangeのx,y片方だけpopするのは問題があり、全部リセットすることになった
			DataMapRangeOperation* opt = new DataMapRangeOperation();
			opt->setShortName( "datamp_range" );
			opt->setOperationName( "DataMap Range" );
			opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
			opt->setTargetSample( dataMap.getSample() );
			opt->setCanvas( canvas );
			opt->setDataMapInfo( &dataMap );
			opt->setStartMz( dataMap.getMinMz() );
			opt->setEndMz( dataMap.getMaxMz() );
			opt->setStartRt( dataMap.getMinRt() );
			opt->setEndRt( dataMap.getMaxRt() );
			opt->setbReset( true );

			if( opt->execute() ){
				kome::window::WindowTool::refresh();
			}else{
				delete opt;
			}
			refresh = true;
		}else{
			// heatmap 領域内でのダブルクリックならこの処理を実行
			DataMapRangeOperation* opt = new DataMapRangeOperation();
			opt->setShortName( "datamp_range" );
			opt->setOperationName( "DataMap Range" );
			opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
			opt->setTargetSample( dataMap.getSample() );
			opt->setCanvas( canvas );
			opt->setDataMapInfo( &dataMap );
			opt->setStartMz( dataMap.getMinMz() );
			opt->setEndMz( dataMap.getMaxMz() );
			opt->setStartRt( dataMap.getMinRt() );
			opt->setEndRt( dataMap.getMaxRt() );
			opt->setbReset( true );

			if( opt->execute() ){
				kome::window::WindowTool::refresh();
			}else{
				delete opt;
			}
			refresh = true;
		}
		return refresh;	// refresh

	}

	return refresh;
}

// set changing datamap info
void DataMapRangeManager::setChangingDataMapInfo( kome::objects::DataMapInfo* dataMapInfo ){
	m_changing.dataMap = dataMapInfo;
}
