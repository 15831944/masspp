/**
 * @file SelectRangeManager.cpp
 * @brief implements of SelectRangeManager class
 *
 * @author S.Tanaka
 * @date 2007.03.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 
 *  <補足説明>

 * ・Version 2.0.0 での変更点  2011.05.19 <Mod> M.Izumi
   【マウス操作】
   ・X軸制御領域
   　なし       … カーソル位置の座標をステータスバー（coordinate）に表示
	 左クリック … 
	 左ドラッグ … X指定ズーム
	 右クリック … コンテクストメニュー表示（内容がまだ決まっていません）
	 右ドラッグ … MC/BPC表示選択ダイアログを起動
	 左Dクリック… X,Yともパン、ズーム処理をリセットして全体表示	   
 
 ・@※ 変更箇所が多いため変更箇所は削除しています。
   　 必要そうな箇所には、コメントを残しています。

 */

#include "stdafx.h"
#include "SelectRangeManager.h"

#include <boost/format.hpp>
#include <wx/window.h>

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define STATUS_BAR_COORDINATE				"coordinate"
#define STATUS_BAR_RANGE					"range"
#define SPEC_RANGE							"spectrum_range"
#define CHROM_RANGE							"chromatogram_range"

#define SELECT_RANGE						"Select Range"
#define ICON_NAME_MOVE_X					"move_selectRange"
#define ICON_NAME_XRANGE_ZOOM				"xrange_zoom"
#define ICON_NAME_MC_BPC					"mc_bpc"

#define SPECTRUM_RANGE_ACTION_STATUS_NAME	"SPECTRUM_RANGE_ACTION"
#define CHROM_RANGE_ACTION_STATUS_NAME		"CHROMATOGRAM_RANGE_ACTION"
#define BASE_MENU_ID                        13579

wxWindow* iFocusID = NULL;

// active canvas の各オブジェクト（Stacked用） @date 2011.09.07 <Add> M.Izumi
kome::objects::Spectrum* m_selSpec = NULL;
kome::objects::Chromatogram* m_selChrom = NULL;

// constructor
SelectRangeManager::SelectRangeManager() {
	m_activeData.obj	   = NULL;
	m_activeData.startPos  = -1;
	m_px = -1;
	m_py = -1;
	m_start = -1.0;
	m_end = -1.0;
	m_specMenu = NULL;
	m_chromMenu = NULL;
}

// destructor
SelectRangeManager::~SelectRangeManager() {	
	if( m_specMenu != NULL ) {
		delete m_specMenu;
	}
	if( m_chromMenu != NULL ) {
		delete m_chromMenu;
	}
}

// close spectrum
void SelectRangeManager::close( kome::objects::Spectrum& spec ) {
	
	if( m_specRangeMap.find( &spec ) != m_specRangeMap.end() ) {
		m_specRangeMap.erase( &spec );
	}
}

// close chromatogram
void SelectRangeManager::close( kome::objects::Chromatogram& chrom ) {
	
	if( m_chromRangeMap.find( &chrom ) != m_chromRangeMap.end() ) {
		m_chromRangeMap.erase( &chrom );
	}
}

// down button (spectrum)
bool SelectRangeManager::downButton(
	kome::objects::Spectrum& spec,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics
) {
	bool refresh = false;
	int px = evt.x();
	int py = evt.y();
	
	// 右クリック
	if( evt.rbutton() || evt.lbutton() ) {   // start to select
		if( evt.lbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_XRANGE_ZOOM );
		}else if( evt.rbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MC_BPC );
		}

		m_btnDown = true;	// @date 2011.07.14 <Add> M.Izumi
		
		if( px >= graphics.getGraphPositionLeft()
				&& px <= graphics.getGraphPositionRight()
				&& py >= graphics.getGraphPositionTop()
				&& py <= graphics.getGraphPositionBottom()
		) {
			// set active data
			m_activeData.chromFlag = false;
			m_activeData.obj = &spec;
			m_activeData.startPos = px;

			// set coordinate
			double x = double();
			double y = double();
			graphics.transformPositionToData( px, py, &x, &y );

			// create pair
			m_specRangeMap[ &spec ] = std::make_pair( x, x );

			// refresh
			refresh = true;	
		}
	}
	
	return refresh;
}

// down button (chromatogram)
bool SelectRangeManager::downButton(
	kome::objects::Chromatogram& chrom,
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics
) {
	bool refresh = false;
	int px = evt.x();
	int py = evt.y();
	
	// 右クリック
	if( evt.rbutton() || evt.lbutton() ) {   // start to select
		if( evt.lbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_XRANGE_ZOOM );
		}else if( evt.rbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MC_BPC );
		}

		m_btnDown = true;	// @date 2011.07.14 <Add> M.Izumi

		if( px >= graphics.getGraphPositionLeft()
				&& px <= graphics.getGraphPositionRight()
				&& py >= graphics.getGraphPositionTop()
				&& py <= graphics.getGraphPositionBottom()
		) {
			// set active data
			m_activeData.chromFlag = true;
			m_activeData.obj = &chrom;
			m_activeData.startPos = px;

			// set coordinate
			double x = double();
			double y = double();
			graphics.transformPositionToData( px, py, &x, &y );

			// create pair
			m_chromRangeMap[ &chrom ] = std::make_pair( x, x );

			// refresh
			refresh = true;
		}
	}
	
	return refresh;
}

// move mouse (spectrum)
bool SelectRangeManager::moveMouse(
	kome::objects::Spectrum& spec,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics
) {
	m_px = evt.x();
	m_py = evt.y();
	
	 //check active data
	if( m_activeData.chromFlag || m_activeData.obj != &spec ) {
		return false;
	}
	bool refresh = false;
	
	// set selected range
	std::string strRangeFirst;
	std::string strRangeSecond;
	std::string strDisp;
	
	if( evt.rbutton() || evt.lbutton() ) {   
		if( evt.lbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_XRANGE_ZOOM );
		}else if( evt.rbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MC_BPC );
		}
		m_btnDown = true;	// @date 2011.07.14 <Add> M.Izumi
		kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
		statusBar.setFlgDisp( STATUS_BAR_COORDINATE, 1 );	// ステータスバーを有効化
		statusBar.setStatusMsg(STATUS_BAR_COORDINATE,"");
		statusBar.setFlgDisp( STATUS_BAR_RANGE, 1 );			// ステータスバーを有効化
	
		
		setSelectedRange( m_specRangeMap[ &spec ], m_activeData.startPos, evt.x(), graphics );
		
		std::pair< double, double >& range = m_specRangeMap[ &spec ];
		strRangeFirst = (boost::format("%.2f") % range.first).str();
		strRangeSecond = (boost::format("%.2f") % range.second).str();
		strDisp =  strRangeFirst + ", " + strRangeSecond ;	

		statusBar.setStatusMsg(STATUS_BAR_RANGE, strDisp.c_str());
		refresh = true;
	}
	
	return refresh;
}

// move mouse (chromatogram)
bool SelectRangeManager::moveMouse(
	kome::objects::Chromatogram& chrom,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics
) {
	m_px = evt.x();
	m_py = evt.y();
	// check active data
	if( !m_activeData.chromFlag || m_activeData.obj != &chrom ) {
		return false;
	}

	// set selected range
	bool refresh = false;

	std::string strRangeFirst;
	std::string strRangeSecond;
	std::string strDisp;

	// dragging
	if( evt.rbutton() || evt.lbutton() ) {   
		if( evt.lbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_XRANGE_ZOOM );
		}else if( evt.rbutton() ){
			setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MC_BPC );
		}
		
		m_btnDown = true;	// @date 2011.07.14 <Add> M.Izumi

		kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
		statusBar.setFlgDisp( STATUS_BAR_COORDINATE, 1 );	// ステータスバーを有効化
		statusBar.setStatusMsg(STATUS_BAR_COORDINATE,"");
		statusBar.setFlgDisp( STATUS_BAR_RANGE, 1 );			// ステータスバーを有効化
		
		setSelectedRange( m_chromRangeMap[ &chrom ], m_activeData.startPos, evt.x(), graphics );

		std::pair< double, double >& range = m_chromRangeMap[ &chrom ];
		
		strRangeFirst = (boost::format("%.2f") % range.first).str();
		strRangeSecond = (boost::format("%.2f") % range.second).str();
		strDisp =  strRangeFirst + ", " + strRangeSecond ;	
		statusBar.setStatusMsg(STATUS_BAR_RANGE, strDisp.c_str());

		refresh = true;
	}

	return refresh;
}

// up button (spectrum)
bool SelectRangeManager::upButton(
	kome::objects::Spectrum& spec,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// マウスカーソル変更
	setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MOVE_X );
	m_btnDown = false;
	// check active data
	if( m_activeData.chromFlag || m_activeData.obj != &spec ) {
		return false;
	}
	
	bool refresh = false;

	// 右ドラッグ終了
	if( evt.rbutton() ) {   
		setSelectedRange(m_specRangeMap[ &spec ], m_activeData.startPos, evt.x(), graphics );
		
		std::pair< double, double >& range = m_specRangeMap[ &spec ];
		
		if( range.first != range.second ){
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
			kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
			kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( canvas );
			
			if( c != NULL && c == acCanvas ){
				if( wxWindow::GetCapture() == c ) {
					c->ReleaseMouse();
				}
				wxMenu* menu = getSpecRangeMenu();
				setCurrentRange( (double)MIN( range.first, range.second ), (double)MAX( range.first, range.second ) );
				c->PopupMenu( menu );
				refresh = true;
			}				
		}	
	// 左ドラッグ終了	
	}else if( evt.lbutton() ){
		std::pair< double, double >& range = m_specRangeMap[ &spec ];
		
		if( range.first != range.second ){			
			kome::objects::Parameters params;
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		
			kome::plugin::PluginCallTool::setSpectrum( params, spec );
			kome::plugin::PluginCallTool::setStartOfRange( params, (double)MIN( range.first, range.second ) );
			kome::plugin::PluginCallTool::setEndOfRange( params, (double)MAX( range.first, range.second ) );
			kome::plugin::PluginCallTool::setCanvas( params, canvas );

			plgMgr.callAllFunctions( &params, SPEC_RANGE );
			refresh = true;
		}
	}
	
	// unset active data
	m_activeData.obj = NULL;
	m_specRangeMap.erase( &spec );//選択範囲解除		
	
	return refresh;
}

// up button (chromatogram)
bool SelectRangeManager::upButton(
	kome::objects::Chromatogram& chrom,
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	// マウスカーソルアイコン設定
	setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MOVE_X );
	m_btnDown = false;
	// check active data
	if( !m_activeData.chromFlag || m_activeData.obj != &chrom ) {
		return false;
	}
		
	// set selected range
	bool refresh = false;
		
	// 右ドラッグ終了
	if( evt.rbutton() ) {  
		setSelectedRange( m_chromRangeMap[ &chrom ], m_activeData.startPos, evt.x(), graphics );
			
		std::pair< double, double >& range = m_chromRangeMap[ &chrom ];
				
		// ダイアログ表示
		if( range.first != range.second ){
			kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( canvas );
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
			kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
			if( c != NULL && c == acCanvas ) {
				if( wxWindow::GetCapture() == c ) {
					c->ReleaseMouse();
				}
				wxMenu* menu = getChromRangeMenu();
				setCurrentRange( (double)MIN( range.first, range.second ), (double)MAX( range.first, range.second ) );
				c->PopupMenu( menu );
				refresh = true;
			}
		}

	// 左ドラッグ終了	
	}else if( evt.lbutton() ){
		std::pair< double, double >& range = m_chromRangeMap[ &chrom ];
		
		if( range.first != range.second ){		
			kome::objects::Parameters params;
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		
			kome::plugin::PluginCallTool::setChromatogram( params, chrom );
			kome::plugin::PluginCallTool::setStartOfRange( params, (double)MIN( range.first, range.second ) );
			kome::plugin::PluginCallTool::setEndOfRange( params, (double)MAX( range.first, range.second ) );
			kome::plugin::PluginCallTool::setCanvas( params, canvas );

			plgMgr.callAllFunctions( &params, CHROM_RANGE);
			refresh = true;
		}		
	}	
	
	// unset active data
	m_activeData.obj = NULL;
	m_chromRangeMap.erase( &chrom );//選択範囲解除
	
	return refresh;
}

// double click (spectrum)
bool SelectRangeManager::dblClick(
	kome::objects::Spectrum& spec,
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	// return value
	bool ret = false;

	// range action
	if( canvas != NULL && evt.rbutton()
			&& evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {
		// coordinate
		double x = double();
		double y = double();
		graphics.transformPositionToData( evt.x(), evt.y(), &x, &y );
		setCurrentRange( x, x );

		// spectrum menu
		wxMenu* menu = getSpecRangeMenu();
		canvas->PopupMenu( menu );

		ret = true;
	}

	return ret;
}

// double click (chromatogram)
bool SelectRangeManager::dblClick(
	kome::objects::Chromatogram& chrom,
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	// return value
	bool ret = false;

	// range action
	if( canvas != NULL && evt.rbutton()
			&& evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {
		// coordinate
		double x = double();
		double y = double();
		graphics.transformPositionToData( evt.x(), evt.y(), &x, &y );
		setCurrentRange( x, x );

		// chromatogram menu
		wxMenu* menu = getChromRangeMenu();
		canvas->PopupMenu( menu );

		ret = true;
	}

	return ret;
}

// set selected range
void SelectRangeManager::setSelectedRange(
	std::pair< double, double >& range,
	int startPos,
	int nowPos,
	kome::img::Graphics& graphics
) {
	// get range
	double x0 = double();
	double x1 = double();
	double x2 = double();
	double x3 = double();
	double y = double();

	int minPx = MIN( startPos, nowPos );
	int maxPx = MAX( startPos, nowPos );

	graphics.transformPositionToData( minPx, 0, &x0, &y );
	if( startPos == nowPos ) {
		range.first = x0;
		range.second = x0;
		return;
	}

	graphics.transformPositionToData( minPx - 1, 0, &x1, &y );
	graphics.transformPositionToData( maxPx, 0, &x2, &y );
	graphics.transformPositionToData( maxPx + 1, 0, &x3, &y );

	double startX = ( x0 + x1 ) / 2.0;
	startX = MAX( startX, graphics.getMinX() );

	double endX = ( x2 + x3 ) / 2.0;
	endX = MIN( endX, graphics.getMaxX() );

	// set to pair
	range.first = startX;
	range.second = endX;
}

// draw foreground (spectrum)
void SelectRangeManager::drawForeground( 
	kome::objects::Spectrum& spec, 
	kome::img::Graphics& graphics, 
	wxWindow* canvas 
){
	// マウス移動時のラインセット
	setMouseMoveLine( graphics, canvas, &spec, NULL );

	// check the map
	if( m_specRangeMap.find( &spec ) == m_specRangeMap.end() ) {
		return;
	}

	// draw foreground
	drawForeground( m_specRangeMap[ &spec ], graphics );
}

// draw foreground (chromatogram)
void SelectRangeManager::drawForeground( 
	kome::objects::Chromatogram& chrom, 
	kome::img::Graphics& graphics, 
	wxWindow* canvas
){
	// マウス移動時のラインセット
	setMouseMoveLine( graphics, canvas, NULL, &chrom );

	// check the map
	if( m_chromRangeMap.find( &chrom ) == m_chromRangeMap.end() ) {
		return;
	}

	// draw foreground
	drawForeground( m_chromRangeMap[ &chrom ], graphics );
}

// draw background (spectrum)
void SelectRangeManager::drawBackground( 
	kome::objects::Spectrum& spec, 
	kome::img::Graphics& graphics
	 ) 
{			
	// check the map
	if( m_specRangeMap.find( &spec ) == m_specRangeMap.end() ) {
		return;
	}

	// draw background
	drawBackground( m_specRangeMap[ &spec ], graphics );
}

// draw background (chromatogram)
void SelectRangeManager::drawBackground( 
	kome::objects::Chromatogram& chrom, 
	kome::img::Graphics& graphics 
	 )
{	
	// check the map
	if( m_chromRangeMap.find( &chrom ) == m_chromRangeMap.end() ) {
		return;
	}

	// draw background
	drawBackground( m_chromRangeMap[ &chrom ], graphics );
}

// draws foreground
void SelectRangeManager::drawForeground( std::pair< double, double >& range, kome::img::Graphics& graphics )
{
	// points array
	int points[] = { 0, 0, 0, 0 };

	// get position
	double px = double();
	double py = double();

	graphics.transformDataToPosition( range.first, 0.0, &px, &py );	
	int startPos = roundnum( px );

	graphics.transformDataToPosition( range.second, 0.0, &px, &py );
	int endPos = roundnum( px );

	if( startPos >= graphics.getGraphPositionRight()
			|| endPos <= graphics.getGraphPositionLeft() ) {
		return;
	}

	// set
	COLOR oldColor = graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = graphics.getDrawStyle();

	graphics.setColor( kome::img::ColorTool::BLACK );
	graphics.setDrawStyle( kome::img::Graphics::STYLE_DOT );

	// draw stat position
	if( startPos > graphics.getGraphPositionLeft()
			&& startPos < graphics.getGraphPositionRight() ) {
		points[0] = startPos;
		points[1] = graphics.getGraphPositionTop();
		points[2] = startPos;
		points[3] = graphics.getGraphPositionBottom();

		graphics.drawLines( points, 2 );
	}

	// draw end position
	if( endPos > graphics.getGraphPositionLeft()
			&& endPos < graphics.getGraphPositionRight() ) {
		points[0] = endPos;
		points[1] = graphics.getGraphPositionTop();
		points[2] = endPos;
		points[3] = graphics.getGraphPositionBottom();

		graphics.drawLines( points, 2 );
	}

	// recover
	graphics.setColor( oldColor );
	graphics.setDrawStyle( oldStyle );
}

// draws background
void SelectRangeManager::drawBackground( std::pair< double, double >& range, kome::img::Graphics& graphics )
{
	// get position
	double px = double();
	double py = double();

	graphics.transformDataToPosition( range.first, 0.0, &px, &py );	
	int startPos = roundnum( px );

	graphics.transformDataToPosition( range.second, 0.0, &px, &py );
	int endPos = roundnum( px );

	if( startPos >= graphics.getGraphPositionRight()
			|| endPos <= graphics.getGraphPositionLeft() ) {
		return;
	}

	int left = MAX( startPos, graphics.getGraphPositionLeft() );
	int right = MIN( endPos, graphics.getGraphPositionRight() );
	if( left == right ) {
		return;
	}

	// set
	COLOR oldColor = graphics.getColor();
	graphics.setColor( kome::img::ColorTool::SILVER );
	graphics.setFillPattern( kome::img::Graphics::PATTERN_SOLID );

	// fill rect
	graphics.fillRect(
		left,
		graphics.getGraphPositionTop(),
		right - left,
		graphics.getGraphHeight()
	);

	// recover color
	graphics.setColor( oldColor );
}

// ステータスバーに表示する値をセット
bool SelectRangeManager::setValStatusBar(
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas,
	kome::objects::Spectrum* spec,
	kome::objects::Chromatogram* chrom
){	
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp( STATUS_BAR_COORDINATE, 1 );		// ステータスバーを有効化
	statusBar.setFlgDisp( STATUS_BAR_RANGE, 1 );			// ステータスバーを有効化
	statusBar.setStatusMsg( STATUS_BAR_RANGE, "" );

	std::string strDisp="";
	
	int px = evt.x();
	int py = evt.y();

	// マウスカーソルのアイコンを設定
	setMouseCursor( graphics, evt, SELECT_RANGE, ICON_NAME_MOVE_X );
	
	// グラフ表示内
	if( px >= graphics.getGraphPositionLeft()
			&& px <= graphics.getGraphPositionRight()
			&& py >= graphics.getGraphPositionTop()
			&& py <= graphics.getGraphPositionBottom()
	) {
		m_px = px;
		m_py = py;
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
		kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
		if( !evt.lbutton() && !evt.rbutton() && acCanvas == canvas ){			
			// @date 2011.09.07 <Add> M.Izumi ->
			// SPEC 82338 :Cross line is always displayed in the first object.  
			if( spec != NULL ){
				m_selSpec = spec;				
			}
			if( chrom != NULL ){
				m_selChrom = chrom;
			}
			// <-

			int cx;
			int cy;
			
			cx = CLAMP( px, graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
			cy = CLAMP( py, graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );

			double x1 = double();
			double y1 = double();
	
			graphics.transformPositionToData( (double)cx, (double)cy, &x1, &y1 );
			std::string strRangeFirst;
			std::string strRangeSecond;
			std::string strDisp;
			
			strRangeFirst = (boost::format("%.2f") % x1).str();
			strRangeSecond = (boost::format("%.2f") % y1).str();
			strDisp =  strRangeFirst + ", " + strRangeSecond ;	

			// ステータスバーに表示
			statusBar.setStatusMsg( STATUS_BAR_COORDINATE, strDisp.c_str() );	
			
			// フォーカスがあるキャンバス
			iFocusID = canvas;
			
			// [peak edit]が”ON”の時, trueを返す
			return !isPeakLabelEditMode();	
		}
	}else{
		m_px = -1;
		m_py = -1;
	}

	return false;
}

// マウスカーソルアイコンを設定する
void SelectRangeManager::setMouseCursor(
	kome::img::Graphics& graphics, 
	kome::evt::MouseEvent& evt, 
	const char* pluginName , 
	const char* iconName
){
	// get active canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
	
	int px = evt.x();
	int py = evt.y();
	
	// グラフ領域内
	if( px >= graphics.getGraphPositionLeft()
			&& px <= graphics.getGraphPositionRight()
			&& py >= graphics.getGraphPositionTop()
			&& py <= graphics.getGraphPositionBottom()
	) {
		
		//マウスカーソル変更
		kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();
		kome::plugin::PluginInfo* info;
		kome::plugin::PluginIcon* icon;

		info = pluginMgr.getPlugin( pluginName );
		icon = info->getIcon( iconName );
		
		wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
		wxImage img( bitmap.ConvertToImage() );
		int width = img.GetWidth();
		int height = img.GetHeight();

		if( strcmp( iconName, ICON_NAME_MOVE_X ) == 0 ){
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, width/2);
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, height/2);
		}else{
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 5);
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 5);
		}
		
		// This check is necessary to prevent a crash when no canvas is active.
		if (acCanvas != NULL)
		{
			acCanvas->SetCursor(wxCursor(img));			
		}
			
	}else{

		if (acCanvas != NULL && ( !evt.lbutton() && !evt.rbutton()) )
		{
		// This check is necessary to prevent a crash when no canvas is active.
			acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
		}
	}
}

// マウス移動時のラインを設定する
void SelectRangeManager::setMouseMoveLine( 
	kome::img::Graphics& graphics, 
	wxWindow* canvas,
	kome::objects::Spectrum* spec,
	kome::objects::Chromatogram* chrom
){
	// points array
	int points[] = { 0, 0, 0, 0 };
	
	COLOR oldColor = graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = graphics.getDrawStyle();
	
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
	
	if( !m_btnDown && canvas == acCanvas && iFocusID == acCanvas ){
		
		// @date 2011.09.07 <Add> M.Izumi ->
		// SPEC 82338 :Cross line is always displayed in the first object. 
		bool bflg = false;
		if( (spec != NULL  && spec == m_selSpec) ||
			(chrom != NULL && chrom == m_selChrom) ){
			bflg = true;
			m_selSpec = NULL;
			m_selChrom = NULL;
		}
			
		if( !bflg ){ 
			return;
		}
		// @date 2011.09.07 <Add> M.Izumi <-

		graphics.setColor( kome::img::ColorTool::BLACK );
		graphics.setDrawStyle( kome::img::Graphics::STYLE_SOLID );
			
		// draw stat position	
		points[0] = m_px;
		points[1] = graphics.getGraphPositionTop();
		points[2] = m_px;
		points[3] = graphics.getGraphPositionBottom();

		graphics.drawLines( points, 2 );
	
		// draw end position	
		points[0] = graphics.getGraphPositionLeft();
		points[1] = m_py;
		points[2] = graphics.getGraphPositionRight();
		points[3] = m_py;

		graphics.drawLines( points, 2 );
	
		// recover
		graphics.setColor( oldColor );
		graphics.setDrawStyle( oldStyle );
	}		
	
	iFocusID = NULL;
	
}

// is Peak Label Edit mode						// @date 2012.01.11 <Add> M.Izumi
bool SelectRangeManager::isPeakLabelEditMode(){
	bool bflg = false;

	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* cStr = statusMgr.getValue( "LABEL_EDIT_MODE" );
	if( cStr != NULL && strcmp( cStr, "edit" ) == 0 ) {
		bflg = true;
	}
	return bflg;
}

// load actions
void SelectRangeManager::loadActions() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// clear
	m_specRangeActions.clear();
	m_chromRangeActions.clear();

	// get actions
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );
		for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
			kome::plugin::PluginCall* call = plugin->getCall( j );

			if( compareignorecase( call->getType(), SPECTRUM_RANGE_ACTION_STATUS_NAME ) == 0 ) {
				m_specRangeActions.push_back( call );
			}
			else if( compareignorecase( call->getType(), CHROM_RANGE_ACTION_STATUS_NAME ) == 0 ) {
				m_chromRangeActions.push_back( call );
			}
		}
	}

	// set function
	unsigned int num = std::max( m_specRangeActions.size(), m_chromRangeActions.size() );
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();
	for( unsigned int i = 0; i < num; i++ ) {
		mainFrame->setExecuteFunction( BASE_MENU_ID + i,  SelectRangeManager::actionSpecOrChromRange );
	}
}

// set current range
void SelectRangeManager::setCurrentRange( const double start, const double end ) {
	m_start = start;
	m_end = end;
}

// create spectrum range menu
void SelectRangeManager::createSpecRangeMenu( wxMenu& menu ) {
	// load
	if( m_specRangeActions.size() == 0 && m_chromRangeActions.size() == 0 ) {
		loadActions();
	}

	// each actions
	for( unsigned int i = 0; i < m_specRangeActions.size(); i++ ) {
		kome::plugin::PluginCall* call = m_specRangeActions[ i ];
		std::string name = call->getProperties().getStringValue( "name", "(null)" );
		menu.Append( BASE_MENU_ID + i, wxT( name.c_str() ), wxT( call->getDescription() ) );
	}
}

// create chromatogram range menu
void SelectRangeManager::createChromRangeMenu( wxMenu& menu ) {
	// load
	if( m_specRangeActions.size() == 0 && m_chromRangeActions.size() == 0 ) {
		loadActions();
	}

	// each actions
	for( unsigned int i = 0; i < m_chromRangeActions.size(); i++ ) {
		kome::plugin::PluginCall* call = m_chromRangeActions[ i ];
		std::string name = call->getProperties().getStringValue( "name", "(null)" );
		menu.Append( BASE_MENU_ID + i, wxT( name.c_str() ), wxT( call->getDescription() ) );
	}
}

// get spectrum menu
wxMenu* SelectRangeManager::getSpecRangeMenu() {
	if( m_specMenu == NULL ) {
		m_specMenu = new wxMenu();
		createSpecRangeMenu( *m_specMenu );
	}

	return m_specMenu;
}

// get chromatogram menu
wxMenu* SelectRangeManager::getChromRangeMenu() {
	if( m_chromMenu == NULL ) {
		m_chromMenu = new wxMenu();
		createChromRangeMenu( *m_chromMenu );
	}

	return m_chromMenu;
}

// range action
bool SelectRangeManager::actionSpecOrChromRange( kome::objects::Variant obj, int id ) {
	// index
	int idx = id - BASE_MENU_ID;
	if( idx < 0 ) {
		LOG_WARN( FMT( "Invalid ID." ) );
		return false;
	}

	// manager
	SelectRangeManager& mgr = getInstance();

	// set range
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setDouble( params, "start", mgr.m_start );
	kome::plugin::PluginCallTool::setDouble( params, "end", mgr.m_end );

	// active object
	kome::objects::MsDataVariant var( obj );
	kome::objects::Spectrum* spec = var.getSpectrum();
	kome::objects::Chromatogram* chrom = var.getChromatogram();
	
	if( spec != NULL ) {
		kome::plugin::PluginCallTool::setSpectrum( params, *spec );
		mgr.m_specRangeActions.at( idx )->invoke( &params );
	}
	else if( chrom != NULL ) {
		kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
		mgr.m_chromRangeActions.at( idx )->invoke( &params );
	}
	else {
		LOG_WARN( FMT( "Failed to get the object." ) );
		return false;
	}

	return true;
}

// get object
SelectRangeManager& SelectRangeManager::getInstance() {
	// create object (This is the only object)
	static SelectRangeManager mgr;

	return mgr;
}
