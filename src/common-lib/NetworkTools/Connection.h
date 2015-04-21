/**
 * @file Connection.h
 * @brief interfaces of Connection class
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_CONNECTION_H__
#define __KOME_NETWORK_CONNECTION_H__


#include <string>


namespace kome {
	namespace net {

		/**
		 * @class Connection
		 * @brief network connection abstract class
		 */
		class NETWORK_TOOLS_CLASS Connection : public kome::core::DataAccessor {
		public:
			/**
			 * @fn Connection()
			 * @brief constructor
			 */
			Connection();

			/**
			 * @fn virtual ~Connection()
			 * @brief destructor
			 */
			virtual ~Connection();

		protected:
			/** request file path */
			std::string m_reqPath;

			/** response file path */
			std::string m_resPath;

			/** tmp path */
			std::string m_tmpPath;

			/** open flag */
			bool m_opened;

			/** server */
			std::string m_server;

			/** port number */
			int m_port;

			/** reader */
			FILE* m_reader;

			/** response */
			kome::core::Buffer m_responseBuffer;

		protected:
			/** connection counter */
			static unsigned long m_counter;

		public:
			/**
			 * @fn void initRequest()
			 * @brief initializes request
			 */
			void initRequest();

			/**
			 * @fn virtual void writeToRequest( char* req, const unsigned int len )
			 * @brief writes to request
			 * @param[in] req request
			 * @param[in] len the length of request
			 */
			virtual void writeToRequest( char* req, const unsigned int len );

		public:
			/**
			 * @fn void getResponse( kome::core::DataAccessor& acc )
			 * @brief gets the response
			 * @param[out] acc data accessor object to store the response.
			 */
			void getResponse( kome::core::DataAccessor& acc );

			/**
			 * @fn void* getResponse()
			 * @brief gets the response
			 * @return response
			 */
			void* getResponse();

			/**
			 * @fn unsigned int getResponseLength()
			 * @brief gets the response length
			 * @return response length
			 */
			unsigned int getResponseLength();

		public:
			/**
			 * @fn const char* getServer()
			 * @brief gets the server name
			 * @return server name
			 */
			const char* getServer();

			/**
			 * @fn int getPort()
			 * @brief gets the port number
			 * @return port number
			 */
			int getPort();

		public:
			/**
			 * @fn bool openConnection( const char* server, const int port )
			 * @brief open the connection
			 * @param[in] server server name
			 * @param[in] port port number
			 * @return If true, opening the connection is succeeded
			 */
			bool openConnection( const char* server, const int port );

			/**
			 * @fn void closeConnection()
			 * @brief closes the connection
			 */
			void closeConnection();

			/**
			 * @fn virtual void sendRequest()
			 * @brief sends request
			 */
			virtual void sendRequest();

			/**
			 * @fn bool isOpened()
			 * @brief judges whether the connection is opened or not.
			 * @return If true, the connection is opened.
			 */
			bool isOpened();

		public:
			/**
			 * @fn void release()
			 * @brief releases connection
			 */
			void release();

		public:
			/**
			 * @fn virtual int write( void* addr, int size )
			 * @brief writes data (override method)
			 * @param[in] addr the pointer to data
			 * @param[in] size data size
			 * @return actually writing size
			 */
			virtual int write( void* addr, int size );

			/**
			 * @fn virtual int read( void* addr, int size )
			 * @brief reads data (override method)
			 * @param[out] addr the pointer to store data
			 * @param[in] size read size
			 * @return actually reading size
			 */
			virtual int read( void* addr, int size );

		protected:
			/**
			 * @fn virtual bool onOpen( const char* server, const int port ) = 0
			 * @brief This method is called by openConnection method (abstract method)
			 * @param[in] server server name
			 * @param[in] port port number
			 * @return If true, opening the connection is succeeded
			 */
			virtual bool onOpen( const char* server, const int port ) = 0;

			/**
			 * @fn virtual void onClose() = 0
			 * @brief This method is called by closeConnection method (abstract method)
			 */
			virtual void onClose() = 0;

			/**
			 * @fn virtual void onCreateRequest( kome::core::DataAccessor& req ) = 0
			 * @brief creates the request. (abstract method)
			 * @param[out] req request
			 */
			virtual void onCreateRequest( kome::core::DataAccessor& req ) = 0;

			/**
			 * @fn virtual void onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) = 0
			 * @brief This method is called by sendRequest method. (abstract method)
			 * @param[in] req request
			 * @param[out] res response
			 */
			virtual void onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) = 0;
		};
	}
}


#endif	// __KOME_NETWORK_CONNECTION_H__
