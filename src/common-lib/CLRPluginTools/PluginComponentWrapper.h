/**
 * @file PluginComponentWrapper.h
 * @brief interfaces of PluginComponentWrapper class
 *
 * @author S.Tanaka
 * @date 2013.08.02
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_COMPONENT_WRAPPER_H__
#define __KOME_CLR_PLUGIN_COMPONENT_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginIconWrapper;
		ref class PluginCallWrapper;
		ref class ParameterSettingsWrapper;
		ref class SettingsPageWrapper;

		/**
		 * @class PluginComponentWrapper
		 * @brief plug-in component wrapper class to use on CLR environment
		 */
		public ref class PluginComponentWrapper {
		public:
			/**
			 * @fn PluginComponentWrapper( kome::plugin::PluginComponent& plugin )
			 * @brief constructor
			 * @param[in] insComponent plugin component object
			 */
			PluginComponentWrapper( kome::plugin::PluginComponent& insComponent );

			/**
			 * @fn virtual ~PluginComponentWrapper( )
			 * @brief destructor
			 */
			virtual	~PluginComponentWrapper( );

		public:
			/**
			 * @fn kome::plugin::PluginComponent& getPluginComponent( void )
			 * @brief gets plug-in component object
			 * @return plug-in component object
			 */
			kome::plugin::PluginComponent&	getPluginComponent( void );

		protected:
			/** plug-in component object */
			kome::plugin::PluginComponent&	m_insComponent;

		public:
			/**
			 * @fn void setName( System::String^ strName )
			 * @brief sets the plug-in component name
			 * @param[in] strName plug-in component name
			 */
			void	setName( System::String^ strName );

			/**
			 * @fn System::String^ getName( void )
			 * @brief gets the plug-in component name
			 * @return plug-in component name
			 */
			System::String^	getName( void );

			/**
			 * @fn void setDesc( System::String^ strDescription )
			 * @brief sets the plug-in component description
			 * @param[in] strDescription plug-in component description
			 */
			void	setDesc( System::String^ strDescription );

			/**
			 * @fn System::String^ getDesc( void )
			 * @brief gets the plug-in component description
			 * @return plug-in component description
			 */
			System::String^	getDesc( void );

			/**
			 * @fn void addCall( PluginCallWrapper^ pluginCall )
			 * @brief adds call
			 * @param[in] pluginCall pointer of PluginCall
			 */
			void	addCall( PluginCallWrapper^ pluginCall );

			/**
			 * @fn unsigned int getNumberOfCalls( void )
			 * @brief gets the number of calls
			 * @return the number of calls
			 */
			unsigned int	getNumberOfCalls( void );

			/**
			 * @fn PluginCallWrapper^ getCall( const unsigned int unIndex )
			 * @brief gets PluginCallWrapper
			 * @param[in] unIndex stack index
			 * @return pointer of PluginCallWrapper
			 */
			PluginCallWrapper^	getCall( const unsigned int unIndex );

			/**
			 * @fn void addPackage( System::String^ strPackage )
			 * @brief adds call
			 * @param[in] strPackage package name
			 */
			void	addPackage( System::String^ strPackage );

			/**
			 * @fn bool hasPackage( System::String^ strPackage )
			 * @brief has package
			 * @param[in] strPackage package name
			 * @return return true:has package false:not has
			 */
			bool	hasPackage( System::String^ strPackage );

// >>>>>>	@Date:2013/08/22	<Add>	A.Ozaki
//
			/**
			 * @fn void setEnable( void )
			 * @brief sets enable component
			 */
			void	setEnable( void );

			/**
			 * @fn void resetEnable( void )
			 * @brief sets disable component
			 */
			void	resetEnable( void );

			/**
			 * @fn bool isEnable( void )
			 * @brief judges whether it's enable component.
			 * @return If it's enable component, returns true.
			 */
			bool	isEnable( void );
//
// <<<<<<	@Date:2013/08/22	<Add>	A.Ozaki

		};
	}
}

#endif	// __KOME_CLR_PLUGIN_COMPONENT_WRAPPER_H__
