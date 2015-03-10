/**
 * @file BasicScalingPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author M.Izumi
 * @date 2012.11.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_BASIC_SCALING_PLUGIN_FUNCTION_H__
#define __KOME_BASIC_SCALING_PLUGIN_FUNCTION_H__

/**
 * @fn kome::objects::Variant calcParetoValue( kome::objects::Parameters* params )
 * @brief calc pareto value
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant calcParetoValue( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant calcLogValue( kome::objects::Parameters* params )
 * @brief calc log value
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant calcLogValue( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant calcLnValue( kome::objects::Parameters* params )
 * @brief calc ln value
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant calcLnValue( kome::objects::Parameters* params );

#endif	// __KOME_BASIC_SCALING_PLUGIN_FUNCTION_H__

