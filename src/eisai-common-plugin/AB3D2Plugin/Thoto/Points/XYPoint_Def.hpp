/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYPoint_Def.hpp
	@brief      X,Y の構成定義
	@date       2012.12.03
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once
#include <vector>
#include <utility> // ! 新規追加。エラーが出るなら排除
//
// XY_Pointの基本定義部分
//
namespace RFN {
namespace XYP {

// XYPointの構造体(Pair) M/Z or RT & Intensity.
typedef std::pair<double, double> XYP_;

// XYPoint Vector
typedef std::vector<XYP_> vXYP_;

// NGな構造体。
const XYP_ NG_XYP(-1.0, -1.0);

}// end of namespace
}

/* ------ TYPEDEF Infomations --------------------------- /
const XYP_ NG_XYP(-1.0,-1.0);

-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
/ -------------------------------------------------------*/
// Typedef For Point Objects ELEMENT
namespace ELM {
	// @type : Base Point Objects
	// @value: std::pair<(M/Z or RT), Intensity>
	typedef RFN::XYP::XYP_ XYP;

	// @type : Base Point Objects
	// @value: std::vector<XYP>
	// @param: XYP : std::pair<(M/Z or RT), Intensity>
	typedef RFN::XYP::vXYP_ vXYP;

	//static const XYP defXYP(-1.0,-1.0);
}

// Typedef For Iterator
namespace ITR {
	// vector<XYP>::iterator
	typedef RFN::XYP::vXYP_::iterator vXYP_Itr;
	// vector<XYP>::const_iterator
	typedef RFN::XYP::vXYP_::const_iterator vXYP_cItr;
}

//#define __USE_SPTR__

// Typedef For Pointer　(Pointer)
namespace SPTR {
	typedef RFN::XYP::vXYP_* vXYP_sp;
}
