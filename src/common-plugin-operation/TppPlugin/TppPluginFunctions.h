/**
 * @file TppPluginFunctions.h
 * @brief interfaces of TPP plug-in functions
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu Co., Ltd. All rights reserved.
 */

#ifndef __KOME_TPP_PLUGIN_FUNCTIONS_H__
#define __KOME_TPP_PLUGIN_FUNCTIONS_H__

/**
 * @fn kome::objects::Variant openTppDialog( kome::objects::Parameters* )
 * @brief opens Tools > TPP dialog
 * @return This function always returns true.
 */
kome::objects::Variant openTppDialog( kome::objects::Parameters* );


/**
 * @fn kome::objects::Variant openTppResultDialog( kome::objects::Parameters* )
 * @brief opens View > Results > Tpp dialog
 * @return This function always returns true.
 */
kome::objects::Variant openTppResultDialog( kome::objects::Parameters* );

#endif //__KOME_TPP_PLUGIN_FUNCTIONS_H__
