/*********************************************************/
/*!
 @addtoGroup AB3D
 @file AB3DLabelingTop.h
 @brief AB3DLabeling Top Entry Point Interface Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

#include "stdafx.h"
#include <string>

#include <iostream>
#include <fstream>
#include <boost/timer.hpp>

//@@Date 2014.01.22 
//PeakPositionsの定義変更のため、不要コードになった。
#define __NO_NEED_EMS_MINIMUN_STFAFX_H__
#define __CALLTYPE_2DPEAKPICKING__
#define __CALLTYPE_2DPEAKPOSITION__
#include  "Thoto/PluginToolHelper.hpp"

#include "NewAB3DLabeling.h"
#include "AB3DPeakPosDetector.h"

// AB3Dのパラメータを取得する
bool getAB3DParameters(
	kome::objects::SettingParameterValues* settings, 
	std::map<std::string, std::string>& setmap
){
	std::string tmpName;
	unsigned int numOfParams;

	if (settings == nullptr) {
		LOG_ERROR(FMT("SettingParameterValues was not set."));
		return false;
	}

	// Set advanced parameters to setmap
	numOfParams = settings->getNumberOfParameters();
	LOG_DEBUG(FMT("Number of simple parameters: %d", numOfParams));
	for (unsigned int i = 0; i < numOfParams; i++) {
		tmpName = settings->getParameterName(i);
		if (tmpName != "AdvancedParams") {
			setmap[tmpName] = settings->getParameterValue(i);
			LOG_DEBUG(FMT("(#%d)[%s]:(%s)", i, tmpName.c_str(), settings->getParameterValue(i)));
		}
	}
	
	// Set advanced parameters to setmap
	kome::objects::SettingParameterValues* subParams = settings->getSubParameterValues("AdvancedParams", "ab3dadvanced2");
	if (subParams == nullptr) {
		LOG_ERROR(FMT("Failed to get subParams"));
		return false;
	}
	numOfParams = subParams->getNumberOfParameters();
	LOG_DEBUG(FMT("Number of advanced parameters: %d", numOfParams));
	for (unsigned int i = 0; i < numOfParams; ++i) {
		tmpName = subParams->getParameterName(i);
		setmap[tmpName] = subParams->getParameterValue(i);
		LOG_DEBUG(FMT("(#%d)[%s]:(%s)", i, tmpName.c_str(), subParams->getParameterValue(i)));
	}

	return true;
}

// Dialogから2d_peak_pickingとしてAB3Dを選択されたときに呼び出されます。
// ... .defでインタフェースを定義、関連付けは plugin.xmlで行われます。
// 関数名を変更する場合は　def, plugin両方の変更を行ってください。
kome::objects::Variant detect2dPeaks(kome::objects::Parameters* params) {

	using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


	// return objects
	Variant ret;
	ret.prim.boolVal = false;

	DataSet* dataSet;
	Peaks2D* peaks2d;
	SettingParameterValues* settings;
	kome::core::Progress* progress;

	boost::timer timer;

	LOG_INFO(FMT("AB3D detecting 2D peaks algorithm started."));

	wxBeginBusyCursor();

	// Get 2D peak picking parameters
	if (mspp::check2DPeakPickingParams(params, dataSet, peaks2d, settings, progress)) {

		std::map<std::string, std::string> setmap;
		
		// Get AB3D parameters
		if (getAB3DParameters(settings, setmap)) {

			// Testが押下されたときに、画面で表示している範囲を対象とする。
			const double startMz = kome::plugin::PluginCallTool::getDouble(params, "start_mz", -1);
			const double endMz = kome::plugin::PluginCallTool::getDouble(params, "end_mz", -1);
			const double startRt = kome::plugin::PluginCallTool::getDouble(params, "start_rt", -1);
			const double endRt = kome::plugin::PluginCallTool::getDouble(params, "end_rt", -1);
			if (startMz != -1 && endMz != -1) {
				setmap["MzRange"] = FMT("%f:%f", startMz, endMz);
			}
			if (startRt != -1 && endRt != -1) {
				setmap["RtRange"] = FMT("%f:%f", startRt, endRt);
			}

			NewAB3DLabeling ab3d(setmap);
			ab3d.bindProgress(progress);
			ab3d.doAB3D(*dataSet, peaks2d);
			ret.prim.boolVal = true;

		} else {
			LOG_ERROR(FMT("Failed to get AB3D parameters."));
		}
	} else {
		LOG_ERROR(FMT("Failed to check 2D peak picking parameters."));
	}

	wxEndBusyCursor();

	LOG_INFO(FMT("AB3D detecting 2D peak algorithm finished. Elapsed Time: %.03f sec", timer.elapsed()));

	return ret;
}

// peak position
kome::objects::Variant detectPeakPosition(
	kome::objects::Parameters* params
) {
	// return value
	kome::objects::Variant ret;
	ret.prim.pt = nullptr;

	kome::objects::SettingParameterValues* settings 
		= kome::plugin::PluginCallTool::getSettingValues(params);

	if (settings == nullptr) {
		LOG_ERROR(FMT("It failed to get the settings."));
		return ret;
	}

	double mzppm = settings->getDoubleValue("mz_tol", 10.0);
	double rttol = settings->getDoubleValue("rt_tol",  1.0);
		
	kome::objects::SettingParameterValues* peakSettings
		= settings->getSubParameterValues("AB3DPeakParam", "ab3d2");
	bool successfg = false;

	if (peakSettings != nullptr) {
		std::map<std::string, std::string> setmap;
		// Get AB3D parameters
		if (getAB3DParameters(peakSettings, setmap)) {
			// AB3D Peak positon detector
			Ab3dPeakPosDetect* peakPos = new Ab3dPeakPosDetect(mzppm, rttol, setmap, settings);
			ret.prim.pt = peakPos;
			successfg = true;
		}
	}
	if(!successfg) {
		LOG_ERROR(FMT("Failed to get AB3D parameters."));
	}

	return ret;
}
