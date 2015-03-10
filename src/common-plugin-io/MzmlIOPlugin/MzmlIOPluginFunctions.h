/**
 * @file MzmlIOPluginFunctions.h
 * @brief interfaces of MzmlIO plug-in function
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_MZML_IO_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openMzml( kome::objects::Parameters* params )
 * @brief open mzml file
 * @param params parameters object. (This object has path information.)
 * @return return value. (Sample Object)
 */
kome::objects::Variant openMzml( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveMzml( kome::objects::Parameters* params )
 * @brief save mzml file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns ture. [bool])
 */
kome::objects::Variant saveMzml( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix

  

#endif// __KOME_IO_MZML_IO_PLUGIN_FUNCTIONS_H__
