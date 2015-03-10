/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYZPoint_Def.hpp
	@brief      <X,Y> * N ,Z の構成定義
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
	@note
		要素がポインタで格納されています。		
		12/12 デストラクタにポインタ破棄を持たせてみました。
		オブジェクトのコピーの際は注意して下さい。
****************************************************************/
#pragma once

#include "XYPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
NG   NG_XYP(-1.0,-1.0);
/ -------------------------------------------------------*/

namespace RFN {
namespace XYZP {

// XY+Zの構造体
struct XYZPt_ {
	double z;            // Z (RT / M/Z)
	SPTR::vXYP_sp vxyp;  // XYArray(M/Z or RT & Intensity)

	// Shared Pointerは使わないのでこのコンストラクタは破棄する
	// 2014.01.23
//	// constructor
//#ifdef __USE_SPTR__
//	XYZPt_(const double& _z) : z(_z), vxyp(std::make_shared<ELM::vXYP>()) {}
//	XYZPt_(const double& _z, const ELM::vXYP& _vxyp) 
//		: z(_z), vxyp(std::make_shared<ELM::vXYP>(_vxyp)){}
//#else
//	XYZPt_(const double& _z) : z(_z), vxyp(new ELM::vXYP()) {

	XYZPt_(const double& _z) : z(_z), vxyp(nullptr) {
	}

	XYZPt_(const double& _z, const ELM::vXYP& _vxyp) 
		: z(_z), vxyp(new ELM::vXYP(_vxyp)){
	}
//	~XYZPt_(){ if(vxyp != nullptr) delete vxyp; }
};

typedef XYZPt_ XYZP_;
typedef std::vector<XYZP_> vXYZP_;

} // end of namespace XYZP
} // end of namespace RFN

/* ------ TYPEDEF Infomations --------------------------- /
-- RFN::XYZP --
ELM  XYZP      --- struct {double z, SPTR::vXYZ_sp}
     vXYZP     --- std::vector<XYZPt>
ITR  vXYZP_Itr --- vXYZP::iterator
/ -------------------------------------------------------*/
// Typedef For Point Objects ELEMENT
namespace ELM {
	// @type  : XYData Points & Z Point.
	// @value : std::vector<std::pair<double,double>(Pointer), Z(RT or M/Z)
	typedef RFN::XYZP::XYZP_ XYZP;
	
	// @type : XYZData Point Objects Vector
	// @value: std::vector<XYZP>
	// @param: XYZP : struct(has *vxyp, double(RT)
	typedef std::vector<XYZP> vXYZP;
}

// Typedef For Iterator
namespace ITR {
	typedef RFN::XYZP::vXYZP_::iterator vXYZP_Itr;
}
