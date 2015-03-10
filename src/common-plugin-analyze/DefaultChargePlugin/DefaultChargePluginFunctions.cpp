/**
 * @file DefaultChargePluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author H. Parry
 * @date 2012.04.24
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "DefaultChargePluginFunctions.h"

#define CHARGE1_PARAM_NAME			"charge1"
#define CHARGE2_PARAM_NAME			"charge2"
#define CHARGE3_PARAM_NAME			"charge3"
#define CHARGE4_PARAM_NAME			"charge4"
#define CHARGE5_PARAM_NAME			"charge5"
#define CHARGE6_PARAM_NAME			"charge6"
#define CHARGE7_PARAM_NAME			"charge7"
#define CHARGE8_PARAM_NAME			"charge8"
#define TYPE_PARAM_NAME	            "type"


// detect peaks
kome::objects::Variant addDefaultCharges( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum object
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// get peaks object
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ) {
		return ret;
	}

	// get setting parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		return ret;
	}

	// type
	const char* type = settings->getParameterValue( TYPE_PARAM_NAME );
	std::string t = tolowercase( type );
	if( t.compare( "prop" ) == 0 ) {
		peaks->clearCharge();
		int charge = spec->getPrecursorCharge();
		if( charge > 0 ) {
			peaks->addCharge( charge );
		}
	}
	else if( t.compare( "force" ) == 0 ) {
		peaks->clearCharge();
	}

	// set default value
	if( peaks->getNumberOfCharges() == 0 )
	{
		if (settings->getBoolValue( CHARGE1_PARAM_NAME, true ))
		{
			peaks->addCharge(1);
		}
		if (settings->getBoolValue( CHARGE2_PARAM_NAME, true ))
		{
			peaks->addCharge(2);
		}
		if (settings->getBoolValue( CHARGE3_PARAM_NAME, true ))
		{
			peaks->addCharge(3);
		}
		if (settings->getBoolValue( CHARGE4_PARAM_NAME, true ))
		{
			peaks->addCharge(4);
		}
		if (settings->getBoolValue( CHARGE5_PARAM_NAME, true ))
		{
			peaks->addCharge(5);
		}
		if (settings->getBoolValue( CHARGE6_PARAM_NAME, true ))
		{
			peaks->addCharge(6);
		}
		if (settings->getBoolValue( CHARGE7_PARAM_NAME, true ))
		{
			peaks->addCharge(7);
		}
		if (settings->getBoolValue( CHARGE8_PARAM_NAME, true ))
		{
			peaks->addCharge(8);
		}
	}

	return ret;
}
