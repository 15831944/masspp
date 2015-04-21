/**
 * @file PluginHelp.h
 * @brief interfaces of PluginHelp class
 *
 * @author S.Tanaka
 * @date 2007.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_HELP_H__
#define __KOME_PLUGIN_HELP_H__

namespace kome {
	namespace plugin {

		class PluginInfo;

		/**
		 * @class PluginHelp
		 * @brief plug-in help information class
		 */
		class PLUGIN_TOOLS_CLASS PluginHelp {
		public:
			/**
			 * @fn PluginHelp()
			 * @brief constructor
			 */
			PluginHelp();

			/**
			 * @fn virtual ~PluginHelp()
			 * @brief destructor
			 */
			virtual ~PluginHelp();

		protected:
			/** plug-in */
			PluginInfo* m_plugin;

			/** path */
			std::string m_path;

			/** order */
			int m_order;

		public:

			/**
			 * @fn void setPlugin( PluginInfo* plugin )
			 * @brief sets plug-in
			 * @param[in] plugin plug-in
			 */
			void setPlugin( PluginInfo* plugin );

			/**
			 * @fn PluginInfo* getPlugin()
			 * @brief gets plug-in 
			 * @return plug-in
			 */
			PluginInfo* getPlugin();

			/**
			 * @fn void setHelpFile( const char* path )
			 * @brief sets help file path
			 * @param[in] path help file path
			 */
			void setHelpFile( const char* path );

			/**
			 * @fn const char* getHelpFile()
			 * @brief gets help file path
			 */
			const char* getHelpFile();

			/**
			 * @fn void setOrder( const int order )
			 * @brief sets order
			 * @param[in] order order
			 */
			void setOrder( const int order );

			/**
			 * @fn int getOrder()
			 * @brief gets order
			 * @return order
			 */
			int getOrder();
		};
	}
}


#endif		// __KOME_PLUGIN_HELP_H__

