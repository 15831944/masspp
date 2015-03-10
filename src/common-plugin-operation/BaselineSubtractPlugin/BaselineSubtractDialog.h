/**
 * @file BaselineSubtractDialog.h
 * @brief interfaces of BaselineSubtractDialog class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_BASELINE_SUBTRACT_DIALOG_H__
#define __KOME_VIEW_BASELINE_SUBTRACT_DIALOG_H__


#include <vector>

#include <wx/wx.h>
#include <wx/spinctrl.h>


namespace kome {
	namespace baseline {

		/**
		 * @class BaselineSubtractDialog
		 * @brief baseline subtract dialog
		 */
		class BaselineSubtractDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn BaselineSubtractDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			BaselineSubtractDialog( wxWindow* parent );

			/**
			 * @fn virtual ~BaselineSubtractDialog()
			 * @brief destructor
			 */
			virtual ~BaselineSubtractDialog();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** baseline detector */
			kome::plugin::PluginFunctionItem* m_baselineDetector;

			/** baseline settings */
			kome::objects::SettingParameterValues* m_baselineSettings;
			
			/** PluginSelectionPanel */
			kome::window::PluginSelectionPanel* m_panel;	// @date 2014.03.03 <Mod> M.Izumi

			/** redraw flag */
			int m_flgReDraw;
		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem* getBaselineDetector()
			 * @brief gets baseline detector
			 * @return baseline detector
			 */
			kome::plugin::PluginFunctionItem* getBaselineDetector();

			/**
			 * @fn kome::objects::SettingParameterValues* getBaselineSettings()
			 * @brief gets baseline settings
			 * @return baseline settings
			 */
			kome::objects::SettingParameterValues* getBaselineSettings();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();


		protected:
			/**
			 * @fn void onDefault( wxCommandEvent& evt )
			 * @brief This method is called on default button event
			 * @param evt event information
			 */
			void onDefault( wxCommandEvent& evt );

			/**
			 * @fn void onDraw( wxCommandEvent& evt )
			 * @brief This method is called on draw baseline button event
			 * @param evt event information
			 */
			void onDraw( wxCommandEvent& evt );

			/**
			 * @fn void onErase( wxCommandEvent& evt )
			 * @brief This method is called on erase baseline button event
			 * @param evt event information
			 */
			void onErase( wxCommandEvent& evt );

			/**
			 * @fn virtual bool onOK()
			 * @brief This method is called when the OK button is clicked.
			 * @param evt button event
			 */
			virtual bool onOK();

			/**
			 * @fn virtual bool onCancel()
			 * @brief This button is called when the Cancel button is clicked.
			 * @param evt button event
			 */
			virtual bool onCancel();

		public:
			// @date 2011/07/22 <Add> OKADA
			/**
			 * @fn void drawBaseline(void )
			 * @brief This method is called on draw baseline
			 */
			void drawBaseline( void );

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

			

		protected:
			/**
			 * @fn void onSettingsPanel( kome::window::SettingsPanelEvent& evt )
			 * @brief This method is called when the settings panel is edited.
			 * @param[in] evt event information
			 */
			void onSettingsPanel( kome::window::SettingsPanelEvent& evt );

		private:
			/**
			 * @fn void ProcDraw()
			 * @brief
			 */
			void ProcDraw();

			/**
			 * @fn void ProcErase()
			 * @brief
			 */
			void ProcErase();
		private:
			DECLARE_EVENT_TABLE();

			
		};
	}
}

#endif		// __KOME_VIEW_BASELINE_SUBTRACT_DIALOG_H__
