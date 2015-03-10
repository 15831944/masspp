/**
 * @file MatrixPeaksMergeFunction.h
 * @brief interfaces of MatrixPeaksMergeFunction plug-in functions
 *
 * @author M.Izumi
 * @date 2013.01.15
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MATRIX_PEAKS_MERGE_FUNCTIONS_H__
#define __KOME_MATRIX_PEAKS_MERGE_FUNCTIONS_H__

#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant getMergePeakPanel( kome::objects::Parameters* params )
 * @brief open merge peak panel dialog
 * @param params parameters object
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant getMergePeakPanel( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix

#endif //  __KOME_MATRIX_PEAKS_MERGE_FUNCTIONS_H__
