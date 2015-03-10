/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYPoint.hpp
	@brief      X,Y 関数の実装部
	@date       2012.12.12
	@author     M.Fukuda@Eisai
	@version    1.2.0
	@note
****************************************************************/
#pragma once

#include "XYPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const XYP_ NG_XYP(-1.0,-1.0);

-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
/ -------------------------------------------------------*/
#include <algorithm>
#include <tuple>     // for std::tie

#include "../Standardinline.hpp"

namespace RFN {
namespace XYP {

// ---------------------------------------------------------
// 関数オブジェクト
// --------------------------------------------------------

//// $$$ 使用箇所無し#2014.01.22
//// Y方向のソート関数オブジェクト
//struct lessY : public std::binary_function<const RFN::XYP::XYP_&, const RFN::XYP::XYP_&, bool> {
//	bool operator()(const RFN::XYP::XYP_& lxyp, const RFN::XYP::XYP_& rxyp) const {
//		return lxyp.second < rxyp.second;
//	}
//};

// binary sort 用のX方向関数オブジェクト
struct compX { 
	bool operator()(const RFN::XYP::XYP_& lxy, double rx)  { return lxy.first < rx; }
	bool operator()(double lx, const RFN::XYP::XYP_& rxy) { return lx < rxy.first; }
	bool operator()(const RFN::XYP::XYP_& lxy, const RFN::XYP::XYP_& rxy) { return lxy.first < rxy.first; }
};

//// $$$ 使用箇所無し#2014.01.22
//// Zero以下のポイントを探す
struct underZeroY : public std::unary_function<const RFN::XYP::XYP_&, bool> {
	bool operator()(const RFN::XYP::XYP_& tgt) const {
		return tgt.second < 0.0;
	}
};

// ---------------------------------------------------------
// 実装部
// ---------------------------------------------------------

//// $$$ 使用箇所無し#2014.01.22
//// vXYP の余剰(Capacity部分)を削る
//inline void shrinkToFit(RFN::XYP::vXYP_& vxyp) {
//	ELM::vXYP(vxyp).swap(vxyp);
//}

//// $$$ 使用箇所無し#2014.01.22
//// vXYP を完全にクリアする
//inline void fullClear(RFN::XYP::vXYP_& vxyp) {
//	vxyp.clear();
//	shrinkToFit(vxyp);
//}

// vxyp に　ポイントを追加する
inline void addXYPoint(const double& x, const double& y, RFN::XYP::vXYP_& vxyp) {
	vxyp.push_back(std::make_pair(x, y));
}

//// $$$ 使用箇所無し#2014.01.22
//// 最大Intensityを持つ XYP　位置のイテレータを返す
//inline RFN::XYP::vXYP_::iterator getXYP_MaxIntensity(RFN::XYP::vXYP_& vxyp){
//	return std::max_element(vxyp.begin(), vxyp.end(), RFN::XYP::lessY());
//}

// vxypの中から近似値 x <= result.x である値を探す。無い場合は vxyp
inline const RFN::XYP::vXYP_::iterator nearSearchX(
	const double& x, RFN::XYP::vXYP_& vxyp
) {
	return std::lower_bound(vxyp.begin(), vxyp.end(), x, RFN::XYP::compX());
}

// イテレータの探す先を固定して（本当は計算して確定しておくのが望ましい）
inline const RFN::XYP::vXYP_::iterator nearSearchX(const double& x, const ITR::vXYP_Itr& bit, RFN::XYP::vXYP_& vxyp) {
	return std::lower_bound(bit, vxyp.end(), x, RFN::XYP::compX());
}

//// $$$ 使用箇所無し#2014.01.22
//inline const bool onInDistance(
//	const ITR::vXYP_Itr& bgnit,
//	const ITR::vXYP_Itr& endit,
//	const unsigned int lenRange
//){
//	if(lenRange == 0) return false;
//	return (std::distance(bgnit,endit) > static_cast<int>(lenRange));
//}

//// $$$ 使用箇所無し#2014.01.22
//// イテレータの探す先を固定して（本当は計算して確定しておくのが望ましい）
//inline const RFN::XYP::vXYP_::iterator nearSearchX_withABSRange(
//	const double& x, 
//	const ITR::vXYP_Itr& bit,
//	const unsigned int lenRange,
//	RFN::XYP::vXYP_& vxyp
//) {
//	if (onInDistance(bit, vxyp.end(), lenRange)) {
//		return std::lower_bound(bit, bit + lenRange, x, RFN::XYP::compX());
//	}
//	return std::lower_bound(bit, vxyp.end(), x, RFN::XYP::compX());
//}

//// $$$ 使用箇所無し#2014.01.22
//// 最大のIntensityを返す
//inline double getMaxIntensity(RFN::XYP::vXYP_& vxyp) {
//	return (*RFN::XYP::getXYP_MaxIntensity(vxyp)).second;
//}

// X, Y の要素の乗算を返す (product)
inline double productXY(const RFN::XYP::XYP_& src) {
	return src.first * src.second;
}

// Sort X 
inline void sortX(RFN::XYP::vXYP_& vxyp, const bool desc) {
	if (!desc) {
		std::sort(vxyp.begin(), vxyp.end());
	} else { // 降順なのでネゲータで反転
		std::sort(vxyp.begin(), vxyp.end(), std::greater<RFN::XYP::XYP_>());
	}
}

//// $$$ 使用箇所無し#2014.01.22
//// Sort Y
//inline void sortY(RFN::XYP::vXYP_& vxyp, const bool desc) {
//	if (!desc) {
//		std::sort(vxyp.begin(), vxyp.end(), RFN::XYP::lessY());
//	} else { // 降順なのでネゲータで反転
//		std::sort(vxyp.begin(), vxyp.end(), std::not2(RFN::XYP::lessY()));
//	}
//}

// vxyp から minX, maxXの範囲を持つイテレータを探す。
// [Error] : litr = vxyp.end()
// Ex) minX < vxyp[0].mz であれば、vxyp.begin()を返す。 
inline const std::pair<ITR::vXYP_Itr, ITR::vXYP_Itr>
	searchX_MinMax(
	const double& minX, 
	const double& maxX, 
	RFN::XYP::vXYP_& vxyp
) {
	// デフォルトを設定する
	ITR::vXYP_Itr lowit = vxyp.end(); // 例外の為、lowitをEndに固定。
	ITR::vXYP_Itr highit = vxyp.end();

	if (! vxyp.empty() && vxyp[0].first <= maxX) {
		// minX, maxXの整合性を判定する
		size_t num = vxyp.size();
		const double arrlow(vxyp[0].first), arrhigh(vxyp[num-1].first);

		if (minX < arrlow) { // 配列の最小値であれば先頭を返す。
			lowit = vxyp.begin();
		} else if (RFN::STD::eq_less(minX, arrhigh)) {
			lowit = nearSearchX(minX, vxyp);
		}
		if (maxX < 0.0 || arrhigh < maxX) { // 無限指定、配列より大きい場合は末端を返す
			highit = vxyp.end();
		} else if (RFN::STD::eq_less(arrlow, maxX)) {
			highit = nearSearchX(maxX, lowit, vxyp);
		}
	}
	return std::make_pair(lowit, highit);
}

//// $$$ 使用箇所無し#2014.01.22
//// 速度テスト中。make_pairは・・
//inline const bool
//	searchX_MinMaxF(const double& minX, const double& maxX, RFN::XYP::vXYP_& vxyp, ITR::vXYP_Itr& lowit, ITR::vXYP_Itr& highit) {
//	// デフォルトを設定する
//	lowit = vxyp.end(); // 例外の為、lowitをEndに固定。
//	highit = vxyp.end();
//
//	size_t num = vxyp.size();
//	const double arrlow(vxyp[0].first), arrhigh(vxyp[num-1].first);
//
//	if (minX < arrlow) { // 配列の最小値であれば先頭を返す。
//		lowit = vxyp.begin();
//	} else if (RFN::STD::eq_less(minX, arrhigh)) {
//		lowit = nearSearchX(minX, vxyp);
//	}
//	if (maxX < 0.0 || arrhigh < maxX) { // 無限指定、配列より大きい場合は末端を返す
//		highit = vxyp.end();
//	} else if (RFN::STD::eq_less(arrlow, maxX)) {
//		highit = nearSearchX_withABSRange(maxX, lowit, 63, vxyp);
//	}
//	return lowit != vxyp.end();
//}


// src 内の　lx - rx の　データを dst にコピーする
// [in]  lx  : 最小値 (minMz)
// [in]  rx  : 最大値 (maxMz)
// [in]  src : 元となる vxyp
// [out] dst : 結果の出力
// 値が無いなら「空」で帰る
inline bool getInRangeElements(
	const double& lx, const double& rx, 
	RFN::XYP::vXYP_& src, RFN::XYP::vXYP_& dst
){
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);

	dst.clear();
	// error. Break.
	if (lit == src.end()) { return false; }

	dst.reserve(std::distance(lit, rit));
	while(lit != rit) {
		dst.push_back(*lit);
		++lit;
	}
	return true;
}

// src　の　lx-rx　の　total Intensity (TIC) を返す
// [in]  lx  : 最小値 (minMz)
// [in]  rx  : 最大値 (maxMz)
// [in]  src : 元となる vxyp
// [rst] TIC or 0 (無し、エラー等は全部 0 )
inline const double getInRangeTotalIntensity(
	const double lx, const double rx, RFN::XYP::vXYP_& src
){
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//	searchX_MinMaxF(lx, rx, src, lit, rit);
	//ITR::vXYP_Itr lit = nearSearchX(lx, src);
	//ITR::vXYP_Itr rit = nearSearchX(rx, src);

	register double rstVal(0.0);
	if (lit != src.end()) {
		while(lit != rit) {
			rstVal += lit->second;
			++lit;
		}
	}
	return rstVal;
}

//// $$$ 使用箇所無し#2014.01.22
inline const double getInRangeTotalIntensity(
	const double lx, const double rx, RFN::XYP::vXYP_& src, double rt, std::vector<std::tuple<double, double, double>>& vPnt
) {
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//	searchX_MinMaxF(lx, rx, src, lit, rit);
	//ITR::vXYP_Itr lit = nearSearchX(lx, src);
	//ITR::vXYP_Itr rit = nearSearchX(rx, src);

	register double rstVal(0.0);
	if (lit != src.end()) {
		while(lit != rit) {
			rstVal += lit->second;
			vPnt.push_back(std::tuple<double, double, double>(rt, lit->first, lit->second));
			++lit;
		}
	}
	return rstVal;
}

//// $$$ 使用箇所無し#2014.01.22
//// src　の　lx-rx　の　Max Intensity を返す
//// [in]  lx  : 最小値 (minMz)
//// [in]  rx  : 最大値 (maxMz)
//// [in]  src : 元となる vxyp
//// [rst] Max Intensity or 0 (無し、エラー等は全部 0 )
//inline const double getInRangeMaxIntensity(
//	const double& lx, const double& rx, RFN::XYP::vXYP_& src
//){
//	ITR::vXYP_Itr lit, rit;
//	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//
//	double rstVal(0.0);
//	if (lit != src.end()) {
//		while(lit != rit) {
//			rstVal = (RFN::STD::eq_less(rstVal, lit->second)) ? lit->second : rstVal;
//			++lit;
//		}
//	}
//	return rstVal;
//}

//// $$$ 使用箇所無し#2014.01.22
//// srcの指定区間のY(Intensity)を全て０にする。
//inline void clearY(
//	const double& lx, const double& rx, RFN::XYP::vXYP_& src
//) {
//	ITR::vXYP_Itr lit, rit;
//	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//
//	if (lit == src.end()) {	
//		return;
//	}
//	for (; lit != rit; ++lit) {
//		lit->second = 0.0;
//	}
//}

// srcの指定区間のY(Intensity)を全て0にする。
// 最後の引数のリストに0にする前のIntensityを格納する。
inline void clearY(
	const double& lx, const double& rx, RFN::XYP::vXYP_& src, std::vector<double>& intensities) {
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);

	if (lit == src.end()) {	
		return;
	}
	for (; lit != rit; ++lit) {
		if (lit->second > 0) {
			intensities.push_back(lit->second);
			lit->second = 0.0;
		}
	}
}


//// $$$ 使用箇所無し#2014.01.22
// 以下、末尾まで未使用
//---------------------------------------------------------
// 不要空間を削る関数群。オブジェクトが多過ぎて
// メモリリークを起こしそうな時の対策群。
// ０が連続する空間は確保する価値がなければデータから削る等。
//---------------------------------------------------------

//// underPoint 以下の連続区間を和を負で、それ以外の和を正で返す。
//// EX) 0,0,4,5,2,0,0,1,0,9,9,9,.... => -2,3,-2,1,-1,3,...
inline const bool tranceZeroPoints (
	RFN::XYP::vXYP_& src,
	std::vector<int>& fgs
) {
	fgs.resize(src.size());
	std::vector<int>::iterator it = fgs.begin();
	bool gg, fg = false;
	ITR::vXYP_Itr itr = src.begin(), edItr = src.end();
	while(itr != edItr) {
		gg = (itr->second > 0.0);
		// 直前の値と正負が異なる場合
		if (fg != gg) {
			fg = !fg;
			++it;
			*(it) = 0;
		}
		(*it) += (fg) ? 1 : -1;
		++itr;
	}
	// 不要な区間を削除
	fgs.erase(std::remove(fgs.begin(),fgs.end(),0),fgs.end());
	return true;
}
//
// コード中で未使用。
//// src中から N点しか構成されないポイントを0にする。
//// [in] lessnum : この数値以下のポイントを対象にする。
//// [in] zeroPts : 判定区間の前後に "0"　がいくつある前提か？
//// ex ) less : 1 / zeroPts 2 なら
//// * * ! * * * * * * * * ! * * (! が消す点)
//// 0 0 3 0 0 2 4 0 9 0 0 9 0 0
//inline const int toZeroLonlyOnlyPoint (
//	RFN::XYP::vXYP_& src, 
//	const unsigned int lessnum, 
//	const unsigned int zeroPts
//){
//	std::vector<int> fgs;
//	tranceZeroPoints(src, fgs);
//	return 0;
//}
//
// underPoint以下の区間が,「有効」な点からみて
// keepZeroNum 以上続く場合は削る。
// ピークの構成の関係上、最低限、１点は保証する。
// EX) underPoint = 0, keepZeroNum=2
// ####00000000### なら -> (####00)0000(00###) ()の区間を残して
// ####0000### の様にする。
inline const bool deleteSereisOfZeroPoints(
	RFN::XYP::vXYP_&src,
	const unsigned int keepZeroNum
) {
	std::vector<int> fgs;
	unsigned int msCnt(0), deleteNum(0);
	unsigned int zeroNum = (keepZeroNum < 1) ?  1 : keepZeroNum;
	int limitVal = zeroNum * -2;
	// ０の連続区間を取得する
	tranceZeroPoints(src, fgs);
	std::vector<int>::iterator it = fgs.begin() , eit = fgs.end();

	while(it != eit) {
		// 削る区間が見つかった
		if (*it < limitVal) {
			deleteNum = limitVal - *it;
			msCnt += zeroNum;
			for (unsigned int i = 0; i < deleteNum; i++) {
				src[msCnt].second = -1.0;
				msCnt++;
			}
			msCnt += zeroNum;
		} else {
			msCnt += std::abs(*it);
		}
		++it;
	}
	src.erase(std::remove_if(src.begin(),src.end(), underZeroY()), src.end());

	return true;
}

}// end of namespace
}

