///*********************************************************/
///*!
// @addtoGroup AB3D
// @file EMSDB.h
// @brief AB3D Peak Position Main DB Ctrl
// 
// @date 2013.07.01
// @author M.Fukuda
// @varsion 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2013 
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
//    EMSのDBインタフェース実装部分
// */
///*********************************************************/
//#pragma once
//#include "proxyDBClass.h"
//
//#ifdef __EMS__
//namespace AB3D {
//
//	class EmsDB : public DbProxyClass {
//	public:
//		EmsDB();
//	private:
//		std::unique_ptr<kome::db::Statement> stmt_peak_;
//		std::unique_ptr<kome::db::Statement> stmt_mxint_;
//		std::unique_ptr<kome::db::Statement> stmt_prof_;
//		std::unique_ptr<kome::db::Statement> stmt_xyd_;
//
//		kome::quant::BaseQuantPeaksManager::GroupInfo groupInfo_;
//		kome::objects::DataGroupNode* cgroup_;
//	private:
//		
//		// 以降、インスタンスは共通しているが、
//		// 中身は個別に書き換えたほうが良い部分。
//		const bool onInitMe();
//
//		void onCommitMe();
//
//		void onCloseActiveSample();
//
//		const bool onOpenTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet);
//
//		const bool onGetMatrixId(int& refMatrixId);
//
//		const bool onGetSampleIds(
//			const int matrixId,
//			std::vector<int>& refSampleIds);
//
//		const int onSetXYBuffer(
//			kome::core::Buffer* buffer);
//
//		const int onSetPeak(
//			const int mtId,
//			const double mz,
//			const double rt,
//			const int charge);
//
//		const bool onSetValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval
//		);
//
//		// EMS 独自の実装
//		const int onSetEMSValues(
//			const int bufId,
//			const double peak_x,
//			const AB3D::ElmPoint& left,
//			const AB3D::ElmPoint& right);
//
//		const int onSetEMSInts(
//			const int peakId,
//			const int sampleId,
//			const int profileId,
//			const double val);
//
//		// 各ステートメントを作成する準備
//		const bool onCreateStatements();
//		// 各々のすてーとめんと
//		const bool onCreateStmt_Insert_EMSMTXPEAK();
//		const bool onCreateStmt_Insert_EMSMTXINT();
//		const bool onCreateStmt_Insert_EMSPROFILE();
//		const bool onCreateStmt_Insert_EMSXYDATA();
//
//
//	};
//}
//#endif
