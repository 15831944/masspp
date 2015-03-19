/**
 * @file SpectrumListCtrl.cpp
 * @brief implemnets of SpectrumListCtrl class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumListCtrl.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SpectrumListCtrl, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
SpectrumListCtrl::SpectrumListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::CheckGridListCtrl( parent, wxID_ANY, width, height, false ) {
	m_spectrumCol = 0;
	m_rtCol = 1;
	m_stageCol = 2;
	m_precursorCol = 3;
	m_appendedCol = 0;
}

// destructor
SpectrumListCtrl::~SpectrumListCtrl() {
}

// get the number of spectra
unsigned int SpectrumListCtrl::getNumberOfSpectra() {
	return m_spectra.size();
}

// get spectrum
kome::objects::Spectrum* SpectrumListCtrl::getSpectrum( const unsigned int idx ) {
	if( idx >= m_spectra.size() ) {
		return NULL;
	}

	return m_spectra[ idx ];
}

// clear spectra
void SpectrumListCtrl::clearSpectra() {
	m_spectra.clear();
	clearData( false );
}

// add spectra
void SpectrumListCtrl::addSpectrum( kome::objects::Spectrum* spec ) {
	addData( m_spectra.size(), false );
	m_spectra.push_back( spec );
}


// on create
void SpectrumListCtrl::onCreate() {
	// header
	m_spectrumCol = -1;
	m_rtCol = -1;
	m_stageCol = -1;
	m_precursorCol = -1;
	m_appendedCol = 0;

	addColumn( "Spectrum", TYPE_STRING );
	m_spectrumCol = m_appendedCol;
	m_appendedCol += 1;

	addColumn( "RT", TYPE_UDOUBLE );
	m_rtCol = m_appendedCol;
	m_appendedCol += 1;

	addColumn( "Stage", TYPE_UINT );
	m_stageCol = m_appendedCol;
	m_appendedCol += 1;

	addColumn( "Precursor", TYPE_UDOUBLE );
	m_precursorCol = m_appendedCol;
	m_appendedCol += 1;

	// add data
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		addData( i, false );
	}
}

// get double value
double SpectrumListCtrl::getDouble( const long long data, const int column ) const {

	// get value
	double val = double();

	if( column == m_rtCol ) {	// RT
		double rt = m_spectra[data]->getRt();
		val = rt;
	}
	else if( column == m_precursorCol ) {	// precursor
		double precursor = m_spectra[data]->getPrecursor();

		val = precursor;
	}

	return val;
}

// get int value
int SpectrumListCtrl::getInt( const long long data, const int column ) const {

	// get value
	int val = int();

	if( column == m_stageCol ) {	// stage
		int stage = m_spectra[data]->getMsStage();

		val = stage;
	}

	return val;
}

// get string value
std::string SpectrumListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	if( column == m_spectrumCol ) {	// spectrum
		std::string name = m_spectra[data]->getName();

		s = name;
	}

	return s;
}
