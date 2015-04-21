/**
 * @file PluginManagerWrapper.h
 * @brief interfaces of PluginManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_MANAGER_WRAPPER_H__
#define __KOME_CLR_PLUGIN_MANAGER_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginMenuWrapper;
		ref class PluginInfoWrapper;
		ref class PluginCallWrapper;
		ref class ParameterSettingsWrapper;
		ref class SettingsChapterWrapper;
		ref class PluginFunctionItemWrapper;


		/** filter function */
		public delegate bool PluginFilterFunction( System::String^ );

		/** each call function */
		public delegate void EachCallFunction( PluginCallWrapper^ );

		/**
		 * @class PluginManagerWrapper
		 * @brief plug-in manager wrapper class to use on CLR environment
		 */
		public ref class PluginManagerWrapper {
		protected:
			/**
			 * @fn PluginManagerWrapper( kome::plugin::PluginManager& mgr )
			 * @brief constructor
			 * @param[in] mgr plug-in manager object
			 */
			PluginManagerWrapper( kome::plugin::PluginManager& mgr );

			/**
			 * @fn virtual ~PluginManagerWrapper()
			 * @brief destructor
			 */
			virtual ~PluginManagerWrapper();

		public:
			/**
			 * @fn kome::plugin::PluginManager& getPluginManager()
			 * @brief gets plug-in manager object
			 * @return plug-in manager object
			 */
			kome::plugin::PluginManager& getPluginManager();

		protected:
			/** plug-in manager object */
			kome::plugin::PluginManager& m_mgr;

		public:
			/**
			 * @fn PluginMenuWrapper^ getMenu()
			 * @brief gets plug-in menu object
			 */
			PluginMenuWrapper^ getMenu();

			/**
			 * @fn SettingsChapterWrapper^ getSettingsRootChapter()
			 * @brief gets settings root chapter
			 * @return settings root chapter object
			 */
			SettingsChapterWrapper^ getSettingsRootChapter();

			/**
			 * @fn SettingsChapterWrapper^ getParametersRootChapter()
			 * @brief gets parameters root chapter
			 * @return parameters root chapter object
			 */
			SettingsChapterWrapper^ getParametersRootChapter();

			/**
			 * @fn bool loadPluginFile( System::String^ xmlPath )
			 * @brief loads plug-in file.
			 * @param[in] xmlPath plug-in xml file path.
			 * @return If true, it succeeded to load the file.
			 */
			bool loadPluginFile( System::String^ xmlPath );

			/**
			 * @fn bool loadSettingsFile( System::String^ xmlPath, ParameterSettingsWrapper^ settings )
			 * @brief loads settings file
			 * @param[in] xmlPath settings  file
			 * @param[out] settings the object to be stored settings information
			 * @return If true, it succeeded to load the file.
			 */
			bool loadSettingsFile( System::String^ xmlPath, ParameterSettingsWrapper^ settings );

			/**
			 * @fn bool loadPath( System::String^ path, PluginFilterFunction^ filterFun )
			 * @brief loads plug-in path
			 * @param[in] path plug-in path
			 * @param[in] filterFun the function that judges whether specified path is plug-in file.
			 * @return If true, it succeeded to load the file.
			 */
			bool loadPath( System::String^ path, PluginFilterFunction^ filterFun );

			/**
			 * @fn unsigned int issueMenuId()
			 * @brief issues menu ID
			 * @return menu ID
			 */
			unsigned int issueMenuId();

			/**
			 * @fn void openLink( System::String^ path )
			 * @brief opens dynamic link library.
			 * @param[in] path dynamic link library path
			 * @return link handle
			 */
			void openLink( System::String^ path );

			/**
			 * @fn void closeLink( System::String^ path )
			 * @brief closes dynamic link library.
			 * @param[in] path dynamic link library path
			 */
			void closeLink( System::String^ path );

			/**
			 * @fn unsigned int getNumberOfPlugins()
			 * @brief gets the number of plug-in informations.
			 * @return the number of plug-in informations
			 */
			unsigned int getNumberOfPlugins();

			/**
			 * @fn PluginInfoWrapper^ getPlugin( unsigned int index )
			 * @brief gets plug-in information
			 * @param[in] index plug-in information index
			 * @return plug-in information object (If index is illegal value, this method returns NULL.)
			 */
			PluginInfoWrapper^ getPlugin( unsigned int index );

			/**
			 * @fn PluginInfoWrapper^ getPlugin( System::String^ name )
			 * @brief gets plugi-in information object
			 * @param[in] name plug-in name
			 * @return plug-in object that has specified name.
					   (If there isn't plug-in that has specified name, this method returns NULL.)
			 */
			PluginInfoWrapper^ getPlugin( System::String^ name );

			/**
			 * @fn void forEachCalls( System::String^ type, EachCallFunction^ eachFun, bool checkStatus )
			 * @brief applies function to all plug-in function object specified type
			 * @param[in] type function type
			 * @param[in] eachFun the function to be applied
			 * @param[in] checkStatus If true, check status before calling function.
			 */
			void forEachCalls( System::String^ type, EachCallFunction^ eachFun, bool checkStatus );

			/**
			 * @fn unsigned int getNumberOfFunctionItems( System::String^ type )
			 * @brief gets the number of function items
			 * @param[in] type function item type
			 * @return the number of function items
			 */
			unsigned int getNumberOfFunctionItems( System::String^ type );

			/**
			 * @fn PluginFunctionItemWrapper^ getFunctionItem( System::String^ type, unsigned int index )
			 * @brief gets plug-in function item
			 * @param[in] type function item type
			 * @param[in] index function item index
			 * @return function item object (If NULL, the type or the index is illegal.)
			 */
			PluginFunctionItemWrapper^ getFunctionItem( System::String^ type, unsigned int index );

			/**
			 * @fn PluginFunctionItemWrapper^ getFunctionItem( System::String^ type, System::String^ name, SettingParameterValuesWrapper^ settings )
			 * @brief gets plug-in function item
			 * @param[in] type function item type
			 * @param[in] name function item name
			 * @param[out] settings the object to store parameters
			 * @return function item object (If NULL, the type or the name is illegal.)
			 */
			PluginFunctionItemWrapper^ getFunctionItem( System::String^ type, System::String^ name, SettingParameterValuesWrapper^ settings );

			/**
			 * @fn void callAllFunctions( ClrParameters^ params, System::String^ type, bool checkStatus )
			 * @brief calls all functions of the specified type. (This method cannot get return value.)
			 * @param[in] params parameter information
			 * @param[in] type plug-in call type
			 * @param[in] checkStatus If true, check status before executing.
			 */
			void callAllFunctions( ClrParameters^ params, System::String^ type, bool checkStatus );

			/**
			 * @fn void clearPlugins()
			 * @brief remove plug-in information objects from the array
			 */
			void clearPlugins();

// >>>>>>	@Date:2013/08/02	<Add>	A.Ozaki
//
			/**
			 * @fn PluginInfoWrapper^	getHiddenPlugin( System::String^ strName )
			 * @brief gets hidden type plug-in
			 * @param[in] strName plug-in name
			 * @return plug-in information object (hidden type)
			 */
			PluginInfoWrapper^	getHiddenPlugin( System::String^ strName );
//
// <<<<<<	@Date:2013/08/02	<Add>	A.Ozaki

// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
//
			/**
			 * @fn void addActiveLookupTable( PluginInfoWrapper^ pInfo )
			 * @param[in] pInfo add PluginInfo
			 * @brief add active lookup table
			 */
			void	addActiveLookupTable( PluginInfoWrapper^ pInfo );
//
// <<<<<<	@Date:2013/08/02	<Add>	A.Ozaki

		public:
			/**
			 * @fn static PluginManagerWrapper^ getInstance()
			 * @brief get PluginManager object
			 * @return refer to the only PluginManager object
			 */
			static PluginManagerWrapper^ getInstance();
		};

		/**
		 * @class PluginFilterHelper
		 * @brief plug-in filter helper class
		 */
		class PluginFilterHelper {
		public:
			/**
			 * @fn PluginFilterHelper()
			 * @brief constructor
			 */
			PluginFilterHelper();

			/**
			 * @fn virtual ~PluginFilterHelper()
			 * @brief destructor
			 */
			virtual ~PluginFilterHelper();

		public:
			/**
			 * @fn void setFilterFunction( gcroot< PluginFilterFunction^ > fun )
			 * @brief sets filter function
			 * @param[in] fun filter function
			 */
			void setFilterFunction( gcroot< PluginFilterFunction^ > fun );

			/**
			 * @fn bool pathFilter( const char* path )
			 * @brief execute fitler
			 * @param[in] path file path
			 * @return If the specified path is plug-in file, this method returns true.
			 */
			bool pathFilter( const char* path );

		protected:
			/** filter function */
			gcroot< PluginFilterFunction^ > m_fun;
		};

		/**
		 * @class EachCallHelper
		 * @brief plug-in functions for_each helper class
		 */
		class EachCallHelper {
		public:
			/**
			 * @fn EachCallHelper()
			 * @brief constructor
			 */
			EachCallHelper();

			/**
			 * @fn virtual ~EachCallHelper()
			 * @brief destructor
			 */
			virtual ~EachCallHelper();

		public:
			/**
			 * @fn void setForEachFunction( gcroot< EachCallFunction^ > fun )
			 * @brief sets for each function
			 * @param[in] fun for each function
			 */
			void setForEachFunction( gcroot< EachCallFunction^ > fun );

			/**
			 * @fn void forEach( kome::plugin::PluginCall* call );
			 * @brief applies function to plug-in function
			 * @param[in] call plug-in function object
			 */
			void forEach( kome::plugin::PluginCall* call );

		protected:
			/** filter function */
			gcroot< EachCallFunction^ > m_fun;
		};

	}
}

#endif	// __KOME_CLR_PLUGIN_MANAGER_WRAPPER_H__
