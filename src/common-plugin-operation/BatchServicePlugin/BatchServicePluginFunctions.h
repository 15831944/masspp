/**
 * @file BatchServicePluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author okada_h
 * @date 2012.02.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__
#define __KOME_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant batchProcessingMethodConfiguration( kome::objects::Parameters* params );
 * @brief 
 * @param[in] params
 */
kome::objects::Variant batchProcessingMethodConfiguration( kome::objects::Parameters* params );


/**
 * @kome::objects::Variant batchSetFunctionTest( kome::objects::Parameters* params );
 * @brief batch_set BatchInfo*‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ð•Ô‚·
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant batchSetFunctionTest( kome::objects::Parameters* params );



#endif    // __KOME_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__
