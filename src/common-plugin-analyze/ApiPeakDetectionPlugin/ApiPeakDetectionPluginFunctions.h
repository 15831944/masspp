/**
 * @file ApiPeakDetectionPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_API_PEAK_DETECTION_PLUGIN_FUNCTIONS_H__
#define __KOME_API_PEAK_DETECTION_PLUGIN_FUNCTIONS_H__



/**
 * @fn kome::objects::Variant detectPeaks( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parametes
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant detectPeaks( kome::objects::Parameters* params );




#endif    // __KOME_API_PEAK_DETECTION_PLUGIN_FUNCTIONS_H__
