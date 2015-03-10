/**
 * @file MassBankToolsFunctions.cpp
 * @brief implements of MassBankTools plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.06.24
 * 
 * @modify M.Fukuda / Eisai Co 
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MassBankToolsFunctions.h"

#include "MassBankManager.h"
#include "cWizard\ErrorCheck.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//// init GUI
//kome::objects::Variant initGui( kome::objects::Parameters* params ) 
//{
//	// return value
//	kome::objects::Variant ret;
//	MassBankManager& mgr = MassBankManager::getInstance();
//	mgr.createMassBankSearchResultsPane();
//
//	return ret;
//}

//// exit GUI
//kome::objects::Variant exitGui( kome::objects::Parameters* ) 
//{
//	// return value
//	kome::objects::Variant ret;
//
//	// toggle visibility
//	MassBankManager& mgr = MassBankManager::getInstance();
//	mgr.releaseMassBankSearchResultsPane();
//
//	return ret;
//}


// toggle the visibility of the overlapping dialog
kome::objects::Variant toggleSearchResultsPane( kome::objects::Parameters* ) 
{
	// return value
	kome::objects::Variant ret =setBoolVal(true);

	// toggle visibility
	MassBankManager& mgr = MassBankManager::getInstance();
	mgr.toggleMassBankSearchResultsPaneVisibility();

	return ret;
}

// judge whether the overlapping dialog is shown
kome::objects::Variant ischeckSearchResultsPaneOpened( kome::objects::Parameters* ) 
{
	// return value
	kome::objects::Variant ret =setBoolVal(false);

	// check the visibility
	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.isVisibileMassBankSearchResultsPane();

	return ret;
}

// Call Create Spectrum Record Dialog
kome::objects::Variant callCreateSpectrumRecordDialog(kome::objects::Parameters* params)
{
	// return value
	kome::objects::Variant ret =setBoolVal(true);

	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.callCreateMassBankRecord(inGetActiveDataGroupNode(params));
	return ret;
}

// Call (Similar) Spectrum Search Dialog
kome::objects::Variant callSpectrumSearchDialog( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret =setBoolVal(true);

	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.callSimilarSpectraSearch(inGetActiveSpectrum(params));
	return ret;
}

// Call Peak Search Dialog
kome::objects::Variant callPeakSearchDialog( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret =setBoolVal(true);

	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.callPeakSearch( false );
	return ret;
}

// Call Diff Peak Search Dialog
kome::objects::Variant callPeakDifferenceSearchDialog( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret =setBoolVal(true);

	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.callPeakSearch( true );
	return ret;
}

// draw peaks
kome::objects::Variant drawMassBankRecordPeaks( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret;
	
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	if (mgr.getUseOverlapSpec()) {
		// graphics
		kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics(params);
		kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum(params);
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

		if(g != nullptr && spec != nullptr && frame != nullptr) {
			spec = spec->getOrgSpectrum();
			kome::objects::Variant var = frame->getActiveObject();
			kome::objects::MsDataVariant obj( var );
			if ( obj.getSpectrum() == spec ) {
				mgr.setToDrawToActiveRecord( *g, spec ); 
			}
		}
	}

	return ret;
}

// Called Save.
kome::objects::Variant batchCommandCalled(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);

	MassBankManager& mgr = MassBankManager::getInstance();
	ret.prim.boolVal = mgr.createMassBankRecordonBatchMode(params);

	return ret;
}

typedef kome::plugin::SettingsValue::EnumItem Enumeration;
typedef std::vector<Enumeration> EnumerationArr;

// Get Vendor Info Array
kome::objects::Variant getVendorArrayData(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumitems = onGetEnumItems(params);
	if (enumitems != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getVenderArraytoEnumItems(enumitems);
	}

	return ret;
}

// Get Instrument Name Array
kome::objects::Variant getInstrumentArrayData(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumeration = onGetEnumItems(params);
	if (enumeration != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getInstrumentArraytoEnumItems(enumeration);
	}

	return ret;
}

// Get License Data Array
kome::objects::Variant getLicenseArrayData(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumeration = onGetEnumItems(params);
	if (enumeration != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getLicenseArraytoEnumItems(enumeration);
	}

	return ret;
}

kome::objects::Variant getGettedInstrumentTypeArrayDataWithFree(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumeration = onGetEnumItems(params);
	if (enumeration != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getGettedInstrumentTypeArraytoEnumItems(enumeration,true);
	}
	return ret;
}

// Get Instrument-Type Data Array
kome::objects::Variant getGettedInstrumentTypeArrayData(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumeration = onGetEnumItems(params);
	if (enumeration != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getGettedInstrumentTypeArraytoEnumItems(enumeration,false);
	}
	return ret;
}

// Get Appended Values Data Array
kome::objects::Variant getAppendedValuesArrayData(kome::objects::Parameters* params)
{
	kome::objects::Variant ret = setBoolVal(true);
	EnumerationArr* enumeration = onGetEnumItems(params);
	if (enumeration != NULL) {
		MassBankManager& mgr = MassBankManager::getInstance();
		mgr.getAppendedValuesArraytoEnumItems(enumeration);
	}
	return ret;
}

kome::objects::Variant initialiseSearchEngine(kome::objects::Parameters* params) 
{
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// return value
	kome::objects::Variant ret;
	ret.prim.pt = mgr.getEngine();
	ret.type = kome::objects::Variant::OTHER;
	
	return ret;
}

kome::objects::Variant checkPrefixStatus(kome::objects::Parameters* params)
{
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::STRING;
	ret.setString("");

	// settings
	kome::objects::SettingParameterValues* settings = 
		kome::plugin::PluginCallTool::getSettingValues(params);
	if( settings == NULL ) {
		ret.setString("Failed to get the parameters.");
	} else {
		std::string prefix = settings->getParameterValue("prefix");
		if(!CHK::checkPrefix(prefix)) {
			ret.setString("consists of two or three alphabetical capital characters.");
		} else {
			std::string basenum = settings->getParameterValue("prefixnum");
			int num = toint(basenum.c_str(),10,0);
			if(!CHK::checkBaseNum(prefix,num)) {
				ret.setString("The total length of the [Prefix] and [Base Num] fields should be at most 8 characters.\n");
			}
		}
	}
	return ret;
}

// --------------------------------------------------------
// ##---- LOCAL FUNCTIONS
// --------------------------------------------------------

// Create Bool Variant Objects
kome::objects::Variant setBoolVal(const bool fg) 
{   
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = fg;
	return ret;
}

// Create EnumItems
std::vector<kome::plugin::SettingsValue::EnumItem>* onGetEnumItems(
	kome::objects::Parameters* params)
{
	kome::objects::Variant var = params->getValue("enumeration");
	return (std::vector<kome::plugin::SettingsValue::EnumItem>*)var.prim.pt;
}

// Get Avctive Spectrum ( local Function )
kome::objects::Spectrum* inGetActiveSpectrum (kome::objects::Parameters* params) 
{
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject(params);
	kome::objects::MsDataVariant msObj( var );
	return msObj.getSpectrum();
}

// Get Avctive DataGroupNode ( local Function )
kome::objects::DataGroupNode* inGetActiveDataGroupNode (kome::objects::Parameters* params) 
{
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant msObj( var );
	return msObj.getGroup();
}
