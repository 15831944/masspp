/**
 * @file ColorTheme.h
 * @brief interfaces of ColorTheme class
 *
 * @author S.Tanaka
 * @date 2007.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_COLOR_THEME_H__
#define __KOME_VIEW_COLOR_THEME_H__


namespace kome {
	namespace view {

		/**
		 * @class ColorTheme
		 * @brief color map theme class
		 */
		class ColorTheme {
		public:
			/**
			 * @fn ColorTheme( const char* name )
			 * @brief constructor
			 * @param name theme name
			 */
			ColorTheme( const char* name );

			/**
			 * @fn virtual ~ColorTheme()
			 * @brief destructor
			 */
			virtual ~ColorTheme();

		protected:
			/** name */
			std::string m_name;

			/** zero middle flag */
			bool m_zeroMiddle;

			/** color array */
			std::vector< std::pair< double, COLOR > > m_colors;

			/** high width */
			double m_highWidth;

			/** width */
			double m_width;

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets theme name
			 * @param name theme name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets theme name
			 * @return theme name
			 */
			const char* getName();

			/**
			 * @fn void setZeroMiddle( const bool flg )
			 * @brief sets zero middle flag
			 * @param flg zero middle flag value
			 */
			void setZeroMiddle( const bool flg );

			/**
			 * @fn bool getZeroMiddle()
			 * @brief gets zero middle flag
			 * @return zero middle
			 */
			bool getZeroMiddle();

			/**
			 * @fn void setHighWidth( const double width )
			 * @brief sets high width
			 * @param width high width
			 */
			void setHighWidth( const double width );

			/**
			 * @fn double getHighWidth()
			 * @brief gets high width
			 * @return high width
			 */
			double getHighWidth();

			/**
			 * @fn void addColor( const double width, const COLOR color )
			 * @brief adds color
			 * @param width width
			 * @param color color
			 */
			void addColor( const double width, const COLOR color );

			/**
			 * @fn COLOR getColor( const unsigned int idx )
			 * @brief gets color
			 * @param idx color index
			 * @return color
			 */
			COLOR getColor( const unsigned int idx );

			/**
			 * @fn double getWidth( const unsigned int idx )
			 * @brief gets color width
			 * @param idx color index
			 * @return color width
			 */
			double getWidth( const unsigned int idx );

		public:
			/**
			 * @fn double getMinIntensity( kome::objects::DataMapInfo& dataMap )
			 * @brief gets minimum intensity
			 * @param dataMap data map information
			 * @return minimum intensity
			 */
			double getMinIntensity( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn double getMaxIntensity( kome::objects::DataMapInfo& dataMap )
			 * @brief gets maximum intensity
			 * @param dataMap data map information
			 * @return maximum intensity
			 */
			double getMaxIntensity( kome::objects::DataMapInfo& dataMap );

		public:
			/**
			 * @fn COLOR getColor( const double intensity, const double minInt, const double maxInt )
			 * @brief gets data color
			 * @param intensity intenisty
			 * @param minInt minimum intensity
			 * @param maxInt maximum intensity
			 * @return data color
			 */
			COLOR getColor( const double intensity, const double minInt, const double maxInt );
		};
	}
}

#endif		// __KOME_VIEW_COLOR_THEME_H__
