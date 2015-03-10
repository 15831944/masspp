/**********************************************************
Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
   @addtogroup C++ CommonFunctions
	@file       Standardinline.h
	@brief      使用頻度の高いフレーズをinlineとして纏める。
	@date       2012.11.16
	@author     M.Fukuda
	@version    1.0.0
    @note       Boost等、外部ライブラリに関わる処理は入れない。
**********************************************************/
#pragma once
#include <cmath>

namespace RFN {
namespace STD {

const double kDiffLimit = 1.0E-10;
const double kPPMRatio = 1.0E-6;

// Doubleの誤差(1.0E-10) を吸収して dl <= dr の判定
inline const bool eq_less(const double dl, const double dr) {
	return dl < (dr + 1.0E-10);
}

//// $$$ 使用箇所無し#2014.01.22
//// Doubleの誤差(1.0E-10) を吸収して dl == dr の判定
//inline const bool eq(const double& dl, const double& dr) {
//	return (std::fabs(dl-dr) < 1.0E-10);
//}

// l <= tgt <= R の T/F を判定
inline bool withinLimits(const double& tgt, const double& l, const double& r) {
	return eq_less(l, tgt) && eq_less(tgt, r);
}

//// PPM * Tol の計算をして返す。
//inline const double getPPMTol(const double& tgt, const double& ppmVal) {
//	return tgt * (ppmVal * kPPMRatio);
//}

//// $$$ 使用箇所無し#2014.01.22
///* @fn xxx */
//template<typename T_n>
//inline const int compVal(const T_n& l, const T_n& r, const T_n& tgt) {
//	if (tgt < l) return -1;
//	else if(tgt > r) return 1;
//	else return 0;
//}

// Refing.hppのみ使用。
// 問題が無ければ、Refing.hppに移動する。
inline const int compVal_d(const double& l, const double& r, const double& tgt) {
	if (tgt < l) return -1;
	else if(tgt > r) return 1;
	else return 0;
}

//// $$$ 使用箇所無し#2014.01.22
//// 対象の値が -1.0(初期値)なら 0 に置換
//inline const double fixZero(const double& tgt) {
//	return (tgt < 0.0) ? 0.0 : tgt;
//}

//// $$$ 使用箇所無し#2014.01.22
//// 対象の値が -1.0(初期値)なら DBL_MAX に置換
//inline const double fixMaxDbl(const double& tgt) {
//	return (tgt < 0.0) ? DBL_MAX : tgt;
//}

//// $$$ 使用箇所無し #2014.01.22
//// 対象の値が -1.0(初期値)なら -DBL_MAX に置換
//inline const double fixNegaMaxDbl(const double& tgt) {
//	return (tgt < 0.0) ? DBL_MAX * (-1.0) : tgt;
//}

//// $$$ 該当箇所非使用 #2014.01.22
//// $$$ 該当箇所 XYZPoints.hpp : searchZ_MinMax
//
//// dmin, dmax　間の整合性のチェックをする。
//// isFreeRng
//inline const bool checkRangeConsistency(
//	const double& dmin, const double& dmax, const bool minus_is_Infinity) {
//	if (!minus_is_Infinity) return (dmin < dmax);
//	// use minus is infinity
//	if (dmin > 0.0 && dmax > 0.0) return (dmin < dmax);
//	return true;
//}

}// end of namespace
}