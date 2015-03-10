/**
 * @file SampleMergeFunctions.h
 * @brief interfaces of SampleMerge plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SAMPLE_MERGE_FUNCTIONS_H__
#define __KOME_SAMPLE_MERGE_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant mergeSamples( kome::objects::Parameters* )
 * @brief merge samples
 * @return If true, it succeeded to merge samples. (boolean)
 */
kome::objects::Variant mergeSamples( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getAverageSampleOperation( kome::objects::Parameters* )
 * @brief gets average sample operation
 * @return return value AverageSampleOperation object
 */
kome::objects::Variant getAverageSampleOperation( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_SAMPLE_MERGE_FUNCTIONS_H__
