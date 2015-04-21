/**
 * @file WinHttpConnection.h
 * @brief interfaces of WinHttpConnection class
 *
 * @author S.Tanaka
 * @date 2013.06.03
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_NETWORK_WIN_HTTP_CONNECTION_H__
#define __KOME_NETWORK_WIN_HTTP_CONNECTION_H__


#include "HttpConnection.h"


namespace kome {
	namespace net {

		class HttpConnection;

		/**
		 * @class WinHttpConnection
		 * @brief Windows HTTP connection class
		 */
		class NETWORK_TOOLS_CLASS WinHttpConnection : public HttpConnection {
		public:
			/**
			 * @fn WinHttpConnection()
			 * @brief constructor
			 */
			WinHttpConnection();

			/**
			 * @fn virtual ~WinHttpConnection()
			 * @brief destructor
			 */
			virtual ~WinHttpConnection();

		protected:
			/** proxy server */
			std::string m_proxyServer;

			/** proxy user */
			std::string m_proxyUser;

			/** proxy password */
			std::string m_proxyPassword;

			/** proxy exception */
			std::string m_proxyException;

		protected:
			/** base connection */
			HttpConnection* m_baseCon;

		protected:			
			/**
			 * @fn int getCastLength( const char* castChar )
			 * @brief get cast length
			 * @param castChar cast char
			 * @return cast length
			 */
			int getCastLength( const char* castChar );

			/**
			 * @fn int getCastLength( wchar_t* castChar )
			 * @brief get cast length
			 * @param[in] castChar cast char
			 * @return cast length
			 */
			int getCastLength( wchar_t* castChar );

			/**
			 * @fn void castWChar( const char* castChar, wchar_t* conveWchar, int nLength )
			 * @brief char -> wchar_t
			 * @param[in] castChar char string
			 * @param[out] conveWchar the buffer to store char string
			 * @param[in] nLength buffer lenght
			 */
			void castWChar( const char* castChar, wchar_t* conveWchar, int nLength );

			/**
			 * @fn void castChar( wchar_t* castChar, char* conveChar, int nLength )
			 * @brief wchar_t -> char
			 * @param[in] castChar wchar_t string
			 * @param[out] conveChar the buffer to store char string
			 * @param[in] nLength buffer length
			 */
			void castChar( wchar_t* castChar, char* conveChar, int nLength );

		public:
			/**
			 * @fn void setProxy( const char* server, const char* e )
			 * @brief sets the proxy information
			 * @param[in] server proxy server
			 * @param[in] e proxy exception
			 */
			void setProxy( const char* server, const char* e );

			/**
			 * @fn const char* getProxyServer()
			 * @brief gets the proxy server
			 * @return proxy server
			 */
			const char* getProxyServer();

			/**
			 * @fn const char* getProxyException()
			 * @brief gets the proxy exception
			 * @return proxy exception
			 */
			 const char* getProxyException();

			/**
			 * @fn void setProxyAccount( const char* user, const char* pass )
			 * @brief sets the proxy account
			 * @param[in] user user name
			 * @param[in] pass password
			 */
			void setProxyAccount( const char* user, const char* pass );

			/**
			 * @fn void getProxyAccount( std::string& user, std::string& pass )
			 * @brief gets proxy account
			 * @param[out] user string object to store user name
			 * @param[out] pass string object to store password
			 */
			void getProxyAccount( std::string& user, std::string& pass );

			/**
			 * @fn bool showAuthorizationDialog( std::string& user, std::string& pass )
			 * @brief shows authorization dialog
			 * @param[out] user string object to store user name
			 * @param[out] pass string object to store password
			 * @return If true, user input user name and password successfully.
			 */
			bool showAuthorizationDialog( std::string& user, std::string& pass );

			/**
			 * @fn DWORD chooseAuthScheme( DWORD supportedSchemes )
			 * @brief chooses authorization scheme
			 * @param[in] supportedSchemes supported schemes
			 * @return chosen scheme
			 */
			DWORD chooseAuthScheme( DWORD supportedSchemes );

		protected:
			/**
			 * @fn bool isProxy( const char* url )
			 * @brief check the specified url should be connected with proxy or not.
			 * @param[in] url url
			 * @return If true, the specified url should be connected with proxy.
			 */
			bool isProxy( const char* url );

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


#endif	// __KOME_NETWORK_WIN_HTTP_CONNECTION_H__
