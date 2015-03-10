/**
 * @file MsbIOPluginFunctions.h
 * @brief interfaces of MsbIO plug-in function
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_MSB_IO_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openMsb( kome::objects::Parameters* params )
 * @brief open msb file
 * @param params parameters object. (This object has path information.)
 * @return return value. (Sample Set Object)
 */
kome::objects::Variant openMsb( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveMsb( kome::objects::Parameters* params )
 * @brief save msb file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns true. [bool])
 */
kome::objects::Variant saveMsb( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_MSB_IO_PLUGIN_FUNCTIONS_H__
