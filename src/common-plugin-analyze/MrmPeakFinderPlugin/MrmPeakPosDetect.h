/**
 * @file MrmPeakPosDetect.h
 * @brief interfaces of MrmPeakPosDetector class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_POSITON_MRM_PEAK_POS_DETECT_H__
#define __KOME_POSITON_MRM_PEAK_POS_DETECT_H__


namespace kome {
	namespace position {
		namespace mrm {

			class MrmPeakPosDetect : public kome::ident::PeakPositionsDetector {
			public:
				/**
				 * @fn MrmPeakPosDetect()
				 * @brief constructor
				 */
				MrmPeakPosDetect();

				/**
				 * @fn virtual ~MrmPeakPosDetect()
				 * @brief destructor
				 */
				virtual ~MrmPeakPosDetect();

			protected:
				/** settintgs */
				kome::objects::SettingParameterValues* m_settings;
			protected:

			public:
				 /* @fn
				  * @brief
				  * @return
				  */
				 int onDetectPeakPositions( kome::ident::SampleGetter& sample, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress );

				/**
				 * @fn void detectPeaks(
							kome::objects::Chromatogram* chrom,
							std::map< long long, std::map< long, int > >& peakMap,
							kome::plugin::PluginFunctionItem* peakDetector,
							kome::objects::SettingParameterValues* peakSettings,
							const bool areaFlg,
							const double rtTol,
							const double mzTol,
							kome::ident::PeakPositions& peakPos,
							const int sampleId
						)
				 * @brief detect peaks
				 * @param[in] chrom chromatogram
				 * @param[in,out] peakMap peak map
				 * @param[in] peakDetector peak detector
				 * @param[in] peakSettings peak settings
				 * @param[in] areaFlg area flag
				 * @param[in] rtTol RT tolerance
				 * @param[in] mzTol m/z tolerance
				 * @param[out] peakPos peak position
				 * @param[in] sampleId sample ID
				 */
				void detectPeaks(
					kome::objects::Chromatogram* chrom,
					std::map< long long, std::map< long, int > >& peakMap,
					kome::plugin::PluginFunctionItem* peakDetector,
					kome::objects::SettingParameterValues* peakSettings,
					const bool areaFlg,
					const double rtTol,
					const double mzTol,
					kome::ident::PeakPositions& peakPos,
					const int sampleId
				);

			public:
				void setSettings( kome::objects::SettingParameterValues* settings );
				kome::objects::SettingParameterValues* getSettings();
			};
		}
	}
}

#endif // __KOME_POSITON_MRM_PEAK_POS_DETECT_H__
