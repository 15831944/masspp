/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressMatrix.h
 @brief progress adapter
 
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
#include "progressAdapter.h"

class MatrixProg : public ProgressAdapter {

public :
	MatrixProg(
	kome::core::Progress* _progress,
	const int lastCount);

	kome::core::Progress* getSubProgress();

private:
	void onInitMe();
	const bool onNextStep();
	void onCloseMe();
};
