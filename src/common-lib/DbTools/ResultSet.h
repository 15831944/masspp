/**
 * @file ResultSet.h
 * @brief interfaces of ResultSet class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_RESULT_SET_H__
#define __KOME_DB_RESULT_SET_H__


namespace kome {
	namespace db {

		class Database;

		/**
		 * @class ResultSet
		 * @brief database statement class
		 */
		class DB_TOOLS_CLASS ResultSet {
		public:
			/**
			 * @fn ResultSet( Database& db )
			 * @brief constructor
			 * @param[in] db database
			 */
			ResultSet( Database& db );

			/**
			 * @fn virtual ~ResultSet()
			 * @brief destructor
			 */
			virtual ~ResultSet();

		protected:
			/** database */
			Database& m_db;

			/** has next flag */
			bool m_hasNext;

		public:
			/**
			 * @fn Database& getDb()
			 * @brief gets database
			 * @return database
			 */
			Database& getDb();

		public:
			/**
			 * @fn void close()
			 * @brief closes result set
			 */
			void close();

		public:
			/**
			* @fn bool next()
			* @brief move cursor to the next item
			* @return If true, the current item is valid.
			*/
			bool next();

			/**
			 * @fn std::string getString( const int col, const char* df )
			 * @brief gets string value
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return string value
			 */
			std::string getString( const int col, const char* df );

			/**
			 * @fn int getInt( const int col, const int df )
			 * @brief gets integer value
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return integer value
			 */
			int getInt( const int col, const int df );

			/**
			 * @fn double getReal( const int col, const double df )
			 * @brief gets real number value
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return real number value
			 */
			double getReal( const int col, const double df );

			/**
			* @fn void* getData( const int col )
			* @brief gets data
			* @param[in] col column index
			* @return data
			*/
			void* getData( const int col );

			/**
			 * @fn int getDataSize( const int col )
			 * @brief gets data size
			 * @param[in] col column index
			 * @return data size
			 */
			int getDataSize( const int col );

		protected:
			/**
			 * @fn virtual bool onNext() = 0
			 * @brief This method is called by next method. (abstract method)
			 * @return If true, the current item is valid.
			 */
			virtual bool onNext() = 0;

			/**
			 * @fn virtual std::string onGetString( const int col, const char* df )
			 * @brief This method is called by getString method. (abstract method)
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return string value
			 */
			virtual std::string onGetString( const int col, const char* df ) = 0;

			/**
			 * @fn virtual int onGetInt( const int col, const int df )
			 * @brief This method is called by getInt method. (abstract method)
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return integer value
			 */
			virtual int onGetInt( const int col, const int df ) = 0;

			/**
			 * @fn virtual double onGetReal( const int col, const double df )
			 * @brief gets This method is called by getReal method. (abstract method)
			 * @param[in] col column index
			 * @param[in] df default value
			 * @return real number value
			 */
			virtual double onGetReal( const int col, const double df ) = 0;

			/**
			* @fn virtual void* onGetData( const int col )
			* @brief This method is called by getData method. (abstract method)
			* @param[in] col column index
			* @return data
			*/
			virtual void* onGetData( const int col ) = 0;

			/**
			 * @fn virtual int onGetDataSize( const int col )
			 * @brief This method is called by getDataSize method. (abstract method)
			 * @param[in] col column index
			 * @return data size
			 */
			virtual int onGetDataSize( const int col ) = 0;
		};
	}
}


#endif	// __KOME_DB_RESULT_SET_H__
