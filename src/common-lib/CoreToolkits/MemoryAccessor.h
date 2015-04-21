/**
 * @file MemoryAccessor.h
 * @brief interfarces of MemoryAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_MEMORY_ACCESSOR_H__
#define __KOME_CORE_MEMORY_ACCESSOR_H__


#include "DataAccessor.h"


namespace kome {
	namespace core {

		/**
		 * @class MemoryAccessor
		 * @brief memory accessor class
		 */
		class CORE_TOOLKITS_CLASS MemoryAccessor : public DataAccessor {
		public:
			/**
			 * @fn MemoryAccessor( void* addr, const unsigned int size )
			 * @brief constructor
			 * @param[in] addr memory access point
			 * @param[in] size data size
			 */
			MemoryAccessor( void* addr, const unsigned int size );

			/**
			 * @fn virtual ~MemoryAccessor()
			 * @brief destructor
			 */
			virtual ~MemoryAccessor();

		protected:
			/** memory access point */
			unsigned char* m_addr;

			/** data size */
			unsigned int m_size;

			/** reading position */
			unsigned int m_pos;

		public:
			/**
			 * @fn void* getData()
			 * @brief gets memory access point
			 * @return memory access point
			 */
			void* getData();

			/**
			 * @fn unsigned int getSize()
			 * @brief gets data size
			 * @return data size
			 */
			unsigned int getSize();

			/**
			 * @fn void setPosition( const unsigned int pos )
			 * @brief sets reading postion
			 * @param[in] pos reading position
			 */
			void setPosition( const unsigned int pos );

		public:
			/**
			 * @fn virtual int write( void* addr, int size )
			 * @brief writes data (override method)
			 * @param[in] addr the pointer to data
			 * @param[in] size data size
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


#endif	// __KOME_CORE_MEMORY_ACCESSOR_H__
