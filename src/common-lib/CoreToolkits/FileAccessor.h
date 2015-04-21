/**
 * @file FileAccessor.h
 * @brief interfarces of FileAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_FILE_ACCESSOR_H__
#define __KOME_CORE_FILE_ACCESSOR_H__


#include "DataAccessor.h"


namespace kome {
	namespace core {

		/**
		 * @class FileAccessor
		 * @brief file accessor class
		 */
		class CORE_TOOLKITS_CLASS FileAccessor : public DataAccessor {
		public:
			/**
			 * @fn FileAccessor( FILE* fp )
			 * @brief constructor
			 * @param[in] fp file descriptor
			 */
			FileAccessor( FILE* fp );

			/**
			 * @fn virtual ~FileAccessor()
			 * @brief destructor
			 */
			virtual ~FileAccessor();

		protected:
			/** file descriptor */
			FILE* m_fp;

		public:
			/**
			 * @fn FILE* getFile()
			 * @brief gets file descriptor
			 * @return file descriptor
			 */
			FILE* getFile();

		public:
			/**
			 * @fn virtual int write( void* addr, int size )
			 * @brief writes data (override method)
			 * @param[in] addr the pointer to data
			 * @param size data size
			 * @return actually writing size
			 */
			virtual int write( void* addr, int size );

			/**
			 * @fn virtual int read( void* addr, int size )
			 * @brief reads data (override method)
			 * @param[out] addr the pointer to store data
			 * @param[in] size read size
			 * @return actually reading size
			 */
			virtual int read( void* addr, int size );
		};
	}
}


#endif	// __KOME_CORE_FILE_ACCESSOR_H__
