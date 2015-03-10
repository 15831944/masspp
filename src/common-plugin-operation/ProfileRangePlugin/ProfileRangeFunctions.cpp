/**
 * @file ProfileRangeFunctions.cpp
 * @brief implements of ProfileRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * <補足説明>
 * ・Version 2.0.0での変更点　2011.05.19 <Mod> M.Izumi
   View > Operation > Profile Range をメニューバーから削除
   statusに関係なく範囲選択処理を行なうように変更

   ステータスバーの設定・表示の呼び出し関数追加

    ※ 変更箇所が多いため変更箇所は削除しています。
   　 必要そうな箇所にはコメントを残しています。
 */


#include "stdafx.h"
#include "ProfileRangeFunctions.h"
#include "ProfileRangeManager.h"
#include "ProfileRangeOperation.h"

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define  STATUS_SYNCHRONIZE "set_datamap_status"


// prepare to draw spectrum
kome::objects::Variant prepareSpectrum( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}
	// prepare
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.prepare( *spectrum, *graphics, canvas );

	return ret;
}

// prepare to draw chromatogram
kome::objects::Variant prepareChromatogram( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}
	// prepare
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.prepare( *chromatogram, *graphics, canvas );

	return ret;
}

// on close spectrum
kome::objects::Variant onCloseSpec( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ) {
		return ret;
	}

	// @date 2011.11.04 <Mod> M.Izumi
	// オーバーラッピング設定パネルでRemoveボタンをクリックすると、範囲が初期表示に戻る不具合の修正
	kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( canvas );
	if( c != NULL && c->getCanvasStyle() == 0 ){
		// close
		ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
		mgr.close( canvas );
	}// Stacked Windowの場合にclose処理が走らない問題の修正	// @date 2012/12/11 <Add> OKADA
	else if( c != NULL && c->getCanvasStyle() == 1 ){
		// close
		ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
		mgr.close( canvas );
	}

	return ret;
}

// on close chromatogram
kome::objects::Variant onCloseChrom( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}
	// @date 2011.11.04 <Mod> M.Izumi
	// オーバーラッピング設定パネルでRemoveボタンをクリックすると、範囲が初期表示に戻る不具合の修正
	kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( canvas );
	if( c!= NULL && c->getCanvasStyle() == 0 ){
		// close
		ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
		mgr.close( canvas );
	}	// Stacked Windowの場合にclose処理が走らない問題の修正	// @date 2012/12/11 <Add> OKADA
	else if( c != NULL && c->getCanvasStyle() == 1 ){
		// close
		ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
		mgr.close( canvas );
	}

	return ret;
}

// on button down (spectrum)
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.downButton( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on button down (chromatogram)
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.downButton( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// on mouse move (spectrum)
kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on mouse move
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.moveMouse( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on mouse move (chromatogram)
kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on mouse move
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.moveMouse( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// on button up (spectrum)
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.upButton( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on button up (chromatogram)
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.upButton( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// on double click (spectrum)
kome::objects::Variant onSpecDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on double click
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.dblClick( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on double click (chromatogram)
kome::objects::Variant onChromDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on double click
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.dblClick( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// on button down (spectrum)
kome::objects::Variant onSpecKeyDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::KeyEvent* keyEvent = kome::plugin::PluginCallTool::getKeyEvent( params );
	if( keyEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.keyDown( *keyEvent, *graphics, canvas );

	return ret;
}

// on button down (chromatogram)
kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::KeyEvent* keyEvent = kome::plugin::PluginCallTool::getKeyEvent( params );
	if( keyEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button down
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.keyDown( *keyEvent, *graphics, canvas );

	return ret;
}

// draw spectrum foreground
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// draw
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.drawForeground( *spectrum, *graphics, canvas );

	return ret;
}

// draw chromatogram foreground
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// draw
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.drawForeground( *chromatogram, *graphics, canvas );

	return ret;
}

// draw spectrum background
kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// draw
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.drawBackground( *spectrum, *graphics, canvas );

	return ret;
}

// draw chromatogram background
kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// draw
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.drawBackground( *chromatogram, *graphics, canvas );

	return ret;
}

// set range status
kome::objects::Variant setRangeStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
			
	// toggles status
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.toggleRangeStatus();
	
	// @date 2011.08.22 <Mod> M.Izumi ->
	// Spec No.82145 : Cannot use Synchronize Axes function against heatmaps 
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::objects::Parameters params;
	plgMgr.callAllFunctions( &params, STATUS_SYNCHRONIZE );
	// <-
	
	return ret;
}

// reset range status
kome::objects::Variant resetZoomFromContextMenu( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );

	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	wxWindow* canvas = mgr.getCanvas( var );
	kome::window::SpectrumCanvas* specCanvas = dynamic_cast< kome::window::SpectrumCanvas* >( canvas );
	if( specCanvas != NULL ) {
		kome::objects::Spectrum* spec = specCanvas->getBaseSpectrum();
		// add operation
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("spec_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( spec->getSample() );
		
		if( spec != NULL ){
			opt->setSpec( spec );
		}

		opt->setCanvas( canvas );
		opt->setIndex( 2 );	// rest zoom
		opt->setRangeInfo( mgr.getRange( canvas ) );
		opt->setbReset( true );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	kome::window::ChromatogramCanvas* chromCanvas = dynamic_cast< kome::window::ChromatogramCanvas* >( canvas );
	if( chromCanvas != NULL ) {
		kome::objects::Chromatogram* chrom = chromCanvas->getBaseChromatogram();
		// add operation
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("chrom_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( chrom->getSample() );
		if( chrom != NULL ){
			opt->setChrom( chrom );
		}
		opt->setCanvas( canvas );
		
		opt->setIndex( 2 ); // rest zoom
		opt->setRangeInfo( mgr.getRange( canvas ) );
		opt->setbReset( true );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	return ret;
}

// @date 2012/11/30 <Add> OKADA
// reset range status
kome::objects::Variant resetZoomSubSpecFromContextMenu( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
		
	// get parameters
	kome::objects::Variant varObj =  kome::plugin::PluginCallTool::getActiveObject( params );
	
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	wxWindow* canvas = mgr.getCanvas( varObj );

	kome::window::SpectrumCanvas* specCanvas = dynamic_cast< kome::window::SpectrumCanvas* >( canvas );
	if( specCanvas != NULL ) {
		kome::objects::Spectrum* spec = specCanvas->getBaseSpectrum();
		// add operation
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("spec_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( spec->getSample() );
		
		if( spec != NULL ){
			opt->setSpec( spec );
		}

		opt->setCanvas( canvas );
		opt->setIndex( 2 );	// rest zoom
		opt->setRangeInfo( mgr.getRange( canvas ) );
		opt->setbReset( true );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	return ret;
}

// change spectrum range
kome::objects::Variant changeSpecRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}
	
	wxWindow* actCanvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( actCanvas == NULL ) {
		for( unsigned int i = 0; i < kome::window::Canvas::getNumberOfCanvases() && actCanvas == NULL; i++ ) {
			kome::window::SpectrumCanvas* canvas = dynamic_cast< kome::window::SpectrumCanvas* >( kome::window::Canvas::getCanvas( i ) );
			if( canvas != NULL ) {
				if( spec == canvas->getBaseSpectrum() ) {
					actCanvas = canvas;
				}
			}
		}
	}
	if( actCanvas == NULL ){
		return ret;
	}

	// range
	double start = double();
	double end = double();

	if( !kome::plugin::PluginCallTool::getStartOfRange( params, &start ) 
			|| !kome::plugin::PluginCallTool::getEndOfRange( params, &end ) ) {
		return ret;
	}
	
	// canvas
	// Spec No.87829 : Stack表示でスペクトルの拡大が出来ない @date 2012.10.12 <Mod> M.Izumi
	kome::window::Canvas* canvas = (actCanvas == NULL ? NULL : dynamic_cast< kome::window::SpectrumCanvas* >(actCanvas));
		
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	// add operation
	ProfileRangeOperation* opt = new ProfileRangeOperation();
	opt->setShortName("spec_range");
	opt->setOperationName( "Change Displaying Range" );
	opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
	opt->setTargetSample( spec->getSample() );
	opt->setSpec( spec );
	opt->setCanvas( canvas );
	
	opt->setIndex( 0 );//X軸のみ
	opt->setMinX( start );
	opt->setMaxX( end );
	opt->setRangeInfo( mgr.getRange( canvas ) );

	if( opt->execute() ){
		kome::window::WindowTool::refresh();
	}else{
		delete opt;
	}

	return ret;
}

// change chromatogram range
kome::objects::Variant changeChromRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	wxWindow* actCanvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( actCanvas == NULL ) {
		for( unsigned int i = 0; i < kome::window::Canvas::getNumberOfCanvases() && actCanvas == NULL; i++ ) { 
			kome::window::ChromatogramCanvas* canvas = dynamic_cast< kome::window::ChromatogramCanvas* >( kome::window::Canvas::getCanvas( i ) );
			if( canvas != NULL ) {
				if( chrom == canvas->getBaseChromatogram() ) {
					actCanvas = canvas;
				}
			}
		}
	}
	if( actCanvas == NULL ){
		return ret;
	}

	// range
	double start = double();
	double end = double();
	
	if( !kome::plugin::PluginCallTool::getStartOfRange( params, &start ) 
			|| !kome::plugin::PluginCallTool::getEndOfRange( params, &end ) ) {
		return ret;
	}

	// canvas
	// Spec No.87829 : Stack表示でスペクトルの拡大が出来ない @date 2012.10.12 <Mod> M.Izumi
	kome::window::Canvas* canvas = (actCanvas==NULL ? NULL : dynamic_cast< kome::window::ChromatogramCanvas* >(actCanvas));
	
	// manager
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	// add operation
	ProfileRangeOperation* opt = new ProfileRangeOperation();
	opt->setShortName("chrom_range");
	opt->setOperationName( "Change Displaying Range" );
	opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
	opt->setTargetSample( chrom->getSample() );
	opt->setChrom( chrom );
	opt->setCanvas( canvas );
	opt->setMinX( start );
	opt->setMaxX( end );
	opt->setIndex( 0 );
	opt->setRangeInfo( mgr.getRange( canvas ) );

	if( opt->execute() ){
		kome::window::WindowTool::refresh();
	}else{
		delete opt;
	}

	return ret;
}

// ステータスバーの設定 (Spectrum,Chromatogram)
kome::objects::Variant SetValStatusBar( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;	// @date 2011/07/29 <Add> OKADA
	
	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on mouse move
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	ret.prim.boolVal = mgr.setValStatusBar( *mouseEvent, *graphics, canvas );

	return ret;
}

// on mouse leave
kome::objects::Variant onMouseLeave( kome::objects::Parameters*  ){
	kome::objects::Variant ret;
		
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
		
	statusBar.setStatusMsg( "range", "" );
	statusBar.setStatusMsg( "coordinate", "" );
	statusBar.setFlgDisp( "range", 0 );			// ステータスバーを無効化
	statusBar.setFlgDisp( "coordinate", 0 );	// ステータスバーを無効化

	return ret;
}

// get spectrum range Operation
kome::objects::Variant getSpecRangeOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new ProfileRangeOperation();
	
	return ret;
}

// get chromatogram range Operation
kome::objects::Variant getChromRangeOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new ProfileRangeOperation();
	
	return ret;
}

// show Change Displayed Range
kome::objects::Variant showChangeSpectrumRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get specterum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// get range
	double startMz = double();
	double endMz = double();
	if( !kome::plugin::PluginCallTool::getRange( params, &startMz, &endMz ) ) {
		return ret;
	}

	// canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = (frame == NULL ? NULL : frame->getCanvas());

	// create Change Displayed Spectrum Range dialog
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.createChangeSpectrumRange( spec, canvas, startMz, endMz );

	return ret;
}

// show Change Displayed Range
kome::objects::Variant showChangeChromatogramRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}
	
	// get range
	double startMz = double();
	double endMz = double();
	if( !kome::plugin::PluginCallTool::getRange( params, &startMz, &endMz ) ) {
		return ret;
	}
	
	// canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = (frame == NULL ? NULL : frame->getCanvas());

	// create Change Displayed Chromatogram Range dialog
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	mgr.createChangeChromatogramRange( chromatogram, canvas, startMz, endMz );

	return ret;
}

// change displayed range on batch
kome::objects::Variant changeDisplayingRangeOnBatch( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// range
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to gettting the range information." ) );
		return ret;
	}

	std::string rangeStr = settings->getParameterValue( "range" );
	std::vector< std::string > tokens;
	stringtoken( rangeStr.c_str(), RANGE_VALUES_DELIMITER, tokens );
	if( tokens.size() == 0 ) {
		LOG_ERROR( FMT( "Failed to gettting the range information." ) );
		return ret;
	}

	double start = double();
	double end = double();

	if( !isdouble( tokens[ 0 ].c_str(), &start )
			|| !isdouble( tokens[ 1 ].c_str(), &end ) ) {
		LOG_ERROR( FMT( "Failed to gettting the range information." ) );
		return ret;
	}


	// samples
	const unsigned int specNum = aoMgr.getNumberOfOpenedSamples();

	// filtered spectra
	for( unsigned int i = 0; i < specNum; i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( sample );

		// spectrum range
		for( unsigned int j = 0; j < dataSet->getNumberOfSpectra(); j++ ) {
			kome::objects::Spectrum* spec = dataSet->getSpectrum( j );
			
			mgr.pushSpectrumXRange( mgr.getRange( NULL ), NULL, start, end );
		}

		// chromatogram range
		for( unsigned int j = 0; j < dataSet->getNumberOfChromatograms(); j++ ) {
			kome::objects::Chromatogram* chrom = dataSet->getChromatogram( j );
			mgr.pushChromatogramXRange( mgr.getRange( NULL ), NULL, start, end );
		}
	}

	return ret;
}
