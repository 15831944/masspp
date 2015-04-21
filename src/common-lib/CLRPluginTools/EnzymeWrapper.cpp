/**
 * @file EnzymeWrapper.cpp
 * @brief implements of EnzymeWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "EnzymeWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
EnzymeWrapper::EnzymeWrapper( kome::ident::Enzyme& enzyme )
		: m_enzyme( enzyme ) {
}

// destrucotr
EnzymeWrapper::~EnzymeWrapper() {
}

// get enzyme object
kome::ident::Enzyme& EnzymeWrapper::getEnzyme() {
	return m_enzyme;
}

// get name
System::String^ EnzymeWrapper::getName() {
	return ClrObjectTool::convertString( m_enzyme.getName().c_str(), NULL );
}

// set ID
void EnzymeWrapper::setId( int id ) {
	m_enzyme.setId( id );
}

// get ID
int EnzymeWrapper::getId() {
	return m_enzyme.getId();
}

// clear cutters
void EnzymeWrapper::clearCutters() {
	m_enzyme.clearCutters();
}

// add cutter
void EnzymeWrapper::addCutter( TermType term, System::String^ cleave, System::String^ restrict ) {
	// terminal
	kome::ident::Enzyme::TermType t = kome::ident::Enzyme::UNKNOWN;
	if( term == TermType::C_TERM ) {
		t = kome::ident::Enzyme::C_TERM;
	}
	else if( term == TermType::N_TERM ) {
		t = kome::ident::Enzyme::N_TERM;
	}

	// add
	m_enzyme.addCutter(
		t,
		ClrObjectTool::convertString( cleave ).c_str(),
		ClrObjectTool::convertString( restrict ).c_str()
	);
}

// get number of cutters
unsigned int EnzymeWrapper::getNumberOfCutters() {
	return m_enzyme.getNumberOfCutters();
}

// get cutter
EnzymeWrapper::Cutter^ EnzymeWrapper::getCutter( unsigned int idx ) {
	// cutter
	kome::ident::Enzyme::Cutter* c = m_enzyme.getCutter( idx );
	if( c == NULL ) {
		return nullptr;
	}

	// convert
	Cutter^ cutter = gcnew Cutter();

	TermType term = TermType::UNKNOWN;

	kome::ident::Enzyme::TermType t = c->term;
	if( t == kome::ident::Enzyme::C_TERM ) {
		term = TermType::C_TERM;
	}
	else if( t == kome::ident::Enzyme::N_TERM ) {
		term = TermType::N_TERM;
	}

	cutter->term = term;
	cutter->cleave = ClrObjectTool::convertString( c->cleave.c_str(), NULL );
	cutter->restrict = ClrObjectTool::convertString( c->restrict.c_str(), NULL );

	return cutter;
}

// set hidden
void EnzymeWrapper::setHidden( bool hidden ) {
	m_enzyme.setHidden( hidden );
}

// get hidden
bool EnzymeWrapper::isHidden() {
	return m_enzyme.isHidden();
}
