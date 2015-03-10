/**
 * @file CommonLicenseArray.cpp
 * @brief Common License (make vector)
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "CommonLicenseArray.h"

const std::string kLK_CCBY("CC BY");
const std::string kLK_CCBYSA("CC BY-SA");
const std::string kLK_CCBYND("CC BY-ND");
const std::string kLK_CCBYNC("CC BY-NC");
const std::string kLK_CCBYNCSA("CC BY-NC-SA");
const std::string kLK_CCBYNCND("CC BY-NC-ND");

const std::string k_help_CCBY = "CC BY (commercial use: allowed, modify: allowed)";
const std::string k_help_CCBYSA = "CC BY-SA (commercial use: allowed, modify: allowed, but license inherited)";
const std::string k_help_CCBYND = "CC BY-ND (commercial use: allowed, modify: not allowed)";
const std::string k_help_CCBYNC = "CC BY-NC (commercial use: not allowed, modify: allowed)";
const std::string k_help_CCBYNCSA = "CC BY-NC-SA (commercial use: not allowed, modify: allowed, but license inherited)";
const std::string k_help_CCBYNCND = "CC BY-NC-ND (commercial use: not allowed, modify: not allowed)";

const bool CCL::getLisenceElum(
	std::vector<std::string>& refElum,
	std::vector<std::string>& refDscs
){
	// Add View & Help (0-5)
	refElum.push_back(k_help_CCBY);
	refElum.push_back(k_help_CCBYSA);
	refElum.push_back(k_help_CCBYND);
	refElum.push_back(k_help_CCBYNC);
	refElum.push_back(k_help_CCBYNCSA);
	refElum.push_back(k_help_CCBYNCND);
	// Add Value (6-11)
	refDscs.push_back(kLK_CCBY);
	refDscs.push_back(kLK_CCBYSA);
	refDscs.push_back(kLK_CCBYND);
	refDscs.push_back(kLK_CCBYNC);
	refDscs.push_back(kLK_CCBYNCSA);
	refDscs.push_back(kLK_CCBYNCND);
	return true;
}
