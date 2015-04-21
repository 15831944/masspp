/**
 * @file Wsdl.h
 * @brief interfaces of Wsdl class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_WSDL_H__
#define __KOME_NETWORK_WSDL_H__


#include <vector>


namespace kome {
	namespace net {

		class WsdlPortType;
		class WsdlBinding;
		class WsdlOperation;

		/**
		 * @class Wsdl
		 * @brief WSDL management class
		 */
		class NETWORK_TOOLS_CLASS Wsdl {
		public:
			/**
			 * @fn Wsdl()
			 * @brief constructor
			 */
			Wsdl();

			/**
			 * @fn virtual ~Wsdl()
			 * @brief destructor
			 */
			virtual ~Wsdl();

		protected:
			/** port types */
			std::vector< WsdlPortType* > m_portTypes;

			/** bindings */
			std::vector< WsdlBinding* > m_bindings;

		public:
			/**
			 * @fn void clear()
			 * @brief clears WSDL information
			 */
			void clear();

		public:
			/**
			 * @fn WsdlPortType* addPortType( const char* name )
			 * @brief adds port type
			 * @param[in] name port type name
			 * @return new port type object
			 */
			WsdlPortType* addPortType( const char* name );

			/**
			 * @fn WsdlPortType* searchPortType( const char* name )
			 * @brief searches port type which has specified name
			 * @param[in] name port type name
			 * @return port type object (If NULL, there is no port type which has specified name.)
			 */
			WsdlPortType* searchPortType( const char* name );

			/**
			 * @fn unsigned int getNumberOfPortTypes()
			 * @brief gets the number of port types
			 * @return the number of port types
			 */
			unsigned int getNumberOfPortTypes();

			/**
			 * @fn WsdlPortType* getPortType( const unsigned int idx )
			 * @brief gets port type
			 * @param[in] idx port type index
			 * @return port type object (If NULL, the index is illegal.)
			 */
			WsdlPortType* getPortType( const unsigned int idx );

		public:
			/**
			 * @fn WsdlBinding* addBinding( const char* name )
			 * @brief adds binding
			 * @param[in] name binding name
			 * @return new binding object
			 */
			WsdlBinding* addBinding( const char* name );

			/**
			 * @fn WsdlBinding* searchBinding( const char* name )
			 * @brief searches binding which has specified name
			 * @return binding object (If NULL, there is no binding which has specified name.)
			 */
			WsdlBinding* searchBinding( const char* name );

			/**
			 * @fn unsigned int getNumberOfBindings()
			 * @brief gets the number of bindings
			 * @return the number of bindings
			 */
			unsigned int getNumberOfBindings();

			/**
			 * @fn WsdlBinding* getBinding( const unsigned int idx )
			 * @brief gets binding object
			 * @param[in] idx binding index
			 * @return binding object (If NULL, the index is illegal.)
			 */
			WsdlBinding* getBinding( const unsigned int idx );

		public:
			/**
			 * @fn WsdlBinding* searchMethod( const char* method )
			 * @brief searches method
			 * @param[in] method method name
			 * @return binding object which includes specified method
			 */
			WsdlBinding* searchMethod( const char* method );

		public:
			/**
			 * @fn bool readWsdl( kome::core::DataAccessor& acc )
			 * @brief reads WSDL
			 * @param[in] acc reader accessor object to read WSDL
			 * @return If true, it succeeded to read WSDL
			 */
			bool readWsdl( kome::core::DataAccessor& acc );

		public:
			/**
			 * @fn static void getSoapResponse( kome::core::DataAccessor& acc, std::vector< char >& res )
			 * @brief gets soap response
			 * @param[in] acc data accessor object to read
			 * @param[out] res the array to store soap response
			 */
			static void getSoapResponse( kome::core::DataAccessor& acc, std::vector< char >& res );
		};
	}
}


#endif	// __KOME_NETWORK_WSDL_H__
