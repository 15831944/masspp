/**
 * @file BackgroundSpectrum.cpp
 * @brief implements of BackgroundSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BackgroundSpectrum.h"
#include "BackgroundSubtraction.h"
#include "BackgroundSubtractManager.h"


using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BackgroundSpectrum::BackgroundSpectrum() : kome::objects::Spectrum( NULL, "background" ) {
}

// destructor
BackgroundSpectrum::~BackgroundSpectrum() {
}

// get xy data
void BackgroundSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) {
	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// background subtract
	BackgroundSubtraction* bs = mgr.getCurrentBackgroundSubtraction();
	if( bs == NULL ) {
		return;
	}

	// get xy data
	const unsigned int num = bs->getNumberOfPoints();
	for( unsigned int i = 0; i < num; i++ ) {
		const double x = bs->getX( i );
		const double y = bs->getY( i );

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}
}

// get x range
void BackgroundSpectrum::onGetXRange( double* minX, double* maxX ) {
	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// background subtract
	BackgroundSubtraction* bs = mgr.getCurrentBackgroundSubtraction();
	if( bs == NULL ) {
		return;
	}

	// get range
	const unsigned int num = bs->getNumberOfPoints();
	if( num > 0 ) {
		if( minX != NULL ) {
			*minX = bs->getX( 0 );
		}
		if( maxX != NULL ) {
			*maxX = bs->getX( num - 1 );
		}
	}
}

// get total intensity
double BackgroundSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// total intensity
	double tic = 0.0;

	// background subtract
	BackgroundSubtraction* bs = mgr.getCurrentBackgroundSubtraction();
	if( bs == NULL ) {
		return tic;
	}

	// get total intensity
	const unsigned int num = bs->getNumberOfPoints();
	for( unsigned int i = 0; i < num; i++ ) {
		const double x = bs->getX( i );
		const double y = bs->getY( i );

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			tic += y;
		}
	}

	return tic;
}

// get max intensity
double BackgroundSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// max intensity
	double bpc = 0.0;

	// background subtract
	BackgroundSubtraction* bs = mgr.getCurrentBackgroundSubtraction();
	if( bs == NULL ) {
		return bpc;
	}

	// get total intensity
	const unsigned int num = bs->getNumberOfPoints();
	for( unsigned int i = 0; i < num; i++ ) {
		const double x = bs->getX( i );
		const double y = bs->getY( i );

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			bpc = MAX( bpc, y );
		}
	}

	return bpc;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	BackgroundSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	BackgroundSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	BackgroundSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	BackgroundSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	BackgroundSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	BackgroundSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	BackgroundSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki
