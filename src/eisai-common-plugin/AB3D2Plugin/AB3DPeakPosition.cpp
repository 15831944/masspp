///*********************************************************/
///*!
// @addtoGroup AB3D
// @file AB3DPeakPosition.cpp
// @brief AB3D Peak Position Algorizhm
// 
// @date 2013.07.01
// @author M.Fukuda
// @version 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2014 
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
//    AB3Dオブジェクトと内部結合しています。
//    Plugin Call 時のMass++オブジェクト変換のコストが大きいので、
//    通常のPluginCallとは異なるフローで処理しています。
// */
///*********************************************************/
//
////
//// 検証次項について。
//// PeakPositionの閾値の計算は要調整です。
//// 絞り込み後、複数のピークがある場合の処理についても要調整です。
////
//
//#include "stdafx.h"
//#include "AB3DPeakPosition.h"
//
//// include AB3D Parts
//#include "NewAB3DLabeling.h"
//
//// include DB Controler
//#include "DBTools\DbController.h"
//#include "DBTools\tempContainer.h"
//
//// include progress controler
//#include "progress\progressMatrix.h"
//
//#define AB3D_SHORT_NAME "ab3d2"
//
//#include <fstream>
//
//// constructor
//AB3DPeakPositions::AB3DPeakPositions(
//	const double mzppm,
//	const double rttol,
//	std::map<std::string, std::string>& setmap,
//	kome::objects::SettingParameterValues* settings
//	) : mzppm_(mzppm)
//	, rttol_(rttol)
//	, setmap_(setmap)
//	, settings_(settings)
//{}
//
////********************************
//// 実行部
////********************************
//// 概要
//// 01.AB3Dの中で、候補となる XICのXYData データ、Peakの情報を取得/保存
//// 02.AB3Dの後半の処理で、[Peakである]データを更新する(Flagを立てる)
//// 03.全サンプルで、AB3Dが終わった後、[Peakである]データを全て取得する
//// 04.サンプル毎のPeakで、閾値内の M/Z, RT, Chargeの値を纏め、
////    確定した値から、実テーブルのMatrix　Row(Peak値)として書き込む。
//// 05.付随する各Valueの値をTempTableからコピーする。
//// 06.必要な分の計算を全て行うと、Matrixが出来上がる。
//// 07.処理終了時にTemp Tableが破棄されるので、不要な値はDBには残らない。
////--------------------------------
//const bool AB3DPeakPositions::execute (
//	kome::core::Progress& progress
//) {
//	// DB Object
//	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
//
//	// check DB Status
//	if (dbctrl->checkDBStatus() && (dbctrl->setupTempTables() != true)){
//		// DBが使えない or TempTableが作れないので準備できない
//		LOG_ERROR_CODE(FMT("Failed to access database."),ERR_DB);
//		return false;
//	}
//
//	// Execute AB3D
//	if (onExecAB3D(dbctrl.get(), progress)) {
//		// Exec Create Matrix
//		if(onCreateMatrix(dbctrl.get(), progress)) {
//			// 生成成功
//			LOG_INFO(FMT("Complete create matrix."));
//			return true;
//		} else {
//			// CreateMatrixに失敗
//			LOG_ERROR(FMT("Failed to create matrix table."));
//		}
//	} else {
//		//AB3Dに失敗
//		LOG_ERROR(FMT("Failed to execute AB3D."));
//	}
//	return false;
//}
//
//// AB3D実行部
//const bool AB3DPeakPositions::onExecAB3D(
//	AB3D::DbController* dbctrl,
//	kome::core::Progress& progress
//){
//	//AB3D object
//	NewAB3DLabeling ab3d(setmap_);
//
//	// Progress をスタート
//	MatrixProg prog(&progress, dbctrl->getSampleNumbers());
//
//	// サンプルがある限り繰り返す。
//	while(dbctrl->doNextSample() && !progress.isStopped() ) { // @date 2013.11.22 <Mod> M.Izumi
//		kome::objects::DataSet dataSet;
//		//計算用のオブジェクトをDBから取得する
//		if(dbctrl->getNowDataGroup(dataSet)) {
//			kome::core::Progress* subProgress = prog.getSubProgress();
//			ab3d.bindProgress(subProgress);
//			// AB3Dを実行。
//			ab3d.doAB3D(dataSet, dbctrl);
//		} else {
//			// dataSetを取得できない場合。
//			LOG_ERROR(FMT("Faild to the dataSet"));
//			return false;
//		}
//	}
//	prog.closeMe();
//	return true;
//}
//
//// Matrixを作成する
//// Progressの管理目途待ち
//const bool AB3DPeakPositions::onCreateMatrix(
//	AB3D::DbController* dbctrl,
//	kome::core::Progress& progress
//){
//	return this->onCreateMatrix(dbctrl);
//}
//
//// Matrixを作成する
//const bool AB3DPeakPositions::onCreateMatrix(
//	AB3D::DbController* dbctrl
//){
//	std::vector<AB3D::TmpPeakVal*> tmpPeaks;
//	std::vector<AB3D::TmpPeakVal*> errPeaks;
//	
//	// Temp Tableから Charge > -2 のPeak情報を、Valueの降順で取得する。
//	// TmpPeakValは内部で[New]されて戻る。
//	if(dbctrl->getMatchDataFromTemp(tmpPeaks)) {
//
//		std::vector<stRows> rowArr;
//		// Matrix Peaks Rowの為のオブジェクトを作る。
//		std::for_each(tmpPeaks.begin(), tmpPeaks.end(), [&](AB3D::TmpPeakVal* p){
//			if(!setupMatrixPeakRow(p, rowArr)) {
//				errPeaks.push_back(p);
//			}
//		});
//		// for Debug 
//		//debugPrint(tmpPeaks,errPeaks,rowArr);
//
//		// Matrix DBにオブジェクトの要素を書き込む
//		this->onWriteToMatrix(dbctrl, rowArr);
//
//		// NewされたオブジェクトをDeleteする。
//		for each(auto tpv in tmpPeaks){delete tpv;}
//
//		//std::for_each(tmpPeaks.begin(), tmpPeaks.end(), [](const AB3D::TmpPeakVal* tpv){
//		//	delete tpv;
//		//});
//		
//	} else {
//		// 取得に失敗 or 結果が何もない
//		LOG_ERROR(FMT("No Result"));
//		return false;
//	}
//	return true;
//}
//
//// Matrix Peaks Rowの為のオブジェクトを作る。
//// Labeling & Margeに当たる処理を行う。
//const bool AB3DPeakPositions::setupMatrixPeakRow(
//	AB3D::TmpPeakVal* pt,
//	std::vector<stRows>& rowArr
//){
//	bool newFg = true;
//	bool ret = true;
//	std::vector<stRows>::iterator it = rowArr.begin();
//	std::vector<stRows>::iterator eit = rowArr.end();
//
//	for(; it != eit && newFg; ++it) {
//		//登録済みROW(it)の範囲内かを判定
//		if(isInRow(pt, *it)) {
//			// 範囲内
//			newFg = false;
//			if(!isSameIdInRow(pt->sampleId, *it)) {
//				// SampleIDが重複せず。子の要素として登録する。
//				it->vals.push_back(pt);
//			} else {
//				//同一Sample内で閾値内のピークがある場合上書き「登録しない」
//				//別途Peak Rowを分ける必要が有る場合は記述。
//				ret = false;
//			}
//		}
//	}
//	//登録済みのROWと一致せず。新規要素として登録する。
//	if(newFg) {
//		rowArr.push_back(createRows(pt));
//	}
//	return ret;
//}
//
//// 対象が rowの閾値と同じか否かを返す
//const bool AB3DPeakPositions::isInRow(
//	AB3D::TmpPeakVal* p,
//	const stRows& row
//){
//	// Charge不一致
//	if (p->charge != row.charge) return false;
//	// m/zの閾値から外れる
//	if (p->mz < row.mztol.lo || row.mztol.hi < p->mz) return false;
//	// rtの閾値から外れる
//	if (p->rt < row.rttol.lo || row.rttol.hi < p->rt) return false;
//	// 閾値内で且つChargeが同じ。故に一致。
//	return true;
//}
//
////　同列の中に、同じSampleのピークがあるか否か探す。
//const bool AB3DPeakPositions::isSameIdInRow(
//	const int tgtId, 
//	const stRows& tgtRow
//){
//	for each(auto& t in tgtRow.vals) {
//		if(t->sampleId == tgtId) return true;
//	}
//	return false;
//}
//
//// rowを生成する
//stRows AB3DPeakPositions::createRows(
//	AB3D::TmpPeakVal* p
//){
//	stRows rst(p->mz, p->rt, p->charge);
//	double mztol = p->mz * 0.000001 * mzppm_;
//	// 閾値判定用の範囲を計算して保持する。
//	rst.mztol = tolerance(p->mz, mztol);
//	rst.rttol = tolerance(p->rt, rttol_);
//	rst.vals.push_back(p);
//	return rst;
//}
//
//
//// Matrixの実体を作成する
//const bool AB3DPeakPositions::onWriteToMatrix(
//	AB3D::DbController* dbctrl,
//	std::vector<stRows>& rowArr
//) {
//	// Matrixの実体部を作成の為のSTMTを作成する。
//	if (!rowArr.empty() && dbctrl->setupMsppSTMT()) {
//		std::for_each(rowArr.begin(), rowArr.end(), [&](stRows& r){
//			dbctrl->writeRowLine(r.mz, r.rt, r.charge, r.vals);
//		});
//		dbctrl->finishMakeMatrix(true);
//		return true;
//	}
//	return false;
//}
//
////-----------------------------------------------------
////For Debug Matrix only
////-----------------------------------------------------
//void AB3DPeakPositions::debugPrint(
//	std::vector<AB3D::TmpPeakVal*>& tmpPeaks,
//	std::vector<AB3D::TmpPeakVal*>& errPeaks,
//	std::vector<stRows>& rowArr
//){
//	std::string AllResults = "R:\\tmpPeaks.txt";
//	std::string AllRow = "R:\\rowvals.txt";
//	std::string ErrResults = "R:\\errPeaks.txt";
//
//	//--AB3Dの全結果が得られます。
//	this->debugOutputTmpPeaksValues(AllResults, tmpPeaks);
//	//--Matrixを構成するオブジェクトの内容が得られます
//	this->debugOutputRowValues(AllRow, rowArr);
//	//--Labeling&Mergeのアルゴリズムにより「登録されない」データが得られます
//	this->debugOutputTmpPeaksValues(ErrResults, errPeaks);
//}
//
//// Debug用 TmpPeaksValuesの中身を全て出力
//void AB3DPeakPositions::debugOutputTmpPeaksValues(
//	const std::string& outputfilePath,
//	const std::vector<AB3D::TmpPeakVal*>& tmpPeaks
//){
//	std::ofstream ofs(outputfilePath);
//	ofs << getTmpPeakAllElements(nullptr) << std::endl;
//
//	std::for_each(tmpPeaks.begin(),tmpPeaks.end(),
//		[&](AB3D::TmpPeakVal* p){ofs << getTmpPeakAllElements(p) << std::endl;});
//	ofs.close();
//}
//
//// Debug Row Valuesの中身を確認用
//void AB3DPeakPositions::debugOutputRowValues(
//	const std::string& outputfilePath,
//	const std::vector<stRows>& rowArr
//){
//	std::ofstream ofs2(outputfilePath);
//	ofs2 << "mxId,m/z,rt,z," << getTmpPeakAllElements(nullptr) << std::endl;
//	std::string hs(""),ss("");
//	int cnt = 1;
//
//	for each(auto& r in rowArr) {
//		hs = FMT("%d,%f,%f,%d,",cnt, r.mz, r.rt, r.charge);
//		for each(auto& vt in r.vals) {
//			ss = getTmpPeakAllElements(vt);
//			ofs2 << hs << ss << std::endl;
//		}
//		cnt++;
//	}
//	ofs2.close();
//}
//
//// Debug用
//const std::string AB3DPeakPositions::getTmpPeakAllElements(
//	AB3D::TmpPeakVal* p
//){
//	std::string ts;
//	if(p != nullptr) {
//		ts = FMT("%f,%f,%d,%d",p->mz, p->rt, p->charge, p->sampleId);
//		ts += FMT(",%f", p->mxval.value);
//		ts += FMT(",%f,%f", p->mxval.core.x, p->mxval.core.y);
//		ts += FMT(",%f,%f", p->mxval.left.x, p->mxval.left.y);
//		ts += FMT(",%f,%f", p->mxval.right.x, p->mxval.right.y);
//	} else {
//		ts = "m/z,rt,z,sampleId,value,Core.x,Core.y,Left.x,Left.y,Right.x,Right.y";
//	}
//	return ts;
//}
//
////AB3D を実体のあるTempTableに吐き出して、実行する
//const bool AB3DPeakPositions::executeDebug (
//) {
//	// DB Object
//	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
//	dbctrl->setupTempSTMT();
//	dbctrl->checkDBStatus();
//	bool ret = true;
//	//FeedBack Results from temp table
//	ret = this->onCreateMatrix(dbctrl.get());
//	
//	return true;
//}
