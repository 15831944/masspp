/**
 * @file MergedSpecPeakPosDetect.h
 * @brief interfaces of MergedSpecPeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_POSITON_MERGED_SPEC_PEAK_POS_DETECT_H__
#define __KOME_POSITON_MERGED_SPEC_PEAK_POS_DETECT_H__

namespace kome {
	namespace position {
		namespace merged {
			class MergedSpecPeakPosDetect : public kome::ident::PeakPositionsDetector {
			public:
				/**
				 * @fn MergedSpecPeakPosDetect( bool bcalcFlg, bool balignmentFlg )
				 * @brief constructor
				 */
				MergedSpecPeakPosDetect( bool bCalcFlg, bool bAlignmentFlg );

				/**
				 * @fn virtual ~MergedSpecPeakPosDetect()
				 * @brief destructor
				 */
				virtual ~MergedSpecPeakPosDetect();

			protected:
				/** settintgs */
				kome::objects::SettingParameterValues* m_settings;
			protected:
				 /* @fn
				  * @brief
				  * @return
				  */
				 int onDetectPeakPositions( kome::ident::SampleGetter& sample, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress );
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

#endif // __KOME_POSITON_MERGED_SPEC_PEAK_POS_DETECT_H__
