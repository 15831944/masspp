/**
 * @file MzRangeFilterManager.cpp
 * @brief interfaces of MzRangeFilterManager class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MzRangeFilterManager.h"
#include "MzRangeFilterDialog.h"
#include "MzRangeFilterOperation.h"

using namespace kome::mzfilter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MzRangeFilterManager::MzRangeFilterManager(){
}

// destructor
MzRangeFilterManager::~MzRangeFilterManager(){
}

// open dialog
bool MzRangeFilterManager::openDialog( kome::objects::Variant& activeObj ){
		
	MzRangeFilterDialog dlg( kome::window::WindowTool::getMainWindow(), activeObj );
	if( dlg.ShowModal() == wxID_OK ){
				
		const char* val = dlg.getMzRange();
		std::vector< std::string >tokens;
		kome::plugin::SettingsValue::separateListValue( val, tokens );
		
		double startMz = 0.0;
		double endMz = 0.0;
		if( !tokens.empty() ){
			startMz = atoi( tokens[0].c_str() );
			endMz = atoi( tokens[1].c_str() );
		}

		// add operation
		MzRangeFilterOperation* opt = new MzRangeFilterOperation();
		opt->setShortName( "mz_range_filter" );
		opt->setOperationName( "m/z Range Filter" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setStartMz( startMz );
		opt->setEndMz( endMz );

		std::vector< kome::objects::Spectrum* > spectra;
		dlg.getCheckSpectra( spectra );
		opt->setSpectra( spectra );
		if( !spectra.empty() ){
			opt->setTargetSample( spectra[0]->getSample() );
		}
		
		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;			
		}
	}

	return true;

}

// get instance
MzRangeFilterManager& MzRangeFilterManager::getInstance(){
	// create object (This is the only object.)
	static MzRangeFilterManager mgr;

	return mgr;
}
