/**
 * @file FontTool.h
 * @brief interfaces of FontTool class
 *
 * @author S.Tanaka
 * @date 2013.04.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_FONT_TOOL_H__
#define __KOME_WINDOW_FONT_TOOL_H__


class wxListCtrl;


#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class FontTool
		 * @brief This class has some functions for list
		 */
		class WINDOW_MANAGER_CLASS FontTool {
		public:
			/**
			 * @fn static wxFont convertFont( kome::img::Font& font )
			 * @brief converts font object
			 * @param[in] font font object
			 * @return converted font object
			 */
			static wxFont convertFont( kome::img::Font& font );

			/**
			 * @fn static kome::img::Font convertFont( wxFont& font )
			 * @brief converts font object
			 * @param[in] font font object
			 * @return converted font object
			 */
			static kome::img::Font convertFont( wxFont& font );
			
			/**
			 * @fn static kome::img::Font convertFontFromString( const char* fontInfo )
			 * @brief converts font from string
			 * @param[in] fontInfo font info
			 */
			static kome::img::Font convertFontFromString( const char* fontInfo );

			/**
			 * @fn static kome::img::Font getFontFromIni( const char* section, const char* key )
			 * @brief gets font from ini file
			 * @param[in] section ini file section
			 * @param[in] key ini file key
			 * @return font
			 */
			static kome::img::Font getFontFromIni( const char* section, const char* key );

		};
	}
}


#endif		// __KOME_WINDOW_FONT_TOOL_H__
