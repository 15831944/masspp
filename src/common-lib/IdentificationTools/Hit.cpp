/**
 * @file Hit.cpp
 // implementation of Hit class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



const char* Hit::ITEM_HIT_RANK              = "Rank";
const char* Hit::ITEM_NUM_TOT_PROTEINS      = "Protein Count";
const char* Hit::ITEM_NUM_MATCHED_IONS      = "Matched Ion Count";
const char* Hit::ITEM_TOT_NUM_IONS          = "Ion Count";
const char* Hit::ITEM_CALC_NEUTRAL_PEP_MASS = "Calculated MR";
const char* Hit::ITEM_MASSDIFF              = "Delta";
const char* Hit::ITEM_NUM_TOL_TERM          = "Termini Consistent Count";
const char* Hit::ITEM_NUM_MISSED_CLEAVAGES  = "Missed Cleavages Count";
const char* Hit::ITEM_IS_REJECTED           = "Rejected";
const char* Hit::ITEM_CALC_PI               = "pI";
const char* Hit::ITEM_PROTEIN_EXPECT        = "proteinexpect";
const char* Hit::ITEM_PEPTIDE_EXPECT        = "expect";
const char* Hit::ITEM_PROTEIN_SCORE         = "proteinscore";
const char* Hit::ITEM_PEPTIDE_SCORE         = "ionscore";

//
// SpecInfo
//

// constructor
SpecInfo::SpecInfo() {
	clearInfo();
}

// destructor
SpecInfo::~SpecInfo() {
}

void SpecInfo::clearInfo(){
	id = -1;
	specId = -1;
	sampleIndex = -1;
	sampleName = "";
	path = "";
	retentionTime = -1.0;
	precursor = -1.0;
	stage = -1;
	polarity = kome::objects::Spectrum::POLARITY_UNKNOWN;
	peakIds.clear();
	charges.clear();
	name = "";
	peaks = -1;
}

//
// Hit
//

// constructor
Hit::Hit( SearchResult* pSrchResult, Substance* pSubstance ) :
		m_pSrchResult(pSrchResult), m_pSubstance(pSubstance)
{
	m_spectrumInfoList.clear();
	m_arrValueInfo.clear();
	m_charge = -1;
	m_id = -1;
}

// destructor
Hit::~Hit( void )
{
	m_pSubstance = NULL;
}

// set the identified substance information
void Hit::setSubstance( Substance * sub )
{
	m_pSubstance = sub;
}

// get the identified substance information
Substance* Hit::getSubstance( void ) const
{
	return m_pSubstance;
}

// add alternative substance
void Hit::addAltSubstance( Substance* sub, const char* before, const char* after, int start, int end ) {
	if( sub != NULL && sub != m_pSubstance ) {
		AltSubstanceInfo as;
		as.substance = sub;
		as.startPos = start;
		as.endPos = end;
		as.before = NVL( before, "" );
		as.after = NVL( after, "" );
		m_altSubstances.push_back( as );
	}
}

int Hit::getID() {
	return m_id;
}

void Hit::setID(const int id) {
	m_id = id;
}

// get the number of alternative substance
int Hit::getNumberOfAltSubstances() {
	return (int)m_altSubstances.size();
}

// get the alternative substance
Substance* Hit::getAltSubstance( const int index ) {
	if( index < 0 || index >= (int)m_altSubstances.size() ) {
		return NULL;
	}

	return m_altSubstances[ index ].substance;
}

// get the alternative substance start
int Hit::getAltSubstanceStartPos( const int index ) {
	if( index < 0 || index >= (int)m_altSubstances.size() ) {
		return -1;
	}

	return m_altSubstances[ index ].startPos;
}

// get the alternative substance end
int Hit::getAltSubstanceEndPos( const int index ) {
	if( index < 0 || index >= (int)m_altSubstances.size() ) {
		return -1;
	}

	return m_altSubstances[ index ].endPos;
}

// get the alternative substance before
const char* Hit::getAltSubstanceBefore( const int index ) {
	if( index < 0 || index >= (int)m_altSubstances.size() ) {
		return NULL;
	}

	return m_altSubstances[ index ].before.c_str();
}

// get the alternative substance after
const char* Hit::getAltSubstanceAfter( const int index ) {
	if( index < 0 || index >= (int)m_altSubstances.size() ) {
		return NULL;
	}

	return m_altSubstances[ index ].after.c_str();
}

// add a hit spectrum
void Hit::addSpectrum( SpecInfo* spectrumInfo )
{
	if( spectrumInfo != NULL ) {
		m_spectrumInfoList.push_back(spectrumInfo);
	}
}

// get the number of hit spectra
int Hit::getNumberOfSpectra( void ) const
{
	return m_spectrumInfoList.size();
}

// get the specified hit spectrum
SpecInfo* Hit::getSpectrum( const int index )
{
	SpecInfo* ret = NULL;
	
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_spectrumInfoList.size())) );

	if( index < static_cast<int>(m_spectrumInfoList.size()) ) {
		ret = m_spectrumInfoList[ index ];
	}
	return ret;
}

// clear modification
void Hit::clearModifications() {
	m_mods.clear();
}

// add modificaiton
void Hit::addModification( Modification* m, const int pos ) {
	m_mods.resize( m_mods.size() + 1 );
	m_mods.back().mod = m;
	m_mods.back().pos = pos;
}

// get the number of modifications
unsigned int Hit::getNumberOfModifications() {
	return m_mods.size();
}

// get modification
Modification* Hit::getModification( const unsigned int idx ) {
	if( idx >= m_mods.size() ) {
		return NULL;
	}

	return m_mods.at( idx ).mod;
}

// get modification position
int Hit::getModificationPos( const unsigned int idx ) {
	if( idx >= m_mods.size() ) {
		return -1;
	}

	return m_mods.at( idx ).pos;
}

// add hit property information
void Hit::addValue( const char* propName, const char* propValue, const ValueType propValueType )
{
	_ASSERT(strlen(propName) > 0);

	if (strlen(propName) > 0)
	{
		kome::ident::ValueInfo* pValInfo = NULL;

		// First of all, check if a value with this name already exists.
		pValInfo = getValueInfo(propName);

		// If the value doesn't already exist, add it.
		if (!pValInfo)
		{
			m_arrValueInfo.resize( m_arrValueInfo.size() + 1 );
			pValInfo = &m_arrValueInfo.back();
		}

		pValInfo->name = propName;
		pValInfo->value = propValue;
		pValInfo->valueType = propValueType;
	}
}

// get the number of hit property values
int Hit::getNumberOfValues( void ) const
{
	return m_arrValueInfo.size();
}

// get the specified hit property value name
std::string Hit::getValueName( const int index ) const
{
	std::string ret = "";
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrValueInfo.size())) );

	if( index < static_cast<int>(m_arrValueInfo.size()) ) {
		ret = m_arrValueInfo[ index ].name;
	}
	return ret;
}

// get the specified hit property value
std::string Hit::getValue( const int index ) const
{
	std::string ret = "";
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrValueInfo.size())) );

	if( index < static_cast<int>(m_arrValueInfo.size()) ) {
		ret = m_arrValueInfo[ index ].value;
	}
	return ret;
}

// get the specified hit property value type
ValueType Hit::getValueType( const int index ) const
{
	ValueType ret = TYPE_PROPERTY;
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrValueInfo.size())) );

	if( index < static_cast<int>(m_arrValueInfo.size()) ) {
		ret = m_arrValueInfo[ index ].valueType;
	}
	return ret;
}

// search for the specified hit value
ValueInfo* Hit::getValueInfo( const char* name )
{
	ValueInfo* pValInfo = NULL;

	for (int i = 0; (i < static_cast<int>(m_arrValueInfo.size())) && (!pValInfo); i++)
	{
		if (m_arrValueInfo[i].name.compare(name) == 0)
		{
			pValInfo = &m_arrValueInfo[i];
		}
	}

	return pValInfo;
}

// set the rank
void Hit::setRank( const int rank )
{
	addValue(ITEM_HIT_RANK, FMT("%d", rank).c_str(), TYPE_PROPERTY);
}

// set charge
void Hit::setCharge( const int charge ) {
	m_charge = charge;
}

// get charge
int Hit::getCharge() {
	return m_charge;
}

// get the rank
int Hit::getRank( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_HIT_RANK);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the protein count
void Hit::setProteinCount( const int proteincount )
{
	addValue(ITEM_NUM_TOT_PROTEINS, FMT("%d", proteincount).c_str(), TYPE_PROPERTY);
}

// get the protein count
int Hit::getProteinCount( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_NUM_TOT_PROTEINS);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the matched ion count
void Hit::setMatchedIonCount( const int matchedioncount )
{
	addValue(ITEM_NUM_MATCHED_IONS, FMT("%d", matchedioncount).c_str(), TYPE_PROPERTY);
}

// get the matched ion count
int Hit::getMatchedIonCount( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_NUM_MATCHED_IONS);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the ion count
void Hit::setIonCount( const int ioncount )
{
	addValue(ITEM_TOT_NUM_IONS, FMT("%d", ioncount).c_str(), TYPE_PROPERTY);
}

// get the ion count
int Hit::getIonCount( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_TOT_NUM_IONS);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the calculated MR
void Hit::setCalculatedMR( const double calculatedMR )
{
	addValue(ITEM_CALC_NEUTRAL_PEP_MASS, FMT("%lf", calculatedMR).c_str(), TYPE_PROPERTY);
}

// get the calculated MR
double Hit::getCalculatedMR( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_CALC_NEUTRAL_PEP_MASS);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}

// set the delta
void Hit::setDelta( const double delta )
{
	addValue(ITEM_MASSDIFF, FMT("%lf", delta).c_str(), TYPE_PROPERTY);
}

// get the delta
double Hit::getDelta( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_MASSDIFF);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}

// set the termini consistent count
void Hit::setTerminiConsistentCount( const int terminiConsistentCount )
{
	addValue(ITEM_NUM_TOL_TERM, FMT("%d", terminiConsistentCount).c_str(), TYPE_PROPERTY);
}

// get the termini consistent count
int Hit::getTerminiConsistentCount( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_NUM_TOL_TERM);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the missed cleavages count
void Hit::setMissedCleavagesCount( const int missedcleavagescount )
{
	addValue(ITEM_NUM_MISSED_CLEAVAGES, FMT("%d", missedcleavagescount).c_str(), TYPE_PROPERTY);
}

// get the missed cleavages count
int Hit::getMissedCleavagesCount( void )
{
	int i = -1;

	ValueInfo *pValInfo = getValueInfo(ITEM_NUM_MISSED_CLEAVAGES);
	if (pValInfo != NULL)
	{
		i = atol(pValInfo->value.c_str());
	}

	return i;
}

// set the rejected value
void Hit::setRejected( bool rejected )
{
	addValue(ITEM_IS_REJECTED, rejected ? "true" : "false", TYPE_PROPERTY);
}

// get the rejected value
bool Hit::getRejected( void )
{
	bool b = false;

	ValueInfo *pValInfo = getValueInfo(ITEM_IS_REJECTED);
	if (pValInfo != NULL)
	{
		b = (pValInfo->value.compare("true") == 0);
	}

	return b;
}

// set the pI value
void Hit::setPI( const char* pI )
{
	addValue(ITEM_CALC_PI, pI, TYPE_PROPERTY);
}

// get the pI value
std::string Hit::getPI( void )
{
	std::string s = "";

	ValueInfo *pValInfo = getValueInfo(ITEM_CALC_PI);
	if (pValInfo != NULL)
	{
		s = pValInfo->value;
	}

	return s;
}

// set the protein expect value
void Hit::setProteinExpect( const double proteinExpect )
{
	addValue(ITEM_PROTEIN_EXPECT, FMT("%g", proteinExpect).c_str(), TYPE_EXPECTATION_VALUE);
}

// get the protein expect value
double Hit::getProteinExpect( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_PROTEIN_EXPECT);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}

// set the peptide expect value
void Hit::setPeptideExpect( const double peptideExpect )
{
	addValue(ITEM_PEPTIDE_EXPECT, FMT("%g", peptideExpect).c_str(), TYPE_EXPECTATION_VALUE);
}

// get the peptide expect value
double Hit::getPeptideExpect( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_PEPTIDE_EXPECT);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}

// set the protein score
void Hit::setProteinScore( const double proteinscore )
{
	addValue(ITEM_PROTEIN_SCORE, FMT("%lf", proteinscore).c_str(), TYPE_SCORE);
}

// get the protein score
double Hit::getProteinScore( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_PROTEIN_SCORE);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}

// set the peptide score
void Hit::setPeptideScore( const double peptidescore )
{
	addValue(ITEM_PEPTIDE_SCORE, FMT("%lf", peptidescore).c_str(), TYPE_SCORE);
}

// get the peptide score
double Hit::getPeptideScore( void )
{
	double d = -1.0;

	ValueInfo *pValInfo = getValueInfo(ITEM_PEPTIDE_SCORE);
	if (pValInfo != NULL)
	{
		d = atof(pValInfo->value.c_str());
	}

	return d;
}
