/**
 * @file MzXMLHandler.h
 * @brief interfaces of MzXMLHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_HANDLER_H__
#define __KOME_IO_MZXML_HANDLER_H__


#include <map>


namespace kome {
	namespace io {
		namespace mzxml {

			class MzXmlSample;
			class MzXmlSpectrum;

			/**
			 * @class MzXMLHandler
			 * @brief XML handler class to read mzXML file
			 */
			class MzXMLHandler : public kome::xml::XmlHandler {
			public:
				/**
				 * @fn MzXMLHandler( MzXmlSample& file )
				 * @brief constructor
				 * @param file mzXML file object
				 */
				MzXMLHandler( MzXmlSample& file );

				/**
				 * @fn virtual ~MzXMLHandler()
				 * @brief destructor
				 */
				virtual ~MzXMLHandler();

			protected:
				/** mzXML file */
				MzXmlSample& m_file;

				/** scan number map */
				std::map< unsigned int, MzXmlSpectrum* > m_scanNumberMap;

				/** current spectrum */
				MzXmlSpectrum* m_currentSpec;

				/** position */
				long long m_pos;

				/** current data info */
				struct DataInfo {
					int num;
					int precision;
				} m_currentData;

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
}


#endif		// __KOME_IO_MZXML_HANDLER_H__
