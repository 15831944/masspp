/**
 * @file IdentificationPluginManager.h
 * @brief interfaces of IdentificationPluginManager class
 *
 * @author S.Tanaka
 * @date 2013.07.26
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_IDENTIFICATION_PLUGIN_MANAGER_H__
#define __KOME_IDENT_IDENTIFICATION_PLUGIN_MANAGER_H__


namespace kome {
	namespace ident {

		class SpectrumGetter;

		/**
		 * @class IdentificationPluginManager
		 * @brief identification operation manager class
		 */
		class IdentificationPluginManager {
		public:
			/**
			 * @fn IdentificationPluginManager()
			 * @brief constructor
			 */
			IdentificationPluginManager();
			
			/**
			 * @fn virtual ~IdentificationPluginManager()
			 * @brief destructor
			 */
			~IdentificationPluginManager();

		public:
			/**
			 * @fn static int identify(
					const char* title,
					const char* comment,
					kome::plugin::PluginFunctionItem* searchEngine,
					kome::objects::SettingParameterValues* searchSettings,
					kome::plugin::PluginFunctionItem* peakDetector,
					kome::objects::SettingParameterValues* peakSettings,
					kome::plugin::PluginFunctionItem* chargeCalculator,
					kome::objects::SettingParameterValues* chargeSettings,
					SpectrumGetter* specGetter,
					const char* peakFilter,
					kome::core::Progress* progress
				)
			 * @brief performs the identification
			 * @param[in] title search title
			 * @param[in] comment search comment
			 * @param[in] searchEngine search engine
			 * @param[in] searchSettings search settings
			 * @param[in] peakDetector peak detector
			 * @parma[in] peakSettings peak settings
			 * @param[in] chargeCalculator charge calculator
			 * @param[in] chargeSettings charge settings
			 * @param[in] specGetter spectrum getter
			 * @param[in] peakFilter peak filter
			 * @param[in,out] the object to show the progress
			 * @return search ID
			 */
			static int identify(
				const char* title,
				const char* comment,
				kome::plugin::PluginFunctionItem* searchEngine,
				kome::objects::SettingParameterValues* searchSettings,
				kome::plugin::PluginFunctionItem* peakDetector,
				kome::objects::SettingParameterValues* peakSettings,
				kome::plugin::PluginFunctionItem* chargeCalculator,
				kome::objects::SettingParameterValues* chargeSettings,
				SpectrumGetter* specGetter,
				const char* peakFilter,
				kome::core::Progress* progress,
				std::vector< kome::plugin::PluginFunctionItem* > items,				// @date 2013.10.29 <Add> M.Izumi
				std::vector< kome::objects::SettingParameterValues > settings		// @date 2013.10.29 <Add> M.Izumi
			);

		protected:
			/** 
			 * @fn static bool detectPeaks(
					SpectrumGetter& specGetter,
					kome::ident::SearchEngine* engine,
					kome::objects::SettingParameterValues* searchSettings,
					kome::plugin::PluginFunctionItem* peakDetector,
					kome::objects::SettingParameterValues* peakSettings,
					kome::plugin::PluginFunctionItem* chargeCalculator,
					kome::objects::SettingParameterValues* chargeSettings,
					const char* peakFilter,
					kome::core::Progress& progress,
					std::vector< kome::ident::SpecInfo* >& spectra,
				)
			 * @brief detects peaks
			 * @param[in] specGetter spectrum getter
			 * @param[in] engine search engine
			 * @param[in] searchSettings search settings
			 * @param[in] peakDetector peak detector
			 * @param[in] peakSettings peak settings
			 * @param[in] chargeCalculator charge calculator
			 * @param[in] chargeSettings charge settings
			 * @param[in] peakFilter peak filter
			 * @param[in,out] progress the object to show the progress.
			 * @param[out] the object to store spectrum information.
			 * @return If true, peaks are detected successfully.
			 */
			static bool detectPeaks(
				SpectrumGetter& specGetter,
				kome::ident::SearchEngine* engine,
				kome::objects::SettingParameterValues* searchSettings,
				kome::plugin::PluginFunctionItem* peakDetector,
				kome::objects::SettingParameterValues* peakSettings,
				kome::plugin::PluginFunctionItem* chargeCalculator,
				kome::objects::SettingParameterValues* chargeSettings,
				const char* peakFilter,
				kome::core::Progress& progress,
				std::vector< kome::ident::SpecInfo* >& spectra,
				std::vector< kome::plugin::PluginFunctionItem* > items,
				std::vector< kome::objects::SettingParameterValues > settings
			);

		public:
			/**
			 * @fn static IdentificationPluginManager& getInstance()
			 * @brief gets identification manager
			 * @return identification manager class (This is the only object.)
			 */
			static IdentificationPluginManager& getInstance();
		};
	}
}

#endif	//  __KOME_IDENT_IDENTIFICATION_PLUGIN_MANAGER_H__
