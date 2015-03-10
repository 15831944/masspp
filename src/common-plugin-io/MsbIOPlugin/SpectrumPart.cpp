/**
 * @file SpectrumPart.cpp
 * @brief implements of SpectrumPart class
 *
 * @author S.Tanaka
 * @date 2007.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumPart.h"


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectrumPart::SpectrumPart() {
	m_mz = 0.0f;
	m_minMz = 0.0f;
	m_maxMz = 0.0f;
	m_totalIntensity = 0.0f;
	m_minIntensity = 0.0f;
	m_maxIntensity = 0.0f;
	m_length = 0;
	m_msAlign = 0;
	m_intAlign = 0;
}

// destructor
SpectrumPart::~SpectrumPart() {
}

// set m/z
void SpectrumPart::setMz( const float mz ) {
	m_mz = mz;
}

// get m/z
float SpectrumPart::getMz() {
	return m_mz;
}

// set min m/z
void SpectrumPart::setMinMz( const float mz ) {
	m_minMz = mz;
}

// get min m/z
float SpectrumPart::getMinMz() {
	return m_minMz;
}

// set max m/z
void SpectrumPart::setMaxMz( const float mz ) {
	m_maxMz = mz;
}

// get max m/z
float SpectrumPart::getMaxMz() {
	return m_maxMz;
}

// set total intensity
void SpectrumPart::setTotalIntensity( const float totalIntensity ) {
	m_totalIntensity = totalIntensity;
}

// get total intensity
float SpectrumPart::getTotalIntensity() {
	return m_totalIntensity;
}

// set min intensity
void SpectrumPart::setMinIntensity( const float intensity ) {
	m_minIntensity = intensity;
}

// get min intensity
float SpectrumPart::getMinIntensity() {
	return m_minIntensity;
}

// set max intensity
void SpectrumPart::setMaxIntensity( const float intensity ) {
	m_maxIntensity = intensity;
}

// get max intensity
float SpectrumPart::getMaxIntensity() {
	return m_maxIntensity;
}

// set the number of points
void SpectrumPart::setLength( const unsigned int length ) {
	m_length = length;
}

// get the number of points
unsigned int SpectrumPart::getLength() {
	return m_length;
}

// set m/z data align
void SpectrumPart::setMsAlign( const unsigned int align ) {
	m_msAlign = align;
}

// get m/z data align
unsigned int SpectrumPart::getMsAlign() {
	return m_msAlign;
}

// set intensity data align
void SpectrumPart::setIntAlign( const unsigned int align ) {
	m_intAlign = align;
}

// get intensity data alig
unsigned int SpectrumPart::getIntAlign() {
	return m_intAlign;
}
