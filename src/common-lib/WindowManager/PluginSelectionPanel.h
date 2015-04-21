/**
 * @file PluginSelectionPanel.h
 * @brief interfaces of PluginSelectionPanel class
 *
 * @author S.Tanaka
 * @date 2011.05.18
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_PLUGIN_SELECTION_PANEL_H__
#define __KOME_WINDOW_PLUGIN_SELECTION_PANEL_H__


#include "HelpGetterPanel.h"
#include "SettingsPanel.h"

#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class PluginSelectionPanel
		 * @brief each settings panel class
		 */
		class WINDOW_MANAGER_CLASS PluginSelectionPanel : public HelpGetterPanel {
		public:
			/**
			 * @typedef enum PanelStyle
			 * @brief panel style
			 */
			typedef enum {
				TYPE_CHOICE_BOOK,
				TYPE_BUTTON
			} PanelStyle;

		public:
			/**
			 * @fn PluginSelectionPanel(
						wxWindow* parent,
						const int id,
						kome::plugin::SettingsForm* form,
						kome::objects::SettingParameterValues* settings,
						const bool titleFlg,
						PanelStyle style = TYPE_BUTTON,
						const bool saveVal = true
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id control ID
			 * @param[in] form form information
			 * @param[out] settings the object to store selected plug-in information
			 * @param[in] titleFlg title flag
			 * @param[in] style panel style
			 * @param[in] saveVal save value flag
			 */
			PluginSelectionPanel(
				wxWindow* parent,
				const int id,
				kome::plugin::SettingsForm* form,
				kome::objects::SettingParameterValues* settings,
				const bool titleFlg,
				PanelStyle style = TYPE_BUTTON,
				const bool saveVal = true
			);

			/**
			 * @fn virtual ~PluginSelectionPanel()
			 * @brief destructor
			 */
			virtual ~PluginSelectionPanel();

		protected:
			/** form */
			kome::plugin::SettingsForm* m_form;

			/** settings */
			kome::objects::SettingParameterValues* m_settings;

			/** new settings flag */
			bool m_newSettings;

			/** style */
			PanelStyle m_style;

			/** title flag */
			bool m_titleFlg;

			/** save value flag */
			bool m_saveVal;

			/** plug-ins */
			std::vector< kome::plugin::PluginFunctionItem* > m_items;

			/** panels */
			std::vector< SettingsPanel* > m_panels;

			/** Parameters button */
			wxButton* m_button;

		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem* getItem()
			 * @brief gets the item
			 * @return item
			 */
			kome::plugin::PluginFunctionItem* getItem();

			/**
			 * @fn void setItem( const char* val )
			 * @brief sets the item
			 */
			void setItem( const char* val );

		public:			
			/**
			 * @fn void setValue( const char* val );
			 * @brief sets the value
			 * @param[in] val value
			 */
			void setValue( const char* val );

		public:
			/**
			 * @fn void setDefaultParameters()
			 * @brief sets the default parameters
			 */
			void setDefaultParameters();

		protected:
			/**
			 * @fn void getItems( const char* val = NULL )
			 * @brief updates the array of function item
			 */
			void getItems( const char* val = NULL );

			/**
			 * @fn void createCombo()
			 * @brief creates combo box
			 */
			 void createCombo();

			/**
			 * @fn void createComboSizer( wxSizer* sizer )
			 * @brief creates combo sizer
			 * @param[out] sizer parent sizer
			 */
			void createComboSizer( wxSizer* sizer );

			/**
			 * @fn void createChoicebookSizer( wxSizer* sizer )
			 * @brief creates choicebook sizer
			 * @param[out] sizer parent sizer
			 */
			void createChoicebookSizer( wxSizer* sizer );

		protected:
			/**
			 * @fn int getSelectedIndex()
			 * @brief gets selected index
			 */
			int getSelectedIndex();

			/**
			 * @fn void SetParametersButtonEnabledState(wxButton* button)
			 * @brief sets the Parameters... button enabled state
			 * @param button Parameters... button
			 */
			void SetParametersButtonEnabledState(wxButton* button);

        protected:
			/**
			 * @fn void processEvent()
			 * @brief processes event
			 */
			void processEvent();

		public:
			/**
			 * @fn void updateEnum( const char* val )
			 * @brief updates enumeration 
			 * @param[in] val enumeration value
			 */
			void updateEnum( const char* val );			

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );

			/**
			 * @fn virtual std::string getImageFilePath( wxWindow* wnd )
			 * @brief method to get image file path from a parameter control
			 * @param[in] wnd parameter control
			 * @return image file path
			 */
			virtual std::string getImageFilePath( wxWindow* wnd );

		protected:
			/**
			 * @fn virtual bool Validate()
			 * @brief validates the values
			 * @return If any of the valudations failed, this method returns false.
			 */
			virtual bool Validate();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();
				
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onParams( wxCommandEvent& evt )
			 * @brief This method is called when the parameters button is clicked.
			 * @param evt event information
			 */
			void onParams( wxCommandEvent& evt );

			/**
			 * @fn void onCombo( wxCommandEvent& evt )
			 * @brief  This method is called when the combobox selection is changed.
			 * @param[in] evt event information
			 */
			void onCombo( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};

	}
}

#endif		// __KOME_WINDOW_PLUGIN_SELECTION_PANEL_H__
