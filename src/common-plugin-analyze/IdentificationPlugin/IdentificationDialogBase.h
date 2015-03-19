/**
 * @file IdentificationDialogBase.h
 * @brief interfaces of IdentificationDialogBase class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_IDENTIFICATION_DIALOG_BASE_H__
#define __KOME_IDENT_IDENTIFICATION_DIALOG_BASE_H__

#include <vector>

#include <wx/wx.h>


namespace kome {
	namespace ident {

		class SpectrumGetter;

		/**
		 * @class IdentificationDialogBase
		 * @brief auto labeling dialog class
		 */
		class IdentificationDialogBase : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn IdentificationDialogBase( wxWindow* pParent, const char* title )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @parma[in] title dialog title
			 */
			IdentificationDialogBase( wxWindow* pParent, const char* title );
			
			/**
			 * @fn virtual ~IdentificationDialogBase()
			 * @brief destructor
			 */
			virtual ~IdentificationDialogBase();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** combo box */
			wxComboBox* m_combo;

			/** search engine form */
			kome::plugin::SettingsForm* m_engineForm;

		public:
			/**
			 * @fn std::string getTitle()
			 * @brief gets the title
			 * @return title
			 */
			std::string getTitle();

			/**
			 * @fn std::string getComment()
			 * @brief gets the comment
			 * @return comment
			 */
			std::string getComment();

			/**
			 * @fn kome::objects::SettingParameterValues* getSearchSettings()
			 * @brief gets the search settings 
			 * @return search settings
			 */
			kome::objects::SettingParameterValues* getSearchSettings();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getSearchItem()
			 * @brief gets the search engine function
			 * @return search engine function
			 */
			kome::plugin::PluginFunctionItem* getSearchItem();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			 * @brief gets the peak detector
			 * @return peak detector
			 */
			kome::plugin::PluginFunctionItem* getPeakDetector();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			 * @brief gets the peak detector settings
			 * @retrun peak detect settings
			 */
			kome::objects::SettingParameterValues* getPeakDetectSettings();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getChargeDetector()
			 * @brief gets the charge detector
			 * @return charge detector
			 */
			kome::plugin::PluginFunctionItem* getChargeDetector();

			/**
			 * @fn kome::objects::SettingParameterValues* getChargeDetectSettings()
			 * @brief gets the charge detect settings
			 * @return charge detect settings
			 */
			kome::objects::SettingParameterValues* getChargeDetectSettings();

			/**
			 * @fn std::string getPeakFilter()
			 * @brief gets the peak filter
			 * @return peak filter
			 */
			std::string getPeakFilter();

			/**
			 * @fn void getSpecDataManipulat( 
						std::vector< kome::plugin::PluginFunctionItem* >& items,
						std::vector< kome::objects::SettingParameterValues >& settingVals
					)
			 * @brief get the spectrum maniplation settings
			 * @param[out] items plugin function item 
			 * @param[out] settingVals setting parameter values object
			 */
			void getSpecDataManipulat( 
				std::vector< kome::plugin::PluginFunctionItem* >& items,
				std::vector< kome::objects::SettingParameterValues >& settingVals
			);

		protected:
			/**
			 * @fn kome::ident::SearchEngine* getSearchEngine()
			 * @brief gets the search engine
			 * @return search engine
			 */
			kome::ident::SearchEngine* getSearchEngine();

		protected:
			/**
			 * @fn wxSizer* createSearchSizer()
			 * @brief creates search sizer
			 * @return search sizer
			 */
			wxSizer* createSearchSizer();

			/**
			 * @fn wxSizer* createSearchSizer()
			 * @brief creates search sizer
			 * @return search sizer
			 */
			wxSizer* createPeaksSizer();

		protected:
			/**
			 * @fn void onSettings( kome::window::SettingsPanelEvent& evt )
			 * @brief This method is called when a form is changed.
			 * @param[in] evt event information
			 */
			 void onSettings( kome::window::SettingsPanelEvent& evt );

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();
			
			/**
			 * @fn virtual wxSizer* createButtonSizer();
			 * @brief creates button sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createButtonSizer();

		public:
			/**
			 * @fn virtual SpectrumGetter* createSpectrumGetter()
			 * @brief creates spectrum getter
			 * @return spectrum getter
			 */
			virtual SpectrumGetter* createSpectrumGetter();

		protected:
			/**
			 * @fn virtual wxSizer* createSpectraSizer()
			 * @brief creates spectra sizer
			 * @return spectra sizer
			 */
			virtual wxSizer* createSpectraSizer();
		
		protected:
			/**
			 * @fn virtual void onChangeEngine( kome::ident::SearchEngine* engine )
			 * @brief This method is called when selected search engine is changed.
			 * @param[in] engine selected search engine
			 */
			virtual void onChangeEngine( kome::ident::SearchEngine* engine );

		public:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfers data from window (override method)
			 * @return If true, all values are valid.
			 */
			virtual bool TransferDataFromWindow();

		public:
			/**
			 * @fn static bool prepare()
			 * @brief prepare for the DB search
			 * @return If false, identification should not be continued.
			 */
			static bool prepare();

			/**
			 * @fp static bool identify( IdentificationDialogBase* dlg, kome::objects::Sample* sample )
			 * @brief identifies with dialog
			 * @param[in] dlg dialog object
			 * @param[in] sample active sample
			 * @return If true, identification is finished successfully.
			 */
			static bool identify( IdentificationDialogBase* dlg, kome::objects::Sample* sample );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//  __KOME_IDENT_IDENTIFICATION_DIALOG_BASE_H__
