/**
 * @file ClrSpectrum.cpp
 * @brief implements of ClrSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrSpectrum.h"

#include "ClrSpectrumBase.h"
#include "SampleWrapper.h"
#include "XYDataWrapper.h"
#include "PropertiesWrapper.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSpectrum::ClrSpectrum( SampleWrapper^ sample, System::String^ name )
		: kome::objects::Spectrum( sample == nullptr ? NULL : &( sample->getSample() ), NULL ) {
	// name
	setName( ClrObjectTool::convertString( name ).c_str() );
}

// destructor
ClrSpectrum::~ClrSpectrum() {
	ClrObjectPool::getInstance().removeSpectrum( *this );
	m_baseSpectrum = nullptr;
}

// set base spectrum
void ClrSpectrum::setBaseSpectrum( ClrSpectrumBase^ baseSpectrum ) {
	m_baseSpectrum = baseSpectrum;
}

// get base spectrum
ClrSpectrumBase^ ClrSpectrum::getBaseSpectrum() {
	ClrSpectrumBase^ baseSpectrum = m_baseSpectrum;
	return baseSpectrum;
}

// get data points
void ClrSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) {
	ClrSpectrumBase^ baseSpectrum = m_baseSpectrum;
	if( baseSpectrum != nullptr ) {
		baseSpectrum->onGetXYData( gcnew XYDataWrapper( *xyData ), minX, maxX );
	}
}

// get x range
void ClrSpectrum::onGetXRange( double* minX, double* maxX ) {
	double minXX = double();
	double maxXX = double();

	ClrSpectrumBase^ baseSpectrum = m_baseSpectrum;
	if( baseSpectrum != nullptr ) {
		baseSpectrum->onGetXRange( minXX, maxXX );
	}

	*minX = minXX;
	*maxX = maxXX;
}

// get total intensity
double ClrSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	ClrSpectrumBase^ baseSpectrum = m_baseSpectrum;
	if( baseSpectrum == nullptr ) {
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	return baseSpectrum->onGetTotalIntensity( minX, maxX );
}

// get max intensity
double ClrSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	ClrSpectrumBase^ baseSpectrum = m_baseSpectrum;
	if( baseSpectrum == nullptr ) {
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	return baseSpectrum->onGetMaxIntensity( minX, maxX );
}

// >>>>>>	@Date:2013/07/18	<Add>	A.Ozaki
//

// set the flag of request load data
void	ClrSpectrum::onSetRequestLoadData( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	baseSpectrum->onSetRequestLoadData( );

	return;
}

// set the flag of request load data
void	ClrSpectrum::onResetRequestLoadData( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	baseSpectrum->onResetRequestLoadData( );

	return;
}

// check the flag of request load data
bool	ClrSpectrum::onIsRequestLoadData( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	return	baseSpectrum->onIsRequestLoadData( );
}

// set the flag of first access
void	ClrSpectrum::onSetFirstAccess( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	baseSpectrum->onSetFirstAccess( );

	return;
}

// reset the flag of first access
void	ClrSpectrum::onResetFirstAccess( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}
	baseSpectrum->onResetFirstAccess( );

	return;
}

// check the flag of first access
bool	ClrSpectrum::onIsFirstAccess( void  )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}

	return	baseSpectrum->onIsFirstAccess( );
}

// load data
bool	ClrSpectrum::onLoadData( void )
{
	ClrSpectrumBase^	baseSpectrum = m_baseSpectrum;
	if  ( nullptr == baseSpectrum )
	{
		throw gcnew System::Exception( "There is no spectrum data object." );
	}

	return	baseSpectrum->onLoadData( );
}

//
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
