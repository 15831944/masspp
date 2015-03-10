/**
 * @file MultiAlgorithmLabelingFunctions.h
 * @brief interfaces of MultiAlgorithmLabeling plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_LABELING_MULTI_ALGORITHM_FUNCTIONS_H__
#define __KOME_LABELING_MULTI_ALGORITHM_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectPeaks( kome::objects::Parameters* params )
 * @brief detects peaks
 * @param params parameters information object
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detectPeaks( kome::objects::Parameters* params )
 * @brief detects peaks
 * @param params parameters information object
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectParentPeaks( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif // __KOME_LABELING_MULTI_ALGORITHM_FUNCTIONS_H__
