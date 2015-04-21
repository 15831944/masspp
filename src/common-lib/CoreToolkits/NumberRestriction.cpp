/**
 * @file NumberRestriction.cpp
 * @brief implements of NumberRestriction class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NumberRestriction.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
NumberRestriction::NumberRestriction() {
}

// destructor
NumberRestriction::~NumberRestriction() {
}

// reset
void NumberRestriction::reset() {
	m_minInclusive.reset();
	m_maxInclusive.reset();
	m_minExclusive.reset();
	m_maxExclusive.reset();
}

// set inclusive min value
void NumberRestriction::setInclusiveMinValue( const double value ) {
	m_minInclusive = value;
}

// judge whether to have inclusive min value
bool NumberRestriction::hasInclusiveMinValue() {
	if( m_minInclusive ) {
		return true;
	}
	return false;
}

// get inclusive min value
double NumberRestriction::getInclusiveMinValue( const double dfValue ) {
	if( m_minInclusive ) {
		return m_minInclusive.get();
	}
	return dfValue;
}

// set inclusive max value
void NumberRestriction::setInclusiveMaxValue( const double value ) {
	m_maxInclusive = value;
}

// judge whether to have inclusive max value
bool NumberRestriction::hasInclusiveMaxValue() {
	if( m_maxInclusive ) {
		return true;
	}
	return false;
}

// get inclusive max value
double NumberRestriction::getInclusiveMaxValue( const double dfValue ) {
	if( m_maxInclusive ) {
		return m_maxInclusive.get();
	}
	return dfValue;
}

// set exclusive min value
void NumberRestriction::setExclusiveMinValue( const double value ) {
	m_minExclusive = value;
}

// judge whether to have exclusive min value
bool NumberRestriction::hasExclusiveMinValue() {
	if( m_minExclusive ) {
		return true;
	}
	return false;
}

// get exclusive min value
double NumberRestriction::getExclusiveMinValue( const double dfValue ) {
	if( m_minExclusive ) {
		return m_minExclusive.get();
	}
	return dfValue;
}

// set exclusive max value
void NumberRestriction::setExclusiveMaxValue( const double value ) {
	m_maxExclusive = value;
}

// judge whether to have exclusive max value
bool NumberRestriction::hasExclusiveMaxValue() {
	if( m_maxExclusive ) {
		return true;
	}
	return false;
}

// get exclusive max value
double NumberRestriction::getExclusiveMaxValue( const double dfValue ) {
	if( m_maxExclusive ) {
		return m_maxExclusive.get();
	}
	return dfValue;
}

// get lower bound value
double NumberRestriction::getLowerBound( const double dfValue ) {
	if( m_minInclusive ) {
		double v0 = m_minInclusive.get();
		if( m_minExclusive ) {
			double v1 = m_minExclusive.get();

			// compare
			return MAX( v0, v1 );
		 }
		return v0;
	}
	else if( m_minExclusive ) {
		double v1 = m_minExclusive.get();
		return v1;
	}

	// default value
	return dfValue;
}

// get upper bound value
double NumberRestriction::getUpperBound( const double dfValue ) {
	if( m_maxInclusive ) {
		double v0 = m_maxInclusive.get();
		if( m_maxExclusive ) {
			double v1 = m_maxExclusive.get();

			// compare
			return MIN( v0, v1 );
		 }
		return v0;
	}
	else if( m_maxExclusive ) {
		double v1 = m_maxExclusive.get();
		return v1;
	}

	// default value
	return dfValue;	
}

// check double range
bool NumberRestriction::checkDoubleValue( const double value ) {
	// min inclusive
	if( m_minInclusive ) {
		if( value < m_minInclusive.get() ) {
			return false;
		}
	}

	// max inclusive
	if( m_maxInclusive ) {
		if( value > m_maxInclusive.get() ) {
			return false;
		}
	}

	// min exclusive
	if( m_minExclusive ) {
		if( value <= m_minExclusive.get() ) {
			return false;
		}
	}

	// max exclusive
	if( m_maxExclusive ) {
		if( value >= m_maxExclusive.get() ) {
			return false;
		}
	}

	return true;
}

// check int range
bool NumberRestriction::checkIntValue( const int value ) {
	// min inclusive
	if( m_minInclusive ) {
		if( value < roundnum( m_minInclusive.get() ) ) {
			return false;
		}
	}

	// max inclusive
	if( m_maxInclusive ) {
		if( value > roundnum( m_maxInclusive.get() ) ) {
			return false;
		}
	}

	// min exclusive
	if( m_minExclusive ) {
		if( value <= roundnum( m_minExclusive.get() ) ) {
			return false;
		}
	}

	// max exclusive
	if( m_maxExclusive ) {
		if( value >= roundnum( m_maxExclusive.get() ) ) {
			return false;
		}
	}

	return true;
}

// get range string
std::string NumberRestriction::getRangeStr( const char* name ) {
	// string
	std::string range;

	// variables
	boost::optional< double > v;
	bool inclusive = false;

	// get min value
	if( m_minExclusive ) {
		v = m_minExclusive.get();
	}
	if( m_minInclusive ) {
		if( !v || m_minInclusive.get() > v.get() ) {
			v = m_minInclusive.get();
			inclusive = true;
		}
	}

	// min value
	if( v ) {

		if (m_maxExclusive || m_maxInclusive)
		{
			range = FMT( "%g <", v.get() );
			if( inclusive ) {
				range.append( "=" );
			}
			range.append( " " );
			range.append( NVL( name, "" ) );
		}
		else
		{
			// In the case where no maximum value is going to be expressed
			// in the range string, it is better to put the name first.
			range.append( NVL( name, "" ) );
			range.append( " " );
			if( inclusive ) {
				range += FMT( ">= %g", v.get() );
			}
			else
			{
				range += FMT( "> %g", v.get() );
			}
		}
	}

	// get max value
	v.reset();
	inclusive = false;

	if( m_maxExclusive ) {
		v = m_maxExclusive.get();
	}
	if( m_maxInclusive ) {
		if( !v || m_maxInclusive.get() < v.get() ) {
			v = m_maxInclusive.get();
			inclusive = true;
		}
	}

	// print maxvalue
	if( v ) {
		if( range.empty() ) {
			range = NVL( name, "" );
		}
		range.append( " <" );
		if( inclusive ) {
			range.append( "=" );
		}
		range.append( FMT( " %g", v.get() ) );
	}

	return range;
}
