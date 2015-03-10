/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_included headers
    @file       Def_Points.hpp
	@brief      Def_Pointsの各ヘッダを纏めてインクルードするためのもの
	@date       2012.12.19
	@author     M.Fukuda
	@version    1.0.0
****************************************************************/

#pragma once
#include "Points\LRPoint_Def.hpp"
#include "Points\LCRPoint_Def.hpp"
#include "Points\XYZPoint_Def.hpp"

//情報整理の為、一度コメントアウト 2014/01/23
//
//#include "Points\LRPoint_Def.hpp"
///* ------ TYPEDEF Infomations --------------------------- /
//const RFN::LR::LRP_ NG_LRP(-1.0,-1.0);
//-- RFN::LR --
//ELM  LRP        --- std::pair<double,double>
//     MZRTP      --- struct{LRP mz, LRP rt}
/// -------------------------------------------------------*/
//
//#include "Points\LCRPoint_Def.hpp"
///* ------ TYPEDEF Infomations --------------------------- /
//NG_LCRP(-1.0);
//-- RFN::LCR --
//ELM  LCRP      --- struct{double l(left), c(center), r(right)
/// -------------------------------------------------------*/
//
//#include "Points\XYZPoint_Def.hpp"
//// (included XYPoint_def)
///* ------ TYPEDEF Infomations --------------------------- /
//const XYP_ NG_XYP(-1.0,-1.0);
//
//-- RFN::XYP --
//ELM  XYP       --- std::pair<double,double>
//     vXYP      --- std::vector<XYP>
//ITR  vXYP_Itr  --- vXYP::iterator
//	 vXYP_cItr --- vXYP::const_iterator
//SPTR vXYP_sp   --- vXYP*
//
//-- RFN::XYZP --
//ELM  XYZP      --- struct {double z, SPTR::vXYZ_sp}
//     vXYZP     --- std::vector<XYZPt>
//ITR  vXYZP_Itr --- vXYZP::iterator
/// -------------------------------------------------------*/
