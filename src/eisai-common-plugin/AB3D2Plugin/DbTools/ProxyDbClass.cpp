///*********************************************************/
///*!
// @addtoGroup AB3D
// @file proxyDBClass.h
// @brief AB3D Peak Position Main DB Ctrl
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
//    Mass++とEMSでDBの実装が異なるため、
//	proxyでインスタンスを合わせておき、
//	継承先をMass++とEMSで切り替えて整合性をあわせる。
// */
///*********************************************************/
//#include "stdafxwp.h"
//#include "proxyDBClass.h"
//#include "DBAccess.h"
//#include "tempContainer.h"
//
//#include <boost/assign/list_of.hpp>
//
//using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
//// constructor
//DbProxyClass::DbProxyClass() : ptr_(new DbAccess){}
//
//// destructor
//DbProxyClass::~DbProxyClass(){}
//
//// 後処理。デストラクタとは別に行う
//void DbProxyClass::closeMe() 
//{
//}
//
////初期化
//const bool DbProxyClass::initMe()
//{
//	return onInitMe();
//}
//
////内部初期化
//const bool DbProxyClass::onInitMe()
//{
//	return true;
//}
//
//// Check Ready DataBase.
//const bool DbProxyClass::isReadyDB(
//	int& refMatrixId,
//	std::vector<int>& refSampleIds
//) {
//	if(this->onGetMatrixId(refMatrixId)) {
//		matrixID_ = refMatrixId;
//		return this->onGetSampleIds(refMatrixId,refSampleIds);
//	}
//	return false;
//}
//
//// Open TargetID Sample
//const bool DbProxyClass::openTargtIdSample(
//	const int sampleId,
//	kome::objects::DataSet& refDataSet
//) {
//	return this->onOpenTargtIdSample(sampleId, refDataSet);
//}
//
//// 所定のアクティブサンプルを開く
//void DbProxyClass::closeActiveSample(
//){
//	this->onCloseActiveSample();
//}
//
////結果をコミットする
//void DbProxyClass::commitMe()
//{
//	this->onCommitMe();
//}
//
//// ピークの情報を書き込む
//const int DbProxyClass::setPeak(
//	const int mtId,
//	const double mz,
//	const double rt,
//	const int charge
//){
//	return onSetPeak(mtId, mz, rt, charge);
//}
//
//// Bufferを書き込む
//const int DbProxyClass::setXYBuffer(
//	kome::core::Buffer* buffer
//){
//	return onSetXYBuffer(buffer);
//}
//
//// Valueをセットする
//const bool DbProxyClass::setValues(
//	const int peakId,
//	const int sampleId,
//	const int bufId,
//	AB3D::TmpMXVal& tmval
//) {
//	return onSetValues(peakId, sampleId, bufId, tmval);
//}
