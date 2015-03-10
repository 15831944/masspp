/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAdpt.cpp
 @brief progress adapter
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    プログレスを制御する外側のベースです。
 */
/*********************************************************/
#include "../stdafx.h"
#include "ProgressAdapter.h"

// memo
// progressについて

// progressは　setRangeで範囲を決めて、
// setPositionで位置を動かし、
// プログレスの為のカウントを用意し、
// setStatusでメッセージを変える必要が有ります。
// 手続きが煩雑で、コードの可読性が下がります。
// なので、クラスとしてまとめてあります。

// constructor
ProgressAdapter::ProgressAdapter(
	kome::core::Progress* _progress,
	const int lastCount
	) : progress_(_progress)
	, firstCount_(0)
	, lastCount_(lastCount)
{
//	initMe();
}

// constrctor
ProgressAdapter::ProgressAdapter(
	kome::core::Progress* _progress,
	const int firstCount,
	const int lastCount
	) : progress_(_progress)
	, firstCount_(firstCount)
	, lastCount_(lastCount)
{
//	initMe();
}

// 初期化。継承クラス側で呼び出す。
void ProgressAdapter::initMe()
{
	if (progress_ != nullptr) {
		onInitMe();
	}
}

// initで呼び出される。
void ProgressAdapter::onInitMe() 
{
	onShowStatus("");
}

// 固定メッセージでプログレスを進める時はこちらを呼ぶ
const bool ProgressAdapter::nextStep()
{
	if(progress_ == nullptr) {
		return false;
	}

	nowCount_++;
	if (lastCount_ < nowCount_) {
		return false;
	}
	progress_->setPosition(nowCount_);
	return onNextStep();
}

// プログレスを一歩進める。
// メッセージを変更するときはこちらを使う
const bool ProgressAdapter::nextStep(
	const std::string& msg
){
	if(progress_ == nullptr) {
		return false;
	}

	nowCount_++;

	if (lastCount_ < nowCount_) {
		return false;
	}
	
	progress_->setPosition(nowCount_);
	onShowStatus(msg);
	return true;
}


const bool ProgressAdapter::onNextStep()
{
	// 進行状況を表すメッセージを
	onShowStatus("");
	return true;
}

// progress をクローズ
void ProgressAdapter::closeMe()
{
	if (progress_ != nullptr) {
		onCloseMe();
		progress_->fill();
	}
}

void ProgressAdapter::onCloseMe()
{
	// 終了時のメッセージ
	onShowStatus("");
}

void ProgressAdapter::onShowStatus(const std::string& msg)
{
	if (progress_ != nullptr) {
		progress_->setStatus(msg.c_str());
	}
}
