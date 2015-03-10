/*********************************************************/
/*!
 @addtoGroup AB3D
 @file TempTableCtrl.cpp
 @brief Temp Table Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
   中間計算部分のテンポラリテーブル部分を担当する。
 */
/*********************************************************/
#include "stdafxwp.h"

#include "TempTableCtrl.h"
#include "DBAccess.h"
#include "tempContainer.h"

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/join.hpp>

// Define Table
#define TEMP_TBL_AB3D_PEAK	"TEMP_AB3D_PEAKS"
#define TEMP_TBL_AB3D_VAL	"TEMP_AB3D_MXVALUES"
#define TEMP_TBL_AB3D_PROF	"TEMP_AB3D_PROFILES"

// Temp Peak Talbe
#define REL_MZ          "MZ"
#define REL_RT          "RT"
#define INT_CHARGE      "CHARGE"
#define REL_VALUE       "VALUE"
#define INT_MATRIX_ID   "MATRIX_ID"

// Temp Matrix Value Table
#define INT_SAMPLE_ID	"SAMPLE"
#define INT_PROFILE_ID	"PROFILE_ID"
#define REL_PEAK_POS	"PEAK_POSITION"
#define REL_PEAK_VAL	"PEAK_VALUE"
#define REL_LEFT_X		"LEFT_X"
#define REL_LEFT_Y		"LEFT_Y"
#define REL_RIGHT_X		"RIGHT_X"
#define REL_RIGHT_Y		"RIGHT_Y"

// Temp Profile Table
#define INT_ID			"ID"
#define IS_CHROM		"IS_CHROM"
#define BIN_XYDATA		"DATA"

#define NO_USE_CHARGE   -2

using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
TmpTblCtrl::TmpTblCtrl() : ptr_(new DbAccess)
{
}

// destructor
TmpTblCtrl::~TmpTblCtrl() 
{
}

// 後処理。デストラクタとは別に行う。
void TmpTblCtrl::closeMe()
{
}

// TempTableを作成する。
// 正常に作成 or 既にある場合 はTrue。
// 生成時に何らかのエラーが出た場合は　False。
const int TmpTblCtrl::createTempTable()
{
	if (!ptr_->createTempTable(TEMP_TBL_AB3D_PEAK, this->createTempTableAB3DPeak)) {
		return kDBERROR;
	}

	if(!ptr_->createTempTable(TEMP_TBL_AB3D_VAL, this->createTempTableAB3DVal)) {
		return kDBERROR;
	}

	if(!ptr_->createTempTable(TEMP_TBL_AB3D_PROF, this->createTempTableAB3DProf)) {
		return kDBERROR;
	}

	return kSUCCESS;
}

// Temp Peak Value Table を作成する
void TmpTblCtrl::createTempTableAB3DPeak(
	kome::db::Table& table, const char* tname
){
	setTableInfo tmp(tname, table);
	tmp.addColumn(INT_ID,        kome::db::TableColumn::INTEGER, false, true);
	tmp.addColumn(INT_SAMPLE_ID, kome::db::TableColumn::INTEGER, true);
	tmp.addColumn(INT_MATRIX_ID, kome::db::TableColumn::INTEGER);
	tmp.addColumn(REL_RT,        kome::db::TableColumn::REAL);
	tmp.addColumn(REL_MZ,        kome::db::TableColumn::REAL);
	tmp.addColumn(INT_CHARGE,    kome::db::TableColumn::INTEGER);
}

// Temp Matrix Value Table を　作成する
void TmpTblCtrl::createTempTableAB3DVal(
	kome::db::Table& table, const char* tname
) {
	setTableInfo tmp(tname, table);
	// Columns
	tmp.addColumn(INT_ID,         kome::db::TableColumn::INTEGER, false, true);
	tmp.addColumn(INT_SAMPLE_ID,  kome::db::TableColumn::INTEGER, true);
	tmp.addColumn(INT_PROFILE_ID, kome::db::TableColumn::INTEGER);
	tmp.addColumn(REL_PEAK_POS,   kome::db::TableColumn::REAL);
	tmp.addColumn(REL_PEAK_VAL,   kome::db::TableColumn::REAL);
	tmp.addColumn(REL_LEFT_X,     kome::db::TableColumn::REAL);
	tmp.addColumn(REL_LEFT_Y,     kome::db::TableColumn::REAL);
	tmp.addColumn(REL_RIGHT_X,    kome::db::TableColumn::REAL);
	tmp.addColumn(REL_RIGHT_Y,    kome::db::TableColumn::REAL);
	tmp.addColumn(REL_VALUE,     kome::db::TableColumn::REAL);
}

// Create Temp Table (Masspp = Profile / EMS = XYData)
void TmpTblCtrl::createTempTableAB3DProf(
	kome::db::Table& table, const char* tname
) {
	setTableInfo tmp(tname,table);
	// Columns
	tmp.addColumn(INT_ID,     kome::db::TableColumn::INTEGER, true, true);
	tmp.addColumn(BIN_XYDATA, kome::db::TableColumn::BINARY, true);
}

// Create Inserts STMT
const bool TmpTblCtrl::createTempStatements()
{
	if(!onCreateStmt_Insert_TmpPEAKS()) return false;
	if(!onCreateStmt_Insert_TmpMXVALUES()) return false;
	if(!onCreateStmt_Insert_TmpPROFILES()) return false;
	if(!onCreateStmt_UpdateCharge()) return false;
	if(!onCreateStmt_SelectTmpMXVALUES()) return false;
	if(!onCreateStmt_SelectTmpPROFILES()) return false;
	return true;
}

// BufferをTmpに登録
const int TmpTblCtrl::setXYBuffer(
	kome::core::Buffer* buffer
){
	if(onDoInsertBuffer(buffer)) {
		return onGetMaxID_TmpPROFILES();
	}
	return kNOTHING;
}

// TempTable にBufferを入れる
const bool TmpTblCtrl::onDoInsertBuffer(
	kome::core::Buffer* buffer
){
	stmt_tmpProf_->resetBind();
	stmt_tmpProf_->bindData(1, buffer->getBuffer(), buffer->getLength());

	if(!stmt_tmpProf_->execute()) {
		return false;
	} // execute faile.
	return true;
}

// Temp Profile の最大IDを貰う
const int TmpTblCtrl::onGetMaxID_TmpPROFILES()
{
	return ptr_->getMaxID(TEMP_TBL_AB3D_PROF);
}

// Temp MXValueに値を登録する
const int TmpTblCtrl::setTmpMXValue(
	const int sampleid,   
	const int bufid,
	const double peak_x,  
	const double peak_y,
	const double left_x,  
	const double left_y,
	const double right_x, 
	const double right_y,
	const double value
) {
	if(onSetTmpMXValue(sampleid, bufid, peak_x, peak_y, left_x, left_y, right_x, right_y, value)) {
		return onGetMaxID_TmpPROFILES();
	}
	return kNOTHING;
}

// Matrix Valueにあたる部分をテンポラリに吐き出す
// (STMT実行部)
const bool TmpTblCtrl::onSetTmpMXValue(
	const int sampleid,   
	const int bufid,
	const double peak_x,  
	const double peak_y,
	const double left_x,  
	const double left_y,
	const double right_x, 
	const double right_y,
	const double value
) {
	stmt_tmpVal_->resetBind();
	stmt_tmpVal_->bindInteger(1, sampleid);
	stmt_tmpVal_->bindInteger(2, bufid);
	stmt_tmpVal_->bindReal(3, peak_x);
	stmt_tmpVal_->bindReal(4, peak_y);
	stmt_tmpVal_->bindReal(5, left_x);
	stmt_tmpVal_->bindReal(6, left_y);
	stmt_tmpVal_->bindReal(7, right_x);
	stmt_tmpVal_->bindReal(8, right_y);
	stmt_tmpVal_->bindReal(9, value);
	bool ret = stmt_tmpVal_->execute();
	return ret;
}

//最大値を返す
const int TmpTblCtrl::onGetMaxID_TmpMXVALUES()
{
	return ptr_->getMaxID(TEMP_TBL_AB3D_VAL);
}

// Temp Peakに値をInsertする。
const int TmpTblCtrl::setTmpPeak(
	const int sampleid,
	const int mxvid,
	const double rt,
	const double mz
){
	if(onSetTmpPeak(sampleid, mxvid, rt, mz)) {
		return onGetMaxID_TmpPEAKS();
	}
	return kNOTHING;
}

// Peak 情報を更新する
const bool TmpTblCtrl::onSetTmpPeak(
	const int sampleid,
	const int mxvid,
	const double rt,
	const double mz
) {
	stmt_tmpPeak_->resetBind();
	stmt_tmpPeak_->bindInteger(1, sampleid);
	stmt_tmpPeak_->bindInteger(2, mxvid);
	stmt_tmpPeak_->bindReal(3, rt);
	stmt_tmpPeak_->bindReal(4, mz);
	stmt_tmpPeak_->bindInteger(5, NO_USE_CHARGE);
	bool ret = stmt_tmpPeak_->execute();
	return ret;
}

// Charge を更新する
void TmpTblCtrl::updateCharge(
	const int nId, const int charge
){
	kome::db::Statement* stmt = stmt_UpdtCrg_.get();
	stmt->bindInteger(1, charge);
	stmt->bindInteger(2, nId);
	stmt->execute();
	stmt->resetBind();
}

// XYBufferを取得する
const bool TmpTblCtrl::getXYBuffer(
	const int bufId,
	kome::core::Buffer& refBuffer
){
	kome::db::Statement* stmt = stmt_SlctProf_.get();
	stmt->resetBind();
	stmt->bindInteger(1, bufId);
	kome::db::ResultSet* rs = stmt->executeQuery();
	if (rs != nullptr && rs->next()) {
		void* addr = rs->getData(0);
		unsigned int len = rs->getDataSize(0);
		refBuffer.write(addr,len);
	}
	rs->close();

	return true;
}

// Temp Tableから値を取得する
const bool TmpTblCtrl::getTemporaryDatas(
	std::vector<TmpPeakVal*>& refPeakvals
){
	// Charge > -2 の値を　Mz, RT を昇順に並べて取得する。
	std::string sql = onCreateSQL_TempResults();
	kome::db::Statement* stmt = nullptr;
	if(!ptr_->createStmt(sql, stmt)) {
		return false;
	}
	kome::db::ResultSet* rs = stmt->executeQuery();
	int mxvId(-1);

	if (rs != nullptr) {
		while(rs->next()) {
			TmpPeakVal* pkval = new TmpPeakVal();
			pkval->sampleId = rs->getInt(0, -1);
			pkval->rt = rs->getReal(2, -1.0);
			pkval->mz = rs->getReal(3, -1.0);
			pkval->charge = rs->getInt(4, -1);
			// Get matrix value Id
			mxvId = rs->getInt(1,-1);
			this->onGetTmpMXValue(mxvId, pkval->mxval);
			refPeakvals.push_back(pkval);
		}
		rs->close();
	}
	stmt->close();
	return true;
}

// Temp Matrix Value の　ID値を取得する
const bool TmpTblCtrl::onGetTmpMXValue(
	const int mxvId,
	TmpMXVal& refMxval
){
	kome::db::Statement* stmt = stmt_SlctVal_.get();
	stmt->resetBind();
	stmt->bindInteger(1, mxvId);
	kome::db::ResultSet* rs = stmt->executeQuery();
	if (rs != nullptr && rs->next()) {
		refMxval.profId = rs->getInt(0, -1);
		refMxval.core.x = rs->getReal(1, -1.0);
		refMxval.core.y = rs->getReal(2, -1.0);
		refMxval.left.x = rs->getReal(3, -1.0);
		refMxval.left.y = rs->getReal(4, -1.0);
		refMxval.right.x = rs->getReal(5, -1.0);
		refMxval.right.y = rs->getReal(6, -1.0);
		refMxval.value = rs->getReal(7, -1.0);
		rs->close();
		return true;
	}
	return false;
}

//-----------------------------------------
// Statment を作成する
//-----------------------------------------

typedef std::vector<const std::string> ELMVALS;
using namespace boost::assign;

//最大値を返す
const int TmpTblCtrl::onGetMaxID_TmpPEAKS()
{
	return ptr_->getMaxID(TEMP_TBL_AB3D_PEAK);
}

// Temp Peaks の Insert STMTを生成して、内部的に保持する
const bool TmpTblCtrl::onCreateStmt_Insert_TmpPEAKS()
{
	ELMVALS vals = 
		list_of(INT_SAMPLE_ID)(INT_MATRIX_ID)(REL_RT)(REL_MZ)(INT_CHARGE);
	int ret = ptr_->createInsertStmt(TEMP_TBL_AB3D_PEAK, vals, stmt_tmpPeak_);
	return ret == kSUCCESS;
}

// Temp Matrix Value のInsert STMTを生成して、内部的に保持する
const bool TmpTblCtrl::onCreateStmt_Insert_TmpMXVALUES() 
{
	ELMVALS vals = 
		list_of(INT_SAMPLE_ID)(INT_PROFILE_ID)
		(REL_PEAK_POS)(REL_PEAK_VAL)(REL_LEFT_X)(REL_LEFT_Y)(REL_RIGHT_X)(REL_RIGHT_Y)(REL_VALUE);
	int ret = ptr_->createInsertStmt(TEMP_TBL_AB3D_VAL, vals, stmt_tmpVal_);
	return ret == kSUCCESS;
}

// Temp Profile のInsert STMTを生成して、内部的に保持する
const bool TmpTblCtrl::onCreateStmt_Insert_TmpPROFILES()
{
	ELMVALS vals = list_of(BIN_XYDATA);
	int ret = ptr_->createInsertStmt(TEMP_TBL_AB3D_PROF, vals, stmt_tmpProf_);
	return ret == kSUCCESS;
}

// Temp Peaks の　Chargeの　STMTを作成する
const bool TmpTblCtrl::onCreateStmt_UpdateCharge()
{
	ELMVALS vals = list_of(INT_CHARGE);
	ELMVALS whrvals = list_of(INT_ID);
	int ret = ptr_->createUpdateStmt(TEMP_TBL_AB3D_PEAK, vals, whrvals, stmt_UpdtCrg_);
	return ret == kSUCCESS;
}

// 変更前
//  select * from TEMP_PEAKS where charge > -2 order by mz asc, rt asc から
// 変更後
// select t1.SAMPLE,t1.MATRIX_ID,t1.RT,t1.MZ,t1.CHARGE,t2.value 
// from TEMP_AB3D_PEAKS as t1, TEMP_AB3D_MXVALUES as t2  
// where t1.charge > -2 and t1.MATRIX_ID = t2.ID order by t2.VALUE desc;
const std::string TmpTblCtrl::onCreateSQL_TempResults()
{
	std::vector<std::string> vals =
	//ELMVALS vals = 
		list_of(INT_SAMPLE_ID)(INT_MATRIX_ID)(REL_RT)(REL_MZ)(INT_CHARGE);
	std::for_each(vals.begin(),vals.end(),[](std::string& t){t = "t1." + t;});
	std::string valstr = boost::algorithm::join(vals,",");
//	valstr += FMT(",t2.%s", REL_VALUE);

	std::string sql = FMT("select %s", valstr.c_str());
	sql += FMT(" from %s as t1, %s as t2", TEMP_TBL_AB3D_PEAK, TEMP_TBL_AB3D_VAL);
	sql += FMT(" where t1.%s > -2 and t1.%s = t2.%s", INT_CHARGE, INT_MATRIX_ID, INT_ID); 
	sql += FMT(" order by t2.%s desc;", REL_VALUE);
	return sql;
}

// Id から必要な要素を Temp Matrix Value から　回収する　STMTを作る
const bool TmpTblCtrl::onCreateStmt_SelectTmpMXVALUES()
{
	ELMVALS vals = 
		list_of(INT_PROFILE_ID)(REL_PEAK_POS)(REL_PEAK_VAL)
		(REL_LEFT_X)(REL_LEFT_Y)(REL_RIGHT_X)(REL_RIGHT_Y)(REL_VALUE);
	ELMVALS whrvals = list_of(INT_ID);
	int ret = ptr_->createSelectStmt(TEMP_TBL_AB3D_VAL, vals, whrvals, stmt_SlctVal_);
	return ret == kSUCCESS;
}

const bool TmpTblCtrl::onCreateStmt_SelectTmpPROFILES()
{
	ELMVALS vals = list_of(BIN_XYDATA);
	ELMVALS whrvals = list_of(INT_ID);
	int ret = ptr_->createSelectStmt(TEMP_TBL_AB3D_PROF, vals, whrvals, stmt_SlctProf_);
	return ret == kSUCCESS;
}
