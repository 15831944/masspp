/**
 * @file settingParamsKeywords
 * @brief MassBank's parameter.ini's Keyword
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#pragma once
#include <string>

struct CPRM {
	struct SPEC {
		static const std::string UNIT;
		static const std::string TOL;
		static const std::string CUTOFF;
		static const std::string PRECURSOR;	
		static const std::string MSTYPE;
		static const std::string IONMODE;
		static const std::string MAXNUM;
	};
	struct SRCH {
		static const std::string MZLIST;
		static const std::string INTNSTY;
		static const std::string TOL;
		static const std::string MSTYPE;
		static const std::string IONMODE;
		static const std::string MAXNUM;
	};
	struct DIFF {
		static const std::string VALLIST;
		static const std::string INTNSTY;
		static const std::string TOL;
		static const std::string MSTYPE;
		static const std::string IONMODE;
		static const std::string MAXNUM;
	};
};


