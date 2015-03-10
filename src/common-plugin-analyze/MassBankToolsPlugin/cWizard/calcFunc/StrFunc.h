/**
 * @file StrFunc
 * @brief convert to Selections
 *
 * @author M.Fukuda
 * @date 2013.03.05
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

namespace ext {
	const std::string tranceLowerToUpper(const std::string& src);
	const std::string trancetoMassBankTag(const std::string& src);

	const bool isAlphabet(const std::string& src);
	const unsigned int toUnInt(const std::string& src);
	const int toInt(const std::string& src);
	const double toDbl(const std::string& src);
	const std::string toStr(const unsigned int num);
	const std::string toStr(const int num);
	const std::string toStr(const double val);
}
