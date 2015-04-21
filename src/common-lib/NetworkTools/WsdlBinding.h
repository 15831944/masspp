/**
 * @file WsdlBinding.h
 * @brief interfaces of WsdlBinding class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_WSDL_BINDING_H__
#define __KOME_NETWORK_WSDL_BINDING_H__


#include <vector>


namespace kome {
	namespace net {

		class WsdlOperation;

		/**
		 * @class WsdlBinding
		 * @brief WSDL binding class
		 */
		class NETWORK_TOOLS_CLASS WsdlBinding {
		public:
			/**
			 * @fn WsdlBinding( const char* name )
			 * @brief constructor
			 * @param[in] name binding name
			 */
			WsdlBinding( const char* name );

			/**
			 * @fn virtual ~WsdlBinding()
			 * @brief destructor
			 */
			virtual ~WsdlBinding();

		protected:
			/** binding name */
			std::string m_name;

			/** end point */
			std::string m_endPoint;

			/** operations */
			std::vector< std::pair< WsdlOperation*, std::string > > m_operations;

		public:
			/**
			 * @fn const char* getName()
			 * @brief gets binding name
			 * @return binding name
			 */
			const char* getName();

		public:
			/**
			 * @fn void setEndPoint( const char* endPoint )
			 * @brief sets end point
			 * @param[in] endPoint end point
			 */
			void setEndPoint( const char* endPoint );

			/**
			 * @fn const char* getEndPoint()
			 * @brief gets end point
			 * @return end point
			 */
			const char* getEndPoint();

		public:
			/**
			 * @fn void addOperation( WsdlOperation* operation, const char* action )
			 * @brief adds operation
			 * @param[in] operation operation object
			 * @param[in] action action name
			 */
			void addOperation( WsdlOperation* operation, const char* action );

			/**
			 * @fn unsigned int getNumberOfOperations()
			 * @brief gets the nubmer of operations
			 * @return the number of operations
			 */
			unsigned int getNumberOfOperations();

			/**
			 * @fn int searchOperation( const char* name )
			 * @brief searches operation
			 * @param[in] name operation name
			 * @return operation index (If negative value, specified opeartion name is not found.)
			 */
			int searchOperation( const char* name );

			/**
			 * @fn WsdlOperation* getOperation( const unsigned int idx )
			 * @brief gets operation
			 * @param[in] idx operation index
			 * @return operation object (If NULL, the index is illegal.)
			 */
			WsdlOperation* getOperation( const unsigned int idx );

			/**
			 * @fn const char* getAction( const unsigned int idx )
			 * @brief gets action name
			 * @param[in] idx operation index
			 * @return action name
			 */
			const char* getAction( const unsigned int idx );
		};
	}
}


#endif	// __KOME_NETWORK_WSDL_BINDING_H__
