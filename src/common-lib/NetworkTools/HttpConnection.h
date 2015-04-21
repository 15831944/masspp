/**
 * @file HttpConnection.h
 * @brief interfaces of HttpConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_HTTP_CONNECTION_H__
#define __KOME_NETWORK_HTTP_CONNECTION_H__


#include "BasicConnection.h"


namespace kome {
	namespace net {

		/**
		 * @class HttpConnection
		 * @brief HTTP connection class
		 */
		class NETWORK_TOOLS_CLASS HttpConnection : public BasicConnection {
		public:
			/**
			 * @fn HttpConnection()
			 * @brief constructor
			 */
			HttpConnection();

			/**
			 * @fn virtual ~HttpConnection()
			 * @brief destructor
			 */
			virtual ~HttpConnection();

		public:
			/**
			 * @typedef HttpMethod
			 * @brief http method
			 */
			typedef enum {
				METHOD_GET,
				METHOD_POST,
				METHOD_MULTI_PART
			} HttpMethod;

			/**
			 * @struct ParamInfo
			 * @brief parameter information
			 */
			struct ParamInfo {
				std::string name;
				std::string value;
				bool isFile;
			};

		protected:
			/** url */
			std::string m_url;

			/** path */
			std::string m_path;

			/** headers */
			std::vector< std::pair< std::string, std::string > > m_headers;

			/** parameters */
			std::vector< ParamInfo > m_parameters;

			/** method */
			HttpMethod m_method;

			/** boundary */
			std::string m_boundary;

			/** user name */
			std::string m_user;

			/** password */
			std::string m_pass;

		public:
			/**
			 * @fn void setLoginInfo(
						const char* user = NULL,
						const char* pass = NULL
					)
			 * @brief sets login information
			 * @param[in] user user
			 * @param[in] pass password
			 */
			void setLoginInfo(
				const char* user,
				const char* pass
			);

			/**
			 * @fn void setPath( const char* path )
			 * @brief sets the path
			 * @param[in] path path
			 */
			void setPath( const char* path );

			/**
			 * @fn const char* getPath();
			 * @brief gets the path
			 * @return path
			 */
			const char* getPath();

		public:
			/**
			 * @fn void clearHeaders()
			 * @brief clears headers
			 */
			void clearHeaders();

			/**
			 * @fn void setHeader( const char* name, const char* value )
			 * @brief sets header value
			 * @param[in] name header name
			 * @param[in] value header value
			 */
			void setHeader( const char* name, const char* value );

			/**
			 * @fn unsigned int getNumberOfHeaders()
			 * @brief gets the number of headers
			 * @return the number of headers
			 */
			unsigned int getNumberOfHeaders();

			/**
			 * @fn const char* getHeaderName( const unsigned int idx )
			 * @brief gets header name
			 * @param[in] idx header index
			 * @return header name (If NULL, the index is illegal.)
			 */
			const char* getHeaderName( const unsigned int idx );

			/**
			 * @fn const char* getHeaderValue( const unsigned int idx )
			 * @brief gets header value
			 * @param[in] idx header index
			 * @return header value (If NULL, the index is illegal.)
			 */
			const char* getHeaderValue( const unsigned int idx );

			/**
			 * @fn const char* getHeaderValue( const char* name )
			 * @brief gets header value
			 * @param[in] name header name
			 * @return header value (If NULL, the specified header name does not exist.)
			 */
			const char* getHeaderValue( const char* name );

		public:
			/**
			 * @fn void clearParameters()
			 * @brief clears parameters
			 */
			void clearParameters();

			/**
			 * @fn void setParameter( const char* name, const char* value, const bool isFile )
			 * @brief sets parameter value
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 * @param[in] isFile If true, the parameter is file path
			 */
			void setParameter( const char* name, const char* value, const bool isFile = false );

			/**
			 * @fn void deleteParameter( const int idx )
			 * @brief deletes parameter
			 * @param idx parameter index
			 */
			void deleteParameter( const int idx );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn const char* getParameterName( const unsigned int idx )
			 * @brief gets parameter name
			 * @param[in] idx parameter index
			 * @return parameter name (If NULL, the index is illegal.)
			 */
			const char* getParameterName( const unsigned int idx );

			/**
			 * @fn const char* getParameterValue( const unsigned int idx )
			 * @brief gets parameter value
			 * @param[in] idx parameter index
			 * @return parameter value (If NULL, the index is illegal.)
			 */
			const char* getParameterValue( const unsigned int idx );

			/**
			 * @fn const char* getParameterValue( const char* name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value (If NULL, the specified parameter name does not exist.)
			 */
			const char* getParameterValue( const char* name );

		public:
			/**
			 * @fn const char* getBoundary()
			 * @brief gets the boundary text
			 * @return boundary text
			 */
			const char* getBoundary();

		protected:
			/**
			 * @fn int searchHeader( const char* name )
			 * @brief searches header
			 * @param[in] name header name
			 * @return header index (If negative value, the specified header name does not exist.)
			 */
			int searchHeader( const char* name );

			/**
			 * @fn int searchParameter( const char* name )
			 * @brief searches parameter
			 * @param[in] name parameter name
			 * @return parameter index (If negative value, the specified parameter name does not exist.)
			 */
			int searchParameter( const char* name );

		public:
			/**
			 * @fn void setMethod( const HttpMethod method )
			 * @brief sets HTTP method
			 * @param[in] method HTTP method
			 */
			void setMethod( const HttpMethod method );

			/**
			 * @fn HttpMethod getMethod()
			 * @brief gets HTTP method
			 * @return HTTP method
			 */
			HttpMethod getMethod();

		protected:
			/**
			 * @fn virtual void createGetRequest( kome::core::DataAccessor& req )
			 * @brief creates GET request
			 * @param[out] req request
			 */
			virtual void createGetRequest( kome::core::DataAccessor& req );

			/**
			 * @fn virtual void createPostRequest( kome::core::DataAccessor& req )
			 * @brief creates POST request
			 * @param[out] req request
			 */
			virtual void createPostRequest( kome::core::DataAccessor& req );

			/**
			 * @fn virtual void createMultiPartFormRequest( kome::core::DataAccessor& req )
			 * @brief creates multi-part form request
			 * @param[out] req request
			 */
			virtual void createMultiPartFormRequest( kome::core::DataAccessor& req );

			/**
			 * @fn virtual std::string getHeadersString()
			 * @brief gets the headers strinig
			 * @return headers string
			 */
			virtual std::string getHeadersString();

			/**
			 * @fn virtual std::string getParamsString()
			 * @brief gets the parameters string
			 * @return parameter string
			 */
			virtual std::string getParamsString();

			/**
			 * @fn virtual void getMultiPartParamsString( const char* path )
			 * @brief gets the parameters string when multi-part-form
			 * @param[in] path file path to write parameter string
			 */
			virtual void getMultiPartParamsString( const char* path );

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
			 * @brief This method is called by sendRequest method. (override method)
			 * @param[in] req request
			 * @param[out] res response
			 */
			virtual void onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res );
		};
	}
}


#endif	// __KOME_NETWORK_HTTP_CONNECTION_H__
