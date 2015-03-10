/**
 * @file PluginListCtrl.h
 * @brief interfaces of PluginListCtrl class
 *
 * @author OKADA, H
 * @date 2013/02/22
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PLUGINMANAGER_VIEW_PLUGIN_LIST_CTRL_H__
#define __KOME_PLUGINMANAGER_VIEW_PLUGIN_LIST_CTRL_H__


typedef struct tag_plugin {	
	int nCnt;
	std::string strName;		// Plugin Name
	std::string strProvider;	// Plugin Provider
	std::string strPath;		// Plugin Path
} PLUGIN, *LPPLUGIN ;


namespace kome{
	namespace pluginmanager{
		namespace view{
			
			/**
			 * @class SpectrumListCtrl
			 * @brief peak position list control class
			 */
			class PluginsListCtrl : public kome::window::GridListCtrl {

			public:
				/**
				 * @fn PluginListCtrl( wxWindow* parent, const int width, const int height )
				 * @brief constructor
				 * @param parent parent window
				 * @param width width
				 * @param height height
				 */
				PluginsListCtrl( wxWindow* parent, const int width, const int height );

				/**
				 * @fn virtual ~PluginListCtrl()
				 * @brief destructor
				 */
				virtual ~PluginsListCtrl();
	
			protected:
				/** plugin column */
				int m_pluginCol;

				/** provider column */
				int m_providerCol;

				/** path column */
				int m_pathCol;
				
				/** appended base column */
				int m_appendedCol;

			protected:
				/** plugins */
				std::vector< PLUGIN > m_vecPlugins;

				/** selected no */
				int m_nSelectedRawNo;	// ëIëÇ≥ÇÍÇΩçsî‘çÜ 

				/** parent */
				wxWindow* m_parent;

			public:
				/**
				 * @fn unsigned int getNumberOfPlugins()
				 * @brief gets the number of Plugins
				 * @return the number of Plugins
				 */
				unsigned int getNumberOfPlugins();

				/**
				 * @fn PLUGIN getPlugin( const unsigned int idx )
				 * @brief gets the Plugin
				 * @param[in] idx Plugin index
				 * @return Plugin
				 */
				PLUGIN getPlugin( const unsigned int idx );

				/**
				 * @fn void clearPlugins()
				 * @brief clear plugins
				 */
				void clearPlugins();

				/**
				 * @fn void addPlugins( PLUGIN plugin )
				 * @brief add plugins
				 */
				void addPlugins( PLUGIN plugin );

			protected:
				/**
				 * @fn virtual void onCreate()
				 * @brief This method is called by create method
				 */
				virtual void onCreate();

				/**
				 * @fn virtual int getInt( const long long data, const int column ) const
				 * @brief gets integer value from data (override method)
				 * @param data data
				 * @param column column number
				 * @return integer value
				*/
				virtual int getInt( const long long data, const int column ) const;

				/**
				 * @fn virtual double getDouble( const long long data, const int column ) const
				 * @brief gets double value from data
				 * @param data data
				 * @param column column number
				 */
				virtual double getDouble( const long long data, const int column ) const;

				/**
				 * @fn virtual std::string getString( const long long data, const int column ) const
				 * @brief gets character string value (override method)
				 * @param data data
				 * @param column column number
				 */
				virtual std::string getString( const long long data, const int column ) const;

			public:
				/**
				 * @fn void onSizePluginMgr( wxSizeEvent& evt )
				 * @brief This method is called when list size is changed
				 * @param[in] evt size event information
				 */
				void onSizePluginMgr( wxSizeEvent& evt );	// @date 2013/05/13 <Add> OKADA

			public:
				/**
				 * @fn void OnSelect( wxListEvent& evt )
				 * @brief This method is called when a list item is selected
				 * @param[in] evt list event object
				 */
				void OnSelect( wxListEvent& evt );

				/**
				 * @fn int getSelectedRawNo()
				 * @brief gets selected raw no
				 * @return selected raw no
				 */
				int getSelectedRawNo();

				/**
				 * @fn void OnSelectPlugin()
				 * @brief This method is called when a list item is selected
				 */
				void OnSelectPlugin();

			private:
				DECLARE_EVENT_TABLE();

			};
		}
	}
}

#endif
