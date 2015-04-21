/**
 * @file PluginInfo.h
 * @brief interfaces of PluginInfo class
 *
 * @author S.Tanaka
 * @date 2006.08.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_INFO_H__
#define __KOME_PLUGIN_INFO_H__


#include <string>
#include <vector>
#include <map>

#include "StatusBarInfo.h"	// 2011/01/13 <Add> OKADA,H

namespace kome {
	namespace plugin {

		class PluginCall;
		class PluginIcon;
		class ParameterSettings;
		class PluginHelp;
		class SettingsPage;
		class PluginComponent;	// @Date:2013/07/29	<Add>	A.Ozaki

		/**
		 * @class PluginInfo
		 * @brief plug-in information management class
		 */
		class PLUGIN_TOOLS_CLASS PluginInfo {
		public:
			/**
			 * @fn PluginInfo()
			 * @brief constructor
			 */
			PluginInfo();

			/**
			 * @fn virtual ~PluginInfo()
			 * @brief destructor
			 */
			virtual ~PluginInfo();

		protected:
			/**	the directory that this plug-in exists */
			std::string m_dir;

			/** plug-in name */
			std::string m_name;

			/** plug-in version */
			std::string m_version;

			/** plug-in provider */
			std::string m_provider;

			/** library link file path */
			std::string m_link;

			/** the array of plug-in calls */
			std::vector< PluginCall* > m_calls;
			/** the map of plug-in calls ( name -> object ) */
			std::map< std::string, PluginCall* > m_callMap;

			/** the array of plug-in icons */
			std::vector< PluginIcon* > m_icons;
			/** the map of plug-in icons ( name -> object ) */
			std::map< std::string, PluginIcon* > m_iconMap;

			/** options information */
			std::vector< ParameterSettings* > m_options;

			/** parameters information */
			std::vector< ParameterSettings* > m_parameters;

			/** the array of plug-in help */
			std::vector< PluginHelp* > m_helps;

			/** Status Bar Info */
			std::vector< StatusBarInfo* > m_statusBarInfo;

			/** Java class path */
			std::vector< std::string > m_javaClassPathes;

			/** enable lookup table */
			bool m_flgActiveEnable;	// @date 2013/05/24 <Add> OKADA

			/** map active table */
			std::map< unsigned int, unsigned int >	m_mapActiveTable;	

// >>>>>>	@Date:2013/07/30	<Add>	A.Ozaki
			/** hidden flag */
			bool	m_bHidden;

			/** the array of plug-in calls */
			std::vector< PluginComponent* > m_vpComponent;

// <<<<<<	@Date:2013/07/30	<Add>	A.Ozaki

		public:
			/**
			 * @fn void setDir( const char* dir )
			 * @brief sets the directory that this plug-in exists
			 * @param[in] dir directory path
			 */
			void setDir( const char* dir );

			/**
			 * @fn const char* getDir()
			 * @brief gets the directory that this plug-in exists
			 * @return directory path
			 */
			const char* getDir();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets the plug-in name
			 * @param[in] name plug-in name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets the plug-in name
			 * @return plug-in name
			 */
			const char* getName();

			/** 
			 * @fn void setVersion( const char* version )
			 * @brief sets plug-in version
			 * @param[in] version plug-in version
			 */
			void setVersion( const char* version );

			/**
			 * @fn const char* getVersion()
			 * @brief gets plug-in version
			 * @return plug-in version
			 */
			const char* getVersion();

			/**
			 * @fn void setProvider( const char* provider )
			 * @brief sets plug-in provider
			 * @param[in] provider plug-in provider
			 */
			void setProvider( const char* provider );

			/**
			 * @fn const char* getProvider()
			 * @brief gets plug-in provider
			 * @return plug-in provider
			 */
			const char* getProvider();

			/**
			 * @fn void setLink( const char* link )
			 * @brief sets library link file path
			 * @param[in] link library link file path
			 */
			void setLink( const char* link );

			/**
			 * @fn const char* getLink()
			 * @brief gets library link file path
			 * @return library link file path
			 */
			const char* getLink();

			/**
			 * @fn void clearIcons()
			 * @brief clear the array of plugi-in icon
			 */
			void clearIcons();

			/**
			 * @fn PluginIcon* createNewIcon()
			 * @brief creates new plug-in icon object
			 * @return the pointer to new plug-in call object
			 */
			PluginIcon* createNewIcon();

			/**
			 * @fn unsigned int getNumberOfIcons()
			 * @brief gets the number of plug-in icons
			 * @return the number of plug-in icons
			 */
			unsigned int getNumberOfIcons();

			/**
			 * @fn PluginIcon* getIcon( const unsigned int index )
			 * @brief gets plug-in icon object
			 * @param[in] index plug-in icon index
			 * @return the pointer to plug-in icon object. (If index is illegal, this method returns NULL.)
			 */
			PluginIcon* getIcon( const unsigned int index );			

			/**
			 * @fn PluginIcon* getIcon( const char* name )
			 * @brief gets plug-in icon object
			 * @param[in] name plug-in icon name
			 * @return the pointer to plug-in icon object hat has specified name.
					   (If there isn't plug-in icon object that has specified name, this method returns NULL.)
			 */
			PluginIcon* getIcon( const char* name );

			/**
			 * @fn void clearCalls()
			 * @brief clear the array of plug-in call
			 */
			void clearCalls();

			/**
			 * @fn PluginCall* createNewCall()
			 * @brief creates new plug-in call object
			 * @return the pointer to new plug-in call object
			 */
			PluginCall* createNewCall();

			/**
			 * @fn unsigned int getNumberOfCalls()
			 * @brief gets the number of plug-in call objects
			 * @return the number of plug-in call objects
			 */
			unsigned int getNumberOfCalls();

			/**
			 * @fn PluginCall* getCall( const unsigned int index )
			 * @brief gets a plug-in call object
			 * @param[in] index plug-in call index
			 * @return plug-in call object ( If index is illegal, this method returns NULL. )
			 */
			PluginCall* getCall( const unsigned int index );

			/** 
			 * @fn PluginCall* getCall( const char* name )
			 * @brief gets plug-in call object
			 * @param[in] name plug-in call name
			 * @return the pointer to the plug-in call object that has specified name.
					   (If there isn't plug-in call that has specified name, this method returns NULL.)
			 */
			PluginCall* getCall( const char* name );

			/**
			 * @fn void clearOptions()
			 * @brief clears options
			 */
			void clearOptions();

			/**
			 * @fn bool addOptionsFile( const char* path )
			 * @brief adds options file
			 * @param[in] path options file path
			 */
			bool addOptionsFile( const char* path );

			/**
			 * @fn unsigned int getNumberOfOptions()
			 * @brief gets the number of settings files
			 * @return the number of settings files
			 */
			unsigned int getNumberOfOptions();

			/**
			 * @fn ParameterSettings* getOptions( const unsigned int index )
			 * @brief gets options file information
			 * @param[in] index options file index
			 * @return options file information
			 */
			ParameterSettings* getOptions( const unsigned int index );

			/**
			 * @fn void clearParameters()
			 * @brief clears parameters
			 */
			void clearParameters();

			/**
			 * @fn bool addParametersFile( const char* path )
			 * @brief adds parameters file
			 * @param[in] path parameters file path
			 */
			bool addParametersFile( const char* path );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters files
			 * @return the number of parameters files
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn ParameterSettings* getParameters( const unsigned int index )
			 * @brief gets parameters file information
			 * @param[in] index parameters file index
			 * @return parameters file information
			 */
			ParameterSettings* getParameters( const unsigned int index );

			/**
			 * @fn void clearHelps()
			 * @brief clears plug-in helps
			 */
			void clearHelps();

			/**
			 * @fn PluginHelp* createNewHelp()
			 * @brief creates new plug-in help
			 * @return new plug-in help object
			 */
			PluginHelp* createNewHelp();

			/**
			 * @fn unsigned int getNumberOfHelps()
			 * @brief gets the number of plug-in helps
			 * @return plug-in help
			 */
			unsigned int getNumberOfHelps();

			/**
			 * @fn PluginHelp* getHelp( const unsigned int index )
			 * @brief gets plug-in help object
			 * @param[in] index plug-in help index
			 * @return the pointer to the plug-in help object. (If the index is illegal, this method returns NULL.)
			 */
			PluginHelp* getHelp( const unsigned int index );

			/**
			 * @fn SettingsPage* getPage( const char* name )
			 * @brief gets parameters page
			 * @param[in] name parameters page name
			 * @return parameters page object (If NULL, specified parameters page does not exist.)
			 */
			SettingsPage* getPage( const char* name );

			/**
			 * @fn void clearStatusBarInfo()
			 * @brief clear StatusBar Info
			 */
			void clearStatusBarInfo();

			/**
			 * @fn StatusBarInfo* createCommonStatusBarInfo()
			 * @brief creates new plug-in StatusBarInfo Common
			 * @return new plug-in StatusBarInfo Common object
			 */
			StatusBarInfo* createCommonStatusBarInfo();

			/**
			 * @fn StatusBarInfo* createNewStatusBarInfo()
			 * @brief creates new plug-in StatusBarInfo
			 * @return new plug-in StatusBarInfo object
			 */
			StatusBarInfo* createNewStatusBarInfo();

			/**
			 * @fn unsigned int getNumberOfStatusBarInfos()
			 * @brief gets the number of StatusBarInfos
			 * @return number of StatusBarInfos
			 */
			unsigned int getNumberOfStatusBarInfos();

			/**
			 * @fn StatusBarInfo* getStatusBarInfo( const unsigned int index )
			 * @brief gets StatusBarInfo object
			 * @param[in] index StatusBarInfo index
			 * @return the pointer to the StatusBarInfo object. (If the index is illegal, this method returns NULL.)
			 */
			StatusBarInfo* getStatusBarInfo( const unsigned int index );

			/**
			 * @fn void addJavaClassPath( const char* path )
			 * @brief adds java class path
			 * @param[in] path class path
			 */
			void addJavaClassPath( const char* path );

			/**
			 * @fn unsigned int getNumberOfJavaClassPathes()
			 * @brief gets the number of java class pathes
			 * @return the number of java class pathes
			 */
			unsigned int getNumberOfJavaClassPathes();

			/**
			 * @fn const char* getJavaClassPath( const unsigned int idx )
			 * @brief get java class path
			 * @param[in] idx class path index
			 * @return java class path. (If null the index is illegal.)
			 */			 
			const char* getJavaClassPath( const unsigned int idx );

		protected:
			/**
			 * @fn void createIconMap()
			 * @brief creates icon map
			 */
			void createIconMap();

			/**
			 * @fn void createCallMap()
			 * @brief create call map
			 */
			void createCallMap();

		public:
			/**
			 * @fn void setActiveEnable( bool flgActiveEnable )
			 * @brief set active flag for enable
			 * @param[in] flgActiveEnable true:use active table
			 */
			void setActiveEnable( bool flgActiveEnable ){	// @date 2013/05/24 <Add> OKADA ------->
				m_flgActiveEnable = flgActiveEnable;
			}												// @date 2013/05/24 <Add> OKADA <-------

			/**
			 * @fn void updateActiveLookupTable( void )
			 * @brief update active lookup table
			 */
			void updateActiveLookupTable( void );			// @date 2013/05/24 <Add> OKADA


// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
		public:
			/**
			 * @fn void setHidden( const bool bHidden )
			 * @brief set hidden flag
			 * @param[in] bHidden true:hidden yes false:hidden no
			 */
			void setHidden( const bool bHidden );

			/**
			 * @fn bool isHidden( void )
			 * @brief get hidden flag
			 * @return true:hidden yes false:hidden no
			 */
			bool isHidden( void );

			/**
			 * @fn void addCall( PluginCall *pCall )
			 * @brief add PluginCall
			 * @param[in] pCall pointer of PluginCall
			 */
			void addCall( PluginCall *pCall );

			/**
			 * @fn void deleteCall( PluginCall *pCall )
			 * @brief delete PluginCall
			 * @param[in] pCall pointer of PluginCall
			 */
			void deleteCall( PluginCall *pCall );

			/**
			 * @fn void addComponent( PluginComponent *pComponent )
			 * @brief add PluginComponent
			 * @param[in] pComponent pointer of PluginComponent
			 */
			void addComponent( PluginComponent *pComponent );
//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki
// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//
			/**
			 * @fn PluginComponent *getComponent( const char* lpcstrComponentName )
			 * @brief get PluginComponent
			 * @param[in] lpcstrComponentName plugin component name
			 * @return pointer of PluginComponent. (If null the not exist component name)
			 */
			PluginComponent	*getComponent( const char* lpcstrComponentName );

			/**
			 * @fn unsigned int getNumberOfComponents( void )
			 * @brief gets the number of PluginComponent
			 * @return the number of PluginComponent
			 */
			unsigned int getNumberOfComponents( void );

			/**
			 * @fn PluginComponent *getComponent( const unsigned int nIndex )
			 * @brief get PluginComponent
			 * @param[in] nIndex PluginComponent index
			 * @return pointer of PluginComponent. (If null the nIndex is illegal.)
			 */
			PluginComponent	*getComponent( const unsigned int nIndex );
			
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki

// @date 2013/08/22 <Add> OKADA ------->

			/**
			 * @fn int setEnableComponent( const char *lpcstrComponentName )
			 * @param[in] lpcstrComponentName plugin component name
			 * @brief sets enable component
			 * @return If this function is successful, 0 will be returned.
			 */
			int setEnableComponent( const char *lpcstrComponentName );

			/**
			 * @fn int setEnableComponent( const unsigned int nIndex )
			 * @param[in] nIndex PluginComponent index
			 * @brief sets enable component
			 * @return If this function is successful, 0 will be returned.
			 */
			int setEnableComponent( const unsigned int nIndex );

			/**
			 * @fn int resetEnableComponent( const char *lpcstrComponentName )
			 * @brief sets disable component
			 * @param[in] lpcstrComponentName plugin component name
			 * @return If this function is successful, 0 will be returned.
			 */
			int resetEnableComponent( const char *lpcstrComponentName );

			/**
			 * @fn int resetEnableComponent( const unsigned int nIndex )
			 * @brief sets disable component
			 * @param[in] nIndex PluginComponent index
			 * @return If this function is successful, 0 will be returned.
			 */
			int resetEnableComponent( const unsigned int nIndex );

			/**
			 * @fn bool isEnableComponent( const char *lpcstrComponentName )
			 * @brief judges whether it's enable component.
			 * @param[in] lpcstrComponentName PluginComponent name
			 * @return If it's enable component, returns true.
			 */
			bool isEnableComponent( const char *lpcstrComponentName );

			/**
			 * @fn bool isEnableComponent( const unsigned int nIndex )
			 * @brief judges whether it's enable component.
			 * @param[in] nIndex index of plugin component
			 * @return If it's enable component, returns true.
			 */
			bool isEnableComponent( const unsigned int nIndex );

			/**
			 * @fn bool hasFunction( const char *lpcstrComponentName, kome::plugin::PluginCall* pCall )
			 * @brief It is investigated whether the specified component has function to specify. 
			 * @param[in] lpcstrComponentName plugin component name
			 * @param[in] pCall function
			 * @return If the specified component has function to specify, true will be returned.
			 */
			bool hasFunction( const char *lpcstrComponentName, kome::plugin::PluginCall* pCall );

			/**
			 * @fn bool hasFunction( const unsigned int nComponentIndex, kome::plugin::PluginCall* pCall )
			 * @brief It is investigated whether the specified component has function to specify. 
			 * @param[in] nComponentIndex PluginComponent index
			 * @param[in] pCall function
			 * @return If the specified component has function to specify, true will be returned.
			 */
			bool hasFunction( const unsigned int nComponentIndex, kome::plugin::PluginCall* pCall );

// @date 2013/08/22 <Add> OKADA <-------

		};
	}
}

#endif	// __KOME_PLUGIN_INFO_H__
