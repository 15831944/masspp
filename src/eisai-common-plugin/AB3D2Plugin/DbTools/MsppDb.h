///*********************************************************/
///*!
// @addtoGroup AB3D
// @file MsppDB.h
// @brief AB3D Peak Position Main DB Ctrl
// 
// @date 2013.07.01
// @author M.Fukuda
// @varsion 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2014
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
//    Mass++のDBインタフェース実装部分
// */
///*********************************************************/
//#pragma once
//#include "proxyDBClass.h"
//
//#ifndef __EMS__
//
//namespace AB3D {
//
///*!--------------------------------------------------------
//    @name   MsppDB
//    @brief  AB3Dの結果をMass++ DBに書き込む
//    @date   2013.07.01
//	@par    DbProxyClass から継承。
//-----------------------------------------------------------*/
//	class MsppDB : public DbProxyClass {
//
//	public:
//		MsppDB();
//	private:
//		// Sampleを呼び出した情報を一時保管
//		kome::ident::SampleReadInfo sampleInfo_;
//		// 現在オープン中のサンプルのポインタ
//		kome::objects::Sample* sample_;
//	private:
//
//		/*!
//		 * @brief  自身の初期化を行う
//		 * @retval 失敗時は False 
//		 */
//		const bool onInitMe();
//
//		/*!
//		 * @brief  指定IDのサンプルデータを取得の実装部
//		 * @param[in]   サンプルID
//		 * @param[out]  サンプルのDataSet
//		 * @retval 失敗時は False 
//		 */
//		const bool onOpenTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet);
//
//		/*!
//		 * @brief  DBで開いたアクティブサンプルを閉じる実装部
//		 */
//		void onCloseActiveSample();
//
//		/*!
//		 * @brief  アクティブなMatrixIDを取得する
//		 * @param[out]  MatrixID
//		 * @retval 失敗時は False 
//		 */
//		const bool onGetMatrixId(
//			int& refMatrixId);
//
//		/*!
//		 * @brief  指定MatrixIDのサンプルID群を取得する
//		 * @param[in]   MatrixID
//		 * @param[out]  サンプルのID群
//		 * @retval 失敗時は False 
//		 */
//		const bool onGetSampleIds(
//			const int matrixId,
//			std::vector<int>& refSampleIds);
//
//		/*!
//		 * @brief  Bufferをテーブルに書き込む
//		 * @param[in] 書き込むBuffer
//		 * @retval    書き込んだBufferID。失敗時は -1
//		 */
//		const int onSetXYBuffer(
//			kome::core::Buffer* buffer);
//
//		/*!
//		 * @brief  Peak情報をテーブルに書き込む
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @retval    書き込んだPeakID。失敗時は -1
//		 */
//		const int onSetPeak(
//			const int mtId,
//			const double mz,
//			const double rt,
//			const int charge
//			);
//
//		/*!
//		 * @brief  Peakの詳細をテーブルに書き込む
//		 * @param[in] PeakID
//		 * @param[in] SampleID
//		 * @param[in] BufferのID
//		 * @param[in] Peakの詳細情報構造体
//		 * @retval    失敗時は False
//		 */
//		const bool onSetValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval
//		);
//		
//		/*!
//		 * @brief コミット
//		 */
//		void onCommitMe();
//	};
//}
//#endif
