/**
 * @file PngManager.h
 * @brief interfaces of PngManager class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_PNG_MANAGER_H__
#define __KOME_IO_PNG_MANAGER_H__


namespace kome {
	namespace io {
		namespace png {

			/**
			 * @class PngManager
			 * @brief PNG output management class
			 */
			class PngManager {
			protected:
				/**
				 * @fn PngManager()
				 * @brief constructor
				 */
				PngManager();

				/**
				 * @fn virtual ~PngManager()
				 * @brief destructor
				 */
				virtual ~PngManager();

			public:
				/**
				 * @fn bool saveSpectra(
						kome::objects::DataSet& dataSet,
						const char* path,
						kome::objects::SettingParameterValues* settings,
						kome::core::Progress& progress
					)
				* @brief save spectra
				* @param dataSet spectra
				* @param path save file path
				* @param settings parameters
				* @param progress the object to show progress
				* @return If true, it succeeded to save spectra
				*/
				bool saveSpectra(
					kome::objects::DataSet& dataSet,
					const char* path,
					kome::objects::SettingParameterValues* settings,
					kome::core::Progress& progress
				);

				/**
				 * @fn bool saveChromatogram(
							kome::objects::DataSet& dataSet,
							const char* path,
							kome::objects::SettingParameterValues* settings,
							kome::core::Progress& progress
						)
				 * @brief saves chromatogram
				 * @param dataSet spectra which consists chromatogram
				 * @param path save file path
				 * @param settings parameters
				 * @progress the object to show progress
				 * @return If tru, it succeeeded to save chromatogram
				 */
				bool saveChromatogram(
					kome::objects::DataSet& dataSet,
					const char* path,
					kome::objects::SettingParameterValues* settings,
					kome::core::Progress& progress
				);

			public:
				/**
				 * @fn static PngManager& getInstance()
				 * @brief gets PNG manager object
				 * @return PNG manager object (This is the only object.)
				 */
				static PngManager& getInstance();
			};
		}
	}
}


#endif	// __KOME_IO_PNG_MANAGER_H__
