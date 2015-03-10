/**
 * @file OperationManager.h
 * @brief interfaces of OperationManager class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OPERATION_MANAGER_H__
#define __KOME_OPERATION_MANAGER_H__


namespace kome {
	namespace operation {

		/**
		 * @class OperationManager
		 * @brief operation management class
		 */
		class OperationManager {
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

		public:
			/**
			 * @fn bool clear()
			 * @brief clear operations
			 * @return If true, it succeeded to clear operations.
			 */
			bool clear();

			/**
			 * @fn bool openDialog()
			 * @brief opens operation management dialog
			 * @return If true, it succeeded to open dialog.
			 */
			bool openDialog();

		public:
			/**
			 * @fn static OperationManager& getInstance()
			 * @brief OperationManager get operation management object
			 * @return operation management object (This is the only object.)
			 */
			static OperationManager& getInstance();
		};
	}
}

#endif		// __KOME_OPERATION_MANAGER_H__
