/**
 * @file NumberRestrictionWrapper.cpp
 * @brief implements of NumberRestrictionWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "NumberRestrictionWrapper.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
NumberRestrictionWrapper::NumberRestrictionWrapper( kome::core::NumberRestriction& restriction )
		: m_restriction( restriction ) {
}

// destructor
NumberRestrictionWrapper::~NumberRestrictionWrapper() {
}

// get number restriction object
kome::core::NumberRestriction& NumberRestrictionWrapper::getNumberRestriction() {
	return m_restriction;
}

// reset
void NumberRestrictionWrapper::reset() {
	m_restriction.reset();
}

// set inclusive min value
void NumberRestrictionWrapper::setInclusiveMinValue( double value ) {
	m_restriction.setInclusiveMinValue( value );
}

// judges whether this restriction has inclusive min value
bool NumberRestrictionWrapper::hasInclusiveMinValue() {
	return m_restriction.hasInclusiveMinValue();
}

// get inclusive min value
double NumberRestrictionWrapper::getInclusiveMinValue( double dfValue ) {
	return m_restriction.getInclusiveMinValue( dfValue );
}

// set inclusive max value
void NumberRestrictionWrapper::setInclusiveMaxValue( double value ) {
	m_restriction.setInclusiveMaxValue( value );
}

// judges whether this restriction has inclusive max value
bool NumberRestrictionWrapper::hasInclusiveMaxValue() {
	return m_restriction.hasInclusiveMaxValue();
}

// get inclusive max value
double NumberRestrictionWrapper::getInclusiveMaxValue( double dfValue ) {
	return m_restriction.getInclusiveMaxValue( dfValue );
}

// set exclusive min value
void NumberRestrictionWrapper::setExclusiveMinValue( double value ) {
	m_restriction.setExclusiveMinValue( value );
}

// judges whether this restriction has exclusive min value
bool NumberRestrictionWrapper::hasExclusiveMinValue() {
	return m_restriction.hasExclusiveMinValue();
}

// get exclusive min value
double NumberRestrictionWrapper::getExclusiveMinValue( double dfValue ) {
	return m_restriction.getExclusiveMinValue( dfValue );
}

// set exclusive max value
void NumberRestrictionWrapper::setExclusiveMaxValue( double value ) {
	m_restriction.setExclusiveMaxValue( value );
}

// judges whether this restriction has exclusive max value
bool NumberRestrictionWrapper::hasExclusiveMaxValue() {
	return m_restriction.hasExclusiveMaxValue();
}

// get exclusive max value
double NumberRestrictionWrapper::getExclusiveMaxValue( double dfValue ) {
	return m_restriction.getExclusiveMaxValue( dfValue );
}

// get lower bound
double NumberRestrictionWrapper::getLowerBound( double dfValue ) {
	return m_restriction.getLowerBound( dfValue );
}

// get upper bound
double NumberRestrictionWrapper::getUpperBound( double dfValue ) {
	return m_restriction.getUpperBound( dfValue );
}

// checks double value
bool NumberRestrictionWrapper::checkDoubleValue( double value ) {
	return m_restriction.checkDoubleValue( value );
}

// checks int value
bool NumberRestrictionWrapper::checkIntValue( int value ) {
	return m_restriction.checkIntValue( value );
}

// get string meanings range
System::String^ NumberRestrictionWrapper::getRangeStr( System::String^ name ){
	return gcnew System::String(
		m_restriction.getRangeStr(
			ClrObjectTool::convertString( name ).c_str()
		).c_str()
	);
}
