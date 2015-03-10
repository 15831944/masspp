/**
 * @file OperationXmlHandler.h
 * @brief interfaces of Operation Xml Handler class
 *
 * @author OKADA, H
 * @date 2013/08/15
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_XML_HANDLER_H__
#define __KOME_OPERATION_XML_HANDLER_H__

namespace kome { 
	namespace operation {
		
		class XmlHandler;

		/**
		 * @class OperationXmlHandler
		 * @brief operation log xml handler class
		 */
		class OperationXmlHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn OperationXmlHandler()
			 * @brief constructor
			 */
			OperationXmlHandler();

			/**
			 * @fn virtual ~OperationXmlHandler()
			 * @brief destructor
			 */
			virtual ~OperationXmlHandler();

		protected:

		public:
			// 構造体定義
			typedef struct XmlCommand{
				/** operation type */
				int optType;
				
				/** operation state */
				int optState;
				
				/** operation name */
				std::string name;
				
				/** short name */
				std::string shortName;
				
				/** description */
				std::string desc;
				
				/** parameter */
				std::string param;
				
				/** before data */
				char* beforeData;
				
				/** before data size */
				int beforeSize;
				
				/** after data */
				char* afterData;
				
				/** after data size */
				int afterSize;
				
				/** update path */
				std::string upDatepath;
				
				/** prev path */
				std::string prevDatapath;

				/** temp path */
				std::string tmpPath;
			}XmlCommand_t;
			
		protected:
			// 作業用構造体
			XmlCommand_t m_xmlcommand;
			
			// データ格納配列
			std::vector< XmlCommand_t > m_xmlcommands;
			
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


			/**
			 * @fn void OperationXmlHandler::clearCommand()
			 * @brief This method is clear working structure
			 */
			void OperationXmlHandler::clearCommand();

		public:
			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param name tag name
			 * @param text body text
			 */
			void getXmlCommands( std::vector< kome::operation::OperationXmlHandler::XmlCommand_t >& xmlcommands );

		};
	}
}

#endif	// __KOME_OPERATION_XML_HANDLER_H__
