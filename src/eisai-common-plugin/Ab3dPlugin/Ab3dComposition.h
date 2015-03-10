/**
 * @file Ab3dComposition.h
 * @brief interfaces of Ab3dComposition class
 *
 * @author S.Tanaka
 * @date 2007.09.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_COMPOSITION_H__
#define __KOME_AB3D_COMPOSITION_H__


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dComposition
		 * @brief AB3D composition class
		 */
		class Ab3dComposition {
		public:
			/**
			 * @fn Ab3dComposition(
					kome::objects::DataSet& ctrlSpectra,
					kome::objects::DataSet& trmtSpectra
				)
			  * @param ctrlSpectra control spectra
			 * @param trmtSpectra treatment spectra
			 * @brief constructor
			 */
			Ab3dComposition(
				kome::objects::DataSet& ctrlSpectra,
				kome::objects::DataSet& trmtSpectra
			);

			/**
			 * @fn virtual ~Ab3dComposition()
			 * @brief destructor
			 */
			virtual ~Ab3dComposition();

		public:
			/**
			 * @typedef IntensityType
			 * @brief intensity type
			 */
			typedef enum {
				INTENSITY_MAX,
				INTENSITY_TOTAL
			} IntensityType;

			/**
			 * @typedef Operation
			 * @brief Raw Data Operation
			 */
			typedef enum {
				OPERATION_SUBTRACTION,
				OPERATION_MERGE,
				OPERATION_RATIO
			} Operation;

		protected:
			/** control spectra */
			kome::objects::DataSet& m_ctrlSpectra;

			/** treatment spectra */
			kome::objects::DataSet& m_trmtSpectra;

		public:
			/**
			 * @fn kome::objects::Sample* createCompositionData(
					kome::objects::SettingParameterValues* vals,
					kome::operation::Alignment* alignment,
					kome::core::Progress* progress
				)
			 * @brief creates composition data
			 * @param progress the object to show progress
			 * @param alignment alignment object
			 * @return composition data
			 */
			kome::objects::Sample* createCompositionData(
				kome::objects::SettingParameterValues* vals,
				kome::operation::Alignment* alignment,
				kome::core::Progress* progress
			);
		};
	}
}

#endif		// __KOME_AB3D_COMPOSITION_H__
