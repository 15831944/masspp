/**
 * @file MovingAverage.h
 * @brief interfaces of MovingAverage class
 *
 * @author S.Tanaka
 * @date 2012.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_MOVING_AVERAGE_H__
#define __KOME_NUMERIC_MOVING_AVERAGE_H__


namespace kome {
	namespace numeric {

		/**
		 * @class MovingAverage
		 * @brief moving average class
		 */
		class NUMERICAL_TOOLS_CLASS MovingAverage {
		public:
			/**
			 * @fn static void filter(
						kome::core::XYData& src,
						kome::core::XYData& dst,
						const int width,
						const int positionAtIndex0
						const double* weights
				)
			 * @brief applies moving average
			 * @param[in] src data points
			 * @param[out] dst the object to store filtered points
			 * @param[in] width the width of moving average
			 * @param[in] positionAtIndex0 the relative position of index 0
			 * @param[in] weights the array of weights
			 */
			static void filter(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				const int width,
				const int positionAtIndex0,
				const double* weights
			);
		};
	}
}

#endif		// __KOME_NUMERIC_MOVING_AVERAGE_H__
