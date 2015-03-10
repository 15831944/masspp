///*********************************************************/
///*!
// @addtoGroup AB3D
// @file EMSDB.cpp
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
//    EMSのDBインタフェース実装部分
// */
///*********************************************************/
//
//#include "stdafxwp.h"
//#include "EmsDb.h"
//#include "DbAccess.h"
//#include "tempContainer.h"
//
//#include <boost/assign/list_of.hpp>
//
//#ifdef __EMS__
//
//typedef kome::quant::QuantManagerFactory   QMFactory;
//typedef kome::quant::BaseQuantPeaksManager DBMgr;
//
//inline DBMgr& getDBMgr() {
//	return QMFactory::getInstance().getDBInstance();
//}
//
//using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
//EmsDB::EmsDB() : AB3D::DbProxyClass(), cgroup_(nullptr){}
//
//const bool EmsDB::onInitMe() 
//{
//	return this->onCreateStatements();
//}
//
//// IFDEF で切り替える。Matrix ID /Project IDを受け取る
//const bool EmsDB::onGetMatrixId(int& refMatrixId) {
//	refMatrixId = getDBMgr().getSelectedProject();
//	return (refMatrixId >= 0);
//}
//
//// IFDEF で切り替える。
//// SampleIdsを受け取る。
//const bool EmsDB::onGetSampleIds(
//	const int matrixId,
//	std::vector<int>& refSampleIds
//){
//	getDBMgr().getGroupNumbers(refSampleIds, matrixId, -1);
//	return (!refSampleIds.empty());
//}
//
//// Open TargetID Sample
//const bool EmsDB::onOpenTargtIdSample(
//	const int sampleId,
//	kome::objects::DataSet& refDataSet
//) {
//	DBMgr& dbMgr = getDBMgr();
//	cgroup_ = dbMgr.openGroup(sampleId, groupInfo_);
//	if(cgroup_!=nullptr) {
//		cgroup_->getDataSet(&refDataSet);
//		return true;
//	}
//	return false;
//}
//
//// 所定のアクティブサンプルを開く
//void EmsDB::onCloseActiveSample(
//){
//	if (cgroup_ != NULL) {
//		groupInfo_.sClose = true;
//		groupInfo_.ssClose = true;
//		getDBMgr().releaseGroup(cgroup_, groupInfo_);
//		cgroup_ = NULL;
//	}
//}
//
//void EmsDB::onCommitMe() 
//{
//	return getDBMgr().commit();
//}
//
//
//// 実書き込み部
//const int EmsDB::onSetPeak(
//	const int mtId,
//	const double mz,
//	const double rt,
//	const int charge
//){
//	kome::db::Statement* stmt = stmt_peak_.get();
//	stmt->resetBind();
//	stmt->bindInteger(1, mtId);
//	stmt->bindReal(2, rt);
//	stmt->bindReal(3, mz);
//	stmt->bindInteger(4, charge);
//	stmt->bindInteger(5, 1); // As True.
//	if (stmt->execute()) {
//		return ptr_->getMaxID("PKMTX_MATRIX_PEAK");
//	} else {
//		return -1;
//	}
//}
//
//const int EmsDB::onSetXYBuffer(
//	kome::core::Buffer* buffer
//){
//	kome::db::Statement* stmt = stmt_xyd_.get();
//	stmt->resetBind();
//	stmt->bindData(1, buffer->getBuffer(), buffer->getLength());
//	if (stmt->execute()) {
//		return ptr_->getMaxID("PKMTX_XYDATA");
//	} else {
//		return -1;
//	}
//}
//
//const bool EmsDB::onSetValues(
//	const int peakId,
//	const int sampleId,
//	const int bufId,
//	AB3D::TmpMXVal& tmval
//) {
//	const int profId 
//		= onSetEMSValues(bufId, tmval.core.x, tmval.left, tmval.right);
//	if (profId > 0) {
//		onSetEMSInts(peakId, sampleId,profId, tmval.core.y);
//		return true;
//	}
//	return false;
//}
//
//// -----------------------------
//// 以降　非継承の独自実装
//// -----------------------------
//
//// pkmtx profile
//const int EmsDB::onSetEMSValues(
//	const int bufId,
//	const double peak_x,
//	const AB3D::ElmPoint& left,
//	const AB3D::ElmPoint& right
//){
//	kome::db::Statement* stmt = stmt_prof_.get();
//	stmt->resetBind();
//	stmt->bindInteger(1,1);
//	stmt->bindReal(2,peak_x);
//	stmt->bindReal(3, left.x);
//	stmt->bindReal(4, left.y);
//	stmt->bindReal(5, right.x);
//	stmt->bindReal(6, right.y);
//	stmt->bindReal(7,peak_x);
//	stmt->bindReal(8,peak_x);
//	stmt->bindReal(9,0.5);
//	stmt->bindInteger(10,bufId);
//	stmt->execute();
//	return ptr_->getMaxID("PKMTX_PROFILE");
//}
//
//const int EmsDB::onSetEMSInts(
//	const int peakId,
//	const int sampleId,
//	const int profileId,
//	const double val
//){
//	kome::db::Statement* stmt = stmt_mxint_.get();
//	stmt->resetBind();
//	stmt->bindInteger(1,peakId);
//	stmt->bindInteger(2,sampleId);
//	stmt->bindInteger(3,profileId);
//	stmt->bindReal(4, val);
//	stmt->bindReal(5, val);
//	stmt->execute();
//	return 0;
//}
//
//
//const bool EmsDB::onCreateStatements()
//{
//	bool ret(true);
//	ret &= this->onCreateStmt_Insert_EMSMTXPEAK();
//	ret &= this->onCreateStmt_Insert_EMSMTXINT();
//	ret &= this->onCreateStmt_Insert_EMSPROFILE();
//	ret &= this->onCreateStmt_Insert_EMSXYDATA();
//	return ret;
//}
//
//const bool EmsDB::onCreateStmt_Insert_EMSMTXPEAK()
//{
//	std::vector<const std::string> vals = 
//		boost::assign::list_of("PROJECT_ID")("RT")("MZ")("CHARGE")("IS_SHOWN");
//	int ret = ptr_->createInsertStmt("PKMTX_MATRIX_PEAK", vals, stmt_peak_);
//	return ret == kSUCCESS;
//}
//
//const bool EmsDB::onCreateStmt_Insert_EMSMTXINT()
//{
//	std::vector<const std::string> vals = 
//		boost::assign::list_of("PEAK_ID")("GROUP_ID")("PROFILE_ID")
//		("PEAK_VALUE")("CORRECTED_VALUE");
//	int ret = ptr_->createInsertStmt("PKMTX_INTENSITY", vals, stmt_mxint_);
//	return ret == kSUCCESS;
//
//}
//
//const bool EmsDB::onCreateStmt_Insert_EMSPROFILE()
//{
//	std::vector<const std::string> vals = 
//		boost::assign::list_of("IS_CHROM")
//		("POSITION")("LEFT_X")("LEFT_Y")("RIGHT_X")("RIGHT_Y")
//		("RT")("ORG_RT")("TOLERANCE")("DATA_ID");
//	int ret = ptr_->createInsertStmt("PKMTX_PROFILE", vals, stmt_prof_);
//	return ret == kSUCCESS;
//}
//
//const bool EmsDB::onCreateStmt_Insert_EMSXYDATA()
//{
//	std::vector<const std::string> vals = 
//		boost::assign::list_of("DATA");
//	int ret = ptr_->createInsertStmt("PKMTX_XYDATA", vals, stmt_xyd_);
//	return ret == kSUCCESS;
//}
//
//#endif
