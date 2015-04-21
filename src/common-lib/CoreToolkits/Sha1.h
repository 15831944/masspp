/**
 * @file Sha1.h
 * @brief interfarces of Sha1 class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_SHA1_H__
#define __KOME_CORE_SHA1_H__


namespace kome {
	namespace core {
		/**
		 * @class Sha1
		 * @brief the class computing SHA1 hash
		 */
		class CORE_TOOLKITS_CLASS Sha1 {
		protected:
			/**
			 * @fn static unsigned long f( const long t, const unsigned long b, const unsigned long c, const unsigned long d )
			 * @brief logical function used in SHA1
			 * @param[in] t t count
			 * @param[in] b variable B
			 * @param[in] c variable C
			 * @param[in] d variable D
			 * @return computed value
			 */
			static unsigned long f( const long t, const unsigned long b, const unsigned long c, const unsigned long d );

			/**
			 * @fn static unsigned long K( const long t )
			 * @brief constant word used in SHA1
			 * @param[in] t t count
			 * @return constant word
			 */
			static unsigned long K( const long t );

		public:
			/**
			 * @fn static std::string transformData( void* src, const unsigned int length )
			 * @brief transform data
			 * @param[in] src source data
			 * @param[in] length data length
			 * @return SHA-1 string
			 */
			static std::string transformData( void* src, const unsigned int length );

			/**
			 * @fn static std::string transformFile( const char* path )
			 * @brief transform
			 * @param[in] path file path
			 * @return SHA-1 string
			 */
			static std::string transformFile( const char* path );

		protected:
			/**
			 * @class Sha1Context
			 * @brief SHA1 context
			 */
			class Sha1Context {
			public:
				/**
				 * @fn Sha1Context()
				 * @brief constructor
				 */
				Sha1Context();

				/**
				 * @fn virtual ~Sha1Context()
				 * @brief destructor
				 */
				virtual ~Sha1Context();

				/**
				 * @fn void appendMessage( unsigned char* msg, const unsigned long length )
				 * @brief appends message
				 * @param[in] msg the pointer to the message
				 * @param[in] length message length
				 */
				void appendMessage( unsigned char* msg, const unsigned long length );

				/**
				 * @fn void finish()
				 * @brief finishes to append message
				 */
				void finish();

			protected:
				/**
				 * @fn void updateHash()
				 * @brief updates hash
				 */
				void updateHash();

				/**
				 * @fn void padBlock()
				 * @brief pads block
				 */
				void padBlock();

			public:
				/** intermediate hash */
				unsigned long m_h[ 5 ];

				/** block */
				unsigned char m_block[ 64 ];

				/** length */
				unsigned long m_length;

				/** position */
				unsigned long m_pos;

			public:
				/**
				 * @fn void reset()
				 * @brief resets member variables
				 */
				void reset();
			};
		};
	}
}


#endif		// __KOME_CORE_SHA1_H__
