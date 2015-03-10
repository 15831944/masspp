/**
 * @file WindowFilterFunctions.h
 * @brief interfaces of WindowFilter plug-in functions
 *
 * @author M.Fukuda
 * @date 2010.6.4
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_WINDOW_FILTER_FUNCTIONS_H__
#define __KOME_FILTER_WINDOW_FILTER_FUNCTIONS_H__

#include "RectangularWindowFilter.h"
#include "BartlettWindowFilter.h"
#include "GaussianWindowFilter.h"

#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant execRectangularWindow( kome::objects::Parameters* params )
 * @brief executes rectangular window filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant execRectangularWindow( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execBartlettWindow( kome::objects::Parameters* params )
 * @brief executes bartlett window filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant execBartlettWindow( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execGaussianWindow( kome::objects::Parameters* params )
 * @brief executes gaussian window filter
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant execGaussianWindow( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix

#endif		// __KOME_FILTER_WINDOW_FILTER_FUNCTIONS_H__
