/**
 * @file NormalDistribution.h
 * @brief interfaces of NormalDistribution class
 *
 * @author S.Tanaka
 * @date 2012.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_NORMAL_DISTRIBUTION_H__
#define __KOME_NUMERIC_NORMAL_DISTRIBUTION_H__


#include <vector>
#include <list>


namespace kome {
	namespace numeric {

		/**
		 * @class NormalDistribution
		 * @brief Dynamic Programing algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS NormalDistribution {
		public:
			/**
			 * @fn NormalDistribution()
			 * @brief constructor
			 */
			NormalDistribution();

			/**
			 * @fn virtual ~NormalDistribution()
			 * @brief destructor
			 */
			virtual ~NormalDistribution();

		protected:
			/** table size */
			unsigned int m_tableSize;
			
			/** table */
			double* m_table;

		public:
			/**
			 * @fn double calcP( const double x )
			 * @brief calculates p-value
			 * @param[in] x value
			 * @return p-value
			 */
			double calcP( const double x );
		};
	}
}


#endif		// __KOME_NUMERIC_NORMAL_DISTRIBUTION_H__
