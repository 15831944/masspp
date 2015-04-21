/**
 * @file PluginCallWrapper.h
 * @brief interfaces of PluginCallWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_CALL_WRAPPER_H__
#define __KOME_CLR_PLUGIN_CALL_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginInfoWrapper;
		ref class PluginIconWrapper;

		/**
		 * @class PluginCallWrapper
		 * @brief plug-in function wrapper class to use on CLR environment
		 */
		public ref class PluginCallWrapper {
		public:
			/**
			 * @fn PluginCallWrapper( kome::plugin::PluginCall& call )
			 * @brief constructor
			 * @param[in] call plugi-in function object
			 */
			PluginCallWrapper( kome::plugin::PluginCall& call );

			/**
			 * @fn virtual ~PluginCallWrapper()
			 * @brief destructor
			 */
			virtual ~PluginCallWrapper();

		public:
			/**
			 * @fn kome::plugin::PluginCall& getPluginCall()
			 * @brief gets plug-in function object
			 * @return plug-in function object
			 */
			kome::plugin::PluginCall& getPluginCall();

		protected:
			/** plug-in function object */
			kome::plugin::PluginCall& m_call;

		public:
			/**
			 * @fn PluginInfoWrapper^ getPlugin()
			 * @brief gets plug-in information
			 * @return plug-in information object
			 */
			PluginInfoWrapper^ getPlugin();

			/**
			 * @fn void setType( System::String^ type )
			 * @brief sets call type
			 * @param[in] type call type
			 */
			void setType( System::String^ type );

			/**
			 * @fn System::String^ getType()
			 * @brief gets call type
			 * @return call type
			 */
			System::String^ getType();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets call name
			 * @param[in] name call name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets call name
			 * @return call name
			 */
			System::String^ getName();

			/**
			 * @fn void setIcon( PluginIconWrapper^ icon )
			 * @brief sets plug-in icon
			 * @param[in] icon plug-in icon information object
			 */
			void setIcon( PluginIconWrapper^ icon );

			/**
			 * @fn PluginIconWrapper^ getIcon()
			 * @brief gets plug-in icon
			 * @return plug-in icon information object
			 */
			PluginIconWrapper^ getIcon();

			/**
			 * @fn void short setOrder( unsigned short order )
			 * @brief sets function order
			 * @param[in] order function order
			 */
			void setOrder( unsigned short order );

			/**
			 * @fn unsigned short getOrder()
			 * @brief gets function order
			 * @return function order
			 */
			unsigned short getOrder();

			/**
			 * @fn void addStatus( System::String^ name, System::String^ value )
			 * @brief adds status
			 * @param[in] name status name
			 * @param[in] value status value
			 */
			void addStatus( System::String^ name, System::String^ value );

			/**
			 * @fn unsigned int getNumberOfStatuses()
			 * @brief gets the number of statuses
			 * @return the number of statues
			 */
			unsigned int getNumberOfStatuses();

			/**
			 * @fn System::String^ getStatusName( unsigned int idx )
			 * @brief gets status name
			 * @param[in] idx status index
			 * @return status name
			 */
			System::String^ getStatusName( unsigned int idx );

			/**
			 * @fn System::String^ getStatusValue( unsigned int idx )
			 * @brief gets status value
			 * @param[in] idx status index
			 * @return status value
			 */
			System::String^ getStatusValue( unsigned int idx );

			/**
			 * @fn void clearStatuses()
			 * @brief clears statuses
			 */
			void clearStatuses();

			/**
			 * @fn void setIfLoaded( bool ifloaded )
			 * @brief sets ifloaded flag
			 * @param[in] ifloaded ifloaded flag
				   - true: If library file is not loaded, the function is not called.
				   - false: If library file is not loaded, it is opened and the function is called.
			 */
			void setIfLoaded( bool ifloaded );

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
					System::String^ methodName,
					System::String^ className,
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
				System::String^ methodName,
				System::String^ className,
				bool isStatic
			);

			/**
			 * @fn System::String^ getMethodName()
			 * @brief gets method or function name
			 * @return method or function name
			 */
			System::String^ getMethodName();

			/**
			 * @fn System::String^ getClassName()
			 * @brief gets class name.
			 * @return class name (If it's not class method, returns NULL.)
			 */
			System::String^ getClassName();

			/**
			 * @fn bool isStaticMethod()
			 * @brief judges whether it's static method.
			 * @return If it's static method, returns true.
			 */
			bool isStaticMethod();

			/**
			 * @fn PropertiesWrapper^ getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			PropertiesWrapper^ getProperties();

			/**
			 * @fn void setDescription( System::String^ description )
			 * @brief sets plug-in call description
			 * @param[in] description
			 */
			void setDescription( System::String^ description );

			/**
			 * @fn System::String^ getDescription()
			 * @brief gets plug-in calldescription
			 * @return description
			 */
			System::String^ getDescription();

			/**
			 * @fn bool checkStatus()
			 * @brief checks status variable
			 * @return If status value is mutched or this function ignores status variable, this method returns true.
			 */
			bool checkStatus();

			/**
			 * @fn ClrVariant^ invoke( ClrParameters^ params )
			 * @brief execute function or method
			 * @param[in] params parameter information
			 * @return return value
			 */
			ClrVariant^ invoke( ClrParameters^ params );
		};
	}
}

#endif	// __KOME_CLR_PLUGIN_CALL_WRAPPER_H__
