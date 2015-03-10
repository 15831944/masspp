/**
 * @file NeighborPeakValuesCalculator.h
 * @brief interfaces of NeighborPeakValuesCalculator class
 *
 * @author M.Izumi
 * @date 2013.12.20
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_SEARCH_NEIGHBOR_PEAK_VALUE_CALCULATOR_H__
#define __KOME_SEARCH_NEIGHBOR_PEAK_VALUE_CALCULATOR_H__

namespace kome { 
	namespace search {
		namespace neighbor {
			
			/**
			 * @class NeighborPeakValuesCalculator
			 * @brief neighbor peak values calculator
			 */
			class NeighborPeakValuesCalculator : public kome::ident::PeakValuesCalculator {
			public:
				/**
				 * @fn NeighborPeakValuesCalculator()
				 * @brief constructor
				 */
				NeighborPeakValuesCalculator( bool bSpec );
				
				/**
				 * @fn virtual ~NeighborPeakValuesCalculator()
				 * @brief destructor
				 */
				virtual ~NeighborPeakValuesCalculator();

			protected:
				/** select flag */
				bool m_bSpec;
				/** settings */
				kome::objects::SettingParameterValues* m_settings;

			public:
				/**
				 * @fn void onCalculateValues( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress )
				 * @brief on calculate values
				 * @param getter
				 * @param peakPos
				 * @param progress
				 */
				void onCalculateValues( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress );

			public:
				/**
				 * @fn void setSettings( kome::objects::SettingParameterValues* settings )
				 * @brief set the settings
				 * @param settings
				 */
				void setSettings( kome::objects::SettingParameterValues* settings );

				/**
				 * @fn kome::objects::SettingParameterValues* getSettings()
				 * @brief get settings
				 * @return settings object
				 */
				kome::objects::SettingParameterValues* getSettings();
			
			};

		}
	}
}


#endif // __KOME_SEARCH_NEIGHBOR_PEAK_VALUE_CALCULATOR_H__
