/**
 * @file OperationManager.h
 * @brief implements of GridEmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.12.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOM_OPERATION_OPERATION_MANAGER_H__
#define __KOM_OPERATION_OPERATION_MANAGER_H__

#include "stdafx.h"
#include "Operation.h"

#include <vector>

namespace kome {
	namespace operation {
		
		class Operation;

		/**
		 * @class OperationManager
		 * @brief operation management class
		 */
		class DATA_OPERATION_CLASS OperationManager {			
		protected:
			/**
			 * @fn OperationManager()
			 * @brief constructor
			 */
			OperationManager();

			/**
			 * @fn virtual ~OperationManager()
			 * @brief destructor
			 */
			virtual ~OperationManager();

		protected:
			/** update file path */
			std::string m_updateFilePath;

			/** previous file path */
			std::string m_prevFilePath;

			/**
			 * @typedef OperationState
			 * @brief the operation state type of array
			 */
			typedef enum {
				TYPE_NONE = 5,
				TYPE_FINISH = 0,
				TYPE_CANCEL = 1
			} OperationState;
					
			/**
			 * @struct OperationInfo
			 * @brief operation information
			 */
			struct OperationInfo{
				Operation* operation;
				int iState;
			};
			
			/** operation infomation array */
			std::vector<OperationInfo> m_operations;
			
			/** save count */
			int m_saveCount;
			
			/** save count flg */
			bool m_saveFlg;

			/** cancel flg */
			bool m_cancelFlg;
		public:
			/**
			 * @fn void addOperation( Operation* operation  )
			 * @brief add managed operation
			 * @param[in] operation Operation object
			 */
			void addOperation( Operation* operation  );
			
			/**
			 * @fn void removeOperation( Operation* operation )
			 * @brief remove managed operation
			 * @param[in] operation operation object
			 */
			void removeOperation( Operation* operation  );
			
			/**
			 * @fn unsigned int getNumberOfOperations()
			 * @brief gets the number of managed operation
			 * @return the number of operations
			 */
			unsigned int getNumberOfOperations();
			
			/**
			 * @fn Operation* getOperation( const unsigned int index )
			 * @brief gets the managed operation
			 * @param[in] index managed operation index
			 * @return managed operation
			 */
			Operation* getOperation( const unsigned int index );

			/**
			 * @fn int getNumberOfFinishedOperations()
			 * @brief gets the number of finished operations
			 * @return the number of finished operations
			 */
			int getNumberOfFinishedOperations();

			/**
			 * @fn Operation* getFinishedOperation( int i )
			 * @brief gets the finished operation.
			 * @param[in] i finished operation index
			 * @return finished operation
			 */
			Operation* getFinishedOperation( int i );
			
			/**
			 * @fn int getNumberOfCanceledOperations()
			 * @brief gets the number of canceled operations
			 * @return the number of canceled operations
			 */
			int getNumberOfCanceledOperations();
			
			/**
			 * @fn Operation* getCanceledOperation( int i )
			 * @brief gets the canceled operation
			 * @param[in] i canceled operation index
			 * @return canceled operation
			 */
			Operation* getCanceledOperation( int i );

			/**
			 * @fn const char* getUpdateDataFilePath( Operation* operation )
			 * @param[in] operation operation
			 * @brief the file path to save the update status
			 * @return file path to save the update status
			 */
			const char* getUpdateDataFilePath( Operation* operation );
			
			/**
			 * @fn const char* getPreviousDataFilePath( Operation* operation )
			 * @brief gets the file path to save the previous status
			 * @param[in] operation operation
			 * @return the file path to save the previous status
			 */
			const char* getPreviousDataFilePath( Operation* operation );
			
			/**
			 * @fn void moveToFinished( Operation* operation )
			 * @brief moves the operation to the finished queue.
			 * @param[in] operation operation to be moved
			 */
			void moveToFinished( Operation* operation );
			
			/**
			 * @fn void movetoCanseled( Operation* operation )
			 * @brief move the operation to the canceled queue.
			 * @param[in] operation operation to be moved.
			 */
			void movetoCanseled( Operation* operation );
			
			/**
			 * @fn void clearCanceledOperation()
			 * @brief clears canceled operations
			 */
			void clearCanceledOperation();
			
			/**
			 * @fn void setState( const unsigned int index, int state )
			 * @brief sets the operation stateus
			 * @param[in] index
			 * @param[in] state operation status
			 */
			void setState( const unsigned int index, int state );

			/**
			 * @fn int getState( const unsigned int index )
			 * @brief gets the operation status
			 * @param[in] index
			 * @return operation status
			 */
			int getState( const unsigned int index );

			/**
			 * @fn static OperationManager& getInstance()
			 * @brief get operation manager object (This is the only object.)
			 * @return operation manager object
			 */
			static OperationManager& getInstance();

			/**
			 * @fn void setSaveCount( int count )
			 * @brief set the save count
			 * @param count
			 */
			void setSaveCount( int count ){ m_saveCount = count; }

			/**
			 * @fn int getSaveCount()
			 * @brief get the save count
			 * @return save count
			 */
			int getSaveCount(){ return m_saveCount; }

			/**
			 * @fn void setSaveCountFlg( bool bSave )
			 * @brief set the save count flag
			 * @param bSave
			 */
			void setSaveCountFlg( const bool bSave ){ m_saveFlg = bSave; }
			
			/**
			 * @fn bool getSaveCountFlg()
			 * @brief get the save count flag
			 * @return save flag
			 */
			bool getSaveCountFlg(){ return m_saveFlg; }

			/**
			 * @fn void setSaveCancelFlg( const bool bCancel )
			 * @brief set the save cancel flag
			 * @param bCancel cancel flag
			 */
			void setSaveCancelFlg( const bool bCancel ){ m_cancelFlg = bCancel; }

			/**
			 * @fn bool getSaveCancelFlg()
			 * @brief get the save cancel flag
			 * @return cancel flag
			 */
			bool getSaveCancelFlg(){ return m_cancelFlg; }
		private:

			/**
			 * @fn void SendStatusToList()
			 * @brief sends status to the list
			 */
			void SendStatusToList();

		};
	}
}

#endif // __KOM_OPERATION_OPERATION_MANAGER_H__
