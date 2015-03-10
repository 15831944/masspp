/**
 * @file MassBankInstrumentsHandler.h
 * @brief interfaces of MassBankInstrumentsHandler class
 *
 * @author S.Tanaka
 * @date 2009.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASBANK_INSTRUMENTS_HANDLER_H__
#define __KOME_MASBANK_INSTRUMENTS_HANDLER_H__


namespace kome {
	namespace massbank {

		/**
		 * @class MassBankInstrumentsHandler
		 * @brief MassBank result handler class
		 */
		class MassBankInstrumentsHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn MassBankInstrumentsHandler()
			 * @brief constructor
			 */
			MassBankInstrumentsHandler();

			/**
			 * @fn virtual ~MassBankInstrumentsHandler()
			 * @brief destructor
			 */
			virtual ~MassBankInstrumentsHandler();

		protected:
			/** values */
			std::vector<std::pair<std::string, std::string>> m_vals;

			/** tag name */
			std::string m_tagName;

		public:
			std::string doGetBatchId(const char* fname);
			/**
			 * @fn unsigned int getNumberOfValues()
			 * @brief gets the number of values
			 * @return the number of values
			 */
			unsigned int getNumberOfValues();
			
			/**
			 * @fn const char* getName( const unsigned int idx )
			 * @brief gets parameter names
			 * @param idx parameter index
			 * @return parameter name (If NULL, the index is illegal.)
			 */
			const char* getName( const unsigned int idx );

			/**
			 * @fn const char* getValue( const unsigned int idx )
			 * @brief gets parameter value
			 * @param idx parameter index
			 * @return parameter value (If NULL, the index is illegal.)
			 */
			const char* getValue( const unsigned int idx );

			/**
			 * @fn void getValues( const char* name, std::vector< std::string >& vals )
			 * @brief gets values of specified name
			 * @param name parameter name
			 * @param vals the array to store values
			 */
			void getValues( const char* name, std::vector< std::string >& vals );

			void getInValues(std::vector<std::string>& vals);

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


#endif	// __KOME_MASBANK_INSTRUMENTS_HANDLER_H__
