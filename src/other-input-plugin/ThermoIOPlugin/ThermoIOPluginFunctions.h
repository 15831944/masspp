/**
 * @file ThermoIOPluginFunction.h
 * @brief Thermo IO Plugin Functions header file
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_THERMO_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_THERMO_IO_PLUGIN_FUNCTIONS_H__

/**
 * @fn kome::objects::Variant open( kome::objects::Parameters* params )
 * @brief open Thermo file
 * @param params parameters object. (This object has path information.)
 * @return return value. (Sample Object)
 */
kome::objects::Variant openRaw( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveMzml( kome::objects::Parameters* params )
 * @brief save mzml file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns ture. [bool])
 */
//kome::objects::Variant saveMzml( kome::objects::Parameters* params );

#endif
