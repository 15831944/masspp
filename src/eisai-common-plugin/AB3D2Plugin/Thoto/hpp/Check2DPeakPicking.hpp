/**
2014/01/22 M.Fukuda
#Mass++全体でAb3DPluginTop.cppのみ使用を確認。
#コードを移植したため、不要になりました。
#ソリューションから削除対象にします
**/

/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Mspp PluginCall helper headers
    @file       2DPeak_Picking.hpp
	@brief      2DPeak_Pickingとしてコールされた際の補助
	@date       2012.12.13
	@author     M.Fukuda
	@version    1.0.0
****************************************************************/


#pragma once

//#include "stdafx_mininumSet.h"

namespace mspp {

// Plugin
inline const bool check2DPeakPickingParams (
	kome::objects::Parameters* params,
	kome::objects::DataSet*& dataSet,
	kome::objects::Peaks2D*& peaks2d,
	kome::objects::SettingParameterValues*& settings,
	kome::core::Progress*& progress
){
	// Plugin Call された時、
	// Paramから所定の手続きをして、各要素を引き出す必要有り。
	// 途中でエラーになった場合は必ず処理を打ち切るので、
	// 共通化の為に書いた。
	typedef kome::plugin::PluginCallTool tPluginCallTool;
	bool noErrorFg = true;

	// check dataSet
	dataSet = tPluginCallTool::getDataSet(params);
	if ( dataSet == nullptr ) {
		LOG_ERROR(FMT("It failed to get the dataSet"));
		noErrorFg =  false;
	}
	// check peaks2D
	peaks2d = tPluginCallTool::getPeaks2D(params);
	if ( peaks2d == nullptr ) {
		LOG_ERROR(FMT("It failed to get the peaks2D"));
		noErrorFg =  false;
	}
	// check have setting params
	settings = tPluginCallTool::getSettingValues(params);
	if (settings == nullptr) {
		LOG_ERROR(FMT("It failed to get the settings"));
		noErrorFg = false;
	}
	// set Progress
	progress = tPluginCallTool::getProgress(params);

	if( progress == nullptr ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	return noErrorFg;
}

} // end of namespace
