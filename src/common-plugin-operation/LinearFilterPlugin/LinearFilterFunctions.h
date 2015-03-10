/**
 * @file LinearFilterFunctions.h
 * @brief interfaces of LinearFilter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_LINEAR_FILTER_FUNCTIONS_H__
#define __KOME_FILTER_LINEAR_FILTER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant executeLowPass( kome::objects::Parameters* params )
 * @brief executes Low-Pass filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant executeLowPass( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeHighPass( kome::objects::Parameters* params )
 * @brief executes High-Pass filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant executeHighPass( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeGaussian( kome::objects::Parameters* params )
 * @brief executes Gaussian filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant executeGaussian( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_FILTER_LINEAR_FILTER_FUNCTIONS_H__
