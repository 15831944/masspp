/**********************************************************/
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       FilterParser.h
    @brief      Filter Parser Functions Header
    @date       2012.10.16
	@author     M.Fukuda
	@version    1.0.0
	@par        
	@note 
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.19
SPECTRUMのFILTER情報(Thermoのみ)をパースする関数群
・Q1 Q3 区間の文字列へ区切る
・Q1 を doubleにキャストする
・Q3 の　xxx.x - yyy.y を分離して flont ,center(q3), end とする
*/
/**********************************************************/
#pragma once

#include <string>
#include <vector>

/*!
 * @brief Parse spectrum has filter into to Q1 & Q3Array
 * @param[in]  string : spectrum had Filter
 * @param[out] string : q1 info (as precursor)
 * @param[out] string : q3 info (splited canma)
 * @retval true Successed parse.
 * @date 2012.10.15
 */
const bool MRMFilterParser(
	const std::string& srcFilter,
	std::string& q1Str,
	std::vector<std::string>& q3StrArr);

/*!
 * @brief Cast Q1 Value from string to double 
 * @param[in]  string : q1 info
 * @param[out] double : q1 value
 * @retval true Successed cast
 * @date 2012.10.15
 */
const bool Q1_Cast(const std::string& q1str, double& q1);

/*!
 @brief Parse Q3 Value
 @param[in]  string : q3 info
 @param[out] double : q3 (q3 range's center value)
 @param[out] double : q3f (q3 front value)
 @param[out] double : q3e (q3 end value)
 @retval true Successed parse.
 @date 2012.10.18
 */
const bool Q3Parser (
	const std::string& q3str,
	double& q3, double& q3front, double& q3end);
