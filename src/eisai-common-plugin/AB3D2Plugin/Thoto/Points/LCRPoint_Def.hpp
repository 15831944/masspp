/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       LRCPoint_Def.hpp
	@brief      Left - Center - Right の構成定義
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

namespace RFN {
namespace LCR {

// Left - Right 等の区間空間系
struct LCRP_ {
	double l; // left Value 
	double c; // center Value
	double r; // right Value

	// constructor
	LCRP_(): l(-1.0),c(-1.0),r(-1.0){}

	explicit LCRP_(const double& x) : l(x), c(x), r(x){}

	LCRP_(const double& xl, const double& xc, const double& xr) : l(xl), c(xc), r(xr){}

	LCRP_(const LCRP_& obj) : l(obj.l), c(obj.c), r(obj.r){}
};

static const LCRP_ NG_LCRP(-1.0);

} // end of namesapce
}

/* ------ TYPEDEF Infomations --------------------------- /
NG_LCRP(-1.0);
-- RFN::LCR --
ELM  LCRP      --- struct{double l(left), c(center), r(right)
/ -------------------------------------------------------*/
// Typedef For Base Point Objects
namespace ELM {
	// @type : L-C-R　３点保持オブジェクト
	// @value: struct{double Left, Center, Right}
	typedef RFN::LCR::LCRP_ LCRP;
}
