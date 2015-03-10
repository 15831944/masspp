/**
 * @file EditSampleUserPropertiesFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author H. Parry
 * @date 2011.04.04
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */



#ifndef __KOME_EDIT_SAMPLE_USER_PROPERTIES_FUNCTIONS_H__
#define __KOME_EDIT_SAMPLE_USER_PROPERTIES_FUNCTIONS_H__

/**
 * @fn kome::objects::Variant editSampleUserProperties( kome::objects::Parameters* params )
 * @brief edit sample user properties
 * @param params
 * @return If true, edit sample user properties is succeeded.
 */
kome::objects::Variant editSampleUserProperties( kome::objects::Parameters* params );

/**
 * @fn getSampleUserPropertiesOperation( kome::objects::Parameters* params )
 * @brief get sample user properties operation
 * @return sample user properties operation
 */
kome::objects::Variant getSampleUserPropertiesOperation( kome::objects::Parameters* params );

#endif    // __KOME_EDIT_SAMPLE_USER_PROPERTIES_FUNCTIONS_H__
