/**********************************************************
Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
    @addtogroup Mass++ Plugins Support Lib
	@file       Tragind.h
	@brief      Thotoの各要素との置換等。
	@date       2012.12.06
	@author     M.Fukuda
	@version    1.0.0
**********************************************************/
#pragma once
#include "AlchmyLib.h"
#include <map>
#include <tuple>
#include "../Thoto/Def_All.hpp"

// 推奨できないけど仮定義
typedef BS::Peak3D* Ptr;
typedef std::vector<Ptr> vPeak3D;

namespace ALC {

class TradingPost {

public:
	// constrcutor
	TradingPost();

	// destrcutor
	~TradingPost();

private:
	// deny copy.
	TradingPost(const TradingPost&);
       TradingPost& operator=(const TradingPost&); 
private:
	typedef std::pair<ITR::vXYZP_Itr, ITR::vXYZP_Itr> PITR;
	typedef std::map<double, PITR> tIMAP;
	typedef tIMAP::const_iterator imapItr;

	struct DtLabel {
		double mz, intensity, resolution;
		int charge;
	};

private:

	bool isCreateDBFg_;
	tIMAP imap_;
	
public:
//	const bool tryOutput(kome::objects::DataSet& dataSet){return true;}

	// itr MAP Clear
//	void clearItrMap() {imap_.clear();}

//	size_t getImapCnt() const {return imap_.size();}

//	const bool AutoDBSetup (kome::objects::DataSet& dataSet);

	//// get MS1 & has Chromatogram's Spectra Datas
	//const bool convertDataSet (
	//	kome::objects::DataSet& dataSet, 
	//	const ELM::MZRTP& tgt_mzrt, 
	//	ELM::vXYZP& rst_vxyzp
	//);

	//const bool convertDataSet (
	//	kome::objects::DataSet& dataSet, 
	//	const ELM::MZRTP& tgt_mzrt,
	//	const unsigned int msStage,
	//	const CHROM_FLAG ch_flag,
	//	ELM::vXYZP& rst_vxyzp
	//);

	const bool getSpectrumDataPoints(
		kome::objects::DataSet& dataSet, 
		const ELM::MZRTP& tgt_mzrt,
		const unsigned int msStage,
		const CHROM_FLAG ch_flag,
		ELM::vXYZP& rst_vxyzp,
		vPeak3D& rst_Peaks,
		const bool removeIsotopicDataPoints,
		const int minCharge,
		const int maxCharge,
		const double mzTolRatio
	);

private:
	// get Spectra Datas from Mspp DataSet Object
	//const bool onConvertDataSet (
	//	kome::objects::DataSet& dataSet, 
	//	const ELM::MZRTP& tgt_mzrt,
	//	const unsigned int msStage,
	//	const CHROM_FLAG ch_flag,
	//	ELM::vXYZP& rst_vxyzp,
	//	bool needMyDB
	//	);

	//const bool onGetDataLabel (
	//	kome::objects::Spectrum* tmpspec,
	//	const double hmz,
	//	const double lmz,
	//	std::vector<DtLabel>& rstDTLArr
	//);

	inline const bool getDataPointsFromSpectrum(
		kome::objects::Spectrum* tmpspec,
		const double hmz,
		const double lmz,
		std::vector<DtLabel>& rstDTLArr,
		kome::objects::Peaks* peaks
	);

public:
	const bool createXIC (
		RFN::XYZP::vXYZP_& vxyzp, 
//		const ELM::MZRTP& tgt_mzrt, 
		const double lowerMz, const double upperMz,
		const double lowerRt, const double upperRt,
		RFN::XYP::vXYP_& rstXIC
	);

	const bool TradingPost::clearIntensity(
		ELM::vXYZP& vxyzp,
//		const ELM::MZRTP& mzrtp,
		const double lowerMz, const double upperMz,
		const double lowerRt, const double upperRt,
		std::vector<double>& intensities
	);

	// for Created DataBase XYData (X.I.C)
	void insertMeXIC (
		const RFN::PEAK::Peak3DPointValue& p,
		const ELM::vXYP& xicVal,
		ELM::vPKPT& xicPeaks);

	//kome::core::XYData* convertToXYData(const ELM::vXYP& vxyp);

	//void convertFromXYData(kome::core::XYData* xydata, ELM::vXYP& tmpXyp);

private:

	std::vector<std::pair<double, double>> getIsotopicDataPoints(
		std::vector<std::pair<double, double>>& dataPoints,
		const unsigned int lowerCharge,
		const unsigned int higherCharge,
		const double mzTolRate);

};

} // end of ALC namespace
