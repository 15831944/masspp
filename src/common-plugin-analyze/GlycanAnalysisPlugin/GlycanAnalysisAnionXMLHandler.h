/**
 * @file GlycanAnalysisAnionXMLHandler.h
 * @brief handler of Anion XML file
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_ANION_XML_HANDLER_H__
#define __KOME_GLYCAN_ANALYSIS_ANION_XML_HANDLER_H__

#include "stdafx.h"
#include "GlycanAnalysisStructures.h"

namespace kome {
		namespace glycan {

			/**
			 * @class GlycanAnalysisAnionXMLHandler
			 * @brief XML handler class to read Anion xml file
			 */
			class GlycanAnalysisAnionXMLHandler : public kome::xml::XmlHandler {
			public:
				
				/**
				 * @fn GlycanAnalysisAnionXMLHandler()
				 * @brief constructor
				 */
				GlycanAnalysisAnionXMLHandler();

				/**
				 * @fn virtual ~GlycanAnalysisAnionXMLHandler()
				 * @brief destructor
				 */
				virtual ~GlycanAnalysisAnionXMLHandler();
			
				/**
				 * @fn bool getAnionNameList( GLYCAN_ANION *anionList )
				 * @brief return anions
				 * @param[out] vector of Anion
				 * @return if false, anionList is NULL.
				 */
				bool getAnionList( std::vector< GLYCAN_ANION > *anionList );

				/**
				 * @fn bool isIncludeDefault()
				 * @brief check parameters
				 * @param[in] default parameter
				 * @return if include default, return true
				 */
				bool isIncludeDefault(const char* strDefaultAnion);

				/**
				 * @fn bool isIncludeDefault()
				 * @brief check parameters
				 * @param[in] default parameter
				 * @return if include default, return true
				 */
				double getAnionMass(std::string strAnion);

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

			protected:

				/** Anion List  */
				std::vector< GLYCAN_ANION > m_anionList;

				/** Parsing Anion */
				GLYCAN_ANION m_parsingAnion;

				/** parsing flag for <an> */
				bool m_bChkAn;

				/** parsed flag for <name> */
				bool m_bChkName;

				/** parsed flag for <mz> */
				bool m_bChkMz;

			};
		}
}

#endif	//__KOME_GLYCAN_ANALYSIS_ANION_XML_HANDLER_H__