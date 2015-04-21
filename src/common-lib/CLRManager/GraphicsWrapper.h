/**
 * @file GraphicsWrapper.h
 * @brief interfaces of GraphicsWrapper class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_GRAPHICS_WRAPPER_H__
#define __KOME_CLR_GRAPHICS_WRAPPER_H__

namespace kome {
	namespace clr {

		ref class FontWrapper;
		ref class BitmapDataWrapper;

		/**
		 * @class GraphicsWrapper
		 * @brief Graphics object wrapper to use on CLR environment
		 */
		public ref class GraphicsWrapper {
		public:
			/**
			 * @fn GraphicsWrapper( kome::img::Graphics& g )
			 * @brief constructor
			 * @param[in] g Graphics object
			 */
			GraphicsWrapper( kome::img::Graphics& g );

			/**
			 * @fn virtual ~GraphicsWrapper()
			 * @brief destructor
			 */
			virtual ~GraphicsWrapper();

		protected:
			/** Graphics object */
			kome::img::Graphics& m_graphics;

		public:
			/**
			 * @fn kome::img::Graphics& getGraphics()
			 * @brief gets Graphics object
			 * @return the pointer to Graphics object
			 */
			kome::img::Graphics& getGraphics();

		public:
			/** definition of drawing style */
			ENUM_CLASS DrawStyle {
				SOLID,		 // "_______________________________"
				DOT,		   // "..............................."
				LONG_DASH,	 // "___ ___ ___ ___ ___ ___ ___ ___"
				SHORT_DASH,	// "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ "
				DOT_DASH	   // "__ . __ . __ . __ . __ . __ . __"
			};

			/** definition of filling pattern */
			ENUM_CLASS FillPattern {
				SOLID,		 //
				BDIAGONAL,	 // "////////////////////////////////"
				FDIAGONAL,	 // "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
				CROSS,		 // "++++++++++++++++++++++++++++++++"
				CROSS_DIAG,	// "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
				HORIZONTAL,	// "================================"
				VERTICAL,	  // "||||||||||||||||||||||||||||||||"
			};

			/** definition of filling style */
			ENUM_CLASS FillStyle {
				ODDEVEN,
				WINDING
			};

			/** definition of text horizontal alignment */
			ENUM_CLASS TextHAlign {
				LEFT,
				CENTER,
				RIGHT
			};

			/** definition of text vertical alignment */
			ENUM_CLASS TextVAlign {
				TOP,
				MIDDLE,
				BOTTOM
			};

			/** definition of marks */
			ENUM_CLASS Mark {
				NONE,
				CIRCLE,
				FILLED_CIRCLE,
				DELTA,
				FILLED_DELTA,
				NABLA,
				FILLED_NABLA,
				SQUARE,
				FILLED_SQUARE,
				DIAMOND,
				FILLED_DIAMOND,
				STAR,
				FILLED_STAR,
				PLUS,
				TIMES
			};

		public:
			/**
			 * @fn void setGraphPosition(
					int left,
					int top,
					int right,
					int bottom
				)
			 * @brief sets the position of the graph
			 * @param[in] left x coordinate in left end of graph
			 * @param[in] top y coordinate in top end of graph
			 * @param[in] right x coordinate in right end of graph
			 * @param[in] bottom y coordinate in bottom end of graph
			 */
			void setGraphPosition(
				int left,
				int top,
				int right,
				int bottom
			);

			/**
			 * @fn void setGraphPositionLeft( int left )
			 * @brief sets x coordinate in left end of graph
			 * @param[in] left x coordinate in left end of graph
			 */
			void setGraphPositionLeft( int left );

			/**
			 * @fn void setGraphPositionTop( int top )
			 * @brief sets y coordinate in top end of graph
			 * @param[in] top y coordinate in top end of graph
			 */
			void setGraphPositionTop( int top );

			/**
			 * @fn void setGraphPositionRight( int right )
			 * @brief sets x coordinate in right end of graph
			 * @param[in] right y coordinate in right end of graph
			 */
			void setGraphPositionRight( int right );

			/**
			 * @fn void setGraphPositionBottom( int bottom )
			 * @brief sets y coordinate in bottom end of graph
			 * @param[in] bottom y coordinate in bottom end of graph
			 */
			void setGraphPositionBottom( int bottom );

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
					int left,
					int top,
					int right,
					int bottom
				)
			 * @brief sets graph margin information
			 * @param[in] left length of margin left in graph
			 * @param[in] top length of margin top in graph
			 * @param[in] right length of margin right in graph
			 * @param[in] bottom length of margin bottom in graph
			 */
			void setGraphMargin(
				int left,
				int top,
				int right,
				int bottom
			);

			/**
			 * @fn void setGraphMarginLeft( int left )
			 * @brief sets length of margin left in graph
			 * @param[in] left length of margin left in graph
			 */
			void setGraphMarginLeft( int left );

			/**
			 * @fn void setGraphMarginTop( int top )
			 * @brief sets length of margin top in graph
			 * @param[in] top length of margin top in graph
			 */
			void setGraphMarginTop( int top );

			/**
			 * @fn void setGraphMarginRight( int right )
			 * @brief sets length of margin right in graph
			 * @param[in] right length of margin right in graph
			 */
			void setGraphMarginRight( int right );

			/**
			 * @fn void setGraphMarginBottom( int bottom )
			 * @brief sets length of margin bottom in graph
			 * @param[in] bottom length of margin bottom in graph
			 */
			void setGraphMarginBottom( int bottom );

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
			 * @fn void setSize( unsigned int width, unsigned int height )
			 * @brief sets size of drawing area
			 * @param[in] width width of drawing area
			 * @param[in] height height of drawing area
			 */
			void setSize( unsigned int width, unsigned int height );

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
					double minX,
					double maxX,
					double minY,
					double maxY
				)
			 * @brief sets range of data
			 * @brief minX minimum x of displayed range
			 * @brief maxX maximum x of displayed range
			 * @brief minY minimum y of displayed range
			 * @brief maxY maximum y of displayed range
			 */
			void setDisplayedRange(
				double minX,
				double maxX,
				double minY,
				double maxY
			);

			/**
			 * @fn void setMinX( double minX )
			 * @brief sets minimum x of displayed range
			 * @param[in] minX minimum x of displayed range
			 */
			void setMinX( double minX );

			/**
			 * @fn void setMaxX( double maxX )
			 * @brief sets maximum x of displayed range
			 * @param[in] maxX maximum x of displayed range
			 */
			void setMaxX( double maxX );

			/**
			 * @fn void setMinY( double minY )
			 * @brief sets minimum y of displayed range
			 * @param[in] minY minimum y of displayed range
			 */
			void setMinY( double minY );

			/**
			 * @fn void setMaxY( double maxY )
			 * @brief sets maximum y of displayed range
			 * @param[in] maxY maximum y of displayed range
			 */
			void setMaxY( double maxY );

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
			 * @fn void setScaleX( double scX )
			 * @brief sets x coordinate scaling element
			 * @param[in] scX x coordinate scaling element
			 */
			void setScaleX( double scX );

			/**
			 * @fn void setScaleY( double scY )
			 * @brief sets y coordinate scaling element
			 * @param[in] scY y coordinate scaling element
			 */
			void setScaleY( double scY );

			/**
			 * @fn void setScale( double scX, double scY )
			 * @brief sets scaling elements
			 * @param[in] scX x coordinate scaling element
			 * @param[in] scY y coordinate scaling element
			 */
			void setScale( double scX, double scY );

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
			 * @fn void setShearX( double shX )
			 * @brief sets x coordinate shearing element
			 * @param[in] shX x coordinate shearing element
			 */
			void setShearX( double shX );

			/**
			 * @fn void setShearY( double shY )
			 * @brief sets y coordinate shearing element
			 * @param[in] shY y coordinate shearing element
			 */
			void setShearY( double shY );

			/**
			 * @fn void setShear( double shX, double shY )
			 * @brief sets shearing elemnts
			 * @param[in] shX x coordinate shearing element
			 * @param[in] shY y coordinate shearing element
			 */
			void setShear( double shX, double shY );

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
			 * @fn void setTranslateX( double trX )
			 * @brief sets x coordinate translation element
			 * @param[in] trX x coordinate translation element
			 */
			void setTranslateX( double trX );

			/**
			 * @fn void setTranslateY( double trY )
			 * @brief sets y coordinate translation element
			 * @param[in] trY y coordinate translation element
			 */
			void setTranslateY( double trY );

			/**
			 * @fn void setTranslate( double trX, double trY )
			 * @brief sets translation elements
			 * @param[in] trX x coordinate translation element
			 * @param[in] trY y coordinate translation element
			 */
			void setTranslate( double trX, double trY );

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
					double x,
					double y,
					double% px,
					double% py
				)
			 * @brief transform data coordinate to position
			 * @param[in] x x data
			 * @param[in] y y data
			 * @param[out] px the pointer to store x coordinate value
			 * @param[out] py the pointer to store y coordinate value
			 */
			void transformDataToPosition(
				double x,
				double y,
				double% px,
				double% py
			);

			/**
			 * @fn void transformPositionToData(
					double px,
					double py,
					double% x,
					double% y
				)
			 * @brief transform position coordinate to data
			 * @param[in] px x coordinate
			 * @param[in] py y coordinate
			 * @param[out] x the pointer to store x data value
			 * @param[out] y the pointer to store y data value
			 */
			void transformPositionToData(
				double px,
				double py,
				double% x,
				double% y
			);

		public:
			/**
			 * @fn void setDrawStyle( DrawStyle style )
			 * @brief sets draw style
			 * @param[in] style draw style
			 */
			void setDrawStyle( DrawStyle style );

			/**
			 * @fn DrawStyle getDrawStyle()
			 * @brief gets draw style
			 * @return draw style
			 */
			DrawStyle getDrawStyle();

			/**
			 * @fn void setFillPattern( FillPattern pattern )
			 * @brief sets fill pattern
			 * @param[in] pattern fill pattern
			 */
			void setFillPattern( FillPattern pattern );

			/**
			 * @fn FillPattern getFillPattern()
			 * @brief gets fill pattern
			 * @return fill pattern
			 */
			FillPattern getFillPattern();

			/**
			 * @fn void setFillStyle( FillStyle style )
			 * @brief sets fill style
			 * @param[in] style fill style
			 */
			void setFillStyle( FillStyle style );

			/**
			 * @fn FillStyle getFillStyle()
			 * @brief gets fill style
			 * @return fill style
			 */
			FillStyle getFillStyle();

			/**
			 * @fn void setTextHAlign( TextHAlign hAlign )
			 * @brief sets text horizontal align
			 * @param[in] hAlign text horizontal align
			 */
			void setTextHAlign( TextHAlign hAlign );

			/**
			 * @fn TextHAlign getTextHAlign()
			 * @brief gets text horizontal align
			 * @return text horizontal align
			 */
			TextHAlign getTextHAlign();

			/**
			 * @fn void setTextVAlign( TextVAlign vAlign )
			 * @brief sets text vertical align
			 * @param[in] vAlign text vertical align
			 */
			void setTextVAlign( TextVAlign vAlign );

			/**
			 * @fn TextVAlign getTextVAlign()
			 * @brief gets text vertical align
			 * @return text vertical align
			 */
			TextVAlign getTextVAlign();

			/**
			 * @fn void setColor( System::Drawing::Color color )
			 * @brief sets color
			 * @param[in] color color
			 */
			void setColor( System::Drawing::Color color );

			/**
			 * @fn System::Drawing::Color getColor()
			 * @brief gets color
			 * @return color
			 */
			System::Drawing::Color getColor();

			/**
			 * @fn void setFont( FontWrapper^ font )
			 * @brief sets font
			 * @param[in] font font information object
			 */
			void setFont( FontWrapper^ font );

			/**
			 * @fn FontWrapper^ getFont()
			 * @brief gets font
			 * @return font information object
			 */
			FontWrapper^ getFont();

			/**
			 * @fn void setThicknessLine( int width )
			 * @brief sets thickness line
			 * @param[in] width
			 */
			void setThicknessLine( int width );

			/**
			 * @fn int getThicknessLine()
			 * @brief gets thickness line
			 * @return thickness
			 */
			int getThicknessLine();
		public:
			/**
			 * @fn void drawLine( int px0, int py0, int px1, int py1 )
			 * @brief draws line
			 * @param[in] px0 x-coordinate of first point
			 * @param[in] py0 y-coordinate of first point
			 * @param[in] px1 x-coordinate of second point
			 * @param[in] py1 y-coordinate of second point
			 */
			void drawLine( int px0, int py0, int px1, int py1 );

			/**
			 * @fn void drawLines(
					cli::array< int, 2 >^ points,
				)
			 * @brief draws line
			 * @param[in] points the array of points
			 */
			void drawLines(
				cli::array< int, 2 >^ points
			);

			/**
			 * @fn void fillRect(
					int px,
					int py,
					int width,
					int height
				)
			 * @brief fills rectangle
			 * @param[in] px x coordinate of rectangle position
			 * @param[in] py y coordinate of rectangle position
			 * @param[in] width width of rectangle
			 * @param[in] height height of rectangle
			 */
			void fillRect(
				int px,
				int py,
				int width,
				int height
			);

			/**
			 * @fn void fillPolygon(
					cli::array< int, 2 >^ points
				)
			 * @brief fills polygon
			 * @param[in] points the vertex of a polygon
			 */
			void fillPolygon(
				cli::array< int, 2 >^ points
			);

			/**
			 * @fn void drawCircle(
						int cx,
						int cy,
						int r,
						bool fill
					)
			 * @brief draws circle
			 * @param[in] cx the x coordinate of the center
			 * @param[in] cy the y coordinate of the center
			 * @param[in] r radius
			 * @param[in] fill If true, the circle is filled.
			 */
			void drawCircle(
				int cx,
				int cy,
				int r,
				bool fill
			);

			/**
			 * @fn void drawArc(
						int cx,
						int cy,
						int r,
						double start,
						double end,
						bool fill
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
				int cx,
				int cy,
				int r,
				double start,
				double end,
				bool fill
			);

			/**
			 * @fn void drawEllipse(
						int px,
						int py,
						int width,
						int height,
						bool fill
					)
			 * @brief draws ellipse
			 * @param[in] px the x coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] py the y coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] width the width of the rectangle that contains the ellipse
			 * @param[in] height the height of the rectangle that contains the ellipse
			 * @param[in] fill If true, the ellipse is filled.
			 */
			void drawEllipse(
				int px,
				int py,
				int width,
				int height,
				bool fill
			);

			/**
			 * @fn void drawEllipticArc(
						int px,
						int py,
						int width,
						int height,
						double start,
						double end,
						bool fill
					)		
			 * @brief draws elliptic arc
			 * @param[in] px the x coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] py the y coordinate of upper-left corner of the rectangle that contains the ellipse
			 * @param[in] width the width of the rectangle that contains the ellipse
			 * @param[in] height the height of the rectangle that contains the ellipse
			 * @param[in] start start of the arc (degree)
			 * @param[in] end end of the arc (degree)
			 * @param[in] fill If true, the elliptic arc is filled.
			 */
			void drawEllipticArc(
				int px,
				int py,
				int width,
				int height,
				double start,
				double end,
				bool fill
			);

			/**
			 * @fn void drawText(
					System::String^ str,
					int px,
					int py,
					int angle
				)
			 * @brief draws text
			 * @param[in] str the string to be drawn
			 * @param[in] px x coordinate of position
			 * @param[in] py y coordinate of position
			 * @param[in] angle rotated text angle
			 */
			void drawText(
				System::String^ str,
				int px,
				int py,
				int angle
			);

			/**
			 * @fn void getTextSize( System::String^ str, int% width, int% height )
			 * @brief gets text size
			 * @param[in] str text
			 * @param[out] width the pointer to store text width
			 * @param[out] height the pointer to store text height
			 */
			void getTextSize( System::String^ str, int% width, int% height );

			/**
			 * @fn void drawPoint( int px, int py )
			 * @brief draws point
			 * @param[in] px x coordinate of point
			 * @param[in] py y coordinate of point
			 */
			void drawPoint( int px, int py );

			/**
			 * @fn void drawBitmap( BitmapDataWrapper^ bitmap, int px, int py, bool transparent )
			 * @brief draws bitmap data
			 * @param[in] bitmap bitmap data
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @param[in] transparent transparent flag
			 */
			void drawBitmap( BitmapDataWrapper^ bitmap, int px, int py, bool transparent );

			/**
			 * @fn void drawMark(
						Mark mark,
						int px,
						int py,
						int r
					)
			 * @brief draws mark
			 * @param[in] mark mark shape
			 * @param[in] px x-coordinate of position to draw
			 * @param[in] py y-coordinate of position to draw
			 * @param[in] r mark size
			 */
			void drawMark(
				Mark mark,
				int px,
				int py,
				int r
			);

		public:
			/**
			 * @fn System::Drawing::Color getPixel( int px, int py )
			 * @brief gets pixel color
			 * @param[in] px x coordinate of bitmap position
			 * @param[in] py y coordinate of bitmap position
			 * @return pixel color
			 */
			System::Drawing::Color getPixel( int px, int py );

		public:
			/**
			 * @fn void flush()
			 * @brief flushes graphics
			 */
			void flush();
		};
	}
}


#endif	// __KOME_CLR_GRAPHICS_WRAPPER_H__
