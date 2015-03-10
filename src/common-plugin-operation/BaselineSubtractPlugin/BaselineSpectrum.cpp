/**
 * @file BaselineSpectrum.cpp
 * @brief implements of BaselineSpectrum class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BaselineSpectrum.h"


using namespace kome::baseline;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BaselineSpectrum::BaselineSpectrum(
		kome::objects::Spectrum& spec,		
		kome::plugin::PluginCall& fun,
		kome::objects::SettingParameterValues* settings
) : kome::objects::Spectrum( spec.getSample(), spec.getName() ), m_spec( spec ) {
	// set item
	m_item.setInfo( &fun );

	// set parameters
	if( settings != NULL ) {
		m_settings = *settings;
	}
}

// destructor
BaselineSpectrum::~BaselineSpectrum() {
}

// get xy data
void BaselineSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) {
	// check the member
	if( xyData == NULL ) {
		return;
	}

	// get data point
	kome::core::XYData* src = m_spec.getXYData();
	if( src == NULL ) {
		LOG_WARN( FMT( "Failed to get spectrum points." ) );
		return;
	}

	// set parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setSpectrum( params, m_spec );
	kome::plugin::PluginCallTool::setXYData( params, *src );
	kome::plugin::PluginCallTool::setBaseline( params, *xyData );
	kome::plugin::PluginCallTool::setSettingValues( params, m_settings );

	// get baseline
	m_item.getCall()->invoke( &params );
}

// get x range
void BaselineSpectrum::onGetXRange( double* minX, double* maxX ) {
}

// get total intensity
double BaselineSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	return -1.0;
}

// get max intensity
double BaselineSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	return -1.0;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	BaselineSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	BaselineSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	BaselineSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	BaselineSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	BaselineSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	BaselineSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	BaselineSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

