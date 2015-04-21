/**
 * @file Modification.cpp
 * @brief implementation of Modification class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "Modification.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Modification::Modification( const char* name, const double monomass, const char* aminoAcidResidue, const bool hidden )
	: m_name(name), m_monomass(monomass), m_aminoAcidResidue(aminoAcidResidue), m_hidden( hidden )
{
	m_id = -1;
}

// destructor
Modification::~Modification( void )
{
}

// get the modification name
std::string Modification::getName( void ) const
{
	return m_name;
}

// set mono-isotopic m/z value
void Modification::setMonoMass( const double monoMass ) {
	m_monomass = monoMass;
}

// get the mono-isotopic m/z value
double Modification::getMonoMass( void ) const
{
	return m_monomass;
}

// set the amino acid residue
void Modification::setAminoAcidResidue( const char* aar ) {
	m_aminoAcidResidue = NVL( aar, "" );
}

// get the amino acid residue
std::string Modification::getAminoAcidResidue( void ) const
{
	return m_aminoAcidResidue;
}

// get hidden flag valuer
bool Modification::isHidden() {
	return m_hidden;
}

// set modification ID
void Modification::setId( const int id ) {
	m_id = id;
}

// get modification ID
int Modification::getId() {
	return m_id;
}

/// set hidden flag value
void Modification::setHidden( const bool hidden ) {
	m_hidden = hidden;
}
