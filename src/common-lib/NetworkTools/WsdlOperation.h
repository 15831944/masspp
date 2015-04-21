/**
 * @file WsdlOperation.h
 * @brief interfaces of WsdlOperation class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_WSDL_OPERATION_H__
#define __KOME_NETWORK_WSDL_OPERATION_H__


#include <vector>


namespace kome {
	namespace net {

		/**
		 * @class WsdlOperation
		 * @brief WSDL operation class
		 */
		class NETWORK_TOOLS_CLASS WsdlOperation {
		public:
			/**
			 * @fn WsdlOperation( const char* name )
			 * @brief constructor
			 * @param[in] name operation name
			 */
			WsdlOperation( const char* name );

			/**
			 * @fn virtual ~WsdlOperation()
			 * @brief destructor
			 */
			virtual ~WsdlOperation();

		protected:
			/** operation name */
			std::string m_name;

		public:
			/**
			 * @fn const char* getName()
			 * @brief gets operation name
			 * @return operation name
			 */
			const char* getName();


		};
	}
}


#endif	// __KOME_NETWORK_WSDL_OPERATION_H__
