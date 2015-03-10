/**
 * @file MzXMLSpectrumDataHandler.h
 * @brief interfaces of MzXMLSpectrumDataHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_SPECTRUM_DATA_HANDLER_H__
#define __KOME_IO_MZXML_SPECTRUM_DATA_HANDLER_H__


namespace kome {
	namespace io {
		namespace mzxml {

			class MzXmlSpectrum;

			/**
			 * @class MzXMLSpectrumDataHandler
			 * @brief XML data handler to get spectrum data points
			 */
			class MzXMLSpectrumDataHandler : public kome::xml::XmlHandler {
			public:
				/**
				 * @fn MzXMLSpectrumDataHandler( MzXmlSpectrum& spec, kome::core::XYData& xyData )
				 * @brief constructor
				 * @param spec spectrum object
				 * @param xyData xy data object to store data points
				 */
				MzXMLSpectrumDataHandler( MzXmlSpectrum& spec, kome::core::XYData& xyData );

				/**
				 * @fn virtual ~MzXMLSpectrumDataHandler()
				 * @brief destructor
				 */
				virtual ~MzXMLSpectrumDataHandler();

			protected:
				/** spectrum object */
				MzXmlSpectrum& m_spec;

				/** xy data */
				kome::core::XYData& m_xyData;

				/** peaks count */
				unsigned int m_peaksCount;

				/** data array */
				union {
					float* flt;
					double* dbl;
				} m_dataArray;

				/** precision */
				unsigned int m_precision;

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

#endif		// __KOME_IO_MZXML_SPECTRUM_DATA_HANDLER_H__
