/**
 * @file LinearBaselineDetectionFunctions.h
 * @brief interfaces of LinearBaselineDetection plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BASELINE_LINEAR_FUNCTIONS_H__
#define __KOME_BASELINE_LINEAR_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant detectBaseline( kome::objects::Parameters* params )
 * @brief detects baseline
 * @param params parameters information
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant detectBaseline( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif // __KOME_BASELINE_LINEAR_FUNCTIONS_H__
