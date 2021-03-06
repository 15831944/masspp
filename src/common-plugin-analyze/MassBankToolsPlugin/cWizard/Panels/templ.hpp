/**
 * @file templ
 * @brief Masspp Managerから直接 ini値のやり取りをする際に使う。
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

// -- Forward declaration --
namespace kome {
	namespace core {
		class IniFile;
	}
}

class ParamIniAcc
{
public:
	ParamIniAcc(const std::string& _section, const std::string& _paramKey);
private:
	kome::core::IniFile* mparam;
	const std::string& section;
	const std::string& paramKey;
public:
	template<typename T> T getParam(void) const;
	template<typename U> void setParam(const U& u);
};

// 特殊化されている部分の実装部は CPP内にある