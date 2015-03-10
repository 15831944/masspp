/**
 * @file ExportSampleDialog.h
 * @brief interfaces of FileIOManager class
 *
 * @author M.Izumi
 * @date 2011.7.19 
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IO_EXPORT_SAMPLE_DIALOG_H__
#define __KOME_IO_EXPORT_SAMPLE_DIALOG_H__

#include <wx/wx.h>
#include <vector>

// ‘I‘ðŽˆ‚ðkome.ini‚É•Û‘¶	// @date 2011/08/22 <Add> OKADA
#define EXPORT_SECTION		"EXPORT_SAMPLE"
#define TARGET_KEY			"TARGET"
#define FORMAT_KEY			"FORMAT"

namespace kome {
	namespace io {
		/**
		 * @class Export Sample Dialog
		 * @brief  Export Sample dialog
		 */
		class ExportSampleDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn ExportSampleDialog( wxWindow* parent, kome::objects::Variant acObj )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] acObj active object
			 */
			ExportSampleDialog( wxWindow* parent, kome::objects::Variant acObj );
			
			/**
			 * @fn virtual ~ExportSampleDialog()
			 * @brief destructor
			 */
			virtual~ ExportSampleDialog();

		public:
			/**
			 * @typedef ExportTarget
			 * @brief export target
			 */
			typedef enum {
				TARGET_SPECTRUM = 1,
				TARGET_FILTERED = 2,
				TARGET_SAMPLE = 4,
				TARGET_CHROMATOGRAM = 8	// @date 2011/11/28 <Add> OKADA
			} ExportTarget;

		protected:
			/**
			 * @struct ItemInfo
			 * @brief item information
			 */
			struct ItemInfo {
				kome::plugin::PluginFunctionItem* item;
				int target;
			};
		
		protected:
			/** radio box */
			wxRadioBox* m_RadioBox;
			
			/** combo box*/
			wxComboBox* m_comboBox;

			/** selected item */
			kome::plugin::PluginFunctionItem* m_item;

			/** target */
			ExportTarget m_target;

			/** items */
			std::vector< ItemInfo > m_items;

			/** active object */
			kome::objects::Variant m_obj;
			 
			/** parameters */                                       // @date 2012/8/6 <Add> FUJITA             
			kome::plugin::ParameterSettings m_parameters;

			/** settings */                                         // @date 2012/8/6 <Add> FUJITA 
			kome::objects::SettingParameterValues m_settings; 

			/** save settings */                                    // @date 2012/8/20 <Add> FUJITA
		    kome::objects::SettingParameterValues* m_saveSettings; 

			/** spectrum flag */
			bool m_specFlg;

			/** chromatogram flag */
			bool m_chromFlg;

		protected:
			/**
			 * @fn void createCombo()
			 * @brief create combo box
			 */
			void createCombo();

			/**
			 * @fn void setMembers()
			 * @brief set members
			 */
			void setMembers();

			/**
			 * @fn bool updateRadios()
			 * @brief updates radio button
			 * @return If true, updating is finished successfully.
			 */
			bool updateRadios();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			  * @return field sizer
			 */
			virtual wxSizer* createMainSizer();
			/**
             * @fn virtual wxSizer* createButtonSizer()
             * @brief creates button sizer(override method)
			 * @return button sizer
             */
			virtual wxSizer* createButtonSizer();

		public:
			/**
			 * @fn ExportTarget getTarget()
			 * @brief get target
			 * @return ExportTarget
			 */
			ExportTarget getTarget();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getSelectedItem()
			 * @brief get selected item
			 * @return plugin function item
			 */
			kome::plugin::PluginFunctionItem* getSelectedItem();

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfers data from window (override method)
			 * @return true or false
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/** 
			 * @fn void onRadio( wxCommandEvent& evt )
			 * @brief This method is called whe the target is selected
			 * @param[in] evt event information
			 */
			void onRadio( wxCommandEvent& evt );

			/**
			 * @fn void onSettingsPanel( kome::window::SettingsPanelEvent& evt )
			 * @brief settings panel
			 * @param[in] evt event information
			 */
			void onSettingsPanel( kome::window::SettingsPanelEvent& evt );

		private:
			 DECLARE_EVENT_TABLE()
		};
	}
}
#endif		//  __KOME_IO_EXPORT_SAMPLE_DIALOG_H__
