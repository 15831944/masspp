/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Peaks
    @file       Peak3DBase_Def.hpp
	@brief      3DPeak の構成定義
	@date       2012.12.13
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include <vector>
#include "../Points/LCRPoint_Def.hpp"

namespace RFN {
namespace PEAK{

// 3次元ピークの構造体。MZ,RTの範囲、Intensity,Area等の情報を持つ
struct Peak3DPointValue {
	bool sflag;           // if true, using. false, delete.
//	RFN::LCR::LCRP_ mz;   // MZ Point(As Range)
//	RFN::LCR::LCRP_ rt;   // RT Point(As Range)
	ELM::LCRP mz;   // MZ Point(As Range)
	ELM::LCRP rt;   // RT Point(As Range)
	double intensity;     // Peaks intensity
	double area;          // Peaks Area
	double score;         // Peaks Fit Score
	double logintensity;  // Log Intensity (Calced)
	double fwhm;		  // FWHM
	double numOfDataPoints;	// Number of data points

	//constructor
	Peak3DPointValue() 
		: intensity(-1.0), area(-1.0), score(-1.0), mz(-1.0), rt(-1.0), sflag(true), logintensity(-1.0), fwhm(-1.0){}

	Peak3DPointValue(const double& intval, const double& mzL, const double& mzC, const double& mzR, const double& rtval) 
		: intensity(intval), mz(mzL, mzC, mzR), rt(rtval), area(-1.0), score(-1.0), sflag(true), logintensity(-1.0), fwhm(-1.0) {}

	Peak3DPointValue(
		const double& intVal, const ELM::LCRP& _mz, const ELM::LCRP& _rt,
		const double& areaVal, const double& scoreVal, const double& logIntVal, const double& fwhmVal, const int& numOfDataPointsVal)
		: intensity(intVal), area(areaVal), score(scoreVal)
		, logintensity(logIntVal), mz(_mz), rt(_rt), sflag(true), fwhm(fwhmVal), numOfDataPoints(numOfDataPointsVal){}

	~Peak3DPointValue(){}

	Peak3DPointValue(const Peak3DPointValue& obj){
		this->mz = obj.mz;
		this->rt = obj.rt;
	}

	Peak3DPointValue &operator=(const Peak3DPointValue &o){
	this->mz = o.mz;
	this->rt = o.rt;
	return(*this);
	}

};

typedef Peak3DPointValue Peak3D_;

} // end of namespace PEAK
}

// Typedef For Point Objects ELEMENT
namespace BS {
	typedef RFN::PEAK::Peak3D_ Peak3D;
	typedef std::vector<Peak3D> vPeak3D;
}

// Typedef For Point Objects ELEMENT
namespace ELM {
	typedef RFN::PEAK::Peak3D_ Peak3D;
	typedef ELM::Peak3D* PK3D_ptr;
	typedef std::vector<ELM::PK3D_ptr> vPeak3D;
}

// Typedef For Iterator
namespace ITR {
	typedef ELM::vPeak3D::iterator vPeak3D_Itr;
}

// Typedef For Pointer　(Shared Pointer)
namespace SPTR {
	typedef ELM::vPeak3D* vPeak3D_Ptr;
}