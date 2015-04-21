/**
 * @file ClrPluginFunction.h
 * @brief interfaces of ClrPluginFunction class
 *
 * @author S.Tanaka
 * @date 2006.08.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_FUNCTION_H__
#define __KOME_CLR_PLUGIN_FUNCTION_H__


#include <string>
#include <vector>
#include <boost/function.hpp>


namespace kome {
	namespace clr {

		/**
		 * @class ClrPluginFunction
		 * @brief class method call class on CLR environment
		 */
		class CLR_MANAGER_CLASS ClrPluginFunction {
		public:
			/**
			 * @fn ClrPluginFunction()
			 * @brief constructor
			 */
			ClrPluginFunction();

			/**
			 * @fn virtual ~ClrPluginFunction()
			 * @brief destructor
			 */
			virtual ~ClrPluginFunction();

		protected:
			/** plug-in call ID on CLR environment */
			unsigned int m_id;

			/** dynamic link library path */
			std::string m_dllPath;
			/** class name */
			std::string m_className;
			/** method name */
			std::string m_methodName;
			/** static method flag */
			bool m_isStatic;

			/** function flag */
			bool m_funcFlag;

			/** function object */
			PLUGIN_FUNCTION m_func;

		public:
			/**
			 * @fn unsigned int getId()
			 * @brief gets plug-in call ID on CLR environment
			 * @return plug-in call ID
			 */
			unsigned int getId();

			/**
			 * @fn void setMethodInfo(
					const char* dllPath,
					const char* className,
					const char* methodName,
					bool isStatic
				)
			 * @brief set call method information
			 * @param[in] dllPath dynamic link library path
			 * @param[in] className class name
			 * @param[in] methodName method name
			 * @param[in] isStatic If specified method is static method, set true to this variable.
			 */
			void setMethodInfo(
				const char* dllPath,
				const char* className,
				const char* methodName,
				bool isStatic
			);

			/**
			 * @fn PLUGIN_FUNCTION* getFunction( bool ifloaded )
			 * @brief gets the pointer to the function that executes method.
			 * @param[in] ifloaded ifloaded flag
				  - true: If the DLL is not opened, this method returns NULL.
				  - false: If the DLL is not opened, it is opened and get the pointer of the function
			 * @return the pointer to the function
			 */
			PLUGIN_FUNCTION* getFunction( bool ifloaded );

			/**
			 * @fn kome::objects::Variant invoke( kome::objects::Parameters* params )
			 * @brief invoke method
			 * @param[in] params parameter information of calling method.
			 * @return return value of method
			 */
			kome::objects::Variant invoke( kome::objects::Parameters* params );

		public:
			/**
			 * @fn static ClrPluginFunction* createNewInstance()
			 * @brief create new instance
			 * @return the pointer to the new instance
			 */
			static ClrPluginFunction* createNewInstance();

		private:
			/** plug-in call ID counter */
			static unsigned int CLR_PLUGIN_CALL_ID_COUNTER;
		};
	}
}


#endif	// __KOME_CLR_PLUGIN_CALL_H__
