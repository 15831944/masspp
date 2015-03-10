/**
 * @file SpectrumChangerPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2010.09.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SpectrumChangerPluginFunctions.h"

#include <wx/wx.h>

// 表示スペクトルを切り替えるプラグイン

kome::objects::Variant openNextSpectrum( kome::objects::Parameters* params ) {
	return openNextPrevSpectrum( params, 1 );
}

kome::objects::Variant openPrevSpectrum( kome::objects::Parameters* params ) {
	return openNextPrevSpectrum( params, 0 );
}

// flgPrevNext: 0=Prev 1=Next
kome::objects::Variant openNextPrevSpectrum( kome::objects::Parameters* params, int flgPrevNext ) {
	// 戻り値 (成功時 true, 失敗時 false)
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// 現在選択されているスペクトルを取得
	kome::objects::Spectrum* specSelected = NULL;
	if( var.type == var.SPECTRUM ){
		// メニューで切り替える時
		kome::objects::MsDataVariant obj( var );
		specSelected = obj.getSpectrum();
	}else{
		// キーボードイベント
		specSelected = kome::plugin::PluginCallTool::getSpectrum( params );
	}
	if( specSelected == NULL ) {
		return ret;
	}

	// スペクトルからgroupを取得
	kome::objects::DataGroupNode* group = specSelected->getGroup();

	if( group == NULL ) {
		LOG_ERROR( FMT( "A group is not active." ) );
		return ret;
	}
	
	unsigned int uNumberOfSpectrum = group->getNumberOfSpectra();
	if( uNumberOfSpectrum <= 0 ){
		// error	// スペクトルが１つもない
		return ret;	// 異常終了
	}else{
		long lnNextNum = -1;
		long lnPrevtNum = -1;
		kome::objects::Spectrum* spec;
		for( unsigned int i=0; i<uNumberOfSpectrum; i++ ){
			spec = group->getSpectrum( i );
			if( specSelected->getScanNumber() == spec->getScanNumber() ){
				// 一致
				// SPEC No.79890 Next Spectrum/Prev Spectrum 次のSpectrum画像を表示しない不具合修正
				// @date 2011.02.07 <Mod> M.Izumi
				lnNextNum =  i+1;
				lnPrevtNum = i-1;

				if( (long)uNumberOfSpectrum <= lnNextNum ){
					lnNextNum = (uNumberOfSpectrum-1);
				}
				if( lnPrevtNum < 0 ){
					lnPrevtNum = 0;
				}

				if( flgPrevNext == 0 ){
					spec = group->getSpectrum( lnPrevtNum );	// Prev
				}else{
					spec = group->getSpectrum( lnNextNum );		// Next
				}

				break;	// 検索終了
			}
		}

		// Spectrum表示
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *spec );
		}
	}

	// 正常終了
	ret.prim.boolVal = true;

	return ret;
}

// @date 2012.04.19 <Add> M.Izumi ->
// enable next spectrum
kome::objects::Variant enableNextSpectrum( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		kome::objects::DataGroupNode* group = specSelected->getGroup();
		if( group == NULL ){
			return ret;
		}

		int inum = group->getNumberOfSpectra()-1;
		// 選択中のスペクトルが後尾かどうかチェック
		if(  specSelected != group->getSpectrum( inum ) ){
			ret.prim.boolVal = true;
		}
	}

	return ret;
}

// enable prev spectrum
kome::objects::Variant enablePrevSpectrum( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		kome::objects::DataGroupNode* group = specSelected->getGroup();
		if( group == NULL ){
			return ret;
		}
		// 選択中のスペクトルが先頭かどうかチェック
		if( specSelected != group->getSpectrum(0) ){
			ret.prim.boolVal = true;
		}
	}

	return ret;
}
// @date 2012.04.19 <Add> M.Izumi <-

// on button down (spectrum)
kome::objects::Variant onSpecKeyDownX( kome::objects::Parameters* params ) {
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
		
	kome::objects::DataGroupNode* group = spectrum->getGroup();
	if( group == NULL ) {
		return ret;
	}
	
	if( ( keyEvent->getCode() == kome::core::key::KEY_UP ) && ( keyEvent->ctrl() == true ) ){
			openPrevSpectrum( params );
	}else if( ( keyEvent->getCode() == kome::core::key::KEY_DOWN ) && ( keyEvent->ctrl() == true ) ){
			openNextSpectrum( params );
	}
	else{
		return ret;
	}

	return ret;
}
