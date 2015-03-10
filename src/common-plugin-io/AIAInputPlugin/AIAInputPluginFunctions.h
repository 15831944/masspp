/**
 * @file AIAInputPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_IO_AIA_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_AIA_IO_PLUGIN_FUNCTIONS_H__

/**
 * @fnkome::objects::Variant openAIA( kome::objects::Parameters* params )
 * @brief opens AIA file
 * @param params parameters object
 * @return Sample Set object
 */
kome::objects::Variant openAIA( kome::objects::Parameters* params );


#endif    // __KOME_IO_AIA_IO_PLUGIN_FUNCTIONS_H__
