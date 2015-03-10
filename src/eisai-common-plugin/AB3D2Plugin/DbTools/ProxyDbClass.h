///*********************************************************/
///*!
// @addtoGroup AB3D
// @file proxyDBClass.h
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
//    Mass++とEMSでDBの実装が異なるため、
//	proxyでインスタンスを合わせておき、
//	継承先をMass++とEMSで切り替えて整合性をあわせる。
// */
///*********************************************************/
//#pragma once
//
//// 前方宣言
//namespace AB3D {
//	class DbAccess;
//	struct TmpMXVal;
//	struct ElmPoint;
//}
//
//namespace AB3D {
//
///*!--------------------------------------------------------
//    @name   DbProxyClass
//    @brief  Data Base Proxy Class
//    @date   2013.07.01
//    @par    異なる実装に対応するための共通インターフェースのクラス
//
//	下記について継承時に本体が必要
//		+ onInitMe
//		+ onCommitMe
//		+ onCloseActiveSample
//		+ onOpenTargtIdSample
//		+ onGetMatrixId
//		+ onGetSampleIds
//		+ onSetXYBuffer
//		+ onSetPeak
//		+ onSetValues
//
//-----------------------------------------------------------*/
//	class DbProxyClass {
//	
//	public:
//		// constructor
//		DbProxyClass();
//		// destructor
//		~DbProxyClass();
//
//	private:
//		// Uncopyable
//		DbProxyClass(const DbProxyClass&);
//		DbProxyClass& operator=(const DbProxyClass&);
//
//	protected:
//		// DB Accesser を作成して保持する
//		std::unique_ptr<DbAccess> ptr_;
//		// 現在作成中のMatrix IDを保持する
//		int matrixID_;
//
//		kome::ident::PeakPositions* peakpos_;
//
//	public:
//		/*!
//		 * @brief  自身の初期化を行う
//		 * @retval 失敗時は False 
//		 * @note   onInitMeを内部的に呼ぶ
//		 */
//		const bool initMe();
//
//		/*!
//		 * @brief  DBが使用できるかチェックする
//		 * @param[out]  アクティブなMatrixID
//		 * @param[out]  その類するサンプルのID群
//		 * @retval 失敗時は False 
//		 * @note + onGetMatrixId
//		         + onGetSampleIds
//		 */
//		const bool isReadyDB(
//			int& refMatrixId,
//			std::vector<int>& refSampleIds);
//
//		/*!
//		 * @brief  指定IDのサンプルデータを取得
//		 * @param[in]   サンプルID
//		 * @param[out]  サンプルのDataSet
//		 * @retval 失敗時は False 
//		 * @note   + onOpenTargtIdSample
//		 */
//		const bool openTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet);
//
//		/*!
//		 * @brief  DBで開いたアクティブサンプルを閉じる
//		 * @note   + onCloseActiveSample
//		 */
//		void closeActiveSample();
//
//		/*!
//		 * @brief  Bufferをテーブルに書き込む
//		 * @param[in] 書き込むBuffer
//		 * @retval    書き込んだBufferID。失敗時は -1
//		 * @note   + onSetXYBuffer
//		 */
//		const int setXYBuffer(
//			kome::core::Buffer* buffer);
//
//		/*!
//		 * @brief  Peak情報をテーブルに書き込む
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @param[in] Value
//		 * @retval    書き込んだPeakID。失敗時は -1
//		 * @note   + onSetPeak
//		 */
//		const int setPeak(
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
//		 * @note   + onSetValues
//		 */
//		const bool setValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval);
//		
//		/*!
//		 * @brief  終了処理
//		 */
//		void closeMe();
//
//		/*!
//		 * @brief コミット
//		 * @note + onCommitMe
//		 */
//		void commitMe();
//
//	protected:
//		// 以降、インスタンスは共通しているが、
//		// 中身は個別に書き換えたほうが良い部分。
//
//		/*!
//		 * @brief  自身の初期化を行う
//		 * @retval 失敗時は False 
//		 */
//		virtual const bool onInitMe();
//
//		/*!
//		 * @brief  指定IDのサンプルデータを取得の実装部
//		 * @param[in]   サンプルID
//		 * @param[out]  サンプルのDataSet
//		 * @retval 失敗時は False 
//		 */
//		virtual const bool onOpenTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet) = 0;
//
//		/*!
//		 * @brief  DBで開いたアクティブサンプルを閉じる実装部
//		 */
//		virtual void onCloseActiveSample() = 0;
//
//		/*!
//		 * @brief  アクティブなMatrixIDを取得する
//		 * @param[out]  MatrixID
//		 * @retval 失敗時は False 
//		 */
//		virtual const bool onGetMatrixId(
//			int& refMatrixId) = 0;
//
//		/*!
//		 * @brief  指定MatrixIDのサンプルID群を取得する
//		 * @param[in]   MatrixID
//		 * @param[out]  サンプルのID群
//		 * @retval 失敗時は False 
//		 */
//		virtual const bool onGetSampleIds(
//			const int matrixId,
//			std::vector<int>& refSampleIds) = 0;
//
//		/*!
//		 * @brief  Bufferをテーブルに書き込む
//		 * @param[in] 書き込むBuffer
//		 * @retval    書き込んだBufferID。失敗時は -1
//		 */
//		virtual const int onSetXYBuffer(
//			kome::core::Buffer* buffer) = 0;
//
//		/*!
//		 * @brief  Peak情報をテーブルに書き込む
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @retval    書き込んだPeakID。失敗時は -1
//		 */
//		virtual const int onSetPeak(
//			const int mtId,
//			const double mz,
//			const double rt,
//			const int charge
//			) = 0;
//
//		/*!
//		 * @brief  Peakの詳細をテーブルに書き込む
//		 * @param[in] PeakID
//		 * @param[in] SampleID
//		 * @param[in] BufferのID
//		 * @param[in] Peakの詳細情報構造体
//		 * @retval    失敗時は False
//		 */
//		virtual const bool onSetValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval) = 0;
//
//		/*!
//		 * @brief コミット
//		 */
//		virtual void onCommitMe() = 0;
//	};
//}
//
//
