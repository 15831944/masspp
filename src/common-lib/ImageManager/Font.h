/**
 * @file Font.h
 * @brief interfaces of Font class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_FONT_H__
#define __KOME_IMG_FONT_H__


#include <string>

#include "ColorTool.h"


namespace kome {
	namespace img {
		/**
		 * @class Font
		 * @brief font information management class
		 */
		class IMAGE_MANAGER_CLASS Font {
		public:
			/**
			 * @typedef Family
			 * @brief font family
			 */
			typedef enum {
				FAMILY_SERIF,
				FAMILY_SANS_SERIF,
				FAMILY_CURSIVE,
				FAMILY_FANTASY,
				FAMILY_MONOSPACE
			} Family;

			/**
			 * @typedef Style
			 * @brief font style
			 */
			typedef enum {
				STYLE_NORMAL,
				STYLE_SLANT,
				STYLE_ITALIC
			} Style;

			/**
			 * @typedef Weight
			 * @brief font weight
			 */
			typedef enum {
				WEIGHT_LIGHT,
				WEIGHT_BOLD
			} Weight;

		public:
			/**
			 * @fn Font(
					const int size,
					const Family family = FAMILY_SERIF,
					const Style style = STYLE_NORMAL,
					const Weight weight = WEIGHT_LIGHT,
					const char* face = NULL,
					const bool underline = false,
					const bool strikethrough = false
				)
			 * @brief constructor
			 * @param[in] size font size
			 * @param[in] family font family
			 * @param[in] style font style
			 * @param[in] weight font weight
			 * @param[in] face font face name
			 * @param[in] underline under-line flag
			 * @param[in] strikethrough strike-through flag
			 */
			Font(
				const int size,
				const Family family = FAMILY_SERIF,
				const Style style = STYLE_NORMAL,
				const Weight weight = WEIGHT_LIGHT,
				const char* face = NULL,
				const bool underline = false,
				const bool strikethrough = false
			);

			/**
			 * @fn virtual ~Font()
			 * @brief destructor
			 */
			virtual ~Font();

		protected:
			/** font size */
			int m_size;

			/** font family */
			Family m_family;

			/** font style */
			Style m_style;

			/** font weight */
			Weight m_weight;

			/** font face name */
			std::string m_face;

			/** underline flag */
			bool m_underline;

			/** strike-through flag */
			bool m_strikeThrough;

		public:
			/**
			 * @fn void setSize( const int size )
			 * @brief sets font size
			 * @param[in] size font size 
			 */
			void setSize( const int size );

			/**
			 * @fn int getSize()
			 * @brief gets font size
			 * @return font size
			 */
			int getSize();

			/**
			 * @fn void setFamily( const Family family )
			 * @brief sets font family
			 * @param[in] family font family
			 */
			void setFamily( const Family family );

			/**
			 * @fn Family getFamily()
			 * @brief gets font family
			 * @return font family
			 */
			Family getFamily();

			/**
			 * @fn void setStyle( const Style style )
			 * @brief sets font style
			 * @param[in] style font style
			 */
			void setStyle( const Style style );

			/**
			 * @fn Style getStyle()
			 * @brief gets font style
			 * @return font style
			 */
			Style getStyle();

			/**
			 * @fn void setWeight( const Weight weight )
			 * @brief sets font weight
			 * @param[in] weight font weight
			 */
			void setWeight( const Weight weight );

			/**
			 * @fn Weight getWeight()
			 * @brief gets font weight 
			 * @return font weight
			 */			 
			Weight getWeight();

			/**
			 * @fn void setFace( const char* face )
			 * @brief sets font face name
			 * @param[in] face font face name
			 */
			void setFace( const char* face );

			/**
			 * @fn const char* getFace()
			 * @brief gets font face name
			 * @return font face name
			 */
			const char* getFace();

			/**
			 * @fn void setUnderline( const bool underline )
			 * @brief sets underline flag
			 * @param[in] underline underline flag
			 */
			void setUnderline( const bool underline );

			/**
			 * @fn bool hasUnderline()
			 * @brief judges this font has underline or not.
			 * @return If true, this font has underline.
			 */			
			bool hasUnderline();

			/**
			 * @fn void setStrikeThrough( const bool strikethrough )
			 * @brief sets strike-through flag
			 * @param[in] strikethrough strike-through flag
			 */
			void setStrikeThrough( const bool strikethrough );

			/**
			 * @fn bool hasStrikeThrough()
			 * @brief judges this font has strike-through or not.
			 * @return If true, this font has strike-through.
			 */
			bool hasStrikeThrough();
		};
	}
}

#endif	// __KOME_IMG_FONT_H__
