/**
 * @file SettingsDialog.h
 * @brief interfaces of SettingsDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_SETTINGS_DIALOG_H__
#define __KOME_VIEW_SETTINGS_DIALOG_H__


#include "SettingsDialogBase.h"


#include <wx/wx.h>
#include <wx/propdlg.h>


namespace kome {
	namespace view {
		/**
		 * @class SettingsDialog
		 * @brief settings dialog class
		 */
		class SettingsDialog : public SettingsDialogBase {
		public:
			/**
			 * @fn SettingsDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			SettingsDialog( wxWindow* parent );

			/**
			 * @fn virtual ~SettingsDialog()
			 * @brief destructor
			 */
			virtual ~SettingsDialog();


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

#endif		// __KOME_VIEW_SETTINGS_DIALOG_H__
