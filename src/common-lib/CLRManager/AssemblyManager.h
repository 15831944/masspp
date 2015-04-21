/**
 * @file AssemblyManager.h
 * @brief interfaces of AssemblyManager class
 *
 * @author S.Tanaka
 * @date 2006.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_ASSEMBLY_MANAGER_H__
#define __KOME_CLR_ASSEMBLY_MANAGER_H__


namespace kome {
	namespace clr {

		/**
		 * @class AssemblyManager
		 * @brief assembly management class
		 */
		ref class AssemblyManager {
		protected:

			/**
			 * @fn AssemblyManager()
			 * @brief constructor
			 */
			AssemblyManager();

			/**
			 * @fn virtual ~AssemblyManager()
			 * @brief destructor
			 */
			virtual ~AssemblyManager();

		protected:
			/** Assembly object (This is the only object.) */
			static AssemblyManager^ m_instance = nullptr;

			/** Assembly hash table */
			System::Collections::Hashtable m_assemblyTable;

			/** Type hash table */
			System::Collections::Hashtable m_typeTable;

			/** method hash table */
			System::Collections::Hashtable m_methodTable;

		public:
			/**
			 * @fn System::Reflection::Assembly^ getAssembly( const char* dllPath )
			 * @brief get assembly
			 * @param[in] dllPath dll file path
			 * @return assembly (If specified DLL path is not opened, this method returns nullptr.)
			 */
			System::Reflection::Assembly^ getAssembly( const char* dllPath );

			/**
			 * @fn System::Reflection::Assembly^ loadAssembly( const char* dllPath )
			 * @brief loads assembly
			 * @param[in] dllPath dll file path
			 * @return assembly
			 */
			System::Reflection::Assembly^ loadAssembly( const char* dllPath );

			/**
			 * @fn void setType( unsigned int id, System::Type^ type )
			 * @brief sets type information
			 * @param[in] id plug-in call method ID
			 * @param[in] type type information
			 */
			void setType( unsigned int id, System::Type^ type );			

			/**
			 * @fn System::Type^ getType( unsigned int id )
			 * @brief gets type information
			 * @param[in] id plug-in call method ID
			 * @return type information
			 */
			System::Type^ getType( unsigned int id );

			/**
			 * @fn void setMethod( unsigned int id, System::Reflection::MethodInfo^ method )
			 * @brief sets method information
			 * @param[in] id plug-in call method ID
			 * @param[in] method method information
			 */
			void setMethod( unsigned int id, System::Reflection::MethodInfo^ method );

			/**
			 * @fn System::Reflection::MethodInfo^ getMethod( unsigned int id )
			 * @brief gets method information
			 * @param[in] id plug-in call method ID
			 * @return method information
			 */
			System::Reflection::MethodInfo^ getMethod( unsigned int id );

			/**
			 * @fn static AssemblyManager^ getInstance()
			 * @brief get AssemblyManager object
			 * @return AssemblyManager object
			 */
			static AssemblyManager^ getInstance();
		};
	}
}

#endif	// __KOME_CLR_ASSEMBLY_MANAGER_H__
