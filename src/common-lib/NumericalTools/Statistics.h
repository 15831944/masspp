/**
 * @file Statistics.h
 * @brief interfaces of Statistics class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_NUMERIC_STATISTICS_H__
#define __KOME_NUMERIC_STATISTICS_H__


namespace kome {
	namespace numeric {

		/**
		 * @class Statistics
		 * @brief statistics tool class
		 */
		class NUMERICAL_TOOLS_CLASS Statistics {
		public:
			/**
			 * @fn static double select( double* values, const unsigned int size, const unsigned int k )
			 * @brief gets the kth smallest value
			 * @param[in] values value array
			 * @param[in] size array size
			 * @param[in] k searching position
			 * @return the kth smallest value
			 */
			static double select( double* values, const unsigned int size, const unsigned int k );

			/**
			 * @fn static double median( double* values, const unsigned int size )
			 * @brief calculates median
			 * @param[in] values value array
			 * @param[in] size array size
			 * @return median
			 */
			static double median( double* values, const unsigned int size );

			/**
			 * @fn static double average( double* values, const unsigned int size )
			 * @brief calculates average
			 * @param[in] values value array
			 * @param[in] size array size
			 * @return average
			 */
			static double average( double* values, const unsigned int size );

			/**
			 * @fn static double variance( double* values, const unsigned int size )
			 * @brief calculates variance
			 * @param[in] values value array
			 * @param[in] size array size
			 * @return variance
			 */
			static double variance( double* values, const unsigned int size );

			/**
			 * @fn static double sd( double* values, const unsigned int size )
			 * @brief calculates standard deviation
			 * @param[in] values value array
			 * @param[in] size array size
			 * @return starndard deviation
			 */
			static double sd( double* values, unsigned int size );

			/**
			 * @fn static double pearson( double* xArray, double* yArray, const unsigned int size )
			 * @brief calculates Pearson's product-moment correlation coefficient
			 * @param[in] xArray the array of x data
			 * @param[in] yArray the array of y data
			 * @param[in] size the size of array
			 * @return Pearson's product-moment correlation coefficient
			 */
			static double pearson( double* xArray, double* yArray, const unsigned int size );

		protected:
			/**
			 * @fn static int partition( double* values, const int left, const int right, const int pvtIdx )
			 * @brief partitions elements of the array
			 * @param[in] values value array
			 * @param[in] left left index
			 * @param[in] right right index
			 * @param[in] pvtIdx pivot index
			 * @return partition position
			 */
			static int partition( double* values, const int left, const int right, const int pvtIdx ); 

			/**
			 * @fn static double select( double* values, const int left, const int right, const unsigned int k )
			 * @brief gets the kth smallest value
			 * @param[in] values value array
			 * @param[in] left left index
			 * @param[in] right right index
			 * @param[in] k search position
			 */
			static double select( double* values, const int left, const int right, const unsigned int k );
		};
	}
}

#endif		// __KOME_NUMERIC_STATISTICS_H__
