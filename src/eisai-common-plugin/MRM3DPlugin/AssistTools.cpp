/**********************************************************
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       AssistTool.cpp
    @brief      PluginCallTools Helper Functions
    @date       2012.10.22
	@author     M.Fukuda
	@version    1.0.0
	@note 
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
**********************************************************
	@par   
	Mass++のPluginCall処理様式をクラス・カプセル化の試験。
	setUpにてsettingsから関数名等の情報を読んで確保。
	invokeMeにて実行。
**********************************************************/

#include "stdafx.h"
#include "AssistTools.h"

using namespace mspp::sprt;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//--------------------------------------------------------------
//  PEAK DETECTER
//--------------------------------------------------------------
PeakDetector::PeakDetector(const std::string& XMLparamname)
	: paramName_(XMLparamname)
{
}

PeakDetector::~PeakDetector(){}

// Setup from setting XML params
const bool PeakDetector::setUp(kome::objects::SettingParameterValues* settings, bool isChrom) {
	// settingを読み込む。
	// NULL、該当パラメータ値が存在しない、該当アルゴリズムが無い。場合はFALSEを返す
	// (Detectに関しては計算不能になる）
	if (settings == NULL) { 
		return false;
	}
	const char* shortname = settings->getParameterValue(paramName_.c_str());
	if (shortname == NULL) {
		return false;
	}

	// Get Plugin Manager Instance
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	if (isChrom) {
		// Chromatogramなら Chromatogramタイプを探す
		m_func = plgMgr.getFunctionItem(chromFunc.c_str(), shortname);
	} else {
		m_func = plgMgr.getFunctionItem(specFunc.c_str(), shortname);
	}
	if (m_func == NULL) {
		return false;
	}
	m_setParams = settings->getSubParameterValues(paramName_.c_str(), shortname);
	return true;
}

// Call Peak Detector
const bool PeakDetector::invokeMe(kome::core::XYData& xyData, kome::objects::Peaks& peaks) {
	// setup Parameters
	kome::objects::Parameters peakParams;
	kome::plugin::PluginCallTool::setXYData(peakParams, xyData);
	kome::plugin::PluginCallTool::setPeaks(peakParams, peaks);
	if (m_setParams != NULL) {
		kome::plugin::PluginCallTool::setSettingValues(peakParams, *m_setParams);
	}
	kome::objects::Variant ret = m_func->getCall()->invoke(&peakParams);
	return true;
}

//--------------------------------------------------------------
//  PEAK SMOOTER
//--------------------------------------------------------------
/*! constructor */
PeakSmoother::PeakSmoother(const std::string& XMLparamname) : paramName_(XMLparamname){}

/*! destructor */
PeakSmoother::~PeakSmoother(){}

const bool PeakSmoother::setUp(kome::objects::SettingParameterValues* settings) {
	if (settings == NULL) {
		return false;
	}
	const char* shortname = settings->getParameterValue(paramName_.c_str());
	if (shortname == NULL) {
		return false;
	}
	// Get Plugin Manager Instance
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_func = plgMgr.getFunctionItem(kXYDataSmooth.c_str(), shortname);
	if (m_func == NULL) {
		return false;
	}
	m_setParams = settings->getSubParameterValues(paramName_.c_str(), shortname);
	return true;
}

// 単体実行（結果を上書きして構わない場合）
const bool PeakSmoother::invokeMe(kome::core::XYData*& srcXY) {
	kome::core::XYData* dstXY = new kome::core::DataPoints();
	if (invokeMe(*srcXY, *dstXY)) {
		srcXY->clearPoints();
		delete srcXY;
		srcXY = dstXY;
		return true;
	} else {
		delete dstXY;
	}
	return false;
}

// 複数実行（結果を残しておきたい場合）
const bool PeakSmoother::invokeMe(kome::core::XYData& srcXY, kome::core::XYData& dstXY) {
	if (m_func == nullptr) return false;
	kome::objects::Parameters smoothParams;
	kome::plugin::PluginCallTool::setXYData(smoothParams, srcXY);
	kome::plugin::PluginCallTool::setUpdatedXYData(smoothParams, dstXY);
	if (m_setParams != NULL) {
		kome::plugin::PluginCallTool::setSettingValues(smoothParams, *m_setParams);
	}
	kome::objects::Variant ret = m_func->getCall()->invoke(&smoothParams);
	return true;
}


//--------------------------------------------------------------
//  PROGRESS KEEPER
//--------------------------------------------------------------
ProgressKeeper::ProgressKeeper (
	kome::core::Progress& progress, 
	unsigned int maxcount, 
	const std::string& statusMsg
) : progress_(progress) , maxcount_(maxcount), status_(statusMsg)
{
	progress.setRange(0, maxcount);
	totalcount_ = 0;
	clockcount_ = 0;
	clockbasecount_ = maxcount / 100 + 1;
	isEnd_ = false;
}

// オブジェクト破棄時にprogressをfillしてやる。
ProgressKeeper::~ProgressKeeper(){
	if (!isEnd_) {
		progress_.fill();
	}
}

// 呼ばれる都度、内部カウントと付随メッセージを変える。
// for each 等の内部で呼ばれる影響を受けない。
void ProgressKeeper::showNext() {
	if (clockcount_ > clockbasecount_) {
		clockcount_ = 0;
		progress_.setPosition(totalcount_);
		progress_.setStatus(FMT("%s : [%d/%d]", status_.c_str(), totalcount_, maxcount_).c_str());
	}
	totalcount_++;
	clockcount_++;
}

void ProgressKeeper::doFinish() {
	progress_.fill();
	isEnd_ = true;
}
