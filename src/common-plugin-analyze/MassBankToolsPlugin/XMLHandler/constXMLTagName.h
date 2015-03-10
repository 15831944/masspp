/**
 * @file constXMLTagName.cpp
 * @brief XMLで使用するタグを定義する
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#pragma once
#include <string>

struct XML {
	struct TAG {
		static const std::string MZS;
		static const std::string INTENSITY;
		static const std::string INSTTYPE;
		static const std::string INST;
		static const std::string UNIT;
		static const std::string TOL;
		static const std::string CUTOFF;
		static const std::string ION_MODE;
		static const std::string MAX_NUM;
		static const std::string MZ;
		static const std::string RELINT;
		static const std::string TITLE;
		static const std::string MAIL;
		static const std::string PRECURSOR;
		static const std::string MSTYPE;
		static const std::string RETURN;
	};
};
