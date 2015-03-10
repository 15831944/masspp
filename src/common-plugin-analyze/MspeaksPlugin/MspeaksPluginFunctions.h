/**
 * @file MspeaksPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MSPEAKS_PLUGIN_FUNCTIONS_H__
#define __KOME_MSPEAKS_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant mspeaksDetection( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant mspeaksDetection( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant exitPlugin( kome::objects::Parameters* params );


#endif    // __KOME_MSPEAKS_PLUGIN_FUNCTIONS_H__
