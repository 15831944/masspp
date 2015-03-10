/**
 * @file Accession.cpp
 * @brief interfaces of Accession class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Accession.h"


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Accession::Accession( const char* id, const char* name ) {
	m_id = NVL( id, "" );
	m_name = NVL( name, "" );
	m_parent = NULL;
	m_origin = NULL;
}

// constructor
Accession::Accession( const char* id ) {
	m_id = NVL( id, "" );
	m_parent = NULL;
	m_origin = NULL;
}

// destructor
Accession::~Accession() {
}

// get ID
const char* Accession::getId() {
	return m_id.c_str();
}

// set name
void Accession::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get name
const char* Accession::getName() {
	return m_name.c_str();
}

// set parent accession
void Accession::setParent( Accession* parent ) {
	m_parent = parent;
}

// get parent accession
Accession* Accession::getParent() {
	return m_parent;
}

// get origin
Accession* Accession::getOrigin() {
	// check members
	if( m_parent == NULL ) {
		return this;
	}

	if( m_origin != NULL ) {
		return m_origin;
	}

	// search origin
	Accession* origin = this;

	std::set< Accession* > accSet;

	do {
		accSet.insert( origin );
		origin = origin->getParent();
	} while( origin != NULL && origin->getParent() != NULL
			&& accSet.find( origin->getParent() ) == accSet.end() );

	m_origin = NVL( origin, this );

	return m_origin;
}
