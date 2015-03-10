/**
 * @file FirstViewPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.01.08
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "FirstViewPluginFunctions.h"


// open a first view
kome::objects::Variant openFirstView( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// sample
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}

	// data set
	kome::objects::DataSet dataSet;
	sample->getRootDataGroupNode()->getDataSet( &dataSet );
	
	// open
	if( dataSet.getNumberOfChromatograms() > 0 ) {
		// Spec No.90897 @date 2013.03.29 <Mod> M.Izumi
		if( kome::plugin::PluginCallTool::onOpenChromatogram( *( dataSet.getChromatogram( 0 ) ) ) ) {
			kome::plugin::PluginCallTool::openChromatogram( *( dataSet.getChromatogram( 0 ) ) );
		}
	}
	else if( dataSet.getNumberOfSpectra() > 0 ) {
		// Spec No.90897 @date 2013.03.29 <Mod> M.Izumi
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *( dataSet.getSpectrum( 0 ) ) ) ){
			kome::plugin::PluginCallTool::openSpectrum( *( dataSet.getSpectrum( 0 ) ) );
		}
	}

	return ret;
}
