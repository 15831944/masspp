/**
 * @file SQLiteOutputPluginFunctdbns.h
 * @brief interfaces of SQLiteOutput plug-in function
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_SQLITE_OUTPUT_PLUGIN_FUNCTIONS_H__
#define __KOME_DB_SQLITE_OUTPUT_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant getSQLiteDatabaseObject( kome::objects::Parameters* params )
 * @brief gets SQLite database object
 * @params params parameters object
 * @return SQLite DB object
 */
kome::objects::Variant getSQLiteDatabaseObject( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant checkDbFile( kome::objects::Parameters* params )
 * @brief checks DB file
 * @param[in] params parameters object
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant checkDbFile( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_DB_SQLITE_OUTPUT_PLUGIN_FUNCTIONS_H__
