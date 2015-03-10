/**
 * @file PeakPickingManager.h
 * @brief interfaces of PeakPickingManager class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_PEAK_PICKING_MANAGER_H__
#define __KOME_LABELING_PEAK_PICKING_MANAGER_H__


#include <vector>
#include <set>


namespace kome {
	namespace labeling {
		
		class PeakDetectOperation;

		/**
		 * @class PeakPickingManager
		 * @brief peak picking management class
		 */
		class PeakPickingManager {
		protected:
			/**
			 * @fn PeakPickingManager()
			 * @brief constructor
			 */
			PeakPickingManager();

			/**
			 * @fn virtual ~PeakPickingManager()
			 * @brief destructor
			 */
			virtual ~PeakPickingManager();

		protected:
			std::string m_targetName;

		protected:
			// @date 2011.01.25 <Add> M.Izumi ->
			/** peak picking function */
			kome::plugin::PluginFunctionItem m_peakFun;

			/** peak parameters */
			kome::objects::SettingParameterValues m_peakSettings;
			// @date 2011.01.25 <Add> M.Izumi <-
			
		public:
			/**
			 * @typedef PeakPickingTarget
			 * @brief peak picking target
			 */
			typedef enum {
				TARGET_UNKNOWN,
				TARGET_CURRENT_SPECTRUM,
				TARGET_MS_SPECTRA,
				TARGET_MSMS_SPECTRA,
				TARGET_FILTERED_SPECTRA,
				TARGET_ALL_SPECTRA
			} PeakPickingTarget;
		
		public:
			/**
			 * @fn void getTargetName( PeakPickingTarget target, const char** targetName )
			 * @brief ピーク検出のターゲットを文字列変換
			 * @param[in] target peak picking target
 			 * @param[out] targetName 取得した文字列 ( target name )
			 */
			std::string getTargetName( PeakPickingTarget target );	
						
			
		// @date 2011.01.25 <Add> M.Izumi ->
		public:
			/**
			 * @fn void setPeakDetectFunction( kome::plugin::PluginFunctionItem fun )
			 * @brief sets peak detect function
			 * @param fun parent peak detect function
			 */
			void setPeakDetectFunction( kome::plugin::PluginFunctionItem fun );

			/**
			 * @fn kome::plugin::PluginFunctionItem getPeakDetectFunction()
			 * @brief gets parent peak picking function
			 * @return parent peak picking function
			 */
			kome::plugin::PluginFunctionItem getPeakDetectFunction();

			/**
			 * @fn void setPeakDetectSettings( kome::objects::SettingParameterValues settings )
			 * @brief sets peak detect settings
			 * @param settings peak detect settings
			 */
			void setPeakDetectSettings( kome::objects::SettingParameterValues settings );

			/**
			 * @fn kome::objects::SettingParameterValues getPeakDetectSettings()
			 * @brief get peak detect settings
			 * @return parent peak picking settings
			 */
			kome::objects::SettingParameterValues getPeakDetectSettings();
		// @date 2011.01.25 <Add> M.Izumi <-
		public:
			/**
			 * @fn void detectPeaks(
					kome::objects::Spectrum* spec,
					kome::core::XYData* xyData,
					kome::plugin::PluginFunctionItem* peakPickingFun,
					kome::objects::SettingParameterValues* labelSettings,
					kome::plugin::PluginFunctionItem* chargeDetectFun,
					kome::objects::SettingParameterValues* chargeSettings
				)
			  * @brief detects peaks
			 * @param spec spectrum to be detected peaks
			 * @param xyData xy data
			 * @param peakPickingFun peak picking function
			 * @param labelSettings peak picking parameters
			 * @param chargeDetectFun charge detect function
			 * @param chargeSettings charge detect parameters
			 */
			void detectPeaks(
				kome::objects::Spectrum* spec,
				kome::core::XYData* xyData,
				kome::plugin::PluginFunctionItem* peakPickingFun,
				kome::objects::SettingParameterValues* labelSettings,
				kome::plugin::PluginFunctionItem* chargeDetectFun,
				kome::objects::SettingParameterValues* chargeSettings
			);

			/**
			 * @fn void detectPeaks(
					kome::objects::Chromatogram* chrom,
					kome::core::XYData* xyData,
					kome::plugin::PluginFunctionItem* fun
					kome::objects::SettingParameterValues* labelSettings
				)
			 * @brief detects peaks
			 * @param chrom chromatogram to be detected peaks
			 * @param xyData xy data
			 * @param fun peak picking function
			 * @param labelSettings peak picking paremters
			 */
			void detectPeaks(
				kome::objects::Chromatogram* chrom,
				kome::core::XYData* xyData,
				kome::plugin::PluginFunctionItem* fun,
				kome::objects::SettingParameterValues* labelSettings
			);

			/**
			 * @fn void getTargetSpectra(
						kome::objects::Variant& activeObj,
						std::vector< kome::objects::Spectrum* >& spectra,
						PeakPickingTarget target
					)
			 * @brief get target spectra
			 * @param activeObj active object
			 * @param spectra the array to store target spectra
			 * @param target target type
			 */
			void getTargetSpectra(
				kome::objects::Variant& activeObj,
				std::vector< kome::objects::Spectrum* >& spectra,
				PeakPickingTarget target
			);

        public:
			/**
			 * @fn bool savePeaks(
						kome::objects::Peaks2D& peaks,
						const char* filePath,
						kome::core::Progress& progress
					)
			 * @brief saves peaks
			 * @param peaks peaks object
			 * @param filePath output file path
			 * @param progress the object to show progress
			 * @return If true, it succeeded to save file.
			 */
			bool savePeaks(
				kome::objects::Peaks2D& peaks,
				const char* filePath,
				kome::core::Progress& progress
			);
		public:
			/**
			 * @fn bool executePeakFiltering( 
						std::vector<kome::objects::Spectrum*>spectra,
						const char* peak_filter
				   )
			 * @brief execute peak filtering
			 * @param spectra spectra objects
			 * @param peak_filter peak filter name
			 * @return error code
			 */
			bool executePeakFiltering( 
				std::vector<kome::objects::Spectrum*>spectra,
				const char* peak_filter 
			);

		public:
			/**
			 * @fn static PeakPickingManager& getInstance()
			 * @brief gets PeakPickingManager object
			 * @return PeakPickingManager object. (This is the only object.)
			 */
			static PeakPickingManager& getInstance();

		public:
			double NoiseCauculation(double halfWidth, double threshold, kome::core::XYData* xyData);
		};
	}
}

#endif	// __KOME_LABELING_PEAK_PICKING_MANAGER_H__
