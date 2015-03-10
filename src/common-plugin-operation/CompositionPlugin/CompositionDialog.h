/**
 * @file CompositionDialog.h
 * @brief interfaces of CompositionDialog class
 *
 * @author S.Tanaka
 * @date 2009.02.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_COMPOSITION_DIALOG_H__
#define __KOME_COMPOSITION_DIALOG_H__


#include <vector>
#include <wx/wx.h>
#include <wx/notebook.h>


namespace kome {
	namespace composition {

		class PanelBase;

		/**
		 * @class CompositionDialog
		 * @brief composition subtract dialog
		 */
		class CompositionDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn CompositionDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			CompositionDialog( wxWindow* parent );

			/**
			 * @fn virtual ~CompositionDialog()
			 * @brief destructor
			 */
			virtual ~CompositionDialog();

		protected:
			/** control group combobox */
			wxComboBox* m_ctrlGrpCombo;

			/** treatment group combobox */
			wxComboBox* m_trmtGrpCombo;

		protected:
			/** control group */
			kome::objects::DataGroupNode* m_ctrlGroup;

			/** treatment group */
			kome::objects::DataGroupNode* m_trmtGroup;

			/** alignment */
			kome::plugin::PluginFunctionItem* m_alignment;

			/** alignment settings */
			kome::objects::SettingParameterValues* m_alignSettings;

			/** composition */
			kome::plugin::PluginFunctionItem* m_composition;

			/** composition settings */
			kome::objects::SettingParameterValues* m_compositionSettings;

		protected:
			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

		public:
			/**
			 * @fn kome::objects::SettingParameterValues& getParameters()
			 * @brief gets parameters
			 * @return parameters object
			 */
			kome::objects::SettingParameterValues& getParameters();

		public:
			/**
			 * @fn kome::objects::DataGroupNode* getCtrlGroup()
			 * @brief gets control group
			 * @return control group
			 */
			kome::objects::DataGroupNode* getCtrlGroup();

			/**
			 * @fn kome::objects::DataGroupNode* getTrmtGroup()
			 * @brief gets treatment group
			 * @return treatment group
			 */
			kome::objects::DataGroupNode* getTrmtGroup();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getAlignment()
			 * @brief gets alignment
			 * @return alignment
			 */
			kome::plugin::PluginFunctionItem* getAlignment();

			/**
			 * @fn kome::objects::SettingParameterValues* getAlignmentSettings()
			 * @brief gets alignment settings
			 * @return alignment settings
			 */
			kome::objects::SettingParameterValues* getAlignmentSettings();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getComposition()
			 * @brief gets composition
			 * @return composition
			 */
			kome::plugin::PluginFunctionItem* getComposition();

			/**
			 * @fn kome::objects::SettingParameterValues* getCompositionSettings()
			 * @brief gets composition settings
			 * @return composition settings
			 */
			kome::objects::SettingParameterValues* getCompositionSettings();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn wxComboBox* createGroupCombo( const int id )
			 * @brief creates group combo box
			 * @param id combo box ID
			 * @return group combo box
			 */
			wxComboBox* createGroupCombo( const int id );

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_COMPOSITION_DIALOG_H__
