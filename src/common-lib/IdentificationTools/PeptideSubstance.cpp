/**
 * @file PeptideSubstance.h
 * @brief implements of PeptideSubstance class
 *
 * @author S.Tanaka
 * @date 2012.08.01
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "PeptideSubstance.h"
#include "Hit.h"
#include "SearchResult.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static members
const char* PeptideSubstance::PEP_ITEM_SEQUENCE = "Sequence";
const char* PeptideSubstance::PEP_ITEM_PREV_AA  = "Residue Before";
const char* PeptideSubstance::PEP_ITEM_NEXT_AA  = "Residue After";
const char* PeptideSubstance::PEP_ITEM_START    = "Start";
const char* PeptideSubstance::PEP_ITEM_END      = "End";


// constructor
PeptideSubstance::PeptideSubstance( SearchResult* pSrchResult, const char* accession, const char* name, Substance* parent )
		: Substance( pSrchResult, accession, name, TYPE_PEPTIDE, parent ) {
}

// destructor
PeptideSubstance::~PeptideSubstance() {
}

// set the sequence
void PeptideSubstance::setSequence( const char* sequence )
{
	m_sequence = NVL( sequence, "" );
	getProperties().setValue(PEP_ITEM_SEQUENCE, sequence);
	setName();
}

// get the sequence
std::string PeptideSubstance::getSequence( void )
{
	return getProperties().getStringValue(PEP_ITEM_SEQUENCE, "");
}

// set the residue before
void PeptideSubstance::setResidueBefore( const char* residueBefore )
{
	m_before = NVL( residueBefore, "" );
	getProperties().setValue(PEP_ITEM_PREV_AA, residueBefore);
	setName();
}

// get the residue before
std::string PeptideSubstance::getResidueBefore( void )
{
	return getProperties().getStringValue(PEP_ITEM_PREV_AA, "");
}

// set the residue after
void PeptideSubstance::setResidueAfter( const char* residueAfter )
{
	m_after = NVL( residueAfter, "" );
	getProperties().setValue(PEP_ITEM_NEXT_AA, residueAfter);
	setName();
}

// get the residue after
std::string PeptideSubstance::getResidueAfter( void )
{
	return getProperties().getStringValue(PEP_ITEM_NEXT_AA, "");
}

// set the start index
void PeptideSubstance::setStart( const int start )
{
	getProperties().setIntValue(PEP_ITEM_START, start);
}

// get the start index
int PeptideSubstance::getStart( void )
{
	return getProperties().getIntValue(PEP_ITEM_START, -1);
}

// set the end index
void PeptideSubstance::setEnd( const int end )
{
	getProperties().setIntValue(PEP_ITEM_END, end);
}

// get the end index
int PeptideSubstance::getEnd( void )
{
	return getProperties().getIntValue(PEP_ITEM_END, -1);
}

// add modification
void PeptideSubstance::addModification( const char* modification, const int pos ) {
	Modification m;
	m.mod = NVL( modification, "" );
	m.pos = pos;

	m_mods.push_back( m );

	setName();
}

// name
void PeptideSubstance::setName() {
	m_name = createPeptideName( m_sequence.c_str(), m_before.c_str(), m_after.c_str(), m_mods );
}

// create name
std::string PeptideSubstance::createPeptideName( const char* seq, const char* before, const char* after, std::vector< Modification >& mods ){
	// return value
	std::string name;

	// check the sequence
	std::string seqStr = NVL( seq, "" );
	if( seqStr.empty() ) {
		return name;
	}

	// sequence
	char buff[ 4096 ];
	sprintf( buff, "%s", seqStr.c_str() );

	std::set< int > modPosSet;
	std::set< std::string > modSet;
	std::string modStr;
	for( unsigned int i = 0; i < mods.size(); i++ ) {
		std::string m = mods[ i ].mod;
		int pos = mods[ i ].pos;

		if( pos < 0 ) {
			pos = 0;
		}
		if( pos >= (int)seqStr.length() ) {
			pos = (int)seqStr.length() - 1;
		}
		modPosSet.insert( pos );

		if( modSet.find( m ) == modSet.end() ) {
			modSet.insert( m );
			modStr = FMT( "%s + %s", modStr.c_str(), m.c_str() );
		}
	}

	for( unsigned int i = 0; i < seqStr.length(); i++ ) {
		if( modPosSet.find( i ) != modPosSet.end() ) {
			buff[ i ] = (char)tolower( (int)seq[ i ] );
		}
	}

	name = FMT( "%s.%s.%s", before, seq, after );
	if( !modStr.empty() ) {
		name = FMT( "%s%s", buff, modStr.c_str() );
	}

	return name;
}
