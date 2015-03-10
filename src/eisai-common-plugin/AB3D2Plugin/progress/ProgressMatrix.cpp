/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressMatrix.cpp
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

#include "../stdafx.h"
#include "progressMatrix.h"

// peak position (Matrix生成中)に　呼ばれるプログレスです。
// 子供のプロセスを作成するので、少し扱いが異なります。

MatrixProg::MatrixProg(
	kome::core::Progress* _progress,
	const int lastCount
) : ProgressAdapter(_progress,lastCount)
{
	this->initMe();
	nowCount_ = 0;
}


kome::core::Progress* MatrixProg::getSubProgress()
{
	if (progress_ == nullptr) {
		return nullptr;
	} else {
		kome::core::Progress* subprog 
			= progress_->getSubProgress(nowCount_);
		nowCount_++;
		return subprog;
	}
}


void MatrixProg::onInitMe()
{
	if (progress_ != nullptr) {
		progress_->createSubProgresses(lastCount_);
	}
}
	
const bool MatrixProg::onNextStep()
{
	onShowStatus(FMT("Do New AB3D .. [%003d / %003d]", nowCount_, lastCount_));
	return true;
}

void MatrixProg::onCloseMe()
{
	onShowStatus("Create Matrix");
}
