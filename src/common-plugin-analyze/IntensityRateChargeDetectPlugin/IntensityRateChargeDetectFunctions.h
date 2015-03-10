/**
 * @file IntensityRateChargeDetectFunctions.h
 * @brief interfaces of IntensityRateChargeDetect plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.10.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_FUNCTIONS_H__
#define __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_FUNCTIONS_H__


extern "C" {

	/**
	 * @fn kome::objects::Variant detectCharge( kome::objects::Parameters* params )
	 * @brief detects charge
	 * @return return value. (This value has no meanings.)
	 */
	kome::objects::Variant detectCharge( kome::objects::Parameters* params );

}

#endif // __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_FUNCTIONS_H__
