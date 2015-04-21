/**
 * @file BitmapData.h
 * @brief interfaces of BitmapData class
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_BITMAP_DATA_H__
#define __KOME_IMG_BITMAP_DATA_H__


#include "ColorTool.h"


namespace kome {
	namespace img {

		/**
		 * @class BitmapData
		 * @brief Bitmap image management class.
		 */
		class IMAGE_MANAGER_CLASS BitmapData {
		public:
			/**
			 * @fn BitmapData()
			 * @brief constructor
			 */
			BitmapData();

			/**
			 * @fn BitmapData( unsigned int width, unsigned int height )
			 * @brief constructor
			 * @param[in] width width of bitmap image
			 * @param[in] height height of bitmap image
			 */
			BitmapData( unsigned int width, unsigned int height );

			/**
			 * @fn BitmapData( const BitmapData& bitmapData )
			 * @brief copy constructor
			 * @param[in] bitmapData BitmapData object
			 */
			BitmapData( const BitmapData& bitmapData );

			/**
			 * @fn virtual ~BitmapData()
			 * @brief destructor
			 */
			virtual ~BitmapData();

		protected:
			/** bitmap data array */
			COLOR* m_bitmap;

			/** width */
			unsigned int m_width;

			/** height */
			unsigned int m_height;

		public:
			/**
			 * @fn unsigned int getWidth()
			 * @brief gets bitmap width
			 * @return bitmap width
			 */
			unsigned int getWidth();

			/**
			 * @fn unsigned int getHeight()
			 * @brief gets bitmap height
			 * @return bitmap height
			 */
			unsigned int getHeight();

			/**
			 * @fn void resize( const unsigned int width, const unsigned int height )
			 * @brief resize bitmap
			 * @param[in] width bitmap width
			 * @param[in] height bitmap height
			 */
			void resize( const unsigned int width, const unsigned int height );

			/**
			 * @fn void setPixel( const unsigned int x, const unsigned int y, const COLOR color )
			 * @brief sets the color of the pixel located in specified coordinate
			 * @param[in] x x coordinate of the pixel
			 * @param[in] y y coordinate of the pixel
			 * @param[in] color color
			 */
			void setPixel( const unsigned int x, const unsigned int y, const COLOR color );

			/**
			 * @fn COLOR getPixel( const unsigned int x, const unsigned int y )
			 * @brief gets the color of the pixel located in specified coordinate
			 * @param[in] x x coordinate of the pixel
			 * @param[in] y y coordinate of the pixel
			 * @return the color of the pixel located in specified coordinate
			 */
			COLOR getPixel( const unsigned int x, const unsigned int y );

		protected:
			/**
			 * @fn void createArray()
			 * @brief creates bitmap data array
			 */
			void createArray();

			/**
			 * @fn void deleteArray()
			 * @brief deletes bitmap data array
			 */
			void deleteArray();

			/**
			 * @fn int getIndexOfArray( unsigned int x, unsigned int y );
			 * @brief detects the index of the array with coordinate
			 * @param[in] x x coordinate
			 * @param[in] y y coordinate
			 * @return the index of the array ( returns negative number if x or y is illegal )
			 */
			int getIndexOfArray( unsigned int x, unsigned int y );
		};
	}
}

#endif	// __KOME_IMG_BITMAP_DATA_H__
