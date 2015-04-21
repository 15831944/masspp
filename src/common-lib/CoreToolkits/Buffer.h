/**
 * @file Buffer.h
 * @brief interfarces of Buffer class
 *
 * @author S.Tanaka
 * @date 2007.03.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_BUFFER_H__
#define __KOME_CORE_BUFFER_H__


#include "DataAccessor.h"

#include <vector>


namespace kome {
	namespace core {

		/**
		 * @class Buffer
		 * @brief memory buffer management class
		 */
		class CORE_TOOLKITS_CLASS Buffer : public DataAccessor {
		public:
			/**
			 * @fn Buffer()
			 * @brief constructor
			 */
			Buffer();

			/**
			 * @fn virtual ~Buffer()
			 * @brief destructor
			 */
			virtual ~Buffer();

		protected:
			/** buffer */
			std::vector< unsigned char > m_buffer;

			/** position */
			unsigned long m_pos;

		public:
			/**
			 * @fn void clear()
			 * @brief clears buffer
			 */
			void clear();

		public:
			/**
			 * @fn void* getBuffer()
			 * @brief gets buffer
			 @return the pointer to the buffer
			 */
			void* getBuffer();

			/**
			 * @fn unsigned int getLength()
			 * @brief gets buffer size
			 * @return the size of the buffer
			 */
			unsigned int getLength();

			/**
			 * @fn void setPosition( const unsigned int pos )
			 * @brief sets reading position
			 * @param[in] pos reading position
			 */
			void setPosition( const unsigned int pos );

			/**
			 * @fn unsigned int getPosition()
			 * @brief gets reading position
			 * @return reading position
			 */
			unsigned int getPosition();

			/**
			 * @fn void rewind()
			 * @brief rewinds reading position
			 */
			void rewind();

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

#endif	// __KOME_CORE_BUFFER_H__
