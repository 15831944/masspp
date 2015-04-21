/**
 * @file FontCanvas.h
 * @brief interfaces of FontCanvas class
 *
 * @author S.Tanaka
 * @date 2013.04.08
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_FONT_CANVAS_H__
#define __KOME_WINDOW_FONT_CANVAS_H__


#include "CanvasBase.h"


namespace kome {
	namespace window {

		/**
		 * @class FontCanvas
		 * @brief base panel class to draw profile
		 */
		class WINDOW_MANAGER_CLASS FontCanvas : public CanvasBase {
		public:
			/**
			 * @fn FontCanvas( wxWindow* parent, const int id, const int width = 150, const int height = 50 )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			FontCanvas( wxWindow* parent, const int id, const int width = 150, const int height = 50 );

			/**
			 * @fn virtual ~FontCanvas()
			 * @brief destructor
			 */
			virtual ~FontCanvas();

		protected:
			/** font */
			kome::img::Font m_font;

			/** color */
			COLOR m_color;

			/** text */
			std::string m_text;

		public:
			/**
			 * @fn void setFont( kome::img::Font& font )
			 * @brief set font
			 * @param font
			 */
			void setFont( kome::img::Font& font );

			/**
			 * @fn kome::img::Font& getFont()
			 * @brief get font
			 * @return font
			 */
			kome::img::Font& getFont();

			/**
			 * @fn void setColor( const COLOR c )
			 * @brief set color
			 * @param c
			 */
			void setColor( const COLOR c );

			/**
			 * @fn COLOR getColor()
			 * @brief get color
			 * @return color
			 */
			COLOR getColor();

			/**
			 * @fn void setText( const char* text )
			 * @brief set text
			 * @param text
			 */
			void setText( const char* text );

			/**
			 * @fn const char* getText()
			 * @brief get text
			 * @return text
			 */
			const char* getText();

		protected:
			/**
			 * @fn virtual void onDraw( wxDC& dc )
			 * @brief This method is called when draw event (override method)
			 * @param[in,out] dc device context
			 */
			virtual void onDraw( wxDC& dc );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_FONT_CANVAS_H__
