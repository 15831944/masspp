/**
 * @file ParametersDialog.h
 * @brief interfaces of ParametersDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_PARAMETERS_DIALOG_H__
#define __KOME_VIEW_PARAMETERS_DIALOG_H__


#include "SettingsDialogBase.h"

#include <wx/wx.h>
#include <wx/propdlg.h>


class wxTreebook;


namespace kome {
	namespace view {
		/**
		 * @class ParametersDialog
		 * @brief settings dialog class
		 */
		class ParametersDialog : public SettingsDialogBase {
		public:
			/**
			 * @fn ParametersDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			ParametersDialog( wxWindow* parent );

			/**
			 * @fn virtual ~ParametersDialog()
			 * @brief destructor
			 */
			virtual ~ParametersDialog();

		protected:
			/**
			 * @fn void addPanels()
			 * @brief adds settings panels
			 */
			void addPanels();

			/**
			 * @fn unsigned int addPanels(
						wxTreebook* book,
						kome::plugin::SettingsChapter& chapter,
						unsigned int pos,
						unsigned int count
				)
			 * @brief adds settings panels
			 * @param book book object to be added panel
			 * @param chapter chapter object to add
			 * @param pos appending position
			 * @param count current count
			 */
			unsigned int addPanels(
				wxTreebook* book,
				kome::plugin::SettingsChapter& chapter,
				unsigned int pos,
				unsigned int count
			);

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_VIEW_PARAMETERS_DIALOG_H__
