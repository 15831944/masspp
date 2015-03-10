/**
 * @file LocalMaximumLabelingFunctions.h
 * @brief interfaces of LocalMaximumLabeling plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_LABELING_LOCAL_MAXIMUM_LABELING_FUNCTIONS_H__
#define __KOME_LABELING_LOCAL_MAXIMUM_LABELING_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectPeaks( kome::objects::Parameters* params )
 * @brief detects peaks by local peak picking algorithm
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectPeaks( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix


#endif // __KOME_LABELING_LOCAL_MAXIMUM_LABELING_FUNCTIONS_H__
