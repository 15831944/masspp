/**
 * @file MzmlDataHandler.h
 * @brief interfaces of MzmlDataHandler class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_DATA_HANDLER_H__
#define __KOME_IO_MZML_DATA_HANDLER_H__


#include "MzmlSample.h"


namespace kome {
	namespace io {
		namespace mzml {

			class MzmlSample;

			/**
			 * @class MzmlDataHandler
			 * @brief XML data handler to get spectrum data points
			 */
			class MzmlDataHandler : public kome::xml::XmlHandler {
			public:
				/**
				 * @fn MzmlDataHandler( MzmlSample& sample, const char* id, kome::core::XYData& xyData )
				 * @brief constructor
				 * @param sample sample
				 * @param id data ID
				 * @param xyData xy data object to store data points
				 */
				MzmlDataHandler( MzmlSample& sample, const char* id, kome::core::XYData& xyData );

				/**
				 * @fn virtual ~MzmlDataHandler()
				 * @brief destructor
				 */
				virtual ~MzmlDataHandler();

			protected:
				/** sample */
				MzmlSample& m_sample;

				/** ID */
				std::string m_id;

				/** xy data */
				kome::core::XYData& m_xyData;

				/** data length */
				int m_dataLength;

				/** reverse flag */
				bool m_reverseFlg;

			protected:
				/** array information */
				MzmlSample::ArrayInfo m_arrayInfo;

				/** available array flag */
				bool m_available;

				/** x array */
				double* m_xArray;

				/** y array */
				double* m_yArray;

			protected:
				/** reading flag */
				bool m_reading;

			protected:
				/**
				 * @fn void setPoints()
				 * @brief set data points
				 */
				void setPoints();

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

#endif		// __KOME_IO_MZML_DATA_HANDLER_H__
