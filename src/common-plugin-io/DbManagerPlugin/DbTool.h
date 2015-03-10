/**
 * @file DbTool.h
 * @brief interfaces of DbTool class
 *
 * @author S.Tanaka
 * @date 2008.12.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_DB_DB_TOOL_H__
#define __KOME_IO_DB_DB_TOOL_H__


#define DB_FUNCTION_TYPE						"GET_DB"

#define SECTION									"Database"
#define DB_PARAM_KEY						    "DATABASE"

#define DB_PARAM_NAME							"database"


namespace kome{
	namespace io {
		namespace db {

			/**
			 * @class DbTool
			 * @brief This class has some functions of database management
			 */
			class DbTool {
			public:
				/**
				 * @fn static kome::db::Database* getDb()
				 * @brief get database
				 * @return database
				 */
				static kome::db::Database* getDb();

				/**
				 * @fn static kome::db::Database* getDb(
						kome::plugin::PluginFunctionItem* dbGetter,
						kome::objects::SettingParameterValues* settings
					)
				 * @brief gets DB from function
				 * @param dbGetter DB getter funtion
				 * @param settings database settings
				 * @return database
				 */
				static kome::db::Database* getDb(
					kome::plugin::PluginFunctionItem* dbGetter,
					kome::objects::SettingParameterValues* settings
				);

				/**
				 * @fn static kome::db::Database* getFirstDb()
				 * @brief gets first database
				 * @return first database
				 */
				static kome::db::Database* getFirstDb();

				/**
				 * @fn static void setDb( kome::db::Database* db )
				 * @brief sets database
				 * @param db database
				 */
				static void setDb( kome::db::Database* db );
			};
		}
	}
}

#endif	// __KOME_IO_DB_DB_TOOL_H__
