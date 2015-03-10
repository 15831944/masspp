/**
 * @file MassBankPeaksHandler.h
 * @brief interfaces of MassBankPeaksHandler class
 *
 * @author S.Tanaka
 * @date 2009.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASBANK_PEAKS_HANDLER_H__
#define __KOME_MASBANK_PEAKS_HANDLER_H__


namespace kome {
	namespace massbank {

		/**
		 * @class MassBankPeaksHandler
		 * @brief MassBank result handler class
		 */
		class MassBankPeaksHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn MassBankPeaksHandler()
			 * @brief constructor
			 */
			MassBankPeaksHandler();

			/**
			 * @fn virtual ~MassBankPeaksHandler()
			 * @brief destructor
			 */
			virtual ~MassBankPeaksHandler();

		public:
			/**
			 * @struct PeakInfo
			 * @brief peak information
			 */
			struct PeakInfo{
				std::string id;
				std::vector< double > mzs;
				std::vector< double > ints;
				unsigned int num;
			};

		protected:
			/** peaks */
			std::vector< PeakInfo > m_peaks;

		public:
			void doGetXYDatas(const char* fname, const char* id, kome::core::XYData& xyData);
		private:
			/**
			 * @fn unsigned int getNumberOfRecords()
			 * @brief gets the number of records
			 * @return the number of records
			 */
			unsigned int getNumberOfRecords();

			/**
			 * @fn PeakInfo* getPeakInfo( const unsigned int idx )
			 * @brief gets peak information
			 * @param idx record index
			 * @return peak information (If NULL, the index is illegal.)
			 */
			PeakInfo* getPeakInfo( const unsigned int idx );

			/** 
			 * @fn PeakInfo* getPeakInfo( const char* id )
			 * @brief gets peak information
			 * @param id record ID
			 * @return peak information (If NULL, the record ID is illegal.)
			 */
			PeakInfo* getPeakInfo( const char* id );

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


#endif	// __KOME_MASBANK_PEAKS_HANDLER_H__
