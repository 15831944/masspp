/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DbController.h
 @brief AB3D Peak Position DB Controller
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    AB3D Positionから呼ばれて、DBの一連の操作を代理する。
 */
/*********************************************************/
#pragma once

//前方宣言
namespace kome {
	namespace core{
		class Buffer;
	}
	namespace objects{
		class DataSet;
		class DataGroupNode;
	}
	namespace db{
		class Statement;
	}
}

namespace AB3D {
	class TmpTblCtrl;
	class DbProxyClass;
	struct TmpPeakVal;
}

namespace AB3D {
/*!--------------------------------------------------------
    @name   DbController
    @brief  Data Base を AB3D からコントロールする
    @date   2013.07.01
    @par    AB3D本体から呼ばれる
            Matrixに設定されたサンプルの管理(Next等)を行う。
-----------------------------------------------------------*/
	class DbController {

	public:
		// constructor
		DbController();
		// destructor
		~DbController();
		
		// Uncopyable
		DbController(const DbController&);
		DbController& operator=(const DbController&);

	private:
		// Temp Table 操作のオブジェクト
		std::unique_ptr<TmpTblCtrl> tblPtr_;

		// Table 操作のオブジェクト(Proxy)
//		std::unique_ptr<DbProxyClass> pxyPtr_;

		// 使用中のMatrixのID
		int matrixId_;
		// 現在計算中のサンプルのID
		int nowSampleId_;
		//// サンプル全体の経過数
		//unsigned int nowCount_;
		//// サンプルの配列(ID群)
		//std::vector<int>sampleIds_;

	public:
		/*!
		 * @brief  DBがセットアップできるか、状態をチェックする
		 * @retval DBが使えない場合はFalseを返す
		 */
		//const bool checkDBStatus();

		/*!
		 * @brief  作業用のTempTableを作成する
		 * @retval 作成時にエラーを吐いたときにFalseを返す
		 */
		const bool setupTempTables();

		/*!
		 * @brief  Temp Tableのステートメントをセットアップする
		 * @retval 出来ない場合はFalseを返す
		 */
		const bool setupTempSTMT();

		///*!
		// * @brief  EMS/Mass++ Tableのステートメントをセットアップする
		// * @retval 出来ない場合はFalseを返す
		// * @note
		//      Mass++では本ステートメントは使用しない
		// */
		//const bool setupMsppSTMT();

		//// 抱えているサンプルサイズを返す(Proguressの為に)
		//unsigned int getSampleNumbers() const {
		//	return sampleIds_.size();
		//}

		///*!
		// * @brief  次のサンプルに遷移する
		// * @retval 次が無ければ False.
		// * @note
		//      内部的にポインタを進めて次をアクティブにする
		// */
		//const bool doNextSample();

		void setSampleId(const int spId);

		///*!
		// * @brief      現在のサンプルのXYDataを呼び戻す
		// * @param[out] (内部的に)アクティブなサンプルDataSet
		// */
		//const bool getNowDataGroup(
		//	kome::objects::DataSet& refDataSet);

		///*!
		// * @brief  現在のサンプルを（開いていれば）閉じる
		// */
		//void onCloseActivegroup();

		/*!
		 * @brief      XICをTempTableに書き込む
		 * @param[in]  ターゲットのピーク
		 * @param[in]  XICとしての実データ(XYPoints)
		 * @param[in]  XICのピークの候補。
		 * @retval     TempTableに書き込んだ結果ID(PeakID)が戻る
		 * @note
		      AB3Dから引き渡された時点で、ピークの候補の中に
			  ターゲットのピークがあるかないか判定していないので、
			  ここで判定する必要が有る
		 */
		const int setXICData(
			const std::vector<std::pair<double, double>>& xicXyDatas,
			const double rt,
			const double mz,
			const double value,
			const double peak_top_x,
			const double peak_top_y,
			const double peak_left_x,
			const double peak_left_y,
			const double peak_right_x,
			const double peak_right_y
			);

		/*!
		 * @brief      Chargeの値をアップデートする
		 * @param[in]  対象のPeak ID
		 * @param[in]  Charge
		 */
		void updateCharge(
			const int peakId,
			const int charge);

		/*!
		 * @brief      TempTableから該当データを取得する
		 * @param[out] 一致したTempPeak情報群
		 * @result     取得の成否
		 */
		const bool getMatchDataFromTemp(
			std::vector<TmpPeakVal*>& tpvArr);

		///*!
		// * @brief      TableにRow(Peak)+Valueを書き込む
		// * @param[in]  M/Z
		// * @param[in]  Retention Time(Min)
		// * @param[in]  Charge
		// * @param[in]  このROWに属するValue群
		// */
		//void writeRowLine(
		//	const double mz,
		//	const double rt,
		//	const int charge,
		//	std::vector<AB3D::TmpPeakVal*>& tpvArr);

		void getDataPointsFromTemp(
			const int tmpProfId,
			kome::core::DataPoints& dps);

		/*!
		 * @brief 終了フェーズを走らせる
		 */
		void finishMakeMatrix(const bool ret);

	private:
		/*!
		 * @brief  現在のサンプルを（開いていれば）閉じる
		 * @retval DBのオブジェクトを返す
		 */
		kome::db::Database* onGetDB();

		const int setTempProfile(
			const std::vector<std::pair<double, double>>& xicXyData
			);


		/*!
		 * @brief      XYPointsをBufferに変換
		 * @param[in]  XYPoints Data
		 * @param[out] Buffer オブジェクト
		 * @retval     変換時にエラーがあればFalse
		 */
		const bool tranceToBuffer(
			const std::vector<std::pair<double, double>>& xicXyData,
			kome::core::Buffer& rstBuffer);

		///*!
		// * @brief      Temp ProfileにBufferを登録
		// * @param[in]  Buffer オブジェクト
		// * @retval     登録したtemp profile ID
		// */
		//const int onSetTmpProfile(
		//	kome::core::Buffer& buffer);
	};
}
