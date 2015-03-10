/**
 * @file CompositionFunctions.h
 * @brief interfaces of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.02.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_COMPOSITION_FUNCTIONS_H__
#define __KOME_COMPOSITION_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openCompositionDialog( kome::objects::Parameters* )
 * @brief opens composition subtract dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openCompositionDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getSampleFusionOperation( kome::objects::Parameters* )
 * @brief gets sample fusion operation
 * @return return value sample fusion operation object
 */
kome::objects::Variant getSampleFusionOperation( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_COMPOSITION_FUNCTIONS_H__
