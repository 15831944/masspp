/**
 * @file PluginIcon.h
 * @brief interfaces of PluginIcon class
 *
 * @author S.Tanaka
 * @date 2006.09.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_ICON_H__
#define __KOME_PLUGIN_ICON_H__


#include <string>


namespace kome {
	namespace plugin {

		class PluginInfo;

		/**
		 * @class PLUGIN_TOOLS_CLASS PluginIcon 
		 * @brief plug-in icon information class
		 */
		class PLUGIN_TOOLS_CLASS PluginIcon {
		public:
			/**
			 * @fn PluginIcon( PluginInfo* plugin )
			 * @brief constructor
			 * @param[in] plugin plug-in information object that has this icon
			 */
			PluginIcon( PluginInfo* plugin );

			/**
			 * @fn virtual ~PluginIcon()
			 * @brief destructor
			 */
			virtual ~PluginIcon();
		protected:
			/** plug-in information object that has this icon */
			PluginInfo* m_plugin;

			/** plug-in icon ID */
			unsigned int m_id;

			/** plug-in icon type */
			std::string m_type;

			/** the width of icon image data */
			unsigned short m_width;
			/** the height of icon image data */
			unsigned short m_height;

			/** plug-in icon name */
			std::string m_name;

			/** plug-in icon file path */
			std::string m_path;
		public:
			/**
			 * @fn void setId( const unsigned int id )
			 * @brief sets plug-in icon ID
			 * @param[in] id plug-in icon ID
			 */
			void setId( const unsigned int id );

			/**
			 * @fn unsigned int getId()
			 * @brief gets plug-in icon ID
			 * @return plug-in icon ID
			 */
			unsigned int getId();

			/**
			 * @fn void setType( const char* type )
			 * @brief sets plug-in icon type
			 * @param[in] type plug-in icon type
			 */
			void setType( const char* type );

			/**
			 * @fn const char* getType()
			 * @brief gets plug-in icon type
			 * @return plug-in icon type
			 */
			const char* getType();

			/**
			 * @fn void setSize( const unsigned short width, const unsigned short height )
			 * @brief sets size of icon image data
			 * @param[in] width the width of icon image data
			 * @param[in] height the height of icon image data
			 */
			void setSize( const unsigned short width, const unsigned short height );

			/**
			 * @fn unsigned short getWidth()
			 * @brief gets the width of icon image data
			 */
			unsigned short getWidth();

			/**
			 * @fn unsigned short getHeight()
			 * @brief get the height of icon image data
			 */
			unsigned short getHeight();

			/** 
			 * @fn void setName( const char* name )
			 * @brief sets plug-in icon name
			 * @param[in] name plug-in icon name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets plug-in icon name
			 * @return plug-in icon name
			 */
			const char* getName();

			/** 
			 * @fn void setPath( const char* path )
			 * @brief sets plug-in icon path
			 * @param[in] path plug-in icon path
			 */
			void setPath( const char* path );

			/**
			 * @fn const char* getPath()
			 * @brief get plug-in icon path
			 * @return plug-in icon path
			 */
			const char* getPath();

	
		};
	}
}

#endif	// __KOME_PLUGIN_ICON_H__
