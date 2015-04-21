/**
 * @file PluginIconWrapper.h
 * @brief interfaces of PluginIconWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_ICON_WRAPPER_H__
#define __KOME_CLR_PLUGIN_ICON_WRAPPER_H__


namespace kome {
	namespace clr {

		/**
		 * @class PluginIconWrapper
		 * @brief plug-in icon wrapper class to use on CLR environment
		 */		 
		public ref class PluginIconWrapper {
		public:
			/**
			 * @fn PluginIconWrapper( kome::plugin::PluginIcon& icon )
			 * @brief constructor
			 * @param[in] icon plug-in icon object
			 */
			PluginIconWrapper( kome::plugin::PluginIcon& icon );

			/**
			 * @fn virtual ~PluginIconWrapper()
			 * @brief destructor
			 */
			virtual ~PluginIconWrapper();

		public:
			/**
			 * @fn kome::plugin::PluginIcon& getPluginIcon()
			 * @brief gets plug-in icon object
			 * @return plug-in icon object
			 */
			kome::plugin::PluginIcon& getPluginIcon();

		protected:
			/** plug-in icon object */
			kome::plugin::PluginIcon& m_icon;

		public:
			/**
			 * @fn void setId( unsigned int id )
			 * @brief sets plug-in icon ID
			 * @param[in] id plug-in icon ID
			 */
			void setId( unsigned int id );

			/**
			 * @fn unsigned int getId()
			 * @brief gets plug-in icon ID
			 * @return plug-in icon ID
			 */
			unsigned int getId();

			/**
			 * @fn void setType( System::String^ type )
			 * @brief sets plug-in icon type
			 * @param[in] type plug-in icon type
			 */
			void setType( System::String^ type );

			/**
			 * @fn System::String^ getType()
			 * @brief gets plug-in icon type
			 * @return plug-in icon type
			 */
			System::String^ getType();

			/**
			 * @fn void setSize( unsigned short width, unsigned short height )
			 * @brief sets size of icon image data
			 * @param[in] width the width of icon image data
			 * @param[in] height the height of icon image data
			 */
			void setSize( unsigned short width, unsigned short height );

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
			 * @fn void setName( System::String^ name )
			 * @brief sets plug-in icon name
			 * @param[in] name plug-in icon name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets plug-in icon name
			 * @return plug-in icon name
			 */
			System::String^ getName();

			/** 
			 * @fn void setPath( System::String^ path )
			 * @brief sets plug-in icon path
			 * @param[in] path plug-in icon path
			 */
			void setPath( System::String^ path );

			/**
			 * @fn System::String^ getPath()
			 * @brief get plug-in icon path
			 * @return plug-in icon path
			 */
			System::String^ getPath();
		};
	}
}

#endif	// __KOME_CLR_PLUGIN_ICON_WRAPPER_H__
