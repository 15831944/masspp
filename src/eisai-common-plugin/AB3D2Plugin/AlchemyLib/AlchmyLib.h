/**********************************************************
Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
    @addtogroup Mass++ Plugins Support Lib
	@file       AlchemyLib.h
	@brief      AlchemyLib 共通のヘッダ。
	@date       2012.12.06
	@author     M.Fukuda
	@version    1.0.0
    @note       余計な定義は入れない。
**********************************************************/
#pragma once


namespace ALC {
	enum CHROM_FLAG {
		NO_USE = 0,         // Chrom 有無の判定なし 
		HAS_CHROM = 1,      // Chrom を持つ
		NOT_HAS_CHROM = -1, // Chrom を持たない（通常は使わない）
		// -------------------
		CHROM_FLAG_NUM,
	};
} // end of namespace ALC
