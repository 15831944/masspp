/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAdpt.h
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
#pragma once

class ProgressAdapter {
public:
	ProgressAdapter(
		kome::core::Progress* _progress,
		const int lastCount);

	ProgressAdapter(
		kome::core::Progress* _progress,
		const int firstCount,
		const int lastCount);

	~ProgressAdapter(){}

private:
	// Uncopyable
	ProgressAdapter(const ProgressAdapter&);
	ProgressAdapter& operator=(const ProgressAdapter&);

protected:
	// keep original progress.
	kome::core::Progress* progress_;

	const int firstCount_;
	const int lastCount_;
	int nowCount_;

public:
	void initMe();
	const bool nextStep();
	const bool nextStep(const std::string& msg);
	void closeMe();

protected:
	virtual void onInitMe();
	virtual const bool onNextStep();
	virtual void onCloseMe();

	void onShowStatus(const std::string& msg);

private:
	const bool isShowNextStep();
};
