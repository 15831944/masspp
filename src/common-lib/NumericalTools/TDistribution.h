/**
 * @file TDistribution.h
 * @brief interfaces of TDistribution class
 *
 * @author S.Tanaka
 * @date 2010.11.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_T_DISTRIBUTION_H__
#define __KOME_NUMERIC_T_DISTRIBUTION_H__


#include <vector>
#include <list>


namespace kome {
	namespace numeric {

		/**
		 * @class TDistribution
		 * @brief Dynamic Programing algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS TDistribution {
		public:
			/**
			 * @fn TDistribution( const int n )
			 * @brief constructor
			 * @param[in] n the degree of freedom
			 */
			TDistribution( const int n );

			/**
			 * @fn virtual ~TDistribution()
			 * @brief destructor
			 */
			virtual ~TDistribution();

		protected:
			/** degree of freedom */
			int m_n;

			/** coefficient */
			double m_coefficient;

		protected:
			/** object list */
			static std::list< TDistribution > m_objList;

		public:
			/**
			 * @fn double calcP( const double t )
			 * @brief calculates p-value
			 * @param[in] t t-value
			 * @return p-value
			 */
			double calcP( const double t );

		protected:
			/** distribution */
			std::vector< double > m_distribution;

		protected:
			/**
			 * @fn void createArray()
			 * @brief creates array
			 */
			void createArray();

		public:
			/**
			 * @fn static double calcT(
						double* vals0,
						const unsigned int num0,
						double* vals1,
						const unsigned int num1
					)
             * @brief calculates t value
			 * @param[in] vals0 the array of the first data set
			 * @param[in] num0 the size of the first data set
			 * @param[in] vals1 the array of second data set
			 * @param[in] num1 the size of the first data set
			 * @return T value
			 */
			static double calcT(
				double* vals0,
				const unsigned int num0,
				double* vals1,
				const unsigned int num1
			);

			/**
			 * @fn static double ttest(
						double* vals0,
						const unsigned int num0,
						double* vals1,
						const unsigned int num1
				)
			 * @brief executes t-test
			 * @param[in] vals0 the array of the first data set
			 * @param[in] num0 the size of the first data set
			 * @param[in] vals1 the array of second data set
			 * @param[in] num1 the size of the first data set
			 * @return p value
			 */
			static double ttest(
				double* vals0,
				const unsigned int num0,
				double* vals1,
				const unsigned int num1
			);

		public:
			/**
			 * @fn double calcFrequency( const double x )
			 * @brief calculates frequency
 			 * @param[in] x probability variable
			 */
			double calcFrequency( const double x );

			/**
			 * @fn static double calcFrequency( const double x, const int v )
			 * @brief frequency function of t-distribution
			 * @param[in] x probability variable
			 * @param[in] v degree of freedom
			 */
			static double calcFrequency( const double x, const int v );

		protected:
			/**
			 * @fn static double calcCoefficient( const int v )
			 * @brief calculate coefficient
			 * @param[in] v degree of freedom
			 * @return coefficient
			 */
			static double calcCoefficient( const int v );
		};
	}
}

#endif		// __KOME_NUMERIC_T_DISTRIBUTION_H__
