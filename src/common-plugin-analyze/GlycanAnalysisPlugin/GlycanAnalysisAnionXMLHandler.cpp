/**
 * @file GlycanAnalysisAnionXMLHandler.cpp
 * @brief handler of Anion XML file
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "GlycanAnalysisAnionXMLHandler.h"

/** XML Tags in anion.xml (an) */
#define ANION_XML_TAG_AN	"an"

/** XML Tags in anion.xml (name) */
#define ANION_XML_TAG_NAME	"name"

/** XML Tags in anion.xml (mz) */
#define ANION_XML_TAG_MZ	"mz"

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
GlycanAnalysisAnionXMLHandler::GlycanAnalysisAnionXMLHandler(){
	
	m_bChkAn = false;

	m_anionList.clear();
	m_parsingAnion.dAnionMz = 0;
	m_parsingAnion.strAnionName = "";
}

// destructor
GlycanAnalysisAnionXMLHandler::~GlycanAnalysisAnionXMLHandler(){
	m_anionList.clear();
}

// check parameters
bool GlycanAnalysisAnionXMLHandler::isIncludeDefault(const char* strDefaultAnion){

	for(unsigned int i = 0;i < m_anionList.size();i++){
		if(strcmp(m_anionList.at(i).strAnionName.c_str(), strDefaultAnion) == 0){
			return true;
		}
	}
	return false;
}

// start document
void GlycanAnalysisAnionXMLHandler::onStartDocument() {
}

// end document
void GlycanAnalysisAnionXMLHandler::onEndDocument() {
}

// start element
void GlycanAnalysisAnionXMLHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {

	// null check
	if ( name == NULL || strcmp(name, "?xml") == 0) {
		return;
	}

	if( strcmp( name, ANION_XML_TAG_AN ) == 0 ) {	// <lab>
		// init setdata
		m_parsingAnion.dAnionMz = 0;
		m_parsingAnion.strAnionName = "";

		// set parse flag
		m_bChkAn = true;
	}

}

// end element
void GlycanAnalysisAnionXMLHandler::onEndElement( const char* name, const char* text ) {
	// null check
	if ( name == NULL || text == NULL ) {
		return;
	}

	if( strcmp( name, ANION_XML_TAG_AN ) == 0 ) {				// </lab>
		if ( m_bChkAn && m_bChkName && m_bChkMz ) {
			m_anionList.push_back(m_parsingAnion);
		}

		m_bChkAn = false;
		m_bChkName = false;
		m_bChkMz = false;
	}
	else if ( strcmp( name, ANION_XML_TAG_MZ ) == 0 ) {		// </mz>
		m_parsingAnion.dAnionMz = atof(text);
		m_bChkMz = true;
	}
	else if ( strcmp( name, ANION_XML_TAG_NAME ) == 0 ) {		// </name>
		m_parsingAnion.strAnionName = std::string(text);
		m_bChkName = true;
	}
}

// return anions
bool GlycanAnalysisAnionXMLHandler::getAnionList( std::vector< GLYCAN_ANION > *anionList ){

	bool ret = false;

	if(anionList == NULL){
		return ret;	//false
	}

	unsigned int unSize = m_anionList.size();

	for(unsigned int i = 0; i < unSize; i++){
		anionList->push_back(m_anionList.at(i));
	}

	if(unSize == anionList->size()){
		ret = true;
	}

	return ret;
}

// return mass of argument anion
double GlycanAnalysisAnionXMLHandler::getAnionMass(std::string strAnion){

	for(unsigned int i = 0;i < m_anionList.size();i++){
		if(strcmp(m_anionList.at(i).strAnionName.c_str(), strAnion.c_str()) == 0){
			return m_anionList.at(i).dAnionMz;
		}
	}
	return 0.0;
}

