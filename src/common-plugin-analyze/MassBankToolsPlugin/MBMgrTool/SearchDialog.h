/**
 * @file SearchDialog.h
 * @brief interfaces of SearchDialog
 *
 * @author S.Tanaka
 * @date 2010.03.26
 *
 * @modify M.Fukuda
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_SEARCH_DIALOG_H__
#define __KOME_MASBANK_SEARCH_DIALOG_H__


namespace kome {
	namespace massbank {

		/**
		 * @class SearchDialog
		 * @brief MassBank search dialog class
		 */
		class SearchDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @typedef SearchType
			 * @brief search type
			 */
			typedef enum {
				TYPE_SPECTRUM,
				TYPE_PEAKS,
				TYPE_DIFF
			} SearchType;

		public:
			/**
			 * @fn SearchDialog(
						wxWindow* parent,
						const SearchType type,
						kome::objects::SettingParameterValues& settings
						const char* title
					)
			 * @brief constructor
			 * @param parent parent window
			 * @param type search type
			 * @param settings parameters object to store values
			 * @param title				
			 */
			SearchDialog(
				wxWindow* parent,
				const SearchType type,
				kome::objects::SettingParameterValues& settings,
				const char* title,
				kome::objects::Spectrum* spec = nullptr
			);

			/**
			 * @fn virtual ~SearchDialog()
			 * @brief destructor
			 */
			virtual ~SearchDialog();

		protected:
			/** search type */
			const SearchType m_type;

			/** settings */
			kome::objects::SettingParameterValues& m_settings;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** keep spectrum */
			kome::objects::Spectrum* spec_;

		protected:
			/**
			 * @fn void createSimilarSpectraSearchPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage* page )
			 * @brief creates similar spectra search page
			 * @param params parameters object
			 * @param page page object
			 */
			void createSimilarSpectraSearchPage(
				kome::plugin::ParameterSettings& params, 
				kome::plugin::SettingsPage* page
				);

			/**
			 * @fn void createPeakSearchPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage* page, const bool diff )
			 * @brief creates peak search page
			 * @param params parameters object
			 * @param page page object
			 * @param diff true: differential peak search, false: peak search
			 */
			void createPeakSearchPage(
				kome::plugin::ParameterSettings& params,
				kome::plugin::SettingsPage* page, 
				const bool diff
				);

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:

			kome::plugin::SettingsValue* onCreateTolUnitVals(
				kome::plugin::ParameterSettings& params);

			kome::plugin::SettingsValue* onCreateIonVals(
				kome::plugin::ParameterSettings& params);

			kome::plugin::SettingsValue* onCreateMsTypeVals(
				kome::plugin::ParameterSettings& params);

			/**
			 * @fn void onCall_InstDialog( wxCommandEvent& evt);
			 * @brief Call Instrument Dialog
			 * @param[in] evt event information
			 */
			void onCall_InstDialog( wxCommandEvent& evt);

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	// __KOME_MASBANK_SEARCH_DIALOG_H__
