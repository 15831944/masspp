/**
 * @file DataMapFilterPluginFunction.h
 * @brief interfaces of Data Map Filter plug-in functions
 *
 * @author M.Izumi
 * @date 2012.09.06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_DATA_MAP_FILTER_FUNCTIONS_H__
#define __KOME_VIEW_DATA_MAP_FILTER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant updateSettings( kome::objects::Parameters* )
 * @brief This function is called when settings value is updated
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSettings( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix


#endif//  __KOME_VIEW_DATA_MAP_DRAWER_FUNCTIONS_H__
