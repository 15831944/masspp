/**
 * @file Variant.h
 * @brief interfaces of Variant class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_VARIANT_H__
#define __KOME_OBJECTS_VARIANT_H__


#include <map>


namespace kome {
	namespace objects {

		/**
		 * @class Variant
		 * @brief class that treats data of various types
		 */
		class DATA_OBJECTS_CLASS Variant {
		public:
			/**
			 * @fn Variant()
			 * @brief constructor
			 */
			Variant();

			/**
			 * @fn Variant( const Variant& v )
			 * @brief copy constructor
			 * @param[in] v variant object to copy
			 */
			Variant( const Variant& v );

			/**
			 * @fn virtual ~Variant()
			 * @brief destructor
			 */
			virtual ~Variant();

		public:
			/**
			 * @fn Variant& operator=( const Variant& other )
			 * @brief substitution operator
			 * @param[in] other source variant object
			 * @return copied variant object
			 */
			Variant& operator=( const Variant& other );

		public:
			/** data type */
			typedef enum {
				UNKNOWN,
				BOOL,
				INTEGER,
				DOUBLE,
				STRING,
				ARRAY,
				PAIR,
				SAMPLE_SET,
				SAMPLE,
				SAMPLE_GETTER,
				SPECTRUM,
				DATA_SET,
				DATA_GROUP_NODE,
				CHROMATOGRAM,
				XY_DATA,
				PEAKS,
				PEAKS_2D,
				DRAWING_DATA,
				GRAPHICS,
				FONT,
				PROGRESS,
				MOUSE_EVT,
				KEY_EVT,
				DATA_MAP,
				SIZE_EVT,
				MOVE_EVT,
				WINDOW,
				SETTING_VALUES,
				SCRIPT,
				FILE_READER,
				OTHER
			} DataType;

			/** data type */
			DataType type;

			/** primitive type value */
			union PrimitiveType {
				bool	 boolVal;
				int	  intVal;
				double   dblVal;
				void*	pt;
			} prim;

		protected:
			/** array */
			void* arr;
			
		public:
			/**
			 * @fn void setString( const char* s )
			 * @brief sets character string
			 * @param[in] s character string
			 */
			void setString( const char* s );

			/**
			 * @fn char* getString()
			 * @brief gets character string
			 * @return character string
			 */
			char* getString();

			/**
			 * @fn Variant* createArray( const unsigned int size )
			 * @brief creates array
			 * @param[in] size array size
			 * @return new array
			 */
			Variant* createArray( const unsigned int size );

			/**
			 * @fn Variant* getArray()
			 * @brief gets array
			 * @return the pointer to array
			 */
			Variant* getArray();

			/**
			 * @fn Variant* getElement( const unsigned int index )
			 * @brief gets element of the array
			 * @param[in] index element index
			 * @return element of the array (If index is illegal, this method returns NULL.)
			 */
			Variant* getElement( const unsigned int index );

			/**
			 * @fn unsigned int getArraySize()
			 * @brief gets array size
			 * @return array size
			 */
			unsigned int getArraySize();

		protected:
			/**
			 * @fn void deleteArray()
			 * @brief deletes array
			 */
			void deleteArray();

		};
	}
}

#endif	// __KOME_OBJECTS_VARIANT_H__
