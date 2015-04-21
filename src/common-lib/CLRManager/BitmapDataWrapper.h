/**
 * @file BitmapDataWrapper.h
 * @brief interfaces of BitmapDataWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_BITMAP_DATA_WRAPPER_H__
#define __KOME_CLR_BITMAP_DATA_WRAPPER_H__


namespace kome {
	namespace clr {

		/**
		 * @class BitmapDataWrapper
		 * @brief BitmapData object wrapper to use on CLR environment
		 */
		public ref class BitmapDataWrapper {
		public:
			/**
			 * @fn BitmapDataWrapper( kome::img::BitmapData& bitmap )
			 * @brief constructor
			 * @param[in] bitmap BitmapData object
			 */
			BitmapDataWrapper( kome::img::BitmapData& bitmap );

			/**
			 * @fn virtual ~BitmapDataWrapper()
			 * @brief destructor
			 */
			virtual ~BitmapDataWrapper();

		protected:
			/** BitmapData object */
			kome::img::BitmapData& m_bitmap;

		public:
			/**
			 * @fn kome::img::BitmapData& getBitmapData()
			 * @brief gets BitmapData object
			 * @return the pointer to BitmapData object
			 */
			kome::img::BitmapData& getBitmapData();

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
			 * @fn void resize( unsigned int width, unsigned int height )
			 * @brief resize bitmap
			 * @param[in] width bitmap width
			 * @param[in] height bitmap height
			 */
			void resize( unsigned int width, unsigned int height );

			/**
			 * @fn void setPixel( unsigned int x, unsigned int y, COLOR color )
			 * @brief sets the color of the pixel located in specified coordinate
			 * @param[in] x x coordinate of the pixel
			 * @param[in] y y coordinate of the pixel
			 * @param[in] color color
			 */
			void setPixel( unsigned int x, unsigned int y, COLOR color );

			/**
			 * @fn COLOR getPixel( unsigned int x, unsigned int y )
			 * @brief gets the color of the pixel located in specified coordinate
			 * @param[in] x x coordinate of the pixel
			 * @param[in] y y coordinate of the pixel
			 * @return the color of the pixel located in specified coordinate
			 */
			COLOR getPixel( unsigned int x, unsigned int y );

		};
	}
}


#endif	// __KOME_CLR_BITMAP_DATA_WRAPPER_H__
