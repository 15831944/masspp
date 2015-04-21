/**
 * @file Graphics.h
 * @brief interfaces of Graphics object
 *
 * @author S.Tanaka
 * @date 2006.09.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

  

#ifndef __KOME_IMG_GRAPHICS_H__
#define __KOME_IMG_GRAPHICS_H__


#include "ColorTool.h"
#include "Font.h"


namespace kome {
	namespace img {

		class BitmapData;

		/**
		 * @class Graphics
		 * @brief abstraction class that treats operation of graphics
		 */
		class IMAGE_MANAGER_CLASS Graphics {
		public:
			/**
			 * @fn Graphics()
			 * @brief constructor
			 */
			Graphics();

			/** 
			 * @fn virtual ~Graphics()
			 * @brief destructor
			 */
			virtual ~Graphics();

		public:
			/**
			 * @typedef DrawStyle
			 * @brief definition of drawing style
			 */
			typedef enum {
				STYLE_SOLID,		// "_______________________________"
				STYLE_DOT,		  // "..............................."
				STYLE_LONG_DASH,	// "___ ___ ___ ___ ___ ___ ___ ___"
				STYLE_SHORT_DASH,   // "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ "
				STYLE_DOT_DASH	  // "__ . __ . __ . __ . __ . __ . __"
			} DrawStyle;

			/**
			 * @typedef FillPattern
			 * @brief definition of filling pattern
			 */
			typedef enum {
				PATTERN_SOLID,		//
				PATTERN_BDIAGONAL,	// "////////////////////////////////"
				PATTERN_FDIAGONAL,	// "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
				PATTERN_CROSS,		// "++++++++++++++++++++++++++++++++"
				PATTERN_CROSS_DIAG,   // "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
				PATTERN_HORIZONTAL,   // "================================"
				PATTERN_VERTICAL,	 // "||||||||||||||||||||||||||||||||"
			} FillPattern;

			/**
			 * @typedef FillStyle
			 * @brief definition of filling style
			 */
			typedef enum {
				RULE_ODDEVEN,
				RULE_WINDING
			} FillStyle;

			/**
			 * @typedef TextHAlign
			 * @brief definition of horizontal alignment
			 */
			typedef enum {
				HALIGN_LEFT,
				HALIGN_CENTER,
				HALIGN_RIGHT
			} TextHAlign;

			/**
			 * @typedef TextVAlign
			 * @brief definition of vertical alignment
			 */
			typedef enum {
				VALIGN_TOP,
				VALIGN_MIDDLE,
				VALIGN_BOTTOM
			} TextVAlign;

			/**
			 * @typedef Mark
			 * @brief deeefinition of marks
			 */
			typedef enum {
				MARK_NONE		    =  0,
				MARK_CIRCLE		    =  1,
				MARK_FILLED_CIRCLE  =  2,
				MARK_DELTA		    =  3,
				MARK_FILLED_DELTA   =  4,
				MARK_NABLA		    =  5,
				MARK_FILLED_NABLA   =  6,
				MARK_SQUARE		    =  7,
				MARK_FILLED_SQUARE  =  8,
				MARK_DIAMOND		=  9,
				MARK_FILLED_DIAMOND = 10,
				MARK_STAR		    = 11,
				MARK_FILLED_STAR	= 12,
				MARK_PLUS		    = 13,
				MARK_TIMES		    = 14,
				MARK_TRIANGLE		= 15	// @date 2011.03.29 <Add> M.Izumi
			} Mark;

		public:
			/**
			 * @fn void setGraphPosition(
					const int left,
					const int top,
					const int right,
					const int bottom
				)
			 * @brief sets the position of the graph
			 * @param[in] left x coordinate in left end of graph
			 * @param[in] top y coordinate in top end of graph
			 * @param[in] right x coordinate in right end of graph
			 * @param[in] bottom y coordinate in bottom end of graph
			 */
			void setGraphPosition(
				const int left,
				const int top,
				const int right,
				const int bottom
			);

			/**
			 * @fn void setGraphPositionLeft( const int left )
			 * @brief sets x coordinate in left end of graph
			 * @param[in] left x coordinate in left end of graph
			 */
			void setGraphPositionLeft( const int left );

			/**
			 * @fn void setGraphPositionTop( const int top )
			 * @brief sets y coordinate in top end of graph
			 * @param[in] top y coordinate in top end of graph
			 */
			void setGraphPositionTop( const int top );

			/**
			 * @fn void setGraphPositionRight( const int right )
			 * @brief sets x coordinate in right end of graph
			 * @param[in] right y coordinate in right end of graph
			 */
			void setGraphPositionRight( const int right );

			/**
			 * @fn void setGraphPositionBottom( const int bottom )
			 * @brief sets y coordinate in bottom end of graph
			 * @param[in] bottom y coordinate in bottom end of graph
			 */
			void setGraphPositionBottom( const int bottom );

			/**
			 * @fn int getGraphPositionLeft()
			 * @brief gets x coordinate in left end of graph
			 * @return x coordinate in left end of graph
			 */
			int getGraphPositionLeft();

			/**
			 * @fn int getGraphPositionTop()
			 * @brief gets y coordinate in top end of graph
			 * @return y coordinate in top end of graph
			 */
			int getGraphPositionTop();

			/**
			 * @fn int getGraphPositionRight()
			 * @brief gets x coordinate in right end of graph
			 * @return x coordinate in right end of graph
			 */
			int getGraphPositionRight();

			/**
			 * @fn int getGraphPositionBottom()
			 * @brief gets y coordinate in bottom end of graph
			 * @return y coordinate in bottom end of graph
			 */
			int getGraphPositionBottom();

			/**
			 * @fn void setGraphMargin(
					const int left,
					const int top,
					const int right,
					const int bottom
				)
			 * @brief sets graph margin information
			 * @param[in] left length of margin left in graph
			 * @param[in] top length of margin top in graph
			 * @param[in] right length of margin right in graph
			 * @param[in] bottom length of margin bottom in graph
			 */
			void setGraphMargin(
				const int left,
				const int top,
				const int right,
				const int bottom
			);

			/**
			 * @fn void setGraphMarginLeft( const int left )
			 * @brief sets length of margin left in graph
			 * @param[in] left length of margin left in graph
			 */
			void setGraphMarginLeft( const int left );

			/**
			 * @fn void setGraphMarginTop( const int top )
			 * @brief sets length of margin top in graph
			 * @param[in] top length of margin top in graph
			 */
			void setGraphMarginTop( const int top );

			/**
			 * @fn void setGraphMarginRight( const int right )
			 * @brief sets length of margin right in graph
			 * @param[in] right length of margin right in graph
			 */
			void setGraphMarginRight( const int right );

			/**
			 * @fn void setGraphMarginBottom( const int bottom )
			 * @brief sets length of margin bottom in graph
			 * @param[in] bottom length of margin bottom in graph
			 */
			void setGraphMarginBottom( const int bottom );

			/**
			 * @fn int getGraphMarginLeft()
			 * @brief gets length of margin left in graph
			 * @return length of margin left in graph
			 */
			int getGraphMarginLeft();

			/**
			 * @fn int getGraphMarginTop()
			 * @brief gets length of margin top in graph
			 * @return length of margin top in graph
			 */
			int getGraphMarginTop();

			/**
			 * @fn int getGraphMarginRight()
			 * @brief gets length of margin right in graph
			 * @return length of margin right in graph
			 */
			int getGraphMarginRight();

			/**
			 * @fn int getGraphMarginBottom()
			 * @brief gets length of margin bottom in graph
			 * @return length of margin bottom in graph
			 */
			int getGraphMarginBottom();

			/**
			 * @fn void setSize( const unsigned int width, const unsigned int height )
			 * @brief sets size of drawing area
			 * @param[in] width width of drawing area
			 * @param[in] height height of drawing area
			 */
			void setSize( const unsigned int width, const unsigned int height );

			/**
			 * @fn unsigned int getWidth()
			 * @brief gets width of drawing area
			 * @return width of drawing area
			 */
			unsigned int getWidth();

			/**
			 * @fn unsigned int getHeight()
			 * @brief gets height of drawing area
			 * @return height of drawing area
			 */
			unsigned int getHeight();

			/**
			 * @fn int getGraphWidth()
			 * @brief gets width of graph drawing area
			 * @return width
			 */
			int getGraphWidth();

			/**
			 * @fn int getGraphHeight()
			 * @brief gets height of graph drawing area
			 * @return height
			 */
			int getGraphHeight();

			/**
			 * @fn void setDisplayedRange(
					const double minX,
					const double maxX,
					const double minY,
					const double maxY
				)
			 * @brief sets range of data
			 * @brief minX minimum x of displayed range
			 * @brief maxX maximum x of displayed range
			 * @brief minY minimum y of displayed range
			 * @brief maxY maximum y of displayed range
			 */
			void setDisplayedRange(
				const double minX,
				const double maxX,
				const double minY,
				const double maxY
			);

			/**
			 * @fn void setMinX( const double minX )
			 * @brief sets minimum x of displayed range
			 * @param[in] minX minimum x of displayed range
			 */
			void setMinX( const double minX );

			/**
			 * @fn void setMaxX( const double maxX )
			 * @brief sets maximum x of displayed range
			 * @param[in] maxX maximum x of displayed range
			 */
			void setMaxX( const double maxX );

			/**
			 * @fn void setMinY( const double minY )
			 * @brief sets minimum y of displayed range
			 * @param[in] minY minimum y of displayed range
			 */
			void setMinY( const double minY );

			/**
			 * @fn void setMaxY( const double maxY )
			 * @brief sets maximum y of displayed range
			 * @param[in] maxY maximum y of displayed range
			 */
			void setMaxY( const double maxY );

			/**
			 * @fn double getMinX()
			 * @brief gets minimum x of displayed range
			 * @return minimum x of displayed range
			 */
			double getMinX();

			/**
			 * @fn double getMaxX()
			 * @brief gets maximum x of displayed range
			 * @return maximum x of displayed range
			 */
			double getMaxX();

			/**
			 * @fn double getMinY()
			 * @brief gets minimum y of displayed range
			 * @return minimum y of displayed range
			 */
			double getMinY();

			/**
			 * @fn double getMaxY()
			 * @brief gets maximum y of displayed range
			 * @return maximum y of displayed range
			 */
			double getMaxY();

			/**
			 * @fn void setScaleX( const double scX )
			 * @brief sets x coordinate scaling element
			 * @param[in] scX x coordinate scaling element
			 */
			void setScaleX( const double scX );

			/**
			 * @fn void setScaleY( const double scY )
			 * @brief sets y coordinate scaling element
			 * @param[in] scY y coordinate scaling element
			 */
			void setScaleY( const double scY );

			/**
			 * @fn void setScale( const double scX, const double scY )
			 * @brief sets scaling elements
			 * @param[in] scX x coordinate scaling element
			 * @param[in] scY y coordinate scaling element
			 */
			void setScale( const double scX, const double scY );

			/**
			 * @fn double getScaleX()
			 * @brief gets x coordinate scaling element
			 * @return x coordinate scaling element
			 */
			double getScaleX();

			/**
			 * @fn double getScaleY()
			 * @brief y coordinate scaling element
			 * @return y coordinate scaling element
			 */
			double getScaleY();

			/**
			 * @fn void setShearX( const double shX )
			 * @brief sets x coordinate shearing element
			 * @param[in] shX x coordinate shearing element
			 */
			void setShearX( const double shX );

			/**
			 * @fn void setShearY( const double shY )
			 * @brief sets y coordinate shearing element
			 * @param[in] shY y coordinate shearing element
			 */
			void setShearY( const double shY );

			/**
			 * @fn void setShear( const double shX, const double shY )
			 * @brief sets shearing elemnts
			 * @param[in] shX x coordinate shearing element
			 * @param[in] shY y coordinate shearing element
			 */
			void setShear( const double shX, const double shY );

			/**
			 * @fn double getShearX()
			 * @brief gets x coordinate shearing element
			 * @return x coordinate shearing element
			 */
			double getShearX();

			/**
			 * @fn double getShearY()
			 * @brief gets y coordinate shearing element
			 * @return y coordinate shearing element
			 */
			double getShearY();

			/**
			 * @fn void setTranslateX( const double trX )
			 * @brief sets x coordinate translation element
			 * @param[in] trX x coordinate translation element
			 */
			void setTranslateX( const double trX );

			/**
			 * @fn void setTranslateY( const double trY )
			 * @brief sets y coordinate translation element
			 * @param[in] trY y coordinate translation element
			 */
			void setTranslateY( const double trY );

			/**
			 * @fn void setTranslate( const double trX, const double trY )
			 * @brief sets translation elements
			 * @param[in] trX x coordinate translation element
			 * @param[in] trY y coordinate translation element
			 */
			void setTranslate( const double trX, const double trY );

			/**
			 * @fn double getTranslateX()
			 * @brief gets x coordinate translation element
			 * @return x coordinate translation element
			 */
			double getTranslateX();

			/**
			 * @fn double getTranslateY()
			 * @brief gets y coordinate translation element
			 * @return y coordinate translation element
			 */
			double getTranslateY();

			/**
			 * @fn void resetTransform()
			 * @brief resets transform matrix
			 */
			void resetTransform();

			/**
			 * @fn void transformDataToPosition(
					const double x,
					const double y,
					double* px,
					double* py
				)
			 * @brief transform data coordinate to position
			 * @param[in] x x data
			 * @param[in] y y data
			 * @param[out] px the pointer to store x coordinate value
			 * @param[out] py the pointer to store y coordinate value
			 */
			void transformDataToPosition(
				const double x,
				const double y,
				double* px,
				double* py
			);

			/**
			 * @fn void transformPositionToData(
					const double px,
					const double py,
					double* x,
					double* y
				)
			 * @brief transform position coordinate to data
			 * @param[in] px x coordinate
			 * @param[in] py y coordinate
			 * @param[out] x the pointer to store x data value
			 * @param[out] y the pointer to store y data value
			 */
			void transformPositionToData(
				const double px,
				const double py,
				double* x,
				double* y
			);

		protected:
			/**
			 * @fn void createMatrix()
			 * @brief creates transformation matrix
			 */
			void createMatrix();

		protected:
			/** draw style */
			DrawStyle m_drawStyle;
			/** fill pattern */
			FillPattern m_fillPattern;
			/** fill style */
			FillStyle m_fillStyle;
			/** horizontal align */
			TextHAlign m_hAlign;
			/** vertical align */
			TextVAlign m_vAlign;
			/** color */
			COLOR m_color;
			/** font */
			Font m_font;
			/** thickness line */
			int m_thickness;

			/** unused color */
			bool m_bUnusedColor;
			/** unused thickness */
			bool m_bUnusedThickness;
		public:
			/**
			 * @fn void setDrawStyle( const DrawStyle style )
			 * @brief sets draw style
			 * @param[in] style draw style
			 */
			void setDrawStyle( const DrawStyle style );

			/**
			 * @fn DrawStyle getDrawStyle()
			 * @brief gets draw style
			 * @return draw style
			 */
			DrawStyle getDrawStyle();

			/**
			 * @fn void setFillPattern( const FillPattern pattern )
			 * @brief sets fill pattern
			 * @param[in] pattern fill pattern
			 */
			void setFillPattern( const FillPattern pattern );

			/**
			 * @fn FillPattern getFillPattern()
			 * @brief gets fill pattern
			 * @return fill pattern
			 */
			FillPattern getFillPattern();

			/**
			 * @fn void setFillStyle( const FillStyle style )
			 * @brief sets fill style
			 * @param[in] style fill style
			 */
			void setFillStyle( const FillStyle style );

			/**
			 * @fn FillStyle getFillStyle()
			 * @brief gets fill style
			 * @return fill style
			 */
			FillStyle getFillStyle();

			/**
			 * @fn void setTextHAlign( const TextHAlign hAlign )
			 * @brief sets text horizontal align
			 * @param[in] hAlign text horizontal align
			 */
			void setTextHAlign( const TextHAlign hAlign );

			/**
			 * @fn TextHAlign getTextHAlign()
			 * @brief gets text horizontal align
			 * @return text horizontal align
			 */
			TextHAlign getTextHAlign();

			/**
			 * @fn void setTextVAlign( const TextVAlign vAlign )
			 * @brief sets text vertical align
			 * @param[in] vAlign text vertical align
			 */
			void setTextVAlign( const TextVAlign vAlign );

			/**
			 * @fn TextVAlign getTextVAlign()
			 * @brief gets text vertical align
			 * @return text vertical align
			 */
			TextVAlign getTextVAlign();

			/**
			 * @fn void setColor( const COLOR color )
			 * @brief sets color
			 * @param[in] color color
			 */
			void setColor( const COLOR color );

			/**
			 * @fn COLOR getColor()
			 * @brief gets color
			 * @return color
			 */
			COLOR getColor();

			/**
			 * @fn void setFont( Font& font )
			 * @brief sets font
			 * @param[in] font font information object
			 */
			void setFont( Font& font );

			/**
			 * @fn Font& getFont()
			 * @brief gets font
			 * @return font information object
			 */
			Font& getFont();

			/**
			 * @fn void setThicknessLine( int width )
			 * @brief sets thickness line
			 * @param[in] width width
			 */
			void setThicknessLine( int width );

			/**
			 * @fn int getThicknessLine()
			 * @brief gets thickness line
			 * @return width
			 */
			int getThicknessLine();

			/**
			 * @fn bool isUnusedDefaultColor()
			 * @brief is Unused default color
			 * @return If you want to use the default settings, this method returns true.
			 */
			bool isUnusedDefaultColor(){ return m_bUnusedColor; }

			/**
			 * @fn void setUnusedDefaultColor( const bool bSet )
			 * @brief set unused default color
			 * @param[in] bSet 
			 */
			void setUnusedDefaultColor( const bool bSet ){ m_bUnusedColor = bSet; }

			/**
			 * @fn bool isUnusedDefaultThickness()
			 * @brief is Unused default Thickness
			 * @return If you want to use the default settings, this method returns true.
			 */
			bool isUnusedDefaultThickness(){ return m_bUnusedThickness; }

			/**
			 * @fn void setUnusedDefaultThickness( const bool bSet )
			 * @brief set unused default thickness
			 * @param[in] bSet
			 */
			void setUnusedDefaultThickness( const bool bSet ){ m_bUnusedThickness = bSet; }
		public:
			/**
			 * @fn void drawLine(
					const int px0,
					const int py0,
					const int px1,
					const int py1
				)
			 * @brief draws line
			 * @param[in] px0 x-coordinate of first point
			 * @param[in] py0 y-coordinate of first point
			 * @param[in] px1 x-coordinate of second point
			 * @param[in] py1 y-coordinate of second point
			 */
			void drawLine(
				const int px0,
				const int py0,
				const int px1,
				const int py1
			);

			/**
			 * @fn void drawLines(
					int* const points,
					const unsigned int num
				)
			 * @brief draws line
			 * @param[in] points points array
			 * @param[in] num the number of points. (The size of points is 2 * num.)
			 */
			void drawLines(
				int* const points,
				const unsigned int num
			);

			/**
			 * @fn void fillRect(
					const int px,
					const int py,
					const int width,
					const int height
				)
			  * @brief fills rectangle
			  * @param[in] px x coordinate of rectangle position
			  * @param[in] py y coordinate of rectangle position
			  * @param[in] width width of rectangle
			  * @param[in] height height of rectangle
			  */
			void fillRect(
				const int px,
				const int py,
				const int width,
				const int height
			);

			/**
			 * @fn void fillPolygon(
					int* const points,
					const unsigned int num
				)
			 * @brief fills polygon
			 * @param[in] points the vertex of a polygon
			 * @param[in] num the number of vertices (The size of points is 2 * num.)
			 */
			void fillPolygon(
				int* const points,
				const unsigned int num
			);

			/**
			 * @fn void drawCircle(
						const int cx,
						const int cy,
						const int r,
						const bool fill
					)
			 * @brief draws circle
			 * @param[in] cx the x coordinate of the center
			 * @param[in] cy the y coordinate of the center
			 * @param[in] r radius
			 * @param[in] fill If true, the circle is filled.
			 */
			void drawCircle(
				const int cx,
				const int cy,
				const int r,
				const bool fill
			);

			/**
			 * @fn void drawArc(
						const int cx,
						const int cy,
						const int r,
						const double start,
						const double end,
						const bool fill
					)
			 * @brief draws arc of a circle
			 * @param[in] cx the x coordinate of the center
			 * @param[in] cy the y coordinate of the center
			 * @param[in] r radius
			 * @param[in] start the start of the arc (degree)
			 * @param[in] end the end of the arc (degree)
			 * @param[in] fill If true, the arc is filled.
			 */
			void drawArc(
				const int cx,
				const int cy,
				const int r,
				const double start,
				const double end,
				const bool fill
			);

			/**
			 * @fn void drawEllipse(
						const int px,
						const int py,
						const int width,
						const int height,
						const bool fill
					)
			 * @brief draws ellipse
			 * @param[in] px the x coordinate of the upper-left corner of the rectangle that contains
			 * @param[in] py the y coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] width the width of the rectangle that contains the ellipse
			 * @param[in] height the height of the rectangle that contains the ellipse
			 * @param[in] fill If true, the ellipse is filled.
			 */
			void drawEllipse(
				const int px,
				const int py,
				const int width,
				const int height,
				const bool fill
			);

			/**
			 * @fn void drawEllipticArc(
						const int px,
						const int py,
						const int width,
						const int height,
						const double start,
						const double end,
						const bool fill
					)		
			 * @brief draws elliptic arc
			 * @param[in] px the x coordinate of the upper-left corner of the rectangle that contains
			 * @param[in] py the y coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] width the width of the rectangle that contains the ellipse
			 * @param[in] height the height of the rectangle that contains the ellipse
			 * @param[in] start start of the arc (degree)
			 * @param[in] end end of the arc (degree)
			 * @param[in] fill If true, the elliptic arc is filled.
			 */
			void drawEllipticArc(
				const int px,
				const int py,
				const int width,
				const int height,
				const double start,
				const double end,
				const bool fill
			);

			/**
			 * @fn void drawText(
					const char* str,
					const int px,
					const int py,
					const int angle = 0
				)
			 * @brief draws text
			 * @param[in] str the string to be drawn
			 * @param[in] px x coordinate of position
			 * @param[in] py y coordinate of position
			 * @param[in] angle rotated text angle
			 */
			void drawText(
				const char* str,
				const int px,
				const int py,
				const int angle = 0
			);

			/**
			 * @fn void getTextSize( const char* str, int* width, int* height )
			 * @brief get text size
			 * @param[in] str text
			 * @param[out] width the pointer to store text width
			 * @param[out] height the pointer to store text height
			 */
			void getTextSize( const char* str, int* width, int* height );

			/**
			 * @fn void drawPoint( const int px, const int py )
			 * @brief draws point
			 * @param[in] px x coordinate of point
			 * @param[in] py y coordinate of point
			 */
			void drawPoint( const int px, const int py );

			/**
			 * @fn void drawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent )
			 * @brief draws bitmap 
			 * @param[in] bitmap bitmap data
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @param[in] transparent transparent flag
			 */
			void drawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent );


			/**
			 * @fn void drawMark(
						const Mark mark,
						const int px,
						const int py,
						const int r
					)
			 * @brief draws mark
			 * @param[in] mark mark shape
			 * @param[in] px x-coordinate of position to draw
			 * @param[in] py y-coordinate of position to draw
			 * @param[in] r mark size
			 */
			void drawMark(
				const Mark mark,
				const int px,
				const int py,
				const int r
			);

		public:
			/**
			 * @fn COLOR getPixel( const int px, const int py )
			 * @brief gets pixel color
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @return pixel color
			 */
			COLOR getPixel( const int px, const int py );

		public:
			/**
			 * @fn void flush();
			 * @brief flushes graphics
			 */
			void flush();

		protected:
			/** graph position */
			kome::core::Rect< int > m_graphPos;
			/** graph margin */
			kome::core::Rect< int > m_graphMargin;
			/** graphic width */
			unsigned int m_width;
			/** graphic height */
			unsigned int m_height;

			/** data range */
			struct {
				double minX;
				double maxX;
				double minY;
				double maxY;
			} m_range;

			/** coordinate transformation matrix */
			kome::core::Matrix m_ctr;

			/** transformation matrix [ (x, y) -> position ] */
			kome::core::Matrix m_tr;
			/** inverse transformation matrix [ position -> (x, y) ] */
			kome::core::Matrix m_trInv;

			/** transformation matrix is created */
			bool m_trFlg;

		protected:
			/**
			 * @fn virtual void onSetSize( const unsigned int width, const unsigned int height ) = 0
			 * @brief This method is called when the size of graphics is changed. (abstract method)
			 * @param[in] width graphics width
			 * @param[in] height graphics height
			 */
			virtual void onSetSize( const unsigned int width, const unsigned int height ) = 0;

			/**
			 * @fn virtual void onSetDrawStyle( const DrawStyle style ) = 0
			 * @brief This method is called by setDrawStyle method. (abstract method)
			 * @param[in] style draw style
			 */
			virtual void onSetDrawStyle( const DrawStyle style ) = 0;

			/**
			 * @fn virtual void onSetFillPattern( const FillPattern pattern ) = 0
			 * @brief This method is called by setFillPattern method. (abstract method)
			 * @param[in] pattern fill pattern
			 */
			virtual void onSetFillPattern( const FillPattern pattern ) = 0;

			/**
			 * @fn virtual void onSetFillStyle( const FillStyle style ) = 0
			 * @brief This method is called by setFillStyle method. (abstract method)
			 * @param[in] style fill style
			 */
			virtual void onSetFillStyle( const FillStyle style ) = 0;

			/**
			 * @fn virtual void onSetTextHAlign( const TextHAlign hAlign ) = 0
			 * @brief This method is called by setTextHAlign method. (abstract method)
			 * @param[in] hAlign text horizontal align
			 */
			virtual void onSetTextHAlign( const TextHAlign hAlign ) = 0;

			/**
			 * @fn virtual void onSetTextVAlign( const TextVAlign vAlign ) = 0
			 * @brief This method is called by setTextVAlign method. (abstract method)
			 * @param[in] vAlign text vertical align
			 */
			virtual void onSetTextVAlign( const TextVAlign vAlign ) = 0;

			/**
			 * @fn virtual void onSetColor( const COLOR color ) = 0
			 * @brief this method is called by setColor method. (abstract method)
			 * @param[in] color color
			 */
			virtual void onSetColor( const COLOR color ) = 0;

			/**
			 * @fn virtual void onSetFont( Font& font ) = 0
			 * @brief This method is called by setFont method. (abstract method)
			 * @param[in] font font information object
			 */
			virtual void onSetFont( Font& font ) = 0;

			/**
			 * @fn virtual void onSetPen( unsigned int width ) = 0
			 * @brief This method is called by setPen method. (abstract method)
			 * @param[in] width width
			 */
			virtual void onSetPen( unsigned int width ) = 0;

			/**
			 * @fn virtual void onDrawLines(
					int* const points,
					unsigned int num
				) = 0
			 * @brief This mtdhod is called by drawLines method. (abstract method)
			 * @param[in] points points array
			 * @param[in] num the number of points. (The size of points is 2 * num.)
			 */
			virtual void onDrawLines(
				int* const points,
				const unsigned int num
			) = 0;

			/**
			 * @fn virtual void onFillPolygon(
					int* const points,
					const unsigned int num
				) = 0
			 * @brief This method is called by fillPolygon or fillRect. (abstract method)
			 * @param[in] points the vertex of a polygon
			 * @param[in] num the number of vertices (The size of points is 2 * num.)
			 */
			virtual void onFillPolygon(
				int* const points,
				const unsigned int num
			) = 0;

			/**
			 * @fn virtual void onDrawEllipticArc(
						const int px,
						const int py,
						const int width,
						const int height,
						const double start,
						const double end,
						const bool fill
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
				const int px,
				const int py,
				const int width,
				const int height,
				const double start,
				const double end,
				const bool fill
			) = 0;

			/**
			 * @fn virtual void onDrawText(
					const char* str,
					const int px,
					const int py,
					const int angle
				) = 0
			 * @brief This method is called by drawText method. (abstract method)
			 * @param[in] str the string to be drawn
			 * @param[in] px x coordinate of position
			 * @param[in] py y coordinate of position
			 * @param[in] angle rotated text angle
			 */
			virtual void onDrawText(
				const char* str,
				const int px,
				const int py,
				const int angle
			) = 0;

			/**
			 * @fn virtual void onGetTextSize(
					const char* str,
					int* width,
					int* height
				) = 0
			 * @brief This method is called by getTextSize method. (abstract method)
			 * @param[in] str the string
			 * @param[out] width the pointer to store text width
			 * @param[out] height the pointer to store text height
			 */
			virtual void onGetTextSize(
				const char* str,
				int* width,
				int* height
			) = 0;

			/**
			 * @fn virtual void onDrawPoint( const int px, const int py ) = 0
			 * @brief This method is called by drawPoint method. (abstract method)
			 * @param[in] px x coordinate of point
			 * @param[in] py y coordinate of point
			 */
			virtual void onDrawPoint( const int px, const int py ) = 0;

			/**
			 * @fn virtual void onDrawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent ) = 0
			 * @brief This method is called by drawBitmap method (abstract method)
			 * @param[in] bitmap bitmap data
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @param[in] transparent transparent flag
			 */
			virtual void onDrawBitmap( BitmapData& bitmap, const int px, const int py, const bool transparent ) = 0;

			/**
			 * @fn virtual COLOR onGetPixel( const int px, const int py ) = 0
			 * @brief This method is called by getPixel method (abstract method)
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @return pixel color
			 */
			virtual COLOR onGetPixel( const int px, const int py ) = 0;

			/**
			 * @fn virtual void onFlush() = 0
			 * @brief This method is called by flush method. (abstract method)
			 */
			virtual void onFlush() = 0;
		};
	}
}

#endif	//  __KOME_IMG_GRAPHICS_H__
