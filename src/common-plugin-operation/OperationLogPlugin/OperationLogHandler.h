/**
 * @file OperationLogHandler.h
 * @brief interfaces of Operation Log Handler class
 *
 * @author M.Izumi
 * @date 2013.07.24
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_LOG_HANDLER_H__
#define __KOME_OPERATION_LOG_HANDLER_H__

namespace kome { 
	namespace operation {
		
		class XmlHandler;

		/**
		 * @class OperationLogHandler
		 * @brief operation log xml handler class
		 */
		class OperationLogHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn OperationLogHandler()
			 * @brief constructor
			 */
			OperationLogHandler();

			/**
			 * @fn virtual ~OperationLogHandler()
			 * @brief destructor
			 */
			virtual ~OperationLogHandler();

		protected:
			/** operation type */
			int m_optType;
			/** operation state */
			int m_optState;
			/** operation name */
			std::string m_name;
			/** short name */
			std::string m_shortName;
			/** description */
			std::string m_desc;
			/** parameter */
			std::string m_param;
			/** before data */
			char* m_beforeData;
			/** before data size */
			int m_beforeSize;
			/** after data */
			char* m_afterData;
			/** after data size */
			int m_afterSize;
			/** update path */
			std::string m_upDatepath;
			/** prev path */
			std::string m_prevDatapath;
			/** temp path */
			std::string m_tmpPath;
		protected:
			/**
			 * @fn void Initialize()
			 * @brief initialize
			 */
			void Initialize();

			/**
			 * @fn void setOperation( kome::operation::Operation* opt )
			 * @brief set operation
			 * @param operation
			 */
			void setOperation( kome::operation::Operation* opt );

			/**
			 * @fn void writeData( kome::operation::Operation* opt )
			 * @brief write binary data
			 * @param operation
			 */
			void writeData( kome::operation::Operation* opt );

			/**
			 * @fn void deleteBinaryData()
			 * @brief delete binary data
			 */
			void deleteBinaryData();

			/**
			 * @fn void getBinaryData( const char* name, const char* text )
			 * @brief get binary data
			 * @param name tag name
			 * @param text body text
			 */
			void getBinaryData( const char* name, const char* text );
		protected:
			/**
			 * @fn virtual void onStartDocument()
			 * @brief This method is called by startDocument method. (override method)
			 */
			virtual void onStartDocument();

			/**
			 * @fn virtual void onEndDocument()
			 * @brief This method is called by endDocument method. (override method)
			 */
			virtual void onEndDocument();

			/**
			 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs )
			 * @brief This method is called by startElement method. (override method)
			 * @param name tag name
			 * @param attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param name tag name
			 * @param text body text
			 */
			virtual void onEndElement( const char* name, const char* text );

		};
	}
}

#endif	// __KOME_OPERATION_LOG_HANDLER_H__
