/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DbController.cpp
 @brief AB3D Peak Position DB Controller
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    AB3D Positionから呼ばれて、DBの一連の操作を代理する。
	TempTableは全て独自のSQLを書いているので独立させてある。

	DBの本体へ実データの書き込みは
	PeakPositionsクラスが請け負うのでProxyDBクラスは廃止する。
 */
/*********************************************************/

// AB3Dから呼ばれる口を担当する。
#include "stdafxwp.h"

#include "DbController.h"
#include "TempTableCtrl.h"
#include "tempContainer.h"

using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DbController::DbController() 
	: tblPtr_(new TmpTblCtrl)
//	, pxyPtr_(new kTargetDBType)
	, matrixId_(-1)
//	, nowCount_(0)
	, nowSampleId_(-1)
//	, sampleIds_()
{}

// destructor
DbController::~DbController(){}

//// 構築につき、最低限のTBの状態をチェックする
//const bool DbController::checkDBStatus()
//{
//	bool ret = pxyPtr_->isReadyDB(matrixId_, sampleIds_);
//	nowCount_ = 0;
//	nowSampleId_ = -1;
//	return ret;
//}

//Tempテーブルを作成する
const bool DbController::setupTempTables()
{
	if (tblPtr_->createTempTable() == kDBERROR) return false;
	bool ret = tblPtr_->createTempStatements();
	return ret;
}

// TempTableのステートメントをセットしておく
const bool DbController::setupTempSTMT()
{
	return tblPtr_->createTempStatements();
}


void DbController::setSampleId(const int spId)
{
	nowSampleId_ = spId;
}

////***** SampleGetterクラスがあるので、それに任せる。
////***** 但し、仕様上、nowSmapleIdだけは把握しておく必要が有るので、
////***** その取得用のインタフェースに変更をした(setSampleIdを追加)
//// 次のサンプルセットに駒を進める。
//// 次が無くなったらFalseを返す。
//const bool DbController::doNextSample() 
//{
//	this->onCloseActivegroup();
//
//	if (nowCount_ < sampleIds_.size()) {
//		nowSampleId_ = sampleIds_[nowCount_];
//		nowCount_++;
//		return true;
//	}
//	// nothing more sample.
//	return false;
//}

////***** SampleGetterクラスがあるので、それに任せる。
////アクティブ状態のサンプルをクローズ(リリース)する
//void DbController::onCloseActivegroup() 
//{
//	pxyPtr_->closeActiveSample();
//}

////***** SampleGetterクラスがあるので、それに任せる。
////現在指定子にあるサンプルのデータセットを取得する
//const bool  DbController::getNowDataGroup(
//	kome::objects::DataSet& refDataSet
//){
//	return pxyPtr_->openTargtIdSample(nowSampleId_, refDataSet);
//}

// Get DataBase Pointer(Interface)
kome::db::Database* DbController::onGetDB() 
{
	return kome::db::DbManager::getInstance().getDb();
}

// TempTableにXICのデータをセットする
const int DbController::setXICData(
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
){
	// temp tableに　profile(xydataの実体)を書き込む
	int profId = setTempProfile(xicXyDatas);

	//kome::core::Buffer rstBuf;
	//this->onTranceToBuffer(xicXyDatas, rstBuf);
	//int profId = tblPtr_->setXYBuffer(&rstBuf);

	// temp Table に　ピーク情報を書き込む
	int mxvId = tblPtr_->setTmpMXValue(
		nowSampleId_,
		profId,
		peak_top_x,
		peak_top_y,
		peak_left_x,
		peak_left_y,
		peak_right_x,
		peak_right_y,
		value
		);
	if (mxvId > 0) {
		// temp Table に M/Z, RT　を書き込む。
		return tblPtr_->setTmpPeak(nowSampleId_, mxvId, rt, mz);
	} else {
		// 作成されず。
		return -1;
	}
}

// TempTableにProfileを書き込む
const int DbController::setTempProfile(
	const std::vector<std::pair<double, double>>& xicXyDatas
){
	kome::core::Buffer rstBuf;
	tranceToBuffer(xicXyDatas, rstBuf);
	return tblPtr_->setXYBuffer(&rstBuf);
}

// Trance Buffers.
// X,YからBufferの形式に変換する
const bool DbController::tranceToBuffer(
	const std::vector<std::pair<double, double>>& xicXyData,
	kome::core::Buffer& rstBuffer
) {
	rstBuffer.clear();
	unsigned long bufLen = xicXyData.size();
	rstBuffer.write(&bufLen, sizeof(bufLen));

	if (bufLen > 0) {
		double* arr = new double[bufLen*2];
		for(unsigned int i = 0; i < bufLen; i++) {
			arr[i*2] = xicXyData[i].first;
			arr[i*2+1] = xicXyData[i].second;
		}
		rstBuffer.write(arr, sizeof(double) * bufLen *2);
		delete arr;
	}
	return true;
}


// 指定PeakIdのChargeを更新する
// (Peak作成時にはChargeが不明なので、後付けで修正する)
void DbController::updateCharge(
	const int peakId,
	const int charge
){
	tblPtr_->updateCharge(peakId, charge);
}

// Temp Tableから Peakデータを取得する。
const bool DbController::getMatchDataFromTemp(std::vector<TmpPeakVal*>& tpvArr)
{
	bool ret = tblPtr_->getTemporaryDatas(tpvArr);
	if(ret) ret = tpvArr.size() > 0;
	return ret;
}

//// Buffer(XYData)をTempProfileに書き込む
//// 戻り値は Temp Profile Id.
//const int DbController::onSetTmpProfile(
//	kome::core::Buffer& buffer
//) {
//	return tblPtr_->setXYBuffer(&buffer);
//}

// Temp Tableから Profileデータを取得する.
void DbController::getDataPointsFromTemp(
	const int tmpProfId,
	kome::core::DataPoints& dps
){
	kome::core::Buffer buffer;
	tblPtr_->getXYBuffer(tmpProfId, buffer);
	dps.importData(boost::bind(&kome::core::DataAccessor::read, buffer, _1, _2 ));
}

// Matrix作成の終了フェーズを走らせる(Commit等)
void DbController::finishMakeMatrix(
	const bool ret
) {
	if (ret) {
//		pxyPtr_->commitMe();
	}
	// 其々のDBのクロージャを走らせる。
//	pxyPtr_->closeMe();
	tblPtr_->closeMe();
}

//// Mass++のステートメントの準備（Mass++基準だと不要かも)
//const bool DbController::setupMsppSTMT()
//{
//	return pxyPtr_->initMe();
//}

//// ピークの列の情報と、それに準ずるValueのデータを
//// 実テーブルに書き込む。
//void DbController::writeRowLine(
//	const double mz,
//	const double rt,
//	const int charge,
//	std::vector<AB3D::TmpPeakVal*>& tpvArr
//){
//	int peakId = pxyPtr_->setPeak(matrixId_, mz, rt, charge);
//	// Insert Values.
//
//	for each(auto& tpv in tpvArr) {
//		kome::core::Buffer rstBuf;
//		tblPtr_->getXYBuffer(tpv->mxval.profId, rstBuf);
//
//		int bufId = pxyPtr_->setXYBuffer(&rstBuf);
//		AB3D::TmpMXVal& tmv = tpv->mxval;
//
//		pxyPtr_->setValues(peakId, tpv->sampleId, bufId, tmv);
//	}
//}
