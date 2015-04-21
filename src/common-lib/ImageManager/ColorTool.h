/**
 * @file ColorTool.h
 * @brief interfaces of ColorTool class
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_COLOR_TOOL_H__
#define __KOME_IMG_COLOR_TOOL_H__


/**
 * @typedef COLOR
 * @brief type definition of color.
 */
typedef unsigned long COLOR;


namespace kome {
	namespace img {

		/**
		 * @class ColorTool
		 * @brief this class has functions which deals with color.
		 */
		class IMAGE_MANAGER_CLASS ColorTool {
		public:
			/** black */
			static const COLOR BLACK;
			/** silver */
			static const COLOR SILVER;
			/** gray */
			static const COLOR GRAY;
			/** white */
			static const COLOR WHITE;
			/** maroon */
			static const COLOR MAROON;
			/** red */
			static const COLOR RED;
			/** purple */
			static const COLOR PURPLE;
			/** fuchsia */
			static const COLOR FUCHSIA;
			/** green */
			static const COLOR GREEN;
			/** lime */
			static const COLOR LIME;
			/** olive */
			static const COLOR OLIVE;
			/** yellow */
			static const COLOR YELLOW;
			/** navy */
			static const COLOR NAVY;
			/** blue */
			static const COLOR BLUE;
			/** teal */
			static const COLOR TEAL;
			/** aqua */
			static const COLOR AQUA;

			/**
			 * @fn static unsigned char getRed( COLOR color )
			 * @brief gets Red value of color
			 * @return Red value
			 */
			static unsigned char getRed( COLOR color );

			/**
			 * @fn static unsigned char getGreen( COLOR color )
			 * @brief gets Green value of color
			 * @return Green value
			 */
			static unsigned char getGreen( COLOR color );

			/**
			 * @fn static unsigned char getBlue( COLOR color )
			 * @brief gets Blue value of color
			 * @return Blue value
			 */
			static unsigned char getBlue( COLOR color );

			/**
			 * @fn static COLOR getColor( unsigned char r, unsigned char g, unsigned char b )
			 * @brief gets COLOR data
			 * @param[in] r Red value
			 * @param[in] g Green value
			 * @param[in] b Blue value
			 * @return COLOR data
			 */
			static COLOR getColor( unsigned char r, unsigned char g, unsigned char b );

			/**
			 * @fn static COLOR getColor( const char* s )
			 * @brief gets COLOR data from character string
			 * @param[in] s character string (ex. "ff0000", "#ff0000", "red" )
			 * @return COLOR data
			 */
			static COLOR getColor( const char* s );

			/**
			 * @fn static std::string getString( COLOR color )
			 * @brief gets string meanings specified color
			 * @return character string
			 */
			static std::string getString( COLOR color );

			/**
			 * @fn static COLOR getAdditiveColor( const COLOR c0, const COLOR c1, const double w0 = 1.0, const double w1 = 1.0 )
			 * @brief gets additive color
			 * @param[in] c0 color element of additive color
			 * @param[in] c1 color element of additive color
			 * @param[in] w0 weight of c0
			 * @param[in] w1 weight of c1
			 */
			static COLOR getAdditiveColor( const COLOR c0, const COLOR c1, const double w0 = 1.0, const double w1 = 1.0 );
		};
	}
}

#endif	//  __KOME_IMG_COLOR_TOOL_H__
