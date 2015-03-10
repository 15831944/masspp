/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Mspp PluginCall helper headers
    @file       Peak_Positions.hpp
	@brief      Peak_Positions Ç∆ÇµÇƒÉRÅ[ÉãÇ≥ÇÍÇΩç€ÇÃï‚èï
	@date       2012.01.09
	@author     M.Fukuda
	@version    1.0.0
****************************************************************/
//#pragma once
//
//namespace mspp{
//
//inline const bool checkPeakPositionsParams (
//	kome::objects::Parameters* params,
//	kome::objects::DataGroupNode*& stdGroup,
//	kome::objects::SettingParameterValues*& settings,
//	kome::core::Progress*& progress
//) {
//	bool noErrorFg = true;
//	// settings
//	kome::objects::SettingParameterValues*
//	settingss = kome::plugin::PluginCallTool::getSettingValues(params);
//	if (settings == nullptr) {
//		LOG_ERROR(FMT("It failed to get the settings"));
//		noErrorFg = false;
//	}
//
//	// standard group
////	stdGroup = kome::plugin::PluginCallTool::getControlGroup(params);
//
//	// progress
//	progress = kome::plugin::PluginCallTool::getProgress(params);
//	if(progress == NULL) {
//		progress = &kome::core::Progress::getIgnoringProgress();
//	}
//
//	return noErrorFg;
//}
//
//} // end of namespace WRAP
