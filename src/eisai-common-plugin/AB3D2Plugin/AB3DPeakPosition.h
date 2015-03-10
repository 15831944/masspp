///*********************************************************/
///*!
// @addtoGroup AB3D
// @file AB3DPeakPosition.h
// @brief AB3D Peak Position Algorizhm
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
// */
///*********************************************************/
//#pragma once
//
//// 前方宣言
//namespace AB3D {
//	class DbController;
//	struct TmpPeakVal;
//}
//
//class NewAB3DLabeling;
//
//// 最小/最大の閾値を保存する構造体
//struct tolerance {
//	double lo;
//	double hi;
//	// デフォルトコンストラクタ
//	tolerance(){}
//	// コンストラクタで範囲を計算する
//	tolerance(const double val, const double tol) 
//		: lo(val - tol)
//		, hi(val + tol){}
//};
//
//// Peaks Rowの値を保存する構造体
//// *TmpPeakValはTempContiner.h　で記述。
//struct stRows {
//	// Rowに表示される
//	double mz;
//	double rt;
//	int charge;
//	// 閾値内外の比較時に使用
//	tolerance mztol; 
//	tolerance rttol; 
//	// Peaks Rowの子の要素
//	std::vector<AB3D::TmpPeakVal*> vals;
//
//	//constructor(1)
//	stRows(){}
//
//	// constrctor(2)
//	stRows(
//		const double _mz, 
//		const double _rt, 
//		const int _cg
//		) : mz(_mz)
//		, rt(_rt)
//		, charge(_cg){}
//};
//
//// AB3DのMatrix作成の為のアルゴリズム。
//class AB3DPeakPositions {
//public:
//	// constructor
//	explicit AB3DPeakPositions(
//		const double mzppm,
//		const double rttol,
//		std::map<std::string, std::string>& setmap,
//		kome::objects::SettingParameterValues* settings);
//	// destructor
//	~AB3DPeakPositions(){};
//
//private:
//	// Uncopyable
//	AB3DPeakPositions(const AB3DPeakPositions&);
//	AB3DPeakPositions operator=(const AB3DPeakPositions&); 
//	
//private:
//	// Inner parameters
//	double mzppm_;
//	double rttol_;
//	std::map<std::string, std::string> setmap_;
//
//	kome::objects::SettingParameterValues* settings_;
//
//public:
//
//	/*!
//	 * @brief AB3D Peak Position を実行する
//	 * @param[in]  progress
//	 * @retval     成功ならTrueが返る
//	 */
//	const bool execute(kome::core::Progress& progress);
//
//	/*!
//	 * @brief AB3D Peak Position のみを実行する
//	 * @param[in]  progress
//	 * @retval     成功ならTrueが返る
//	 * @note       AB3Dの結果はTempTableにある事が前提
//	 */
//	const bool executeDebug ();
//
//private:
//
//	/*!
//	 * @brief      AB3Dを実行する
//	 * @param[in]  DBCtrlオブジェクトのポインタ
//	 * @param[in]  progress
//	 * @retval     成功ならTrueが返る
//	 */
//	const bool onExecAB3D(
//		AB3D::DbController* dbctrl,
//		kome::core::Progress& progress);
//
//	/*!
//	 * @brief      実DBにPeak Positionの結果を書き込む
//	 * @param[in]  DBCtrlオブジェクトのポインタ
//	 * @param[in]  progress
//	 * @retval     成功ならTrue
//	 */
//	const bool onCreateMatrix(
//		AB3D::DbController* dbctrl,
//		kome::core::Progress& progress);
//
//	/*!
//	 * @brief      実DBにPeak Positionの結果を書き込む
//	 * @param[in]  DBCtrlオブジェクトのポインタ
//	 * @retval     成功ならTrue
//	 */
//	const bool onCreateMatrix(
//		AB3D::DbController* dbctrl);
//
//	const bool setupMatrixPeakRow(
//		AB3D::TmpPeakVal* pt,
//		std::vector<stRows>& rowArr);
//
//	const bool onWriteToMatrix(
//		AB3D::DbController* dbctrl,
//		std::vector<stRows>& rowArr);
//
//	stRows createRows(AB3D::TmpPeakVal* pt);
//
//	const bool isInRow(
//		AB3D::TmpPeakVal* pt, 
//		const stRows& tgtRow);
//
//	const bool isSameIdInRow(
//		const int tgtId, 
//		const stRows& tgtRow);
//
//
//	//----------------
//	//	Debug 用関数
//	//----------------
//
//	void debugPrint(
//		std::vector<AB3D::TmpPeakVal*>& tmpPeaks,
//		std::vector<AB3D::TmpPeakVal*>& errPeaks,
//		std::vector<stRows>& rowArr
//	);
//
//	void debugOutputRowValues(
//		const std::string& outputfilePath,
//		const std::vector<stRows>& rowArr);
//
//	void debugOutputTmpPeaksValues(
//		const std::string& outputfilePath,
//		const std::vector<AB3D::TmpPeakVal*>& tmpPeaks);
//
//	const std::string getTmpPeakAllElements(
//		AB3D::TmpPeakVal* pt);
//
//};
