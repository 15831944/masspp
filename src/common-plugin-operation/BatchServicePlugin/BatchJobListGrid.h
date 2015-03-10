/**
 * @file BatchJobListGrid.h
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_JOB_LIST_GRID_H__
#define __KOME_BATCH_SERVICE_BATCH_JOB_LIST_GRID_H__



/** バッチジョブリスト */
typedef struct{
	std::string strStatus;	// バッチ現在の状態
	std::string strDate;	// バッチ実行を行った日時 
	std::string strName;	// 
	std::string strServer;	// 
	std::string strComment;	// 
}BatchJobList;


namespace kome {
	namespace batchservice {

		class BatchJobListGrid : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn BatchJobListGrid( wxWindow* parent )
			 * @brief コンストラクタ
			 * @param parent
			 */
			BatchJobListGrid( wxWindow* parent );	

			/**
			 * @fn virtual ~BatchJobListGrid()
			 * @brief デストラクタ
			 */
			virtual ~BatchJobListGrid();  			

		protected:
			/** バッチジョブリスト */
			std::vector<BatchJobList>  m_vectBatchJobLists;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief テーブル作成
			 */
			virtual void onCreate();			
						
			/**
			 * @fn virtual int getInt( const long long data, const int col ) const
			 * @brief 整数値取得
			 * @param data
			 * @param col
			 */
			virtual int getInt( const long long data, const int col ) const;
			
			/**
			 * @fn virtual std::string getString( const long long data, const int col ) const
			 * @brief 文字列取得
			 * @param data
			 * @param col
			 */
			virtual std::string getString( const long long data, const int col ) const;
			
		};
	}
}


























#endif
