/**
 * @file CanvasBase.h
 * @brief interfaces of CanvasBase class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_CANVAS_BASE_H__
#define __KOME_WINDOW_CANVAS_BASE_H__


#include "Canvas.h"
#include "DCGraphics.h"


namespace kome {
	namespace window {

		/**
		 * @class CanvasBase
		 * @brief base panel class to draw profile
		 */
		class WINDOW_MANAGER_CLASS CanvasBase : public Canvas {
		public:
			/**
			 * @fn CanvasBase( wxWindow* parent, const int id, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			CanvasBase( wxWindow* parent, const int id, const int width, const int height );

			/**
			 * @fn virtual ~CanvasBase()
			 * @brief destructor
			 */
			virtual ~CanvasBase();

		protected:
			/** graphics object to draw */
			DCGraphics m_graphics;

		public:
			/**
			 * @fn kome::img::Graphics& getGraphics()
			 * @brief gets the graphics object
			 * @return graphics object
			 */
			kome::img::Graphics& getGraphics();

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

		protected:
			/**
			 * @fn virtual void onPrepare( wxDC& dc )
			 * @brief this method is called when prepareing
			 * @param[in,out] dc device context to be drawed
			 */
			virtual void onPrepare( wxDC& dc );

			/**
			 * @fn virtual void onDrawGraphics( wxDC& dc )
			 * @brief this method is called when drawing
			 * @param[in,out] dc device context to be drawed
			 */
			virtual void onDrawGraphics( wxDC& dc );

		protected:
			/**
			 * @fn void setDisplayedRange( kome::objects::Spectrum* spec )
			 * @brief set Displayed Range
			 * @param[in] spec spectrum
			 */
			void setDisplayedRange( kome::objects::Spectrum* spec );

			/**
			 * @fn viod setDisplayedRange( kome::objects::Chromatogram* chrom )
			 * @brief set Displayed Range
			 * @param[in] chrom chromatogram
			 */
			void setDisplayedRange( kome::objects::Chromatogram* chrom );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_CANVAS_BASE_H__
