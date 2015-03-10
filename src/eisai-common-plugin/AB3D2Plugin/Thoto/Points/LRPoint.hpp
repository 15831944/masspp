/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       LRPoint.hpp
	@brief      LRPoint の 構成
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include "LRPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const RFN::LR::LRP_ NG_LRP(-1.0,-1.0);
-- RFN::LR --
ELM  LRP        --- std::pair<double,double>
     MZRTP      --- struct{LRP mz, LRP rt}
/ -------------------------------------------------------*/

#include "../Standardinline.hpp"

namespace RFN {
namespace LR {

// L <= TGT <= R を判定
inline bool withinLimits(const double& tgt, const RFN::LR::LRP_& lrp) {
	return RFN::STD::withinLimits(tgt, lrp.first, lrp.second);
}

// MZRT の　MZ の　範囲内にあるか否か
inline bool withinMZLimits(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
	return RFN::LR::withinLimits(tgt, mzrt.mz);
}

// MZRT の　RT の　範囲内にあるか否か
inline bool withinRTLimits(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
	return RFN::LR::withinLimits(tgt, mzrt.rt);
}

//// L-R の範囲に対し、tgt　が　等しい : 0 小さい -1,　大きい :  1
//inline const int compLRP(const double& tgt, const RFN::LR::LRP_& lrp) {
//	return RFN::STD::compVal_d(lrp.first, lrp.second, tgt);
//}

//// MZRT の MZ 範囲に対し、tgt　が　等しい : 0 小さい -1,　大きい :  1
//inline const int compMZRT_MZ(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
//	return RFN::LR::compLRP(tgt, mzrt.mz);
//}

//// MZRT の RT 範囲に対し、tgt　が　等しい : 0 小さい -1,　大きい :  1
//inline const int compMZRT_RT(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
//	return RFN::LR::compLRP(tgt, mzrt.mz);
//}

//// tgt - tol, tgt + tol　を返す（ABS)
//inline ELM::LRP createLRP_withABSTol(const double& tgt, const double& tol) {
//	return ELM::LRP(tgt - tol, tgt + tol);
//}
//
//// tol = (tgt * PPM), tgt - tol, tgt + tol　を返す（PPM)
//// PPMをRatio化しないで渡す事 (10ppmなら10で渡す)
//inline ELM::LRP createLRP_withPPMTol(const double& tgt, const double& ppmtol) {
//	const double tol = RFN::STD::getPPMTol(tgt, ppmtol);
//	return ELM::LRP(tgt - tol, tgt + tol);
//}

//// LRP * 2 (MZ,RT) to MZRT
//inline void setMZRT(const ELM::LRP& mzp, const ELM::LRP& rtp, ELM::MZRTP& tgt) {
//	tgt.mz = mzp;
//	tgt.rt = rtp;
//}

//// マイナスの値が渡った場合は最大、最小値を代入して返す
//inline ELM::LRP createLRP_NagaToInfinity(const double& dlow, const double& dmax) {
//	double l = (dlow < 0.0) ? -1.0 : dlow;
//	double r = (dmax < 0.0) ? DBL_MAX : dmax;
//	return ELM::LRP(l, r);
//}

} // end of namespace LR
} // end of namespace RFN
