/**
 * @file CommandLineBatchServicePluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author satstnka
 * @date 2012.03.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_COMMAND_LINE_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__
#define __KOME_COMMAND_LINE_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__



/**
 * @fn kome::objects::Variant getBatchService( kome::objects::Parameters* )
 * @brief gets the batch service
 * @return batch service object
 */
kome::objects::Variant getBatchService( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant toggleBatchJobWindow( kome::objects::Parameters* )
 * @brief toggle the visibility of the batch job window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant toggleBatchJobWindow( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleBatchJob( kome::objects::Parameters* )
 * @brief judges whether the batch job window is shown or not.
 * @return If true, the batch job window is shown. (boolean)
 */
kome::objects::Variant isVisibleBatchJob( kome::objects::Parameters* );


#endif    // __KOME_COMMAND_LINE_BATCH_SERVICE_PLUGIN_FUNCTIONS_H__
