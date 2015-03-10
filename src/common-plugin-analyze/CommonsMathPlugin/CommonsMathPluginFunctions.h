/**
 * @file CommonsMathPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author satstnka
 * @date 2012.02.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_COMMONS_MATH_PLUGIN_FUNCTIONS_H__
#define __KOME_COMMONS_MATH_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant calcTTestPValue( kome::objects::Parameters* params )
 * @brief ttest
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant calcTTestPValue( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant calcAnovaValue( kome::objects::Parameters* params )
 * @brief ANNOVA
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant calcAnovaValue( kome::objects::Parameters* params );


#endif    // __KOME_COMMONS_MATH_PLUGIN_FUNCTIONS_H__
