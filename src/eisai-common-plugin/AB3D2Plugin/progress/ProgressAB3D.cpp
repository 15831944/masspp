/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAB3D.cpp
 @brief progress adapter (for AB3D)
 
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
#include "progressAB3D.h"

// AB3Dの進行状況のプログレスです。
// Detectorの進行状況メッセージ、頻度等は
// onNextStep内を適宜書き換えてください。
// 頻度が多過ぎるとこれ自体がボトルネックになります。
// 試験的に、30カウントで用意してあります。
// nextstepを呼ぶたびに、１カウントずつ増えます。
// PeakDetector内で25カウント
// それ以外の個別のフェーズが5回呼ばれています。

// Close(Fill)を呼ぶと不足分は補完してクローズしてくれます。
// 範囲をオーバーすると　落ちるので扱いに注意して下さい。

AB3DProg::AB3DProg(
	kome::core::Progress* _progress,
	const int lastcount
	) : ProgressAdapter(_progress, lastcount)
{
	this->initMe();
}

void AB3DProg::onInitMe()
{
	nowCount_ = firstCount_;
	// Progressの範囲です。setPositionでゲージ位置が変わります。
	progress_->setRange(firstCount_, lastCount_);
	progress_->setPosition(firstCount_);
}

const bool AB3DProg::onNextStep()
{
	std::string msg = FMT("AB3D Peak Detect.. %02d", nowCount_ * 4);
	msg += "%";
	onShowStatus(msg);
	return true;
}

void AB3DProg::onCloseMe()
{
}
