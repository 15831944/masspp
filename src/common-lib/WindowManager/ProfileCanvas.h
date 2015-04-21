/**
 * @file ProfileCanvas.h
 * @brief interfaces of ProfileCanvas class
 *
 * @author S.Tanaka
 * @date 2008.09.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_PROFILE_CANVAS_H__
#define __KOME_WINDOW_PROFILE_CANVAS_H__


#include "CanvasBase.h"


namespace kome {
	namespace window {

		/**
		 * @class ProfileCanvas
		 * @brief base panel class to draw profile
		 */
		class WINDOW_MANAGER_CLASS ProfileCanvas : public CanvasBase {
		public:
			/**
			 * @fn ProfileCanvas( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			ProfileCanvas( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~ProfileCanvas()
			 * @brief destructor
			 */
			virtual ~ProfileCanvas();

		protected:
			/** prifle color */
			COLOR m_profileColor;

		public:
			/**
			 * @fn void setProfileColor( const COLOR color )
			 * @brief sets profile color
			 * @param[in] color profile color
			 */
			void setProfileColor( const COLOR color );

			/**
			 * @fn COLOR getProfileColor()
			 * @brief gets profile color
			 * @return profile color
			 */
			 COLOR getProfileColor();

		protected:
			/**
			 * @fn virtual void onPrepare( wxDC& dc )
			 * @brief this method is called when prepareing (override method)
			 * @param[in,out] dc device context to be drawed
			 */
			virtual void onPrepare( wxDC& dc );

			/**
			 * @fn virtual void onDrawGraphics( wxDC& dc )
			 * @brief this method is called when drawing (override method)
			 * @param[in,out] dc device context to be drawed
			 */
			virtual void onDrawGraphics( wxDC& dc );

		protected:
			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing
			 * @param[in,out] g graphics object to draw
			 */
			virtual void onPrepareDraw( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawWindowBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing window background
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph backgorund
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing profile
			 * @param[in,out] g graphics
			 */
			virtual void onDrawProfile( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foregorund
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_PROFILE_CANVAS_H__
