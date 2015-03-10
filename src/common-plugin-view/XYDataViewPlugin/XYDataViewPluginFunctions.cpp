/**
 * @file XYDataViewPluginFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author fujita
 * @date 2012.04.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "XYDataViewPluginFunctions.h"

#include "XYDataViewManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// initialize data view
kome::objects::Variant initPointsView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// create view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.createPointsView();

	return ret;
}


// initialize peaks view
kome::objects::Variant initPeaksView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// create view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.createPeaksView();

	return ret;
}


// change active object
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	// select
	XYDataViewManager& mgr = XYDataViewManager::getInstance();

	if (obj.getVariant().type == kome::objects::Variant::DATA_MAP){
		// frame 
		wxWindow* wnd = kome::window::WindowTool::getMainWindow();
		kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
		if( mainFrame == NULL ) {
			return ret;
		}

		XYDataViewManager& mgr = XYDataViewManager::getInstance();
		kome::objects::DataGroupNode* group = obj.getGroup();
//		mgr.selectGroup( group );

	}else{
		mgr.selectSpectrum( spec );
		mgr.selectChromatogram( chrom );
	}

	return ret;
}

// update spectrum grid
kome::objects::Variant updateSpecXYData( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// update
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.updateSpectrumXYPointsView( spec );

	return ret;
}


// update chromatogram grid
kome::objects::Variant updateChromXYData( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// update
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.updateChromatogramXYPointsView( chrom );

	return ret;
}





// update peak view
kome::objects::Variant updatePeaksView( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get spectrum group
	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ) {
		return ret;
	}

	// update peaks view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.updatePeakListView( group );

	return ret;
}


// Peaks2dManagerÇ©ÇÁà⁄êA	@date 2011.09.16 <Add> M.Izumi
// draw 2D peaks
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.drawPeaks( *dataMap, *graphics );

	return ret;
}

// draw 2D peaks
kome::objects::Variant draw2dClusters( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.drawClusters( *dataMap, *graphics );

	return ret;
}


// toggle the visibility of the data grid
kome::objects::Variant togglePointsXYData( kome::objects::Parameters* param ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( param );
	kome::objects::MsDataVariant obj( var );

	// toggle visibility
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.toggleDataVisibility();

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();

	if( spec != NULL ) {
		mgr.selectSpectrum( spec );
	}
	else if( chrom != NULL ) {
		mgr.selectChromatogram( chrom );
	}
	else if( group != NULL ) {
		mgr.selectGroup( group );
	}



	return ret;
}

// toggle the visibility of the peaks grid
kome::objects::Variant togglePeakXY2DData( kome::objects::Parameters* param ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( param );
	kome::objects::MsDataVariant obj( var );

	// toggle visibility
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.togglePeakVisibility( param );
	
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();

	if( spec != NULL ) {
		mgr.selectSpectrum( spec );
	}
	else if( chrom != NULL ) {
		mgr.selectChromatogram( chrom );
	}
	else if( group != NULL ) {
		mgr.selectGroup( group );
	}

	return ret;
}

// check the visibility of the data grid
kome::objects::Variant isVisiblePointsXYData( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisiblePointsXYPointsView();

	return ret;
}

// check peak list view visibility
kome::objects::Variant isVisibleXY2DPeakView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility of the peak list view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleXY2DPeaksView();

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// release view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.releaseView();

	return ret;
}


/*


// toggle the visibility of the peaks grid
kome::objects::Variant togglePeakXY2DData( kome::objects::Parameters* param ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( param );
	kome::objects::MsDataVariant obj( var );

	// toggle visibility
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.togglePeakVisibility( param );
	
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();

	if( spec != NULL ) {
		mgr.selectSpectrum( spec );
	}
	else if( chrom != NULL ) {
		mgr.selectChromatogram( chrom );
	}
	else if( group != NULL ) {
		mgr.selectGroup( group );
	}

	return ret;
}


// check peak list view visibility
kome::objects::Variant isVisibleXY2DPeakView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility of the peak list view
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleXY2DPeaksView();

	return ret;
}

// Peaks2dManagerÇ©ÇÁà⁄êA	@date 2011.09.16 <Add> M.Izumi
// draw 2D peaks
kome::objects::Variant draw2dClusters( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.drawClusters( *dataMap, *graphics );

	return ret;
}

// Peaks2dManagerÇ©ÇÁà⁄êA	@date 2011.09.16 <Add> M.Izumi
// draw 2D peaks
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw peaks
	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	mgr.drawPeaks( *dataMap, *graphics );

	return ret;
}

*/
