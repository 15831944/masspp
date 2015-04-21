/**
 * @file ClrPluginCallTool.cpp
 * @brief implements ClrPluginCallTool class
 *
 * @author S.Tanaka
 * @date 2007.05.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrPluginCallTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SAMPLE_SET_PARAM_NAME			   "sample set"
#define SAMPLE_PARAM_NAME				   "sample"
#define SPECTRUM_PARAM_NAME				 "spectrum"
#define CHROMATOGRAM_PARAM_NAME			 "chromatogram"
#define START_RANGE_PARAM_NAME			  "start"
#define END_RANGE_PARAM_NAME				"end"
#define XY_DATA_PARAM_NAME					"xy data"
#define PEAKS_PARAM_NAME					"peaks"
#define PEAKS_2D_PARAM_NAME				 "peaks"
#define DATA_SET_PARAM_NAME				 "data set"
#define GROUP_PARAM_NAME					"group"
#define PARENT_PEAKS_PARAM_NAME			 "parent peaks"
#define BASELINE_PARAM_NAME				 "baseline"
#define UPDATED_XY_DATA_PARAM_NAME		  "updated"
#define UPDATED_PEAKS_PARAM_NAME			"updated"
#define UPDATED_PEAKS_2D_PARAM_NAME			"updated"
#define CONTROL_SPEC_PARAM_NAME			 "control spectrum"
#define CONTROL_CHROM_PARAM_NAME			"control chromatogram"
#define CONTROL_GROUP_PARAM_NAME			"control group"
#define CONTROL_XY_DATA_PARAM_NAME		  "control xy data"
#define PATH_PARAM_NAME					 "path"
#define PROGRESS_PARAM_NAME				 "progress"
#define MOUSE_EVENT_PARAM_NAME			  "event"
#define KEY_EVENT_PARAM_NAME				"event"
#define SIZE_EVENT_PARAM_NAME			   "event"
#define MOVE_EVENT_PARAM_NAME			   "event"
#define GRAPHICS_PARAM_NAME				 "graphics"
#define DATA_MAP_PARAM_NAME				 "data map"
#define SETTINGS_PARAM_NAME				 "settings"
#define ACTIVE_OBJECT_PARAM_NAME			"active"


// open file
void ClrPluginCallTool::openFile( System::String^ path ) {
	kome::plugin::PluginCallTool::openFile(
		ClrObjectTool::convertString( path ).c_str()
	);
}

// on save sample
void ClrPluginCallTool::onSaveSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onSaveSample( *s );
	}
}

// open spectrum
void ClrPluginCallTool::openSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::openSpectrum( *s );
	}
}

// open chromatogram
void ClrPluginCallTool::openChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::openChromatogram( *c );
	}
}

// open data map
void ClrPluginCallTool::openDataMap( DataMapInfoWrapper^ dataMap ) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	if( dm != NULL ) {
		kome::plugin::PluginCallTool::openDataMap( *dm );
	}
}

// on open sample set
void ClrPluginCallTool::onOpenSampleSet( SampleSetWrapper^ sampleSet ) {
	kome::objects::SampleSet* ss = ClrObjectTool::getSampleSet( sampleSet );
		
	if( ss != NULL ) {
		kome::plugin::PluginCallTool::onOpenSampleSet( *ss );
	}
}

// on open sample
void ClrPluginCallTool::onOpenSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onOpenSample( *s );
	}
}

// on close sample
void ClrPluginCallTool::onCloseSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onCloseSample( *s );
	}
}

// can exit
bool ClrPluginCallTool::canExit() {
	return kome::plugin::PluginCallTool::canExit();
}

// on update spectrum
void ClrPluginCallTool::onUpdateSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpectrum( *s );
	}
}

// on update chromatogram
void ClrPluginCallTool::onUpdateChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromatogram( *c );
	}
}

// on update data map
void ClrPluginCallTool::onUpdateDataMap( DataMapInfoWrapper^ dataMap ) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	if( dm != NULL ) {
		kome::plugin::PluginCallTool::onUpdateDataMap( *dm );
	}
}

// on update data set
void ClrPluginCallTool::onUpdateDataSet( DataSetWrapper^ dataSet ) {
	kome::objects::DataSet* s = ClrObjectTool::getDataSet( dataSet );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onUpdateDataSet( *s );
	}
}

// on update group
void ClrPluginCallTool::onUpdateGroup( DataGroupNodeWrapper^ group ) {
	kome::objects::DataGroupNode* g = (kome::objects::DataGroupNode*)ClrObjectTool::getDataSet( group );
	if( g != NULL ) {
		kome::plugin::PluginCallTool::onUpdateGroup( *g );
	}
}

// on update sample
void ClrPluginCallTool::onUpdateSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = (kome::objects::Sample*)ClrObjectTool::getSample( sample );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSample( *s );
	}
}

// on detect spectrum peaks
void ClrPluginCallTool::detectSpecPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress ) {
	kome::objects::DataSet* ds = ClrObjectTool::getDataSet( dataSet );
	kome::core::Progress* p = ClrObjectTool::getProgress( progress );

	if( ds != NULL ) {
		kome::plugin::PluginCallTool::detectSpecPeaks( *ds, p );
	}
}

// on update spectrum peaks
void ClrPluginCallTool::onUpdateSpecPeaks( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( *s );
	}
}

// on detect chromatogram peaks
void ClrPluginCallTool::detectChromPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress ) {
	kome::objects::DataSet* ds = ClrObjectTool::getDataSet( dataSet );
	kome::core::Progress* p = ClrObjectTool::getProgress( progress );

	if( ds != NULL ) {
		kome::plugin::PluginCallTool::detectChromPeaks( *ds, p );
	}
}

// on update chromatogram peaks
void ClrPluginCallTool::onUpdateChromPeaks( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromPeaks( *c );
	}
}

// on detect 2D peaks
void ClrPluginCallTool::detect2DPeaks( DataGroupNodeWrapper^ group, ProgressWrapper^ progress ) {
	kome::objects::DataGroupNode* g = ClrObjectTool::getDataGroupNode( group );
	kome::core::Progress* p = ClrObjectTool::getProgress( progress );

	if( g != NULL ) {
		kome::plugin::PluginCallTool::detect2DPeaks( *g, p );
	}
}

// on update 2D peaks
void ClrPluginCallTool::onUpdate2DPeaks( DataGroupNodeWrapper^ group ) {
	kome::objects::DataGroupNode* g = ClrObjectTool::getDataGroupNode( group );

	if( g != NULL ) {
		kome::plugin::PluginCallTool::onUpdate2DPeaks( *g );
	}
}

// on update peaks matrix
void ClrPluginCallTool::onUpdatePeaksMatrix() {
	kome::plugin::PluginCallTool::onUpdatePeaksMatrix();
}

// open matrix
void ClrPluginCallTool::openMatrix() {
	kome::plugin::PluginCallTool::openMatrix();
}

// close matrix
void ClrPluginCallTool::closeMatrix() {
	kome::plugin::PluginCallTool::closeMatrix();
}

// on commit spectrum peaks
void ClrPluginCallTool::onCommitSpecPeaks(
		SpectrumWrapper^ spec,
		XYDataWrapper^ xyData,
		PeaksWrapper^ src,
		PeaksWrapper^ dst
) {
	// objects
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	kome::objects::Peaks* sp = ClrObjectTool::getPeaks( src );
	kome::objects::Peaks* dp = ClrObjectTool::getPeaks( dst );

	// broadcast
	if( s != NULL && xyd != NULL && sp != NULL && dp != NULL ) {
		kome::plugin::PluginCallTool::onCommitSpecPeaks( *s, *xyd, *sp, *dp );
	}
}

// on commit spectrum peaks
void ClrPluginCallTool::onCommitChromPeaks(
		ChromatogramWrapper^ chrom,
		XYDataWrapper^ xyData,
		PeaksWrapper^ src,
		PeaksWrapper^ dst
) {
	// objects
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	kome::objects::Peaks* sp = ClrObjectTool::getPeaks( src );
	kome::objects::Peaks* dp = ClrObjectTool::getPeaks( dst );

	// broadcast
	if( c != NULL && xyd != NULL && sp != NULL && dp != NULL ) {
		kome::plugin::PluginCallTool::onCommitChromPeaks( *c, *xyd, *sp, *dp );
	}
}

// on rollback spectrum peaks
void ClrPluginCallTool::onRollbackSpecPeaks(
		SpectrumWrapper^ spec,
		XYDataWrapper^ xyData,
		PeaksWrapper^ peaks,
		PeaksWrapper^ canceledPeaks
) {
	// objects
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	kome::objects::Peaks* sp = ClrObjectTool::getPeaks( peaks );
	kome::objects::Peaks* dp = ClrObjectTool::getPeaks( canceledPeaks );

	// broadcast
	if( s != NULL && xyd != NULL && sp != NULL && dp != NULL ) {
		kome::plugin::PluginCallTool::onRollbackSpecPeaks( *s, *xyd, *sp, *dp );
	}
}

// on rollback spectrum peaks
void ClrPluginCallTool::onRollbackChromPeaks(
		ChromatogramWrapper^ chrom,
		XYDataWrapper^ xyData,
		PeaksWrapper^ peaks,
		PeaksWrapper^ canceledPeaks
) {
	// objects
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	kome::objects::Peaks* sp = ClrObjectTool::getPeaks( peaks );
	kome::objects::Peaks* dp = ClrObjectTool::getPeaks( canceledPeaks );

	// broadcast
	if( c != NULL && xyd != NULL && sp != NULL && dp != NULL ) {
		kome::plugin::PluginCallTool::onRollbackChromPeaks( *c, *xyd, *sp, *dp );
	}
}

// commit spectrum peaks
void ClrPluginCallTool::commitSpecPeaks( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::commitSpecPeaks( *s );
	}
}

// commit chromatogram peaks
void ClrPluginCallTool::commitChromPeaks( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::commitChromPeaks( *c );
	}
}

// rollback spectrum peaks
void ClrPluginCallTool::rollbackSpecPeaks( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::rollbackSpecPeaks( *s );
	}
}

// rollback chromatogram peaks
void ClrPluginCallTool::rollbackChromPeaks( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::rollbackChromPeaks( *c );
	}
} 

// on open spectrum
bool ClrPluginCallTool::onOpenSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s == NULL ) {
		return false;
	}
	return kome::plugin::PluginCallTool::onOpenSpectrum( *s );
}

// on open chromatogram
bool ClrPluginCallTool::onOpenChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c == NULL ) {
		return false;
	}
	return kome::plugin::PluginCallTool::onOpenChromatogram( *c );
}

// on open data map
bool ClrPluginCallTool::onOpenDataMap( DataMapInfoWrapper^ dataMap ) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	if( dm == NULL ) {
		return false;
	}
	return kome::plugin::PluginCallTool::onOpenDataMap( *dm );
}

// on close spectrum
void ClrPluginCallTool::onCloseSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::onCloseSpectrum( *s, NULL );
	}
}

// on close chromatogram
void ClrPluginCallTool::onCloseChromatogram( ChromatogramWrapper^ chrom ) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *c, NULL );
	}
}

// on close data map
void ClrPluginCallTool::onCloseDataMap( DataMapInfoWrapper^ dataMap ) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	if( dm != NULL ) {
		kome::plugin::PluginCallTool::onCloseDataMap( *dm, NULL );
	}
}

// on activate
void ClrPluginCallTool::onActivate( ClrVariant^ obj ) {
	kome::objects::Variant var;
	ClrObjectTool::convertVariantFromCLR( &var, obj );
	kome::plugin::PluginCallTool::onActivate( var );
}

// on update settings
void ClrPluginCallTool::onSettings() {
	kome::plugin::PluginCallTool::onSettings();
}

// refresh
void ClrPluginCallTool::refresh() {
	kome::plugin::PluginCallTool::refresh();
}

// change spectrum range
void ClrPluginCallTool::changeSpectrumRange(
	SpectrumWrapper^ spec,
	double start,
	double end,
	long long canvas
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::changeSpectrumRange( *s, start, end, canvas );
	}
}

// change chromatogram range
void ClrPluginCallTool::changeChromatogramRange(
	ChromatogramWrapper^ chrom,
	double start,
	double end,
	long long canvas
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::changeChromatogramRange( *c, start, end, canvas );
	}
}

// spectrum range action
void ClrPluginCallTool::actSpectrumRange(
	SpectrumWrapper^ spec,
	double start,
	double end
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	if( s != NULL ) {
		kome::plugin::PluginCallTool::actSpectrumRange( *s, start, end );
	}
}

// chromatogram range action
void ClrPluginCallTool::actChromatogramRange(
	ChromatogramWrapper^ chrom,
	double start,
	double end,
	kome::core::Progress* progress
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	if( c != NULL ) {
		kome::plugin::PluginCallTool::actChromatogramRange( *c, start, end, progress );
	}
}

// close all spectrum
void ClrPluginCallTool::closeAllSpectrum() {
	kome::plugin::PluginCallTool::closeAllSpectrum();
}

// close all chromatogram
void ClrPluginCallTool::closeAllChromatogram() {
	kome::plugin::PluginCallTool::closeAllChromatogram();
}

// close all data map
void ClrPluginCallTool::closeAllDataMap() {
	kome::plugin::PluginCallTool::closeAllDataMap();
}

// draw spectrum
void ClrPluginCallTool::drawSpectrum(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawSpectrum( *s, *g2, NULL );
	}
}

// prepare to draw spectrum
void ClrPluginCallTool::prepareDrawSpectrum(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::prepareDrawSpectrum( *s, *g2, NULL );
	}
}

// draw spectrum foreground
void ClrPluginCallTool::drawSpectrumFg(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawSpectrumFg( *s, *g2, NULL );
	}
}

// draw spectrum background
void ClrPluginCallTool::drawSpectrumBg(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawSpectrumBg( *s, *g2, NULL );
	}
}

// draw spectrum window foreground
void ClrPluginCallTool::drawSpectrumWindowFg(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawSpectrumWindowFg( *s, *g2, NULL );
	}
}
// draw spectrum window background
void ClrPluginCallTool::drawSpectrumWindowBg(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( s != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawSpectrumWindowBg( *s, *g2, NULL );
	}
}

// draw chromatogram
void ClrPluginCallTool::drawChromatogram(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawChromatogram( *c, *g2, NULL );
	}
}

// prepare to draw chromatogram
void ClrPluginCallTool::prepareDrawChromatogram(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::prepareDrawChromatogram( *c, *g2, NULL );
	}
}

// draw chromatogram foreground
void ClrPluginCallTool::drawChromatogramFg(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawChromatogramFg( *c, *g2, NULL );
	}
}

// draw chromatogram background
void ClrPluginCallTool::drawChromatogramBg(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawChromatogramBg( *c, *g2, NULL );
	}
}

// draw chromatogram window foreground
void ClrPluginCallTool::drawChromatogramWindowFg(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawChromatogramWindowFg( *c, *g2, NULL );
	}
}

// draw chromatogram window background
void ClrPluginCallTool::drawChromatogramWindowBg(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	
	if( c != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawChromatogramWindowBg( *c, *g2, NULL );
	}
}

// draw data map
void ClrPluginCallTool::drawDataMap(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawDataMap( *dm, *g2, NULL );
	}
}

// prepare to draw data map
void ClrPluginCallTool::prepareDrawDataMap(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::prepareDrawDataMap( *dm, *g2, NULL );
	}
}

// draw data map foreground
void ClrPluginCallTool::drawDataMapFg(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawDataMapFg( *dm, *g2, NULL );
	}
}

// draw data map background
void ClrPluginCallTool::drawDataMapBg(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawDataMapBg( *dm, *g2, NULL );
	}
}

// draw data map window foreground
void ClrPluginCallTool::drawDataMapWindowFg(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawDataMapWindowFg( *dm, *g2, NULL );
	}
}
// draw data map window background
void ClrPluginCallTool::drawDataMapWindowBg(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );

	if( dm != NULL && g2 != NULL ) {
		kome::plugin::PluginCallTool::drawDataMapWindowBg( *dm, *g2, NULL );
	}
}

// button down on spectrum
bool ClrPluginCallTool::onSpectrumButtonDown(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumButtonDown( *s, g2, *e, NULL );
	}
	return false;
}

// button up on spectrum
bool ClrPluginCallTool::onSpectrumButtonUp(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumButtonUp( *s, g2, *e, NULL );
	}
	return false;
}

// double click on spectrum
bool ClrPluginCallTool::onSpectrumDoubleClick(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumDoubleClick( *s, g2, *e, NULL );
	}
	return false;
}

// mouse move on spectrum
bool ClrPluginCallTool::onSpectrumMouseMove(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumMouseMove( *s, g2, *e, NULL );
	}
	return false;
}

// mouse leave from spectrum
bool ClrPluginCallTool::onSpectrumMouseLeave(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumMouseLeave( *s, g2, *e, NULL );
	}
	return false;
}

// mouse enter to spectrum
bool ClrPluginCallTool::onSpectrumMouseEnter(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumMouseEnter( *s, g2, *e, NULL );
	}
	return false;
}

// button down on chromatogram
bool ClrPluginCallTool::onChromatogramButtonDown(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramButtonDown( *c, g2, *e, NULL );
	}
	return false;
}

// button up on chromatogram
bool ClrPluginCallTool::onChromatogramButtonUp(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramButtonUp( *c, g2, *e, NULL );
	}
	return false;
}

// double click on chromatogram
bool ClrPluginCallTool::onChromatogramDoubleClick(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramDoubleClick( *c, g2, *e, NULL );
	}
	return false;
}

// mouse move on chromatogram
bool ClrPluginCallTool::onChromatogramMouseMove(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramMouseMove( *c, g2, *e, NULL );
	}
	return false;
}

// mouse leave from chromatogram
bool ClrPluginCallTool::onChromatogramMouseLeave(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramMouseLeave( *c, g2, *e, NULL );
	}
	return false;
}

// mouse enter to chromatogram
bool ClrPluginCallTool::onChromatogramMouseEnter(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( c != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramMouseEnter( *c, g2, *e, NULL );
	}
	return false;
}

// button down on data map
bool ClrPluginCallTool::onDataMapButtonDown(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapButtonDown( *dm, g2, *e, NULL );
	}
	return false;
}

// button up on data map
bool ClrPluginCallTool::onDataMapButtonUp(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapButtonUp( *dm, g2, *e, NULL );
	}
	return false;
}

// double click on data map
bool ClrPluginCallTool::onDataMapDoubleClick(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapDoubleClick( *dm, g2, *e, NULL );
	}
	return false;
}

// mouse move on data map
bool ClrPluginCallTool::onDataMapMouseMove(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapMouseMove( *dm, g2, *e, NULL );
	}
	return false;
}

// mouse leave from data map
bool ClrPluginCallTool::onDataMapMouseLeave(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapMouseLeave( *dm, g2, *e, NULL );
	}
	return false;
}

// mouse enter to data map
bool ClrPluginCallTool::onDataMapMouseEnter(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		MouseEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::MouseEvent* e = ClrObjectTool::getMouseEvent( evt );

	if( dm != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapMouseEnter( *dm, g2, *e, NULL );
	}
	return false;
}

// key down on spectrum
bool ClrPluginCallTool::onSpectrumKeyDown(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumKeyDown( *s, g2, *e, NULL );
	}
	return false;
}

// key up on spectrum
bool ClrPluginCallTool::onSpectrumKeyUp(
		SpectrumWrapper^ spec,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( s != NULL && g2 != NULL && e != NULL ) {
		return kome::plugin::PluginCallTool::onSpectrumKeyUp( *s, g2, *e, NULL );
	}
	return false;
}

// key down on chromatogram
bool ClrPluginCallTool::onChromatogramKeyDown(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( c != NULL && g2 != NULL &&  e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramKeyDown( *c, g2, *e, NULL );
	}
	return false;
}

// key up on chromatogram
bool ClrPluginCallTool::onChromatogramKeyUp(
		ChromatogramWrapper^ chrom,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::Chromatogram* c = ClrObjectTool::getChromatogram( chrom );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( c != NULL && g2 != NULL &&  e != NULL ) {
		return kome::plugin::PluginCallTool::onChromatogramKeyUp( *c, g2, *e, NULL );
	}
	return false;
}

// key down on data map
bool ClrPluginCallTool::onDataMapKeyDown(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( dm != NULL && g2 != NULL &&  e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapKeyDown( *dm, g2, *e, NULL );
	}
	return false;
}

// key up on data map
bool ClrPluginCallTool::onDataMapKeyUp(
		DataMapInfoWrapper^ dataMap,
		GraphicsWrapper^ g,
		KeyEventWrapper^ evt
) {
	kome::objects::DataMapInfo* dm = ClrObjectTool::getDataMapInfo( dataMap );
	kome::img::Graphics* g2 = ClrObjectTool::getGraphics( g );
	kome::evt::KeyEvent* e = ClrObjectTool::getKeyEvent( evt );

	if( dm != NULL && g2 != NULL &&  e != NULL ) {
		return kome::plugin::PluginCallTool::onDataMapKeyUp( *dm, g2, *e, NULL );
	}
	return false;
}

// set sample set
void ClrPluginCallTool::setSampleSet( ClrParameters^ params, SampleSetWrapper^ sampleSet ) {
	ClrVariant^ val = params->getValue( SAMPLE_SET_PARAM_NAME );
	val->type = ClrVariant::DataType::SAMPLE_SET;
	val->obj = sampleSet;
}

// set sample
void ClrPluginCallTool::setSample( ClrParameters^ params, SampleWrapper^ sample ) {
	ClrVariant^ val = params->getValue( SAMPLE_PARAM_NAME );
	val->type = ClrVariant::DataType::SAMPLE;
	val->obj = sample;
}

// set spectrum
void ClrPluginCallTool::setSpectrum( ClrParameters^ params, SpectrumWrapper^ spec ) {
	ClrVariant^ val = params->getValue( SPECTRUM_PARAM_NAME );
	val->type = ClrVariant::DataType::SPECTRUM;
	val->obj = spec;
}

// set chromatogram
void ClrPluginCallTool::setChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom ) {
	ClrVariant^ val = params->getValue( CHROMATOGRAM_PARAM_NAME );
	val->type = ClrVariant::DataType::CHROMATOGRAM;
	val->obj = chrom;
}

// set xy data
void ClrPluginCallTool::setXYData( ClrParameters^ params, XYDataWrapper^ xyData ) {
	ClrVariant^ val = params->getValue( XY_DATA_PARAM_NAME );
	val->type = ClrVariant::DataType::XY_DATA;
	val->obj = xyData;
}

// set data set
void ClrPluginCallTool::setDataSet( ClrParameters^ params, DataSetWrapper^ dataSet ) {
	ClrVariant^ val = params->getValue( DATA_SET_PARAM_NAME );
	val->type = ClrVariant::DataType::DATA_SET;
	val->obj = dataSet;
}

// set spectrum group
void ClrPluginCallTool::setGroup( ClrParameters^ params, DataGroupNodeWrapper^ group ) {
	ClrVariant^ val = params->getValue( GROUP_PARAM_NAME );
	val->type = ClrVariant::DataType::DATA_GROUP_NODE;
	val->obj = group;
}

// set peaks
void ClrPluginCallTool::setPeaks( ClrParameters^ params, PeaksWrapper^ peaks ) {
	ClrVariant^ val = params->getValue( PEAKS_PARAM_NAME );
	val->type = ClrVariant::DataType::PEAKS;
	val->obj = peaks;
}

// set 2D peaks
void ClrPluginCallTool::setPeaks2D( ClrParameters^ params, Peaks2DWrapper^ peaks ) {
	ClrVariant^ val = params->getValue( PEAKS_2D_PARAM_NAME );
	val->type = ClrVariant::DataType::PEAKS_2D;
	val->obj = peaks;
}

// set parent peaks
void ClrPluginCallTool::setParentPeaks( ClrParameters^ params, PeaksWrapper^ parentPeaks ) {
	ClrVariant^ val = params->getValue( PARENT_PEAKS_PARAM_NAME );
	val->type = ClrVariant::DataType::PEAKS;
	val->obj = parentPeaks;
}

// set baseline
void ClrPluginCallTool::setBaseline( ClrParameters^ params, XYDataWrapper^ baseline ) {
	ClrVariant^ val = params->getValue( BASELINE_PARAM_NAME );
	val->type = ClrVariant::DataType::XY_DATA;
	val->obj = baseline;
}

// set updated xy data
void ClrPluginCallTool::setUpdatedXYData( ClrParameters^ params, XYDataWrapper^ updated ) {
	ClrVariant^ val = params->getValue( UPDATED_XY_DATA_PARAM_NAME );
	val->type = ClrVariant::DataType::XY_DATA;
	val->obj = updated;
}

// set updated peaks
void ClrPluginCallTool::setUpdatedPeaks( ClrParameters^ params, PeaksWrapper^ updated ) {
	ClrVariant^ val = params->getValue( UPDATED_PEAKS_PARAM_NAME );
	val->type = ClrVariant::DataType::PEAKS;
	val->obj = updated;
}

// set updated 2d peaks
void ClrPluginCallTool::setUpdatedPeaks2d( ClrParameters^ params, PeaksWrapper^ updated ) {
	ClrVariant^ val = params->getValue( UPDATED_PEAKS_2D_PARAM_NAME );
	val->type = ClrVariant::DataType::PEAKS_2D;
	val->obj = updated;
}

// set control spectrum
void ClrPluginCallTool::setControlSpectrum( ClrParameters^ params, SpectrumWrapper^ spec ) {
	ClrVariant^ val = params->getValue( CONTROL_SPEC_PARAM_NAME );
	val->type = ClrVariant::DataType::SPECTRUM;
	val->obj = spec;
}

// set control chromatogram
void ClrPluginCallTool::setControlChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom ) {
	ClrVariant^ val = params->getValue( CONTROL_CHROM_PARAM_NAME );
	val->type = ClrVariant::DataType::CHROMATOGRAM;
	val->obj = chrom;
}

// set control group
void ClrPluginCallTool::setControlGroup( ClrParameters^ params, DataGroupNodeWrapper^ group ) {
	ClrVariant^ val = params->getValue( CONTROL_GROUP_PARAM_NAME );
	val->type = ClrVariant::DataType::DATA_GROUP_NODE;
	val->obj = group;
}

// set control xy data
void ClrPluginCallTool::setControlXYData( ClrParameters^ params, XYDataWrapper^ xyData ) {
	ClrVariant^ val = params->getValue( CONTROL_XY_DATA_PARAM_NAME );
	val->type = ClrVariant::DataType::XY_DATA;
	val->obj = xyData;
}

// set path
void ClrPluginCallTool::setPath( ClrParameters^ params, System::String^ path ) {
	ClrVariant^ val = params->getValue( PATH_PARAM_NAME );
	val->type = ClrVariant::DataType::STRING;
	val->obj = path;
}

// set start of range
void ClrPluginCallTool::setStartOfRange( ClrParameters^ params, double start ) {
	ClrVariant^ val = params->getValue( START_RANGE_PARAM_NAME );
	val->type = ClrVariant::DataType::DOUBLE;
	val->obj = start;
}

// set end of range
void ClrPluginCallTool::setEndOfRange( ClrParameters^ params, double end ) {
	ClrVariant^ val = params->getValue( END_RANGE_PARAM_NAME );
	val->type = ClrVariant::DataType::DOUBLE;
	val->obj = end;
}

// set range
void ClrPluginCallTool::setRange( ClrParameters^ params, double start, double end ) {
	setStartOfRange( params, start );
	setEndOfRange( params, end );
}

// set progress
void ClrPluginCallTool::setProgress( ClrParameters^ params, ProgressWrapper^ progress ) {
	ClrVariant^ val = params->getValue( PROGRESS_PARAM_NAME );
	val->type = ClrVariant::DataType::PROGRESS;
	val->obj = progress;
}

// set mouse event
void ClrPluginCallTool::setMouseEvent( ClrParameters^ params, MouseEventWrapper^ evt ) {
	ClrVariant^ val = params->getValue( MOUSE_EVENT_PARAM_NAME );
	val->type = ClrVariant::DataType::MOUSE_EVT;
	val->obj = evt;
}

// set key event
void ClrPluginCallTool::setKeyEvent( ClrParameters^ params, KeyEventWrapper^ evt ) {
	ClrVariant^ val = params->getValue( KEY_EVENT_PARAM_NAME );
	val->type = ClrVariant::DataType::KEY_EVT;
	val->obj = evt;
}

// set size event
void ClrPluginCallTool::setSizeEvent( ClrParameters^ params, SizeEventWrapper^ evt ) {
	ClrVariant^ val = params->getValue( SIZE_EVENT_PARAM_NAME );
	val->type = ClrVariant::DataType::SIZE_EVT;
	val->obj = evt;
}

// set move event
void ClrPluginCallTool::setMoveEvent( ClrParameters^ params, MoveEventWrapper^ evt ) {
	ClrVariant^ val = params->getValue( MOVE_EVENT_PARAM_NAME );
	val->type = ClrVariant::DataType::MOVE_EVT;
	val->obj = evt;
}

// set graphics
void ClrPluginCallTool::setGraphics( ClrParameters^ params, GraphicsWrapper^ g ) {
	ClrVariant^ val = params->getValue( GRAPHICS_PARAM_NAME );
	val->type = ClrVariant::DataType::GRAPHICS;
	val->obj = g;
}

// set data map
void ClrPluginCallTool::setDataMapInfo( ClrParameters^ params, DataMapInfoWrapper^ dataMap ) {
	ClrVariant^ val = params->getValue( DATA_MAP_PARAM_NAME );
	val->type = ClrVariant::DataType::DATA_MAP;
	val->obj = dataMap;
}

// set setting parameter values
void ClrPluginCallTool::setSettingValues( ClrParameters^ params, SettingParameterValuesWrapper^ vals ) {
	ClrVariant^ val = params->getValue( SETTINGS_PARAM_NAME );
	val->type = ClrVariant::DataType::SETTING_VALUES;
	val->obj = vals;
}

// set active object
void ClrPluginCallTool::setActiveObject( ClrParameters^ params, ClrVariant^ obj ) {
	ClrVariant^ val = params->getValue( ACTIVE_OBJECT_PARAM_NAME );
	val->type = obj->type;
	val->obj = obj->obj;
}

// get sample set
SampleSetWrapper^ ClrPluginCallTool::getSampleSet( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( SAMPLE_SET_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SAMPLE_SET ) {
		return nullptr;
	}
	return (SampleSetWrapper^)val->obj;
}

// get sample
SampleWrapper^ ClrPluginCallTool::getSample( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( SAMPLE_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SAMPLE ) {
		return nullptr;
	}
	return (SampleWrapper^)val->obj;
}

// get spectrum
SpectrumWrapper^ ClrPluginCallTool::getSpectrum( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( SPECTRUM_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SPECTRUM ) {
		return nullptr;
	}
	return (SpectrumWrapper^)val->obj;
}

// get chromatogram
ChromatogramWrapper^ ClrPluginCallTool::getChromatogram( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( CHROMATOGRAM_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::CHROMATOGRAM ) {
		return nullptr;
	}
	return (ChromatogramWrapper^)val->obj;
}

// get xy data
XYDataWrapper^ ClrPluginCallTool::getXYData( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( XY_DATA_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::XY_DATA ) {
		return nullptr;
	}
	return (XYDataWrapper^)val->obj;
}

// get data set
DataSetWrapper^ ClrPluginCallTool::getDataSet( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( DATA_SET_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DATA_SET ) {
		return nullptr;
	}
	return (DataSetWrapper^)val->obj;
}

// get spectrum group
DataGroupNodeWrapper^ ClrPluginCallTool::getGroup( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( GROUP_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DATA_GROUP_NODE ) {
		return nullptr;
	}
	return (DataGroupNodeWrapper^)val->obj;
}

// get peaks
PeaksWrapper^ ClrPluginCallTool::getPeaks( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( PEAKS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PEAKS ) {
		return nullptr;
	}
	return (PeaksWrapper^)val->obj;
}

// get 2D peaks
Peaks2DWrapper^ ClrPluginCallTool::getPeaks2D( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( PEAKS_2D_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PEAKS_2D ) {
		return nullptr;
	}
	return (Peaks2DWrapper^)val->obj;
}

// get parent peaks
PeaksWrapper^ ClrPluginCallTool::getParentPeaks( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( PARENT_PEAKS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PEAKS ) {
		return nullptr;
	}
	return (PeaksWrapper^)val->obj;
}

// get baseline
XYDataWrapper^ ClrPluginCallTool::getBaseline( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( BASELINE_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::XY_DATA ) {
		return nullptr;
	}
	return (XYDataWrapper^)val->obj;
}

// get updated xy data
XYDataWrapper^ ClrPluginCallTool::getUpdatedXYData( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( UPDATED_XY_DATA_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::XY_DATA ) {
		return nullptr;
	}
	return (XYDataWrapper^)val->obj;
}

// get updated peaks
PeaksWrapper^ ClrPluginCallTool::getUpdatedPeaks( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( UPDATED_PEAKS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PEAKS ) {
		return nullptr;
	}
	return (PeaksWrapper^)val->obj;
}

// get updated peaks
PeaksWrapper^ ClrPluginCallTool::getUpdatedPeaks2d( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( UPDATED_PEAKS_2D_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PEAKS_2D ) {
		return nullptr;
	}
	return (PeaksWrapper^)val->obj;
}


// get control specturm
SpectrumWrapper^ ClrPluginCallTool::getControlSpectrum( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( CONTROL_SPEC_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SPECTRUM ) {
		return nullptr;
	}
	return (SpectrumWrapper^)val->obj;
}

// get control chromatogram
ChromatogramWrapper^ ClrPluginCallTool::getControlChromatogram( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( CONTROL_CHROM_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::CHROMATOGRAM ) {
		return nullptr;
	}
	return (ChromatogramWrapper^)val->obj;
}

// get control group
DataGroupNodeWrapper^ ClrPluginCallTool::getControlGroup( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( CONTROL_GROUP_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DATA_GROUP_NODE ) {
		return nullptr;
	}
	return (DataGroupNodeWrapper^)val->obj;
}

// get control xy data
XYDataWrapper^ ClrPluginCallTool::getControlXYData( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( CONTROL_XY_DATA_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::XY_DATA ) {
		return nullptr;
	}
	return (XYDataWrapper^)val->obj;
}

// get path
System::String^ ClrPluginCallTool::getPath( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( PATH_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::STRING ) {
		return nullptr;
	}
	return (System::String^)val->obj;
}

// get start of range
bool ClrPluginCallTool::getStartOfRange( ClrParameters^ params, double% start ) {
	ClrVariant^ val = params->getValue( START_RANGE_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DOUBLE ) {
		return false;
	}
	start = (double)val->obj;
	return true;
}

// get end of range
bool ClrPluginCallTool::getEndOfRange( ClrParameters^ params, double% end ) {
	ClrVariant^ val = params->getValue( END_RANGE_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DOUBLE ) {
		return false;
	}
	end = (double)val->obj;
	return true;
}

// get range
bool ClrPluginCallTool::getRange( ClrParameters^ params, double% start, double% end ) {
	bool ret0 = getStartOfRange( params, start );
	bool ret1 = getEndOfRange( params, end );
	
	return ( ret0 && ret1 );
}

// get progress
ProgressWrapper^ ClrPluginCallTool::getProgress( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( PROGRESS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::PROGRESS ) {
		return nullptr;
	}
	return (ProgressWrapper^)val->obj;
}

// get mouse event
MouseEventWrapper^ ClrPluginCallTool::getMouseEvent( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( MOUSE_EVENT_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::MOUSE_EVT ) {
		return nullptr;
	}
	return (MouseEventWrapper^)val->obj;
}

// get key event
KeyEventWrapper^ ClrPluginCallTool::getKeyEvent( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( KEY_EVENT_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::KEY_EVT ) {
		return nullptr;
	}
	return (KeyEventWrapper^)val->obj;
}

// get size event
SizeEventWrapper^ ClrPluginCallTool::getSizeEvent( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( SIZE_EVENT_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SIZE_EVT ) {
		return nullptr;
	}
	return (SizeEventWrapper^)val->obj;
}

// get move event
MoveEventWrapper^ ClrPluginCallTool::getMoveEvent( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( MOVE_EVENT_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::MOVE_EVT ) {
		return nullptr;
	}
	return (MoveEventWrapper^)val->obj;
}

// get graphics
GraphicsWrapper^ ClrPluginCallTool::getGraphics( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( GRAPHICS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::GRAPHICS ) {
		return nullptr;
	}
	return (GraphicsWrapper^)val->obj;
}

// get data map
DataMapInfoWrapper^ ClrPluginCallTool::getDataMapInfo( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( DATA_MAP_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::DATA_MAP ) {
		return nullptr;
	}
	return (DataMapInfoWrapper^)val->obj;
}

// get setting parameter values
SettingParameterValuesWrapper^ ClrPluginCallTool::getSettingValues( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( SETTINGS_PARAM_NAME );
	if( val == nullptr || val->type != ClrVariant::DataType::SETTING_VALUES ) {
		return nullptr;
	}
	return (SettingParameterValuesWrapper^)val->obj;
}

// get active object
ClrVariant^ ClrPluginCallTool::getActiveObject( ClrParameters^ params ) {
	ClrVariant^ val = params->getValue( ACTIVE_OBJECT_PARAM_NAME );
	return val;
}

// set bool
void ClrPluginCallTool::setBool( ClrParameters^ params, System::String^ name, bool val ) {
	if( params == nullptr ) {
		return;
	}

	ClrVariant^ v = params->getValue( name );
	v->type = ClrVariant::DataType::BOOL;
	v->obj = val;
}

// get bool
bool ClrPluginCallTool::getBool( ClrParameters^ params, System::String^ name, bool df ) {
	if( params == nullptr || name == nullptr || !params->hasParameter( name ) ) {
		return df;
	}
	ClrVariant^ v = params->getValue( name );
	if( v->type != ClrVariant::DataType::BOOL ) {
		return df;
	}
	return (bool)( v->obj );
}

// set int
void ClrPluginCallTool::setInt( ClrParameters^ params, System::String^ name, int val ) {
	if( params == nullptr ) {
		return;
	}

	ClrVariant^ v = params->getValue( name );
	v->type = ClrVariant::DataType::INTEGER;
	v->obj = val;
}

// get int 
int ClrPluginCallTool::getInt( ClrParameters^ params, System::String^ name, int df ) {
	if( params == nullptr || name == nullptr || !params->hasParameter( name ) ) {
		return df;
	}
	ClrVariant^ v = params->getValue( name );
	if( v->type != ClrVariant::DataType::INTEGER ) {
		return df;
	}
	return (int)( v->obj );
}

// set double 
void ClrPluginCallTool::setDouble( ClrParameters^ params, System::String^ name, double val ) {
	if( params == nullptr ) {
		return;
	}

	ClrVariant^ v = params->getValue( name );
	v->type = ClrVariant::DataType::DOUBLE;
	v->obj = val;
}

// gewt double
double ClrPluginCallTool::getDouble( ClrParameters^ params, System::String^ name, double df ) {
	if( params == nullptr || name == nullptr || !params->hasParameter( name ) ) {
		return df;
	}
	ClrVariant^ v = params->getValue( name );
	if( v->type != ClrVariant::DataType::DOUBLE ) {
		return df;
	}
	return (double)( v->obj );
}

// set string
void ClrPluginCallTool::setString( ClrParameters^ params, System::String^ name, System::String^ val ) {
	if( params == nullptr ) {
		return;
	}

	ClrVariant^ v = params->getValue( name );
	v->type = ClrVariant::DataType::STRING;
	v->obj = val;
}

// get string
System::String^ ClrPluginCallTool::getString( ClrParameters^ params, System::String^ name, System::String^ df ) {
	if( params == nullptr || name == nullptr || !params->hasParameter( name ) ) {
		return df;
	}
	ClrVariant^ v = params->getValue( name );
	if( v->type != ClrVariant::DataType::STRING ) {
		return df;
	}
	return (System::String^)( v->obj );
}
