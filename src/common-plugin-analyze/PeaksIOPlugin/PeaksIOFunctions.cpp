/**
 * @file PeaksIOFunctions.cpp
 * @brief implements of PeaksIO plug-in functions
 *
 * @author S.Tanaka
 * @date 2011.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"                        
#include "PeaksIOFunctions.h"
#include "PeaksIOManager.h"
#include "RadioButtonMenu.h"
#include "PeaksIOPluginOperation.h"
#include "ExportPeaksDialog.h"

using namespace kome::io::peaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// export peaks
kome::objects::Variant exportPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	PeaksIOManager& mgr = PeaksIOManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::DataGroupNode* group = NULL;
	if( var.type == kome::objects::Variant::DATA_MAP ) {
		// file path
		std::string path = mgr.showFileSaveDialog();

		// group
		group = obj.getGroup();
		if( group == NULL ){
			LOG_ERROR( FMT( "Failed to get an active group." ) );
			return ret;
		}

		// export
		if( path.empty() ) {
			ret.prim.boolVal = true;
		}else{
			wxBeginBusyCursor();
		
			ret.prim.boolVal = mgr.export2dPeaks( group, path.c_str() );
			
			wxEndBusyCursor();
		}	
		
	}else{
	
		// Export Peaks dialog
		ExportPeaksDialog dlg( kome::window::WindowTool::getMainWindow() );
		ret.prim.boolVal = true;
		if( dlg.ShowModal() == wxID_OK ){
			// file path
			std::string path = mgr.showFileSaveDialog();
			if( path.empty() ) {
				ret.prim.boolVal = true;
			}else{

				kome::objects::DataSet dataSet;
				ret.prim.boolVal = mgr.getDataSet( var, dlg.getSelTarget(), dataSet );
			
				if( ret.prim.boolVal ){
					wxBeginBusyCursor();
				
					if( var.type == kome::objects::Variant::SPECTRUM ) {
						ret.prim.boolVal = mgr.exportSpecPeaks( dataSet, path.c_str() );
					}else if( var.type == kome::objects::Variant::CHROMATOGRAM ) {
						ret.prim.boolVal = mgr.exportChromPeaks( dataSet, path.c_str() );
					}

					wxEndBusyCursor();
				}
			}
		}
		
	}

	return ret;
}

// import peaks
kome::objects::Variant importPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	PeaksIOManager& mgr = PeaksIOManager::getInstance();
	
	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	
	// spec
	if( var.type == kome::objects::Variant::SPECTRUM ) {

		kome::objects::Spectrum* spec = obj.getSpectrum();

		ret.prim.boolVal = mgr.importSpecPeaks( spec );
	}
	// chrom
	else if( var.type == kome::objects::Variant::CHROMATOGRAM ) {
		
		kome::objects::Chromatogram* chrom = obj.getChromatogram();
		
		ret.prim.boolVal = mgr.importChormPeaks( chrom );
		
	}
	else if( var.type == kome::objects::Variant::DATA_MAP ) {
		// group
		kome::objects::DataGroupNode* group = obj.getGroup();
		if( group == NULL ) {
			LOG_ERROR( FMT( "Failed to get an active group." ) );
			return ret;
		}

		// peaks
		kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
		if( peaks == NULL ) {
			peaks = pkMgr.createPeaks( group );
		}

		// file path
		std::string path = mgr.showFileOpenDialog();

		// import
		if( path.empty() ) {
			ret.prim.boolVal = true;
		}
		else {
			wxBeginBusyCursor();

			peaks->clearClusters();
			peaks->clearPeaks();
			ret.prim.boolVal = mgr.import2dPeaks( peaks, path.c_str() );

			// @date 2012/04/24 <Add> OKADA ------->	// SPEC 85657
			unsigned int unPeaksNum = peaks->getNumberOfPeaks();
			if( unPeaksNum <= 0 ){
				kome::window::WindowTool::showError( "Cannot import 2D peaks. The specified file does not contain peak information.\nSelect a file containing peak information." );
			}
			// @date 2012/04/24 <Add> OKADA <-------

			kome::window::WindowTool::refresh();

			wxEndBusyCursor();
		}
	}

	return ret;
}

// import overlapping peaks
kome::objects::Variant importOverlappingPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeaksIOManager& mgr = PeaksIOManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	if( var.type == kome::objects::Variant::SPECTRUM ) {
		// spectrum
		LOG_ERROR_CODE( FMT( "This function is not available when a spectrum is active. Display a Heatmap or 3D Display window first." ), ERR_OTHER );

		return ret;
	}
	else if( var.type == kome::objects::Variant::CHROMATOGRAM ) {
		// chromatogram
		LOG_ERROR_CODE( FMT( "This function is not available when a chromatogram is active. Display a Heatmap or 3D Display window first." ), ERR_OTHER );

		return ret;
	}
	else if( var.type == kome::objects::Variant::DATA_MAP ) {
		// data map
		kome::objects::DataMapInfo* dataMap = (kome::objects::DataMapInfo*)var.prim.pt;

		// file path
		std::string path = mgr.showFileOpenDialog();

		// export
		if( path.empty() ) {
			ret.prim.boolVal = true;
		}
		else {
			wxBeginBusyCursor();

			ret.prim.boolVal = mgr.addOverlapping2dPeaks( dataMap, path.c_str() );

			kome::window::WindowTool::refresh();

			wxEndBusyCursor();
		}
	}

	return ret;
}

// delete overlapping peaks
kome::objects::Variant deleteOverlap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	PeaksIOManager& mgr = PeaksIOManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// active canvas
	if( var.type == kome::objects::Variant::SPECTRUM ) {
		// spectrum
		LOG_ERROR_CODE( FMT( "This function is not available when a spectrum is active. Display a Heatmap or 3D Display window first." ), ERR_OTHER );

		return ret;
	}
	else if( var.type == kome::objects::Variant::CHROMATOGRAM ) {
		// chromatogram
		LOG_ERROR_CODE( FMT( "This function is not available when a chromatogram is active. Display a Heatmap or 3D Display window first." ), ERR_OTHER );

		return ret;
	}
	else if( var.type == kome::objects::Variant::DATA_MAP ) {
		ret.prim.boolVal = true;

		mgr.deleteOverlapping2dPeaks( obj.getDataMap() );
		kome::window::WindowTool::refresh();
	}

	return ret;
}

// draws overlapping peaks
kome::objects::Variant drawOverlappingPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// managers
	PeaksIOManager& mgr = PeaksIOManager::getInstance();

	// object
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// draw
	mgr.drawOverlapping2dPeaks( dataMap, *g );

	return ret;
}

// @date 2011/05/31 
// import menu
kome::objects::Variant menuImportPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	PeaksIOManager& mgr = PeaksIOManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );

	// open dialog
	kome::labeling::RadioButtonMenuDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		var
	);
	
	int iSelectedNo = -1;
	if( dlg.ShowModal() == wxID_OK ){

		iSelectedNo = dlg.getSelection();

		switch( iSelectedNo ){
		case IMPORT_PEAKS_IMPORT:
			importPeaks( params );
			break;
		case IMPORT_PEAKS_OVERLAP:
			importOverlappingPeaks( params );
			break;
		case IMPORT_PEAKS_DELETE_OVERLAP:
			deleteOverlap( params );
			break;
		default:
			ret.prim.boolVal = false;
			break;
		}

	}

	return ret;
}

// get export peak operation
kome::objects::Variant getExportPeakOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new PeaksIoOperation();
	
	return ret;
}
