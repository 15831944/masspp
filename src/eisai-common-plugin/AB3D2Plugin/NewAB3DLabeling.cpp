/*********************************************************/
/*!
 @addtoGroup AB3D
 @file NewAB3DLabeling.cpp
 @brief AB3DLabeling Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#include "StdAfx.h"
#include "NewAB3DLabeling.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/timer.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/algorithm/string/split.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Algo_LWA.h"

// memo 2013.08.01
// DB関連をコントロールします。
// PeakPosition(Matrix化)の為のデータを確保しています。
#include "DBTools\DbController.h"

// memo 2013.08.01
// Mass++のプログレスをラップするクラスです。
// 呼び出し手続きが煩雑なので作ってみました。
#include "progress\progressAB3D.h"

// 旧 Trading poistのプロジェクトとして外にあったものを
// クラス化して このプロジェクトの中に入れてあります。
#include "./AlchemyLib/TragingPost.h"

#include "Thoto/Peaks/Peak3DBase.hpp"
//#include "Thoto/Points/XYPoint.hpp"
//#include "Thoto/Points/XYZPoint.hpp"

const double kPPM(0.000001);

// param(Range)の分割関数。
const ELM::LRP splitSettingVal(const std::string& tgtStr){
	ELM::LRP rst;
	if (tgtStr.empty()) return rst;

	std::vector<std::string> tmp;
	boost::algorithm::split(tmp, tgtStr, std::bind2nd(std::equal_to<char>(), ':'));
	if (tmp.empty()) {
		return rst;
	}
	rst.first = (tmp[0].size() > 0) ? boost::lexical_cast<double>(tmp[0]) : -1.0;
	if (tmp.size() > 1) {
		rst.second = (tmp[1].size() > 0) ? boost::lexical_cast<double>(tmp[1]) : -1.0;
	}
	return rst;
}

bool getDataPointsByRT(ELM::vXYZP& vxyzp, double rt, RFN::XYP::vXYP_& vxyp) {
	for (ELM::vXYZP::iterator it = vxyzp.begin(); it != vxyzp.end(); ++it) {
		if (it->z == rt) {
			vxyp = *it->vxyp;
			return true;
		}
	}
	return false;
}

vPeak3D::iterator NewAB3DLabeling::searchMZRange(
	vPeak3D::iterator it, 
	vPeak3D::iterator& endIt, 
	const double& rtTol, 
	const bool& direction, 
	ELM::LCRP& mzRange
) {
	const double currRt = (*it)->rt.c;
	vPeak3D::iterator retIt = it;
	while (it != endIt) {
		if (direction) {
			++it;
		} else {
			--it;
		}

		BS::Peak3D* pk3d = (*it);
		if (abs(pk3d->rt.c - currRt) > rtTol) {
			break;
		}

		// m/zの範囲を取得する。
		mzRange.l = std::min(mzRange.l, pk3d->mz.c);
		mzRange.r = std::max(mzRange.r, pk3d->mz.c);
		retIt = it;
	}
	return retIt;
}


bool NewAB3DLabeling::detectMZRangeByPeakPositions(
		Ptr& peak, vPeak3D& peaks,
		const double& rtTol, const double& intensityThreshold,
		ELM::LCRP& mzRange) {

	// Intensityの条件を満たす範囲内のピークを取得して、RTでソートする。
	vPeak3D peaksSortedByRT;
	Cnum::getPeaksAboveIntensity(
		peaks, intensityThreshold, peaksSortedByRT);
	Cnum::sortByRTcenter(peaksSortedByRT);

	vPeak3D::iterator currItr, lItr, rItr;
	// RTの双方向に検索するために、基準となるピークのIteratorを取得する。
	currItr = std::find(peaksSortedByRT.begin(), peaksSortedByRT.end(), peak);
	// RTの正方向の範囲検索
	rItr = searchMZRange(currItr, peaksSortedByRT.end()-1, rtTol, true, mzRange);
	// RTの負方向の範囲検索
	lItr = searchMZRange(currItr, peaksSortedByRT.begin(), rtTol, false, mzRange);

	return true;
}

bool NewAB3DLabeling::getXICPeak(ELM::vXYP& xic, const double& rt, ELM::PKPT& xicPeak) {
	const double XIC_LWA_RT_MERGE_DISTANCE = 0;
	const double XIC_LWA_FWHM_RATIO = getSettingMapDouble("MergeDataPointsWeightOfLocalMaximum");
	const ELM::LRP XIC_LWA_FWHM_WIDTH(RFN::LR::NG_LRP);

	double minRTDiff = 100.0;
	double rtDiff;

	// LWAによるピークピッキングを行う。
	ELM::vPKPT xicPeaks;
	EMS::LWA::onLWALabeling(xic, XIC_LWA_RT_MERGE_DISTANCE, XIC_LWA_FWHM_RATIO, XIC_LWA_FWHM_WIDTH, xicPeaks);

	// LWAにより取得したピークの中で、対象のピークのRTと最も近いピークを取得する。
	for (ELM::vPKPT::iterator xicPIt = xicPeaks.begin(); xicPIt != xicPeaks.end(); ++xicPIt) {
		rtDiff = abs(xicPIt->Apex.first - rt);
		if (rtDiff < minRTDiff && xicPIt->Lp.first <= rt && rt <= xicPIt->Rp.first) {
			xicPeak = *xicPIt;
			minRTDiff = rtDiff;
		}
		if (rtDiff == 0.0) break;
	}
	return (minRTDiff == 0);
}

inline double NewAB3DLabeling::calcAreaByXIC(
		ELM::vXYP& xic, const double& lowerRT, const double& upperRT) {
	double area = 0.0;
	for (ELM::vXYP::iterator xicItr = xic.begin(); xicItr != xic.end(); ++xicItr) {
		if (lowerRT <= (*xicItr).first && (*xicItr).first <= upperRT) {
			area += (*xicItr).second;
		}
	}
	return area;
}

void NewAB3DLabeling::deleteShoulderPeaks(vPeak3D& detectedPeaks) {

	const double INTENSITY_RATIO = getSettingMapDouble("RemoveShoulderPeaksIntRatio");
	const double RT_DIFF = getSettingMapDouble("RemoveShoulderPeaksRtTol");
	const double MZ_DIFF_PPM = getSettingMapDouble("RemoveShoulderPeaksMzTol");
	//const double MZ_DIFF = MZ_DIFF_PPM * 1/1000000;
	const double MZ_DIFF = MZ_DIFF_PPM * kPPM;

	Cnum::sortByIntensity_DESC(detectedPeaks);

	// ピークのリストをコピーし、RTでソートする。
	vPeak3D pksSortedByMZ = detectedPeaks;
	//vPeak3D tgtPks;
	vPeak3D rejectPks;

	Cnum::sortByMZcenter(pksSortedByMZ);

	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {
		const double intThr = (*pkItr)->intensity / INTENSITY_RATIO;
		const double mzDiff = (*pkItr)->mz.c * MZ_DIFF;
		const double rtLower = (*pkItr)->rt.l - RT_DIFF;
		const double rtUpper = (*pkItr)->rt.r + RT_DIFF;

		vPeak3D tgtPks;
		if (!Cnum::getPeaksBelowIntensityFromSortedByMz(
				pksSortedByMZ, intThr,
				(*pkItr)->mz.l - mzDiff, (*pkItr)->mz.r + mzDiff,
				rtLower, rtUpper, tgtPks)) {
			continue;
		}

		Cnum::setFlagToPeaks(tgtPks, false);
	}
	Cnum::removePeaksFromList(detectedPeaks);
}

// 連続するピークの除去
void NewAB3DLabeling::removeContinuousPeaks(vPeak3D& detectedPeaks, vPeak3D& clusterCandidatePeaks) {
	
	const double REMOVE_CONTINUOUS_PEAKS_MZ_PPM_TOLERANCE = getSettingMapDouble("RemoveContinuousPeaksMzTol");
	const double REMOVE_CONTINUOUS_PEAKS_MZ_RATIO = REMOVE_CONTINUOUS_PEAKS_MZ_PPM_TOLERANCE * kPPM;
	const double REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE = getSettingMapDouble("RemoveContinuousPeaksRtTol");
	const double REMOVE_CONTINUOUS_PEAKS_INTENSITY_RATIO = getSettingMapDouble("RemoveContinuousPeaksIntRatio");
	
	vPeak3D::iterator tmpItr;
	vPeak3D nearMZPks;
	double currMZ, lowerMZ, upperMZ;
	double upperRT, lowerRT;
	double intThreshold;
	double threshold;

	// 倒したフラグを元に戻す。
	Cnum::setFlagToPeaks(detectedPeaks, true);

	vPeak3D pksSortedByMZ = detectedPeaks;
	Cnum::sortByMZcenter(pksSortedByMZ);

	Cnum::sortByIntensity_DESC(detectedPeaks);
	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {
		if (!(*pkItr)->sflag) {
			continue;
		}

		intThreshold = (*pkItr)->intensity * REMOVE_CONTINUOUS_PEAKS_INTENSITY_RATIO;
		currMZ = (*pkItr)->mz.c;
		lowerMZ = currMZ - REMOVE_CONTINUOUS_PEAKS_MZ_RATIO * currMZ;
		upperMZ = currMZ + REMOVE_CONTINUOUS_PEAKS_MZ_RATIO * currMZ;

		// MZが近傍のピークリストをクリアする。
		nearMZPks.clear();
		// MZが近傍のピークを取得する。
		// Intensityに+1するのは、それ自身のピークも取得したいため。
		if (!Cnum::getPeaksBelowIntensityFromSortedByMz(
			pksSortedByMZ, (*pkItr)->intensity,
			lowerMZ, upperMZ, -1, -1, nearMZPks)) {
			// m/zの近傍に存在するピークが存在しない場合
			continue;
		}
		// 自分自身のピークを追加する。 
		nearMZPks.push_back(*pkItr);
		// MZが近傍のピークリストをRetention Timeでソートする。
		Cnum::sortByRTcenter(nearMZPks);

		// 対象のピークよりも正方向の処理
		upperRT = (*pkItr)->rt.c;
		lowerRT = (*pkItr)->rt.c;
		for (tmpItr = nearMZPks.begin(); tmpItr != nearMZPks.end(); ++tmpItr) {
			if ((*tmpItr)->rt.c < lowerRT) continue;
			if ((*tmpItr)->rt.l <= upperRT) {
				// オーバーラップしている場合
				threshold = (*pkItr)->intensity;
			} else if ((*tmpItr)->rt.l <= upperRT + REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE) {
				// オーバーラップしていないが、規定値の範囲内の場合
				threshold =  intThreshold;
			} else {
				// RTのcenterでソートしており、ピーク範囲の上限の順番とは異なる可能性があるため
				// breakではなくcontinueとする。
				continue;
			}
			// ピーク範囲の最大を保持する。
			upperRT = std::max(upperRT, (*tmpItr)->rt.r);
			if ((*tmpItr)->sflag && (*tmpItr)->intensity < threshold) {
				if ((*tmpItr)->sflag) {
					clusterCandidatePeaks.push_back(*tmpItr);
				}
				(*tmpItr)->sflag = false;
			}
		}

		// 対象のピークよりも負方向の処理
		std::reverse(nearMZPks.begin(), nearMZPks.end());
		upperRT = (*pkItr)->rt.c;
		lowerRT = (*pkItr)->rt.c;
		for (tmpItr = nearMZPks.begin(); tmpItr != nearMZPks.end(); ++tmpItr) {
			if ((*tmpItr)->rt.c > upperRT) continue;
			if ((*tmpItr)->rt.r >= lowerRT) {
				// オーバーラップしている場合
				threshold = (*pkItr)->intensity;
			} else if ((*tmpItr)->rt.r >= lowerRT - REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE) {
				// オーバーラップしていないが、規定値の範囲内の場合
				threshold = intThreshold;
			} else {
				// RTのcenterでソートしており、ピーク範囲の下限の順番とは異なる可能性があるため
				// breakではなくcontinueとする。
				continue;
			}
			// ピーク範囲の最小を保持する。
			lowerRT = std::min(lowerRT, (*tmpItr)->rt.l);
			if ((*tmpItr)->sflag && (*tmpItr)->intensity < threshold) {
				if ((*tmpItr)->sflag) {
					clusterCandidatePeaks.push_back(*tmpItr);
				}
				(*tmpItr)->sflag = false;
			}
		}
	}

	Cnum::deleteFalseFlagObject(detectedPeaks);
	Cnum::sortByIntensity_DESC(detectedPeaks);
}


//Clusterの作成
void NewAB3DLabeling::createClusters(
	vPeak3D& detectedPeaks, std::map<Ptr, std::pair<int,Ptr>>& clusterMap, vPeak3D& clusterCandidatePeaks, ELM::vXYZP& vxyzp
){
	const double clusterRtTol = getSettingMapDouble("ClusteringRtTol");

	const double clusterMzTolRatio = getSettingMapDouble("ClusteringMzTol") * kPPM;

	// 同位体クラスターを作成する際、同位体のピークのIntensityのThreahold
	const double minIntensity = getSettingMapDouble("MergeMinIntensity");

	ELM::LRP tmpDbl = splitSettingVal(settingMap_["ClusteringIntRatio"]);
	const double clusterIntensityRateLowerLimit = tmpDbl.first;
	const double clusterIntensityRateUpperLimit = tmpDbl.second;

	tmpDbl = splitSettingVal(settingMap_["ClusteringCharge"]);
	const int clusterMinCharge = (int)tmpDbl.first;
	const int clusterMaxCharge = (int)tmpDbl.second;

	const int minNumOfPeaks = getSettingMapInteger("ClusteringNumberOfPeaks");

	// 倒したフラグを元に戻す。
	Cnum::setFlagToPeaks(detectedPeaks, true);
	Cnum::setFlagToPeaks(clusterCandidatePeaks, true);

	// ピークのリストをコピーし、RTでソートする。
	vPeak3D pksSortedByRT = detectedPeaks;
	pksSortedByRT.reserve(pksSortedByRT.size() + clusterCandidatePeaks.size());
	pksSortedByRT.insert(pksSortedByRT.end(), clusterCandidatePeaks.begin(), clusterCandidatePeaks.end());
	Cnum::sortByRTcenter(pksSortedByRT);

	std::map<Ptr, Ptr> pk2MonoIsoPk;

	// ピークの高い方から処理をしていく。
	// ピーク降順に並んでいる前提とする。
	vPeak3D additionalPks;
	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {

		if (clusterMap.find(*pkItr) != clusterMap.end()) {
			continue;
		}

		RFN::XYP::vXYP_ vxyp;
		if (!getDataPointsByRT(vxyzp, (*pkItr)->rt.c, vxyp)) {
			// 該当のScanのDataPointが取得できなかったとき
			continue;
		}

		// RTのTolerance範囲内のピークを取得する。
		// m/zの範囲については、対象のモノアイソピークよりも数Da確保されていればよい。
		// 実際にはそれ以降の処理で更に絞り込まれる。
		vPeak3D neighborPeaksSortedByIntensity;
		if (!Cnum::getPeaksFromSortedByRT(
				pksSortedByRT, 
				(*pkItr)->mz.c - clusterMzTolRatio,	
				(*pkItr)->mz.c + 10.0,				
				(*pkItr)->rt.c - clusterRtTol,
				(*pkItr)->rt.c + clusterRtTol, 
				neighborPeaksSortedByIntensity)) {
			continue;
		}
		Cnum::sortByIntensity_DESC(neighborPeaksSortedByIntensity);

		vPeak3D clusteredPks;
		bool foundExistingCluster = false;
		std::pair<int, Ptr> clusterPair;

		const double mzTol = (*pkItr)->mz.c * clusterMzTolRatio;
		double diffMZ;
		int foundCharge = -1;

		// 価数のループ
		// ループの終了条件
		// 設定されている価数の範囲を超えたとき、または既存のクラスタのモノアイソとなったとき
		for (int charge = clusterMaxCharge;
				!foundExistingCluster && charge >= clusterMinCharge; charge--) {

			diffMZ = 1.0 / (double)charge;
			vPeak3D tmpClusteredPks;

			double latestMZ = (*pkItr)->mz.c;
			double latestIntensity = (*pkItr)->intensity;
			int c = 1;

			// ある価数の同位体ピーク検索処理のループ
			// ループの終了条件
			// 該当位置にピークが見つからなくなったとき、
			// または該当位置に価数が一致する既存のクラスタのモノアイソピークが見つかったとき
			while (1) {
				double calcMz = latestMZ + diffMZ;
				const double upperIntensity = latestIntensity * clusterIntensityRateUpperLimit;
				const double lowerIntensity = latestIntensity * clusterIntensityRateLowerLimit;

				// m/zとIntensityの範囲で候補を絞り込む。
				vPeak3D tmpCandidatePks;
				Cnum::getPeaksFromSortedByIntensityDesc(
						neighborPeaksSortedByIntensity,
						lowerIntensity, upperIntensity,
						calcMz - mzTol, calcMz + mzTol,
						tmpCandidatePks);

				// RTのピークのずれのToleranceを満たしているが、 
				// 最も高いIntensityのピークのRTが低いIntensityのエリアから外れているピークを除外する。
				vPeak3D rtOverlappedPeaks;
				if (!Cnum::getRTOverlappedPeaks(
						tmpCandidatePks, (*pkItr)->rt.c, rtOverlappedPeaks)) {
					break;
				}

				// m/zの理論値との差が最も小さいピークを選択する。
				Ptr foundPk;
				Cnum::getNearestMzPeak(rtOverlappedPeaks, calcMz, foundPk);
				if (clusterMap.find(foundPk) != clusterMap.end()) {
					// 既にクラスター化されていた場合
					if (clusterMap[foundPk].first == charge) {
						// 価数が一致している場合は、そのクラスターに含める。
						clusterPair = clusterMap[foundPk]; 
						foundExistingCluster = true;
					}
					break;
				} else {
					// クラスター化されてないピークが存在する場合
					tmpClusteredPks.push_back(foundPk);
					latestMZ = foundPk->mz.c;
					latestIntensity = foundPk->intensity;
				}
				++c;
			}

			if (tmpClusteredPks.size() >= (minNumOfPeaks - 1)) {
				if (clusteredPks.size() < tmpClusteredPks.size()) {
					// 既に見つかったクラスターの候補よりも構成ピークの数が多い場合
					clusteredPks = tmpClusteredPks;
					foundCharge = charge;
				}
			}
			if (foundExistingCluster) {
				// 既に存在するクラスターに含める場合
				foundCharge = charge;
				break;
			}
		}

		if (foundCharge > 0) {
			if (foundExistingCluster) {
				// 既に存在するクラスターに含める場合には、 
				// このピークがモノアイソになるために、既に存在する同位体ピークのマップを置き換える。
				vPeak3D tmpPks;
				std::map<Ptr, Ptr>::const_iterator foundItr;
				for (foundItr = pk2MonoIsoPk.begin(); foundItr != pk2MonoIsoPk.end(); ++foundItr) {
					if (clusterPair.second != foundItr->second) continue;
					tmpPks.push_back(foundItr->first);
				}
				for (vPeak3D::iterator tmpItr = tmpPks.begin(); tmpItr != tmpPks.end(); ++tmpItr) {
					clusterMap[*tmpItr] = std::make_pair(foundCharge, *pkItr);
					pk2MonoIsoPk[*tmpItr] = *pkItr;
				}
			}
			// Cluster の作成
			clusterPair = std::make_pair(foundCharge, *pkItr);
			clusterMap[*pkItr] = clusterPair;
			pk2MonoIsoPk[*pkItr] = *pkItr;
			for (vPeak3D::iterator clsPkItr = clusteredPks.begin(); clsPkItr != clusteredPks.end(); ++clsPkItr) {
				clusterMap[*clsPkItr] = clusterPair;
				vPeak3D::iterator delItr = std::remove(pksSortedByRT.begin(), pksSortedByRT.end(), *clsPkItr);
				pksSortedByRT.erase(delItr, pksSortedByRT.end());
				pk2MonoIsoPk[*clsPkItr] = *pkItr;

				if (std::find(clusterCandidatePeaks.begin(), clusterCandidatePeaks.end(), *clsPkItr) != clusterCandidatePeaks.end()) {
					additionalPks.push_back(*clsPkItr);
				}
			}
		}
	}

	detectedPeaks.reserve(detectedPeaks.size() + additionalPks.size());
	detectedPeaks.insert(detectedPeaks.end(), additionalPks.begin(), additionalPks.end());

	for (vPeak3D::iterator pkItr = additionalPks.begin(); pkItr != additionalPks.end(); ++pkItr) {
		vPeak3D::iterator endItr = std::remove(clusterCandidatePeaks.begin(), clusterCandidatePeaks.end(), *pkItr);
		clusterCandidatePeaks.erase(endItr, clusterCandidatePeaks.end());
	}
}

// settingMap (paramsより取得した値)　から　Key　-> Val(Integer)を受け取る
// なければ -1 を返す
int NewAB3DLabeling::getSettingMapInteger(const std::string& pname) const 
{
	std::map<std::string, std::string>::const_iterator itr = settingMap_.find(pname);
	if (itr != settingMap_.end()) {
		return boost::lexical_cast<int>(itr->second);
	}
	return -1;
}

double NewAB3DLabeling::getSettingMapDouble(const std::string& pname) const 
{
	std::map<std::string, std::string>::const_iterator itr = settingMap_.find(pname);
	if (itr != settingMap_.end()) {
		return boost::lexical_cast<double>(itr->second);
	}
	return -1.0;
};

// For AB3D Single
// サンプルを開いてAB3Dを実行した場合に呼ばれます。
// [in]  dataSet (mass++のサンプルのオブジェクト)
// [out] peaks2d (mass++のpeaks2Dのオブジェクト（結果))
void NewAB3DLabeling::doAB3D (
	kome::objects::DataSet& dataSet,
	kome::objects::Peaks2D* peaks2d
) {
	onDoAB3D(dataSet, peaks2d, nullptr);
}

// For AB3D with peakPositions
// DB Wizard経由で　サンプルを開いてAB3Dを実行した場合に呼ばれます。
// [in]  dataSet (mass++のサンプルのオブジェクト)
// [out] dbac    (mass++のpeaks2Dの結果)
void NewAB3DLabeling::doAB3D (kome::objects::DataSet& dataSet, tDBAC* dbac) {
	onDoAB3D(dataSet, nullptr, dbac);
}

// プログレスバーをparams(本体)から受け取って、引き渡します。
// ...Top.cpp内で情報を貰っています。
// コンストラクタに入れたほうが良いかもしれません。
void NewAB3DLabeling::bindProgress(kome::core::Progress* progress)
{
	myProgress_ = progress;
}

const bool NewAB3DLabeling::isSelected(const char* key) {
	return (std::strcmp(settingMap_[key].c_str(), "true") == 0);
}

// 今後の拡張を兼ねて on で呼ばれる方式に変更。
bool NewAB3DLabeling::onDoAB3D (
	kome::objects::DataSet& dataSet,
	kome::objects::Peaks2D* peaks2d,
	tDBAC* dbac
) {

	ALC::TradingPost tdPost;

	// 検証用DB取得フラグ
	const bool needSingleXICDB = (std::strcmp(settingMap_["OutPutForDataBase"].c_str(),"true") == 0);
	// Data Point（XICに使えるSpectrum値の集合)
	ELM::vXYZP main_vxyzp;
	// 計算する範囲
	ELM::MZRTP mainMZRT =
		ELM::MZRTP(splitSettingVal(settingMap_["MzRange"]), splitSettingVal(settingMap_["RtRange"]));
	// 初期の計算候補の配列
	vPeak3D peaksSoterdByIntensity;

	// PPM -> Ratioに計算
	const double mzPPMTol = getSettingMapDouble("MergeDataPointsMzTol"); // 10.0;
	const double mzTolRatio = mzPPMTol * kPPM;

	ELM::LRP tmpDbl = splitSettingVal(settingMap_["ClusteringCharge"]);
	const int minCharge = (int)tmpDbl.first;
	const int maxCharge = (int)tmpDbl.second;


	const bool removeIsotopicDataPoints =
			isSelected("MergeDataPointsRemoveNonIsotopicDataPoints");

	if (mainMZRT.mz.first == 0 && mainMZRT.mz.second == 0) {
		mainMZRT.mz.first = -1;
		mainMZRT.mz.second = -1;
	}
	if (mainMZRT.rt.first == 0 && mainMZRT.rt.second == 0) {
		mainMZRT.rt.first = -1;
		mainMZRT.rt.second = -1;
	}
	
	tdPost.getSpectrumDataPoints(
		dataSet, mainMZRT, 1, ALC::HAS_CHROM, main_vxyzp, peaksSoterdByIntensity,
		removeIsotopicDataPoints, minCharge, maxCharge, mzTolRatio);

	Cnum::sortByIntensity_DESC(peaksSoterdByIntensity);

	const double availableIntensityRatio = getSettingMapDouble("MergeDataPointsIntRatio");

	const double rtTol = getSettingMapDouble("MergeDataPointsRtTol");//0.5; 
	const double peakendhighprecent = 0.1;
	const double minIntensity = getSettingMapDouble("MergeMinIntensity");

	// 同位体クラスターを作成する際、同位体のピークのIntensityのThreahold
	const double lowerDatapointIntensityLimit = minIntensity * availableIntensityRatio;

	// m/zの範囲を決定するピークのm/zをそのまま範囲に適用してしまうとぎりぎりXICが取得できない場合があるので、
	// 少し広めに範囲を設定する必要がある。その余白の幅(単位:PPM)。
	const double XIC_MZ_MARGIN_PPM = 5.0;
	const double XIC_MZ_MARGIN_RATE_LOWER = (1.0 - XIC_MZ_MARGIN_PPM * kPPM);
	const double XIC_MZ_MARGIN_RATE_UPPER = (1.0 + XIC_MZ_MARGIN_PPM * kPPM);

	// 一次作業用オブジェクト
	ELM::vPKPT xicLWAPeaks;
	vPeak3D	detectedVPK3D;
	vPeak3D clusterCandidateVPK3D;
	ELM::vXYP rstXIC;

	//低いピーク除去
	Cnum::eraseRemoveFalseFlagObject(peaksSoterdByIntensity, lowerDatapointIntensityLimit);

	vPeak3D peaksSortedByMz = peaksSoterdByIntensity;
	Cnum::sortByMZcenter(peaksSortedByMz);

	vPeak3D::iterator currItr = peaksSoterdByIntensity.begin();
	detectedVPK3D.reserve(peaksSoterdByIntensity.size());

	// Flagが一定以上溜まったら　消す監視フラグ
	long falseFlagCount = 0;
	long refleshcycle = static_cast<long>(peaksSoterdByIntensity.size() * 0.04);

	// DataBase用のMAP
	std::map<Ptr, int> dbmap_Prof;

	// Progressを追加
	// Progress関連の関数呼び出しが煩雑なので、クラスでまとめてあります。
	// 検証時にプログレスが邪魔な場合は、引数をnullptrにしてください
	// 以降の progは記述がありますが、全部無視されるようになります。
	AB3DProg prog(myProgress_);

	prog.nextStep("Detecting peaks...");

	while ( !myProgress_->isStopped() ) {
		// 既定値を超えたのでリセット。
		if (refleshcycle < falseFlagCount) {

			// フラグがfalseなピークを取り除く。
			// Intensityでのソート順は維持されているのでソートし直さない。
			Cnum::removePeaksFromList(peaksSoterdByIntensity);

			peaksSortedByMz = peaksSoterdByIntensity;
			Cnum::sortByMZcenter(peaksSortedByMz);

			currItr = peaksSoterdByIntensity.begin();
			falseFlagCount = 0;
			prog.nextStep();
		}

		// Intensity降順のリストの未処理の先頭に移動する。
		Ptr currPeak;
		if (!Cnum::setForeFront(peaksSoterdByIntensity, currPeak, currItr)) {
			break;
		}

		if (currPeak->intensity < minIntensity) {
			// Intensity降順に処理を行うため、
			// 対象のピークのIntensityが閾値を下回れば終了する。
			break;
		}

		// Intensityの足切をせずに、範囲内の全てのピークをIntensityの降順で取得する。
		// RTに関しては、ピークピッキング処理の都合上、RTの余白をToleranceの2倍確保する。
		const double lowerXICRt = currPeak->rt.c - rtTol;
		const double upperXICRt = currPeak->rt.c + rtTol;
		vPeak3D peaksInArea;
		peaksInArea.clear();
		Cnum::getPeaksFromSortedByMz(peaksSortedByMz,
			currPeak->mz.c * (1.0 - mzTolRatio), currPeak->mz.c * (1.0 + mzTolRatio),
			lowerXICRt, upperXICRt, peaksInArea);

		// 検出したピークのm/zの中心と範囲
		// m/zの範囲については、この後の処理で決定するので、とりあえず対象ピークの中心をセットする。
		ELM::LCRP peakMzRange(currPeak->mz.c, currPeak->mz.c, currPeak->mz.c);

		// 存在するピークから、m/z範囲を決定する。
		if (!detectMZRangeByPeakPositions(
				currPeak, peaksInArea,
				rtTol, currPeak->intensity * availableIntensityRatio, peakMzRange)) {
			// 対象範囲から有効なピークが取得できない場合
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		// 取得したピークのm/zよりも若干広めに範囲を設定する。
		// これをしなければ、m/zがピークすれすれになってしまい、期待したXICが取得できない場合あり。
		peakMzRange.l *= XIC_MZ_MARGIN_RATE_LOWER;
		peakMzRange.r *= XIC_MZ_MARGIN_RATE_UPPER;

		rstXIC.clear();
		if (!tdPost.createXIC(
				main_vxyzp, peakMzRange.l, peakMzRange.r, lowerXICRt, upperXICRt, rstXIC)) {
			// 指定範囲のXICの取得に失敗した場合、
			// 対象のスペクトラムピークを無効にする。
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		ELM::PKPT xicPeak;
		if (!getXICPeak(rstXIC, currPeak->rt.c, xicPeak)) {
			// XICからピークの取得に失敗した場合、
			// 対象のスペクトラムピークを無効にする。
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		// 検出したピークのRTの中心と範囲
		ELM::LCRP peakRtRange(xicPeak.Lp.first, currPeak->rt.c, xicPeak.Rp.first);

		// 範囲内に存在するデータポイントを無効にする。
		falseFlagCount += setFalseFlagToUsedPeak3D(
			peaksInArea,
			peakMzRange.l, peakMzRange.r, peakRtRange.l, peakRtRange.r);

		// ピーク範囲内に存在するデータポイントのIntensityを取得して、取得元の値を0にする。
		std::vector<double> intensities;
		tdPost.clearIntensity(
			main_vxyzp,
			peakMzRange.l, peakMzRange.r,
			peakRtRange.l, peakRtRange.r, intensities);

		// S/N比などの基準でピークの有効判定を行う。
		// XICの取得処理で行わないのは、ピークが無効であった場合でも
		// 範囲内の全てのデータポイントのフラグを無効にしたいため。
		//if (!verifyPeak(rstXIC, xicPeak, intensities)) {
		const int peakStatus = verifyPeak(rstXIC, xicPeak, intensities);
		if (peakStatus == 0) {
			// 無効なピークとして判定された場合
			continue;
		}

		// 範囲内のXICを合計してAreaを取得する。
		double peakArea = calcAreaByXIC(rstXIC, peakRtRange.l, peakRtRange.r);


		// ピークを追加する。
		ELM::PK3D_ptr pk =
			new ELM::Peak3D(currPeak->intensity, peakMzRange, peakRtRange, peakArea,
					0.0, currPeak->logintensity, xicPeak.fwhmVal, intensities.size());
		pk->sflag = true;

		// 必要であればDBに書き込む
		if (!writeToDB(dbac, pk, xicPeak, rstXIC, dbmap_Prof)) {
			return false;
		}

		if (peakStatus == 2 && currPeak->intensity >= minIntensity) {
			// 候補として登録
			detectedVPK3D.push_back(pk);
		} else {
			// クラスタリング時の補助候補として登録
			clusterCandidateVPK3D.push_back(pk);
		}
	}

	unsigned int numOfPeaks = detectedVPK3D.size();
	LOG_DEBUG(FMT("Detected %d peak(s).", numOfPeaks));

	// 計算時に使った3Dピークを全て消す
	// RTvPK3D 側も呼ばないように注意。
	Cnum::deleteAllElm(peaksSoterdByIntensity);

	if( !myProgress_->isStopped() ){

		// RT方向に連続するピークを削除する。
		prog.nextStep("Removing continous peaks...");

		removeContinuousPeaks(detectedVPK3D, clusterCandidateVPK3D);
	
		// Shoulderピークを削除する。
		prog.nextStep("Removing shoulder peaks...");

		deleteShoulderPeaks(detectedVPK3D);

		// Clusterを作成する。
		prog.nextStep("Creating isotopic clusters...");

		std::map<Ptr, std::pair<int,Ptr>> clusterChargeMap;
		createClusters(detectedVPK3D, clusterChargeMap, clusterCandidateVPK3D, main_vxyzp);

		if (peaks2d != nullptr) {
			toMassppPeak2D(peaks2d, clusterChargeMap, detectedVPK3D);
		} else {
			// 必要であればDBにクラスタ情報を含めて書き込む
			writeClusterInfotoDB(dbac,clusterChargeMap,detectedVPK3D, dbmap_Prof);
		}

		// ポインタ確保先をDeleteする。
		BOOST_FOREACH(const ELM::XYZP& tmpxyzp, main_vxyzp) {
			delete tmpxyzp.vxyp;
		}

		// ゴミを除去
		Cnum::deleteAllElm(detectedVPK3D);
		Cnum::deleteAllElm(clusterCandidateVPK3D);
	}
	prog.closeMe();
	return true;
}

// ソートと、FalseFlagになったオブジェクトをいったん消す。
// 状況によっては不要になるかもしれない(ソートの方が重い感が）
//void NewAB3DLabeling::deleteFalseFlagAndSort(vPeak3D& byIntArr, vPeak3D& byMZArr) {
//	Cnum::sortByMZcenter(byMZArr);
//}

// 範囲内に存在するピークのフラグを倒す。
const unsigned int NewAB3DLabeling::setFalseFlagToUsedPeak3D (
	vPeak3D& peaks,
	const double& lowerMz, const double& upperMz,
	const double& lowerRt, const double& upperRt
){
	vPeak3D peaksInArea;
	peaksInArea.reserve(peaks.size());

	// 指定したm/zとRTの範囲内に存在するピークを取得するために、
	// 一度別のリストにコピーして、それをRTでソートしてから
	// 該当の範囲内のピークを取得する。
	vPeak3D& peaksSortedByRt = peaks;
	Cnum::sortByRTcenter(peaksSortedByRt);
	Cnum::getPeaksFromSortedByRT(peaksSortedByRt, lowerMz, upperMz, lowerRt, upperRt, peaksInArea);

	Cnum::setFlagToPeaks(peaksInArea, false);
	return peaksInArea.size();
}

// 該当関数は可視化の為に　下記に切りだした。内容は一緒。
// Mass++フォーマットに変換する(Peak2D & Cluster) 
void NewAB3DLabeling::toMassppPeak2D (
	kome::objects::Peaks2D* peaks2d, 
	std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
	vPeak3D& detectedVPK3D
) {
	std::map<Ptr, std::pair<int,Ptr>>::iterator cCmapItr;
	std::map<Ptr, kome::objects::PeaksCluster2D*> clusterMap;
	std::map<Ptr, kome::objects::PeaksCluster2D*>::iterator cmapItr;
	std::pair<int, Ptr> clusterPair;

	// クラスターを構成しないピークを除外するかどうか
	const bool removeNonClustered = isSelected("ClusteringRemoveNonClustered");
	// モノアイソのみに絞り込むかどうか
	const bool onlyMonoIsoPeaks = isSelected("ClusteringOutputOnlyMonoisotopic");

	Cnum::sortByMZcenter(detectedVPK3D);

	BOOST_FOREACH(const Ptr pk, detectedVPK3D) {
		cCmapItr = clusterChargeMap.find(pk);

		// クラスターを構成するピークかどうか
		bool isClustered = (cCmapItr != clusterChargeMap.end());
		// モノアイソのピークかどうか
		bool isMonoIsotopic = false;
		if (isClustered) {
			clusterPair = cCmapItr->second;
			cmapItr = clusterMap.find(clusterPair.second);
			isMonoIsotopic = (cmapItr == clusterMap.end());
		}

		if (!removeNonClustered || isClustered) {
			// 設定画面で、"Exclude Non-clustered Peaks"が選択された場合は、
			// クラスターを構成するピークのみをピークとして採用する。 

			if (isMonoIsotopic || !onlyMonoIsoPeaks || (!removeNonClustered && !isClustered)) {

				// 設定画面で、"Monoisotopic Peaks Only"が選択された場合は、
				// モノアイソのピークのみを採用する。
				kome::objects::Peak2DElement* pe =
					peaks2d->createPeak(
							pk->rt.c, pk->rt.l, pk->rt.r,
							pk->mz.c, pk->mz.l, pk->mz.r, pk->intensity);
				pe->setArea(pk->area);
				if (isClustered) {
					pe->setCharge(clusterPair.first);
				}

				if (isMonoIsotopic) {
					kome::objects::PeaksCluster2D* cluster = peaks2d->createCluster();
					cluster->setCharge(clusterPair.first);
					clusterMap[clusterPair.second] = cluster;
				}
				if (isClustered) {
					pe->setCluster(clusterMap[clusterPair.second]);
				}
			}
		}
	}
}

// DBへ書き込みの判定
// Single& DBWriteが走っている場合、TDPost経由でDBに書き込む。
// Multiの場合は、一時テーブルとProfileに必要な情報を書いておく。
const bool NewAB3DLabeling::writeToDB(
	tDBAC* dbac,
	ELM::PK3D_ptr pk, 
	ELM::PKPT& foundpeak,
	const ELM::vXYP& rstXIC, 
	std::map<Ptr, int>& dbmapProf
){
	if (dbac != nullptr) {
		return onWriteToDB(dbac, pk, foundpeak, rstXIC, dbmapProf);
	} 
	return true;
}

// ☆１
// memo 2013.08.01
//
// dbmap_prof(map) について
// PK3Dのポインタ　と DBに保存した　PeakIdを紐づけてを確保してあります。

// ☆２
// memo 2013.08.02
// dbac->setXIC のインタフェースを変更しました。
// Thoto系の実装を全て排除してあります。
// 1. 対象のXICの　vXYP(XYData)
// 　　rstXICの受け側は const std::vector<std::pair<double,double>>& で受けてます。
// 　　ELM::vXYP　が それをtypedefしたものなので同じです。

// 2. ピークの RT, M/Z, Matrix上のValue
// 　 Peak2D　としての　ピークの頂点情報を入れてください。
//    M/Z, RT は、Matrix　ROWの代表値　として使われます。
//    Valueの値は　Matrixの格子の値として表示されます。
//    （此方に定量性を示す値を入れるようになると思います）

// 3. 対象のXICの　ピーク情報(実頂点(X,Y), Left(X,Y),Right(X,Y))
//    Matrxiの結果の波形を描画される際に、ピークとして認識・表示される値です。
//    現状は LWAの中で加重平均をしていないので、ApexとCenterの座標は同じかと存じますが、
//    計算上ではなく、実頂点の座標を渡してください。

const bool NewAB3DLabeling::onWriteToDB(
	tDBAC* dbac,
	ELM::PK3D_ptr pk, 
	ELM::PKPT& foundpeak,
	const ELM::vXYP& rstXIC, 
	std::map<Ptr, int>& dbmapProf
){
	// ☆２
	int dbPeakId = dbac->setXICData(
		rstXIC,
		pk->rt.c,
		pk->mz.c,
		pk->intensity,
		foundpeak.Apex.first,
		foundpeak.Apex.second,
		foundpeak.Lp.first,
		foundpeak.Lp.second,
		foundpeak.Rp.first,
		foundpeak.Rp.second
	);

	// ☆１
	if (dbPeakId >= 0) {
		dbmapProf[pk] = dbPeakId;
		return true;
	}				
	return false;
}


// Cluster & Peakの情報をDBに書き出す必要がある場合
// チャージクラスタの情報が入る。
//
// memo 2013.08.01
//
// [有効なPeakである]と判定された Peakの　Charge を
// dbac->updateCharge(peakId, z); として変更します。
// z = -1 or z >= 2 が来ることを期待しています。
// 何も変更されない値は捨てます。
// 有効とする(Matrix上に出すPeak）は　適当なChargeを与えてください。

// 少し変更のお知らせ
// "Exclude Non-clustered Peaks" が　非選択の時に
// 抜けてしまうパターンがあったので、IFの分岐を少し変更してあります。

void NewAB3DLabeling::writeClusterInfotoDB(
	tDBAC* dbac, 
	std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
	vPeak3D& detectedVPK3D,
	std::map<Ptr, int>& dbmap_Prof
){
	// DBが不正なら折り返す
	if (dbac == nullptr) {
		return;
	}
	std::map<Ptr, std::pair<int,Ptr>>::iterator mapItr;
	std::set<Ptr> usedCluster;

	// クラスターを構成しないピークを除外するかどうか
	const bool excludeNonClusteredPeaks = isSelected("ClusteringRemoveNonClustered");
	// モノアイソのみに絞り込むかどうか
	const bool monoisotopicPeaksOnly = isSelected("ClusteringOutputOnlyMonoisotopic");
	// Mz でソート
	Cnum::sortByMZcenter(detectedVPK3D);

	BOOST_FOREACH(const Ptr pk, detectedVPK3D) {
		int peakId = dbmap_Prof[pk];
		mapItr = clusterChargeMap.find(pk);

		// クラスタマップ上に存在しないものが入ってくる。
		if(mapItr == clusterChargeMap.end()) {
			if (!excludeNonClusteredPeaks) {
				// "Exclude Non-clustered Peaks" が　非選択の時は -1 として補完
				// Peak2dとして得られるが、クラスタの無いものになる。
				dbac->updateCharge(peakId, -1);
			}
			continue;
		}

		Ptr monoIsoPtr = mapItr->second.second;
		int charge = mapItr->second.first;

		if(monoisotopicPeaksOnly) {
			// 設定画面で、"Monoisotopic Peaks Only"が選択された場合は、
			// モノアイソのピークのみを採用する。
			if(usedCluster.count(monoIsoPtr) == 0) {
				usedCluster.insert(monoIsoPtr);
				dbac->updateCharge(peakId, charge);
			}
		} else {
			dbac->updateCharge(peakId, charge);
		}
	}
}

int NewAB3DLabeling::verifyPeak(
	ELM::vXYP& xic, ELM::PKPT& foundPeak, std::vector<double>& intensities) {

	// XICのRetention Timeのピーク幅
	const double peakMinRtSpan = getSettingMapDouble("VerificationPeakSpan");
	if (foundPeak.Rp.first - foundPeak.Lp.first < peakMinRtSpan) {
		// ピークのRetention Time幅が設定値以下の場合
		return 0;
	}

	// XICピークの半値幅の最小値
	const double peakMinFWHM = getSettingMapDouble("VerificationPeakFWHM");
	if (foundPeak.fwhmVal < peakMinFWHM) {
		return 0;
	}

	// XICピークに存在する起伏の最大数
	const int maxUndulations = getSettingMapInteger("VerificationPeakUndulations");
	int lInd = 0, rInd = 0;
	int numOfUndulations = 0;
	double xicInt = -1;
	bool isAscent = false;
	for (ELM::vXYP::iterator xicItr = xic.begin(); xicItr != xic.end(); ++xicItr) {
		if ((*xicItr).first == foundPeak.Lp.first) {
			lInd = xicItr - xic.begin();
		}
		if (xicInt == -1) {
			xicInt = (*xicItr).second;
		} else {
			if (lInd > 0 && rInd == 0 && xicInt > (*xicItr).second && isAscent) {
				// 起伏が存在する場合
				++numOfUndulations;
				if (numOfUndulations > maxUndulations) {
					return 0;
				}
			}
			isAscent = (xicInt < (*xicItr).second);
			xicInt = (*xicItr).second;
		}
		if ((*xicItr).first == foundPeak.Rp.first) {
			rInd = xicItr - xic.begin();
			break;
		}
	}

	const double snRatio = getSettingMapDouble("VerificationPeakSNRatio");
	const int dataPoints = getSettingMapInteger("VerificationNumOfDataPoints");
	const double baselineRatio = getSettingMapDouble("VerificationBaselineIntRatio");
	const bool baselineMethodMedian = (settingMap_["VerificationBaselineMethod"] != "baseline_mean");

	std::vector<double> tmpIntensities = std::vector<double>();
	double sum = 0.0;
	for (std::vector<double>::iterator itr = intensities.begin(); itr != intensities.end(); ++itr) {
		double intensity = *itr;
		if (intensity > 0) {
			tmpIntensities.push_back(intensity);
			if (!baselineMethodMedian) {
				sum += log10(intensity);
			}
		}
	}
	std::sort(tmpIntensities.begin(), tmpIntensities.end(), std::greater<double>());
	int lSize = tmpIntensities.size();
	double baseline;
	if (baselineMethodMedian) {
		if (lSize % 2 == 0) {
			baseline = (tmpIntensities[lSize/2-1] + tmpIntensities[lSize/2]) / 2;
		} else {
			baseline = tmpIntensities[lSize/2];
		}
	} else {
		baseline = pow(10, (sum/lSize));
	}

	double s = 0.0;
	for (std::vector<double>::iterator itr = tmpIntensities.begin(); itr != tmpIntensities.end(); ++itr) {
		s += (*itr - baseline) * (*itr - baseline);
	}

	double maxIntensity = *(tmpIntensities.begin());
	double stdev = std::sqrt(s/lSize);
	double sn;
	if (stdev > 0) {
		sn = (maxIntensity - baseline) / stdev;
	} else {
		sn = 0.0;
	}

	// 各パラメータの条件を満たしているかどうかの判定
	bool rightSn = (sn >= snRatio);
	bool rightDps = (lSize >= dataPoints);
	bool rightBaseline = (baseline / maxIntensity) <= baselineRatio;

	// 全ての条件を満たす場合は、2を返す。
	// ただし、S/N以外の条件を満たして、かつS/Nの条件の1/2を満たしている場合は、1を返す。
	if (rightDps && rightBaseline) {
		if (rightSn) return 2;
		if (sn >= snRatio / 2) return 1;
	}
	return 0;
}
