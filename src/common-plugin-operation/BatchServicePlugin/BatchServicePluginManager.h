/**
 * @file BatchServicePluginManager.h
 * @brief implements of plug-in functions
 *
 * @author okada_h
 * @date 2012/02/15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_PLUGIN_MANAGER_H__
#define __KOME_BATCH_SERVICE_PLUGIN_MANAGER_H__


enum stage_no {
    SN_INITIALIZE,		// ①Initialize
    SN_FILTER,			// ②
    SN_MANIPULATE,		// ③
    SN_ANALYZE,			// ④
    SN_IDENTIFY,		// ⑤
    SN_FILTER_RESULTS,	// ⑥Filter Results
	SN_SENTINEL			// 番兵
};

extern char* g_aszStageString[];

/** アクション */
typedef struct{
	stage_no	nStage;			// ステージ番号。①Initialize～⑥FilterResults
	int			nPrecedence;	
	std::string	strName;		// long name
	std::string	strNameShort;	// short name
	int nBatchIndex;			// Plugin ManagerからgetFunctionItem()で読み取る際の番号
								// plgMgr.getFunctionItem( "batch", i );←の「i」に相当
	int flgSettings;			// 1=settings ok

	std::string strSettings;	// kome::plugin::SettingsPage * page->getParameterString( settings );	の返り値を保存
	std::vector<std::string> vec_strSettings;	// パラメータ設定(xmlファイル用) // @date 2013/03/28 <Add> OKADA

}Action;


namespace kome {
	namespace batchservice {

		class BatchServicePluginManager {
		protected:
			/**
			 * @fn BatchServicePluginManager()
			 * @brief constructor
			 */
			BatchServicePluginManager();

			/**
			 * @fn virtual ~BatchServicePluginManager()
			 * @brief destructor
			 */
			virtual ~BatchServicePluginManager();
		public:

			/** Available Actions */
			std::vector<Action>	m_vAvailableActions;	

			/** Selected Actions */
			std::vector<Action>	m_vSelectedActions;	

			/** Actions, read from XML file */
			std::vector<Action>	m_vectXmlActions;

		private:
			/** batch info */
			kome::plugin::BatchInfo   m_batchinfo;

		protected:
			/** database */
			kome::db::Database* m_db;

		private:		
			// 出力先ディレクトリ
			std::string m_strOutputDirBatchProcessingSubmitJob;	// @date 2012/08/09 <Add> OKADA
	
		public:
			/**
			 * @fn void initBatchServicePluginManager( void )
			 * @brief init batch service plugin manager
			 */
			void initBatchServicePluginManager( void );

			/**
			 * @fn stage_no getStageNoFromStepString( const char * pszStep )
			 * @brief get stage no from step string
			 * @param psStep
			 * @return stage_no
			 */
			stage_no getStageNoFromStepString( const char * pszStep );

			/**
			 * @fn int setAvailableActions( Action actDat )
			 * @brief set available action
			 * @param actDat
			 * @return
			 */
			int setAvailableActions( Action actDat );

			
			/**
			 * @fn std::vector<Action>	sortActionsByPrecedence( std::vector<Action>	vActions )
			 * @brief sort action by precedence
			 * @param vActions
			 * @return std::vector<Action>
			 */
			std::vector<Action>	sortActionsByPrecedence( std::vector<Action>	vActions );
			
			/**
			 * @fn void sortActionsByPrecedenceRef( std::vector<Action>&	vActions )
			 * @brief sort action by precedence ref
			 * @param vActions
			 */
			void sortActionsByPrecedenceRef( std::vector<Action>&	vActions );
			
			/**
			 * @fn int getCountOfAvailableActions( void )
			 * @brief get count of available actions
			 * @return
			 */
			int getCountOfAvailableActions( void );
			
			/**
			 * @fn int getAvailableAction( int nCnt, Action * action )
			 * @brief get available action
			 * @param nCnt
			 * @param action
			 */
			int getAvailableAction( int nCnt, Action * action );
			
			/** 
			 * @fn std::string getBatchJobPath( void )
			 * @brief get batch job path
			 * @return
			 */
			std::string getBatchJobPath( void );

			/**
			 * @fn kome::objects::SettingParameterValues* getSettingParameterValues( int nIndexOfBatch );
			 * @brief 各バッチメソッドが記録されたxmlファイル名の一覧を得る
			 * @param[in] nIndexOfBatch index of batch
			 * @return batch processing parameter
			 */
			std::vector<std::string> getSettingXmlFiles( void );

			/**
			 * @fn kome::objects::SettingParameterValues* getSettingParameterValues( int nIndexOfBatch );
			 * @brief A batch processing parameter is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return batch processing parameter
			 */
			kome::objects::SettingParameterValues* getSettingParameterValues( int nIndexOfBatch );

			/**
			 * @fn kome::plugin::BatchInfo& getBatchiInfo( void )
			 * @brief 保存されたBatchInfoを取得
			 * @return BatchInfo
			 */ 
			kome::plugin::BatchInfo& getBatchiInfo( void );

			/**
			 * @fn int exportBatchXML( const char* szMethodName,  std::vector<Action> vectActions );
			 * @brief 
			 * @param[in] XML file name(full path)
			 * @return always 0
			 */
			int exportBatchXML( const char* szMethodName,  std::vector<Action> vectActions );
			
			/**
			 * @fn int readXML( const char * szXmlFilePath );
			 * @brief 最後に選択したxmlファイルの名前(BatchMethod名)をiniファイルに保存する
			 * @param[in] XML file name(full path)
			 * @return always 0
			 */
			int saveLastRecordXmlFilename( const char * szXMLFilename );

			/**
			 * @fn int readXML( const char * szXmlFilePath );
			 * @brief 最後に選択したBatchMethod名をiniファイルから取得する
			 * @return batch method name
			 */
			const char * getLastRecordXmlFilename( void );	// @date 2013/05/22 <Add> OKADA

			/**
			 * @fn int readXML( const char * szXmlFilePath );
			 * @brief 
			 * @param[in] XML file name(full path)
			 * @return always 0
			 */
			int BatchServicePluginManager::readXML( const char * szXmlFilePath );

			/**
			 * @fn std::vector<Action> getVectXmlActions( void );
			 * @brief get Actions(read from XML file)
			 * @return Actions
			 */
			std::vector<Action> getVectXmlActions( void );

		protected:
			/**
			 * @fn kome::db::Database* getDb()
			 * @brief gets the database object
			 * @return database object
			 */
			kome::db::Database* getDb();

		public:
			/**
			 * @fn void registerBatchInfo(
						kome::plugin::BatchInfo& batchInfo,
						kome::plugin::PluginFunctionItem* server,
						const char* id
					)
			 * @brief registers batch information
			 * @param[in] batch information object.
			 * @param[in] server batch server
			 * @param[in] id job ID
			 */
			void registerBatchInfo(
				kome::plugin::BatchInfo& batchInfo,
				kome::plugin::PluginFunctionItem* server,
				const char* id
			);

		public:
			/**
			 * @fn void getJobs( std::vector< int >& jobs )
			 * @brief gets jobs IDs
			 * @param[out] the array object to store job IDs.
			 */
			void getJobs( std::vector< int >& jobs );

			/**
			 * @fn std::string getJobTitle( const int jobId )
			 * @brief gets the job title
			 * @param[in] jobId job ID
			 * @return job title
			 */
			std::string getJobTitle( const int jobId );

			/**
			 * @fn std::string getJobComment( const int jobId )
 			 * @brief gets the job comment
			 * @param[in] jobId job ID
			 * @return job comment
			 */
			std::string getJobComment( const int jobId );

			/**
			 * @fn std::string getJobDate( const int jobId )
			 * @brief gets the job date
			 * @param[in] jobId job ID
			 * @return job date
			 */
			std::string getJobDate( const int jobId );

			/**
			 * @fp std::string getJobString( const int jobId )
			 * @brief gets the job string
			 * @param[in] jobId job ID
			 * @retrun job string
			 */
			std::string getJobString( const int jobId );

			/**
			 * @fn kome::plugin::BatchService::BatchStatus getJobStatus( const int jobId )
			 * @brief gets the job status
			 * @param[in] jobId job ID
			 * @return job status
			 */
			kome::plugin::BatchService::BatchStatus getJobStatus( const int jobId );

			/**
			 * @fn int getJobServer( const int jobId )
			 * @brief gets the job server
			 * @param[in] jobId job ID
			 * @return the job server ID
			 */
			int getJobServer( const int jobId );

			/**
			 * @fn std::string getServerName( const int serverId )
			 * @brief gets the server name
			 * @param[in] serverId server ID
			 * @return server name
			 */
			std::string getServerName( const int serverId );

			/**
			 * @fn std::string getServerDescription( const int serverId )
			 * @brief gets the server description
			 * @param[in] serverId server ID
			 * @return server description
			 */
			std::string getServerDescription( const int serverId );

			/**
			 * @fn void updateJobStatus( const int jobId, kome::plugin::BatchService::BatchStatus status )
			 * @brief updates job status
			 * @parma[in] jobId job ID
			 * @param[in] status job status
			 */
			void updateJobStatus( const int jobId, kome::plugin::BatchService::BatchStatus status );
			
			/**
			 * @fn void deleteJob( const int jobId )
			 * @brief deletes job
			 * @param[in] jobId job ID
			 */
			void deleteJob( const int jobId );

			/**
			 * @fn void updateStatuses()
			 * @brief updates statuses
			 */
			void updateStatuses();

		protected:
			/**
			 * @fn int registerServer( kome::plugin::PluginFunctionItem* server )
			 * @brief registers server
			 * @param[in] server server object
			 * @return sever ID
			 */
			int registerServer( kome::plugin::PluginFunctionItem* server );

			/**
			 * @fn int registerJob(
					const char* title,
					const char* comment,
					const int serverId,
					const char* jobId
				)
			 * @brief registers job
			 * @param[in] title job title
			 * @param[in] comment job comment
			 * @param[in] serverId server ID
			 * @param[in] jobId job ID
			 * @return job ID
			 */
			int registerJob(
				const char* title,
				const char* comment,
				const int serverId,
				const char* jobId
			);

			/**
			 * @fn int registerFormat( kome::plugin::PluginFunctionItem* outFun )
			 * @brief registers output format
			 * @param[in] outFun output function object
			 * @return format ID
			 */
			int registerFormat( kome::plugin::PluginFunctionItem* outFun );

			/**
			 * @fn int registerFilePath( const char* path, const int fmtId, const int jobId, const int inputId )
			 * @brief register file path
			 * @param[in] path file path
			 * @param[in] fmtId format ID
			 * @param[in] jobId job ID
			 * @parma[in] inputId input ID
			 * @return file path ID
			 */
			int registerFilePath( const char* path, const int fmtId, const int jobId, const int inputId );

			/**
			 * @fn int registerFunction( kome::plugin::PluginFunctionItem* fun )
			 * @brief registers function
			 * @param[in] fun function object
			 * @return function ID
			 */
			int registerFunction( kome::plugin::PluginFunctionItem* fun );

			/**
			 * @fn int registerCall( int jobId, int funId, const char* param )
			 * @brief registers call
			 * @param[in] jobId job ID
			 * @param[in] funId function ID
			 * @param[in] param parameter
			 * @return call ID
			 */
			int registerCall( int jobId, int funId, const char* param );

		public:
			/**
			 * @fn void setDirBatchProcessingSubmitJob( std::string );
			 * @brief write default directory to INI file
			 * @param[in] std::string strDir default directory
			 */
			void setDirBatchProcessingSubmitJob( std::string strDir );	// @date 2012/08/09 <Add> OKADA

			/**
			 * @fn std::string getDirBatchProcessingSubmitJob( void );
			 * @brief read default directory from INI file
			 * @return default directory
			 */
			std::string getDirBatchProcessingSubmitJob( void );	// @date 2012/08/09 <Add> OKADA

			/**
			 * @fn std::string getOutputFineName( std::string strInputFileName, std::string strTime, std::string strExt, int nIndex );
			 * @brief get output filename
			 * @param[in] strInputFileName input file name
			 * @param[in] strTime          time string
			 * @param[in] strExt           extension string
			 * @param[in] nIndexOfInputFile index data(line)
			 * @param[in] nIndexOfFormat   index data(format)
			 * @return 0
			 */
			std::string getOutputFineName( std::string strInputFileName, std::string strTime, std::string strOutExt, int nIndexOfInputFile, int nIndexOfFormat );

			/**
			 * @fn void BatchServicePluginManager::dispBatchJobWindow void );
			 * @brief display batch Job Window
			 * @param[in] flgDisp false=toggle mode true:always display
			 */
			void BatchServicePluginManager::dispBatchJobWindow( bool flgDisp=false );

			/**
			 * @fn static BatchServicePluginManager& getInstance()
			 * @brief gets BatchServicePluginManager object
			 * @return BatchServicePluginManager object. (This is the only object.)
			 */
			static BatchServicePluginManager& getInstance();

		};
	}
}


#endif
