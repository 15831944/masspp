/**
 * @file SimpleChargeDetect2Functions.h
 * @brief interfaces of SimpleChargeDetect2 plug-in functions
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CHARGE_SIMPLE_CHARGE_DETECT2_FUNCTIONS_H__
#define __KOME_CHARGE_SIMPLE_CHARGE_DETECT2_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectCharge2( kome::objects::Parameters* params )
 * @brief detects charge2
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectCharge2( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif // __KOME_CHARGE_SIMPLE_CHARGE_DETECT_FUNCTIONS2_H__
