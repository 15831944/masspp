/**
 * @file SpectraSearchFilter.cpp
 * @brief implements of SpectraSearchFilter class
 *
 * @author S.Tanaka
 * @date 2007.02.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraSearchFilter.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectraSearchFilter::SpectraSearchFilter() : m_stageRange( kome::core::RangeList::DEFAULT_ALL ), 
		m_scanRange( kome::core::RangeList::DEFAULT_ALL ),
		m_rtRange( kome::core::RangeList::DEFAULT_ALL ), 
		m_ticRange( kome::core::RangeList::DEFAULT_ALL ), 
		m_bpcRange( kome::core::RangeList::DEFAULT_ALL ), 
		m_precRange( kome::core::RangeList::DEFAULT_ALL ) {
}

// destructor
SpectraSearchFilter::~SpectraSearchFilter() {
}

// set stage range
void SpectraSearchFilter::setStageRange( const char* range ) {
	m_stageRange.importIntRanges( NVL( range, "" ) );
}

// reset stage range
void SpectraSearchFilter::resetStageRange() {
	m_stageRange.clearRanges();
}

// set scan range
void SpectraSearchFilter::setScanRange( const char* range ) {
	m_scanRange.importIntRanges( NVL( range, "" ) );
}

// reset scan range
void SpectraSearchFilter::resetScanRange() {
	m_scanRange.clearRanges();
}

// set RT range
void SpectraSearchFilter::setRtRange( const char* range ) {
	m_rtRange.importDoubleRanges( NVL( range, "" ) );
}

// clear RT range
void SpectraSearchFilter::resetRtRange() {
	m_rtRange.clearRanges();
}

// set TIC range
void SpectraSearchFilter::setTicRange( const char* range ) {
	m_ticRange.importDoubleRanges( NVL( range, "" ) );
}

// clear TIC range
void SpectraSearchFilter::resetTicRange() {
	m_ticRange.clearRanges();
}

// set BPC range;
void SpectraSearchFilter::setBpcRange( const char* range ) {
	m_bpcRange.importDoubleRanges( NVL( range, "" ) );
}

// clear BPC range
void SpectraSearchFilter::resetBpcRange() {
	m_bpcRange.clearRanges();
}

// set precursor range
void SpectraSearchFilter::setPrecursorRange( const char* range ) {
	m_precRange.importDoubleRanges( NVL( range, "" ) );
}

// clear precursor range
void SpectraSearchFilter::resetPrecursorRange() {
	m_precRange.clearRanges();
}

// add property filter
void SpectraSearchFilter::addPropertyFilter(
		const char* key,
		const char* value,
		PropertyFilterType type
) {
	// add to the array
	m_propFilters.resize( m_propFilters.size() + 1 );

	// set elements
	m_propFilters.back().key   = NVL( key, "" );
	m_propFilters.back().value = NVL( value, "" );
	m_propFilters.back().type  = type;
}

// get the number of property filters
unsigned int SpectraSearchFilter::getNumberOfPropertyFilters() {
	return m_propFilters.size();
}

// get property filter key
const char* SpectraSearchFilter::getPropertyFilterKey( const unsigned int index ) {
	// check the index
	if( index >= m_propFilters.size() ) {
		return NULL;
	}

	// get key
	return m_propFilters[ index ].key.c_str();
}

// get property filter value
const char* SpectraSearchFilter::getPropertyFilterValue( const unsigned int index ) {
	// check the index
	if( index >= m_propFilters.size() ) {
		return NULL;
	}

	// get value
	return m_propFilters[ index ].value.c_str();
}

// get property filter type
SpectraSearchFilter::PropertyFilterType SpectraSearchFilter::getPropertyFilterType( const unsigned int index ) {
	// check the index
	if( index >= m_propFilters.size() ) {
		return PROP_FILTER_UNKNOWN;
	}

	// get type
	return m_propFilters[ index ].type;
}

// remove property filter
void SpectraSearchFilter::removePropertyFilter( const unsigned int index ) {
	// check the index
	if( index >= m_propFilters.size() ) {
		return;
	}

	// remove
	m_propFilters.erase( m_propFilters.begin() + index );
}

// reset all
void SpectraSearchFilter::resetAll() {
	resetStageRange();
	resetScanRange();
	resetRtRange();
	resetTicRange();
	resetBpcRange();
	resetPrecursorRange();

	m_propFilters.clear();
}

// execute filter
void SpectraSearchFilter::filter(
		std::vector< kome::objects::Spectrum* >& srcSpectra,
		std::vector< kome::objects::Spectrum* >& dstSpectra
) {
	// working spectrum array
	std::vector< kome::objects::Spectrum* > workingArray;

	// spectrum filter
	for( unsigned int i = 0; i < srcSpectra.size(); i++ ) {		
		// spectrum
		kome::objects::Spectrum* spec = srcSpectra[ i ];

		// execute filter
		if( allowSpectrum( spec ) ) {
			workingArray.push_back( spec );
		}
	}

	// properties filter
	for( unsigned int i = 0; i < workingArray.size(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = workingArray[ i ];

		// properties
		kome::core::Properties props;
		spec->getProperties( props );

		// filter
		if( allowProperties( props ) ) {
			dstSpectra.push_back( spec );
		}
	}
}

// allows spectrum
bool SpectraSearchFilter::allowSpectrum( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return false;
	}

	// check stage
	if( m_stageRange.getNumberOfRanges() > 0 ) {
		int stage = spec->getMsStage();
		if( stage < 0 || !m_stageRange.checkIntValue( stage ) ) {
			return false;
		}
	}

	// check scan
	if( m_scanRange.getNumberOfRanges() > 0 ) {
		int scan = spec->getScanNumber();
		if( scan < 0 || !m_scanRange.checkIntValue( scan ) ) {
			return false;
		}
	}

	// check RT
	if( m_rtRange.getNumberOfRanges() > 0 ) {
		double rt = spec->getRt();
		if( rt < 0.0 || !m_rtRange.checkDoubleValue( rt ) ) {
			return false;
		}
	}

	// check TIC
	if( m_ticRange.getNumberOfRanges() > 0 ) {
		double tic = spec->getTotalIntensity( -1.0, -1.0 );
		if( tic < 0.0 || !m_ticRange.checkDoubleValue( tic ) ) {
			return false;
		}
	}

	// check BPC
	if( m_bpcRange.getNumberOfRanges() > 0 ) {
		double bpc = spec->getMaxIntensity( -1.0, -1.0 );
		if( bpc < 0.0 || !m_bpcRange.checkDoubleValue( bpc ) ) {
			return false;
		}
	}

	// Precursor
	if( m_precRange.getNumberOfRanges() > 0 ) {
		double precursor = spec->getPrecursor();
		if( precursor < 0.0 || !m_precRange.checkDoubleValue( precursor ) ) {
			return false;
		}
	}

	return true;
}

// judges whether this filter allows properties
bool SpectraSearchFilter::allowProperties( kome::core::Properties& properties ) {
	// check the member
	if( m_propFilters.size() == 0 ) {
		return true;
	}

	// check properties
	for( unsigned int i = 0; i < m_propFilters.size(); i++ ) {
		// get filter information
		std::string key = m_propFilters[ i ].key;
		std::string value = m_propFilters[ i ].value;
		PropertyFilterType type = m_propFilters[ i ].type;

		// get value
		std::string v;
		bool existing = false;
		if( properties.hasKey( key.c_str() ) ) {
			v = properties.getStringValue( key.c_str(), "" );
			existing = true;
		}

		// check
		if( type == PROP_FILTER_IS ) {
			if( !existing ) {
				return false;
			}
			if( value.compare( v ) != 0 ) {
				return false;
			}
		}
		else if( type == PROP_FILTER_IS_NOT ) {
			if( existing ) {
				if( value.compare( v ) == 0 ) {
					return false;
				}
			}
		}
		else if( type == PROP_FILTER_CONTAINS ) {
			if( !existing ) {
				return false;
			}
			if( !v.empty() ) {
				if( v.find( value ) == v.npos ) {
					return false;
				}
			}
		}
		else if( type == PROP_FILTER_DOES_NOT_CONTAIN ) {
			if( existing ) {
				if( v.empty() ) {
					return false;
				}
				if( v.find( value ) != v.npos ) {
					return false;
				}
			}
		}
		else {		// compare number
			double v0 = todouble( value.c_str(), 0.0 );
			double v1 = double();
			if( !isdouble( v.c_str(), &v1 ) ) {
				return false;
			}

			if( type == PROP_FILTER_IS_MORE_THAN ) {
				if( v1 <= v0 ) {
					return false;
				}
			}
			else if( type == PROP_FILTER_IS_NOT_MORE_THAN ) {
				if( v1 > v0 ) {
					return false;
				}
			}
			else if( type == PROP_FILTER_IS_LESS_THAN ) {
				if( v1 >= v0 ) {
					return false;
				}

			}
			else if( type == PROP_FILTER_IS_NOT_LESS_THAN ) {
				if( v1 < v0 ) {
					return false;
				}
			}
		}
	}

	return true;
}
