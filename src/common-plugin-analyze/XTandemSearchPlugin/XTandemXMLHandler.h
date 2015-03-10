/**
 * @file XTandemXMLHandler.h
 * @brief interfaces of XTandemXMLHandler class
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IO_XTANDEM_HANDLER_H__
#define __KOME_IO_XTANDEM_HANDLER_H__


namespace kome {
		namespace xtandem {
			/**
			 * @class XTandemXMLHandler
			 * @brief XML handler class to read xtandem output xml file
			 */
			class XTandemXMLHandler : public kome::xml::XmlHandler {
			public:
				/**
				 * @fn XTandemXMLHandler(
						kome::ident::SearchResult& result,
						kome::ident::SearchInfo& searchInfo,
						kome::ident::SpecInfo** spectra,
						const unsigned int specNum
					)
				 * @brief constructor
				 * @param[out] result result object
				 * @param[in] db database name
				 * @param[in] spectra the array of spectrum
				 * @param[in] specNum the size of spectrum array
				 */
				XTandemXMLHandler(
					kome::ident::SearchResult& result,
					kome::ident::SearchInfo& searchInfo,
					kome::ident::SpecInfo** spectra,
					const unsigned int specNum
				);

				/**
				 * @fn virtual ~XTandemXMLHandler()
				 * @brief destructor
				 */
				virtual ~XTandemXMLHandler();

			protected:
				/** result */
				kome::ident::SearchResult& m_result;

				/** search information */
				kome::ident::SearchInfo& m_searchInfo;

				/** spectra */
				std::map< std::string, kome::ident::SpecInfo* > m_specMap;

				/** modifications */
				std::vector< kome::ident::Modification* > m_mods;

				/** regex */
				std::string m_regex;

			protected:
				/** protein flag */
				bool m_inProtein;

				/** peptide flag */
				bool m_inPeptide;

				/** group depth */
				int m_groupDepth;

				/** protein map */
				std::map< std::string, kome::ident::ProteinSubstance* > m_proteinMap;

				/** peptide map */
				std::map< std::string, kome::ident::PeptideSubstance* > m_peptideMap;

				/** protein group */
				struct ProteinGroup {
					double mh;
					double expect;
					double sumI;
					double maxI;
					double fI;
				} m_proteinGroup;

				/** protein information */
				struct ProteinInfo {
					double expect;
					double sumI;
				} m_proteinInfo;

				/** current protein */
				std::vector< kome::ident::ProteinSubstance* > m_currProteins;

				/** current peptide */
				kome::ident::PeptideSubstance* m_currPeptide;

				/** current Hit */
				std::vector< kome::ident::Hit* > m_currHits;

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


#endif		// __KOME_IO_XTANDEM_HANDLER_H__
