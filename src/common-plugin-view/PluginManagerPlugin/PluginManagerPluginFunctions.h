/**
 * @file PluginManagerPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_PLUGIN_MANAGER_PLUGIN_FUNCTIONS_H__
#define __KOME_PLUGIN_MANAGER_PLUGIN_FUNCTIONS_H__

/**
 * @fn kome::objects::Variant openPluginManagerView( kome::objects::Parameters* params )
 * @brief open plugin manager view
 * @return If true, It succeeded to open plugin manager view (boolean)
 */
kome::objects::Variant openPluginManagerView( kome::objects::Parameters* params );


#endif    // __KOME_PLUGIN_MANAGER_PLUGIN_FUNCTIONS_H__
