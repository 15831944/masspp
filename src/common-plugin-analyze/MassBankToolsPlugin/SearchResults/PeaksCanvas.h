/**
 * @file PeaksCanvas.h
 * @brief interfaces of PeaksCanvas
 *
 * @author S.Tanaka
 * @date 2009.09.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_PEAKS_CANVAS_H__
#define __KOME_MASBANK_PEAKS_CANVAS_H__

#include <stack>

namespace kome {
	namespace massbank {
		/**
		 * @class PeaksCanvas
		 * @brief MassBank record edit dialog class
		 */
		class PeaksCanvas : public kome::window::ProfileCanvas {
		public:
			/**
			 * @fn PeaksCanvas( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			PeaksCanvas( wxWindow* parent );

			/**
			 * @fn virtual ~PeaksCanvas()
			 * @brief destructor
			 */
			virtual ~PeaksCanvas();

		protected:
			/** the stack of x range */
			std::stack< std::pair< double, double > > m_xRange;

			/** the stack of y range */
			std::stack< std::pair< double, double > > m_yRange;

			/** click point */
			kome::core::Point< int > m_click;

			/** drag point */
			kome::core::Point< int > m_drag;

		protected:
			/**
			 * @typedef RangeOperation
			 * @brief range operation
			 */
			typedef enum {
				OP_NONE,
				OP_X_RANGE,
				OP_Y_RANGE
			} RangeOperation;

			/** range operation */
			RangeOperation m_rangeOp;


		protected:
			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked.
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved.
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );


		protected:
			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing (override method)
			 * @param g graphics object to draw
			 */
			virtual void onPrepareDraw( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawWindowBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing window background (override method)
			 * @param g graphics
			 */
			virtual void onDrawWindowBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph backgorund (override method)
			 * @param g graphics
			 */
			virtual void onDrawGraphBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing profile (override method)
			 * @param g graphics
			 */
			virtual void onDrawProfile( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foregorund (override method)
			 * @param g graphics
			 */
			virtual void onDrawGraphFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_MASBANK_PEAKS_CANVAS_H__
