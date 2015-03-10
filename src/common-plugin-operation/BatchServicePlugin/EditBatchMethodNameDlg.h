/**
 * @file EditBatchMethodNameDlg.h
 * @brief interfaces of EditBatchMethodNameDlg class
 *
 * @author OKADA, H
 * @date 2012-03-05
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__


#define __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__


namespace kome {
	namespace batchservice {
		class BatchBase;

		class EditBatchMethodNameDlg : public kome::window::StandardDialog {
		public:
			EditBatchMethodNameDlg(
				wxWindow* parent
			);
			
			virtual ~EditBatchMethodNameDlg();

			wxSizer* createMainSizer();
			wxSizer* createButtonSizer();

		private:
			wxTextCtrl* m_nameCtrl;

		public:
			/** for debug */
			int m_isDestructed;

		private:
			DECLARE_EVENT_TABLE()

		};
	}
}



#endif
