/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Algo_LWA.cpp
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

#include "Algo_LWA.h"
#include <tuple>
#include <boost/foreach.hpp>
#include "Thoto\Peaks\PeakBase.hpp"
#include "Thoto\Points\XYPoint.hpp"

//#include "Thoto/Mspp/ConvertXYDatas.hpp"
//#include "Thoto/Mspp/ConvertPeakData.hpp"

namespace EMS {
namespace LWA {

/*
 * Like Private Functions
 * 半値(thrval)以下の初出点を探す(tgtIt)
 * その後、それ以降テーリングする下界値(acutualIt)を探す
 * @breif search FWHM Point
 * @param[in]  coreIt  : start Point iterator
 * @param[in]  endIt   : end of vXYP_ iterator(v.end())
 * @param[in]  thrval  : FWHM High Point (FWHMの高さ)
 * @param[in]  toRight : T search Right / F search Left
 * @param[out] tgtIt   : lessFWHM first Point
 * @param[out] acutualIt : 下まで下がり切ったポイント
 */
const bool getBorderPoints(
	const RFN::XYP::vXYP_::iterator& coreIt,
	const RFN::XYP::vXYP_::iterator& endIt,
	const double thrval,
	const bool toRight,
	RFN::XYP::vXYP_::iterator& ref_tgtIt,
	RFN::XYP::vXYP_::iterator& ref_actualIt);


void LWA_FindBasePeaks (
	RFN::XYP::vXYP_& vxyp,
	const double fwhmRatio,
	const double mergeDistance,
	RFN::PEAK::vPKPT_& vPeaks
);

void LWA_Merge (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const ELM::LRP& fwhm_width,
	RFN::PEAK::vPKPT_& srcPeaks,
	RFN::PEAK::vPKPT_& rstPeaks
);

//---------------------------------------------------------

//ArithmeticGeometricMean
class AriGeoMean {
private:
	const double& intThr_;
	RFN::XYP::XYP_& xyp_;
public : 
	AriGeoMean (const double& intthr, RFN::XYP::XYP_& xyp) : intThr_(intthr), xyp_(xyp) {
		xyp_.first = 0.0;
		xyp_.second = 0.0;
	}
	void operator()(const RFN::XYP::XYP_& src) {
		xyp_.first += src.first * (src.second - intThr_);
		xyp_.second += (src.second - intThr_);
	}
};

//LWALabeling

void onLWALabeling (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const RFN::LR::LRP_& fwhm_width,
	RFN::PEAK::vPKPT_& rstPeaks
) {
	RFN::PEAK::vPKPT_ vPeaks;
	LWA_FindBasePeaks(vxyp, fwhm_ratio, merge_distance, vPeaks);
	LWA_Merge(vxyp, merge_distance, fwhm_ratio, fwhm_width, vPeaks, rstPeaks);
}

void LWA_FindBasePeaks (
	RFN::XYP::vXYP_& vxyp,
	const double fwhmRatio,
	const double mergeDistance,
	RFN::PEAK::vPKPT_& vPeaks
) {

	typedef RFN::XYP::XYP_ tXYP;
	typedef RFN::XYP::vXYP_::iterator tvXYP_It;

	if (vxyp.empty()) { return;}
	RFN::XYP::sortX(vxyp, false);

	// for work while Itr
	tvXYP_It leftIt, rightIt;
	tvXYP_It leftEndIt, rightEndIt;
	const tvXYP_It beginIt = vxyp.begin();
	const tvXYP_It endIt = vxyp.end();

	// 基準点
	tXYP refP, centroidP;
	double intThr(0.0);

	// ベースのイテレータ
	tvXYP_It baseIt = vxyp.begin();
	++baseIt;

	while(baseIt != endIt) {
		// 起点の情報、閾値の設定
		if (baseIt->second <= 0.0) {
			++baseIt;
			continue;
		}

		refP = *baseIt;
		intThr = refP.second * fwhmRatio;

		// search Borders
		if (getBorderPoints(baseIt, endIt,   intThr, true,  rightIt, rightEndIt) &&
			getBorderPoints(baseIt, beginIt, intThr, false, leftIt,  leftEndIt)) {

			// Left,Rightの両端はFWHM以下の初出点なので
			// の計算には採用しない
//			AriGeoMean agm(intThr);
			std::for_each(leftIt + 1, rightIt, AriGeoMean(intThr, centroidP));
			centroidP.first /= centroidP.second;
			centroidP.second = refP.second;
			vPeaks.push_back(RFN::PEAK::PKPT_(centroidP, *leftEndIt, *rightEndIt, refP));

			// get FWHM
			// 
			while(leftIt->second < intThr) {++leftIt;}
			while(rightIt->second < intThr) {--rightIt;}
			// 半値幅は、intThrとそれをまたぐ位置を線形補完で求める。
			vPeaks.back().fwhmVal =
				(leftIt->first - (leftIt-1)->first) * (leftIt->second - intThr) / (leftIt->second - (leftIt-1)->second) +
				(rightIt->first - leftIt->first) +
				((rightIt+1)->first - rightIt->first) * (rightIt->second - intThr) / (rightIt->second - (rightIt+1)->second);
		}
		baseIt = rightEndIt;
	}
	if (vPeaks.empty()) { return; } // no peaks;
}

// back wsAve, totalInt
//std::pair<double,double> setBaseVal(const PKP& src) {
//	return std::make_pair(productCoreXY(src), src.coreP.second);
//}

// get CurrentPeak, Total Base Intensity, wait shift average.
std::tuple<RFN::PEAK::PKPT_, double, double> setBaseVal(const RFN::PEAK::PKPT_& src) {
	return std::make_tuple(src, src.Cp.second, RFN::PEAK::productCoreXY(src));
}


void LWA_Merge (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const RFN::LR::LRP_& fwhm_width,
	RFN::PEAK::vPKPT_& srcPeaks,
	RFN::PEAK::vPKPT_& rstPeaks
) {
	const bool dorevisezero(false);
	if (srcPeaks.empty()) {
		return;
	}
	RFN::PEAK::vPKPT_::iterator peakIt = srcPeaks.begin();
	RFN::PEAK::vPKPT_::iterator peakItEnd = srcPeaks.end();

	rstPeaks.reserve(srcPeaks.size());
	RFN::PEAK::PKPT_ curPk;
	double totalbaseInt, wsAve;

	//PKP curPk = (*peakIt);
	//double totalbaseInt = curPk.coreP.second;
	//double wsAve = productCoreXY(curPk);
	std::tie(curPk, totalbaseInt, wsAve) = setBaseVal(*peakIt);

	for (; peakIt != peakItEnd; ++peakIt) {
		// マージする閾値以内かを判定
		if (distanceCoreX(*peakIt, curPk) > merge_distance) {

			curPk.Cp.first = wsAve / totalbaseInt;
			curPk.Cp.second = curPk.Apex.second;

			// 閾値を纏めて判定 FWHMを使わないので全部 True
			//if (withinThrsholds(vxyp, curPk, fwhm_ratio, fwhm_width, 0)) {
			//	rstPeaks.push_back(curPk);
			//}
			rstPeaks.push_back(curPk);
			std::tie(curPk, totalbaseInt, wsAve) = setBaseVal(*peakIt);
		} else {
			// 加重平均で計算
			totalbaseInt += (*peakIt).Cp.second;
			wsAve += productCoreXY(*peakIt);
			// 右端を更新
			curPk.Rp = (*peakIt).Rp;
			// 頂点情報を最大値に変更
			if ((*peakIt).Apex.second > curPk.Apex.second) {
				curPk.Apex = (*peakIt).Apex;
			}
		}
	}
	// Add End Point;
	rstPeaks.push_back(curPk);


	// ピーク端を０値補正する場合
	//if (dorevisezero) {
	//	BOOST_FOREACH(PKP& tmp, rstPeaks) {
	//		tmp.leftP.second = 0.0;
	//		tmp.rightP.second = 0.0;
	//	}
	//}
}

//---------------------------------------------------------
// Inner Only
//---------------------------------------------------------

const bool getBorderPoints(
	const RFN::XYP::vXYP_::iterator& coreIt,
	const RFN::XYP::vXYP_::iterator& endIt,
	const double intensityThrshold,
	const bool toRight,
	RFN::XYP::vXYP_::iterator& ref_tgtIt,
	RFN::XYP::vXYP_::iterator& ref_actualIt
){
	// checkPoints
	bool foundPointFg(false);
	bool loopEndFg(false);
	const double refInt = coreIt->second;
	ref_tgtIt = coreIt;

	// found under FWHM's Y Point
	while (!loopEndFg && !foundPointFg) {
		(toRight) ? ++ref_tgtIt : --ref_tgtIt;
		if (ref_tgtIt == endIt) {
			loopEndFg = true;
		} else {
			if (refInt < ref_tgtIt->second) { //ref Point's Y Overed.
				loopEndFg = true;
			} else if (intensityThrshold > ref_tgtIt->second) { //found Points
				foundPointFg = true;
			}
		}
	}
	
	ref_actualIt = ref_tgtIt;
	if (loopEndFg) {return false;}

	if (foundPointFg) {
		double prevVal = ref_actualIt->second;
		loopEndFg = false;
		while (!loopEndFg) {
			(toRight) ? ++ref_actualIt : --ref_actualIt;
			if (ref_actualIt == endIt) {
				loopEndFg = true;
			} else if (ref_actualIt->second < prevVal) {
				prevVal = ref_actualIt->second;
			} else {
				loopEndFg = true;
			}
		}
		// ref_actualIt is overstepped. one step undo.
		(!toRight) ? ++ref_actualIt : --ref_actualIt;
	}
	
	return true;
}

} // end of namespace
}


//inline size_t PointDistance(P2DA_ptr ptr, const PKP& p) {
//	return distance(ptr->searchX(p.leftP.x), ptr->searchX(p.rightP.x));
//}
//
