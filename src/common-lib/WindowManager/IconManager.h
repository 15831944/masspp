/**
 * @file IconManager.h
 * @brief interfaces of IconManager class
 *
 * @author S.Tanaka
 * @date 2006.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_ICON_MANAGER_H__
#define __KOME_WINDOW_ICON_MANAGER_H__


class wxIcon;
class wxImageList;


#include <map>


namespace kome {
	namespace window {
		/**
		 * @class IconManager
		 * @brief Icon management class
		 */
		class WINDOW_MANAGER_CLASS IconManager {
		protected:
			/**
			 * @fn IconManager()
			 * @brief constructor
			 */
			IconManager();

			/**
			 * @fn virtual ~IconManager()
			 * @brief destructor
			 */
			virtual ~IconManager();

		protected:
			/** icon map ( icon name -> icon id ) */
			std::map< std::string, int > m_iconMap;

			/** icon index map ( icon -> icon index ) */
			std::map< kome::plugin::PluginIcon*, int > m_indexMap;

			/** icon array */
			std::vector< wxIcon* > m_icons;

		public:
			/**
			 * @fn int getIconIndex( const char* name )
			 * @brief gets icon index
			 * @param[in] name icon name ( [icon name] or [icon name]@[plug-in name] )
			 * @return icon index (If there isn't specified icon, this method returns negative value.)
			 */
			int getIconIndex( const char* name );

			/**
			 * @fn unsigned int getNumberOfIcons()
			 * @brief gets the number of icons
			 * @return the number of icons
			 */
			unsigned int getNumberOfIcons();

			/**
			 * @fn wxIcon* getIcon( const int index )
			 * @brief gets icon object
			 * @param[in] index index
			 * @return icon object (If index is illegal, this method returns NULL.)
			 */
			wxIcon* getIcon( const int index );

			/**
			 * @fn wxIcon* getIcon( const char* name )
			 * @brief get icon object
			 * @param[in] name icon name ( [icon name] or [icon name]@[plug-in name] )
			 * @return icon object. (If there isn't specified icon, this method returns NULL.)
			 */
			wxIcon* getIcon( const char* name );

			/**
			 * @fn wxIcon* getIcon( kome::plugin::PluginIcon* icon )
			 * @brief gets icon object
			 * @param[in] icon icon information
			 * @return icon object. (If there isn't specified icon, this method returns NULL.)
			 */
			wxIcon* getIcon( kome::plugin::PluginIcon* icon );

			/**
			 * @fn wxImageList* createImageList( const unsigned int width, const unsigned int height )
			 * @brief creates image list object
			 * @param[in] width width of icon
			 * @param[in] height height of icon
			 * @return new image list object
			 */
			wxImageList* createImageList( const unsigned int width, const unsigned int height );

		protected:
			/**
			 * @fn void createIconMap()
			 * @brief creates icon map
			 */
			void createIconMap();

		public:
			/**
			 * @fn static void loadBitmap( wxIcon* icon, kome::plugin::PluginIcon* iconInfo )
			 * @brief loads bitmap data to create icon
			 * @param[in] icon icon object
			 * @param[in] iconInfo icon information object
			 */
			static void loadBitmap( wxIcon* icon, kome::plugin::PluginIcon* iconInfo );

		public:
			/**
			 * @fn static IconManager& getInstance()
			 * @brief gets object
			 * @return IconManager object (This is the only object.)
			 */
			static IconManager& getInstance();
		};
	}
}

#endif	// __KOME_WINDOW_ICON_MANAGER_H__
