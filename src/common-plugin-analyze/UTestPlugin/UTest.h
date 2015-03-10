/**
 * @file UTest.h
 * @brief interfaces of CommonsMath class.
 *
 * @author satstnka
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_UTEST_H__
#define __KOME_UTEST_H__


namespace kome {
    namespace statistic {

		/**
		 * @class UTest
		 * @brief commons-math manager class
		 */
		class UTest {
		protected:
			/**
			 * @struct ValueInfo
			 * @brief value information
			 */
			struct ValueInfo {
				double val;
				int group;
				double order;
			};


		public:
			/**
			 * @fn static double test( double* vals0, const int n0, double* vals1, const int n1 )
			 * @brief calculates p-value of u-test
			 * @param[in] vals0 the array of values of the first group.
			 * @param[in] n0 the array size of values of the first group.
			 * @param[in] vals1 the array of values of the second group.
			 * @param[in] n1 the array size of values of the second group.
			 * @return p-value
			 */
			static double test( double* vals0, const int n0, double* vals1, const int n1 );

		protected:
			/**
			 * @fn static bool lessValue( ValueInfo& v0, ValueInfo& v1 )
			 * @brief compares values to sort
			 * @param[in] v0 value to be compared
			 * @parma[in] v1 value to compare
			 * @return If true, v0 sorts before v1.
			 */

			static bool lessValue( ValueInfo& v0, ValueInfo& v1 );
		};
	}
}


#endif    // __KOME_UTEST_H__
