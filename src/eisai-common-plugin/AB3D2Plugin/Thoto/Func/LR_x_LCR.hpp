/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Functions
    @file       LR_x_LCR
	@brief      LR x LCR ÇÃç¨çáä÷êîÇíËã`
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include "../Points/LCRPoint.hpp"
#include "../Points/LRPoint.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
-- RFN::LR --
NG_LRP(-1.0,-1.0);
ELM  LRP        --- std::pair<double,double>
     MZRTP      --- struct{LRP mz, LRP rt}
-- RFN::LCR --
NG_LCRP(-1.0);
ELM  LCRP      --- struct{double l(left), c(center), r(right)
/ -------------------------------------------------------*/

namespace RFN {
namespace FSN {

//// $$$ égópâ”èäñ≥Çµ#2014.01.23
//// LRP to LCRP (center is (L+R)/2 )
//inline ELM::LCRP toLCRP(const ELM::LRP& tgt) {
//	double dmyCnt = (tgt.first+tgt.second) * 0.5;
//	return ELM::LCRP(tgt.first, dmyCnt, tgt.second);
//}

// LCRP to LRP
inline ELM::LRP toLRP(const ELM::LCRP& tgt) {
	return ELM::LRP(tgt.l, tgt.r);
}

// LCRP * 2 (MZ,RT) to MZRT
inline void setMZRT(const ELM::LCRP& mzp, const ELM::LCRP& rtp, ELM::MZRTP& tgt) {
	tgt.mz = toLRP(mzp);
	tgt.rt = toLRP(rtp);
}

inline ELM::MZRTP toMZRTP(const ELM::LCRP& mzp, const ELM::LCRP& rtp) {
	return ELM::MZRTP(toLRP(mzp),toLRP(rtp));
}

} // end of namespace LCRP
}