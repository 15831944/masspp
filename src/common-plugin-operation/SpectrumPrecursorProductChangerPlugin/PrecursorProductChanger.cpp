/**
 * @file SpectrumPrecursorProductChangerPluginFunctions.cpp
 * @brief interfaces of SpectrumPrecursorProductChangerPluginFunctions class
 *
 * @author H.Parry
 * @date 2011.03.22
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PrecursorProductChanger.h"
#include "ProductIonSelectionDialog.h"

#include <vector>
#include <wx/wx.h>

using namespace kome::spectrum;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


kome::objects::Variant openPrecursorIonSpectrum( kome::objects::Parameters* params ) {
	return openPrecursorProductIonSpectrum( params, 1 );
}

kome::objects::Variant openProductIonSpectrum( kome::objects::Parameters* params ) {
	return openPrecursorProductIonSpectrum( params, 0 );
}

// flgPrecursorProduct: 0=Product 1=Precursor
kome::objects::Variant openPrecursorProductIonSpectrum( kome::objects::Parameters* params, int flgPrecursorProduct ) {
	// Return value (Success: true, Failure: false)
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected == NULL ) {
		LOG_ERROR(FMT( "Failed to get a spectrum." ));
		return ret;
	}

	kome::objects::Spectrum* spec = NULL;
	std::vector<kome::objects::Spectrum*> productSpectra;

	if( flgPrecursorProduct == 1 ){
		spec = specSelected->getParentSpectrum();	// Precursor

		if( spec == NULL ) {
			LOG_ERROR( FMT( "No precursor ion spectrum found." ) );
			return ret;
		}

		if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *spec );
		}
	}else{
		// Get the current spectrum's sample, iterate through all its groups
		// to find the current spectrum's product spectra.
	
		kome::objects::Sample* sample = specSelected->getSample();
		unsigned int nGroups = sample->getNumberOfGroups();
		kome::objects::DataGroupNode* group = NULL;

		if( sample == NULL ) {
			LOG_ERROR( FMT( "Failed to get a sample." ) );
			return ret;
		}

		for (unsigned int iGroupIndex = 0; iGroupIndex < nGroups; iGroupIndex++)
		{
			group = sample->getGroup(iGroupIndex);

			if( group == NULL ) {
				LOG_ERROR( FMT( "A group is not active." ) );
				return ret;
			}

			unsigned int uNumberOfSpectra = group->getNumberOfSpectra();
			if( uNumberOfSpectra <= 0 ){
				// error	// There are no spectra
				return ret;	// Return with failure.
			}else{
				for( unsigned int i=0; i<uNumberOfSpectra; i++ ){
					spec = group->getSpectrum( i );
					if ( (specSelected->getMsStage()+1 == spec->getMsStage())
						&& ( ( specSelected == spec->getParentSpectrum() ) || ( specSelected->getOrgSpectrum() == spec->getParentSpectrum() ) ) ) {

						// We've found a product spectrum!!!
						productSpectra.push_back(spec);
					}
				}
			}
		}

		if( productSpectra.size() == 0 ) {
			LOG_INFO_CODE( FMT( "No product ion spectra found." ), ERR_OTHER );
			return ret;
		}

		if( productSpectra.size() == 1 ) {
			// There is only one product spectrum, so simply show it.
			if( kome::plugin::PluginCallTool::onOpenSpectrum( *(productSpectra[0]) ) ) {
				kome::plugin::PluginCallTool::openSpectrum( *(productSpectra[0]) );
			}
		}
		else {
			// There are more than one product spectra, so let the user select one.
			// open dialog
			ProductIonSelectionDialog dlg( kome::window::WindowTool::getMainWindow(), productSpectra );
			if( dlg.ShowModal() == wxID_OK ) {
				// Display the selected spectrum
				if( kome::plugin::PluginCallTool::onOpenSpectrum( *(productSpectra[dlg.GetSelectedItem()]) ) ) {
					kome::plugin::PluginCallTool::openSpectrum( *(productSpectra[dlg.GetSelectedItem()]) );
				}
			}
		}
	}

	// Finished successfully.
	ret.prim.boolVal = true;

	return ret;
}

// judges whether the active spectrum has a precursor ion spectrum or not
kome::objects::Variant enablePrecursorIonSpectrum( kome::objects::Parameters* params) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		ret.prim.boolVal = (specSelected->getParentSpectrum() != NULL);
	}

	return ret;
}


// judges whether active spectrum has a product ion spectrum or not
kome::objects::Variant enableProductIonSpectrum( kome::objects::Parameters* params) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		std::vector<kome::objects::Spectrum*> childSpectra;
		specSelected->getChildSpectra(childSpectra);
		ret.prim.boolVal = (childSpectra.size() > 0);
	}

	return ret;
}
