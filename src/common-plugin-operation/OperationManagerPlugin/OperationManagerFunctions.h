/**
 * @file OperationManagerFunctions.h
 * @brief interfaces of OperationManager plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OPERATION_MANAGER_FUNCTIONS_H__
#define __KOME_OPERATION_MANAGER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openOperationDialog( kome::objects::Parameters* )
 * @brief opens operation dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openOperationDialog( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_MANAGER_FUNCTIONS_H__
