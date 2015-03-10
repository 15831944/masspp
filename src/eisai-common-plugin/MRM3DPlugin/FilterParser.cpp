/**********************************************************
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       FilterParser.h
    @brief      MRM Filter values parse Functions Header
    @date       2012.10.22
	@author     M.Fukuda
	@version    1.0.0
	@note 
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.

**********************************************************
    @par
		SRM ms2 123.4 [222.2-222.4,333.3-333.5] は
		SRM ms2 Q1Val [ Q3Val(1), Q3Val(2)] である。
		Q3Valは範囲。値として利用する場合は中央値を取る。
		この情報を分割する関数群。
**********************************************************/

#include "FilterParser.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

// SRM Regex
const std::string SRM_REGEX("SRM ms2 ([0-9]+[.|0-9]+) (\\[)([^\\]]+)(\\])");

// Like Define Magic Words
const std::string Q3_DELIMITER(",");
const std::string Q3_ELEMEMT_BAR("-");
const int Q1_BLOCK = 1;
const int Q3_BLOCK = 3;

// MRM Filter String Parse to Q1 And Q3 elements Array
const bool MRMFilterParser(
	const std::string& srcFilter,
	std::string& q1Str,
	std::vector<std::string>& q3StrArr
) {
	/* Q1とQ3を分割して文字列で返す。
	 * Ex) in  SRM ms2 123.4 [222.2-222.4,333.3-333.5] 
	 *     out Q1Str    = 123.4
	 *     out Q3StrArr = (222.2-222.4),(333.3-333.5)
     */
	const boost::regex regexSRM(SRM_REGEX);
	const boost::regex regexQ3(Q3_DELIMITER);
	boost::smatch rstReg;
	
	// reflesh
	q1Str.clear();
	q3StrArr.clear();

	if (regex_search(srcFilter, rstReg, regexSRM)) {
		// parse q1
		q1Str = rstReg.str(Q1_BLOCK);
		// parse q3 block
		std::string tmp = rstReg.str(Q3_BLOCK);
		// split q3 block as xxx.x - yyy.y
		regex_split(std::back_inserter(q3StrArr), tmp, regexQ3);
	} else {
		// no match SRM Type
		return false;
	}

	if (q1Str.empty() || q3StrArr.empty()) {
		return false;
	}
	return true;
}

// Change To Q1 Str
const bool Q1_Cast(const std::string& q1str, double& q1) {
	// Q1をdoubleに変換する。キャストエラー時はFalseで終了させる。
	try {
		q1 = boost::lexical_cast<double>(q1str);
    } 
	catch (boost::bad_lexical_cast &e ) 
	{   
		return false;
    }
	return  true;
}

// MRM Q3Parser xxx.x - yyy.y to 
// center val and xxx.x(front) yyy.y(end)
const bool Q3Parser (
	const std::string& q3str,
	double& q3, double& q3front, double& q3end
) {
	// Q3の範囲値を double として　左端、中央、右端 に
	// 分割する。分割時にエラーが発生した場合はエラーを返す。
	std::vector<std::string> q3tmpArr;
	// split q3 elem
	boost::algorithm::split(q3tmpArr, q3str, boost::is_any_of("-"));
	// lexcial_cast 時に空白があるとエラーが出るので対策
	boost::trim(q3tmpArr.at(0));
	boost::trim(q3tmpArr.at(1));

	// cast to double
	try {
		q3front = boost::lexical_cast<double>(q3tmpArr.at(0));
		q3end = boost::lexical_cast<double>(q3tmpArr.at(1));
	} catch (boost::bad_lexical_cast &e )
	{
		return false;
	}
	q3 = (q3front + q3end) / 2.0;
	return true;
}
