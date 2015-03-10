/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYZPoint.hpp
	@brief      <X,Y> * N ,Z の実装部
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
	@note
		要素がポインタで格納されています。
		デストラクタにポインタ破棄を
		オブジェクトのコピーの際は注意して下さい。
****************************************************************/
#pragma once

#include "XYZPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const XYP_ NG_XYP(-1.0,-1.0);

-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*

-- RFN::XYZP --
ELM  XYZP      --- struct {double z, SPTR::vXYZ_sp}
     vXYZP     --- std::vector<XYZPt>
ITR  vXYZP_Itr --- vXYZP::iterator
/ -------------------------------------------------------*/

#include <algorithm>

#include "../Standardinline.hpp"

namespace RFN {
namespace XYZP {

// ---------------------------------------------------------
// 関数オブジェクト
// --------------------------------------------------------
struct lessZ : public std::binary_function<const XYZP_&, const XYZP_&, bool> {
	bool operator()(const RFN::XYZP::XYZP_& lxyz, const RFN::XYZP::XYZP_& rxyz) const {
		return lxyz.z < rxyz.z;
	}
};

struct compZ { 
	bool operator()(const RFN::XYZP::XYZP_& lxyz, double rx)  { return lxyz.z < rx; }
	bool operator()(double lx, const RFN::XYZP::XYZP_& rxyz) { return lx < rxyz.z; }
	bool operator()(const RFN::XYZP::XYZP_& lxyz, const RFN::XYZP::XYZP_& rxyz) { return lxyz.z < rxyz.z; }
};

// ---------------------------------------------------------
// 実装部
// ---------------------------------------------------------

// ポイントを作成する
inline void addZPoint(const double& z, const RFN::XYP::vXYP_&src, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z, src));
}

// ポイントを作成して、そのポインタを貰う
inline SPTR::vXYP_sp createPoint_Sp(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z));
	vxyzp.back().vxyp = new ELM::vXYP();
	return vxyzp.back().vxyp;
}

// ポイントを作成して、そのオブジェクトを貰う
inline RFN::XYP::vXYP_& createPoint(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z));
	return *vxyzp.back().vxyp;
}

// Zでソートする
inline void sortZ(RFN::XYZP::vXYZP_& vxyzp, const bool desc = false) {
	if (!desc) {
		std::sort(vxyzp.begin(), vxyzp.end(), lessZ());
	} else {
		std::sort(vxyzp.begin(), vxyzp.end(), std::not2(lessZ()));
	}
}

// 近傍(<=)の直近値を探す。
inline RFN::XYZP::vXYZP_::iterator nearSearchZ(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	return std::lower_bound(vxyzp.begin(), vxyzp.end(), z, compZ());
}

inline RFN::XYZP::vXYZP_::iterator nearSearchZ(const double& z, const RFN::XYZP::vXYZP_::iterator& fst, RFN::XYZP::vXYZP_& vxyzp) {
	return std::lower_bound(fst, vxyzp.end(), z, compZ());
}

// vxyzp から minZ, maxZの範囲を持つイテレータを探す。
inline const std::pair<ITR::vXYZP_Itr, ITR::vXYZP_Itr>
	searchZ_MinMax(
		const double& minZ, const double& maxZ, ELM::vXYZP& vxyzp
){
	// デフォルトを設定する
	ITR::vXYZP_Itr lowit = vxyzp.end();
	ITR::vXYZP_Itr highit = vxyzp.end();

	if (!vxyzp.empty() && vxyzp[0].z <= maxZ) {	
		size_t num = vxyzp.size();
		const double arrlow(vxyzp[0].z), arrhigh(vxyzp[num-1].z);
		if (minZ < arrlow) { // 配列の最小値であれば先頭を返す。
			lowit = vxyzp.begin();
		} else if (RFN::STD::eq_less(minZ, arrhigh)) {
			lowit = nearSearchZ(minZ, vxyzp);
		}
		if (maxZ < 0.0 || arrhigh < maxZ) { // 無限指定、配列より大きい場合は末端を返す
			highit = vxyzp.end();
		} else if (RFN::STD::eq_less(arrlow, maxZ)) {
			highit = nearSearchZ(maxZ, lowit, vxyzp);
		}
	}
	return std::make_pair(lowit, highit);
}

// vxyzp から minZ, maxZの範囲を持つイテレータを探す。
inline void
	searchZ_MinMaxF(
		const double& minZ, const double& maxZ, ELM::vXYZP& vxyzp, ITR::vXYZP_Itr& lowit, ITR::vXYZP_Itr& highit
){
	// デフォルトを設定する
	lowit = vxyzp.end();
	highit = vxyzp.end();
	size_t num = vxyzp.size();
	const double arrlow(vxyzp[0].z), arrhigh(vxyzp[num-1].z);

	if (minZ < arrlow) { // 配列の最小値であれば先頭を返す。
		lowit = vxyzp.begin();
	} else if (RFN::STD::eq_less(minZ, arrhigh)) {
		lowit = nearSearchZ(minZ, vxyzp);
	}
	if (maxZ < 0.0 || arrhigh < maxZ) { // 無限指定、配列より大きい場合は末端を返す
		highit = vxyzp.end();
	} else if (RFN::STD::eq_less(arrlow, maxZ)) {
		highit = nearSearchZ(maxZ, lowit, vxyzp);
	}
}

} // end of namespace XYZP
}
