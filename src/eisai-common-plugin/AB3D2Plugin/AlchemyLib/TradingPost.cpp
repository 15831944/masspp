/**********************************************************
Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
    @addtogroup Mass++ Plugins Support Lib
	@file       Tragind.cpp
	@brief      Thotoの各要素との置換等。
	@date       2012.12.19
	@author     M.Fukuda
	@version    1.0.0
**********************************************************/
#include "../stdafx.h"
#include "TragingPost.h"
#include "Refining.h"
//#include "ForDB\DBCtrl.h"

//#include <Thoto/Peaks/Peak3DBase.hpp>
#include "../Thoto/Func/PeakFusion.hpp"
#include "../Thoto/Points/LCRPoint.hpp"
#include "../Thoto/Points/XYPoint.hpp"
#include "../Thoto/Points/XYZPoint.hpp"
#include "../Thoto/Func/LR_x_LCR.hpp"
#include "../../../common-plugin-operation/LinearFilterPlugin/LinearFilterManager.h"

#include <boost\foreach.hpp>
#include <boost\timer.hpp>

namespace ALC {

TradingPost::TradingPost(){}
TradingPost::~TradingPost(){}

// XICを作成する
const bool TradingPost::createXIC(
	ELM::vXYZP& vxyzp, 
//	const ELM::MZRTP& mzrtp, 
	const double lowerMz, const double upperMz,
	const double lowerRt, const double upperRt,
	ELM::vXYP& rstXIC
){	
	ITR::vXYZP_Itr lrt, rrt;
	rstXIC.clear();

//	std::tie(lrt, rrt) = RFN::XYZP::searchZ_MinMax(mzrtp.rt.first, mzrtp.rt.second, vxyzp);
	std::tie(lrt, rrt) = RFN::XYZP::searchZ_MinMax(lowerRt, upperRt, vxyzp);
		if (lrt == vxyzp.end()) {
			return false;
		}
		// 右端の処理
		if (rrt == vxyzp.end()) {
			--rrt;
		}
	rstXIC.reserve(std::distance(lrt,rrt));
//	double ticVal(0.0), lmz = mzrtp.mz.first, rmz = mzrtp.mz.second;
	double ticVal(0.0);

	while(1) {
//		ticVal = RFN::XYP::getInRangeTotalIntensity(mzrtp.mz.first, mzrtp.mz.second, *lrt->vxyp);
//		ticVal = RFN::XYP::getInRangeTotalIntensity(lmz, rmz, *lrt->vxyp);
		ticVal = RFN::XYP::getInRangeTotalIntensity(lowerMz, upperMz, *lrt->vxyp);
		RFN::XYP::addXYPoint(lrt->z, ticVal, rstXIC);
		if (lrt == rrt) break;
		++lrt;
	}
	return true;
}

// Intensityを消す
const bool TradingPost::clearIntensity(
	ELM::vXYZP& vxyzp,
//	const ELM::MZRTP& mzrtp,
	const double lowerMz, const double upperMz,
	const double lowerRt, const double upperRt,
	std::vector<double>& intensities
){
	ITR::vXYZP_Itr lrt, rrt;
	// 都度ソートの必要が無いので、Falseに。
//	std::tie(lrt, rrt) = RFN::XYZP::searchZ_MinMax(mzrtp.rt.first, mzrtp.rt.second, vxyzp);
	std::tie(lrt, rrt) = RFN::XYZP::searchZ_MinMax(lowerRt, upperRt, vxyzp);
	if (lrt == vxyzp.end()) {
		return false;
	}
	if (rrt == vxyzp.end()) {
		--rrt;
	}

	while (1) {
//		RFN::XYP::clearY(mzrtp.mz.first, mzrtp.mz.second, *lrt->vxyp, intensities);
		RFN::XYP::clearY(lowerMz, upperMz, *lrt->vxyp, intensities);
		if (lrt == rrt) break;
		++lrt;
	}
	return true;
}

// CreateDBFGがTrueでないと実質、実行されない。
void TradingPost::insertMeXIC (
	const RFN::PEAK::Peak3DPointValue& p,
	const ELM::vXYP& xicVal,
	ELM::vPKPT& xicPeaks
) {
	if (isCreateDBFg_) {
//		DB::InsertXIC(p, xicVal, xicPeaks);
	}
}

ELM::PK3D_ptr createPeak3DPtr(
	const double mz, const double rt, const double intensity
){
	ELM::PK3D_ptr k = new ELM::Peak3D(intensity, mz, mz, mz, rt);
	return k;
}

const bool TradingPost::getSpectrumDataPoints(
	kome::objects::DataSet& dataSet, 
	const ELM::MZRTP& mzrtp,
	const unsigned int msStage,
	const CHROM_FLAG ch_flag,
	ELM::vXYZP& rst_vxyzp,
	vPeak3D& rst_Peaks,
	const bool removeIsotopicDataPoints,
	const int minCharge,
	const int maxCharge,
	const double mzTolRatio
) {

	const size_t datanum = dataSet.getNumberOfSpectra();
	kome::objects::Sample* sp = dataSet.getSample();
	std::vector<kome::objects::Spectrum*> refSpec;

	// 条件に一致する Spectraのリストを作成する
	// この場合は、RT-の幅で、hasChromであり MS = 1 のデータを探す。
	rfn::rfng::refiningSpectra(dataSet, mzrtp.rt, HAS_CHROM, msStage, refSpec);
//	ELM::vXYP srcXyp;
	ELM::vXYP rstXyp;

	// 配列を確保
	rst_vxyzp.reserve(refSpec.size());

	boost::timer timer;

	LOG_INFO(FMT("Started reading data points."));
	kome::objects::Peaks* peaks = new kome::objects::Peaks();

	unsigned int currInd = 1;
	unsigned int allDps = 0;
	unsigned int filteredDps = 0;
	const double lmz = mzrtp.mz.first;
	const bool isMaxInfinity = (mzrtp.mz.second < 0.0);
	const double hmz = (isMaxInfinity) ? DBL_MAX : mzrtp.mz.second;
	const bool isSupportedAPI = sp->isSupportedAPIPeaks();
	for each(kome::objects::Spectrum* tmpspec in refSpec) {

		if (currInd % 100 == 1) {
			LOG_DEBUG(FMT("scan (%d/%d)", currInd, refSpec.size()));
		}
		currInd++;

		rstXyp.clear();
		double xRT = tmpspec->getRt();
		std::vector<std::pair<double, double>> mzIntensityList;
		if (isSupportedAPI) {
			// APIがサポートされているデータ形式の場合
			// APIを用いてピークピッキングをおこなう。
			kome::objects::Peaks pks;
			sp->detectPeaksByAPI(tmpspec, &pks);
			for (unsigned int i = 0; i < pks.getLength(); i++) {
				double mz = pks.getX(i);
				if (lmz <= mz && mz <= hmz) {
					double inty = pks.getY(i);
					mzIntensityList.push_back(std::pair<double, double>(mz, inty));
				}
			}
		} else {
			// APIがサポートされていないデータ形式の場合
			// APIを用いずにピークピッキングをおこなう。
			// (ノーマライズされたデータはこちらで処理する)
			std::vector<DtLabel> vDTL;

//			srcXyp.clear();
			//rstXyp.clear();

			getDataPointsFromSpectrum(tmpspec, hmz, lmz, vDTL, peaks);

			BOOST_FOREACH(const DtLabel dtl, vDTL) {
				mzIntensityList.push_back(std::pair<double, double>(dtl.mz, dtl.intensity));
			}
		}
		allDps += mzIntensityList.size();

		if (removeIsotopicDataPoints) {
			// Isotopic Filtering処理
			std::vector<std::pair<double, double>> isotopicMzIntensityList =
				getIsotopicDataPoints(mzIntensityList, minCharge, maxCharge, mzTolRatio);
			mzIntensityList.clear();
			std::copy(isotopicMzIntensityList.begin(), isotopicMzIntensityList.end(), std::back_inserter(mzIntensityList));
			filteredDps += mzIntensityList.size();
			isotopicMzIntensityList.clear();
		}

		for (std::vector<std::pair<double, double>>::iterator it = mzIntensityList.begin();
				it != mzIntensityList.end(); ++it) {
			RFN::XYP::addXYPoint((*it).first, (*it).second, rstXyp);
			rst_Peaks.push_back(createPeak3DPtr((*it).first, xRT, (*it).second));	
		}
		// エントリポイント作成
		SPTR::vXYP_sp vxyp_sp = RFN::XYZP::createPoint_Sp(xRT, rst_vxyzp);
		
		// 本体にストア
		RFN::XYP::getInRangeElements(lmz, hmz, rstXyp, *vxyp_sp);
	}

	delete peaks;

	if (removeIsotopicDataPoints) {
		LOG_DEBUG(FMT("Got %d datapoint(s) from spectra.(%d datapoint(s) were removed by isotopic filter.)", filteredDps, (allDps - filteredDps)));
	} else {
		LOG_DEBUG(FMT("Got %d datapoint(s) from spectra.", allDps));
	}
	LOG_INFO(FMT("Finished reading data points. Elapsed Time: %.03f", timer.elapsed()));
	return true;
}

const bool TradingPost::getDataPointsFromSpectrum(
	kome::objects::Spectrum* tmpspec,
	const double hmz,
	const double lmz,
	std::vector<DtLabel>& rstDTLArr,
	kome::objects::Peaks* peaks
) {
	// Local Maximum
	const char* alg = "local";
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* m_specPeakDetector
		= plgMgr.getFunctionItem("SPEC_PEAK_PICKING", alg);
	kome::objects::Parameters peakParams;

	kome::objects::SettingParameterValues vals;
	kome::plugin::PluginCallTool::setSettingValues(peakParams, vals);

	kome::plugin::PluginCallTool::setSpectrum(peakParams, *tmpspec);
	kome::plugin::PluginCallTool::setXYData(peakParams, *tmpspec->getXYData());
	kome::plugin::PluginCallTool::setPeaks(peakParams, *peaks);
	m_specPeakDetector->getCall()->invoke(&peakParams);

	const unsigned int pknum = peaks->getLength();
	if (pknum > 0) {
		peaks->sort();
		
		DtLabel tmpDTL;
		tmpDTL.charge = 0;
		tmpDTL.resolution = 0.0;
		for (unsigned int idx = 0; idx < pknum; idx++) {
			double tmpMz = peaks->getPeak(idx)->getX();
			if (tmpMz > hmz) break;
			if (tmpMz < lmz) continue;

			tmpDTL.mz = tmpMz;
			tmpDTL.intensity = peaks->getPeak(idx)->getY();
			rstDTLArr.push_back(tmpDTL);
		}
	}
	peaks->clearPoints();
	return true;
}

std::vector<std::pair<double, double>> TradingPost::getIsotopicDataPoints(
		std::vector<std::pair<double, double>>& dataPoints,
		const unsigned int lowerCharge, 
		const unsigned int higherCharge,
		const double mzTolRate) {

	std::vector<bool> flgs(dataPoints.size());

	int currInd = 0;
	std::pair<double, double> currDp;
	BOOST_FOREACH(currDp, dataPoints) {
		double currMz = currDp.first;
		// 判定しなければならない正方向のm/z
		double uppestMz = (currMz + (1 / (double) lowerCharge)) * (1 + mzTolRate);
		std::pair<double, double> dp;
		int ind = 0;
		BOOST_FOREACH(dp, dataPoints) {
			double mz = dp.first;
			if (mz > currMz || (flgs[currInd] && flgs[ind])) {
				if (mz > uppestMz) break;
				for (unsigned int z = lowerCharge; z <= higherCharge; ++z) {
					double diffMz = (1 / (double) z);
					// m/z軸に対して正方向の同位体ピーク範囲
					double lowerMz = (currMz + diffMz) * (1 - mzTolRate);
					double upperMz = (currMz + diffMz) * (1 + mzTolRate);
					if ((currMz + diffMz) * (1 - mzTolRate) <= mz &&
							mz <= ((currMz + diffMz) * (1 + mzTolRate))) {
						// 同位体の位置に存在していれば、双方のフラグをtrueにする。
						flgs[currInd] = true;
						flgs[ind] = true;
					}
				}
			}
			++ind;
		}
		++currInd;
	}

	currInd = 0;
	std::vector<std::pair<double, double>> isotopicDps;
	isotopicDps.reserve(dataPoints.size());
	BOOST_FOREACH(const bool flg, flgs) {
		//if (flgs[currInd]) {
		if (flg) {
			isotopicDps.push_back(std::pair<double, double>(dataPoints[currInd].first, dataPoints[currInd].second));	
		}
		++currInd;
	}
	return isotopicDps;
}



}//end of namespace
