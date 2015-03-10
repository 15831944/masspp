/**
 * @file BatchProcessingSubmitJobGrid.h
 * @brief interfaces of BatchProcessingSubmitJobGrid class
 *
 * @author OKADA, H
 * @date 2012-02-29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_PROCESSING_SUBMIT_JOB_GRID_H__
#define __KOME_BATCH_SERVICE_BATCH_PROCESSING_SUBMIT_JOB_GRID_H__

// 主にBatchProcessingSubmitJobDlgクラスで使います
typedef struct{
	/** 入力ファイル */
	std::vector< std::string > vectstrPaths;
	/** 出力ファイル時刻情報 */
	std::vector< std::string > vectstrOutputFilenameInfo;
	/** Type名(short name) */
	std::vector< std::string > vectstrTypeName;
}stIOInf;


/** バッチジョブ・Input/Output file String */
typedef struct{
	std::string strTypeName;
	std::string strInput;			// 
	std::vector<std::string> strOutput;	// 
}IOSettingsList;


namespace kome {
	namespace batchservice {
		
		class BatchProcessingSubmitJobGrid : public kome::window::GridListCtrl {
		public:
			
			/**
			 * @fn BatchProcessingSubmitJobGrid( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			BatchProcessingSubmitJobGrid( wxWindow* parent );	// コンストラクタ

			/**
			 * @fn virtual ~BatchActionGrid()
			 * @brief destructor
			 */
			virtual ~BatchProcessingSubmitJobGrid();  			// デストラクタ
	
		private:
			/** Type */
			std::vector< std::string > m_vectstrTypes;	// @date 2012/08/07 <Add> OKADA

			/** 入力ファイル */
			std::vector< std::string > m_vectstrPaths;

			/** 出力ファイル時刻情報 */
			std::vector< std::string > m_vectstrOutputFilenameInfo;

			/** select format */
			std::string m_selectFormatExt;
		private:

			/** parent window */
			wxWindow* m_pParent;

			/** Input/Output file String */
			std::vector<IOSettingsList> m_vecIOSettingsList;
					
		public:
			/**
			 * @fn void setFormatExt( const char* format )
			 * @brief set format ext
			 * @param format
			 */
			void setFormatExt( const char* format ){ m_selectFormatExt = format; }

			/**
			 * @fn const char* getFormatExt()
			 * @brief get format ext
			 * @return format ext
			 */
			const char* getFormatExt(){ return m_selectFormatExt.c_str(); }

		public:
			/**
			 * @fn void onCreate();
			 * @brief 初期化時に呼ばれます
			 */
			void onCreate();

			/**
			 * @fn void doCreate();
			 * @brief INIファイルから出力フォーマットの種類を読み出して前回実行時の設定を再現する
			 */
			void doCreate();
						
		private:
			/**
			 * @fn std::string getString( const long long data, const int col ) const
			 * @brief 文字列取得
			 * @return
			 */
			std::string getString( const long long data, const int col ) const;
						
			/**
			 * @fn int getInt( const long long data, const int col ) const
			 * @brief 整数値取得
			 * @return
			 */
			int getInt( const long long data, const int col ) const;

			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked. (タイトルをクリックしても何も起きないようにする)
			 * @param[in] evt list event information
			 */
			void onColClick( wxListEvent& evt );

			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is double clicked. 
			 * @param[in] evt list event information
			 */
			void onDoubleClick( wxListEvent& evt );

		public:

			/**
			 * @fn void setVectStrPaths( stIOInf stIoInf );
			 * @brief 入出力ファイルの情報を、表示用配列に詰め直す
			 * @param[in] stIoInf 入出力情報
			 */
			void setVectStrPaths( stIOInf stIoInf );

		public:
			/**
			 * @fn int getCountOfVectPaths( void )
			 * @brief This method is return m_vectstrPaths's size
			 */
			int getCountOfVectPaths( void ){ 
				return m_vectstrPaths.size(); 
			};
			
		private:
			DECLARE_EVENT_TABLE()

		};
	}
}





#endif
