/**
 * @file MergePeakPosDetect.h
 * @brief interfaces of MergePeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_POSITON_MERGE_PEAK_POS_DETECTOR_H__
#define __KOME_POSITON_MERGE_PEAK_POS_DETECTOR_H__

namespace kome {
	namespace position {
		namespace merge {

			class MergePeakPosDetect : public kome::ident::PeakPositionsDetector {

			public:
				/**
				 * @fn MergePeakPosDetector( bool bcalcFlg, bool balignmentFlg )
				 * @brief constructor
				 */
				MergePeakPosDetect( bool bCalcFlg, bool bAlignmentFlg );

				/**
				 * @fn virtual ~MergePeakPosDetector()
				 * @brief destructor
				 */
				virtual ~MergePeakPosDetect();

			protected:
				/** settings */
				kome::objects::SettingParameterValues* m_settings;
			
			protected:
			
				 /* @fn
				  * @brief
				  * @return
				  */
				 int onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress );
			public:
				 
				/* @fn
				  * @brief
				  * @return
				  */
				void setSettings( kome::objects::SettingParameterValues* settings );

				 /* @fn
				  * @brief
				  * @return
				  */
				kome::objects::SettingParameterValues* getSettings();

			};
		}
	}
}



#endif // __KOME_POSITON_MERGE_PEAK_POS_DETECTOR_H__
