/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DBAccess.cpp
 @brief AB3D Peak Position DB Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
   DBの細かい構文やパーツを担当する。
 */
/*********************************************************/

#include "stdafxwp.h"
#include "DBAccess.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


const std::string kEndSQL(";");

// constrctor
DbAccess::DbAccess(){}

// destrctor
DbAccess::~DbAccess()
{
}

// GetDB
kome::db::Database* DbAccess::onGetDB() 
{
	return kome::db::DbManager::getInstance().getDb();
}

//// DB_Setup
//const bool DbAccess::setup() 
//{
//	return true;
//}

// 最大のIDを取得する
// 取得エラーは全て -1以下 
const int DbAccess::getMaxID(
	const char* tablename
){
	kome::db::Database* db = onGetDB();
	int rstId = kNOTHING; // default is error.
	if (db == nullptr || tablename == nullptr) {
		return kDBERROR;
	}
	std::string sql = FMT("select MAX(ID) from %s;", tablename);
	kome::db::ResultSet* rs = db->executeQuery(sql.c_str());
	if(rs->next()) {
		rstId = rs->getInt(0, -1);
	}
	rs->close();
	return rstId;
}

// Create Temp Table
// TempTableを管理するクラス内でTableの定義関数を作成したものが、
// 引数として *ctFncとして渡される。
const bool DbAccess::createTempTable(
	const char* tablename, 
	void (*ctFnc)(kome::db::Table& table, const char* tname)
) {
	bool ret(true);
	kome::db::Database* db = onGetDB();
	if(db == nullptr) {
		return false;
	}
	if(!db->hasTable(tablename)){
		// Table が無いので作成
		kome::db::Table tbl;
		ctFnc(tbl, tablename);
		ret = db->createTmpTable(tbl);
		//(For Debug)テーブルの実体化が必要な場合に使用する。
		//ret = db->initTable(tbl);
	} else {
		std::string sql = FMT("delete from %s;", tablename);
		bool ret = db->execute(sql.c_str());
	}
	return ret;
}

//// Temp Tableがあるかチェック (SQLite3用)
//const int DbAccess::onIsHavingTempTable(
//	const char* temptablename
//){
//	kome::db::Database* db = onGetDB();
//	if (db == nullptr) {
//		return kDBERROR;
//	}
//	if(db->hasTable(temptablename)) {
//		// ある
//		return kSUCCESS;
//	} else {
//		return kNOTHING;
//	}
//
//	std::string sql = 
//		FMT("SELECT name FROM sqlite_temp_master WHERE type='table' AND name='%s';", temptablename);
//	kome::db::Statement* stmt = db->prepare(sql.c_str());
//	kome::db::ResultSet* rs = stmt->executeQuery();
//	int havingFlag = kNOTHING;
//
//	if (rs->next()) {
//		havingFlag = kSUCCESS;
//	}
//	rs->close();
//	stmt->close();
//	return havingFlag;
//}

// 指定SQLからStatementを作成する
const bool DbAccess::createStmt(
	const std::string& sql,
	kome::db::Statement*& refstmt
) {
	kome::db::Database* db = onGetDB();
	if (db == nullptr) { 
		return false;
	}

	refstmt = db->prepare(sql.c_str());
	if (refstmt== nullptr) {
		return false;
	}
	return true;
}

// Vector<String>からInsert文を作り、STMTにセットする。
const int DbAccess::createSelectStmt(
	const char* tname,
	const std::vector<const std::string>& sctVals, 
	const std::vector<const std::string>& whereVals,
	std::unique_ptr<kome::db::Statement>& refstmt
){
	kome::db::Statement* stmt = nullptr;
	int ret = setStatement_Select(tname, sctVals, whereVals, stmt);
	if(ret == kSUCCESS) {
		refstmt.reset(stmt);
	}
	return ret;
}

// Vector<String>からInsert文を作り、STMTにセットする。
const int DbAccess::createInsertStmt(
	const char* tname,
	const std::vector<const std::string>& vals,
	std::unique_ptr<kome::db::Statement>& refstmt
){
	kome::db::Statement* stmt = nullptr;
	int ret = setStatement_Insert(tname, vals, stmt);
	if(ret == kSUCCESS) {
		refstmt.reset(stmt);
	}
	return ret;
}

// updateのステートメントをセットする
const int DbAccess::createUpdateStmt(
	const char* tablename, 
	const std::vector<const std::string>& setVals, 
	const std::vector<const std::string>& whereVals,
	std::unique_ptr<kome::db::Statement>& refstmt
){
	kome::db::Statement* stmt = nullptr;
	int ret = setStatement_Update(tablename, setVals, whereVals, stmt);
	if(ret == kSUCCESS) {
		refstmt.reset(stmt);
	}
	return ret;
}

//--------------------------------------------------------#
// prepare statement
//--------------------------------------------------------#
// Select文の準備
const int DbAccess::setStatement_Select(
	const char* tablename, 
	const std::vector<const std::string>& sctVals, 
	const std::vector<const std::string>& whereVals,
	kome::db::Statement*& refstmt
){
	kome::db::Database* db = onGetDB();
	if (db == nullptr) { 
		return kDBERROR;
	}
	std::string sql = makeSQL_Select(tablename, sctVals);
	sql += makeSQL_Where(whereVals);
	sql += kEndSQL;
	refstmt = db->prepare(sql.c_str());
	if (refstmt == nullptr) {
		return kFAILED;
	}
	return kSUCCESS;
}


// Insertの準備
const int DbAccess::setStatement_Insert(
	const char* tablename, 
	const std::vector<const std::string>& intoVals,
	kome::db::Statement*& refstmt
) {
	kome::db::Database* db = onGetDB();
	if (db == nullptr) { 
		return kDBERROR;
	}
	std::string sql = makeSQL_Insert(tablename, intoVals);
	sql += kEndSQL;
	refstmt = db->prepare(sql.c_str());
	if (refstmt == nullptr) {
		return kFAILED;
	}
	return kSUCCESS; // return true code.
}

// Updateの準備
const int DbAccess::setStatement_Update(
	const char* tablename, 
	const std::vector<const std::string>& setVals, 
	const std::vector<const std::string>& whereVals,
	kome::db::Statement*& refstmt
){
	kome::db::Database* db = onGetDB();
	if (db == nullptr) { 
		return kDBERROR;
	}

	std::string sql = makeSQL_Update(tablename, setVals);
	sql += makeSQL_Where(whereVals);
	sql += kEndSQL;
	refstmt = db->prepare(sql.c_str());
	if (refstmt == nullptr) {
		return kFAILED;
	}
	return kSUCCESS;
}

//--------------------------------------------------------#
// SQL Maker
//--------------------------------------------------------#

// SQL : select %s from %TABLE% をつくる
const std::string DbAccess::makeSQL_Select(
	const char* tablename,
	const std::vector<const std::string>& vals
){
	if (tablename == nullptr || vals.empty()) {
		return "";
	}
	std::string sctStr = boost::algorithm::join(vals, ",");
	std::string sql =
		FMT("select %s from %s", sctStr.c_str(), tablename);
	return sql;
}


// SQL : insert into TABLE (VAL1,VAL2,..) values(?,?,..)
const std::string DbAccess::makeSQL_Insert(
	const char* tablename, 
	const std::vector<const std::string>& vals
) {
	if (tablename == nullptr || vals.empty()) {
		return "";
	}

	std::string intoStr = boost::algorithm::join(vals, ",");
	std::string valStr = "?";
	for(unsigned int i = 1; i < vals.size(); i++) {
		valStr += ",?";
	}

	std::string sql = 
		FMT("insert into %s (%s) values(%s)", tablename, intoStr.c_str(), valStr.c_str());

	return sql;
}

// use operater : boost::adapoters::transformed 
const std::string addS(const std::string& st) 
{
	return FMT("%s = ?", st.c_str());
}

// SQL : update TABLE set Val1=?, Val2=?....
const std::string DbAccess::makeSQL_Update(
	const char* tablename, 
	const std::vector<const std::string>& setVal
) {
	if (tablename == nullptr || setVal.empty()) {
		return "";
	} 
	std::string upStr = ""; 
	upStr = boost::algorithm::join(setVal|boost::adaptors::transformed(addS),",");

	std::string sql = 
		FMT("update %s set %s", tablename, upStr.c_str());
	return sql;
}

// Add Where Tgt T1 = 1 AND ....
const std::string DbAccess::makeSQL_Where(
	const std::vector<const std::string>& vals
){
	if (vals.empty()) return "";
	std::string rstStr 
		= boost::algorithm::join(vals|boost::adaptors::transformed(addS)," AND ");
	return FMT(" where %s",rstStr.c_str());
}

/*!--------------------------------------------------------
 setTableInfo class
----------------------------------------------------------*/

// Set Table Name
setTableInfo::setTableInfo(
	const char* tname,
	kome::db::Table& table) : table_(table)
{
	table_.setName(tname);
}

// Add Column Adapter
void setTableInfo::addColumn(
	const std::string& colname,
	const kome::db::TableColumn::DataType& coltype,
	bool notnull,
	bool autoincrement
) {
	kome::db::TableColumn* col = table_.addColumn(colname.c_str());
	col->setDataType(coltype);
	if (notnull) {
		col->setNotNull(true);
	}
	if (autoincrement) {
		col->setAutoIncrement(true);
	}
}
