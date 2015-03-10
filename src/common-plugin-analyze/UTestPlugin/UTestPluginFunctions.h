/**
 * @file UTestPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_U_TEST_PLUGIN_FUNCTIONS_H__
#define __KOME_U_TEST_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant calcUTestPValue( kome::objects::Parameters* params )
 * @brief calculates u-test p-value
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant calcUTestPValue( kome::objects::Parameters* params );


#endif    // __KOME_U_TEST_PLUGIN_FUNCTIONS_H__
