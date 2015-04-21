/**
 * @file Properties.h
 * @brief interfaces of Properties class
 *
 * @author S.Tanaka
 * @date 2006.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_PROPERTIES_H__
#define __KOME_CORE_PROPERTIES_H__


#include <string>
#include <vector>
#include <utility>
#include <boost/function.hpp>


namespace kome {
	namespace core {

		/**
		 * @class Properties
		 * @brief keys and values management class
		 */
		class CORE_TOOLKITS_CLASS Properties {
		public:
			/**
			 * @fn Properties()
			 * @brief constructor
			 */
			Properties();

			/**
			 * @fn virtual ~Properties()
			 * @brief destructor
			 */
			virtual ~Properties();

		protected:
			/** properties */
			std::vector< std::pair< std::string, std::string > > m_props;

		public:
			/**
			 * @fn void setValue( const char* key, const char* value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setValue( const char* key, const char* value );

			/**
			 * @fn void setIntValue( const char* key, int value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setIntValue( const char* key, int value );

			/**
			 * @fn void setDoubleValue( const char* key, double value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setDoubleValue( const char* key, double value );

			/**
			 * @fn void setBoolValue( const char* key, bool value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setBoolValue( const char* key, bool value );

			/**
			 * @fn void setIntValues( const char* key, int* values, int size )
			 * @brief set integer values
			 * @param[in] key parameter key
			 * @param[in] values array of integer values
			 * @param[in] size values array size
			 */
			void setIntValues( const char* key, int* values, int size );

			/**
			 * @fn bool hasKey( const char* key )
			 * @brief judges whether this object has specified parameter key
			 * @param[in] key parameter key
			 * @return If there is specified parameter key, this method returns true.
			 */
			bool hasKey( const char* key );

			/**
			 * @fn const char* getStringValue( const char* key, const char* defaultValue )
			 * @brief gets parameter value
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If this object doesn't have specified key, this method returns default value.)
			 */
			const char* getStringValue( const char* key, const char* defaultValue );

			/**
			 * @fn int getIntValue( const char* key, int defaultValue )
			 * @brief gets parameter value converted to integer
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to integer
					   (If it is impossible to convert this parameter value to integer, this method returns default value.)
			 */
			int getIntValue( const char* key, int defaultValue );

			/**
			 * @fn double getDoubleValue( const char* key, double defaultValue )
			 * @brief gets parameter value converted to double
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to double
					   (If it is impossible to convert this parameter value to double, this method returns default value.)
			 */
			double getDoubleValue( const char* key, double defaultValue );

			/**
			 * @fn bool getBoolValue( const char* key, bool defaultValue )
			 * @brief gets parameter value converted to bool
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to bool
					   (If it is impossible to convert this parameter value to bool, this method returns default value.)
			 */
			bool getBoolValue( const char* key, bool defaultValue );

			/**
			 * @fn void getIntValues(const char* key, std::vector<int> &values)
			 * @brief gets integer values from comma separated values
			 * @param[in] key parameter key
			 * @param[out] values stores integer value array
			 */
			void getIntValues(const char* key, std::vector<int> &values);

			/**
			 * @fn unsigned int getNumberOfProperties()
			 * @brief gets the number of Properties
			 * @return the number of Properties
			 */
			unsigned int getNumberOfProperties();

			/**
			 * @fn const char* getKey( const unsigned int index )
			 * @brief gets the name of parameter
			 * @param[in] index key index
			 * @return the name of parameter (If index is illegal number, this method returns NULL)
			 */			 
			const char* getKey( const unsigned int index );

			/**
			 * @fn const char* getValue( const unsigned int index )
			 * @brief gets the parameter value
			 * @param[in] index key index
			 * @return the value of parameter (If index is illegal number, this method returns NULL)
			 */
			const char* getValue( const unsigned int index );

			/**
			 * @fn void deleteParameter( const char* key )
			 * @brief deletes parameter
			 * @param[in] key parameter key to be deleted
			 */
			void deleteParameter( const char* key );

			/**
			 * @fn std::string replaceString(
					const char* s,
					const char* prefix,
					const char* suffix,
					const char* defaultString
				)
			 * @brief replaces string "(prefix)(Property Key)(suffix)" to "(Property Value)"
			 * @param[in] s character string
			 * @param[in] prefix property key prefix
			 * @param[in] suffix property key suffix
			 * @param[in] defaultString the string to replace when specified key is not found
			 */
			std::string replaceString(
				const char* s,
				const char* prefix,
				const char* suffix,
				const char* defaultString
			);
		};
	}
}

#endif	// __KOME_CORE_Properties_H__
