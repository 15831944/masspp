/**
 * @file HitWrapper.cpp
 * @brief implements of HitWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "HitWrapper.h"

#include "SubstanceWrapper.h"
#include "ModificationWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
HitWrapper::HitWrapper( kome::ident::Hit& hit )
		: m_hit( hit ) {
}

// destrucotr
HitWrapper::~HitWrapper() {
}

// get hit object
kome::ident::Hit& HitWrapper::getHit() {
	return m_hit;
}

// set substance
void HitWrapper::setSubstance( SubstanceWrapper^ sub ) {
	m_hit.setSubstance( sub == nullptr ? NULL : &( sub->getSubstance() ) );
}

// get substance
SubstanceWrapper^ HitWrapper::getSubstance() {
	kome::ident::Substance* sub = m_hit.getSubstance();
	if( sub == NULL ) {
		return nullptr;
	}

	return gcnew SubstanceWrapper( *sub );
}

// add an alternative substance
void HitWrapper::addAltSubstance( SubstanceWrapper^ sub, System::String^ before, System::String^ after, int start, int end ) {
 	m_hit.addAltSubstance(
		sub == nullptr ? NULL : &( sub->getSubstance() ),
		ClrObjectTool::convertString( before ).c_str(),
		ClrObjectTool::convertString( after ).c_str(),
		start,
		end
	);
}

// get the number of alternative substances
int HitWrapper::getNumberOfAltSubstances() {
	return m_hit.getNumberOfAltSubstances();
}

// get alternative substance
SubstanceWrapper^ HitWrapper::getAltSubstance( int index ) {
	kome::ident::Substance* sub = m_hit.getAltSubstance( index );
	if( sub == NULL ) {
		return nullptr;
	}

	return gcnew SubstanceWrapper( *sub );
}

// add spectrum
void HitWrapper::addSpectrum( SpecInfo^ spectrumInfo ) {
	// check the parameter
	if( spectrumInfo == nullptr ) {
		m_hit.addSpectrum( NULL );
		return;
	}

	// copy
	kome::ident::SpecInfo* si = new kome::ident::SpecInfo();
	copySpectrum( spectrumInfo, *si );

	// add
	m_hit.addSpectrum( si );
}

// get number of spectra
int HitWrapper::getNumberOfSpectra() {
	return m_hit.getNumberOfSpectra();
}

// get spectrum
HitWrapper::SpecInfo^ HitWrapper::getSpectrum( int index ) {
	kome::ident::SpecInfo* s = m_hit.getSpectrum( index );
	return copySpectrum( s );
}

// clear modifications
void HitWrapper::clearModifications() {
	m_hit.clearModifications();
}

// add modification
void HitWrapper::addModification( ModificationWrapper^ m, int pos ) {
	kome::ident::Modification* modification = NULL;
	if( m != nullptr ) {
		modification = &( m->getModification() );
	}

	m_hit.addModification( modification, pos );
}

// get number of modifications
unsigned int HitWrapper::getNumberOfModifications() {
	return m_hit.getNumberOfModifications();
}

// get modification
ModificationWrapper^ HitWrapper::getModification( unsigned int idx ) {
	kome::ident::Modification* m = m_hit.getModification( idx );
	if( m == NULL ) {
		return nullptr;
	}

	return gcnew ModificationWrapper( *m );
}

// get modification position
int HitWrapper::getModificationPos( unsigned int idx ) {
	return m_hit.getModificationPos( idx );
}

// add value
void HitWrapper::addValue( System::String^ propName, System::String^ propValue, ValueType propValueType ) {
	// type
	kome::ident::ValueType t = kome::ident::TYPE_PROPERTY;
	if( propValueType == ValueType::SCORE ) {
		t = kome::ident::TYPE_SCORE;
	}
	else if( propValueType == ValueType::EXPECTATION_VALUE ) {
		t = kome::ident::TYPE_EXPECTATION_VALUE;
	}

	// add value
	m_hit.addValue(
		ClrObjectTool::convertString( propName ).c_str(),
		ClrObjectTool::convertString( propValue ).c_str(),
		t
	);
}

// get number of values
int HitWrapper::getNumberOfValues() {
	return m_hit.getNumberOfValues();
}

// get value name
System::String^ HitWrapper::getValueName( int index ) {
	return ClrObjectTool::convertString( m_hit.getValueName( index ).c_str(), NULL );
}

// get value
System::String^ HitWrapper::getValue( int index ) {
	return ClrObjectTool::convertString( m_hit.getValue( index ).c_str(), NULL );
}

// get value type
HitWrapper::ValueType HitWrapper::getValueType( int index ) {
	kome::ident::ValueType t = m_hit.getValueType( index );

	ValueType type = ValueType::PROPERTY;
	if( t == kome::ident::TYPE_SCORE ) {
		type = ValueType::SCORE;
	}
	else if( t == kome::ident::TYPE_EXPECTATION_VALUE ) {
		type = ValueType::EXPECTATION_VALUE;
	}

	return type;
}

// get value information
HitWrapper::ValueInfo^ HitWrapper::getValueInfo( System::String^ name ) {
	// value info
	kome::ident::ValueInfo* v = m_hit.getValueInfo( ClrObjectTool::convertString( name ).c_str() );

	// copy
	ValueInfo^ val = gcnew ValueInfo();
	val->name = ClrObjectTool::convertString( v->name.c_str(), NULL );
	val->value = ClrObjectTool::convertString( v->value.c_str(), NULL );
	val->valueType = ValueType::PROPERTY;
	if( v->valueType == kome::ident::TYPE_SCORE ) {
		val->valueType = ValueType::SCORE;
	}
	else if( v->valueType == kome::ident::TYPE_EXPECTATION_VALUE ) {
		val->valueType = ValueType::EXPECTATION_VALUE;
	}

	return val;
}

// set charge
void HitWrapper::setCharge( int charge ) {
	m_hit.setCharge( charge );
}

// get charge
int HitWrapper::getCharge() {
	return m_hit.getCharge();
}

// set rank
void HitWrapper::setRank( int rank ) {
	m_hit.setRank( rank );
}

// get rank
int HitWrapper::getRank() {
	return m_hit.getRank();
}

// set protein count
void HitWrapper::setProteinCount( int proteincount ) {
	m_hit.setProteinCount( proteincount );
}

// get protein count
int HitWrapper::getProteinCount() {
	return m_hit.getProteinCount();
}

// set matched ion count
void HitWrapper::setMatchedIonCount( int matchedioncount ) {
	m_hit.setMatchedIonCount( matchedioncount );
}

// get matched ion count
int HitWrapper::getMatchedIonCount() {
	return m_hit.getMatchedIonCount();
}

// set ion count
void HitWrapper::setIonCount( int ioncount ) {
	m_hit.setIonCount( ioncount );
}

// get ion count
int HitWrapper::getIonCount() {
	return m_hit.getIonCount();
}

// set calculated MR
void HitWrapper::setCalculatedMR( double calculatedMR ) {
	m_hit.setCalculatedMR( calculatedMR );
}

// get chalculted MR
double HitWrapper::getCalculatedMR() {
	return m_hit.getCalculatedMR();
}

// set delta
void HitWrapper::setDelta( double delta ) {
	m_hit.setDelta( delta );
}

// get delta
double HitWrapper::getDelta() {
	return m_hit.getDelta();
}

// set termini consistent count
void HitWrapper::setTerminiConsistentCount( int terminiConsistentCount ) {
	m_hit.setTerminiConsistentCount( terminiConsistentCount );
}

// get termini consistent count
int HitWrapper::getTerminiConsistentCount() {
	return m_hit.getTerminiConsistentCount();
}

// set missed cleavages count
void HitWrapper::setMissedCleavagesCount( int missedcleavagescount ) {
	m_hit.setMissedCleavagesCount( missedcleavagescount );
}

// get missed cleavages count
int HitWrapper::getMissedCleavagesCount() {
	return m_hit.getMissedCleavagesCount();
}

// set rejected
void HitWrapper::setRejected( bool rejected ) {
	m_hit.setRejected( rejected );
}

// is rejected
bool HitWrapper::getRejected() {
	return m_hit.getRejected();
}

// set pI
void HitWrapper::setPI( System::String^ pI ) {
	m_hit.setPI( ClrObjectTool::convertString( pI ).c_str() );
}

// get pI
System::String^ HitWrapper::getPI() {
	return ClrObjectTool::convertString( m_hit.getPI().c_str(), NULL );
}

// set protein expect
void HitWrapper::setProteinExpect( double proteinExpect ) {
	m_hit.setProteinExpect( proteinExpect );
}

// get protein expect
double HitWrapper::getProteinExpect() {
	return m_hit.getProteinExpect();
}

// set peptide expect
void HitWrapper::setPeptideExpect( double peptideExpect ) {
	m_hit.setPeptideExpect( peptideExpect );
}

// get peptide expect
double HitWrapper::getPeptideExpect() {
	return m_hit.getPeptideExpect();
}

// set protein score
void HitWrapper::setProteinScore( double proteinscore ) {
	m_hit.setProteinScore( proteinscore );
}

// get protein score
double HitWrapper::getProteinScore() {
	return m_hit.getProteinScore();
}

// set peptide score
void HitWrapper::setPeptideScore( double peptidescore ) {
	m_hit.setPeptideScore( peptidescore );
}

// get peptide score
double HitWrapper::getPeptideScore() {
	return m_hit.getPeptideScore();
}

// copy spectrum
HitWrapper::SpecInfo^ HitWrapper::copySpectrum( kome::ident::SpecInfo* spec ) {
	// check the paramter
	if( spec == NULL ) {
		return nullptr;
	}

	// copy
	SpecInfo^ dst = gcnew SpecInfo();
	dst->id = spec->id;
	dst->specId = spec->specId;
	dst->sampleIndex = spec->sampleIndex;
	dst->sampleName = ClrObjectTool::convertString( spec->sampleName.c_str(), NULL );
	dst->path = ClrObjectTool::convertString( spec->path.c_str(), NULL );
	dst->retentionTime = spec->retentionTime;
	dst->precursor = spec->precursor;
	dst->stage = spec->stage;

	dst->polarity = SpectrumWrapper::Polarity::UNKNOWN;
	if( spec->polarity == kome::objects::Spectrum::POLARITY_POSITIVE ) {
		dst->polarity = SpectrumWrapper::Polarity::POSITIVE;
	}
	else if( spec->polarity == kome::objects::Spectrum::POLARITY_NEGATIVE ) {
		dst->polarity = SpectrumWrapper::Polarity::NEGATIVE;
	}

	dst->peakIds = nullptr;
	if( spec->peakIds.size() > 0 ) {
		dst->peakIds = gcnew cli::array< int >( spec->peakIds.size() );
		for( unsigned int i = 0; i < spec->peakIds.size(); i++ ) {
			( dst->peakIds )[ i ] = ( spec->peakIds )[ i ];
		}
	}

	dst->charges = nullptr;
	if( spec->charges.size() > 0 ) {
		dst->charges = gcnew cli::array< int >( spec->charges.size() );
		for( unsigned int i = 0; i < spec->charges.size(); i++ ) {
			( dst->charges )[ i ] = ( spec->charges )[ i ];
		}
	}

	dst->name = ClrObjectTool::convertString( spec->name.c_str(), NULL );
	dst->peaks = spec->peaks;

	return dst;
}

// copy  spectrum
void HitWrapper::copySpectrum( SpecInfo^ src, kome::ident::SpecInfo& dst ) {
	// check the parameter
	if( src == nullptr ) {
		return;
	}

	// copy
	dst.id = src->id;
	dst.specId = src->specId;
	dst.sampleIndex = src->sampleIndex;
	dst.sampleName = ClrObjectTool::convertString( src->sampleName );
	dst.path = ClrObjectTool::convertString( src->path );
	dst.retentionTime = src->retentionTime;
	dst.precursor = src->precursor;
	dst.stage = src->stage;
	
	dst.polarity = kome::objects::Spectrum::POLARITY_UNKNOWN;
	if( src->polarity == SpectrumWrapper::Polarity::POSITIVE ) {
		dst.polarity = kome::objects::Spectrum::POLARITY_POSITIVE;
	}
	else if( src->polarity == SpectrumWrapper::Polarity::NEGATIVE ) {
		dst.polarity = kome::objects::Spectrum::POLARITY_NEGATIVE;
	}
	
	dst.peakIds.clear();
	if( src->peakIds != nullptr ) {
		for( int i = 0; i < src->peakIds->Length; i++ ) {
			dst.peakIds.push_back( ( src->peakIds )[ i ] );
		}
	}

	dst.charges.clear();
	if( src->charges != nullptr ) {
		for( int i = 0; i < src->charges->Length; i++ ) {
			dst.charges.push_back( ( src->charges )[ i ] );
		}
	}

	dst.name = ClrObjectTool::convertString( src->name );
	dst.peaks = src->peaks;
}
