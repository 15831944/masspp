/**
 * @file GlycanAnalysisPluginFunctions.cpp
 * @brief interfaces of GlycanAnalysis plug-in function
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"

#include "GlycanAnalysisManager.h"
#include "GlycanAnalysisPluginFunctions.h"

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// initialize glycan analysis
kome::objects::Variant initGlycanAnalysis( kome::objects::Parameters* params ){

	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	return ret;
}

// check for can be analyzed
kome::objects::Variant isEnableGlycanAnalysis( kome::objects::Parameters* params ){

	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	if(!gaManager.isEnable()){
		return ret;
	}

	// check active spectrum
	kome::objects::Variant activeObj
	= kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant msObj( activeObj );
	kome::objects::Spectrum* spec = msObj.getSpectrum();
	if ( spec == NULL ) {
		return ret;
	}

	// check spectrum data kind
	int nMsStage = spec->getMsStage();
	if ( nMsStage <= 0 ) {
		return ret;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ) {
		return ret;
	}
	else {
		if ( peaks->getLength() < 1 ) {
			return ret;
		}
	}

	ret.prim.boolVal = true;
	return ret;

}

// open glycan analysis dialog
kome::objects::Variant openGlycanAnalysis( kome::objects::Parameters* params ){
	
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.openDialog(params);

	return ret;
}

// close glycan analysis dialog
kome::objects::Variant closeGlycanAnalysis( kome::objects::Parameters* params ){
	
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.closeDialog(params);
	gaManager.closeStructureDialog(params);

	return ret;
}

// draw annotations on spectrum
kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.drawAnnotation(params);

	return ret;
}

// open 
kome::objects::Variant openGlycanStructureDialog( kome::objects::Parameters* params ){

	bool bRet = false;

	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = bRet;

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	if(!gaManager.isEnable()){	
		return ret;
	}

	bRet = gaManager.openStructureDialog(params);

	ret.prim.boolVal = bRet;
	return ret;

}

// close glycan analysis dialog
kome::objects::Variant exitGlycanAnalysis( kome::objects::Parameters* params ){
	
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.closeDialog();
	gaManager.closeStructureDialog();

	return ret;
}
