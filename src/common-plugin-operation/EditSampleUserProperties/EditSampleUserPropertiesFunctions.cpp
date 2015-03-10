/**
 * @file EditSampleUserPropertiesFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author Parry
 * @date 2011.04.04
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include <wx/wx.h>
#include "EditSampleUserPropertiesFunctions.h"
#include "EditSampleUserPropertiesDialog.h"
#include "SampleUserPropertiesOperation.h"

using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


kome::objects::Variant editSampleUserProperties( kome::objects::Parameters* params ) {
	// Return value (Success: true, Failure: false)
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	EditSampleUserPropertiesDialog dlg( kome::window::WindowTool::getMainWindow());

	if( dlg.ShowModal() == wxID_OK ){
		// Add Opeartion
		SampleUserPropertiesOperation* opt = new SampleUserPropertiesOperation();
		opt->setShortName( "user_prop" );
		opt->setOperationName( "Sample User Properties" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		if( dlg.getActiveSpec() != NULL ){
			opt->setTargetSample( dlg.getActiveSpec()->getSample() );
		}
		if(  dlg.getActiveChrom() != NULL ){
			opt->setTargetSample(  dlg.getActiveChrom()->getSample() );
		}
		if( dlg.getActiveGroup() != NULL ){
			opt->setTargetSample( dlg.getActiveGroup()->getSample() );
		}
		opt->setSpec( dlg.getActiveSpec() );
		opt->setChrom( dlg.getActiveChrom() );
		opt->setGroup( dlg.getActiveGroup() );
		opt->setEdit( dlg.getEdit() );

		opt->setValidate( dlg.validateUserProperties() );
		
		kome::core::Properties prop;
		dlg.getProperties( prop );

		std::vector< std::pair< std::string, std::string > > properties;
		for( unsigned int i=0; i < prop.getNumberOfProperties(); i++ ){
			std::pair< std::string, std::string > tmpPair;

			tmpPair.first = prop.getKey( i );
			tmpPair.second = prop.getValue( i );

			properties.push_back( tmpPair );

		}
		opt->setProperties( properties );

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}		
	}

	// Finished successfully.
	ret.prim.boolVal = true;

	return ret;
}

// sample user properties operation
kome::objects::Variant getSampleUserPropertiesOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new SampleUserPropertiesOperation();

	return ret;
}
