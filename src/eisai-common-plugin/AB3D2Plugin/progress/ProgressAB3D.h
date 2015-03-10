/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAB3D.h
 @brief progress adapter(for AB3D)
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#pragma once
#include "ProgressAdapter.h"

class AB3DProg : public ProgressAdapter {

public :
	AB3DProg(
		kome::core::Progress* _progress,
		const int lastcount = 30);

	void onInitMe();

	const bool onNextStep();

	void onCloseMe();
};
