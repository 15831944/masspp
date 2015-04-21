/**
 * @file BasicConnection.h
 * @brief interfaces of BasicConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_BASIC_CONNECTION_H__
#define __KOME_NETWORK_BASIC_CONNECTION_H__


#include "Connection.h"

#include <windows.h>
#include <winhttp.h>
#include <tchar.h>


namespace kome {
	namespace net {

		/**
		 * @class BasicConnection
		 * @brief basic network connection class
		 */
		class NETWORK_TOOLS_CLASS BasicConnection : public Connection {
		public:
			/**
			 * @fn BasicConnection()
			 * @brief constructor
			 */
			BasicConnection();

			/**
			 * @fn virtual ~BasicConnection()
			 * @brief destructor
			 */
			virtual ~BasicConnection();

		protected:
			/** socket */
			int m_soc;

			/** url */
			std::string m_url;

			/** session */
			HINTERNET  m_hSession;

			/** connection */
			HINTERNET  m_hConnect;

			/** request */
			HINTERNET  m_hRequest;

		
		protected:
			/**
			 * @fn virtual bool onOpen( const char* server, const int port )
			 * @brief This method is called by openConnection method (override method)
			 * @param[in] server server name
			 * @param[in] port port number
			 * @return If true, opening the connection is succeeded
			 */
			virtual bool onOpen( const char* server, const int port );

			/**
			 * @fn virtual void onClose()
			 * @brief This method is called by closeConnection method (override method)
			 */
			virtual void onClose();

			/**
			 * @fn virtual void onCreateRequest( kome::core::DataAccessor& req )
			 * @brief creates the request. (override method)
			 * @param[out] req request
			 */
			virtual void onCreateRequest( kome::core::DataAccessor& req );

			/**
			 * @fn virtual void onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res )
			 * @brief This method is called by sendRequest method. (abstract method)
			 * @param[in] req request
			 * @param[out] res response
			 */
			virtual void onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res );

		};
	}
}


#endif	// __KOME_NETWORK_BASIC_CONNECTION_H__
