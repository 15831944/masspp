/**
 * @file AlignmentWrapper.cpp
 * @brief implements of AlignmentWrapper class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "AlignmentWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AlignmentWrapper::AlignmentWrapper( kome::operation::Alignment& align )
		: m_align( align ) {
}

// destrucotr
AlignmentWrapper::~AlignmentWrapper() {
}

// get search engine object
kome::operation::Alignment& AlignmentWrapper::getAlignment() {
	return m_align;
}

// prepare
void AlignmentWrapper::prepare( SampleWrapper^ standard, SampleWrapper^ treatment, ProgressWrapper^ progress ) {
	m_align.prepare(
		ClrObjectTool::getSample( standard ),
		ClrObjectTool::getSample( treatment ),
		ClrObjectTool::getProgress( progress )
	);
}

// convert RT
double AlignmentWrapper::convertRt( double rt, double mz ) {
	return m_align.convertRt( rt, mz );
}

// convert m/z
double AlignmentWrapper::convertMz( double rt, double mz ) {
	return m_align.convertMz( rt, mz );
}

// invert RT
double AlignmentWrapper::invertRt( double rt, double mz ) {
	return m_align.invertRt( rt, mz );
}

// invert m/z
double AlignmentWrapper::invertMz( double rt, double mz ) {
	return m_align.invertMz( rt, mz );
}

