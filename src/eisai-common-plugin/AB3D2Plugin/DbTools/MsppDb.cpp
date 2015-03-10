///*********************************************************/
///*!
// @addtoGroup AB3D
// @file MsppDB.h
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
//    Mass++のDBインタフェース実装部分
// */
///*********************************************************/
//
//#include "stdafxwp.h"
//
//#include "MsppDB.h"
//#include "DBAccess.h"
//#include "tempContainer.h"
//
//// EMSではないとき
//#ifndef __EMS__
//
//// TypeDef
//typedef kome::ident::IdentificationManager DBMgr;
//
//// DBManagerのインスタンス取得をInline化
//inline DBMgr& getDBMgr() 
//{
//	return DBMgr::getInstance();
//}
//
//using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
//// constructor
//MsppDB::MsppDB() 
//	: AB3D::DbProxyClass()
//	, sampleInfo_()
//	, sample_(nullptr)
//{}
//
//// 初期化
//const bool MsppDB::onInitMe() 
//{
//	// DBの MZ, RT, Chargeを有効にする
//	getDBMgr().updateMatrixFlags(matrixID_, true, true, true);
//	return true;
//}
//
//// Open TargetID Sample
//const bool MsppDB::onOpenTargtIdSample(
//	const int sampleId,
//	kome::objects::DataSet& refDataSet
//) {
//	// SampleID(COL)からSampleIDとは異なるので、変換する
//	LOG_DEBUG(FMT("SampleID = %d", sampleId));
//	int keyID = getDBMgr().getMatrixSampleSample(sampleId);
//
//	LOG_DEBUG(FMT("keyID = %d  SampleID = %d", keyID, sampleId));
//
//	sample_ = getDBMgr().openSample(keyID, sampleInfo_);
//	
//	if (sample_ != nullptr) {
//		kome::objects::DataGroupNode* dgn = sample_->getRootDataGroupNode();
//		if(dgn != nullptr) {
//			dgn->getDataSet(&refDataSet);
//			return true;
//		}
////		refDataSet = sample_->getRootDataGroupNode();
//	}
//	return false;
//}
//
//// 所定のアクティブサンプルを開く
//void MsppDB::onCloseActiveSample()
//{
//	if (sample_ != nullptr) {
//		getDBMgr().releaseSample(sample_,sampleInfo_);
//	}
//}
//
//// IFDEF で切り替える。Matrix ID /Project IDを受け取る
//const bool MsppDB::onGetMatrixId(int& refMatrixId) 
//{
//	refMatrixId = getDBMgr().getSelectedMatrix();
//	return (refMatrixId >= 0);
//}
//
//// IFDEF で切り替える。
//// SampleIdsを受け取る。
//const bool MsppDB::onGetSampleIds(
//	const int matrixId,
//	std::vector<int>& refSampleIds
//){
//	std::vector<int> groupIds, sampleIds;
//	DBMgr& dbmgr = getDBMgr();
//	// Group IDを受け取る
//	dbmgr.getGroups(groupIds, matrixId);
//	for each(const int groupId in groupIds) {
//		sampleIds.clear();
//		// Group IDからSampleID(COL_ID)を受け取る
//		dbmgr.getMatrixSamplesFromGroup(sampleIds, groupId);
//		for each(const int sampleId in sampleIds) {
//			refSampleIds.push_back(sampleId);
//		}
//	}
//	return (!refSampleIds.empty());
//}
//
//
//// Buffer-> XTDataへ変換して、
//// Profileに書き込む。
//const int MsppDB::onSetXYBuffer(
//	kome::core::Buffer* buffer
//){
//	// Buffer->DPS変換への標準的な手続き
//	kome::core::DataPoints dps(kome::core::DataPoints::FLOAT);
//	dps.importData(boost::bind(&kome::core::DataAccessor::read, buffer, _1, _2 ));
//
//	return getDBMgr().setProfile(dps,true);
//}
//
//// Peak 情報を書き込む
//const int MsppDB::onSetPeak(
//	const int mtId,
//	const double mz,
//	const double rt,
//	const int charge
//){
//	return getDBMgr().addMatrixPeak(rt, mz, charge, mtId);
//}
//
//// Peakのオブジェクトを一時作成して、
//// Peakの情報を書き込む
//const bool MsppDB::onSetValues(
//	const int peakId,
//	const int sampleId,
//	const int bufId,
//	AB3D::TmpMXVal& tmval
//){
//	kome::objects::PeakElement pk(NULL);
//	pk.setX(tmval.core.x);
//	pk.setY(tmval.core.y);
//	pk.setLeft(tmval.left.x, tmval.left.y);
//	pk.setRight(tmval.right.x, tmval.right.y);
//
//	getDBMgr().setPeakValue(peakId, sampleId, tmval.value, &pk, bufId);
//	return true;
//}
//
//// コミットする
//void MsppDB::onCommitMe()
//{
//	getDBMgr().commit();
//}
//
//#endif
