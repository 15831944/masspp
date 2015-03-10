/**
 * @file NeighborSpecPeaks.h
 * @brief interfaces of NeighborSpecPeaks class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_NEIGHBOR_SPEC_PEAKS_H__
#define __KOME_SEARCH_NEIGHBOR_SPEC_PEAKS_H__


#include "NeighborPeaks.h"


namespace kome {
	namespace search {
		namespace neighbor {

			/**
			 * @class NeighborSpecPeaks
			 * @brief Neighbor Peaks class
			 */
			class NeighborSpecPeaks : public NeighborPeaks {
			public:
				/**
				/**
				 * @fn NeighborSpecPeaks(
							kome::objects::SettingParameterValues* settings
						)
				 * @brief constructor
				 * @param settings setting parameter values
				 */
				NeighborSpecPeaks(
					kome::objects::SettingParameterValues* settings
				);

				/**
				 * @fn virtual ~NeighborSpecPeaks()
				 * @brief destructor
				 */
				virtual ~NeighborSpecPeaks();

			protected:
				/**
				 * @struct ProfileInfo
				 * @brief profile information
				 */
				struct ProfileInfo {
					double rt;
					kome::core::DataPoints dps;
					kome::objects::Peaks peaks;
				};

			public:
				/**
				 * @fn void searchPeaks(
							 kome::objects::Sample* sample,
							 kome::ident::PeakPositions& peakPos,
							 kome::operation::Alignment* align,
							 kome::core::Progress& progress,
							 const int sampleId,
							 const char* val
						)
				 * @brief searches peaks
				 * @param sample sample
				 * @param peakPos peak positions
				 * @param[in] align alignment
				 * @param[in] sampleId sample ID
				 * @param progress the object to show progress
				 * @param val manipulation settings value
				 */
				void searchPeaks(
					kome::objects::Sample* sample,
					kome::ident::PeakPositions& peakPos,
					kome::operation::Alignment* align,
					const int sampleId,
					kome::core::Progress& progress,
					const char* val
				);

			protected:
				/**
				 * @fn void getPeaks( 
							kome::objects::Spectrum& spec,
							kome::core::DataPoints& dps,
							const bool profileFlg,
							std::vector< kome::plugin::PluginFunctionItem* > items,
							std::vector< kome::objects::SettingParameterValues > settingVals
						)
				 * @param[in] spec spectrum
				 * @param[out] peaks the object to store peaks
				 * @param[in] dps data points
				 */
				void getPeaks( 
					kome::objects::Spectrum& spec,
					kome::objects::Peaks& peaks,
					kome::core::DataPoints& dps,
					std::vector< kome::plugin::PluginFunctionItem* > items,
					std::vector< kome::objects::SettingParameterValues > settingVals
				);

				/**
				 * @fn kome::objects::PeakElement* searchPeak(
							const double rt,
							const double mz,
							std::list< ProfileInfo >& profileList,
							double* val,
							ProfileInfo** profile
						)
				 * @brief searches peak
				 * @param[in] rt RT position
				 * @param[in] mz m/z position
				 * @param[in] profileList profile information
				 * @param[out] val the pointer to store peak value
				 * @return peak
				 */
				kome::objects::PeakElement* searchPeak(
						const double rt,
						const double mz,
						std::list< ProfileInfo >& profileList,
						double* val,
						ProfileInfo** profile
				);

				/**
				 * @fn void getManipulatFuncInfo(
							const char* val,
							std::vector< kome::plugin::PluginFunctionItem* > &items, 
							std::vector< kome::objects::SettingParameterValues > &settings
				
						);
				 * @brief get manipulat function info
				 * @param[in] val
				 * @param[out] items
				 * @param[out] settings
				 */
				void getManipulatFuncInfo(
					const char* val,
					std::vector< kome::plugin::PluginFunctionItem* > &items, 
					std::vector< kome::objects::SettingParameterValues > &settings
				
				);

			protected:
				/**
				 * @fn static bool lessPeak( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 )
				 * @brief compare peaks to sort
				 * @param[in] p0 peak to compare
				 * @param[in] p1 peak to be compared
				 * @return If true, p0 sorts before p1.
				 */
				static bool lessPeak( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 );
			};
		}
	}
}

#endif	// __KOME_SEARCH_NEIGHBOR_SPEC_PEAKS_H__
