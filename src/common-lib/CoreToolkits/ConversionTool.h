/**
 * @file ConversionTool.h
 * @brief interfaces of IDTool class
 *
 * @author S.Tanaka
 * @date 2006.09.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_CONVERSION_TOOL_H__
#define __KOME_CORE_CONVERSION_TOOL_H__


#include <string>


// macro
#define GET_ID_NAME( id, idArray )							\
	kome::core::ConversionTool::getIdName(					\
		id,													\
		idArray,											\
		sizeof( idArray ) / sizeof( kome::core::IDInfo )	\
	);

#define CONVERT_FROM_ARRAY( key, defaultValue, conversionArray )	\
	kome::core::ConversionTool::convert(							\
		key,														\
		defaultValue,												\
		conversionArray,											\
		sizeof( conversionArray ) / sizeof( conversionArray[ 0 ] )	\
	);

#define INVERT_FROM_ARRAY( key, defaultValue, conversionArray )		\
	kome::core::ConversionTool::invert(								\
		key,														\
		defaultValue,												\
		conversionArray,											\
		sizeof( conversionArray ) / sizeof( conversionArray[ 0 ] )	\
	);

// class
namespace kome {
	namespace core {

		/**
		 * @struct IDInformation		 
		 * @brief struct that has ID and name
		 */
		struct IDInfo {
			/** ID */
			int id;
			/** name */
			const char* name;
		};


		/**
		 * @struct ConversionElement
		 * @brief key and value data
		 */
		template< typename S, typename T > struct ConversionElement {
			S key;
			T value;
		};

		/**
		 * @class ConversionTool
		 * @brief the class that has some functions for convert
		 */
		class CORE_TOOLKITS_CLASS ConversionTool {
		public:
			/**
			 * @fn static std::string getIdName(
					const int id,
					IDInfo* idInfoArray,
					const unsigned int arraySize					
				)
			 * @brief gets name from id number
			 * @param[in] id ID
			 * @param[in] idInfoArray the array of ID informations
			 * @param[in] arraySize array size
			 * @return name ( If specified ID is not found, this method returns empty string. )
			 */
			static std::string getIdName(
				const int id,
				IDInfo* idInfoArray,
				const unsigned int arraySize
			);

			/**
			 * @fn static T convert(
					const S key,
					const T dfVal,
					ConversionElement< S, T >* elementArray,
					int arraySize
				)
			 * @brief gets converted value from key ( S -> T )
			 * @param[in] key key
			 * @param[in] dfVal default value to be returned when the specified key is not found
			 * @param[in] elementArray the array of conversion element
			 * @param arraySize the size of array
			 * @return converted value 
			 */
			template< typename S, typename T >
			static T convert(
					const S key,
					const T dfVal,
					ConversionElement< S, T >* elementArray,
					int arraySize
			) {
				// check arguments
				if( elementArray == NULL || arraySize <= 0 ) {
					return dfVal;
				}

				// search
				for( int i = 0; i < arraySize; i++ ) {
					if( key == elementArray[ i ].key ) {
						return elementArray[ i ].value;
					}
				}

				return dfVal;
			}

			/**
			 * @fn static S invert(
					const T key,
					const S dfVal,
					ConversionElement< S, T >* elementArray,
					int arraySize
				)
			 * @brief gets converted value from key ( T -> S )
			 * @param[in] key key
			 * @param[in] dfVal default value to be returned when the specified key is not found
			 * @param[in] elementArray the array of conversion element
			 * @param[in] arraySize the size of array
			 * @return converted value 
			 */
			template< typename S, typename T >
			static S invert(
					const T key,
					const S dfVal,
					ConversionElement< S, T >* elementArray,
					int arraySize
			) {
				// check arguments
				if( elementArray == NULL || arraySize <= 0 ) {
					return dfVal;
				}

				// search
				for( int i = 0; i < arraySize; i++ ) {
					if( key == elementArray[ i ].value ) {
						return elementArray[ i ].key;
					}
				}

				return dfVal;
			}
		};
	}
}

#endif	// __KOME_CORE_CONVERSION_TOOL_H__
