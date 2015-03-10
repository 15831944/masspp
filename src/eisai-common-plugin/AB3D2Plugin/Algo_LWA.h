/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Algo_LWA.h
 @brief LWA peak detection(use only AB3D)
 
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
#include "stdafx.h"
#include "Thoto/Points/LRPoint_Def.hpp"
#include "Thoto/Peaks/PeakBase_Def.hpp"

namespace EMS {
namespace LWA {

void onLWALabeling (
	ELM::vXYP& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const ELM::LRP& fwhm_width,
	RFN::PEAK::vPKPT_& rstPeaks
);


//void LWALabeling2 (
//	kome::core::XYData& xyData,
//	kome::objects::Peaks& peaks
//);

//// LWA
//void LWALabeling(P2DA_ptr ptr, SetMapper* setmap, EMS::BO::PeakThrs* pkthr, vPKP& rstPeaks);

} // end of namespace
}
