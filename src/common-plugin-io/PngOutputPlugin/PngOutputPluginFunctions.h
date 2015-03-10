/**
 * @file PngOutputPluginFunctions.h
 * @brief interfaces of PngOutput plug-in function
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_PNG_OUTPUT_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_PNG_OUTPUT_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant saveSpecPng( kome::objects::Parameters* params )
 * @brief saves spectrum as a PNG file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns ture. [bool])
 */
kome::objects::Variant saveSpecPng( kome::objects::Parameters* params );

/** 
 * @fn kome::objects::Variant saveChromPng( kome::objects::Parameters* params )
 * @brief saves chromatogram as PNG file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns ture. [bool])
 */
kome::objects::Variant saveChromPng( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_PNG_OUTPUT_PLUGIN_FUNCTIONS_H__
