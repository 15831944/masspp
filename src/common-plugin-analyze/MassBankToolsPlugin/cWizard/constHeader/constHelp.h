/**
 * @file constHelp.h
 * @brief Wizard help message declaration
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

struct HLP {
	struct WIZ {
		// Trum01
		static const std::string SAMPLELIST;
		static const std::string SPECLIST;
		static const std::string AVERAGELIST;
		static const std::string PROCESSINGLIST;

		// Trum02
		static const std::string PREFIX;
		static const std::string BASENUM;
		static const std::string OUT_FOLDER;

		// Trum03
		static const std::string PEAKDETECT;
		static const std::string PEAKFILTER;

		// Trum04
		static const std::string TITLE;
		static const std::string DATE;
		static const std::string AUTHOR;
		static const std::string LICENSE;
		static const std::string COPYRIGHT;
		static const std::string PUBLICATION;
		static const std::string COMMENT;

		// Trum05
		static const std::string N_PRODUCT;
		static const std::string FORMULA;
		static const std::string EXACTMASS;
		static const std::string SMILES;
		static const std::string INCHIKEY;
		static const std::string CLASSNAME;
		static const std::string CH_NAME;
		static const std::string EX_LINK;
		
		static const std::string SEARCH_BUTTON;
		static const std::string MOL_BUTTON;
		static const std::string EXDB_BUTTON;

		// Trum06
		static const std::string SCIENTIFICNAME;
		static const std::string DATABASE;
		static const std::string ACCESSION;
		static const std::string SAMPLES;

		// Trum07
		static const std::string INSTTYPE;
		static const std::string VENDOR;
		static const std::string INSTRUMENTS;

		// Trum08
		static const std::string MSSTAGE;
		static const std::string POLARITY;
		static const std::string SPECADDINFO;

		struct MS {
			static const std::string AUTO;
			static const std::string NONE;
			static const std::string BASE;
		};
		struct POL {
			static const std::string AUTO;
			static const std::string NONE;
			static const std::string POSI;
			static const std::string NEGA;
		};

		// Trum 09
		static const std::string CHROMADDINFO;

		// Trum 10
		static const std::string PREC_TYPE;
		static const std::string PREC_MOLINFO;

		// Trum 11
		static const std::string PROCESSING;
		static const std::string SOFTWARE;
		static const std::string DETECTOR;

		// Trum 12
		static const std::string ANNOTATION;

		// Trum other
		static const std::string PROP_LIST;
	};
};
