/**
 * @file ErrorCheck
 * @brief 警告の表示をまとめたもの
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once
#include <string>
#include <vector>
namespace CHK {

// 選択されているスペクトラムが無い return false
const bool showErrorNoSpecSelect();
const bool showErrorNoSpecSelectAndAveraged();

const bool checkPrefix(const std::string& prefix, const bool showErrFg = false);
const bool checkInputedPrefix(const std::string& prefix);
const bool checkBaseNum(const std::string& preval, const int basenum);
const bool checkBaseNum(const std::string& preval, const int basenum, const unsigned int futurecount);
const bool checkBaseNum_s(const std::string& preval, const std::string& basenum, const unsigned int futurecount);

const bool checkSaveFolderPath(const std::string& FolderPath);

const bool SplitTopParams(const std::string& src, std::vector<std::string>& vals);

const bool isOverlap(
	std::vector<std::string> vals, 
	const int etype = 0 ,
	bool showMsg = true);

const bool checkSubTags(
	const std::string& src, 
	const std::vector<std::string>& ngVals,
	bool showMsg = true);
}
