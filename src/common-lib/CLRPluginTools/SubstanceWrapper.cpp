/**
 * @file SubstanceWrapper.cpp
 * @brief implements of SubstanceWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "SubstanceWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SubstanceWrapper::SubstanceWrapper( kome::ident::Substance& substance )
		: m_substance( substance ) {
}

// destrucotr
SubstanceWrapper::~SubstanceWrapper() {
}

// get substance object
kome::ident::Substance& SubstanceWrapper::getSubstance() {
	return m_substance;
}

// get accession
System::String^ SubstanceWrapper::getAccession() {
	return ClrObjectTool::convertString( m_substance.getAccession().c_str(), NULL );
}

// get name
System::String^ SubstanceWrapper::getName() {
	return ClrObjectTool::convertString( m_substance.getName().c_str(), NULL );
}

// get substance type
SubstanceWrapper::SubstanceType SubstanceWrapper::getType() {
	SubstanceType type = SubstanceType::UNKNOWN;

	kome::ident::SubstanceType t = m_substance.getType();
	if( t == kome::ident::TYPE_PROTEIN ) {
		type = SubstanceType::PROTEIN;
	}
	else if( t == kome::ident::TYPE_PEPTIDE ) {
		type = SubstanceType::PEPTIDE;
	}
	else if( t == kome::ident::TYPE_METABOLITE ) {
		type = SubstanceType::METABOLITE;
	}

	return type;
}

// get parent substance
SubstanceWrapper^ SubstanceWrapper::getParentSubstance() {
	kome::ident::Substance* parent = m_substance.getParentSubstance();
	if( parent == NULL ) {
		return nullptr;
	}

	return gcnew SubstanceWrapper( *parent );
}

// set parent substance
void SubstanceWrapper::setParentSubstance( SubstanceWrapper^ parent ) {
	m_substance.setParentSubstance( parent == nullptr ? NULL : &( parent->getSubstance() ) );
}

// get the number of child substances
int SubstanceWrapper::getNumberOfChildSubstances() {
	return m_substance.getNumberOfChildSubstances();
}

// get child substances
cli::array< SubstanceWrapper^ >^ SubstanceWrapper::getChildSubstances() {
	// get children
	std::vector< kome::ident::Substance* > children;
	m_substance.getChildSubstances( children );

	// create array
	if( children.size() == 0 ) {
		return nullptr;
	}

	cli::array< SubstanceWrapper^ >^ arr = gcnew cli::array< SubstanceWrapper^ >( children.size() );
	for( unsigned int i = 0; i < children.size(); i++ ) {
		arr[ i ] = gcnew SubstanceWrapper( *children[ i ] );
	}

	return arr;
}

// get properties
PropertiesWrapper^ SubstanceWrapper::getProperties() {
	return ClrObjectTool::createPropertiesWrapper( &( m_substance.getProperties() ) );
}
