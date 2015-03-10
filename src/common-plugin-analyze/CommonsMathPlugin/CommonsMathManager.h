/**
 * @file CommonsMathManager.h
 * @brief interfaces of CommonsMath class.
 *
 * @author satstnka
 * @date 2012.02.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_COMMONS_MATH_MANAGER_H__
#define __KOME_COMMONS_MATH_MANAGER_H__


namespace kome {
    namespace math {
		/**
		 * @class CommonsMathManager
		 * @brief commons-math manager class
		 */
		class CommonsMathManager {
		protected:
			/**
			 * @fn CommonsMathManager()
			 * @brief constructor
			 */
			CommonsMathManager();

			/**
			 * @fn virtual ~CommonsMathManager()
			 * @brief destructor
			 */
			virtual ~CommonsMathManager();

		public:
			/**
			 * @fn double tTest( double* s0, const int n0, double* s1, const int n1 )
			 * @briefi t-test
			 * @param[in] s0 the value array of the first sample.
			 * @param[in] n0 the array size of the first sample.
			 * @param[in] s1 the value array of the second sample
			 * @param[in] n1 the array size of the second sample
			 * @return p-value
			 */
			double tTest( double* s0, const int n0, double* s1, const int n1 );

			/**
			 * @fn double anova( double** vals, int* sizes, int groupCnt )
			 * @brief ANOVA (analysis of variance)
			 * @param[in] vals the array of values
			 * @param[in] sizes the array of group sizes
			 * @param[in] groupCnt group count
			 * @return p-value
			 */
			double anova( double** vals, int* sizes, int groupCnt );

		public:
			/**
			 * @fn static CommonsMathManager& getInstance()
			 * @brief gets the ComonsMathManager object. (This is the only object.)
			 * @return CommonsMathManager object
			 */
			static CommonsMathManager& getInstance();
		};
	}
}


#endif    // __KOME_COMMONS_MATH_MANAGER_H__
