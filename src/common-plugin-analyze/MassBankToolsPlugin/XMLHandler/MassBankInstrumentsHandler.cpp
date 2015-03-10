/**
 * @file MassBankInstrumentsHandler.cpp
 * @brief implements of MassBankInstrumentsHandler class
 *
 * @author S.Tanaka
 * @date 2009.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "MassBankInstrumentsHandler.h"

#include "constXMLTagName.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MassBankInstrumentsHandler::MassBankInstrumentsHandler()
{
}

// destructor
MassBankInstrumentsHandler::~MassBankInstrumentsHandler()
{
}

// get Batch Job ID from Handler
std::string MassBankInstrumentsHandler::doGetBatchId(
	const char* fname
){
	parse(fname);

	// job ID
	std::string id("");
	if( !isError() ) {
		if( getNumberOfValues() > 0 ) {
			id = getValue( 0 );
		}
	}

	// delete temporary file
	removefile(fname);
	return id;
}

// get the number of values
unsigned int MassBankInstrumentsHandler::getNumberOfValues() 
{
	return m_vals.size();
}

// get parameter name
const char* MassBankInstrumentsHandler::getName(
	const unsigned int idx
) {
	if( idx >= m_vals.size() ) {
		return NULL;
	}
	return m_vals[ idx ].first.c_str();
}

// get parameter value
const char* MassBankInstrumentsHandler::getValue(
	const unsigned int idx
) {
	if( idx >= m_vals.size() ) {
		return NULL;
	}
	return m_vals[ idx ].second.c_str();
}

// get values
void MassBankInstrumentsHandler::getValues(
	const char* name, 
	std::vector< std::string >& vals
) {
	if(name != NULL) {
		for each(auto val in m_vals) {
			if(val.first.compare(name) == 0) {
				vals.push_back(val.second);
			}
		}
	}
}

void MassBankInstrumentsHandler::getInValues(
	std::vector<std::string>& vals
){
	if (!this->isError()) {
		for each(const auto& x in m_vals) {
			vals.push_back(x.second);
		}
	}
}

// on start document
void MassBankInstrumentsHandler::onStartDocument() 
{
	m_vals.clear();
	m_tagName.clear();
}

// on end document
void MassBankInstrumentsHandler::onEndDocument() 
{
	m_tagName.clear();
}

// on start element
void MassBankInstrumentsHandler::onStartElement(
	const char* name, 
	kome::core::Properties& attrs
) {
	// set tagname
	if(XML::TAG::RETURN.compare(name) != 0 ) {
		m_tagName = name;
	}
}

// on end element
void MassBankInstrumentsHandler::onEndElement(
	const char* name, 
	const char* text
) {
	if( XML::TAG::RETURN.compare(name) == 0) {
		m_vals.push_back(std::make_pair(m_tagName, text));
	}
	else {
		m_tagName.clear();
	}
}
