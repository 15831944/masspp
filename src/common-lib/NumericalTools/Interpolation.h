/**
 * @file Interpolation.h
 * @brief interfaces of Interpolation class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_NUMERIC_INTERPOLATION_H__
#define __KOME_NUMERIC_INTERPOLATION_H__


namespace kome {
	namespace numeric {

		/**
		 * @class Interpolation
		 * @brief interpolation class
		 */
		class NUMERICAL_TOOLS_CLASS Interpolation {
		public:
			/**
			 * @fn static void linear(
					kome::core::XYData& src, 
					kome::core::XYData& dst,
					double* xArray,
					unsigned int size
				)
			 * @brief linear interpolation
			 * @param[in] src source point
			 * @param[out] dst the object to store interpolation points
			 * @param[in] xArray x coordinate of interpolation points
			 * @param[in] size the number of interpolation points
			 */
			static void linear(
				kome::core::XYData& src, 
				kome::core::XYData& dst,
				double* xArray,
				unsigned int size
			);

			/**
			 * @fn static double linear(
						kome::core::XYData& src,
						const double x
					)
			 * @brief linear interapolation
			 * @param[in] src source point
			 * @param[in] x the x coordinate of the point
			 * @return the y coordinate of the point
			 */
			static double linear(
				kome::core::XYData& src,
				double x
			);

			/**
			 * @fn static void spline(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					double* xArray,
					unsigned int size
				)
			 * @brief cubic spline interpolation
			 * @param[in] src source point
			 * @param[out] dst the object to store interpolation points
			 * @param[in] xArray x coordinate of interpolation points
			 * @param[in] size the number of interpolation points
			 */
			static void spline(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				double* xArray,
				unsigned int size
			);
		};
	}
}

#endif		// __KOME_NUMERIC_INTERPOLATION_H__
