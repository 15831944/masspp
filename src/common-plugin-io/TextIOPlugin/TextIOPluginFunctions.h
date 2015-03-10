/**
 * @file TextIOPluginFunctions.h
 * @brief interfaces of TextIO plug-in function
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_TEXT_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_TEXT_IO_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fnkome::objects::Variant openText( kome::objects::Parameters* params )
 * @brief opens text file
 * @param params parameters object
 * @return Sample Set object
 */
kome::objects::Variant openText( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveText( kome::objects::Parameters* params )
 * @brief saves text file
 * @param params prameters object
 * @return If true, it succeeded to save the text file. (boolean)
 */
kome::objects::Variant saveText( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_TEXT_IO_PLUGIN_FUNCTIONS_H__
