/**
 * @file TppDialog.h
 * @brief interfaces of TppDialog class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_DIALOG_H__
#define __KOME_TPP_DIALOG_H__

#include <wx/wx.h>
#include "TppSearchGrid.h"

namespace kome {
	namespace Tpp{
		class TppDialog : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki

		/* enum for UI ID */
		enum ENUM_CONTROL_ID_TPPDIALOG{
			ID_BUTTON_APPLY = 1,
			ID_BUTTON_CLOSE,
			ID_CHECK_ALL,
			ID_LIST_SEARCH_RESULT,
			ID_TEXTCONTROL_COMMENT,
			ENUM_CONTROL_ID_NUMBER_PLUS1,
			ENUM_CONTROL_ID_NUMBER = ENUM_CONTROL_ID_NUMBER_PLUS1 - 1
		};

		public:
			/**
			 * @fn TppDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			TppDialog( wxWindow* parent );

			/**
			 * @fn virtual ~TppDialog()
			 * @brief destructor
			 */
			virtual ~TppDialog();

		protected:
			/**
			 * @fn void OnClickButtonApply( wxCommandEvent& evt )
			 * @brief clicked Apply button event
			 * @param[in] evt command event object
			 */
			void OnClickButtonApply( wxCommandEvent& evt );

			/**
			 * @fn void OnClickButtonClose( wxCommandEvent& evt )
			 * @brief clicked Close button event
			 * @param[in] evt command event object
			 */
			void OnClickButtonClose( wxCommandEvent& evt );

			/**
			 * @fn void OnClickCheckAll( wxCommandEvent& evt )
			 * @brief clicked All Checkbox event
			 * @param[in] evt command event object
			 */
			void OnClickCheckAll( wxCommandEvent& evt );

		// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
		protected:
			/* @fn vertual wxSizer* createMainSizer()
			 * @brief create main sizer
			 * @return pointer of wxSizer
			 */
			virtual	wxSizer* createMainSizer();
		// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

		public:

			/* dialog */
			static TppDialog* m_dlg;

		private:

			/* Apply Button */
			wxButton* m_btnApply;

			/* Close Button */
			wxButton* m_btnClose;

			wxCheckBox* m_checkAll;

			/* Comment Box */
			wxTextCtrl* m_textComment;

			/* Search result List */
			TppSearchGrid* m_listSearchResult;

			DECLARE_EVENT_TABLE();

		};
	}
}

#endif // __KOME_TPP_DIALOG_H__
