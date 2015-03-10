/**
 * @file ChromatogramCanvasEx.h
 * @brief interfaces of ChromatogramCanvasEx class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_VIEW_CHROMATOGRAM_CANVAS_EX_H__
#define __KOME_VIEW_CHROMATOGRAM_CANVAS_EX_H__


#include <vector>
#include <stack>


namespace kome {
	namespace view {

		/**
		 * @class ChromatogramCanvasEx
		 * @brief drawing profile of DataMap panel class
		 */
		class ChromatogramCanvasEx : public kome::window::ChromatogramCanvas {
		public:
			/**
			 * @fn ChromatogramCanvasEx( wxWindow* parent, kome::objects::Chromatogram* chrom, const int width, const int height )
			 * @brief constructor
			 * @param parent parent frame window
			 * @param chrom chromatogram
			 * @param[in] width canvas width
			 * @param[in] height canvas height
			 */
			ChromatogramCanvasEx( wxWindow* parent, kome::objects::Chromatogram* chrom, const int width, const int height );

			/**
			 * @fn virtual ~ChromatogramCanvasEx()
			 * @brief destructor
			 */
			virtual ~ChromatogramCanvasEx();

		protected:
			/** title font */
			kome::img::Font m_titleFont;

			/**
			 * @struct ButtonRect
			 * @brief button area
			 */
			struct ButtonRect {
				int l;
				int r;
				int t;
				int b;
			};

			/** button area */
			ButtonRect m_buttonRect;

			/** start position */
			double m_startRt;

			/** end position */
			double m_endRt;

		protected:
			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing (override method)
			 * @param[in,out] g graphics object to draw
			 */
			virtual void onPrepareDraw( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph backgorund (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foregorund (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphFg( kome::window::DCGraphics& g );

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

#endif	//  __KOME_VIEW_CHROMATOGRAM_CANVAS_EX_H__
