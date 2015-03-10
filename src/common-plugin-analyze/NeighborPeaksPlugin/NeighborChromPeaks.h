/**
 * @file NeighborChromPeaks.h
 * @brief interfaces of NeighborChromPeaks class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_NEIGHBOR_CHROM_PEAKS_H__
#define __KOME_SEARCH_NEIGHBOR_CHROM_PEAKS_H__


#include "NeighborPeaks.h"


namespace kome {
	namespace search {
		namespace neighbor {

			/**
			 * @class NeighborChromPeaks
			 * @brief Neighbor Chromatogram Peaks class
			 */
			class NeighborChromPeaks : public NeighborPeaks {
			public:
				/**
				 * @fn NeighborChromPeaks(
							kome::objects::SettingParameterValues* settings
						)
				 * @brief constructor
				 * @param group group
				 * @param settings setting parameter values
				 */
				NeighborChromPeaks(
					kome::objects::SettingParameterValues* settings
				);

				/**
				 * @fn virtual ~NeighborChromPeaks()
				 * @brief destructor
				 */
				virtual ~NeighborChromPeaks();

			protected:
				/** base peak chromatogram flag */
				bool m_bpcFlg;

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
							kome::objects::Chromatogram& chrom,
							kome::objects::Peaks& peaks,
							kome::core::DataPoints& dps,
							std::vector< kome::plugin::PluginFunctionItem* > items,
							std::vector< kome::objects::SettingParameterValues > settingVals
						)
				 * @param[in] chrom chromatogram
				 * @param[out] peaks the object to store peaks
				 * @param[in] profileFlg If true, registering profile information
				 */
				void getPeaks( 
					kome::objects::Chromatogram& chrom,
					kome::objects::Peaks& peaks,
					kome::core::DataPoints& dps,
					std::vector< kome::plugin::PluginFunctionItem* > items,
					std::vector< kome::objects::SettingParameterValues > settingVals
				);

				/**
				 * @fn kome::objects::PeakElement* searchPeak( const double rt, kome::objects::Peaks& peaks, double* val )
				 * @brief search peak
				 * @pram[in] rt RT position
				 * @param[in] peaks peaks information
				 * @param[out] val the pointer to store peak value
				 */
				kome::objects::PeakElement* searchPeak( const double rt, kome::objects::Peaks& peaks, double* val );
				
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

#endif	// __KOME_SEARCH_NEIGHBOR_CHROM_PEAKS_H__
