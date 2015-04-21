/**
 * @file Math.h
 * @brief interfaces of Math class
 *
 * @author S.Tanaka
 * @date 2007.01.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_MATH_H__
#define __KOME_NUMERIC_MATH_H__


namespace kome {
	namespace numeric {

		/** 
		 * @class Math
		 * @brief mathematical tool class
		 */
		class NUMERICAL_TOOLS_CLASS Math {
		public:
			/** pi */
			static const double PI;
			/** napierian logarithm */
			static const double E;
		};
	}
}

#endif		// __KOME_NUMERIC_MATH_H__
