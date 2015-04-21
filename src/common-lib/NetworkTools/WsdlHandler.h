/**
 * @file WsdlHandler.h
 * @brief interfaces of WsdlHandler class
 *
 * @author S.Tanaka
 * @date 2009.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_WSDL_HANDLER_H__
#define __KOME_NETWORK_WSDL_HANDLER_H__


namespace kome {
	namespace net {

		class Wsdl;
		class WsdlPortType;
		class WsdlBinding;
		class WsdlOperation;

		/**
		 * @class WsdlHandler
		 * @brief handler class to read WSDL file
		 */
		class NETWORK_TOOLS_CLASS WsdlHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn WsdlHandler( Wsdl& wsdl )
			 * @brief constructor
			 * @param[in] wsdl object
			 */
			WsdlHandler( Wsdl& wsdl );

			/**
			 * @fn virtual ~WsdlHandler()
			 * @brief destructor
			 */
			virtual ~WsdlHandler();

		protected:
			/** WSDL object */
			Wsdl& m_wsdl;

		protected:
			/** current port type */
			WsdlPortType* m_currType;

			/** current binding */
			WsdlBinding* m_currBinding;

			/** current operation */
			WsdlOperation* m_currOperation;

			/** top binding flag */
			bool m_topBin;

			/** top operation flag */
			bool m_topOp;

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
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text );
		};
	}
}


#endif	// __KOME_NETWORK_WSDL_HANDLER_H__
