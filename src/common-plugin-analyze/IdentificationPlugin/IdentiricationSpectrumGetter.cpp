/**
 * @file IdentificationSpectrumGetter.h
 * @brief interfaces of IdentificationSpectrumGetter class
 *
 * @author S.Tanaka
 * @date 2012.08.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "IdentificationSpectrumGetter.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IdentificationSpectrumGetter::IdentificationSpectrumGetter( kome::objects::Sample* sample ) {
	m_index = -1;
	m_sample = sample;
}

// destructor
IdentificationSpectrumGetter::~IdentificationSpectrumGetter() {
}

// add spectrum
void IdentificationSpectrumGetter::addSpectrum( kome::objects::Spectrum* spec ) {
	m_spectra.push_back( spec );
	m_index = -1;
}

// clear spectra
void IdentificationSpectrumGetter::clearSpectra() {
	m_spectra.clear();
	m_index = -1;
}

// prepare next spectrum
bool IdentificationSpectrumGetter::next( kome::core::Progress& progress ) {
	// initialize
	if( m_index < 0 ) {
		// progress
		if( m_spectra.size() > 0 ) {
			progress.setRange( 0, m_spectra.size() );
		}
	}

	// next
	m_index++;
	progress.setPosition( m_index );

	return ( m_index < (int)m_spectra.size() );
}

// get spectrum
kome::objects::Spectrum* IdentificationSpectrumGetter::getSpectrum() {
	return m_spectra[ m_index ];
}

// get peak IDs count
unsigned int IdentificationSpectrumGetter::getNumberOfPeakIds() {
	return 0;
}

// get peak ID
int IdentificationSpectrumGetter::getPeakId( const unsigned int idx ) {
	return -1;
}

// set parameters
void IdentificationSpectrumGetter::setParameters( const char* s ) {
	// clear
	clearSpectra();

	// check the sample
	if( m_sample == NULL ) {
		return;
	}

	// ID
	std::vector< std::string > ids;
	stringtoken( s, ":", ids );

	for( unsigned int i = 0; i < ids.size(); i++ ) {
		int id = toint( ids[ i ].c_str(), 10, -1 );
		kome::objects::Spectrum* spec = m_sample->getSpectrumById( id );
		if( spec != NULL ) {
			addSpectrum( spec );
		}
	}
}

// get parameters
std::string IdentificationSpectrumGetter::getParameters() {
	// return value
	std::string ret;

	// peaks IDs
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		const int id  = m_spectra[ i ]->getId();
		if( i > 0 ) {
			ret.append( ":" );
		}
		ret.append( FMT( "%d", id ) );
	}

	return ret;
}
