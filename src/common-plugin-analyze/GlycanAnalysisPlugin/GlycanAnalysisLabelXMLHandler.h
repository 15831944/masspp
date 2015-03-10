/**
 * @file GlycanAnalysisLabelXMLHandler.h
 * @brief handler of Labeling XML file
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_LABELING_XML_HANDLER_H__
#define __KOME_GLYCAN_ANALYSIS_LABELING_XML_HANDLER_H__

#include "stdafx.h"
#include "GlycanAnalysisStructures.h"

namespace kome {
		namespace glycan {

			/**
			 * @class GlycanAnalysisLabelXMLHandler
			 * @brief XML handler class to read labeling xml file
			 */
			class GlycanAnalysisLabelXMLHandler : public kome::xml::XmlHandler {
			public:
				
				/**
				 * @fn GlycanAnalysisLabelXMLHandler()
				 * @brief constructor
				 */
				GlycanAnalysisLabelXMLHandler();

				/**
				 * @fn virtual ~GlycanAnalysisLabelXMLHandler()
				 * @brief destructor
				 */
				virtual ~GlycanAnalysisLabelXMLHandler();
			
				/**
				 * @fn bool getLabelList( std::vector< GLYCAN_LABEL > *labelList )
				 * @params[out] vector of Label
				 * @return if false, labelList is NULL
				 */
				bool getLabelList( std::vector< GLYCAN_LABEL > *labelList );

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
				double getLabelMass(std::string strLabel);

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
				
				/** Label List */
				std::vector< GLYCAN_LABEL >m_labelList;

				/** Parsing Label */
				GLYCAN_LABEL m_parsingLabel;

				/** parsing flag for <lab> */
				bool m_bChkLab;

				/** parsed flag for <name> */
				bool m_bChkName;

				/** parsed flag for <mz> */
				bool m_bChkMz;

			};
		}
}

#endif	//__KOME_GLYCAN_ANALYSIS_LABELING_XML_HANDLER_H__