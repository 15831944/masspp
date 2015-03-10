/**
 * @file LinearFilterManager.h
 * @brief interfaces of LinearFilterManager class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_LINEAR_FILTER_MANAGER_H__
#define __KOME_FILTER_LINEAR_FILTER_MANAGER_H__


namespace kome {
	namespace filter {
		namespace linear {

			/**
			 * @class LinearFilterManager
			 * @brief linear filter management class
			 */
			class LinearFilterManager {
			protected:
				/**
				 * @fn LinearFilterManager()
				 * @brief constructor
				 */
				LinearFilterManager();

				/**
				 * @fn virtual ~LinearFilterManager()
				 * @brief destructor
				 */
				virtual ~LinearFilterManager();

			public:
				/**
				 * @fn void averaging( const int width, kome::core::XYData& src, kome::core::XYData& dst )
				 * @brief executes averaging filter
				 * @param width width
				 * @param src source xy data
				 * @param dst the object to store filtered xy data
				 */
				void averaging( const int width, kome::core::XYData& src, kome::core::XYData& dst );

				/**
				 * @fn void gaussian( const double sigma, const int width, kome::core::XYData& src, kome::core::XYData& dst )
				 * @brief executes gaussian filter
				 * @param sigma sigma
				 * @param width width
				 * @param src source xy data
				 * @param dst the object to store filtered xy data
				 */
				void gaussian( const double sigma, const int width, kome::core::XYData& src, kome::core::XYData& dst );

				/**
				 * @fn void sharpening( const double strength, const int width, kome::core::XYData& src, kome::core::XYData& dst )
				 * @brief executes sharpening filter
				 * @param strength strength
				 * @param width width
				 * @param src source xy data
				 * @param dst the object to store filtered xy data
				 */
				void sharpening( const double strength, const int width, kome::core::XYData& src, kome::core::XYData& dst );

			public:
				/**
				 * @fn static LinearFilterManager& getInstance()
				 * @brief gets linear filter manager
				 * @return linear filter manager class (This is the only object.)
				 */
				static LinearFilterManager& getInstance();
			};
		}
	}
}

#endif		// __KOME_FILTER_LINEAR_FILTER_MANAGER_H__
