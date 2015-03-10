/**
 * @file MslowessPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author ozaki
 * @date 2013.08.23
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MSLOWESS_PLUGIN_FUNCTIONS_H__
#define __KOME_MSLOWESS_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant mslowessFilter( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant mslowessFilter( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant exitPlugin( kome::objects::Parameters* params );

#endif    // __KOME_MSLOWESS_PLUGIN_FUNCTIONS_H__
