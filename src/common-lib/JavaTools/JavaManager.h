/**
 * @file JavaManager.h
 * @brief interfaces of JavaManager class
 *
 * @author S.Tanaka
 * @date 2012.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_JAVA_MANAGER_H__
#define __KOME_JAVA_MANAGER_H__


#include <jni.h>


namespace kome {
	namespace java {

		/**
		 * @class JavaManager
		 * @brief Java management class
		 */
		class JAVA_TOOLS_CLASS JavaManager {
		protected:
			/**
			 * @fn JavaManager()
			 * @brief constructor
			 */
			JavaManager();

			/**
			 * @fn virtual ~JavaManager()
			 * @brief destructor
			 */
			virtual ~JavaManager();

		protected:
			/** Java VM */
			JavaVM* m_vm;

			/** JNI Env */
			JNIEnv* m_env;

			/** library */
			void* m_lib;

			/** JVM File Directory */
			std::string m_jvmDir;

		public:
			/**
			 * @fn const char* getJvmDir()
			 * @brief gets the JVM directory
			 * @return JVM directory
			 */
			const char* getJvmDir();

		protected:
			/**
			 * @fn void createJVM()
			 * @brief creates Java Virtual Machine
			 */
			void createJVM();

			/**
			 * @fn void destroyJVM()
			 * @brief destroys Java Virtual Machine
			 */
			void destroyJVM();

#ifdef _MSC_VER
		protected:
			/**
			 * @fn void createWindowsJVM( std::vector< JavaVMOption >& options )
			 * @brief creates Java Virtual Machine under the Windows environment.
			 * @param[in] options Java options			 
			 */
			void createWindowsJVM( std::vector< JavaVMOption >& options );
#else
		protected:
			/**
			 * @fn void createUnixJVM( std::vector< JavaVMOption >& options )
			 * @brief creates Java Virtual Machine under the Unix environment.
			 * @param[in] options Java options			 
			 */
			void createUnixJVM( std::vector< JavaVMOption >& options );

#endif    // _MSC_VER

		public:
			/**
			 * @fn JavaVM* getJVM()
			 * @brief gets Java Virtual Machine
			 * @return Java Virtual Machine
			 */
			JavaVM* getJVM();

			/**
			 * @fn JNIEnv* getEnvironment()
			 * @brief gets JNI Environment
			 */
			JNIEnv* getEnvironment();

		public:
			/**
			 * @fn std::string getString( jstring s )
			 * @brief converts java string to string
			 * @param[in] s java string
			 * @return converted string
			 */
			std::string getString( jstring s );

			/**
			 * @fn jstring getString( const char* s )
			 * @brief converts string to java string
			 * @param[in] s string
			 * @return java string
			 */
			jstring getString( const char* s );

		public:
			/**
			 * @fn std::string getException()
			 * @brief gets the exception string
			 * @return exception string
			 */
			std::string getException();

		public:
			/**
			 * @fn static JavaManager& getInstance()
			 * @brief gets the Java manager object (This is the only object.)
			 * @return Java manager object
			 */
			static JavaManager& getInstance();

		};
	}
}


#endif	// __KOME_JAVA_MANAGER_H__
