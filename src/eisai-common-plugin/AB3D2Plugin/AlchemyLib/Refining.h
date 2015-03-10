/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Refining.h
 @brief select spectra in the range
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

#pragma once

#include "AlchmyLib.h"
#include "..\Thoto\Points\LRPoint_Def.hpp"

namespace rfn {
namespace rfng { 

enum FLAGS {
	J_ERROR = -2,
	J_LESS = -1,
	J_TRUE = 0,
	J_OVER = 1,
};

const std::pair<double,double> get_dataSetRTRange(kome::objects::DataSet& dataSet);

//const int searchSpecIndex_FromRT (
//	kome::objects::DataSet& dataSet, 
//	const double tgtrt,
//	unsigned int& rstIdx
//);

const bool refiningSpectra (
	kome::objects::DataSet& dataSet, // In
	const ELM::LRP& RTp,
	const ALC::CHROM_FLAG& ch_flag,
	unsigned int msstage,
	std::vector<kome::objects::Spectrum*>& refSpec
);

}
}
