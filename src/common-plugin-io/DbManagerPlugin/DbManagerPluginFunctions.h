/**
 * @file DbManagerPluginFunctions.h
 * @brief interfaces of DbManager plug-in function
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_DB_MANAGER_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_DB_MANAGER_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant prepareDb( kome::objects::Parameters* )
 * @brief prepares DB
 * @return This value has no meanings.
 */
kome::objects::Variant prepareDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant checkDatabaseSchema( kome::objects::Parameters* )
 * @brief check the database schema
 * @return This value has no meanings.
 */
kome::objects::Variant checkDatabaseSchema( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant changeDb( kome::objects::Parameters* )
 * @brief changes DB
 * @return This value has no meanings.
 */
kome::objects::Variant changeDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openDbDialog( kome::objects::Parameters* )
 * @brief opens database dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openDbDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant selectDb( kome::objects::Parameters* params )
 * @brief selects database
 * @param params parameters object
 * @return If true, selecting database is succeeded.
 */
kome::objects::Variant selectDb( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant releaseDb( kome::objects::Parameters* )
 * @brief releases DB
 * @return This value has no meanings.
 */
kome::objects::Variant releaseDb( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_DB_MANAGER_PLUGIN_FUNCTIONS_H__
