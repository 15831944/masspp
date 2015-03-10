/**
 * @file OperationLOGManager.h
 * @brief interfaces of OperationManager class
 *
 * @author M.Izumi
 * @date 2011.12.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_LOG_MANAGER_H__
#define __KOME_OPERATION_LOG_MANAGER_H__

// Operation Type
#define OPERATION_TYPE_STANDARD	(kome::operation::Operation::TYPE_STANDARD)
#define OPERATION_TYPE_GUI		(kome::operation::Operation::TYPE_GUI)
#define OPERATION_TYPE_INPUT	(kome::operation::Operation::TYPE_INPUT)
#define OPERATION_TYPE_OUTPUT	(kome::operation::Operation::TYPE_OUTPUT)
#define OPERATION_TYPE_AUTO		(kome::operation::Operation::TYPE_AUTO)

/*
	TYPE_STANDARD = 0,   
	TYPE_GUI      = 1,
	TYPE_INPUT    = 2,
	TYPE_OUTPUT   = 4,
	TYPE_AUTO     = 8
*/

namespace kome {
	namespace operation {

		class OperationLogDialog;

		/**
		 * @class OperationLOGManager
		 * @brief Operation Log Manager class
		 */
		class OperationLOGManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn OperationLOGManager()
			 * @brief constructor
			 */
			OperationLOGManager();

			/**
			 * @fn virtual ~OperationLOGManager()
			 * @brief destructor
			 */
			virtual ~OperationLOGManager();

		protected:
			/** operation log dialog */
			OperationLogDialog* m_operationLogView;
			
			/** batch info */
			kome::plugin::BatchInfo* m_batchInfo;

			/** open mspXml file path */
			std::string m_path;

		public:
									
			/**
			 * @fn void setPath( const char* path )
			 * @brief set path
			 * @param path
			 */
			void setPath( const char* path ){ m_path = path; }

			/**
			 * @fn const char* getPath()
			 * @brief get path
			 * @return open mspXml file path
			 */
			const char* getPath(){ return m_path.c_str(); }
		public:
			/**
			 * @fn void openFileSaveDialog()
			 * @brief open file save dialog
			 */
			void openFileSaveDialog();

			/**
			 * @fn void openXmlFile( const char* path )
			 * @brief open xml file
			 * @param path save file path
			 */
			void openXmlFile( const char* path );

			/**
			 * @fn void saveMspXmlFile( const char* path )
			 * @brief save mspXml file
			 * @param path save file path
			 */ 
			void saveMspXmlFile( const char* path );
			
			/**
			 * @fn std::string getOperationType( int index )
			 * @brief get operation type
			 * @param index
			 * @return type
			 */
			std::string getOperationType( int index );

			/**
			 * @fn int OperationLOGManager::getOperationType( const char* type )
			 * @brief get operation type
			 * @param type
			 */
			int OperationLOGManager::getOperationType( const char* type );

			/**
			 * @fn void readOperation( 
						kome::operation::Operation* opt, 
						int state,
						std::string prevPath,
						std::string upDatePath
				   )
			 * @brief read operation
			 * @param opt
			 * @param state
			 * @param prevPath
			 * @param upDatePath
			 */
			void readOperation( 
				kome::operation::Operation* opt, 
				int state,
				std::string prevPath,
				std::string upDatePath );

		protected:
			/**
			 * @fn void writeXmlFile( FILE* fp )
			 * @brief write xml file
			 * @para fp
			 */
			void writeXmlFile( FILE* fp );
			/**
			 * @fn void writeOperations( FILE* fp )
			 * @brief write operations
			 * @param fp
			 */
			void writeOperations( FILE* fp );

			/**
			 * @fn bool writeDataTag( FILE* fp, const char* path, bool bprev )
			 * @brief write data tag
			 * @param fp
			 * @param path
			 * @param bprev
			 */
			bool writeDataTag( FILE* fp, const char* path, bool bprev );
		public:
			/**
			 * @fn void toggleOperationLogViewVisibility()
			 * @brief toggles the visibility of the OperationLog view
			 */
			void toggleOperationLogViewVisibility();

			/**
			 * @fn void createOperationLogView()
			 * @brief creates OperationLog view
			 */
			void createOperationLogView();

			/**
			 * @fn bool isVisibleOperationLogView()
			 * @brief checks the visibility of the OperationLog view
			 * @return If true, the OperationLog view is visible.
			 */
			bool isVisibleOperationLogView();

			/**
			 * @fn void UndoFunc()
			 * @brief Undo function
			 */
			void UndoFunc();

			/**
			 * @fn void RedoFunc()
			 * @brief Redo function
			 */
			void RedoFunc();

			/**
			 * @fn void UpdateOperatonLogList()
			 * @brief Update OperationLog list 
			 */
			void UpdateOperatonLogList();
			
			/**
			 * @fn void releaseView()
			 * @brief releases view
			 */
			void releaseView();
			
			/**
			 * @fn kome::plugin::BatchInfo* getBatchInfo()
			 * @brief get batch infomation
			 * @return batch info
			 */
			kome::plugin::BatchInfo* getBatchInfo();

		
		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn bool execXmlFileFromCommandLine( const char* path )
			 * @brief open xml file, translate for -operation and exec operation
			 * @param path save file path
			 */
			bool execXmlFileFromCommandLine( const char* path );

		public:
			/**
			* @fn static OperationLOGManager& getInstance()
			* @brief OperationLOGManager get operation Log management object
			* @return operation management object (This is the only object.)
			*/
			static OperationLOGManager& getInstance();
		};
	}
}

#endif		// __KOME_OPERATION_LOG_MANAGER_H__
