/**
 * @file SpectraChromatogram.cpp
 * @brief implements of SpectraChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraChromatogram.h"

#include "PointsManager.h"
#include "PeaksManager.h"
#include "Spectrum.h"
#include "DataSet.h"
#include "DataGroupNode.h"
#include "XYDataOperation.h"

#include <math.h>
#include <set>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
const unsigned char SpectraChromatogram::m_msFilters[] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80
};

// constructor
SpectraChromatogram::SpectraChromatogram( DataSet& dataSet, bool bProces )
		: Chromatogram( dataSet.getGroup() == NULL ? NULL : dataSet.getGroup()->getSample() ) {
	// initialize
	m_minMz = -1.0;
	m_maxMz = -1.0;
	m_minRt = -1.0;
	m_maxRt = -1.0;
	m_filter = 0xff;
	m_tic = true;
	
	m_proc = bProces; // add flg @date 2012.10.03 <Add> M.Izumi

	m_spectra = &dataSet;
	
	m_group = dataSet.getGroup();
	setTitle();
}

// destructor
SpectraChromatogram::~SpectraChromatogram() {
}

// get spectra
DataSet& SpectraChromatogram::getMemberSpectra() {
	return *m_spectra;
}

// set m/z range
void SpectraChromatogram::setMzRange( const double minMz, const double maxMz ) {
	setMinMz( minMz );
	setMaxMz( maxMz );
}

// set min m/z
void SpectraChromatogram::setMinMz( const double mz ) {
	if( mz != m_minMz ) {
		deleteXYData();
		m_minMz = mz;
	}
	setTitle();
}

// set max m/z
void SpectraChromatogram::setMaxMz( const double mz ) {
	if( mz != m_maxMz ) {		
		deleteXYData();
		m_maxMz = mz;
	}
	setTitle();
}

// get min m/z
double SpectraChromatogram::getMinMz() {
	return m_minMz;
}

// get max m/z
double SpectraChromatogram::getMaxMz() {
	return m_maxMz;
}

// set RT range
void SpectraChromatogram::setRtRange( const double minRt, const double maxRt ) {
	setMinRt( minRt );
	setMaxRt( maxRt );
}

// set min RT
void SpectraChromatogram::setMinRt( const double rt ) {
	if( rt != m_minRt ) {
		deleteXYData();
		m_minRt = rt;
	}
}

// set max RT
void SpectraChromatogram::setMaxRt( const double rt ) {
	if( rt != m_maxRt ) {
		deleteXYData();
		m_maxRt = rt;
	}
}

// get min RT
double SpectraChromatogram::getMinRt() {
	return m_minRt;
}

// get max RT
double SpectraChromatogram::getMaxRt() {
	return m_maxRt;
}

// set filter
void SpectraChromatogram::setFilter( const unsigned char filter ) {
	if( m_filter != filter ) {
		deleteXYData();
		m_filter = filter;
	}
}

// get filter
unsigned char SpectraChromatogram::getFilter() {
	return m_filter;
}

// set TIC mode
void SpectraChromatogram::setTIC() {
	if( !m_tic ) {
		deleteXYData();
		m_tic = true;
	}
	setTitle();
}

// set BPC mode
void SpectraChromatogram::setBPC() {
	if( m_tic ) {
		deleteXYData();
		m_tic = false;
	}
	setTitle();
}

// get TIC mode
bool SpectraChromatogram::isTIC() {
	return m_tic;
}

// get BPC mode
bool SpectraChromatogram::isBPC() {
	return !m_tic;
}

// get spectra
void SpectraChromatogram::getMemberSpectra( std::vector< Spectrum* >& spectra ) {
	// get xy data
	for( unsigned int i = 0; i < m_spectra->getNumberOfSpectra(); i++ ) {
		// sepectrum
		Spectrum* spec = m_spectra->getSpectrum( i );

		// check
		if( spec->hasChromatogram() ) {
			const unsigned char stage = m_msFilters[ CLAMP( spec->getMsStage(), 1, 8 ) - 1 ];
			if( m_filter & stage ) {
				spectra.push_back( spec );
			}
		}
	}
}

// search spectrum
int SpectraChromatogram::searchSpectrumIndex( std::vector< Spectrum* >& spectra, const double rt, SearchType search ) {
	// search
	int idx = -1;
	double diff = 99999999999.9;
	for( int i = 0; i < (int)spectra.size(); i++ ) {
		// spectrum
		Spectrum* spec = spectra[ i ];

		// 
		if( search == SEARCH_PREV ) {
			if( spec->getRt() <= rt ) {
				idx = i;
			}
		}
		else if( search == SEARCH_NEXT ) {
			if( spec->getRt() >= rt && idx < 0 ) {
				idx = i;
			}
		}
		else {
			double d = fabs( spec->getRt() - rt );
			if( d < diff ) {
				idx = i;
				diff = d;
			}
		}
	}

	return idx;
}

// set title
void SpectraChromatogram::setTitle() {
	// title
	std::string title;

	if( !m_proc ){
		// chromatogram type
		if( m_tic ) {
			if( m_minMz < 0.0 && m_maxMz < 0.0 ) {
				title = "TIC";
				m_icon = "TIC";
			}
			else {
				title = "MC";
				m_icon = "MC";
			}
	
		}else{
			title = "BPC";
			m_icon = "BPC";
		}
	}else{
		title = "Processed MC";
		// chromatogram type
		if( m_tic ) {
			m_icon = "MC";	
		}else{
			m_icon = "BPC";
		}
	}

	// range
	if( m_minMz >= 0.0 && m_maxMz >= 0.0 ) {
		title.append( FMT( "[%.2f-%.2f]", m_minMz, m_maxMz ) );
	}
	else if( m_minMz >= 0.0 ) {
		title.append( FMT( "[%.2f- ]", m_minMz ) );
	}
	else if( m_maxMz >= 0.0 ) {
		title.append( FMT( "[ -%.2f]", m_maxMz ) );
	}

	// name
	if( m_name.empty() ) {
		m_title = title;
	}
	else {
		if( m_group != NULL && m_group->getParentGroup() != NULL ) {
			m_title = FMT( "%s (%s) : %s", m_name.c_str(), m_group->getName(), title.c_str() );
		}
		else {
			m_title = FMT( "%s : %s", m_name.c_str(), title.c_str() );
		}
	}
}

// on get xy data
void SpectraChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// get spectra
	std::vector< Spectrum* > spectra;
	std::set< Spectrum* > specSet;
	getMemberSpectra( spectra );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		specSet.insert( spectra[ i ] );
	}

	// get xy data
	for( unsigned int i = 0; i < m_spectra->getNumberOfSpectra(); i++ ) {
		// spectrum
		Spectrum* spec = m_spectra->getSpectrum( i );

		// RT
		double rt = spec->getRt();

		// add point
		if( ( m_minRt < 0.0 || rt >= m_minRt ) && ( m_maxRt < 0.0 || rt <= m_maxRt )
				&& specSet.find( spec ) != specSet.end() ) {
			// intensity
			double intensity = 0.0;

			// @date 2012.10.03 <Mod> M.Izumi ->
			if( !m_proc ){
				if( m_tic ) {
					intensity = spec->getTotalIntensity( m_minMz, m_maxMz );
				}
				else {
					intensity = spec->getMaxIntensity( m_minMz, m_maxMz );
				}
			}else{ // Use data processing
				kome::core::DataPoints dps;
				spec->getXYData( &dps, true );
				
				for( unsigned int i=0; i < dps.getLength(); i++ ){
					if( m_minMz < dps.getX(i) || dps.getX(i) >= m_maxMz ){
						if( m_tic ){
							intensity += dps.getY(i);	// TIC
						}else{
							intensity = std::max( intensity, dps.getY(i) );	// BPC
						}
					}
				}

			}
			// @date 2012.10.03 <Mod> M.Izumi <-
			
			// add
			xyData->addPoint( rt, intensity );
		}
	}
}

// on get spectra
void SpectraChromatogram::onGetSpectra(
		DataSet& spectra,
		const double startRt,
		const double endRt,
		const SearchType startSearch,
		const SearchType endSearch
) {
	// get spectra
	std::vector< Spectrum* > specArray;
	getMemberSpectra( specArray );

	// search index
	int startIdx = std::max( 0, searchSpectrumIndex( specArray, startRt, startSearch ) );
	int endIdx = std::min( (int)specArray.size() - 1, searchSpectrumIndex( specArray, endRt, endSearch ) );

	// get spectra
	for( int i = startIdx; i <= endIdx; i++ ) {
		spectra.addSpectrum( specArray[ i ] );
	}
}

// on get mass
double SpectraChromatogram::onGetMass( const unsigned int index ) {
	// get spectrum
	if( index >= m_spectra->getNumberOfSpectra() ) {
		return -1.0;
	}
	Spectrum* spec = m_spectra->getSpectrum( index );
	if( spec == NULL ) {
		return -1.0;
	}

	return spec->getBasePeakMass();
}

// on get MS stage
int SpectraChromatogram::onGetMsStage( const unsigned int index ) {
	// get spectrum
	if( index >= m_spectra->getNumberOfSpectra() ) {
		return -1;
	}
	Spectrum* spec = m_spectra->getSpectrum( index );
	if( spec == NULL ) {
		return -1;
	}

	return spec->getMsStage();
}

// on get precursor
double SpectraChromatogram::onGetPrecursor( const unsigned int index ) {
	// get spectrum
	if( index >= m_spectra->getNumberOfSpectra() ) {
		return -1.0;
	}
	Spectrum* spec = m_spectra->getSpectrum( index );
	if( spec == NULL ) {
		return -1.0;
	}

	return spec->getPrecursor();
}
