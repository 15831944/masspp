/**
 * @file PeakValuesCalculator.h
 * @brief Peak Values Calculator class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_TOOL_PEAK_VALUES_CALCULATOR_H__
#define __KOME_IDENT_TOOL_PEAK_VALUES_CALCULATOR_H__

namespace kome {
	namespace ident {

		class PeakPositions;
		class SampleGetter;

		/**
		 * @class PeakValuesCalculator
		 * @brief peak values calculator
		 */
		class IDENT_TOOLS_CLASS PeakValuesCalculator {
		
		public:
			/**
			 * @fn PeakValuesCalculator()
			 * @brief constructor
			 */
			PeakValuesCalculator();

			/**
			 * @fn virtual ~PeakValuesCalculator()
			 * @brief destructor
			 */
			virtual ~PeakValuesCalculator();

		protected:
			/** error message */
			std::string m_errorMsg;

		public:
			/**
			 * @fn void calculateValues( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress )
			 * @brief calculate values
			 * @param[out] sample
			 * @param[out] peakPos
			 * @param[out] progress
			 */
			void calculateValues( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress );
		
			/**
			 * @fn virtual void onCalculateValues( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress )
			 * @brief on calculate value
			 * @param[out] sample
			 * @param[out] peakPos
			 * @param[out] progress
			 */
			virtual void onCalculateValues( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress ) = 0;

			/**
			 * @fn void setLastError( const char* msg )
			 * @brief set last error
			 * @param[in] msg
			 */
			void setLastError( const char* msg );

			/**
			 * @fn std::string getLastError()
			 * @brief get last error
			 * @return last error message
			 */
			std::string getLastError();
		};
	}
}

#endif // __KOME_IDENT_TOOL_PEAK_VALUES_CALCULATOR_H__
