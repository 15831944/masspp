/**
 * @file SettingsDialogPopup.h
 * @brief interfaces of SettingsDialogPopup class
 *
 * @author OKADA,H
 * @date 2012/11/20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_SETTINGS_DIALOG_POPUP_H__
#define __KOME_VIEW_SETTINGS_DIALOG_POPUP_H__


#include "SettingsDialogBase.h"


#include <wx/wx.h>
#include <wx/propdlg.h>


namespace kome {
	namespace view {
		/**
		 * @class SettingsDialogPopup
		 * @brief settings dialog class
		 */
		class SettingsDialogPopup : public SettingsDialogBase {
		public:
			/**
			 * @fn SettingsDialogPopup( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			SettingsDialogPopup( wxWindow* parent );

			/**
			 * @fn virtual ~SettingsDialogPopup()
			 * @brief destructor
			 */
			virtual ~SettingsDialogPopup();

		private:
			/** Context Menu */
			PropertiesPageType m_PropertiesPageType;	// Header

		public:
			/**
			 * @fn void setPropertiesPageType( PropertiesPageType pt );
			 * @brief SettingsDialogPopupで表示する項目のうち、PropertiesPageTypeが指定されている物だけを表示させたい場合に使います
			 * @param pt 	PROP_SPEC_VIEW,PROP_CHROM_VIEW,PROP_HEATMAP_VIEW,PROP_3D_VIEW のいずれかを指定します
			 */
			void setPropertiesPageType( PropertiesPageType pt );

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

			/**
			 * @fn unsigned int checkPanels( kome::plugin::SettingsChapter& chapter )
			 * @brief check panels
			 * @param chapter
			 * @return 
			 */
			unsigned int checkPanels( kome::plugin::SettingsChapter& chapter );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_VIEW_SETTINGS_DIALOG_POPUP_H__
