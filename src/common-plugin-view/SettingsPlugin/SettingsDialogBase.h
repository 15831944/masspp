/**
 * @file SettingsDialogBase.h
 * @brief interfaces of SettingsDialogBase class
 *
 * @author S.Tanaka
 * @date 2013.02.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_SETTINGS_DIALOG_BASE_H__
#define __KOME_VIEW_SETTINGS_DIALOG_BASE_H__


#include <wx/wx.h>
#include <wx/propdlg.h>


class wxTreebook;


namespace kome {
	namespace view {
		/**
		 * @class SettingsDialogBase
		 * @brief settings dialog class
		 */
		class SettingsDialogBase : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn SettingsDialogBase( wxWindow* parent, const char* title )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 */
			SettingsDialogBase( wxWindow* parent, const char* title );

			/**
			 * @fn virtual ~SettingsDialogBase()
			 * @brief destructor
			 */
			virtual ~SettingsDialogBase();

		protected:
			/** tree book */
			wxTreebook* m_book;

		protected:
			/**
			 * @fn virtual void addPanels()
			 * @brief adds settings panels
			 */
			virtual void addPanels();

		protected:
			/**
			 * @fn void updateSettings()
			 * @brief updates settings
			 */
			void updateSettings();

		protected:		
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn virtual bool onOK()
			 * @brief This method is called when the "OK" button is clicked and values in the window is valid.
			 * @return If true, this window is closed.
			 */
			virtual bool onOK();

			/**
			 * @fn virtual bool onCancel()
			 * @brief This method is called when the "Cancel" button is clicked.
			 * @brief If true, this window is closed.
			 */
			virtual bool onCancel();

		protected:
			/**
			 * @fn void onApply( wxCommandEvent& evt )
			 * @brief This method is called when the Apply button is clicked.
			 * @param[in] evt event
			 */
			void onApply( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};


		/**
		 * @class SettingsManager
		 * @brief dialog management class
		 */
		class SettingsManager {
		protected:
			/**
			 * @fn SettingsManager()
			 * @brief constructor
			 */
			SettingsManager();

			/**
			 * @fn virtual ~SettingsManager()
			 * @brief destructor
			 */
			virtual ~SettingsManager();

		protected:
			/** dialog */
			SettingsDialogBase* m_dlg;

		public:
			/**
			 * @fn void setDialog( SettingsDialogBase* dlg )
			 * @brief constructor
			 * @parma[in] dlg dialog
			 */
			void setDialog( SettingsDialogBase* dlg );

			/**
			 * @fn void setDialog( SettingsDialogBase* dlg )
			 * @brief unsert dialog
			 * @param[in] dlg dialog
			 */
			void unsetDialog( SettingsDialogBase* dlg );

		public:
			/**
			 * @fn static SettingsManager& getInstance()
			 * @brief gets the SettingsManager object. (This is the only object.)
			 * @return SettingsManager object
			 */
			static SettingsManager& getInstance();
		};
	}
}

#endif		// __KOME_VIEW_SETTINGS_DIALOG_BASE_H__
