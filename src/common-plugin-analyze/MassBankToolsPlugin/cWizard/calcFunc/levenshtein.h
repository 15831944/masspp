/**
 * @file levenshtein.h
 * @brief Levenshtein distance
 *
 * @author M.Fukuda
 * @date 2013.02.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include<string>

namespace ext {

	/**
	 @brief levenshtein distance function
	 @param[in] sX target string1
	 @param[in] sY target string2
	 @return difference between two sequences score (if 0 ,matched!)
	*/
	const int levenshtein(
		const std::string& sX, 
		const std::string& sY
		);
}
