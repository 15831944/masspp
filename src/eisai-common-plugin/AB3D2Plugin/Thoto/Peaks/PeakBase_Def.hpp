/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Peaks
    @file       PeakBase_Def.hpp
	@brief      Peak の構成定義
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
	@note
		mass++式のピーク構成を構造体化したもの。
****************************************************************/
#pragma once

#include "../Points/XYPoint_Def.hpp"
/* --- TYPEDEF -- RFN::XYP ------------------------------
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
XYP  NG_XYP(-1.0,-1.0)
/ -------------------------------------------------------*/

namespace RFN  {
namespace PEAK {

// struct Peak Values
// Core(Center) Left, Right, Apex
struct PeakPointValues {
	ELM::XYP Cp;    // Peak Center Point (頂点。計算値。実値と変動している可能性有)
	ELM::XYP Lp;    // Peak Left   Point
	ELM::XYP Rp;    // Peak Right  Point
	ELM::XYP Apex;  // Peak Apex   Point (頂点。実値。DataPoint上の実座標が入る)
	double fwhmVal; // Peak's fwhm. (未計算なら -1.0)
	double areaVal; // Peak's Area. (未計算なら -1.0)

	// constructor
	PeakPointValues()
		: Cp(), Lp(), Rp(), Apex(), fwhmVal(-1.0), areaVal(-1.0){}

	//PeakPointValues(const double x, const double intensity) 
	//	: Cp(x, intensity), Lp(), Rp(), Apex(), fwhmVal(-1.0), areaVal(-1.0){}

	PeakPointValues(ELM::XYP& center, ELM::XYP& left, ELM::XYP& right, ELM::XYP& apex) 
		: Cp(center), Lp(left), Rp(right), Apex(apex), fwhmVal(-1.0), areaVal(-1.0){} 

	//PeakPointValues(
	//	const double& cx, const double& cy, const double& lx, const double& ly,
	//	const double& rx, const double& ry, const double& ax, const double& ay)
	//	: Cp(cx,cy), Lp(lx,ly), Rp(rx,ry), Apex(ax,ay), fwhmVal(-1.0), areaVal(-1.0) {}

//	PeakPointValues()
//		: Cp(ELM::defXYP), Lp(RFN::XYP::NG_XYP), Rp(RFN::XYP::NG_XYP), Apex(RFN::XYP::NG_XYP) {}
//	PeakPointValues(const double x, const double intensity) 
//		: Cp(x, intensity), Lp(RFN::XYP::NG_XYP), Rp(RFN::XYP::NG_XYP), Apex(RFN::XYP::NG_XYP) {}
//	PeakPointValues(ELM::XYP& center, ELM::XYP& left, ELM::XYP& right, ELM::XYP& apex) 
//		: Cp(center), Lp(left), Rp(right), Apex(apex) {} 
//	PeakPointValues(
//		const double& cx, const double& cy, const double& lx, const double& ly,
//		const double& rx, const double& ry,const double& ax, const double& ay) 
//		: Cp(cx,cy), Lp(lx,ly), Rp(rx,ry), Apex(ax,ay){}
};

typedef PeakPointValues PKPT_;
typedef std::vector<PKPT_> vPKPT_;

} // end of namespace PEAK
} // end of namespace RFN

/* ------ TYPEDEF Infomations --------------------------- /
-- RFN::PEAKS --
ELM  PKPT      --- struct {ELM::XYP Cp(Center), Lp(Left), Rp(Right), Apex(頂点座標)}
     vPKPT     --- std::vector<PKPT>
ITR  vPKPT_Itr --- vPKPT::iterator
SPTR vPKPT_Ptr --- vPKPT*
/ -------------------------------------------------------*/
// Typedef For Point Objects ELEMENT
namespace ELM {
	typedef RFN::PEAK::PKPT_ PKPT;
	typedef std::vector<PKPT> vPKPT;
}

// Typedef For Iterator
namespace ITR {
	typedef RFN::PEAK::vPKPT_::iterator vPKPT_Itr;
}

// Typedef For Pointer　(Shared Pointer)
namespace SPTR {
	typedef RFN::PEAK::vPKPT_* vPKPT_Ptr;
}