/**
 * @file Differentiation.h
 * @brief interfaces of Differentiation class
 *
 * @author S.Tanaka
 * @date 2012.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_DIFFERENTIATION_H__
#define __KOME_NUMERIC_DIFFERENTIATION_H__


namespace kome {
	namespace numeric {

		/**
		 * @class Differentiation
		 * @brief differentiation class
		 */
		class NUMERICAL_TOOLS_CLASS Differentiation {
		public:
			/**
			 * @fn static void diffrentiate( kome::core::XYData& src, kome::core::XYData& dst )
			 * @brief differentiate points
			 * @param[in] src source points
			 * @param[out] dst the data points object to store  differentiated points
			 */
			static void diffrentiate( kome::core::XYData& src, kome::core::XYData& dst );
		};
	}
}

#endif		// __KOME_NUMERIC_DIFFERENTIATION_H__
