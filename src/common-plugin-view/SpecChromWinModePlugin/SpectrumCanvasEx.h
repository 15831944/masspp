/**
 * @file SpectrumCanvasEx.h
 * @brief interfaces of SpectrumCanvasEx class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_VIEW_SPECTRUM_CANVAS_EX_H__
#define __KOME_VIEW_SPECTRUM_CANVAS_EX_H__


#include <vector>
#include <stack>


namespace kome {
	namespace view {

		/**
		 * @class SpectrumCanvasEx
		 * @brief drawing profile of DataMap panel class
		 */
		class SpectrumCanvasEx : public kome::window::SpectrumCanvas {
		public:
			/**
			 * @fn SpectrumCanvasEx( wxWindow* parent, kome::objects::Spectrum* spec, const int width, const int height );
			 * @param[in] parent parent window
			 * @param[in] spec spectrum
			 * @param[in] size size
			 * @param[in] width canvas width
			 * @param[in] height canvas height
			 */
			SpectrumCanvasEx( wxWindow* parent, kome::objects::Spectrum* spec, const int width, const int height );

			/**
			 * @fn virtual ~SpectrumCanvasEx()
			 * @brief destructor
			 */
			virtual ~SpectrumCanvasEx();

		protected:
			/** title font */
			kome::img::Font m_titleFont;

		protected:
			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing (override method)
			 * @param[in,out] g graphics object to draw
			 */
			virtual void onPrepareDraw( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );	

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_VIEW_SPECTRUM_CANVAS_EX_H__
