/**
 * @file ProxyHttpConnection.h
 * @brief interfaces of ProxyHttpConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NET_PROXY_HTTP_CONNECTION_H__
#define __KOME_NET_PROXY_HTTP_CONNECTION_H__


#include "HttpConnection.h"

#include <boost/function.hpp>


namespace kome {
	namespace net {

		/**
		 * @class ProxyHttpConnection
		 * @brief proxy connection management class
		 */
		class NETWORK_TOOLS_CLASS ProxyHttpConnection : public HttpConnection {
		public:
			/**
			 * @fn ProxyHttpConnection()
			 * @brief constructor
			 */
			ProxyHttpConnection();

			/**
			 * @fn ProxyHttpConnection( boost::function< bool ( std::string&, std::string& ) > fun )
			 * @brief constructor
			 * @param[in] fun the function to get user & password
			 */
			ProxyHttpConnection( boost::function< bool ( std::string&, std::string& ) > fun );

			/**
			 * @fn virtual ~ProxyHttpConnection()
			 * @brief destructor
			 */
			virtual ~ProxyHttpConnection();

		protected:
			/** user name */
			std::string m_user;

			/** password */
			std::string m_pass;

			/** the function to get user & password */
			boost::function< bool ( std::string&, std::string& ) > m_getUserFun;

		public:
			/**
			 * @fn void setProxy(
						const char* server,
						const int port,
						const char* user = NULL,
						const char* pass = NULL
					)
			 * @brief sets proxy information
			 * @param[in] server proxy server
			 * @param[in] port proxy port
			 * @param[in] user proxy useer
			 * @param[in] pass proxy password
			 */
			void setProxy(
				const char* server,
				const int port,
				const char* user = NULL,
				const char* pass = NULL
			);

		protected:
			/**
			 * @fn virtual std::string getHeadersString()
			 * @brief gets the headers string (override method)
			 * @return headers string
			 */
			virtual std::string getHeadersString();

		public:
			/**
			 * @fn virtual bool openUrl( const char* url )
			 * @brief opens connection by URL
			 * @param[in] url URL
			 * @return If true, it succeeded to open connection.
			 */
			virtual bool openUrl( const char* url );

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

		public:
			/**
			 * @fn virtual void sendRequest()
			 * @brief sends request
			 */
			virtual void sendRequest();
		};
	}
}


#endif	// __KOME_NET_PROXY_HTTP_CONNECTION_H__
