/**
 * @file PluginComponent.h
 * @brief interfaces of PluginComponent class
 *
 * @author A.Ozaki
 * @date 2013.07.19
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_PLUGIN_COMPONENT_H__
#define __KOME_PLUGIN_COMPONENT_H__


#include <string>
#include <set>
#include <map>
#include <boost/utility.hpp>

namespace kome
{
	namespace plugin
	{
		class PluginCall;

		/**
		 * @class PluginComponent
		 * @brief plug-in component object class
		 */
		class PLUGIN_TOOLS_CLASS PluginComponent
		{
		public:
			/**
			 * @fn PluginComponent( )
			 * @brief constructor
			 */
			PluginComponent( );

			/**
			 * @fn virtual ~PluginComponent( )
			 * @brief destructor
			 */
			virtual	~PluginComponent( );

		protected:
			/** name */
			std::string	m_strName;

			/** description */
			std::string	m_strDescription;

			/** function in component */
			std::vector<PluginCall *>	m_vpCalls;

			/** Packages information of plugin manager */
			std::vector<std::string>	m_vstrPackages;

			/** Enable component flag */
			bool	m_bEnable;

		public:
			/**
			 * @fn void setName( const char *lpstrName )
			 * @brief sets component name
			 * @param[in] lpstrName component name
			 */
			void	setName( const char *lpstrName );

			/**
			 * @fn const char *getName( void )
			 * @brief gets component name
			 * @return component name
			 */
			const char	*getName( void );

			/**
			 * @fn void setDesc( const char *lpstrDescription )
			 * @brief sets component description
			 * @param[in] lpstrDescription component description
			 */
			void	setDesc( const char *lpstrDescription );

			/**
			 * @fn const char* getDesc( void )
			 * @brief gets component description
			 * @return component description
			 */
			const char	*getDesc( void );

			/**
			 * @fn void addCall( PluginCall *pCall )
			 * @brief adds call
			 * @param[in] pCall pointer of PluginCall
			 */
			void	addCall( PluginCall *pCall );

			/**
			 * @fn unsigned int getNumberOfCalls( void )
			 * @brief gets the number of calls
			 * @return the number of calls
			 */
			unsigned int	getNumberOfCalls( void );

			/**
			 * @fn PluginCall *getCall( const unsigned int unIndex )
			 * @brief gets PluginCall
			 * @param[in] unIndex stack index
			 * @return pointer of PluginCall
			 */
			PluginCall	*getCall( const unsigned int unIndex );

			/**
			 * @fn void addPackage( const char *lpstrPackage )
			 * @brief adds package name
			 * @param[in] lpstrPackage package name
			 */
			void	addPackage( const char *lpstrPackage );

			/**
			 * @fn bool hasPackage( const char* lpstrPackage )
			 * @brief has package
			 * @param[in] lpstrPackage package name
			 * @return return true:has package false:not has
			 */
			bool	hasPackage( const char* lpstrPackage );

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
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
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki
		};
	}
}

#endif	// __KOME_PLUGIN_COMPONENT_H__
