/**
 * @file OperationLogDialog.h
 * @brief interfaces of OperationManager class
 *
 * @author M.Izumi
 * @date 2011.12.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_OPERATION_LOG_DIALOG_H__
#define __KOME_OPERATION_LOG_DIALOG_H__


#include <wx/wx.h>


namespace kome{
	namespace operation {
		
		class LogGridList;

		/**
		 * @class OperationLogDialog
		 * @brief OperationLog Dialog class
		 */
		class OperationLogDialog : public kome::window::StandardPanel {
		
		public:
			/**
			 * @fn OperationLogDialog()
			 * @brief constructor
			 */
			OperationLogDialog( wxWindow* parent );

			/**
			 * @fn virtual ~OperationLogDialog()
			 * @brief destructor
			 */
			virtual ~OperationLogDialog();

		protected:
			/** LogGridList class */
			LogGridList* m_listCtrl;
			
			/** remove button */
			wxButton* m_saveButton;


		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();
			
			/**
			 * @fn wxSizer* createBtnSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			wxSizer* createBtnSizer();

			/**
			 * @fn void onSave( wxCommandEvent& evt )
			 * @brief  Click the "Create Butch File" button to save the operation
			 * @param[in] evt event information
			 */
			void onSave( wxCommandEvent& evt );
	
		public:
			
			/**
			 * @fn void updateLogList()
			 * @brief Update the status of the list
			 */
			void updateLogList();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	// __KOME_OPERATION_LOG_DIALOG_H__
