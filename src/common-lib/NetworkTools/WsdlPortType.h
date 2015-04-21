/**
 * @file WsdlPortType.h
 * @brief interfaces of WsdlPortType class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_WSDL_PORT_TYPE_H__
#define __KOME_NETWORK_WSDL_PORT_TYPE_H__


#include <string>
#include <vector>


namespace kome {
	namespace net {

		class WsdlOperation;
		class WsdlBinding;

		/**
		 * @class WsdlPortType
		 * @brief WSDL port type class
		 */
		class NETWORK_TOOLS_CLASS WsdlPortType {
		public:
			/**
			 * @fn WsdlPortType( const char* name )
			 * @brief constructor
			 * @param[in] name port type name
			 */
			WsdlPortType( const char* name );

			/**
			 * @fn virtual ~WsdlPortType()
			 * @brief destructor
			 */
			virtual ~WsdlPortType();

		protected:
			/** name */
			std::string m_name;

			/** operations */
			std::vector< WsdlOperation* > m_operations;

			/** bindings */
			std::vector< WsdlBinding* > m_bindings;

		public:
			/**
			 * @fn void clear()
			 * @brief clears port type
			 */
			void clear();

		public:
			/**
			 * @fn const char* getName()
			 * @brief gets port type name
			 * @return port type name
			 */
			const char* getName();

		public:
			/**
			 * @fn WsdlOperation* addOperation( const char* name )
			 * @brief adds operation
			 * @param[in] name operation name
			 * @return new operation object
			 */
			WsdlOperation* addOperation( const char* name );

			/**
			 * @fn WsdlOperation* searchOperation( const char* name )
			 * @brief searches operation which has specified name
			 * @param[in] name operation name
			 * @return operation object (If NULL, there is no operation which has specified name.)
			 */
			WsdlOperation* searchOperation( const char* name );

			/**
			 * @fn unsigned int getNumberOfOperations()
			 * @brief gets the number of operations
			 * @return the number of operations
			 */
			unsigned int getNumberOfOperations();

			/**
			 * @fn WsdlOperation* getOperation( const unsigned int idx )
			 * @brief gets operation object
			 * @param[in] idx operation index
			 * @return operation object (If NULL, the index is illegal.)
			 */
			WsdlOperation* getOperation( const unsigned int idx );

		public:
			/**
			 * @fn void addBinding( WsdlBinding* binding )
			 * @brief adds binding
			 * @param[in] binding biding object to be added
			 */
			void addBinding( WsdlBinding* binding );

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
		};
	}
}


#endif	// __KOME_NETWORK_WSDL_PORT_TYPE_H__
