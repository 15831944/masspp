/**
 * @file ClrChromatogramBase.cpp
 * @brief implements of ClrChromatogramBase class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrChromatogramBase.h"

#include "SampleWrapper.h"
#include "XYDataWrapper.h"
#include "ClrChromatogram.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrChromatogramBase::ClrChromatogramBase(
		SampleWrapper^ sample
) : ChromatogramWrapper( ClrObjectPool::getInstance().createChromatogram( sample ) ) {
	( (ClrChromatogram&)m_chromatogram ).setBaseChromatogram( this );
}

// destructor
ClrChromatogramBase::~ClrChromatogramBase() {
	ClrObjectPool::getInstance().destroyChromatogram( (ClrChromatogram&)m_chromatogram );
}
