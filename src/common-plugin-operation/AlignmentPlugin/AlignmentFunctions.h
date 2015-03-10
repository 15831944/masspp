/**
 * @file AlignmentFunctions.h
 * @brief interfaces of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_FUNCTIONS_H__
#define __KOME_ALIGNMENT_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openAlignmentDialog( kome::objects::Parameters* )
 * @brief opens alignment subtract dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openAlignmentDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getAlignmentOperation( kome::objects::Parameters* )
 * @brief gets alignment  operation
 * @return return value AlignmentOperation object
 */
kome::objects::Variant getAlignmentOperation( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_ALIGNMENT_FUNCTIONS_H__
