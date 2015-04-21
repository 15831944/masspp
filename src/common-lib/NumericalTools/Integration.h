/**
 * @file Integration.h
 * @brief interfaces of Integration class
 *
 * @author S.Tanaka
 * @date 2007.09.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_INTEGRATION_H__
#define __KOME_NUMERIC_INTEGRATION_H__


namespace kome {
	namespace numeric {

		/**
		 * @class Integration
		 * @brief integration class
		 */
		class NUMERICAL_TOOLS_CLASS Integration {
		public:
			/**
			 * @fn static double trapezoid( kome::core::XYData& xyData, const double a, const double b )
			 * @brief gets integration value by trapezoidal rule
			 * @param[in] xyData xy data
			 * @param[in] a start of the range
			 * @param[in] b end of the range
			 * @return integration value
			 */
			static double trapezoid( kome::core::XYData& xyData, const double a, const double b );
		};
	}
}

#endif		// __KOME_NUMERIC_INTEGRATION_H__
