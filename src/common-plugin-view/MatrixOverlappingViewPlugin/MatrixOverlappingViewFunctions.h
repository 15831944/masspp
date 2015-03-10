/**
 * @file MatrixOverlappingView.h
 * @brief interfaces of MatrixOverlappingView plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.06.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_OVERLAPPING_VIEW_FUNCTIONS_H__
#define __KOME_MATRIX_OVERLAPPING_VIEW_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant showOverlappingView( kome::objects::Parameters* params )
 * @brief shows overlapping view
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant showOverlappingView( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_MATRIX_OVERLAPPING_VIEW_FUNCTIONS_H__
