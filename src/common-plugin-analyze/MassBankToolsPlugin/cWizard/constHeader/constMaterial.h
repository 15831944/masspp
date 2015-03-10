/**
 * @file constMaterial.h
 * @brief Wizard matrial(Label,ImagePath,..) message declaration
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

struct LBL {
	struct W01 {
		static const std::string ALLSAMPLE;
		static const std::string DISPLAYED;
		static const std::string SAMPLELIST;
		static const std::string SPECLIST;
		static const std::string AVERAGE;
		static const std::string PROCCED;
	};
	struct W02 {
		static const std::string ACCETION;
	};
	struct W08 {
		static const std::string LISTINFO;
	};
	struct W09 {
		static const std::string LISTINFO;
	};
	struct W10 {
		static const std::string LISTINFO;
	};
};

struct MTR {
	struct W01 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W02 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W03 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W04 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W05 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W06 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W07 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W08 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W09 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W10 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W11 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W12 {
		static const std::string IMG;
		static const std::string TITLE;
	};
	struct W15 {
		static const std::string IMG;
		static const std::string TITLE;
	};
};

struct DFT {
	struct MGW {
		static const std::string TITLE;
		static const std::string FILENAME;
	};
};

struct PLG {
	struct KEY {
		static const std::string SpecPeakFilter;
		static const std::string SpecPeakPicking;
	};
};
