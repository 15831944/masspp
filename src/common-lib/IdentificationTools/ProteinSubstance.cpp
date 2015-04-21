/**
 * @file ProteinSubstance.h
 * @brief implements of ProteinSubstance class
 *
 * @author S.Tanaka
 * @date 2012.08.01
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "ProteinSubstance.h"
#include "Hit.h"
#include "SearchResult.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// substance
const char* ProteinSubstance::PROT_ITEM_MW      = "Protein Mass";


// constructor
ProteinSubstance::ProteinSubstance( SearchResult* pSrchResult, const char* accession, const char* name, Substance* parent )
		: Substance( pSrchResult, accession, name, TYPE_PROTEIN, parent ) {
}

// destructor
ProteinSubstance::~ProteinSubstance() {
}

// set mass
void ProteinSubstance::setMass( const double mass ) {
	getProperties().setDoubleValue( PROT_ITEM_MW, mass );
}

// get mass
double ProteinSubstance::getMass() {
	double mass = getProperties().getDoubleValue( PROT_ITEM_MW, -1.0 );
	return mass;
}
