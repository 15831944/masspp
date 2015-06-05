/**
 * @file MsbackadjBaselineDetectionPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_MSBACKADJ_BASELINE_DETECTION_PLUGIN_FUNCTIONS_H__
#define __KOME_MSBACKADJ_BASELINE_DETECTION_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant msbackadjBaselineDetection( kome::objects::Parameters* params )
 * @brief detects baseline
 * @param params parameters information
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant msbackadjBaselineDetection( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
 * @brief detect peaks
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant exitPlugin( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix


#endif    // __KOME_MSBACKADJ_BASELINE_DETECTION_PLUGIN_FUNCTIONS_H__
