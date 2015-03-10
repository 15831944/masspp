/**
 * @file SpectraGridViewPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author okada
 * @date 2011.03.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_SPECTRA_GRID_VIEW_PLUGIN_FUNCTIONS_H__
#define __KOME_SPECTRA_GRID_VIEW_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

		
/**
 * @fn kome::objects::Variant initGridView( kome::objects::Parameters* )
 * @brief initialize spectra view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initGridView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
 * @brief This function is called when active object is changed
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant unsetSample( kome::objects::Parameters* params )
 * @brief unsets sample on closing
 * @param params parameters information (This object has sample to be closed information.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant unsetSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSpectra( kome::objects::Parameters* params )
 * @brief updates spectra
 * @param params parameters information (this object has updated spectra)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSpectra( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSample( kome::objects::Parameters* params )
 * @brief updates sample
 * @param params parameters information (this object has updated sample)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant toggleView( kome::objects::Parameters* )
 * @brief toggles visibility of the spectra view
 * @return This value is always true. (boolean)
 */
kome::objects::Variant toggleView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleView( kome::objects::Parameters* )
 * @brief checks the visibility of the spectra view
 * @return If true, the spectra view is visible. (boolean value)
 */
kome::objects::Variant isVisibleView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief releases details view
 * @return This value has no meanings.
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix






#endif    // __KOME_SPECTRA_GRID_VIEW_PLUGIN_FUNCTIONS_H__
