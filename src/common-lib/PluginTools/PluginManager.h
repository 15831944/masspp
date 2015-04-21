/**
 * @file PluginManager.h
 * @brief interfaces of PluginManager class
 *
 * @author S.Tanaka
 * @date 2006.06.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_MANAGER_H__
#define __KOME_PLUGIN_MANAGER_H__


#include "PluginMenu.h"
#include "SettingsChapter.h"

#include <string>
#include <vector>
#include <map>

#include <boost/function.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>


#ifdef _WIN32
	#define DL_HANDLE HINSTANCE
#else
	#define DL_HANDLE void*
#endif	// _WIN32


namespace kome {
	namespace plugin {

		class PluginInfo;
		class PluginCall;
		class ParameterSettings;
		class PluginFunctionItem;

		/**
		 * @class PluginManager
		 * @brief plugin management class
		 */
		class PLUGIN_TOOLS_CLASS PluginManager {
		protected:
			/**
			 * @fn PluginManager()
			 * @brief constructor
			 */
			PluginManager();

			/**
			 * @fn virtual ~PluginManager()
			 * @brief destructor
			 */
			virtual ~PluginManager();

		protected:
			/** plug-in XML parser */
			xercesc::SAX2XMLReader* m_pluginParser;

			/** settings XML parser */
			xercesc::SAX2XMLReader* m_settingsParser;

		protected:
			/** plug-in menu information object */
			PluginMenu* m_menu;

		protected:
			/** plug-in menu information object of SpectrumItem */
			PluginMenu*	m_pSpectrumItem;		// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of SpectrumView */
			PluginMenu*	m_pSpectrumView;		// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of SpectrumSubView */
			PluginMenu*	m_pSpectrumSubView;		// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of ChromatogramItem */
			PluginMenu*	m_pChromatogramItem;	// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of ChromatogramView */
			PluginMenu*	m_pChromatogramView;	// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of ChromatogramSubView */
			PluginMenu*	m_pChromatogramSubView;	// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of GroupItem */
			PluginMenu*	m_pGroupItem;			// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of DataMapView */
			PluginMenu*	m_pDataMapView;			// @date 2012/10/30 <Add> OKADA

			/** plug-in menu information object of DataMapSubView */
			PluginMenu*	m_pDataMapSubView;		// @date 2012/10/30 <Add> OKADA

		protected:
			/** settings root chapter */
			SettingsChapter m_settingsRootChapter;

			/** parameters root chapter */
			SettingsChapter m_paramsRootChapter;

			/** menu ID */
			unsigned int m_menuId;

			/** the array of plug-in information */
			std::vector< PluginInfo* > m_plugins;

			/** plugi-in information map ( plug-in name -> plug-in object ) */
			std::map< std::string, PluginInfo* > m_pluginMap;

			/** dynamic link handle map (path -> handle) */
			std::map< std::string, void* > m_handleMap;

			/** call map ( type -> call ) */ 
			std::map< std::string, std::vector< PluginCall* > > m_callMap;

			/** function item map ( type -> item ) */
			std::map< std::string, std::vector< PluginFunctionItem * > > m_itemMap;

		private:
			/** Package */
			std::vector< std::string >m_vstrPackages;	// @date 2013/02/21 <Add> OKADA

			/** enable lookup table */
			bool m_flgActiveEnable;	// @date 2013/05/24 <Add> OKADA

			/** map active table */
			std::map< unsigned int, unsigned int >	m_mapActiveTable;

		public:
			/**
			 * @fn void setXMLSchema( const char* pluginXsd, const char* settingsXsd )
			 * @brief sets XML schema file path
			 * @param[in] pluginXsd plug-in schema file path
			 * @param[in] settingsXsd settings schema file path
			 */
			void setXMLSchema( const char* pluginXsd, const char* settingsXsd );

			/**
			 * @fn PluginMenu& getMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in menu object
			 */
			PluginMenu& getMenu();

			// @date 2012/10/29 <Add> OKADA ------->
			/**
			 * @fn PluginMenu& getSpectrumItemMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in SpectrumItem menu object
			 */
			PluginMenu& getSpectrumItemMenu();

			/**
			 * @fn PluginMenu& getSpectrumViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in SpectrumView  menu object
			 */
			PluginMenu& getSpectrumViewMenu();

			/**
			 * @fn PluginMenu& getSpectrumSubViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in SpectrumSubView menu object
			 */
			PluginMenu& getSpectrumSubViewMenu();

			/**
			 * @fn PluginMenu& getChromatogramItemMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in ChromatogramItem menu object
			 */
			PluginMenu& getChromatogramItemMenu();

			/**
			 * @fn PluginMenu& getChromatogramViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in ChromatogramView  menu object
			 */
			PluginMenu& getChromatogramViewMenu();

			/**
			 * @fn PluginMenu& getChromatogramSubViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in ChromatogramSubView menu object
			 */
			PluginMenu& getChromatogramSubViewMenu();

			/**
			 * @fn PluginMenu& getGroupItemMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in GroupItem menu object
			 */
			PluginMenu& getGroupItemMenu();
			

			/**
			 * @fn PluginMenu& getDataMapViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in DataMapView menu object
			 */
			PluginMenu& getDataMapViewMenu();

			/**
			 * @fn PluginMenu& getDataMapSubViewMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in DataMapSubView menu object
			 */
			PluginMenu& getDataMapSubViewMenu();

			// @date 2012/10/29 <Add> OKADA <-------

		   /**
			* @fn SettingsChapter& getSettingsRootChapter()
			* @brief gets settings root chapter
			* @return settings root chapter
			*/
			SettingsChapter& getSettingsRootChapter();

			/**
			 * @fn SettingsChapter& getParametersRootChapter()
			 * @brief gets parameters root chapter
			 * @return parameters root chapter
			 */
			SettingsChapter& getParametersRootChapter();

			/**
			 * @fn bool loadPluginFile( const char* xmlPath )
			 * @brief loads plug-in file.
			 * @param[in] xmlPath plug-in xml file path.
			 * @return If true, it succeeded to load the file.
			 */
			bool loadPluginFile( const char* xmlPath );

			/**
			 * @fn bool loadSettingsFile( const char* xmlPath, ParameterSettings& settings )
			 * @brief loads settings file
			 * @param[in] xmlPath settings  file
			 * @param[out] settings the object to stored settings information
			 * @return If true, it succeeded to load the file.
			 */
			bool loadSettingsFile( const char* xmlPath, ParameterSettings& settings );

			/**
			 * @fn bool loadPath( const char* path, boost::function< bool ( const char* ) > filterFun, const bool createMap = true )
			 * @brief loads plug-in paths
			 * @param[in] path plug-in path
			 * @param[in] filterFun the function that judges whether specified path is plug-in file.
			 * @param[in] createMap create map flag
			 * @return If true, it succeeded to load the file.
			 */
			bool loadPath( const char* path, boost::function< bool ( const char* ) > filterFun, const bool createMap = true );

			/**
			 * @fn unsigned int issueMenuId()
			 * @brief issues menu ID
			 * @return menu ID
			 */
			unsigned int issueMenuId();

			/**
			 * @fn void* getLink( const char* path )
			 * @brief gets link handle
			 * @param[in] path dynamic link library path
			 * @return link handle (If specified link file is not loaded, this method returns NULL.)
			 */
			void* getLink( const char* path );

			/**
			 * @fn void* openLink( const char* path )
			 * @brief opens dynamic link library.
			 * @param[in] path dynamic link library path
			 * @return link handle
			 */
			void* openLink( const char* path );

			/**
			 * @fn void closeLink( const char* path )
			 * @brief closes dynamic link library.
			 * @param[in] path dynamic link library path
			 */
			void closeLink( const char* path );

			/**
			 * @fn unsigned int getNumberOfPlugins()
			 * @brief gets the number of plug-in informations.
			 * @return the number of plug-in informations
			 */
			unsigned int getNumberOfPlugins();

			/**
			 * @fn PluginInfo* getPlugin( const unsigned int index )
			 * @brief gets plug-in information
			 * @param[in] index plug-in information index
			 * @return plug-in information object (If index is illegal value, this method returns NULL.)
			 */
			PluginInfo* getPlugin( const unsigned int index );

			/**
			 * @fn PluginInfo* getPlugin( const char* name )
			 * @brief gets plugi-in information object
			 * @param[in] name plug-in name
			 * @return plug-in object that has specified name.
					   (If there isn't plug-in that has specified name, this method returns NULL.)
			 */
			PluginInfo* getPlugin( const char* name );

			/**
			 * @fn void forEachCalls( const char* type, boost::function< void ( PluginCall* ) > eachFun, const bool checkStatus )
			 * @brief applies function to all plug-in function object specified type
			 * @param[in] type function type
			 * @param[in] eachFun the function to be applied
			 * @param[in] checkStatus If true, check status before calling function.
			 */
			void forEachCalls(
				const char* type,
				boost::function< void ( PluginCall* ) > eachFun,
				const bool checkStatus
			);

			/**
			 * @fn unsigned int getNumberOfFunctionItems( const char* type )
			 * @brief gets the number of function items
			 * @param[in] type function item type
			 * @return the number of function items
			 */
			unsigned int getNumberOfFunctionItems( const char* type );

			/**
			 * @fn PluginFunctionItem* getFunctionItem( const char* type, const unsigned int index )
			 * @brief gets plug-in function item
			 * @param[in] type function item type
			 * @param[in] index function item index
			 * @return function item object (If NULL, the type or the index is illegal.)
			 */
			PluginFunctionItem* getFunctionItem( const char* type, const unsigned int index );

			/**
			 * @fn PluginFunctionItem* getFunctionItem( const char* type, const char* name, kome::objects::SettingParameterValues* settings = NULL )
			 * @brief gets plug-in function item
			 * @param[in] type function item type
			 * @param[in] name function item name
			 * @param[out] settings the object to store parameters
			 * @return function item object (If NULL, the type or the name is illegal.)
			 */
			PluginFunctionItem* getFunctionItem( const char* type, const char* name, kome::objects::SettingParameterValues* settings = NULL );
			
			/**
			 * @fn void callAllFunctions(
					kome::objects::Parameters* params,
					const char* type,
					const bool checkStatus = true
				)
			 * @brief calls all functions of the specified type. (This method cannot get return value.)
			 * @param[in] params parameter information
			 * @param[in] type plug-in call type
			 * @param[in] checkStatus If true, check status before executing.
			 */
			void callAllFunctions(
				kome::objects::Parameters* params,
				const char* type,
				const bool checkStatus = true
			);

			/**
			 * @fn void clearPlugins()
			 * @brief remove plug-in information objects from the array
			 */
			void clearPlugins();

		protected:
			/**
			 * @fn void createPluginMap()
			 * @brief creates plug-in map
			 */
			void createPluginMap();

			/**
			 * @fn void createCallMap()
			 * @brief creates plug-in call map
			 */
			void createCallMap();

			/**
			 * @fn void createFunctionItemMap( const char* type )
			 * @brief creates function item map
			 * @param[in] type item type
			 */
			void createFunctionItemMap( const char* type );

		protected:
			/**
			 * @fn static void addFunctionItem( PluginCall* call, std::vector< PluginFunctionItem * >* items )
			 * @brief adds function item map
			 * @param[in] call plug-in function 
			 * @param[out] items items array to store function item
			 */
			static void addFunctionItem( PluginCall* call, std::vector< PluginFunctionItem * >* items );

			/**
			 * @fn static bool lessCall( PluginCall* call0, PluginCall* call1 )
			 * @brief compare plug-in functions to sort
			 * @param[in] call0 plug-in function object to be compared
			 * @param[in] call1 plug-in function object to compare
			 * @return If call0 sorts before call1, this method returns true.
			 */
			static bool lessCall( PluginCall* call0, PluginCall* call1 );

		public:
			/**
			 * @fn void addPackage( const char * pszPackage )
			 * @brief add Package
			 * @param[in] pszPackage Package
			 */
			void addPackage( const char * pszPackage );	// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn int getNumberOfPackages( void )
			 * @brief get number of packages
			 * @return number of packages
			 */
			int getNumberOfPackages( void );	// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn char* getPackage( int nIndex )
			 * @brief A package name is acquired from an index. 
			 * @param[in] nIndex index
			 * @return Package
			 */
			char* getPackage( int nIndex );	// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn void updatePlugins( void )
			 * @brief Menu information and FunctionItem information are reconstructed in consideration of the active state of PluginCall. 
			 */
			void updatePlugins( void );	// @date 2013/02/21 <Add> OKADA

		public:
			/**
			 * @fn void setActiveEnable( bool flgActiveEnable )
			 * @brief set active flag for enable
			 * @param[in] flgActiveEnable true:use active table
			 */
			void setActiveEnable( bool flgActiveEnable );	// @date 2013/05/24 <Add> OKADA

			/**
			 * @fn bool getActiveEnable( void )
			 * @brief get active flag for enable
			 * @return active flag for enable
			 */
			bool getActiveEnable( void );	// @date 2013/05/27 <Add> OKADA

			/**
			 * @fn void updateActiveLookupTable( void )
			 * @brief update active lookup table
			 */
			void updateActiveLookupTable( void );			// @date 2013/05/24 <Add> OKADA

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
			/**
			 * @fn PluginInfo *getHiddenPlugin( const char *lpszName )
			 * @brief gets hidden type plug-in
			 * @param[in] lpszName plug-in name
			 * @return plug-in information object (hidden type)
			 */
			PluginInfo *getHiddenPlugin( const char *lpszName );
//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
			/**
			 * @fn void addActiveLookupTable( PluginInfo* pInfo )
			 * @param[in] pInfo add PluginInfo
			 * @brief add active lookup table
			 */
			void addActiveLookupTable( PluginInfo *pInfo );

// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki
		public:
			/**
			 * @fn static PluginManager& getInstance()
			 * @brief get PluginManager object
			 * @return refer to the only PluginManager object
			 */
			static PluginManager& getInstance();

		};
	}
}

#endif // __KOME_PLUGIN_MANAGER_H__
