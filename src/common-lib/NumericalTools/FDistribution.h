/**
 * @file FDistribution.h
 * @brief interfaces of FDistribution class
 *
 * @author S.Tanaka
 * @date 2010.11.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_F_DISTRIBUTION_H__
#define __KOME_NUMERIC_F_DISTRIBUTION_H__


#include <vector>
#include <list>


namespace kome {
	namespace numeric {

		/**
		 * @class FDistribution
		 * @brief Dynamic Programing algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS FDistribution {
		public:
			/**
			 * @fn FDistribution( const int m, const int n )
			 * @brief constructor
			 * @param[in] m the first degree of freedom
			 * @param[in] n the second degree of freedom
			 */
			FDistribution( const int m, const int n );

			/**
			 * @fn virtual ~FDistribution()
			 * @brief destructor
			 */
			virtual ~FDistribution();

		protected:
			/** the first degree of freedom */
			int m_m;

			/** the second degree of freedom */
			int m_n;

			/** coefficient */
			double m_coefficient;

		protected:
			/** object list */
			static std::list< FDistribution > m_objList;

		public:
			/**
			 * @fn double calcP( const double f )
			 * @brief calculates p-value
			 * @param[in] f f-value
			 * @return p-value
			 */
			double calcP( const double f );

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
			 * @fn static double calcF(
					const int num,
					double** vals,
					int* nums
				)
			 * @brief calculates f value
			 * @param[in] num the number of groups
			 * @param[in] vals the array of array of values
			 * @param[in] nums the array of array size
			 * @return f value
			 */
			static double calcF(
				const int num,
				double** vals,
				int* nums
			);

			/**
			 * @fn static double anova(
					const int num,
					double** vals,
					int* nums
				)
			 * @brief executes anova (analysis of variance)
			 * @param[in] num the number of groups
			 * @param[in] vals the array of array of values
			 * @param[in] nums the array of array size
			 * @return p value
			 */
			static double anova(
				const int num,
				double** vals,
				int* nums
			);

		public:
			/**
			 * @fn double calcFrequency( const double x )
			 * @brief calculates frequency
 			 * @param[in] x probability variable
			 */
			double calcFrequency( const double x );

			/**
			 * @fn static double calcFrequency( const double x, const int v0, const int v1 )
			 * @brief frequency function of t-distribution
			 * @param[in] x probability variable
			 * @param[in] v0 the first degree of freedom
			 * @param[in] v1 the second degree of freedom
			 */
			static double calcFrequency( const double x, const int v0, const int v1 );

		protected:
			/**
			 * @fn static double calcCoefficient( const int v0, const int v1 )
			 * @brief calculate coefficient
			 * @param[in] v0 the first degree of freedom
			 * @param[in] v1 the second degree of freedom
			 * @return coefficient
			 */
			static double calcCoefficient( const int v0, const int v1 );
		};
	}
}

#endif		// __KOME_NUMERIC_F_DISTRIBUTION_H__
