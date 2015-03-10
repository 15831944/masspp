/**
 * @file MassBankJobStatusHandler.h
 * @brief interfaces of MassBankJobStatusHandler class
 *
 * @author S.Tanaka
 * @date 2010.06.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASBANK_JOB_STATUS_HANDLER_H__
#define __KOME_MASBANK_JOB_STATUS_HANDLER_H__


namespace kome {
	namespace massbank {

		/**
		 * @class MassBankJobStatusHandler
		 * @brief MassBank result handler class
		 */
		class MassBankJobStatusHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn MassBankJobStatusHandler()
			 * @brief constructor
			 */
			MassBankJobStatusHandler();

			/**
			 * @fn virtual ~MassBankJobStatusHandler()
			 * @brief destructor
			 */
			virtual ~MassBankJobStatusHandler();

		protected:
			/** status */
			int m_status;

			/** date */
			std::string m_date;

		public:
			/**
			 * @fn int getStatus()
			 * @brief gets status
			 * @return status
			 */
			int getStatus();

			/**
			 * @fn const char* getDate()
			 * @brief gets date
			 * @return date
			 */
			const char* getDate();

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


#endif	// __KOME_MASBANK_JOB_STATUS_HANDLER_H__
