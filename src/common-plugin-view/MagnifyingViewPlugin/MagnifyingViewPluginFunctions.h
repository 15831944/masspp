/**
 * @file MagnifyingViewPluginFunctions.h
 * @brief interfaces of MagnifyingView plug-in function
 *
 * @author N.Tashiro
 * @date 2012.03.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_VIEW_MAGNIFYING_VIEW_PLUGIN_FUNCTIONS_H__
#define __KOME_VIEW_MAGNIFYING_VIEW_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant onCloseSpectrum( kome::objects::Parameters* params )
 * @brief This function is called when a spectrum is close
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onCloseSpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse is double clicked on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant showMagnify( kome::objects::Parameters* params )
 * @brief shows Magnify
 * @param params parameters information (This object has spectrum and m/z range informations.)
 * @return This value has no meanings.
 */
kome::objects::Variant showMagnify( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecWindowFg( kome::objects::Parameters* params )
 * @brief This function is called when graphics changed on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDrawSpecWindowFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getMagnifyingViewOperation( kome::objects::Parameters* )
 * @brief gets magnifying view  operation
 * @return return value MagnifyingViewOperation object
 */
kome::objects::Variant getMagnifyingViewOperation( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_VIEW_MAGNIFYING_VIEW_PLUGIN_FUNCTIONS_H__
