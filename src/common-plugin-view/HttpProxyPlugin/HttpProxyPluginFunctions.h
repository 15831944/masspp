/**
 * @file HttpProxyPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.03.13
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_HTTP_PROXY_PLUGIN_FUNCTIONS_H__
#define __KOME_HTTP_PROXY_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant setProxyInfo( kome::objects::Parameters* )
 * @brief sets the proxy information
 * @return This value has no meanings
 */
kome::objects::Variant setProxyInfo( kome::objects::Parameters* );


#endif    // __KOME_HTTP_PROXY_PLUGIN_FUNCTIONS_H__
