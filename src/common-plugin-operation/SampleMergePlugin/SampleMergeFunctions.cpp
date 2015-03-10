/**
 * @file SampleMergeFunctions.cpp
 * @brief implements of SampleMerge plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SampleMergeFunctions.h"
#include "SampleMergeDialog.h"
#include "MergedSampleSet.h"
#include "AverageSampleOperation.h"

using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SHORT_NAME "average_sample"


// merge samples
kome::objects::Variant mergeSamples( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// dialog
	SampleMergeDialog dlg( kome::window::WindowTool::getMainWindow() );
	
	if( dlg.ShowModal() == wxID_OK ) {
		
		// sample set
		wxBeginBusyCursor();

		// ----------------
		// Operation ì¬
		// ----------------
		AverageSampleOperation* opt = new AverageSampleOperation();
		opt->setOperationType( kome::operation::Operation::TYPE_INPUT ); 
		opt->setOperationName( "Averaged Sample" );
		opt->setShortName( SHORT_NAME );
		opt->setRtTol( dlg.getRtTol() );
		opt->setMzTol( dlg.getMzTol() );		
		
		std::vector< kome::objects::Sample* > tmpSample;
		for( unsigned int i=0; i < dlg.getNumberOfSamples(); i++ ){
			tmpSample.push_back( dlg.getSample(i) );
		}
		
		opt->setSamples( tmpSample );

		if( opt->execute() ){
		}else{
			delete opt;
		}
		
		wxEndBusyCursor();
	}

	ret.prim.boolVal = true;
	return ret;
}

// get average sample operation
kome::objects::Variant getAverageSampleOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new AverageSampleOperation();
	
	return ret;
}
