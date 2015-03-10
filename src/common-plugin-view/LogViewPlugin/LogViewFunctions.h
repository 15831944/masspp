/**
 * @file LogViewFunctions.h
 * @brief interfaces of log view plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_LOG_VIEW_FUNCTIONS_H__
#define __KOME_VIEW_LOG_VIEW_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant initLog( kome::objects::Parameters* )
 * @brief initializes log
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initLog( kome::objects::Parameters* );


/**
 * @fn kome::objects::Variant initView( kome::objects::Parameters* )
 * @brief initialize log view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant finalizeLog( kome::objects::Parameters* )
 * @brief finalize log
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant finalizeLog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant finalizeView( kome::objects::Parameters* )
 * @brief finalize log
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant finalizeView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant toggleLogView( kome::objects::Parameters* )
 * @brief toggles log view
 * @return return value (This value is always true.[boolean value])
 */
kome::objects::Variant toggleLogView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleLogView( kome::objects::Parameters* )
 * @brief checks the visibility of the log view
 * @return If true, the log view is visible. (boolean value)
 */
kome::objects::Variant isVisibleLogView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onUpdateSettings( kome::objects::Parameters* )
 * @brief This function is called when settings is updated.
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onUpdateSettings( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_VIEW_LOG_VIEW_FUNCTIONS_H__
