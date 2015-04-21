/**
 * @file PluginInfoWrapper.h
 * @brief interfaces of PluginInfoWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_INFO_WRAPPER_H__
#define __KOME_CLR_PLUGIN_INFO_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginIconWrapper;
		ref class PluginCallWrapper;
		ref class ParameterSettingsWrapper;
		ref class SettingsPageWrapper;
		ref class PluginComponentWrapper;	// @Date:2013/08/02	<Add>	A.Ozaki

		/**
		 * @class PluginInfoWrapper
		 * @brief plug-in information wrapper class to use on CLR environment
		 */
		public ref class PluginInfoWrapper {
		public:
			/**
			 * @fn PluginInfoWrapper( kome::plugin::PluginInfo& plugin )
			 * @brief constructor
			 * @param[in] plugin plug-in object
			 */
			PluginInfoWrapper( kome::plugin::PluginInfo& plugin );

			/**
			 * @fn virtual ~PluginInfoWrapper()
			 * @brief destructor
			 */
			virtual ~PluginInfoWrapper();

		public:
			/**
			 * @fn kome::plugin::PluginInfo& getPluginInfo()
			 * @brief gets plug-in information object
			 * @return plug-in information object
			 */
			kome::plugin::PluginInfo& getPluginInfo();

		protected:
			/** plug-in object */
			kome::plugin::PluginInfo& m_plugin;

		public:
			/**
			 * @fn void setDir( System::String^ dir )
			 * @brief sets the directory that this plug-in exists
			 * @param[in] dir directory path
			 */
			void setDir( System::String^ dir );

			/**
			 * @fn System::String^ getDir()
			 * @brief gets the directory that this plug-in exists
			 * @return directory path
			 */
			System::String^ getDir();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets the plug-in name
			 * @param[in] name plug-in name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets the plug-in name
			 * @return plug-in name
			 */
			System::String^ getName();

			/** 
			 * @fn void setVersion( System::String^ version )
			 * @brief sets plug-in version
			 * @param[in] version plug-in version
			 */
			void setVersion( System::String^ version );

			/**
			 * @fn System::String^ getVersion()
			 * @brief gets plug-in version
			 * @return plug-in version
			 */
			System::String^ getVersion();

			/**
			 * @fn void setProvider( System::String^ provider )
			 * @brief sets plug-in provider
			 * @param[in] provider plug-in provider
			 */
			void setProvider( System::String^ provider );

			/**
			 * @fn System::String^ getProvider()
			 * @brief gets plug-in provider
			 * @return plug-in provider
			 */
			System::String^ getProvider();

			/**
			 * @fn void setLink( System::String^ link )
			 * @brief sets library link file path
			 * @param[in] link library link file path
			 */
			void setLink( System::String^ link );

			/**
			 * @fn System::String^ getLink()
			 * @brief gets library link file path
			 * @return library link file path
			 */
			System::String^ getLink();

			/**
			 * @fn void clearIcons()
			 * @brief clear the array of plugi-in icon
			 */
			void clearIcons();

			/**
			 * @fn PluginIconWrapper^ createNewIcon()
			 * @brief creates new plug-in icon object
			 * @return the pointer to new plug-in call object
			 */
			PluginIconWrapper^ createNewIcon();

			/**
			 * @fn unsigned int getNumberOfIcons()
			 * @brief gets the number of plug-in icons
			 * @return the number of plug-in icons
			 */
			unsigned int getNumberOfIcons();

			/**
			 * @fn PluginIconWrapper^ getIcon( unsigned int index )
			 * @brief gets plug-in icon object
			 * @param[in] index plug-in icon index
			 * @return the pointer to plug-in icon object. (If index is illegal, this method returns NULL.)
			 */
			PluginIconWrapper^ getIcon( unsigned int index );

			/**
			 * @fn PluginIconWrapper^ getIcon( System::String^ name )
			 * @brief gets plug-in icon object
			 * @param[in] name plug-in icon name
			 * @return the pointer to plug-in icon object hat has specified name.
					   (If there isn't plug-in icon object that has specified name, this method returns NULL.)
			 */
			PluginIconWrapper^ getIcon( System::String^ name );

			/**
			 * @fn void clearCalls()
			 * @brief clear the array of plug-in call
			 */
			void clearCalls();

			/**
			 * @fn PluginCallWrapper^ createNewCall()
			 * @brief creates new plug-in call object
			 * @return the pointer to new plug-in call object
			 */
			PluginCallWrapper^ createNewCall();

			/**
			 * @fn unsigned int getNumberOfCalls()
			 * @brief gets the number of plug-in call objects
			 * @return the number of plug-in call objects
			 */
			unsigned int getNumberOfCalls();

			/**
			 * @fn PluginCallWrapper^ getCall( unsigned index )
			 * @brief gets a plug-in call object
			 * @param[in] index plug-in call index
			 * @return plug-in call object ( If index is illegal, this method returns NULL. )
			 */
			PluginCallWrapper^ getCall( unsigned index );

			/** 
			 * @fn PluginCallWrapper^ getCall( System::String^ name )
			 * @brief gets plug-in call object
			 * @param[in] name plug-in call name
			 * @return the pointer to the plug-in call object that has specified name.
					   (If there isn't plug-in call that has specified name, this method returns NULL.)
			 */
			PluginCallWrapper^ getCall( System::String^ name );

			/**
			 * @fn void clearOptions()
			 * @brief clears options
			 */
			void clearOptions();

			/**
			 * @fn bool addOptionsFile( System::String^ path )
			 * @brief adds options file
			 * @param[in] path options file path
			 */
			bool addOptionsFile( System::String^ path );

			/**
			 * @fn unsigned int getNumberOfOptions()
			 * @brief gets the number of options files
			 * @return the number of options files
			 */
			unsigned int getNumberOfOptions();

			/**
			 * @fn ParameterSettingsWrapper^ getOptions( unsigned int index )
			 * @brief gets options file information
			 * @param[in] index options file index
			 * @return options file information
			 */
			ParameterSettingsWrapper^ getOptions( unsigned int index );

			/**
			 * @fn void clearParameters()
			 * @brief clears parameters
			 */
			void clearParameters();

			/**
			 * @fn bool addParametersFile( System::String^ path )
			 * @brief adds parameters file
			 * @param[in] path parameters file path
			 */
			bool addParametersFile( System::String^ path );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters files
			 * @return the number of parameters files
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn ParameterSettingsWrapper^ getParameters( unsigned int index )
			 * @brief gets parameters file information
			 * @param[in] index parameters file index
			 * @return parameters file information
			 */
			ParameterSettingsWrapper^ getParameters( unsigned int index );

			/**
			 * @fn SettingsPageWrapper^ getPage( System::String^ name )
			 * @brief gets settings page
			 * @param[in] name settings page name
			 * @return settings page object (If NULL, specified settings page does not exist.)
			 */
			SettingsPageWrapper^ getPage( System::String^ name );

			/**
			 * @fn void addJavaClassPath( System::String^ path )
			 * @brief adds java class path
			 * @param[in] path class path
			 */
			void addJavaClassPath( System::String^ path );

			/**
			 * @fn unsigned int getNumberOfJavaClassPathes()
			 * @brief gets the number of java class pathes
			 * @return the number of java class pathes
			 */
			unsigned int getNumberOfJavaClassPathes();

			/**
			 * @fn System::String^ getJavaClassPath( unsigned int idx )
			 * @brief get java class path
			 * @param[in] idx class path index
			 * @return java class path. (If null the index is illegal.)
			 */			 
			System::String^ getJavaClassPath( unsigned int idx );

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
		public:
			/**
			 * @fn void setHidden( const bool bHidden )
			 * @brief set hidden flag
			 * @param[in] bHidden true:hidden yes false:hidden no 
			 */
			void	setHidden( const bool bHidden );

			/**
			 * @fn bool istHidden( void )
			 * @brief get hidden flag
			 */
			bool	isHidden( void );

			/**
			 * @fn void addCall( PluginCallWrapper^ pluginCall )
			 * @brief add PluginCall
			 * @param[in] pluginCall pointer of PluginCallWrapper
			 */
			void	addCall( PluginCallWrapper^ pluginCall );

			/**
			 * @fn void deleteCall( PluginCallWrapper^ pluginCall )
			 * @brief delete PluginCall
			 * @param[in] pluginCall pointer of PluginCallWrapper
			 */
			void	deleteCall( PluginCallWrapper^ pluginCall );

			/**
			 * @fn void addComponent(PluginComponentWrapper^ pluginComponent )
			 * @brief add PluginComponent
			 * @param[in] pluginComponent pointer of PluginComponentWrapper
			 */
			void	addComponent( PluginComponentWrapper^ pluginComponent );
//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

// >>>>>>	@Date:2013/08/22	<Add>	A.Ozaki
//
			/**
			 * @fn PluginComponentWrapper^	getComponent( System::String^ strComponentName )
			 * @brief get PluginComponent
			 * @param[in] strComponentName plugin component name
			 * @return pointer of PluginComponent. (If null the not exist component name)
			 */			 
			PluginComponentWrapper^	getComponent( System::String^ strComponentName );

			/**
			 * @fn unsigned int getNumberOfComponents( void )
			 * @brief gets the number of PluginComponent
			 * @return the number of PluginComponent
			 */
			unsigned int	getNumberOfComponents( void );

			/**
			 * @fn PluginComponentWrapper^	getComponent( unsigned int nIndex )
			 * @brief get PluginComponent
			 * @param[in] nIndex PluginComponent index
			 * @return pointer of PluginComponent. (If null the nIndex is illegal.)
			 */			 
			PluginComponentWrapper^	getComponent( unsigned int nIndex );
			
//
// <<<<<<	@Date:2013/08/22	<Add>	A.Ozaki

		};
	}
}

#endif	// __KOME_CLR_PLUGIN_INFO_WRAPPER_H__
