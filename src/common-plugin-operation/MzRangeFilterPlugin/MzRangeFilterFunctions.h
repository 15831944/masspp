/**
 * @file MzRangeFilterFunction.h
 * @brief interfaces of MzRangeFilterFunction plug-in functions
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MZRANGE_FILTER_FUNCTIONS_H__
#define __KOME_MZRANGE_FILTER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant openMzRangeFilterDialog( kome::objects::Parameters* params )
 * @brief open m/z range filter dialog
 * @param params parameters object
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openMzRangeFilterDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getmzRangeFilterOperation( kome::objects::Parameters* )
 * @brief get m/z range filter operation
 * @return return value mzRangeFilterOperation object
 */
kome::objects::Variant getmzRangeFilterOperation( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix


#endif // __KOME_MZRANGE_FILTER_FUNCTIONS_H__
