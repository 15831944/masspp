/**
 * @file Enzyme.cpp
 * @brief implementation of Enzyme class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "Enzyme.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Enzyme::Enzyme( const char* name ) : m_name(name)
{
	m_id = -1;
	m_hidden = false;
}

// destructor
Enzyme::~Enzyme( void )
{
}

// get the enzyme name
std::string Enzyme::getName( void ) const
{
	return m_name;
}

// set enzyme ID
void Enzyme::setId( const int id ) {
	m_id = id;
}

// get enzyme ID
int Enzyme::getId() {
	return m_id;
}

// clear cutters
void Enzyme::clearCutters() {
	m_cutters.clear();
}

// add cutter
void Enzyme::addCutter( TermType term, const char* cleave, const char* restrict ) {
	m_cutters.resize( m_cutters.size() + 1 );
	m_cutters.back().term = term;
	m_cutters.back().cleave = NVL( cleave, "" );
	m_cutters.back().restrict = NVL( restrict, "" );
}

// get number of cutters
unsigned int Enzyme::getNumberOfCutters() {
	return m_cutters.size();
}

// get cutter
Enzyme::Cutter* Enzyme::getCutter( const unsigned int idx ) {
	if( idx >= m_cutters.size() ) {
		return NULL;
	}

	return &( m_cutters[ idx ] );
}

// set hidden
void Enzyme::setHidden( const bool hidden ) {
	m_hidden = hidden;
}

// get hidden
bool Enzyme::isHidden() {
	return m_hidden;
}
