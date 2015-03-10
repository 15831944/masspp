/**
 * @file PngManager.cpp
 * @brief implements of PngManager class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PngManager.h"

#define MARGIN_LEFT			100
#define MARGIN_TOP			15
#define MARGIN_RIGHT		15
#define MARGIN_BOTTOM	    50
#define GRAPH_MARGIN_TOP	15

#define SPEC_COLOR	  kome::img::ColorTool::RED
#define CHROM_COLOR	  kome::img::ColorTool::BLUE


using namespace kome::io::png;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PngManager::PngManager() {
}

// destructor
PngManager::~PngManager() {
}

// save spectra
bool PngManager::saveSpectra(
		kome::objects::DataSet& dataSet,
		const char* path,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// progress
	unsigned int num = dataSet.getNumberOfSpectra();
	progress.setRange( 0, 2 * num + 1 );
	progress.setPosition( 0 );

	// get spectrum
	kome::objects::OverlappingSpectrum dummySpec;
	for( unsigned int i = 0; i < num; i++ ) {
		kome::objects::Spectrum* tmpSpec = dataSet.getSpectrum( i );

		dummySpec.addSpectrum( tmpSpec );
		progress.setPosition( i + 1 );
	}
	kome::objects::Spectrum* baseSpec = &dummySpec;
	if( num == 1 ) {
		baseSpec = dataSet.getSpectrum( 0 );
	}

	// size
	int width = 480;
	int height = 360;
	if( settings != NULL ) {
		width = settings->getIntValue( "width", width );
		height = settings->getIntValue( "height", height );
	}

	// graphics
	kome::img::CairoGraphics g0( width, height );
	g0.setGraphPosition(
		MARGIN_LEFT,
		MARGIN_TOP,
		width - MARGIN_RIGHT,
		height - MARGIN_BOTTOM
	);
	g0.setGraphMarginTop( GRAPH_MARGIN_TOP );
	g0.setDisplayedRange(
		baseSpec->getMinX(),
		baseSpec->getMaxX(),
		0.0,
		baseSpec->getMaxIntensity()
	);

	// draw
	COLOR color = g0.getColor();
	g0.setColor( kome::img::ColorTool::BLACK );
	kome::plugin::PluginCallTool::drawSpectrumWindowBg( *baseSpec, g0, NULL );
	kome::plugin::PluginCallTool::drawSpectrumBg( *baseSpec, g0, NULL );

	g0.setColor( SPEC_COLOR );
	for( unsigned int i = 0; i < num && !progress.isStopped(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		kome::plugin::PluginCallTool::drawSpectrum( *spec, g0, NULL );
		progress.setPosition( num + i );
	}

	g0.setColor( kome::img::ColorTool::BLACK );
	kome::plugin::PluginCallTool::drawSpectrumFg( *baseSpec, g0, NULL );
	kome::plugin::PluginCallTool::drawSpectrumWindowFg( *baseSpec, g0, NULL );

	g0.setColor( color );
	
	if( progress.isStopped() ) { // @date 2013.11.22 <Add> M.Izumi
		return true;
	}

	// save
	bool ret = g0.savePng( path );
	if( ret ) {
		progress.fill();
	}

	return ret;
}

// save chromatogram
bool PngManager::saveChromatogram(
		kome::objects::DataSet& dataSet,
		const char* path,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// progress
	progress.setRange( 0, 3 );
	progress.setPosition( 0 );

	// get chromatogram
	kome::objects::Chromatogram *chrom = dataSet.getChromatogram(0);	// @date <Add> 2011/11/30 OKADA
	
	kome::core::DataPoints dps;
	(*chrom).getXYData( &dps, false );	// @date <Add> 2011/11/30 OKADA

	progress.setPosition( 1 );
	if( progress.isStopped() ) {
		return true;
	}

	// size
	int width = 480;
	int height = 360;
	if( settings != NULL ) {
		width = settings->getIntValue( "width", width );
		height = settings->getIntValue( "height", height );
	}

	// graphics
	kome::img::CairoGraphics g( width, height );
	g.setGraphPosition(
		MARGIN_LEFT,
		MARGIN_TOP,
		width - MARGIN_RIGHT,
		height - MARGIN_BOTTOM
	);
	
	// @date 2011.08.25 <Add> M.Izumi ->
	// SPEC 82173 : Cannot export a file in MIS/2D Peaks List/PNG format 
	double minX = dps.getMinX();
	double maxX = dps.getMaxX();
	if( minX == maxX ){
		maxX = 0.01;
	}
	// @date 2011.083.25 <Add> M.Izumi <-

	g.setGraphMarginTop( GRAPH_MARGIN_TOP );
	g.setDisplayedRange(
		minX,
		maxX,
		0.0,
		dps.getMaxY()
	);

	// draw
	COLOR color = g.getColor();

	g.setColor( kome::img::ColorTool::BLACK );
	kome::plugin::PluginCallTool::drawChromatogramWindowBg( *chrom, g, NULL );
	kome::plugin::PluginCallTool::drawChromatogramBg( *chrom, g, NULL );

	g.setColor( CHROM_COLOR );
	kome::plugin::PluginCallTool::drawChromatogram( *chrom, g, NULL );

	g.setColor( kome::img::ColorTool::BLACK );
	kome::plugin::PluginCallTool::drawChromatogramFg( *chrom, g, NULL );
	kome::plugin::PluginCallTool::drawChromatogramWindowFg( *chrom, g, NULL );

	g.setColor( color );

	progress.setPosition( 2 );
	if( progress.isStopped() ) {
		return true;
	}

	// save
	const bool ret = g.savePng( path );
	if( ret ) {
		progress.fill();
	}

	return ret;
}

// get instance
PngManager& PngManager::getInstance() {
	// create object (This is the only object.)
	static PngManager mgr;

	return mgr;
}
