/**
 * @file PostgreSQLPluginFunctions.h
 * @brief interfaces of PostgreSQLOutput plug-in function
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_OUTPUT_PLUGIN_FUNCTIONS_H__
#define __KOME_DB_POSTGRESQL_OUTPUT_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant getPostgreSQLDatabaseObject( kome::objects::Parameters* params )
 * @brief gets PostgreSQL database object
 * @params parameters object
 * @return PostgreSQL DB object
 */
kome::objects::Variant getPostgreSQLDatabaseObject( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_DB_POSTGRESQL_OUTPUT_PLUGIN_FUNCTIONS_H__
