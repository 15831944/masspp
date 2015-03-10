/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       LRPoint_Def.hpp
	@brief      Left - Right の構成定義
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include <utility> // std::pair

namespace RFN {
namespace LR {

// Left - Right 等の区間空間系
typedef std::pair<double, double> LRP_;

const LRP_ NG_LRP(-1.0, -1.0);

// MZ & RT の　４点区間を補完。
struct MZRTPt {
	LRP_ mz;  // MZ Range
	LRP_ rt;  // RT Range

	// constructor
	MZRTPt(void) : mz(NG_LRP), rt(NG_LRP){}
	
	MZRTPt(const double& lmz, const double& rmz, const double& lrt, const double& rrt)
		: mz(lmz, rmz), rt(lrt, rrt){}

	MZRTPt(const LRP_& _mz, const LRP_& _rt)
		: mz(_mz), rt(_rt){}

	// copy const
	MZRTPt(const MZRTPt& obj) {
		mz = obj.mz;
		rt = obj.rt;
	}

	MZRTPt& operator=(const MZRTPt &obj) {
		if(this != &obj) {
			this->mz = obj.mz;
			this->rt = obj.rt;
		}
		return *this;
	}

};
typedef MZRTPt MZRTP_;

} // end of namespace LR
} // end of namespace RFN

/* ------ TYPEDEF Infomations --------------------------- /
const RFN::LR::LRP_ NG_LRP(-1.0,-1.0);
-- RFN::LR --
ELM  LRP        --- std::pair<double,double>
     MZRTP      --- struct{LRP mz, LRP rt}
/ -------------------------------------------------------*/
// Typedef For Point Objects ELEMENT
namespace ELM {
	typedef RFN::LR::LRP_ LRP;
	typedef RFN::LR::MZRTP_ MZRTP;
} // end of namespace ELM
