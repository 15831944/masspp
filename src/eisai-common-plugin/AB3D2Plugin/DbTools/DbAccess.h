/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DBAccess.h
 @brief AB3D Peak Position DB Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#pragma once

namespace AB3D {

/*!--------------------------------------------------------
    @name   DbAccess
    @brief  Data Base Access Class
    @date   2013.07.01
    @par    IdentificationManager の実装に依存せず、
            DBを操作する為のSQL,STMTの定義を管理する。
-----------------------------------------------------------*/
	class DbAccess {

	public:
		// constructor
		DbAccess();
		// destructor
		~DbAccess();

	private:
		// Uncopyable
		DbAccess(const DbAccess&);
		DbAccess& operator=(const DbAccess&);

	public:
//		const bool setup();

		// Only Action
		const int getMaxID(const char* tablename);

		/*!
		 * @brief TempTableを作成する
		 * @param[in]  %TABLE% 名
		 * @param[in]  テーブル生成を定義した関数を渡す
		 * @retval     成功ならTが返る
		 * @note
		      テーブルの生成関数は addcolumnで各々定義したものを渡す。
		 */
		const bool createTempTable(
			const char* tablename, 
			void (*ctFnc)(kome::db::Table& table, const char* tname));

		//------------------------------------------------
		// for create Statemet
		//------------------------------------------------

		/*!
		 * @brief SQLをStmtにセットする
		 * @param[in]  SQL文
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功ならTrueが返る
		 */
		const bool createStmt(
			const std::string& sql,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief SelectのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  selectする要素
		 * @param[in]  whereの要素
		 * @param[out] 生成SQLをセットするstatement(内部でリセットされる)
		 * @retval     成功なら０が返る
		 */
		const int createSelectStmt(
			const char* tname,
			const std::vector<const std::string>& sctVals, 
			const std::vector<const std::string>& whereVals,
			std::unique_ptr<kome::db::Statement>& refstmt);

		/*!
		 * @brief InsertのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  insertする要素
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功なら０が返る
		 */
		const int createInsertStmt(
			const char* tname,
			const std::vector<const std::string>& vals,
			std::unique_ptr<kome::db::Statement>& refstmt);

		/*!
		 * @brief updateのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  updateする要素
		 * @param[in]  whereの要素
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功なら０が返る
		 */
		const int createUpdateStmt(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			std::unique_ptr<kome::db::Statement>& refstmt);

	private:
		/*!
		 * @brief  DBのポインタを取得する
		 * @retval DBのポインタ
		 */
		kome::db::Database* onGetDB();

		//-------------------------------------------------
		// on Prepare Statment
		//-------------------------------------------------

		/*!
		 * @brief SelectのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  selectする要素
		 * @param[in]  whereの要素
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功なら０が返る
		 */
		const int setStatement_Select(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief InsertのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  insertする要素
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功なら０が返る
		 */
		const int setStatement_Insert(
			const char* tablename, 
			const std::vector<const std::string>& intoVals,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief updateのSQLをStmtにセットする
		 * @param[in]  %TABLE% 名
		 * @param[in]  updateする要素
		 * @param[in]  whereの要素
		 * @param[out] 生成SQLをセットするstatement.
		 * @retval     成功なら０が返る
		 */
		const int setStatement_Update(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			kome::db::Statement*& refstmt);

		//-------------------------------------------------
		// 配列からSQL固有の繰り返し部分を作る
		//-------------------------------------------------
		/*!
		 * @brief [select (v1,v2,..) from %TABLE%]を作る
		 * @param[in]  %TABLE% 名
		 * @param[in]  selectする要素
		 * @retval     生成したSQL文
		 * @note       where句は別途生成
		 */
		const std::string makeSQL_Select(
			const char* talbename,
			const std::vector<const std::string>& vals);

		/*!
		 * @brief [insert into %TABLE% (v1,v2,..) values(?,?,..)]のSQLを作る
		 * @param[in]  %TABLE% 名
		 * @param[in]  insertする要素
		 * @retval     生成したSQL文
		 * @note       where句は別途生成
		 */
		const std::string makeSQL_Insert(
			const char* tablename, 
			const std::vector<const std::string>& vals);

		/*!
		 * @brief SQLの[update %TABLE% set val1=?, ...]を作る
		 * @param[in]  %TABLE% 名
		 * @param[in]  updateする要素
		 * @retval     生成したSQL文
		 * @note       where句は別途生成
		 */
		const std::string makeSQL_Update(
			const char* tablename, 
			const std::vector<const std::string>& vals);

		/*!
		 * @brief SQLの[where st1=? AND st2=?,...]を作る
		 * @param[in]  %TABLE% 名
		 * @param[in]  where句の要素
		 * @retval     生成したSQL文
		 */
		const std::string makeSQL_Where(
			const std::vector<const std::string>& vals);
	};

/*!--------------------------------------------------------
    @name   setTableInfo
    @brief  テーブルの作成を補助する
    @date   2013.07.01
    @par    DBテーブル作成の固有ルールを纏める
-----------------------------------------------------------*/
	class setTableInfo {
	public:

		/*!
		 * @brief constructor
		 * @param[in] テーブル名
		 * @param[in] テーブルオブジェクト
		 */
		setTableInfo(
			const char* tname,
			kome::db::Table& table);

		// destrcutor
		~setTableInfo(){}
	private:
		/* inner elements */
		kome::db::Table& table_;

	public:
		/*!
		 * @brief テーブル要素の作成
		 * @param[in] カラム名
		 * @param[in] データタイプ(integet,String等)
		 * @param[in] 未入力を許可しないならTrue
		 * @param[in] 自動加算(IDのタイプ)ならTrue.
		 */
		void addColumn(
			const std::string& colname,
			const kome::db::TableColumn::DataType& coltype,
			bool notNull = false,
			bool autoincrement = false);

	};

}
