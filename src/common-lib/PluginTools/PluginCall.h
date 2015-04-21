/**
 * @file PluginCall.h
 * @brief interfaces of PluginCall class
 *
 * @author S.Tanaka
 * @date 2006.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_CALL_H__
#define __KOME_PLUGIN_CALL_H__


#include <string>
#include <set>
#include <map>
#include <boost/utility.hpp>

// plug-in function definition
typedef kome::objects::Variant (*PLUGIN_FUNCTION_PT)( kome::objects::Parameters* );


namespace kome {
	namespace plugin {

		class PluginInfo;
		class PluginIcon;

		/**
		 * @class PluginCall
		 * @brief plug-in function call object class
		 */
		class PLUGIN_TOOLS_CLASS PluginCall {
		public:
			/**
			 * @fn PluginCall( PluginInfo* plugin )
			 * @brief constructor
			 * @param[in] plugin plugin information object;
			 */
			PluginCall( PluginInfo* plugin );

			/**
			 * @fn virtual ~PluginCall()
			 * @brief destructor
			 */
			virtual ~PluginCall();

		protected:
			/** plug-in information object */
			PluginInfo* m_plugin;

			/** call type */
			std::string m_type;

			/** call name */
			std::string m_name;

			/** icon path */
			PluginIcon* m_icon;

			/** order */
			unsigned short m_order;

			/** statuses */
			std::vector< std::pair< std::string, std::string > > m_statuses;

			/** ifloaded flag */
			bool m_ifloaded;

			/** properties */
			kome::core::Properties m_properties;

			/** call function info */
			struct {
				std::string methodName;
				std::string className;
				bool isStatic;
			} m_functionInfo;

			/** description */
			std::string m_description;

			/** call function */
			PLUGIN_FUNCTION m_func;

			/** the pointer to call function */
			PLUGIN_FUNCTION_PT m_ptFunc;

			/** plug-in call information on CLR environment */
			void* m_clrCall;

			/** active flag of plugin manager */
			bool m_flgActive;	// #81 Plugin Manager	// @date 2013/02/21 <Add> OKADA

			/** Packages information of plugin manager */
			std::vector<std::string> m_vstrPackages;	// #81 Plugin Manager	// @date 2013/02/21 <Add> OKADA

		public:
			/**
			 * @fn PluginInfo* getPlugin()
			 * @brief gets plug-in information
			 * @return plug-in information object
			 */
			PluginInfo* getPlugin();

			/**
			 * @fn void setType( const char* type )
			 * @brief sets call type
			 * @param[in] type call type
			 */
			void setType( const char* type );

			/**
			 * @fn const char* getType()
			 * @brief gets call type
			 * @return call type
			 */
			const char* getType();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets call name
			 * @param[in] name call name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets call name
			 * @return call name
			 */
			const char* getName();

			/**
			 * @fn void setIcon( PluginIcon* icon )
			 * @brief sets plug-in icon
			 * @param[in] icon plug-in icon information object
			 */
			void setIcon( PluginIcon* icon );

			/**
			 * @fn PluginIcon* getIcon()
			 * @brief gets plug-in icon
			 * @return plug-in icon information object
			 */
			PluginIcon* getIcon();

			/**
			 * @fn void setOrder( const unsigned short order )
			 * @brief sets function order
			 * @param[in] order function order
			 */
			void setOrder( const unsigned short order );

			/**
			 * @fn unsigned short getOrder()
			 * @brief gets function order
			 * @return function order
			 */
			unsigned short getOrder();

			/**
			 * @fn void addStatus( const char* name, const char* value )
			 * @brief adds status
			 * @param[in] name status name
			 * @param[in] value status value
			 */
			void addStatus( const char* name, const char* value );

			/**
			 * @fn unsigned int getNumberOfStatuses()
			 * @brief gets the number of statuses
			 * @return the number of statues
			 */
			unsigned int getNumberOfStatuses();

			/**
			 * @fn const char* getStatusName( const unsigned int idx )
			 * @brief gets status name
			 * @param[in] idx status index
			 * @return status name
			 */
			const char* getStatusName( const unsigned int idx );

			/**
			 * @fn const char* getStatusValue( const unsigned int idx )
			 * @brief gets status value
			 * @param[in] idx status index
			 * @return status value
			 */
			const char* getStatusValue( const unsigned int idx );

			/**
			 * @fn void clearStatuses()
			 * @brief clears statuses
			 */
			void clearStatuses();

			/**
			 * @fn void setIfLoaded( const bool ifloaded )
			 * @brief sets ifloaded flag
			 * @param[in] ifloaded ifloaded flag
				   - true: If library file is not loaded, the function is not called.
				   - false: If library file is not loaded, it is opened and the function is called.
			 */
			void setIfLoaded( const bool ifloaded );

			/**
			 * @fn bool getIfLoaded()
			 * @brief gets ifloaded flag
			 * @return ifloaded flag
				   - true: If library file is not loaded, the function is not called.
				   - false: If library file is not loaded, it is opened and the function is called.
			 */
			bool getIfLoaded();

			/**
			 * @fn void setFunction(
					const char* methodName,
					const char* className,
					bool isStatic
				)
			 * @brief sets call function information
			 * @param[in] methodName function or method name
			 * @param[in] className class name.
					(If className is NULL, this is not class method but simple function)
			 * @param[in] isStatic sets static method or not
					(If className is NULL, this parameter is ignored.)
			 */
			void setFunction(
				const char* methodName,
				const char* className,
				bool isStatic
			);

			/**
			 * @fn const char* getMethodName()
			 * @brief gets method or function name
			 * @return method or function name
			 */
			const char* getMethodName();

			/**
			 * @fn const char* getClassName()
			 * @brief gets class name.
			 * @return class name (If it's not class method, returns NULL.)
			 */
			const char* getClassName();

			/**
			 * @fn bool isStaticMethod()
			 * @brief judges whether it's static method.
			 * @return If it's static method, returns true.
			 */
			bool isStaticMethod();

			/**
			 * @fn kome::core::Properties& getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			kome::core::Properties& getProperties();

			/**
			 * @fn void setDescription( const char* description )
			 * @brief sets plug-in call description
			 * @param[in] description
			 */
			void setDescription( const char* description );

			/**
			 * @fn const char* getDescription()
			 * @brief gets plug-in calldescription
			 * @return description
			 */
			const char* getDescription();

			/**
			 * @fn bool checkStatus()
			 * @brief checks status variable
			 * @return If status value is mutched or this function ignores status variable, this method returns true.
			 */
			bool checkStatus();

// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
// change method type ( virtual )
			/**
			 * @fn virtual kome::objects::Variant invoke( kome::objects::Parameters* params )
			 * @brief execute function or method
			 * @param[in] params parameter information
			 * @return return value
			 */
			virtual	kome::objects::Variant invoke( kome::objects::Parameters* params );
//
// <<<<<<	@Date:2013/07/29	<Modify>	A.Ozaki

		protected:
			/**
			 * @fn virtual kome::objects::Variant onInvoke( kome::objects::Parameters* params )
			 * @brief This method is called by invoke method
			 * @param[in] params parameter information
			 * @return return value
			 */
			virtual kome::objects::Variant onInvoke( kome::objects::Parameters* params );

			/**
			 * @fn PLUGIN_FUNCTION* getFunction()
			 * @brief gets the pointer of call function
			 * @return the pointer of call function
			 */			
			PLUGIN_FUNCTION* getFunction();


		public:
			/**
			 * @fn void setActive( bool flgActive )
			 * @brief set active
			 * @param[in] flgActive active flag
			 */
			void setActive( bool flgActive );				// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn bool isActive( void )
			 * @brief is active
			 * @return return true:active false:not active
			 */
			bool isActive( void );							// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn void addPackage( const char* pszPackage )
			 * @brief add package
			 * @param[in] pszPackage package name
			 */
			void addPackage( const char* pszPackage );		// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn void clearPackages( void )
			 * @brief clear packages
			 */
			void clearPackages( void );						// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn void removePackage( const char* pszPackage )
			 * @brief remove package
			 * @param[in] pszPackage package name
			 */
			void removePackage( const char* pszPackage );	// @date 2013/02/21 <Add> OKADA

			/**
			 * @fn bool hasPackage( const char* pszPackage )
			 * @brief has package
			 * @param[in] pszPackage package name
			 * @return return true:has package false:not has
			 */
			bool hasPackage( const char* pszPackage );		// @date 2013/02/21 <Add> OKADA



		};
	}
}

#endif	// __KOME_PLUGIN_CALL_H__
