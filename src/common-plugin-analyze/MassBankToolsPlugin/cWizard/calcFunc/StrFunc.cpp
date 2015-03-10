/**
 * @file StrFunc
 * @brief convert to Selections
 *
 * @author M.Fukuda
 * @date 2013.03.05
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "StrFunc.h"
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace ext {

// 小文字を大文字に変換する
const std::string tranceLowerToUpper(const std::string& src) 
{
	std::string dst = src;
	std::transform(dst.begin(),dst.end(),dst.begin(),[](char c){return (islower(c)) ? toupper(c) : c;});
	return dst;
}

// 小文字を大文字に変換し、空白を _ （アンダーバー）に変換する
const std::string trancetoMassBankTag(const std::string& src) 
{
	std::string dst = src;
	std::transform(dst.begin(),dst.end(),dst.begin(),
		[](char c){return (c == ' ') ? '_' : (islower(c)) ? toupper(c) : c;});
	return dst;
}

//　要素が全てアルファベットか否かをチェックする
const bool isAlphabet(const std::string& src) 
{
	std::string dst = src;
	boost::remove_erase_if(dst,[](char c)->bool{ return ! isalpha(c);});
	return (src.compare(dst) == 0);
}

// キャスト　STR -> Uint
const unsigned int toUnInt(const std::string& src) 
{
	if (src.empty()) return 0;
	return boost::lexical_cast<unsigned int>(src);
}

// キャスト　STR -> int
const int toInt(const std::string& src) 
{
	if (src.empty()) return -1;
	return boost::lexical_cast<int>(src);
}

// キャスト　STR -> double
const double toDbl(const std::string& src)
{
	if (src.empty()) return -1.0;
	return boost::lexical_cast<double>(src);
}

// キャスト　Uint -> STR
const std::string toStr(const unsigned int num) 
{
	return boost::lexical_cast<std::string>(num);
}

// キャスト　int -> STR
const std::string toStr(const int num)
{
	return boost::lexical_cast<std::string>(num);
}

// キャスト　double -> STR
const std::string toStr(const double val)
{
	return boost::lexical_cast<std::string>(val);
}

} // end of namespace
