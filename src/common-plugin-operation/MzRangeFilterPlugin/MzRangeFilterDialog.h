/**
 * @file MzRangeFilterDialog.h
 * @brief interfaces of MzRangeFilterDialog class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MZRANGE_FILTER_DIALOG_H__
#define __KOME_MZRANGE_FILTER_DIALOG_H__

namespace kome {
	namespace mzfilter {
		class SpectraCheckList;

		/**
		 * @class MzRangeFilterDialog
		 * @brief MzRangeFilterDialog class
		 */
		class MzRangeFilterDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn MzRangeFilterDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			MzRangeFilterDialog( wxWindow* parent, kome::objects::Variant& activeObj );

			/**
			 * @fn virtual ~MzRangeFilterDialog()
			 * @brief destructor
			 */
			virtual ~MzRangeFilterDialog();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** sample object */
			kome::objects::Sample* m_sample;

			/** active spectrum object */
			kome::objects::Spectrum* m_spec;
			
			/** check grid list ctrl */
			SpectraCheckList* m_list;

			/** m/z range */
			std::string m_strMzRange;
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

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();
		public:
			/**
			 * @fn void getCheckSpectra( std::vector< kome::objects::Spectrum* >& spectra )
			 * @brief get checked spectra
			 * @param[out] spectra spectrum array
			 */
			void getCheckSpectra( std::vector< kome::objects::Spectrum* >& spectra );

			/**
			 * @fn const char* getMzRange()
			 * @brief get m/z range 
			 */
			const char* getMzRange();
		private:
			DECLARE_EVENT_TABLE();
		};

	}
}

#endif // __KOME_MZRANGE_FILTER_DIALOG_H__
