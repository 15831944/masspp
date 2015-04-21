/**
 * @file DataAccessor.h
 * @brief interfarces of DataAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_DATA_ACCESSOR_H__
#define __KOME_CORE_DATA_ACCESSOR_H__


namespace kome {
	namespace core {

		/**
		 * @class DataAccessor
		 * @brief abstract class of data accessor
		 */
		class CORE_TOOLKITS_CLASS DataAccessor {
		public:
			/**
			 * @fn DataAccessor()
			 * @brief constructor
			 */
			DataAccessor();

			/**
			 * @fn virtual ~DataAccessor()
			 * @brief destructor
			 */
			virtual ~DataAccessor();

		public:
			/**
			 * @fn virtual int write( void* addr, int size ) = 0
			 * @brief writes data (abstract method)
			 * @param[in] addr the pointer to data
			 * @param[in] size data size
			 * @return actually writing size
			 */
			virtual int write( void* addr, int size ) = 0;

			/**
			 * @fn virtual int read( void* addr, int size ) = 0
			 * @brief reads data (abstract method)
			 * @param[out] addr the pointer to store data
			 * @param[in] size read size
			 * @return actually reading size
			 */
			virtual int read( void* addr, int size ) = 0;
		};
	}
}

#endif	// __KOME_CORE_DATA_ACCESSOR_H__
