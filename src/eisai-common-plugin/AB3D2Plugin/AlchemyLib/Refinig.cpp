/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Refining.cpp
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

#pragma warning(disable:4482)
#include "../stdafx.h"
#include "Refining.h"
#include <utility>
#include <tuple>
#include "../Thoto/Standardinline.hpp"
namespace rfn {
namespace rfng { 


const unsigned int kFirst(0);

// 前方宣言部
// この関数群内で使うヘッダに出さないPrivate関数部の宣言のみを行う。

const unsigned int onSearchIndex (kome::objects::DataSet&, const double, const unsigned int, const unsigned int);
const unsigned int onSearchIndexFullRange (kome::objects::DataSet&, const double&);
//const unsigned int onCompareRT(kome::objects::DataSet&, const double&);
const bool onGetRTRangeIndexes(kome::objects::DataSet&, const ELM::LRP&, unsigned int&, unsigned int&);

// 以降実体系===========================================
const std::pair<double,double> get_dataSetRTRange(kome::objects::DataSet& dataSet) {
	const unsigned int fx = 0;
	const unsigned int ex = dataSet.getNumberOfSpectra() - 1;
	return std::make_pair(dataSet.getSpectrum(fx)->getRt(),dataSet.getSpectrum(ex)->getRt());
}

//// DataSet から一致するRTを取得する
//const int searchSpecIndex_FromRT (
//	kome::objects::DataSet& dataSet, 
//	const double tgtrt,
//	unsigned int& rstIdx
//) {
//	if (dataSet.getNumberOfSpectra() < 1) return -2;
//	int xflag = onCompareRT(dataSet, tgtrt);
//	if (xflag == 0) {
//		rstIdx = onSearchIndexFullRange(dataSet, tgtrt);
//	}
//	return xflag;
//}

//const bool isMatchChrom(kome::objects::Spectrum* spec, const bool haschrom, unsigned int msstage) {
const bool isMatchChrom(kome::objects::Spectrum* spec, const ALC::CHROM_FLAG& chflag, unsigned int msstage) {
	if (chflag != ALC::CHROM_FLAG::NO_USE) {
		if (spec->hasChromatogram() != (chflag & ALC::CHROM_FLAG::HAS_CHROM)) {
			return false;
		}
	}
	// msstage が０なら Full Range.
	if (msstage > 0 && spec->getMsStage() != msstage) {
		return false;
	}
	return true;
}

// DataSet からChromatogramの条件に一致する
// Spectrum* の配列を取得する
const bool refiningSpectra (
	kome::objects::DataSet& dataSet, // In
	const ELM::LRP& RTp,
	const ALC::CHROM_FLAG& ch_flag,
//	const bool haschrom,
	unsigned int msstage,
	std::vector<kome::objects::Spectrum*>& refSpec
){
	unsigned int lidx, ridx;
//	if (!onGetRTRangeIndexes(dataSet, minRT, maxRT, lidx, ridx)) {
	if (!onGetRTRangeIndexes(dataSet, RTp, lidx, ridx)) {
		return false;
	}
	if (ridx - lidx <= 0) {
		return false;
	}
	refSpec.reserve(ridx - lidx);
	kome::objects::Spectrum* tmpspec;
	// Indexの範囲から条件に合うSpectrumをVectorに入れる
//	for (unsigned int i = lidx; i <= ridx; i++) {
	for (unsigned int i = lidx; i < ridx; i++) {
		tmpspec = dataSet.getSpectrum(i);
		if (isMatchChrom(tmpspec, ch_flag, msstage)) {
			refSpec.push_back(tmpspec);
		}
	}
	refSpec.shrink_to_fit();
	return true;
}

//template<typename T_n>
//const bool auditPtr(const T_n* tnA, const T_n* tnB, T_n& refA, T_n& refB) {
//	if (tnA != nullptr) refA = *tnA;
//	if (tnB != nullptr) refB = *tnB;
//	return (refA < refB) ? true : false;
//}


//---------------------------------------------------------
//  INNER FUNCTIONS
//---------------------------------------------------------

/*!
 * @breif 近似RTを持つIndexを取得
 * @param[in] dataSet : Mass++のデータセット
 * @param[in] tgtrt   : 検索対象のRT値
 * @param[in] lidx    : dataSetの左辺Index
 * @param[in] ridx    : dataSetの右辺Index
 * @result : 尤もRTに近いIndex
 * @note : 理論的には log(N)の計算量。完全一致ではないので注意。
*/
const unsigned int onSearchIndex (kome::objects::DataSet& dataSet, const double tgtrt, const unsigned int lidx, const unsigned int ridx) {
	unsigned int imin(lidx), imax(ridx),imid(0);
	double tmpVal;
	while (imin < imax) {
		imid = (imin + imax) / 2;
		tmpVal = dataSet.getSpectrum(imid)->getRt();
		if (tmpVal < tgtrt) {
			imin = imid + 1;
		} else if (tmpVal == tgtrt) {
			return imid;
		} else {
			imax = imid - 1;
		} 
	}

	if (dataSet.getSpectrum(imin)->getRt() < tgtrt) imin++;
	return imin;
}

// 近似RTを持つIndexをDataSet全体から取得する。
// "dataSet内である"という前提のもとに使用する事。
const unsigned int onSearchIndexFullRange (kome::objects::DataSet& dataSet, const double& tgtrt) {
	const unsigned int firstidx = 0;
	const unsigned int lastidx = dataSet.getNumberOfSpectra() - 1;
	return onSearchIndex(dataSet, tgtrt, firstidx, lastidx);
}

//const unsigned int onCompareRT(kome::objects::DataSet& dataSet, const double& tgtrt) {
//	if (dataSet.getNumberOfSpectra() < 2) return J_ERROR;
//	double lrt, rrt;
//	std::tie(lrt, rrt) = get_dataSetRTRange(dataSet);
//	return RFN::STD::compVal_d(lrt,rrt,tgtrt);
//}

const bool onGetRTRangeIndexes(
	kome::objects::DataSet& dataSet, const ELM::LRP& RTp,
	unsigned int& lidx, unsigned int& ridx
) {
	const unsigned int specNum = dataSet.getNumberOfSpectra();
	if (specNum < 1) {
		return false;
	}
	lidx = 0;
	ridx = specNum - 1;

	double tlow, thigh;
	std::tie(tlow, thigh) = get_dataSetRTRange(dataSet);

	// 何れかのタイミングで関数化する
	//　サンプル上の最大RTより　要求範囲の最低値が高い
	if (RTp.first > 0.0 && RTp.first > thigh) {
		return false;
	}

	//　サンプル上の最小RTより　要求範囲の最大値が低い
	if (RTp.second > 0.0 && RTp.second < tlow) {
		return false;
	}

	if (RFN::STD::compVal_d(tlow, thigh, RTp.first) == 0) {
		lidx = onSearchIndexFullRange(dataSet, RTp.first);
	}

	if (RFN::STD::compVal_d(tlow, thigh, RTp.second) == 0) {
		ridx = onSearchIndexFullRange(dataSet, RTp.second);
	}
	return true;
}

} //end of .....
}
