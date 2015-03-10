/*********************************************************/
/*!
 @addtoGroup AB3D
 @file TempTableCtrl.h
 @brief Temp Table Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
   Peak Position化にあたり、AB3Dでの結果を
   保存、読み出すためのテーブルを作成する
 */
/*********************************************************/
#pragma once

// 前方宣言
namespace AB3D {
	class DbAccess;
	struct TmpPeakVal;
	struct TmpMXVal;
}

namespace AB3D {

/*!--------------------------------------------------------
    @name   TmpTblCtrl
    @brief  AB3Dの結果を一時保管するTempTableの操作
    @date   2013.07.01
    @par    TempTableの一連の操作を担う
-----------------------------------------------------------*/
	class TmpTblCtrl {
		
	public:
		// constructor
		TmpTblCtrl();
		// destructor
		~TmpTblCtrl();

	private:
		// Uncopyable
		TmpTblCtrl(const TmpTblCtrl&);
		TmpTblCtrl& operator=(const TmpTblCtrl&);

	private:
		std::unique_ptr<DbAccess> ptr_;

		// STMTを作成して保持しておく。
		// ユニークポインタで管理するので、破棄は自動的にされる。
		// // Tempテーブルへのインサート(Set)
		std::unique_ptr<kome::db::Statement> stmt_tmpPeak_;
		std::unique_ptr<kome::db::Statement> stmt_tmpVal_;
		std::unique_ptr<kome::db::Statement> stmt_tmpProf_;

		// // Tempテーブルへのアップデート
		std::unique_ptr<kome::db::Statement> stmt_UpdtCrg_;

		// // Tempテーブルから Select(Get)
		std::unique_ptr<kome::db::Statement> stmt_SlctVal_;
		std::unique_ptr<kome::db::Statement> stmt_SlctProf_;

	public:
		const int createTempTable();

		const bool createTempStatements();

		// テーブル生成関数。DBAccessに関数として渡す
		static void createTempTableAB3DPeak(kome::db::Table& table, const char* tname);
		static void createTempTableAB3DVal(kome::db::Table& table, const char* tname);
		static void createTempTableAB3DProf(kome::db::Table& table, const char* tname);

		// XYBufferをTmpに記録。成功時は０以上のIDを戻す
		const int setXYBuffer(kome::core::Buffer* buffer);

		// Temp Matrix Value にInsert
		const int setTmpMXValue(
			const int sampleid,   
			const int bufid,
			const double peak_x,  
			const double peak_y,
			const double left_x,  
			const double left_y,
			const double right_x, 
			const double right_y,
			const double value
			);

		// Temp Peak にInsert
		const int setTmpPeak(
			const int sampleid,
			const int mxvid,
			const double rt,
			const double mz
			);

		// 外からXYBufferを取り出される
		const bool getXYBuffer(
			const int bufId,
			kome::core::Buffer& refBuffer
			);

		// Tempに吐き出した 有効な値のみを取得する
		const bool getTemporaryDatas(
			std::vector<TmpPeakVal*>& refTmpPeakVals);

		// Temp Peak の　ChargeをUpdate.
		void updateCharge(const int peakId, const int charge);

		void closeMe();

	private:
		// InsertのStmtの生成。一度作ってすべて使いまわす。
		const bool onCreateStmt_Insert_TmpPEAKS();
		const bool onCreateStmt_Insert_TmpMXVALUES();
		const bool onCreateStmt_Insert_TmpPROFILES();

		// UpdateのStmtの生成。一度作ってすべて使いまわす。
		const bool onCreateStmt_UpdateCharge();

		// SelectのStmtの生成。一度作ってすべて使いまわす。
		const bool onCreateStmt_SelectTmpMXVALUES();
		const bool onCreateStmt_SelectTmpPROFILES();

		//BUFFERをTEMPTABLEに登録。
		const bool onDoInsertBuffer(kome::core::Buffer* buffer);

		// Temp Profile テーブルの最新IDを取得
		const int onGetMaxID_TmpPROFILES();

		//MatrixValueをTEMPTABLEに登録。
		const bool onSetTmpMXValue(
			const int sampleid,
			const int bufid,
			const double peak_x,
			const double peak_y,
			const double left_x,
			const double left_y,
			const double right_x,
			const double right_y,
			const double value
			);
		
		// Temp Matrix Value テーブルの最新IDを取得
		const int onGetMaxID_TmpMXVALUES();

		//PeakValueをTEMPTABLEに登録。
		const bool onSetTmpPeak(
			const int sampleid,
			const int mxvid,
			const double rt,
			const double mz
			);

		// TempPeakテーブルの最新IDを取得
		const int onGetMaxID_TmpPEAKS();

		// 結果取得用のSQL文を構成して返す
		const std::string onCreateSQL_TempResults();

		const bool onGetTmpMXValue(
			const int mxvId,
			TmpMXVal& refMxval
			);


	};

}
