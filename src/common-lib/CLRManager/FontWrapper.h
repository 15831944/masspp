/**
 * @file FontWrapper.h
 * @brief interfaces of FontWrapper class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_FONT_WRAPPER_H__
#define __KOME_CLR_FONT_WRAPPER_H__

namespace kome {
	namespace clr {

		/**
		 * @class FontWrapper
		 * @brief Font object wrapper class to use on CLR environment
		 */
		public ref class FontWrapper {
		public:
			/** font family */
			ENUM_CLASS Family {
				SERIF,
				SANS_SERIF,
				CURSIVE,
				FANTASY,
				MONOSPACE
			};

			/** font style */
			ENUM_CLASS Style {
				NORMAL,
				SLANT,
				ITALIC
			};

			/** font weight */
			ENUM_CLASS Weight {
				LIGHT,
				BOLD
			};

		public:
			/**
			 * @fn FontWrapper( kome::img::Font& font )
			 * @brief constructor
			 * @param[in] font
			*/
			FontWrapper( kome::img::Font& font );

			/**
			* @fn virtual ~FontWrapper()
			* @brief destructor
			*/
			virtual ~FontWrapper();

		public:
			/**
			 * @fn kome::img::Font& getFont()
			 * @brief gets font object
			 * @return font object
			 */
			kome::img::Font& getFont();

		public:
			/**
			 * @fn void setSize( int size )
			 * @brief sets font size
			 * @param[in] size font size 
			 */
			void setSize( int size );

			/**
			 * @fn int getSize()
			 * @brief gets font size
			 * @return font size
			 */
			int getSize();

			/**
			 * @fn void setFamily( Family family )
			 * @brief sets font family
			 * @param[in] family font family
			 */
			void setFamily( Family family );

			/**
			 * @fn Family getFamily()
			 * @brief gets font family
			 * @return font family
			 */
			Family getFamily();

			/**
			 * @fn void setStyle( Style style )
			 * @brief sets font style
			 * @param[in] style font style
			 */
			void setStyle( Style style );

			/**
			 * @fn Style getStyle()
			 * @brief gets font style
			 * @return font style
			 */
			Style getStyle();

			/**
			 * @fn void setWeight( Weight weight )
			 * @brief sets font weight
			 * @param[in] weight font weight
			 */
			void setWeight( Weight weight );

			/**
			 * @fn Weight getWeight();
			 * @brief gets font weight 
			 * @return font weight
			 */			 
			Weight getWeight();

			/**
			 * @fn void setFace( System::String^ face )
			 * @brief sets font face name
			 * @param[in] face font face name
			 */
			void setFace( System::String^ face );

			/**
			 * @fn System::String^ getFace()
			 * @brief gets font face name
			 * @return font face name
			 */
			System::String^ getFace();

			/**
			 * @fn void setUnderline( bool underline )
			 * @brief sets underline flag
			 * @param[in] underline underline flag
			 */
			void setUnderline( bool underline );

			/**
			 * @fn bool hasUnderline()
			 * @brief judges this font has underline or not.
			 * @return If true, this font has underline.
			 */			
			bool hasUnderline();

			/**
			 * @fn void setStrikeThrough( bool strikethrough )
			 * @brief sets strike-through flag
			 * @param[in] strikethrough strike-through flag
			 */
			void setStrikeThrough( bool strikethrough );

			/**
			 * @fn bool hasStrikeThrough()
			 * @brief judges this font has strike-through or not.
			 * @return If true, this font has strike-through.
			 */
			bool hasStrikeThrough();

		protected:
			/** Font object */
			kome::img::Font& m_font;
		};
	}
}

#endif	// __KOME_CLR_FONT_WRAPPER_H__
