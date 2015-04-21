/**
 * @file ModificationWrapper.cpp
 * @brief implements of ModificationWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "ModificationWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ModificationWrapper::ModificationWrapper( kome::ident::Modification& modification )
		: m_modification( modification ) {
}

// destrucotr
ModificationWrapper::~ModificationWrapper() {
}

// get modification object
kome::ident::Modification& ModificationWrapper::getModification() {
	return m_modification;
}

// get name
System::String^ ModificationWrapper::getName() {
	return ClrObjectTool::convertString( m_modification.getName().c_str(), NULL );
}

// set mono mass
void ModificationWrapper::setMonoMass( double monoMass ) {
	m_modification.setMonoMass( monoMass );
}

// get mono mass
double ModificationWrapper::getMonoMass() {
	return m_modification.getMonoMass();
}

// set amino acid residue
void ModificationWrapper::setAminoAcidResidue( System::String^ aar ) {
	m_modification.setAminoAcidResidue(
		ClrObjectTool::convertString( aar ).c_str()
	);
}

// get amino acid residue
System::String^ ModificationWrapper::getAminoAcidResidue() {
	return ClrObjectTool::convertString( m_modification.getAminoAcidResidue().c_str(), NULL );
}

// is hidden
bool ModificationWrapper::isHidden() {
	return m_modification.isHidden();
}

// set ID
void ModificationWrapper::setId( int id ) {
	m_modification.setId( id );
}

// get ID
int ModificationWrapper::getId() {
	return m_modification.getId();
}

// set hidden
void ModificationWrapper::setHidden( bool hidden ) {
	m_modification.setHidden( hidden );
}
