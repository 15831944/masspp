/**
 * @file MassBankRecordHandler.h
 * @brief interfaces of MassBankRecordHandler class
 *
 * @author S.Tanaka
 * @date 2009.09.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASBANK_RECORD_HANDLER_H__
#define __KOME_MASBANK_RECORD_HANDLER_H__


namespace kome {
	namespace massbank {

		/**
		 * @class MassBankRecordHandler
		 * @brief MassBank result handler class
		 */
		class MassBankRecordHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn MassBankRecordHandler()
			 * @brief constructor
			 */
			MassBankRecordHandler();

			/**
			 * @fn virtual ~MassBankRecordHandler()
			 * @brief destructor
			 */
			virtual ~MassBankRecordHandler();
		public:
			void doGetRecordInfo(const char* fname,const char* id,std::string& info);
		protected:
			/** records */
			std::vector< std::pair< std::string, std::string > > m_records;

		public:
			/**
			 * @fn unsigned int getNumberOfRecords()
			 * @brief gets the number of records
			 * @return the number of records
			 */
			unsigned int getNumberOfRecords();

			/**
			 * @fn const char* getId( const unsigned int idx )
			 * @brief gets record ID
			 * @param idx record index
			 * @return record ID (If NULL, the index is illegal.)
			 */
			const char* getId( const unsigned int idx );

			/**
			 * @fn const char* getRecordInfo( const unsigned int idx )
			 * @brief gets record information
			 * @param idx record index
			 * @return record information (If NULL, the index is illegal.)
			 */
			const char* getRecordInfo( const unsigned int idx );

			/**
			 * @fn const char* getRecordInfo( const char* id )
			 * @brief gets record information
			 * @param id record ID
			 * @return record information (If NULL, thre record ID is illegal.)
			 */
			const char* getRecordInfo( const char* id );

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


#endif	// __KOME_MASBANK_RECORD_HANDLER_H__
