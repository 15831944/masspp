/**
 * @file DefaultChargePluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author H. Parry
 * @date 2012.04.24
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_DEFAULT_CHARGE_PLUGIN_FUNCTIONS_H__
#define __KOME_DEFAULT_CHARGE_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant addDefaultCharges( kome::objects::Parameters* params )
 * @brief detects charge2
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant addDefaultCharges( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif    // __KOME_DEFAULT_CHARGE_PLUGIN_FUNCTIONS_H__
