/**
 * @file SimplePeakPosDetect.h
 * @brief interfaces of SimplePeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_SIMPLE_PEAK_POS_DETECT_H__
#define __KOME_SIMPLE_PEAK_POS_DETECT_H__

namespace kome {
	namespace position {
		namespace simple {

			class SimplePeakPosDetect : public kome::ident::PeakPositionsDetector {
			public:
				/**
				 * @fn SimplePeakPosDetect( bool bcalcFlg, bool balignmentFlg )
				 * @brief constructor
				 */
				SimplePeakPosDetect( bool bCalcFlg, bool bAlignmentFlg );

				/**
				 * @fn virtual ~MergePeakPosDetector()
				 * @brief destructor
				 */
				virtual ~SimplePeakPosDetect();

			protected:
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

#endif // __KOME_SIMPLE_PEAK_POS_DETECT_H__
