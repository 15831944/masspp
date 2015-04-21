/**
 * @file SettingsHelpPanel.h
 * @brief interfaces of SettingsHelpPanel class
 *
 * @author S.Tanaka
 * @date 2013.01.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_SETTINGS_HELP_PANEL_H__
#define __KOME_WINDOW_SETTINGS_HELP_PANEL_H__


#include "ParametersAndHelpPanel.h"

#include <wx/wx.h>


namespace kome {
	namespace window {

		class SettingsPanel;

		/**
		 * @class SettingsHelpPanel
		 * @brief each settings panel class
		 */
		class WINDOW_MANAGER_CLASS SettingsHelpPanel : public ParametersAndHelpPanel {
		public:
			/**
			 * @fn SettingsHelpPanel(
					wxWindow* parent,
					kome::plugin::SettingsPage* page,
					const bool dfltBtn = true,
					kome::objects::SettingParameterValues* settings = NULL,
					const bool saveVal = true
				)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] page settings page information
			 * @param[in] dfltBtn default button flag
			 * @param[in] settings the object to store settings values
			 * @param[in] saveVal save value flag
			 */
			SettingsHelpPanel(
				wxWindow* parent,
				kome::plugin::SettingsPage* page,
				const bool dfltBtn = true,
				kome::objects::SettingParameterValues* settings = NULL,
				const bool saveVal = true
			);

			/**
			 * @fn virtual ~SettingsHelpPanel()
			 * @brief destructor
			 */
			virtual ~SettingsHelpPanel();

		protected:
			/** page */
			kome::plugin::SettingsPage* m_page;

			/** default button flag */
			bool m_dfltBtn;

			/** settings */
			kome::objects::SettingParameterValues* m_settings;

			/** save value flag */
			bool m_saveVal;

			/** panel */
			SettingsPanel* m_panel;

		public:
			/**
			 * @fn SettingsPanel* getSettingsPanel()
			 * @brief gets the settings panel
			 * @return settings panel
			 */
			SettingsPanel* getSettingsPanel();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_SETTINGS_HELP_PANEL_H__
