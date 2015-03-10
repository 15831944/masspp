///****************************************************************
//  Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
//-----------------------------------------------------------------
//    @addtogroup Mspp PluginCall helper headers
//    @file       PluginCallHelper.hpp
//	@brief      PluginCallされた後の手続きを纏めた関数群
//	@date       2012.12.13
//	@author     M.Fukuda
//	@version    1.0.0
//****************************************************************/
//#pragma once
//
//// stdafx_mininumSet内に下記インクルードガードがある
//#ifndef __NO_NEED_EMS_MINIMUN_STFAFX_H__
//#include "./Mspp/stdafx_mininumSet.h"
//#endif
//
///* -- How to Use --
// * 所謂ファクトリー関数（人力）型
//Ex)
//#define __NO_NEED_EMS_MINIMUN_STFAFX_H__
//#define __CALLTYPE_PEAKPICKING__
//#include "../Z_Horus_Plugin/hpp/PluginToolHelper.hpp"
//
// * 1. EMS/Mass++のライブラリが衝突する場合は前に
//      #define __NO_NEED_EMS_MINIMUN_STFAFX_H__
//	  と書く
// * 2. 呼び出したいコールタイプを defineで宣言する
//      #define __CALLTYPE_*CALL*TYPE*NAME*__
//      対応するヘッダのみが呼ばれる。
// * 3. これをインクルードする。
// * 4. RFN::WRAP::xxxxxxの様に呼び出す。
// */
//
//#ifdef __CALLTYPE_PEAKPICKING__
//#include "./hpp/Peak_Picking.hpp"
//#endif
//
#ifdef __CALLTYPE_2DPEAKPICKING__
#include "./hpp/Check2DPeakPicking.hpp"
#endif
//
//#ifdef __CALLTYPE_2DPEAKPOSITION__
////#include "./hpp/CheckPeakPositions.hpp"
//#endif