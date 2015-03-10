/**********************************************************/
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       SettingsParamDefine.cpp
    @brief      Settings Parameter Value Support Class
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
Setting Parameter Valuesを内部で分離して管理するクラス。
*/
/**********************************************************/
#include "SettingsParamDefine.h"
#include "stdafx.h"

namespace kome{
namespace mrm3d{

/*!
 *  PARAMERTER XMLの定義語一覧をここに書き出す。
 *  補助関数（整合性を取るなど必要ならクラスを追加で定義する
 */
const std::string kXMLPARAM_DETECTOR("chrom_peak_detector");
const std::string kXMLPARAM_SMOOTER("chrom_smoothing");
const std::string kXMLPARAM_USENOISEFILTER("use_noise_filter");
const std::string kXMLPARAM_INTNSITYFILTYPE("intensity_filter_type");
const std::string kXMLPARAM_INTENSITY_MIN_VALUE("minimum_range_value");
const std::string kXMLPARAM_INTENSITY_MAX_VALUE("maximum_range_value");
const std::string kENUM_NOUSE("nouse");
const std::string kENUM_RELATIVE("rel");
const std::string kENUM_ABSOLUTE("abs");

//constructor
ParamParser::ParamParser(kome::objects::SettingParameterValues* s)
	: m_settings(s) {}

//destructor
ParamParser::~ParamParser(){}

//getter
const std::string& ParamParser::getdetectStr(void) const { return kXMLPARAM_DETECTOR; }
const std::string& ParamParser::getsmoothStr(void) const { return kXMLPARAM_SMOOTER; }

//Settings Paramter ValuesをXMLの情報からパースする。
//過不足・違反があればFalseを返す。
const bool ParamParser::checkParams(void) {
	if (m_settings == NULL) return false;
	// 必要であれば追加実装
	//	m_noiseFlag = m_settings->getBoolValue(kXMLPARAM_USENOISEFILTER.c_str(), false);
	m_noiseFlag = false;

	// Intensity Filter Typeからフラグを設定
	std::string stype = m_settings->getParameterValue(kXMLPARAM_INTNSITYFILTYPE.c_str());
	if (stype.compare(kENUM_RELATIVE) == 0) {
		m_intfilFlag = true;
		m_intfilABS = false;
	} else if (stype.compare(kENUM_ABSOLUTE) == 0) {
		m_intfilFlag = true;
		m_intfilABS = true;
	} else {
		m_intfilFlag = false;
	}

	if (m_intfilFlag) {
		double dl,dr;
		dl = m_settings->getDoubleValue(kXMLPARAM_INTENSITY_MIN_VALUE.c_str(),-1.0);
		dr = m_settings->getDoubleValue(kXMLPARAM_INTENSITY_MAX_VALUE.c_str(),DBL_MAX);
		// 絶対値の場合
		// 下界値は空白(0.0以下)なら -1.0に固定する。
		// 上界値は空白(0.0以下)なら DBL_MAXとする。
		if (m_intfilABS) {
			m_filL = (dl < 0.0) ? -1.0 : dl;
			m_filR = (dr < 0.0) ? DBL_MAX : dr;
		// 相対値の場合
		// 後でMaxIntensityを受け取り、再計算するので比率のみを維持する。
		// 下界値は空白(0.0以下)なら 0.0にする。
		// 上界値は空白(0.0以下)なら 1.0とする。
		} else {
			m_filL = (dl < 0.0) ? 0.0 : dl * 0.01;
			m_filR = (dr > 100.0) ? 1.0 : dr * 0.01;
		}
		// 範囲の定義エラー(下界>上界)になる場合はFalse
		if (m_filR <= m_filL) return false;
	}
	return true;
}

// Noise Filterを使うか否か
bool ParamParser::IsUseNoiseFilter(void) const {
	return m_noiseFlag;
}

// Intensity Filterを使うならTrue
bool ParamParser::IsUseIntensityFilter(void) const {
	return m_intfilFlag;
}

// Intensity FilterがABSならTrue
bool ParamParser::IsTypeABS(void) const {
	return m_intfilABS;
}

// Intensity FilterがRelativeならTrue
bool ParamParser::IsTypeREL(void) const {
	return !(m_intfilABS);
}

// MaxIntensityを取得する。
// Relativeであれば内部的に再計算する。
void ParamParser::setMaxIntensity(const double maxIntensity) {
	if (!m_intfilABS) {
		m_maxIntensity = maxIntensity;
		m_filL *= maxIntensity;
		m_filR *= maxIntensity;
	}
}

// 範囲以下なら -1.範囲以上なら 1.範囲内なら 0.(Strcompと同じ)
int ParamParser::compInRange(const double tgtIntensity) {
	if (tgtIntensity < m_filL) return -1;
	if (m_filR < tgtIntensity) return 1;
	return 0;
}

// Intensity Filter Range　を使用した場合に結果を出す。
// 邪魔だと判断された場合はコメント化する。
void ParamParser::showIntensityRangeInfo(const unsigned int removenum) const {
	std::string tmpStr = "-- Filtering Intensity Range --\n";
	tmpStr += (m_filL <= 0.0) ? "0.0" : FMT("%f", m_filL);
	tmpStr += " to ";
	if (m_intfilABS) {
		tmpStr += (m_filR == DBL_MAX) ? "Infinity" : FMT("%f", m_filR);
	} else {
		tmpStr += FMT("%f",m_filR);
		if (m_filR == m_maxIntensity) { tmpStr += "(maxIntensity)";}
	}
	tmpStr += FMT("\nFiltered Peaks %d", removenum);
	kome::window::WindowTool::showInfo(tmpStr.c_str());
}

}
}
