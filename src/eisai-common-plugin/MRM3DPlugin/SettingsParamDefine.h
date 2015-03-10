/**********************************************************/
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       SettingsParamDefine.h
    @brief      Settings Parameter Value Support Class Header
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
Setting Parameter Valuesを内部で分離して管理するクラス。
他のプラグインでもクラスのひな形をコピーすれば使えるはず。
*/
/**********************************************************/

#pragma once
#include <string>

namespace kome {
namespace objects{
	class SettingParameterValues;
} // end of namespace objects
	
namespace mrm3d {

class ParamParser {
private:
	/*! my Parameters */
	kome::objects::SettingParameterValues* m_settings;
	bool m_noiseFlag;
	bool m_intfilFlag;
	bool m_intfilABS;
	double m_filL, m_filR, m_maxIntensity;
public:
	explicit ParamParser (kome::objects::SettingParameterValues*);
	~ParamParser();
	const std::string& getdetectStr(void) const;
	const std::string& getsmoothStr(void) const;
	const bool checkParams(void);
	bool IsUseNoiseFilter(void) const;
	bool IsUseIntensityFilter(void) const;
	bool IsTypeABS(void) const;
	bool IsTypeREL(void) const;
	void setMaxIntensity(const double);
	int compInRange(const double);
	void showIntensityRangeInfo(const unsigned int removenum) const;

};

}// end of namespace mrm3d
}// end of namespace kome
