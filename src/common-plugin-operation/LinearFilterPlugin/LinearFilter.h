/**
 * @file LinearFilter.h
 * @brief interfaces of LinearFilter class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_LINEAR_FILTER_H__
#define __KOME_FILTER_LINEAR_FILTER_H__


#include <vector>
#include <utility>


namespace kome {
	namespace filter {
		namespace linear {

			/**
			 * @class LinearFilter
			 * @brief linear filter
			 */
			class LinearFilter {
			public:
				/**
				 * @fn LinearFilter()
				 * @brief constructor
				 */
				LinearFilter();

				/**
				 * @fn virtual ~LinearFilter()
				 * @brief destructor
				 */
				virtual ~LinearFilter();

			protected:
				/** weights */
				std::vector< std::pair< int, double > > m_weights;

			public:
				/**
				 * @fn void addWeight( const int pos, double w )
				 * @brief adds weight
				 * @param pos position
				 * @param w weight
				 */
				void addWeight( const int pos, double w );

				/**
				 * @fn void clearWeight()
				 * @brief clears weight
				 */
				void clearWeight();
			
			public:
				/**
				 * @fn execute( kome::core::XYData& src, kome::core::XYData& dst )
				 * @brief executes filter
				 * @param src source xy data
				 * @param dst the object to store updated xy data
				 */
				void execute( kome::core::XYData& src, kome::core::XYData& dst );
			};
		}
	}
}

#endif		// __KOME_FILTER_LINEAR_FILTER_H__
