/**
 * @file GlycanAnalysisLabelXMLHandler.cpp
 * @brief handler of Labeling XML file
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "GlycanAnalysisLabelXMLHandler.h"

/** XML Tags int label.xml (lab)*/
#define LABEL_XML_TAG_LAB	"lab"

/** XML Tags int label.xml (name)*/
#define LABEL_XML_TAG_NAME	"name"

/** XML Tags int label.xml (mz)*/
#define LABEL_XML_TAG_MZ	"mz"

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
GlycanAnalysisLabelXMLHandler::GlycanAnalysisLabelXMLHandler(){
	
	m_bChkLab = false;

	m_labelList.clear();
	m_parsingLabel.dLabelMz = 0;
	m_parsingLabel.strLabelName = "";
}

// destructor
GlycanAnalysisLabelXMLHandler::~GlycanAnalysisLabelXMLHandler(){
	m_labelList.clear();
}

// start document
void GlycanAnalysisLabelXMLHandler::onStartDocument() {
}

// end document
void GlycanAnalysisLabelXMLHandler::onEndDocument() {
}

// check parameters
bool GlycanAnalysisLabelXMLHandler::isIncludeDefault(const char* strDefaultAnion){

	for(unsigned int i = 0;i < m_labelList.size();i++){
		if(strcmp(m_labelList.at(i).strLabelName.c_str(), strDefaultAnion) == 0){
			return true;
		}
	}
	return false;
}

// start element
void GlycanAnalysisLabelXMLHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {

	// null check
	if ( name == NULL || strcmp(name, "?xml") == 0) {
		return;
	}

	if( strcmp( name, LABEL_XML_TAG_LAB ) == 0 ) {	// <lab>
		// init setdata
		m_parsingLabel.dLabelMz = 0;
		m_parsingLabel.strLabelName = "";

		// set parse flag
		m_bChkLab = true;
	}

}

// end element
void GlycanAnalysisLabelXMLHandler::onEndElement( const char* name, const char* text ) {
	// null check
	if ( name == NULL || text == NULL ) {
		return;
	}

	if( strcmp( name, LABEL_XML_TAG_LAB ) == 0 ) {				// </lab>
		if ( m_bChkLab && m_bChkName && m_bChkMz ) {
			m_labelList.push_back(m_parsingLabel);	
		}

		m_bChkLab = false;
		m_bChkName = false;
		m_bChkMz = false;
	}
	else if ( strcmp( name, LABEL_XML_TAG_MZ ) == 0 ) {		// </mz>
		m_parsingLabel.dLabelMz = atof(text);
		m_bChkMz = true;
	}
	else if ( strcmp( name, LABEL_XML_TAG_NAME ) == 0 ) {		// </name>
		m_parsingLabel.strLabelName = std::string(text);
		m_bChkName = true;
	}
}

//  return labels
bool GlycanAnalysisLabelXMLHandler::getLabelList( std::vector< GLYCAN_LABEL > *labelList ){

	bool ret = false;

	if(labelList == NULL){
		
		return ret;	//false
	}

	unsigned int unSize = m_labelList.size();

	for(unsigned int i = 0; i < unSize; i++){
		labelList->push_back(m_labelList.at(i));
	}

	if(unSize == labelList->size()){
		ret = true;
	}

	return ret;
}

// return mass of argument label
double GlycanAnalysisLabelXMLHandler::getLabelMass(std::string strLabel){

	for(unsigned int i = 0;i < m_labelList.size();i++){
		if(strcmp(m_labelList.at(i).strLabelName.c_str(), strLabel.c_str()) == 0){
			return m_labelList.at(i).dLabelMz;
		}
	}
	return 0.0;
}
