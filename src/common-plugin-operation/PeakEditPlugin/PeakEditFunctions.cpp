/**
 * @file PeakEditFunctions.cpp
 * @brief implements of PeakEdit plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.12.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakEditFunctions.h"
#include "PeakEditManager.h"


using namespace kome::peaks::edit;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define LABEL_STATUS_NAME		"LABEL_EDIT_MODE"
#define ADD_STATUS_VALUE		"add"
#define DEL_STATUS_VALUE		"del"
#define EDIT_STATUS_VALUE		"edit"

static int flgStartEdirSpec = 0;
static int flgStartEdirChrom= 0;

// initialize label mode
kome::objects::Variant initLabelMode( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( LABEL_STATUS_NAME, "" );

	return ret;
}

// finalize label mode
kome::objects::Variant finalizeLabelMode( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( LABEL_STATUS_NAME, "" );

	return ret;
}

// draw spectrum peak position
kome::objects::Variant drawSpecPeakPos( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::core::XYData* xyData = spec->getXYData();
	if( xyData == NULL ) {
		return ret;
	}

	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ) {
		return ret;
	}

	// draw peak position
	PeakEditManager& mgr = PeakEditManager::getInstance();
	{	// @date 2011/07/11 <Add> OKADA
		kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
		kome::objects::Peaks* peaksTemp = pkMgr.getPeaks( spec );
		mgr.drawPeakPosition( *g, *peaksTemp, *xyData );
	}

	return ret;
}

// draw peak position
kome::objects::Variant drawChromPeakPos( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::core::XYData* xyData = chrom->getXYData();
	if( xyData == NULL ) {
		return ret;
	}

	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ) {
		return ret;
	}

	// draw peak position
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.drawPeakPosition( *g, *peaks, *xyData );

	return ret;
}

// draw spectrum peak shape
kome::objects::Variant drawSpecPeakShape( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::core::XYData* xyData = spec->getXYData();
	if( xyData == NULL ) {
		return ret;
	}

	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ) {
		return ret;
	}

	// draw peak shape
	PeakEditManager& mgr = PeakEditManager::getInstance();

	int m_flgSelectLocal = mgr.m_flgSelect;	// @date 2011/08/02 <Add> OKADA

	try{
		mgr.drawPeakShape( *g, *peaks, *xyData );				// @date <Temp> OKADA
	}
	catch( std::string str )  // 例外をキャッチする
	{
		std::string strTemp = str;	// ここでbreak point設置
	}

	// @date 2011/08/03 <Add> OKADA
	// メモリ描画判定処理
	if( mgr.m_flgRedrawMemory == true ){
		mgr.m_flgRedrawMemory = false;	// @date 2011/08/03 <Add> OKADA
	}else if(( m_flgSelectLocal == 0 )&&( mgr.m_flgSelect != 0 )){// drawPeakShape()関数呼び出し前は０で、呼び出し後に変化した場合
		;	
	}else{
		mgr.m_flgSelect = 0;
		return ret;
	}

	// @date 2011/06/14 <Add> OKADA ------->
	wxBitmap bitmap((*g).getWidth(), (*g).getHeight() );	// @date 2011/06/27 <Add> OKADA

	mgr.m_memDC.SelectObject( bitmap );
	mgr.m_memDC.SetBackground( *wxWHITE_BRUSH );
	mgr.m_memDC.Clear();

	mgr.m_graphicsSpec.setDC( &mgr.m_memDC );

	// Set Margin
	mgr.m_graphicsSpec.setGraphMargin(	(*g).getGraphMarginLeft(),
									(*g).getGraphMarginTop(),
									(*g).getGraphMarginRight(),
									(*g).getGraphMarginBottom() );

	mgr.m_graphicsSpec.setMaxX( (*g).getMaxX() ); 
	mgr.m_graphicsSpec.setMaxY( (*g).getMaxY() ); 
	mgr.m_graphicsSpec.setMinX( (*g).getMinX() ); 
	mgr.m_graphicsSpec.setMinY( (*g).getMinY() ); 

	mgr.m_graphicsSpec.setSize( (*g).getWidth(), (*g).getHeight() );

	mgr.m_graphicsSpec.setGraphPosition(	(*g).getGraphPositionLeft(), 
										(*g).getGraphPositionTop(), 
										(*g).getGraphPositionRight(), 
										(*g).getGraphPositionBottom() );
	
	try{
		// wxMemoryDC に対して描画
		mgr.m_flgSelect = 0;
		mgr.m_flgSelectSpec = 0;
				
		mgr.drawPeakShape( mgr.m_graphicsSpec, *peaks, *xyData );

		mgr.m_flgSelectSpec = mgr.m_flgSelect;
	}
	catch( std::string str )  // 例外をキャッチする
	{
		std::string strTemp = str;
	}

	mgr.wxiImageSpec = bitmap.ConvertToImage();

	return ret;
}

// draw chromatogram peak shape
kome::objects::Variant drawChromPeakShape( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;


	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::core::XYData* xyData = chrom->getXYData();
	if( xyData == NULL ) {
		return ret;
	}

	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ) {
		return ret;
	}

	// draw peak shape
	PeakEditManager& mgr = PeakEditManager::getInstance();
	int m_flgSelectLocal = mgr.m_flgSelect;	// @date 2011/08/02 <Add> OKADA
	mgr.drawPeakShape( *g, *peaks, *xyData );

	// @date 2011/08/03 <Add> OKADA
	// メモリ描画判定処理
	if( mgr.m_flgRedrawMemory == true ){
		mgr.m_flgRedrawMemory = false;	// @date 2011/08/03 <Add> OKADA
	}else if(( m_flgSelectLocal == 0 )&&( mgr.m_flgSelect != 0 )){// drawPeakShape()関数呼び出し前は０で、呼び出し後に変化した場合
		;	
	}else{
		mgr.m_flgSelect = 0;
		return ret;
	}

	// @date 2011/06/23 <Add> OKADA ------->
	wxBitmap bitmap((*g).getWidth(), (*g).getHeight() );	// @date 2011/06/27 <Add> OKADA

	mgr.m_memDC.SelectObject( bitmap );
	mgr.m_memDC.SetBackground( *wxWHITE_BRUSH );
	mgr.m_memDC.Clear();

	mgr.m_graphicsChrom.setDC( &mgr.m_memDC );

	// Set Margin
	mgr.m_graphicsChrom.setGraphMargin(	(*g).getGraphMarginLeft(),
									(*g).getGraphMarginTop(),
									(*g).getGraphMarginRight(),
									(*g).getGraphMarginBottom() );

	mgr.m_graphicsChrom.setMaxX( (*g).getMaxX() ); 
	mgr.m_graphicsChrom.setMaxY( (*g).getMaxY() ); 
	mgr.m_graphicsChrom.setMinX( (*g).getMinX() ); 
	mgr.m_graphicsChrom.setMinY( (*g).getMinY() ); 

	mgr.m_graphicsChrom.setSize( (*g).getWidth(), (*g).getHeight() );

	mgr.m_graphicsChrom.setGraphPosition(	(*g).getGraphPositionLeft(), 
										(*g).getGraphPositionTop(), 
										(*g).getGraphPositionRight(), 
										(*g).getGraphPositionBottom() );
	
	try{
		// wxMemoryDC に対して描画
		mgr.m_flgSelect = 0;
		mgr.m_flgSelectChrom = 0;

		mgr.drawPeakShape( mgr.m_graphicsChrom, *peaks, *xyData );

		mgr.m_flgSelectChrom = mgr.m_flgSelect;
	}
	catch( std::string str )  // 例外をキャッチする
	{
		std::string strTemp = str;
	}

	mgr.wxiImageChrom = bitmap.ConvertToImage();
	// @date 2011/06/23 <Add> OKADA <-------

	return ret;
}

// set label add mode
kome::objects::Variant setLabelAddMode( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// get status value
	const char* val = statusMgr.getValue( LABEL_STATUS_NAME );
	std::string valStr = NVL( val, "" );

	// set value
	if( valStr.compare( ADD_STATUS_VALUE ) == 0 ) {
		statusMgr.setValue( LABEL_STATUS_NAME, "" );
		mgr.setMode( PeakEditManager::MODE_NONE );
	}
	else {
		statusMgr.setValue( LABEL_STATUS_NAME, ADD_STATUS_VALUE );
		mgr.setMode( PeakEditManager::MODE_ADD );
	}

	mgr.unselectPeak();

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// set label edit mode
kome::objects::Variant setLabelEditMode( kome::objects::Parameters* params) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// get status value
	const char* val = statusMgr.getValue( LABEL_STATUS_NAME );
	std::string valStr = NVL( val, "" );

	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	
	// set value
	if( valStr.compare( EDIT_STATUS_VALUE ) == 0 ) {
		// Peak Edit モードから出るとき
		statusMgr.setValue( LABEL_STATUS_NAME, "" );
		mgr.setMode( PeakEditManager::MODE_NONE );

		// マウスカーソルを通常の矢印に戻す	// @date 2011/08/24 <Add> OKADA ------->
		unsigned int uNumberOfCanvas = kome::window::Canvas::getNumberOfCanvases();
		for( unsigned int idx=0; idx<uNumberOfCanvas; idx++ ){
			kome::window::Canvas* p_canvas = kome::window::Canvas::getCanvas( idx );
			if( p_canvas != NULL ){
				p_canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
			}
		}	// @date 2011/08/24 <Add> OKADA <-------
		
		statusBar.setStatusMsg("EditPeaks", "" );
	}
	else {
		// Peak Edit モードに入ったとき
		statusMgr.setValue( LABEL_STATUS_NAME, EDIT_STATUS_VALUE );
		mgr.setMode( PeakEditManager::MODE_EDIT );
		
		statusBar.setFlgDisp( "EditPeaks", 1 );
		statusBar.setStatusMsg("EditPeaks", "Edit Peaks: On" ); // @date2011.09.14 <Add> M.Izumi
	}

	mgr.unselectPeak();

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// deletes selected peak
kome::objects::Variant deleteSelectedPeak( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	PeakEditManager& mgr = PeakEditManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// delete
	mgr.deletePeak();

	return ret;
}

// deleting peak is available or not
kome::objects::Variant canDeletePeak( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeakEditManager& mgr = PeakEditManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// judge
	if( mgr.getSelectedPeak() != NULL ) {
		kome::objects::Spectrum* spec = obj.getSpectrum();
		if( spec != NULL ) {
			ret.prim.boolVal = ( spec == mgr.getSelectedSpectrum() );
		}

		kome::objects::Chromatogram* chrom = obj.getChromatogram();
		if( chrom != NULL ) {
			ret.prim.boolVal = ( chrom == mgr.getSelectedChromatogram() );
		}
	}

	return ret;
}

// start to add spectrum label
kome::objects::Variant startToAddSpecLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.startToAdd( *spec, *g, *evt );

	return ret;
}

// start to edit spectrum label
kome::objects::Variant startToEditSpecLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/06/15 <Add> OKADA ------->
	int cx = evt->x();
	int cy = evt->y();
	COLOR col  = mgr.m_graphicsSpec.getPixel( cx, cy );
	// @date 2011/06/15 <Add> OKADA <-------

	bool bTmpLbuttonStatus  = (*evt).lbutton();
	(*evt).lbutton( true );

	// start to editn
	ret.prim.boolVal = mgr.startToEditPeak( *spec, *g, *evt );

	(*evt).lbutton( bTmpLbuttonStatus );

	// @date 2011/06/21 <Add> OKADA ------->
	cx = evt->x();
	cy = evt->y();
	col  = mgr.m_graphicsSpec.getPixel( cx, cy );	// wxRED_BRUSH:255 wxBLUE_BRUSH = 0x00ff0000 wxGREEN_BRUSH = 0x0000ff00
	if( ( col == 0x0000ff00 )||( col == 0x00ffffff ) )
	{
		int jjj = 0;
		jjj = 1;
	}else{
		int lll = 0;
		lll = 123;
	}
	// @date 2011/06/21 <Add> OKADA <-------

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

	// マウスカーソルを場所にあった物に変更
	kome::objects::Variant retCanDeletePeak =  canDeletePeak( params );

	if( mgr.isInDeleteArea( *g, *evt, CALL_FROM_SPECTRUM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_DEL, canvas ); 
	}else if( mgr.isInEditArea( *g, *evt, CALL_FROM_SPECTRUM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas ); 
	}else if( mgr.isInAddArea( *g, *evt, CALL_FROM_SPECTRUM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_ADD, canvas ); 
	}else{
		mgr.setMouseCursor( *g, *evt, NULL, NULL, canvas ); 	// 通常の矢印
	}

	if( mgr.isInControlArea( *g, *evt ) ){
		// グラフ制御領域内
		ret.prim.boolVal = true;	// この後、mouse click 処理が呼ばれないようにする
	}else{
		// グラフ制御領域外
	}

	return ret;
}

// move cursor in adding spectrum label
kome::objects::Variant moveCursorInAddingSpecLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.moveAddingCursor( *spec, *g, *evt );

	return ret;
}

// move cursor in editing spectrum label
kome::objects::Variant moveCursorInEditingSpecLabel( kome::objects::Parameters* params ) {	// call type="on_spec_mouse_move" status="1" ifloaded="true" order ="1"
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// Add 状態かを判定
	if( mgr.getInAddFlag() == 1 ) 
	{
		ret = moveCursorInAddingSpecLabel( params );

		if( ret.prim.boolVal ){
			wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

			// アイコンを鉛筆印に変更
			mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_ADD, canvas );
		}

		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.moveEditingCursor( *spec, *g, *evt );

	if( ret.prim.boolVal ){
		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

		// アイコンを編集に変更
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas );
	}

	// @date 2011/10/14 <Add> OKADA ------->
	// DELETEで選択処理中（マウスの左ボタン押しっぱなし）の際の処理
	if( mgr.m_peakOnClick ){
		// マウスカーソルをDEL（消しゴム）に設定
		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_DEL, canvas );

		ret.prim.boolVal = true;	// DELETEで選択処理中は次の処理を行わない
	}
	// @date 2011/10/14 <Add> OKADA <-------

	return ret;
}

// finish to add spectrum label
kome::objects::Variant finishToAddSpecLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// finish to add
	ret.prim.boolVal = mgr.finishToAddPeak( *spec, *g, *evt );

	// @date 2012/04/24 <Add> OKADA ------->
	// update	// SPEC 84518
	if( spec != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	}

	// 幅0のピークの手動作成を抑制する	// SPEC 84519
	kome::objects::PeakElement* peak = mgr.getSelectedPeak();
	if( peak != NULL ){
		double dR = peak->getRightX();
		double dL = peak->getLeftX();
		double dWidth = dR - dL;
		int iii = 0;
		if( abs(dWidth) < 0.000001 ){
			// delete
			mgr.deletePeak();
		}
	}

	// @date 2012/04/24 <Add> OKADA <-------

	return ret;
}

// finish to edit spectrum label
kome::objects::Variant finishToEditSpecLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// finish to edit
	ret.prim.boolVal = mgr.finishToEditPeak( *spec, *g, *evt );

	return ret;
}

// starts to add chromatogram label
kome::objects::Variant startToAddChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.startToAdd( *chrom, *g, *evt );

	return ret;
}

// start to edit chromatogram label
kome::objects::Variant startToEditChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	bool bTmpLbuttonStatus  = (*evt).lbutton();
	(*evt).lbutton( true );

	// start to edit
	ret.prim.boolVal = mgr.startToEditPeak( *chrom, *g, *evt );

	(*evt).lbutton( bTmpLbuttonStatus );

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

// @date 2011/06/23 <Add> OKADA ------->
	// マウスカーソルを場所にあった物に変更
	kome::objects::Variant retCanDeletePeak =  canDeletePeak( params );

	if( mgr.isInDeleteArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_DEL, canvas );
	}else if( mgr.isInEditArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas );
	}else if( mgr.isInAddArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) ){
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_ADD, canvas );
	}else{
		mgr.setMouseCursor( *g, *evt, NULL, NULL, canvas );	// 通常の矢印
	}

	if( mgr.isInControlArea( *g, *evt ) ){
		// グラフ制御領域内
		ret.prim.boolVal = true;	// この後、mouse click 処理が呼ばれないようにする
	}else{
		// グラフ制御領域外
	}
// @date 2011/06/23 <Add> OKADA <-------

	return ret;
}

// move cursor in adding chromatogram label
kome::objects::Variant moveCursorInAddingChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// move cursor
	ret.prim.boolVal = mgr.moveAddingCursor( *chrom, *g, *evt );

	return ret;
}

// move cursor in editing chromatogram label
kome::objects::Variant moveCursorInEditingChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/06/23 <Add> OKADA ------->
	// Add 状態かを判定
	if( mgr.getInAddFlag() == 1 ) 
	{
		ret = moveCursorInAddingChromLabel( params );

		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA
		if( ret.prim.boolVal ){
			wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

			// アイコンを鉛筆印に変更
			mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_ADD, canvas );
		}
		return ret;
	}
	// @date 2011/06/23 <Add> OKADA <-------

	// move cursor
	ret.prim.boolVal = mgr.moveEditingCursor( *chrom, *g, *evt );

	// @date 2011/06/23 <Add> OKADA ------->
	if( ret.prim.boolVal ){
		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

		// アイコンを編集に変更
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas );
	}
	// @date 2011/06/23 <Add> OKADA <-------

	// @date 2011/10/14 <Add> OKADA ------->
	// DELETEで選択処理中（マウスの左ボタン押しっぱなし）の際の処理
	if( mgr.m_peakOnClick ){
		// マウスカーソルをDEL（消しゴム）に設定
		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
		mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_DEL, canvas );

		ret.prim.boolVal = true;	// DELETEで選択処理中は次の処理を行わない
	}
	// @date 2011/10/14 <Add> OKADA <-------

	return ret;
}

// finish to add chroamtogram label
kome::objects::Variant finishToAddChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.finishToAddPeak( *chrom, *g, *evt );

	return ret;
}

// finish to edit chroamtogram label
kome::objects::Variant finishToEditChromLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// start to add
	ret.prim.boolVal = mgr.finishToEditPeak( *chrom, *g, *evt );

	// @date 2012/04/24 <Add> OKADA ------->
	if( chrom != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );
	}

	// 幅0のピークの手動作成を抑制する	// SPEC 84519
	kome::objects::PeakElement* peak = mgr.getSelectedPeak();
	if( peak != NULL ){
		double dR = peak->getRightX();
		double dL = peak->getLeftX();
		double dWidth = dR - dL;
		int iii = 0;
		if( abs(dWidth) < 0.000001 ){
			// delete
			mgr.deletePeak();
		}
	}
	// @date 2012/04/24 <Add> OKADA <-------

	return ret;
}

// commit spectrum peaks
kome::objects::Variant commitSpectrumPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// commit
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.commitPeaks( *spec );

	return ret;
}

// commit chromatogram peaks
kome::objects::Variant commitChromatogramPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// commit
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.commitPeaks( *chrom );

	return ret;
}

// rollback spectrum peaks
kome::objects::Variant rollbackSpectrumPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// rollback
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.rollbackPeaks( *spec );

	return ret;

}

// rollback chromatogram peaks
kome::objects::Variant rollbackChromatogramPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// rollback
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.rollbackPeaks( *chrom );

	return ret;
}

// move cursor in editing spectrum label
kome::objects::Variant moveMouseCursorInEditingSpec( kome::objects::Parameters* params ) {	// call type="on_spec_mouse_move"
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	bool bVal = evt->lbutton();

	// start to add

	if( mgr.isInControlArea( *g, *evt ) ){
		// グラフ制御領域内
		if( bVal==true ){
			// マウスカーソル左クリックでドラッグ中
		}else{
			startToEditSpecLabel( params );
			finishToEditSpecLabel( params );
		}

		ret.prim.boolVal = true;	// この後、mouse move 処理が呼ばれないようにする

	}else{
		mgr.m_flgRedrawMemory=true;	// @date 2011/08/03 <Add> OKADA
		// グラフ制御領域外
		return ret;
	}
	
	ret.prim.boolVal = mgr.moveEditingCursor( *spec, *g, *evt );
	
	return ret;
}


kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/08/26 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editのクリック処理を行わない（アイコンはそのまま）
	wxWindow* p_wxwCanvas = kome::plugin::PluginCallTool::getCanvas( params );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >(p_wxwCanvas);
	if( canvas == NULL ){
		return ret;
	}else{
		int nCanvasStyle = canvas->getCanvasStyle();
		if( nCanvasStyle & kome::window::Canvas::OVERLAPPING ){
			return ret;
		}
	}
	// @date 2011/08/26 <Add> OKADA <-------

	if((evt->lbutton()) && ( mgr.isInDeleteArea( *g, *evt, CALL_FROM_SPECTRUM ) )){

		mgr.m_peakOnClick = mgr.getSelectedPeak();	// @date 2011/10/14 <Add> OKADA
		ret.prim.boolVal = true;					// @date 2011/10/14 <Add> OKADA
	}else if( mgr.isInAddArea( *g, *evt, CALL_FROM_SPECTRUM) ){
		mgr.setModeAdd();
		kome::objects::Variant ret =	startToAddSpecLabel( params );
		if( ret.prim.boolVal == true ){
			mgr.setInAddFlag( 1 );
		}else{
			mgr.setModeEdit();
		}
	}

	// @date 2011/06/27 <Add> OKADA 
	else if( mgr.isInEditArea( *g, *evt, 1 ) ){
		ret.prim.boolVal = mgr.moveEditingCursor( *spec, *g, *evt );
		if( ret.prim.boolVal ){
			flgStartEdirSpec=1;

			wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

			// アイコンを編集に変更
			mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas );
		}	}

	return ret;
}


kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/10/14 <Add> OKADA ------->
	if( mgr.isInDeleteArea( *g, *evt, CALL_FROM_SPECTRUM ) ){
		if( mgr.m_peakOnClick == mgr.getSelectedPeak() ){
			ret = deleteSelectedPeak( params );	// エリア削除処理
			mgr.m_flgRedrawMemory = true;
		}
	}
	mgr.m_peakOnClick = NULL;
	// @date 2011/10/14 <Add> OKADA <-------

	if( mgr.getInAddFlag() == 1 ){
		mgr.setModeEdit();
		ret = finishToAddSpecLabel( params );
		mgr.setInAddFlag( 2 );
	}

	if( flgStartEdirSpec == 1 ){
		flgStartEdirSpec = 0;

		// finish to edit
		ret.prim.boolVal = mgr.finishToEditPeak( *spec, *g, *evt );
	}


	return ret;
}

// move cursor in editing spectrum label
kome::objects::Variant moveMouseCursorInEditingChrom( kome::objects::Parameters* params ) {	// call type="on_spec_mouse_move"
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// @date 2011/08/29 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	if( chkOverlapping(params) ){
		return ret;
	}
	// @date 2011/08/29 <Add> OKADA <-------

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	bool bVal = evt->lbutton();

	// start to add
	if( mgr.isInControlArea( *g, *evt ) ){
		// グラフ制御領域内
		if( bVal==true ){
			// マウスカーソル左クリックでドラッグ中
		}else{
			startToEditChromLabel( params );
			finishToEditChromLabel( params ); 
		}

		ret.prim.boolVal = true;	// この後、mouse move 処理が呼ばれないようにする

	}else{
		mgr.m_flgRedrawMemory=true;	// @date 2011/08/03 <Add> OKADA
		// グラフ制御領域外
		return ret;
	}
	
	ret.prim.boolVal = mgr.moveEditingCursor( *chrom, *g, *evt );

	return ret;
}


kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/08/26 <Add> OKADA ------->
	// Overlapping Window の場合、Peak Editのクリック処理を行わない（アイコンはそのまま）
	wxWindow* p_wxwCanvas = kome::plugin::PluginCallTool::getCanvas( params );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >(p_wxwCanvas);
	if( canvas == NULL ){
		return ret;
	}else{
		int nCanvasStyle = canvas->getCanvasStyle();
		if( nCanvasStyle & kome::window::Canvas::OVERLAPPING ){
			return ret;
		}
	}
	// @date 2011/08/26 <Add> OKADA <-------

	if((evt->lbutton()) &&( mgr.isInDeleteArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) )){

		mgr.m_peakOnClick = mgr.getSelectedPeak();	// @date 2011/10/14 <Add> OKADA
		ret.prim.boolVal = true;					// @date 2011/10/14 <Add> OKADA
	}else if( mgr.isInAddArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) ){
		mgr.setModeAdd();
		kome::objects::Variant ret =	startToAddChromLabel( params );
		if( ret.prim.boolVal == true ){
			mgr.setInAddFlag( 1 );
		}else{
			mgr.setModeEdit();
		}
	}

	// @date 2011/06/23 <Add> OKADA 18:18
	else if( mgr.isInEditArea( *g, *evt, 1 ) ){
		ret.prim.boolVal = mgr.moveEditingCursor( *chrom, *g, *evt );
		if( ret.prim.boolVal ){
			flgStartEdirChrom=1;
			wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011/08/03 <Add> OKADA

			// アイコンを編集に変更
			mgr.setMouseCursor( *g, *evt, PLUGIN_NAME_PEAK_EDIT, ICON_NAME_PEAK_EDIT, canvas );
		}
	}

	return ret;
}


kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	kome::evt::MouseEvent* evt = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// @date 2011/10/14 <Add> OKADA ------->
	if( mgr.isInDeleteArea( *g, *evt, CALL_FROM_CHROMATOGRAM ) ){
		if( mgr.m_peakOnClick == mgr.getSelectedPeak() ){
			ret = deleteSelectedPeak( params );	// エリア削除処理
			mgr.m_flgRedrawMemory = true;
		}
	}
	mgr.m_peakOnClick = NULL;
	// @date 2011/10/14 <Add> OKADA <-------

	if( mgr.getInAddFlag() == 1 ){
		mgr.setModeEdit();
		ret = finishToAddChromLabel( params );
		mgr.setInAddFlag( 2 );
	}

	if( flgStartEdirChrom == 1 ){
		flgStartEdirChrom = 0;

		// finish to edit
		ret.prim.boolVal = mgr.finishToEditPeak( *chrom, *g, *evt );

	}

	return ret;
}

// checks whether can exit the Quantitation dialog
kome::objects::Variant canExitPeakEditMode( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	PeakEditManager& mgr = PeakEditManager::getInstance();
	mgr.onExitSample();

	ret.prim.boolVal = true;

	return ret;
}

// set peak draw mode
kome::objects::Variant setPeakDrawMode( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( LABEL_STATUS_NAME, EDIT_STATUS_VALUE );

	return ret;
}

bool chkOverlapping( kome::objects::Parameters* params )
{
	bool ret = true;

	// Overlapping Window の場合、Peak Editの処理を行わない（マウスカーソルは矢印に戻す）
	wxWindow* p_wxwCanvas = kome::plugin::PluginCallTool::getCanvas( params );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >(p_wxwCanvas);
	if( canvas == NULL ){
		return ret;
	}else{
		int nCanvasStyle = canvas->getCanvasStyle();
		if( nCanvasStyle & kome::window::Canvas::OVERLAPPING ){
			// マウスカーソルを通常の矢印に戻す
			unsigned int uNumberOfCanvas = kome::window::Canvas::getNumberOfCanvases();
			for( unsigned int idx=0; idx<uNumberOfCanvas; idx++ ){
				kome::window::Canvas* p_canvas = kome::window::Canvas::getCanvas( idx );
				if( p_canvas != NULL ){
					p_canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
				}
			}
			return ret;
		}else{
			;	// マウスカーソルはは矢印に戻さない
		}
	}

	return false;
}
