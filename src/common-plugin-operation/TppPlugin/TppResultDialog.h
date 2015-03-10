/**
 * @file TppResultDialog.h
 * @brief interfaces of TppResultDialog class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_RESULT_DIALOG_H__
#define __KOME_TPP_RESULT_DIALOG_H__

#include "TppResultGrid.h"

namespace kome {
	namespace Tpp{
		class TppResultDialog : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
		/* enum for UI ID */
		enum ENUM_CONTROL_ID_TPPRESULTDIALOG{
			ID_LIST_TPP_RESULT = 1,
			ID_BUTTON_CLOSE,
			ENUM_CONTROL_ID_NUMBER_PLUS1,
			ENUM_CONTROL_ID_NUMBER = ENUM_CONTROL_ID_NUMBER_PLUS1 - 1
		};

		public:
			/**
			 * @fn TppDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			TppResultDialog( wxWindow* parent );

			/**
			 * @fn virtual ~TppDialog()
			 * @brief destructor
			 */
			virtual ~TppResultDialog();

			/**
			 * @fn void OnInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init dialog event
			 * @param[in] evt init dialog event
			 */
			virtual void OnInitDialog( wxInitDialogEvent& evt );

			/**
			 * @fn void OnClickButtonClose( wxCommandEvent& evt )
			 * @brief clicked Close button event
			 * @param[in] evt command event object
			 */
			void OnClickButtonClose( wxCommandEvent& evt );


		private:
			/* @fn wxSizer* createMainSizer()
			 * @brief create main sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createMainSizer();

		public:

			/* dialog */
			static TppResultDialog* m_dlg;

		private:

			/* Close Button */
			wxButton* m_btnClose;

			kome::Tpp::TppResultGrid* m_listTPPResult;


			DECLARE_EVENT_TABLE();

		};
	}
}

#endif // __KOME_TPP_RESULT_DIALOG_H__
