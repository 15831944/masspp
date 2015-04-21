/**
 * @file Base64.h
 * @brief interfarces of Base64 class
 *
 * @author S.Tanaka
 * @date 2007.03.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_BASE64_H__
#define __KOME_CORE_BASE64_H__


namespace kome {
	namespace core {

		/**
		 * @class Base64
		 * @brief base64 conversion class
		 */
		class CORE_TOOLKITS_CLASS Base64 {
		protected:
			/**
			 * @fn Base64()
			 * @brief constructor
			 */
			Base64();

			/**
			 * @fn virtual ~Base64()
			 * @brief destructor
			 */
			virtual ~Base64();

		protected:
			/** encode map */
			char m_encodeMap[ 64 ];

			/** decode map */
			int m_decodeMap[ 256 ];

		protected:
			/**
			 * @fn void createMap()
			 * @brief creates encode and decode map
			 */
			void createMap();

		public:

			/**
			 * @fn static unsigned int encode(
					void* src,
					const unsigned int srcLength,
					char* dest,
					const unsigned int destLength
				)
			 * @brief encodes source buffer into the destination buffer
			 * @param[in] src source buffer
			 * @param[in] srcLength byte length of the source buffer
			 * @param[out] dest destination buffer
			 * @param[in] destLength byte length of the destination buffer
			 * @return If negative value, destination buffer size is too small, otherwise returns the size of encoded data
			 */
			static int encode(
				void* src,
				const unsigned int srcLength,
				char* dest,
				const unsigned int destLength
			);

			/**
			 * @fn static int decode(
					char* src,
					const unsigned int srcLength,
					void* dest,
					const unsigned int destLength
				)
			 * @brief decodes source buffer into the destination buffer
			 * @param[in] src source buffer
			 * @param[in] srcLength byte length of the source buffer
			 * @param[out] dest destination buffer
			 * @param[in] destLength byte length of the destination buffer
			 * @return If negative value, destination buffer size is to small, otherwise returns the size of decoded data
			 */
			static int decode(
				char* src,
				const unsigned int srcLength,
				void* dest,
				const unsigned int destLength
			);
		
		protected:
			/**
			 * @fn static Base64& getInstance()
			 * @brief gets base64 object
			 * @return base64 object (This is the only object.)
			 */
			static Base64& getInstance();
		};
	}
}


#endif		// __KOME_CORE_BASE64_H__
