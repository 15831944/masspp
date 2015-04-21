/**
 * @file BitmapCanvas.h
 * @brief interfaces of BitmapCanvas class
 *
 * @author S.Tanaka
 * @date 2013.01.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_BITMAP_CANVAS_H__
#define __KOME_WINDOW_BITMAP_CANVAS_H__


#include "Canvas.h"
#include "DCGraphics.h"


namespace kome {
	namespace window {

		/**
		 * @class BitmapCanvas
		 * @brief base panel class to draw profile
		 */
		class WINDOW_MANAGER_CLASS BitmapCanvas : public Canvas {
		public:
			/**
			 * @fn BitmapCanvas( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			BitmapCanvas( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~BitmapCanvas()
			 * @brief destructor
			 */
			virtual ~BitmapCanvas();

		protected:
			/** bitmap */
			wxBitmap* m_bitmap;

			/** path */
			std::string m_path;

		public:
			/**
			 * @fn void setImageFile( const char* path )
			 * @brief sets the image file path
			 * @param[in] path image file path
			 */
			void setImageFile( const char* path );

		protected:
			/**
			 * @fn virtual void onDraw( wxDC& dc )
			 * @brief This method is called when draw event (override method)
			 * @param[in,out] dc device context
			 */
			virtual void onDraw( wxDC& dc );

			/**
			 * @fn virtual void onSize( const unsigned int width, const unsigned int height )
			 * @brief This method is called when window size is changed (override method)
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			virtual void onSize( const unsigned int width, const unsigned int height );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_BITMAP_CANVAS_H__
