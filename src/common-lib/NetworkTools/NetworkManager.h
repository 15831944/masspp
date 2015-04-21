/**
 * @file NetworkManager.h
 * @brief interfaces of NetworkManager class
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_MANAGER_H__
#define __KOME_NETWORK_MANAGER_H__


#include <vector>


namespace kome {
	namespace net {

		class Connection;
		class HttpConnection;

		/**
		 * @class NetworkManager
		 * @brief network management class
		 */
		class NETWORK_TOOLS_CLASS NetworkManager {
		protected:
			/**
			 * @fn NetworkManager()
			 * @brief constructor
			 */
			NetworkManager();

			/**
			 * @fn virtual ~NetworkManager()
			 * @brief destructor
			 */
			virtual ~NetworkManager();

		protected:
			/** connections */
			std::vector< Connection* > m_connections;

			/** proxy server */
			std::string m_proxy;

			/** proxy pass */
			std::string m_pass;

		public:
			/**
			 * @fn void addConnection( Connection* con )
			 * @brief adds connection
			 * @param[in] con connection to be added
			 */
			void addConnection( Connection* con );

			/**
			 * @fn void removeConnection( Connection* con )
			 * @brief removes connection
			 * @param[in] con connection to be removed
			 */
			void removeConnection( Connection* con );

			/**
			 * @fn unsigned int getNumberOfConnections()
			 * @brief gets the number of connections
			 * @return the number of connections
			 */
			unsigned int getNumberOfConnections();

			/**
			 * @fn Connection* getConnection( const unsigned int idx )
			 * @brief gets the connection
			 * @param[in] idx connection index
			 * @return connection object (If NULL, the index is illegal.)
			 */
			Connection* getConnection( const unsigned int idx );

			/**
			 * @fn void deleteAllConnections( const bool onlyClosed )
			 * @brief deletes all connections
			 * @param[in] onlyClosed If true, only closed connections are deleted.
			 */
			void deleteAllConnections( const bool onlyClosed );

		public:
			/**
			 * @fn HttpConnection* getHttpConnection()
			 * @brief gets http connection
			 * @return HTTP connection
			 */
			HttpConnection* getHttpConnection();

		public:
			/**
			 * @fn void setProxyServer( const char* proxy )
			 * @brief sets the proxy server
			 * @param[in] proxy proxy
			 */
			void setProxyServer( const char* proxy );

			/**
			 * @fn const char* getProxyServer()
			 * @brief gets the proxy server
			 * @return proxy server
			 */
			const char* getProxyServer();

			/**
			 * @fn void setProxyPass( const char* pass )
			 * @brief sets the proxy pass
			 * @param[in] pass proxy pass
			 */
			void setProxyPass( const char* pass );

			/**
			 * @fn const char* getProxyPass()
			 * @brief gets the proxy pass
			 * @return proxy pass
			 */
			const char* getProxyPass();

		protected:
			/**
			 * @fn int searchConnection( Connection* con )
			 * @brief searches connection
			 * @param[in] con connection to be searched
			 * @return connection index (If negative value, specified connection does not exist.)
			 */
			int searchConnection( Connection* con );

		public:
			/**
			 * @fn static NetworkManager& getInstance()
			 * @brief gets the network manager object (This is the only object.)
			 * @return network manager object
			 */
			static NetworkManager& getInstance();

		};
	}
}


#endif	// __KOME_NETWORK_MANAGER_H__
