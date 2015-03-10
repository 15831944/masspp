/**
 * @file ScriptPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_SCRIPT_PLUGIN_FUNCTIONS_H__
#define __KOME_SCRIPT_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant openScriptDialog( kome::objects::Parameters* )
 * @brief opens script dialog
 * @return This function always returns true.
 */
kome::objects::Variant openScriptDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initScriptPlugin( kome::objects::Parameters* )
 * @brief Mass++‹N“®Žž‚ÉPluginManager‚ÉScriptCall‚ð’Ç‰Á
 * @return
 */
kome::objects::Variant initScriptPlugin( kome::objects::Parameters* );

#endif    // __KOME_SCRIPT_PLUGIN_FUNCTIONS_H__
