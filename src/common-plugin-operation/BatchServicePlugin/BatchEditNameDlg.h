/**
 * @file BatchEditNameDlg.h
 * @brief interfaces of BatchEditNameDlg class
 *
 * @author OKADA, H
 * @date 2012-04-19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_NEW_BATCH_EDIT_NAME_DLG_H__
#define __KOME_BATCH_SERVICE_NEW_BATCH_EDIT_NAME_DLG_H__

#include "BatchProcessingMethodConfigurationDialog.h"

namespace kome {
	namespace batchservice {
		class BatchBase;

		class BatchEditNameDlg : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
			
		public:
			/**
			 * @fn BatchEditNameDlg( wxWindow* parent, void *pParent )
			 * @brief constructor
			 * @param parent
			 * @param pParent
			 */
			BatchEditNameDlg( wxWindow* parent, void *pParent );

			/**
			 * @fn virtual ~BatchEditNameDlg()
			 * @brief destructor
			 */
			virtual ~BatchEditNameDlg();

		private:
			/** name ctrl */
			wxTextCtrl* m_pNameCtrl;

			/** method name */
			std::string m_strMethodName;

			/** parent dialog */
			BatchProcessingMethodConfigurationDialog * m_pParentDlg;

			/** current method parent */
			std::string m_strCurrentMethodParent;

		public:
			/**
			 * @fn wxSizer* createMainSizer()
			 * @brief create main sizer
			 * @return main sizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn bool onOK()
			 * @brief on ok
			 * @return 
			 */
			bool onOK();

			/**
			 * @fn std::string getNewMethodName()
			 * @brief get new method name
			 */
			std::string getNewMethodName();
			
		
		};
	};
}

#endif
