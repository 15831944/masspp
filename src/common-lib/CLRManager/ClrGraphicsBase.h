/**
 * @file ClrGraphicsBase.h
 * @brief interfaces of ClrGraphicsBase class
 *
 * @author S.Tanaka
 * @date 2006.09.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_GRAPHICS_BASE_H__
#define __KOME_CLR_GRAPHICS_BASE_H__

#include "GraphicsWrapper.h"


namespace kome {
	namespace clr {

		ref class BitmapDataWrapper;

		/**
		 * @class ClrGraphicsBase
		 * @brief abstract class of graphics on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrGraphicsBase : public GraphicsWrapper {
#else
		public ref class ClrGraphicsBase abstract : public GraphicsWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrGraphicsBase()
			 * @brief constructor
			 */
			ClrGraphicsBase();

			/**
			 * @fn virtual ~ClrGraphicsBase()
			 * @brief destructor
			 */
			virtual ~ClrGraphicsBase();

		public:
			/**
			 * @fn virtual void onSetSize( unsigned int width, unsigned int height ) = 0
			 * @brief This method is called when the size of graphics is changed. (abstract method)
			 * @param[in] width graphics width
			 * @param[in] height graphics height
			 */
			virtual void onSetSize( unsigned int width, unsigned int height ) = 0;

			/**
			 * @fn virtual void onSetDrawStyle( DrawStyle style ) = 0
			 * @brief This method is called by setDrawStyleOnClr method. (abstract method)
			 * @param[in] style draw style
			 */
			virtual void onSetDrawStyle( DrawStyle style ) = 0;

			/**
			 * @fn virtual void onSetFillPattern( FillPattern pattern ) = 0
			 * @brief This method is called by setFillPatternOnClr method. (abstract method)
			 * @param[in] pattern fill pattern
			 */
			virtual void onSetFillPattern( FillPattern pattern ) = 0;

			/**
			 * @fn virtual void onSetFillStyle( FillStyle style ) = 0
			 * @brief This method is called by setFillStyleOnClr method. (abstract method)
			 * @param[in] style fill style
			 */
			virtual void onSetFillStyle( FillStyle style ) = 0;

			/**
			 * @fn virtual void onSetTextHAlign( TextHAlign hAlign ) = 0
			 * @brief This method is called by setTextHAlignOnClr method. (abstract method)
			 * @param[in] hAlign text horizontal align
			 */
			virtual void onSetTextHAlign( TextHAlign hAlign ) = 0;

			/**
			 * @fn virtual void onSetTextVAlign( TextVAlign vAlign ) = 0
			 * @brief This method is called by setTextVAlignOnClr method. (abstract method)
			 * @param[in] vAlign text vertical align
			 */
			virtual void onSetTextVAlign( TextVAlign vAlign ) = 0;

			/**
			 * @fn virtual void onSetColor( unsigned char red, unsigned char green, unsigned char blue ) = 0
			 * @brief this method is called by setColorOnClr method. (abstract method)
			 * @param[in] red red element of the color
			 * @param[in] green green element of the color
			 * @param[in] blue blue element of the color
			 */
			virtual void onSetColor( unsigned char red, unsigned char green, unsigned char blue ) = 0;

			/**
			 * @fn virtual void onSetFont( FontWrapper^ font ) = 0
			 * @brief This method is called by setFontOnClr method. (abstract method)
			 * @param[in] font font information object
			 */
			virtual void onSetFont( FontWrapper^ font ) = 0;

			/**
			 * @fn virtual void onSetPen( unsigned int width ) = 0
			 * @brief This method is called by setPenOnClr method. (abstract method)
			 * @param[in] width width
			 */
			virtual void onSetPen( unsigned int width ) = 0;

			/**
			 * @fn virtual void onDrawLines(
					cli::array< int, 2 >^ points
				) = 0
			 * @brief This mtdhod is called by drawLineOnClr method. (abstract method)
			 * @param[in] points points array
			 */
			virtual void onDrawLines(
				cli::array< int, 2 >^ points
			) = 0;

			/**
			 * @fn virtual void onFillPolygon(
					cli::array< int, 2 >^ points
				) = 0
			 * @brief This method is called by fillPolygonOnClr method. (abstract method)
			 * @param[in] points the vertex of a polygon
			 */
			virtual void onFillPolygon(
				cli::array< int, 2 >^ points
			) = 0;

			/**
			 * @fn virtual void onDrawEllipticArc(
						int px,
						int py,
						int width,
						int height,
						double start,
						double end,
						bool fill
					) = 0
			 * @brief This method is called by drawCircle, drawEllipse, drawArc, drawEllipticArc method (abstract method)
			 * @param[in] px the x coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] py the y coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] width the width of the rectangle that contains the ellipse
			 * @param[in] height the height of the rectangle that contains the ellipse
			 * @param[in] start start of the arc (degree)
			 * @param[in] end end of the arc (degree)
			 * @param[in] fill If true, drawing shape is filled.
			 */
			virtual void onDrawEllipticArc(
				int px,
				int py,
				int width,
				int height,
				double start,
				double end,
				bool fill
			) = 0;

			/**
			 * @fn virtual void onDrawText(
					System::String^ str,
					int px,
					int py,
					int angle
				) = 0
			 * @brief This method is called by drawTextOnClr method. (abstract method)
			 * @param[in] str the string to be drawn
			 * @param[in] px x coordinate of position
			 * @param[in] py y coordinate of position
			 * @param[in] angle rotated text angle
			 */
			virtual void onDrawText(
				System::String^ str,
				int px,
				int py,
				int angle
			) = 0;

			/**
			 * @fn virtual void onGetTextSize(
						System::String^ str,
						int% width,
						int% height
					) = 0
			 * @brief This method is called by getTextSize method. (abstract method)
			 * @param[in] str the string
			 * @param[out] width the pointer to store text width
			 * @param[out] height the pointer to store text height
			 */
			virtual void onGetTextSize(
				System::String^ str,
				int% width,
				int% height
			) = 0;

			/**
			 * @fn virtual void onDrawPoint( int px, int py ) = 0
			 * @brief This method is called by drawPoint method. (abstract method)
			 * @param[in] px x coordinate of point
			 * @param[in] py y coordinate of point
			 */
			virtual void onDrawPoint( int px, int py ) = 0;

			/**
			 * @fn virtual void onDrawBitmap( BitmapDataWrapper^ bitmap, int px, int py, bool transparent ) = 0
			 * @brief This method is called by drawBitmap method (abstract method)
			 * @param[in] bitmap bitmap data
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @param[in] transparent transparent flag
			 */
			virtual void onDrawBitmap( BitmapDataWrapper^ bitmap, int px, int py, bool transparent ) = 0;

			/**
			 * @fn virtual System::Drawing::Color onGetPixel( int px, int py ) = 0
			 * @brief This method is called by getPixel method (abstract method)
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @return pixel color
			 */
			virtual System::Drawing::Color onGetPixel( int px, int py ) = 0;

			/**
			 * @fn virtual void onFlush() = 0
			 * @brief This method is called by flush method. (abstract method)
			 */
			virtual void onFlush() = 0;
		};
	}
}


#endif	// __KOME_CLR_GRAPHICS_BASE_H__
