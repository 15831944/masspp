/**
2014/01/22 M.Fukuda
#Mass++全体で使用していない事を確認
#ソリューションから削除対象にします
**/

/**
#pragma once

namespace RFN{
namespace WRAP{

inline const bool judgeParams_PeakPicking (
	kome::objects::Parameters* params,
	kome::core::XYData*& xyData,
	kome::objects::Peaks*& peaks,
	kome::objects::SettingParameterValues*& settings
){
	typedef kome::plugin::PluginCallTool tPluginCallTool;

	if (params == nullptr) return false;

	// get xy data object
	xyData = tPluginCallTool::getXYData(params);
	if( xyData == nullptr ) return false;

	// get peaks object
	peaks = tPluginCallTool::getPeaks(params);
	if( peaks == NULL ) return false;

	settings = tPluginCallTool::getSettingValues(params);
	return true;
}

}
}
**/