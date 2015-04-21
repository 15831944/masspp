/**
 * @file Substance.cpp
 // implementation of Substance class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"
#include "SearchResult.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Substance::Substance( SearchResult* pSrchResult, const char * accession, const char* name, const SubstanceType type, Substance* parent )
	: m_pSrchResult(pSrchResult), m_accession(accession), m_name(name), m_type(type), m_parent(parent)
{
	m_pSrchResult->addSubstance( this );
}

// destructor
Substance::~Substance( void )
{
	m_pSrchResult->removeSubstance( this );
}

// get the accession name
std::string Substance::getAccession( void ) const
{
	return m_accession;
}

// get the substance name
std::string Substance::getName( void ) const
{
	return m_name;
}

// get the substance type
SubstanceType Substance::getType( void ) const
{
	return m_type;
}

// get the parent substance (e.g. peptide -> protein)
Substance* Substance::getParentSubstance( void ) const
{
	return m_parent;
}

// set the parent substance (e.g. peptide -> protein)
void Substance::setParentSubstance( Substance* parent )
{
	m_parent = parent;
}

// get the number of child substances (e.g. protein -> peptides)
int Substance::getNumberOfChildSubstances( void ) const
{
	int nChildCount = 0;
	for (int i = 0; i < m_pSrchResult->getNumberOfSubstances(); i++)
	{
		if (m_pSrchResult->getSubstance(i)->getParentSubstance() == this)
		{
			nChildCount++;
		}
	}

	return nChildCount;
}

// get the child substances (e.g. protein -> peptide)
void Substance::getChildSubstances( std::vector<Substance*>& substanceList ) const
{
	substanceList.clear();
	for (int i = 0; i < m_pSrchResult->getNumberOfSubstances(); i++)
	{
		if (m_pSrchResult->getSubstance(i)->getParentSubstance() == this)
		{
			substanceList.push_back(m_pSrchResult->getSubstance(i));
		}
	}
}

// get the substance properties object
kome::core::Properties& Substance::getProperties( void )
{
	return m_properties;
}
