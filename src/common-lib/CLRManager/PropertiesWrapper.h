/**
 * @file PropertiesWrapper.h
 * @brief interfaces of PropertiesWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PROPERTIES_WRAPPER_H__
#define __KOME_CLR_PROPERTIES_WRAPPER_H__

namespace kome {
	namespace clr {

		/**
		 * @class PropertiesWrapper
		 * @brief Properties object wrapper class to use on CLR environment
		 */
		public ref class PropertiesWrapper {
		public:
			/**
			 * @fn PropertiesWrapper()
			 * @brief constructor
			 */
			PropertiesWrapper();

			/**
			 * @fn PropertiesWrapper( kome::core::Properties& props )
			 * @brief constructor
			 * @param[in] props properties object
			 */
			PropertiesWrapper( kome::core::Properties& props );

			/**
			 * @fn virtual ~PropertiesWrapper()
			 * @brief destructor
			 */
			virtual ~PropertiesWrapper();

		protected:
			/** properties object */
			kome::core::Properties& m_props;

			/** delete flag */
			bool m_deleteFlag;

		public:
			/**
			 * @fn kome::core::Properties& getProperties()
			 * @brief gets properties object
			 * @return properties object
			 */
			kome::core::Properties& getProperties();

		public:
			/**
			 * @fn void setValue( System::String^ key, System::String^ value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setValue( System::String^ key, System::String^ value );

			/**
			 * @fn void setIntValue( System::String^ key, int value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setIntValue( System::String^ key, int value );

			/**
			 * @fn void setDoubleValue( System::String^ key, double value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setDoubleValue( System::String^ key, double value );

			/**
			 * @fn void setBoolValue( System::String^ key, bool value )
			 * @brief sets parameter value
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setBoolValue( System::String^ key, bool value );

			/**
			 * @fn void setIntValues( System::String^ key, cli::array< int >^ values )
			 * @brief set integer values
			 * @param[in] key parameter key
			 * @param[in] values array of integer values
			 */
			void setIntValues( System::String^ key, cli::array< int >^ values );

			/**
			 * @fn bool hasKey( System::String^ key )
			 * @brief judges whether this object has specified parameter key
			 * @param[in] key parameter key
			 * @return If there is specified parameter key, this method returns true.
			 */
			bool hasKey( System::String^ key );

			/**
			 * @fn System::String^ getStringValue( System::String^ key, System::String^ defaultValue )
			 * @brief gets parameter value
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If this object doesn't have specified key, this method returns default value.)
			 */
			System::String^ getStringValue( System::String^ key, System::String^ defaultValue );

			/**
			 * @fn int getIntValue( System::String^ key, int defaultValue )
			 * @brief gets parameter value converted to integer
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to integer
					   (If it is impossible to convert this parameter value to integer, this method returns default value.)
			 */
			int getIntValue( System::String^ key, int defaultValue );

			/**
			 * @fn double getDoubleValue( System::String^ key, double defaultValue )
			 * @brief gets parameter value converted to double
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to double
					   (If it is impossible to convert this parameter value to double, this method returns default value.)
			 */
			double getDoubleValue( System::String^ key, double defaultValue );

			/**
			 * @fn bool getBoolValue( System::String^ key, bool defaultValue )
			 * @brief gets parameter value converted to bool
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value converted to bool
					   (If it is impossible to convert this parameter value to bool, this method returns default value.)
			 */
			bool getBoolValue( System::String^ key, bool defaultValue );

			/**
			 * @fn void getIntValues(
						System::String^ key,
						System::Collections::IList^ list,
					)
			 * @brief gets integer values from comma separated values
			 * @param[in] key parameter key
			 * @param[in] list list object to store int values
			 */
			void getIntValues(
				System::String^ key,
				System::Collections::IList^ list
			);

			/**
			 * @fn unsigned int getNumberOfProperties()
			 * @brief gets the number of Properties
			 * @return the number of Properties
			 */
			unsigned int getNumberOfProperties();

			/**
			 * @fn System::String^ getKey( unsigned int index )
			 * @brief gets the name of parameter
			 * @param[in] index key index
			 * @return the name of parameter (If index is illegal number, this method returns NULL)
			 */			 
			System::String^ getKey( unsigned int index );

			/**
			 * @fn System::String^ getValue( unsigned int index )
			 * @brief gets the parameter value
			 * @param[in] index key index
			 * @return the value of parameter (If index is illegal number, this method returns NULL)
			 */
			System::String^ getValue( unsigned int index );

			/**
			 * @fn void deleteParameter( System::String^ key )
			 * @brief deletes parameter
			 * @param[in] key parameter key to be deleted
			 */
			void deleteParameter( System::String^ key );

			/**
			 * @fn System::String^ replaceString(
					System::String^ s,
					System::String^ prefix,
					System::String^ suffix,
					System::String^ defaultString
				)
			 * @brief replaces string "(prefix)(Property Key)(suffix)" to "(Property Value)"
			 * @param[in] s character string
			 * @param[in] prefix property key prefix
			 * @param[in] suffix property key suffix
			 * @param[in] defaultString the string to replace when specified key is not found
			 */
			System::String^ replaceString(
				System::String^ s,
				System::String^ prefix,
				System::String^ suffix,
				System::String^ defaultString
			);
		};
	}
}

#endif	// __KOME_CLR_PROPERTIES_WRAPPER_H__
