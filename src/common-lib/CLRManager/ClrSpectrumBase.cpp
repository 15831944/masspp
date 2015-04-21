/**
 * @file ClrSpectrumBase.cpp
 * @brief implements of ClrSpectrumBase class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrSpectrumBase.h"

#include "SampleWrapper.h"
#include "XYDataWrapper.h"
#include "PropertiesWrapper.h"
#include "ClrSpectrum.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSpectrumBase::ClrSpectrumBase(
		SampleWrapper^ sample,
		System::String^ name
 ) : SpectrumWrapper( ClrObjectPool::getInstance().createSpectrum( sample, name ) ) {
	( (ClrSpectrum&)m_spec ).setBaseSpectrum( this );
}

// destructor
ClrSpectrumBase::~ClrSpectrumBase() {
	ClrObjectPool::getInstance().destroySpectrum( (ClrSpectrum&)m_spec );
}

// >>>>>>	@Date:2013/07/18	<Add>	A.Ozaki
//

// set the flag of request load data (virtual)
void	ClrSpectrumBase::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	ClrSpectrumBase::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	ClrSpectrumBase::onIsRequestLoadData( void )
{
	return	false;
}

// set the flag of first access (virtual)
void	ClrSpectrumBase::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	ClrSpectrumBase::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	ClrSpectrumBase::onIsFirstAccess( void  )
{
	return	false;
}

// load data (virtual)
bool	ClrSpectrumBase::onLoadData( void )
{
	return	true;
}

//
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
