/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Functions
    @file       PeakFusion.hpp
	@brief      PKPT <-> Peak3D の変換関数群
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once
#include "../Peaks/PeakBase_Def.hpp"
#include "../Peaks/Peak3DBase_Def.hpp"

//#include "stdafx.h"

namespace RFN {
namespace FSN {

// PKPT -> PEAK3D への変換
inline ELM::PK3D_ptr convertPeak3DFromPKPT (
	const double& rt,
	const RFN::PEAK::PKPT_& src
) {
	ELM::PK3D_ptr k = new ELM::Peak3D(
		src.Cp.second,
		src.Lp.first,
	//	src.Cp.first,//LWAのX位置のピークシフトが障害になるのでAPEXを採用。
		src.Apex.first,
		src.Rp.first,
		rt
	);
	return k;
}

} // end of namespace FSN
}