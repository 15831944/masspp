/**
 * @file SettingParameterValues.h
 * @brief interfaces of SettingParameterValues class
 *
 * @author S.Tanaka
 * @date 2008.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_SETTING_PARAMETER_VALUES_H__
#define __KOME_OBJECTS_SETTING_PARAMETER_VALUES_H__


#include <string>
#include <vector>
#include <utility>
#include <boost/utility.hpp>



#define MSPP_SETTINGS_NONE_VALUE         "none"


namespace kome {
	namespace objects {

		/**
		 * @class SettingParameterValues
		 * @brief setting parameter values management class
		 */
		class DATA_OBJECTS_CLASS SettingParameterValues {
		public:
			/**
			 * @fn SettingParameterValues()
			 * @brief constructor
			 */
			SettingParameterValues();

			/**
			 * @fn SettingParameterValues( const  SettingParameterValues& other )
			 * @brief copy constructor
			 * @param[in] other other settings object
			 */
			SettingParameterValues( const SettingParameterValues& other );

			/**
			 * @fn virtual ~SettingParameterValues()
			 * @brief destructor
			 */
			virtual ~SettingParameterValues();

		protected:
			/**
			 * @struct SubSettingsKey
			 * @brief sub settings key
			 */
			struct SubSettingsKey {
				std::string key;
				std::string value;
			};
				
			/** values */
			std::vector< std::pair< std::string, std::string > > m_values;

			/** item values */
			std::vector< std::pair< SubSettingsKey, SettingParameterValues* > > m_itemValues;

		public:
			/**
			 * @fn SettingParameterValues& operator=( const SettingParameterValues& other )
			 * @brief the definition of assignment operator
			 * @param[in] other other settings object
			 * @return assigned settings object
			 */
			SettingParameterValues& operator=( const SettingParameterValues& other );

		public:
			/**
			 * @fn void clear()
			 * @brief clears parameters
			 */
			void clear();

		protected:
			/** 
			 * @fn int getIndex( const char* name )
			 * @brief gets parameter index from name
			 * @param[in] name parameter name
			 * @return parameter index (If negative value, specified parameter name was not found.)
			 */
			int getIndex( const char* name );

		public:
			/**
			 * @fn void setValue( const unsigned int index, const char* value )
			 * @brief sets parameter value
			 * @param[in] index parameter index
			 * @param[in] value parameter value
			 */
			void setValue( const unsigned int index, const char* value );

			/**
			 * @fn void setValue( const char* name, const char* value )
			 * @brief sets parameter value
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 */
			void setValue( const char* name, const char* value );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn const char* getParameterName( const unsigned int index )
			 * @brief gets parameter name
			 * @param[in] index parameter index
			 * @return parameter name
			 */
			const char* getParameterName( const unsigned int index );

			/**
			 * @fn const char* getParameterValue( const unsigned int index )
			 * @brief gets parameter value
			 * @param[in] index parameter index
			 * @return parameter value
			 */
			const char* getParameterValue( const unsigned int index );

			/**
			 * @fn const char* getParameterValue( const char* name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value (If NULL, there is no parameter that has specified name.)
			 */
			const char* getParameterValue( const char* name );

			/**
			 * @fn bool getBoolValue( const unsigned int index, const bool df )
			 * @brief gets boolean value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return boolean value
			 */
			bool getBoolValue( const unsigned int index, const bool df );

			/**
			 * @fn bool getBoolValue( const char* name, const bool df )
			 * @brief gets boolean value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return boolean value
			 */
			bool getBoolValue( const char* name, const bool df );

			/**
			 * @fn int getIntValue( const unsigned int index, const int df )
			 * @brief gets integer value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return integer value
			 */
			int getIntValue( const unsigned int index, const int df );

			/**
			 * @fn int getIntValue( const char* name, const int df )
			 * @brief gets integer value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return integer value
			 */
			int getIntValue( const char* name, const int df );

			/**
			 * @fn double getDoubleValue( const unsigned int index, const double df )
			 * @brief gets double value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return double value
			 */
			double getDoubleValue( const unsigned int index, const double df );

			/**
			 * @fn double getDoubleValue( const char* name, const double df )
			 * @brief gets double value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return double value
			 */
			double getDoubleValue( const char* name, const double df );

			/**
			 * @fn SettingParameterValues* getSubParameterValues( const char* key, const char* val = NULL )
			 * @brief gets sub parameter values
			 * @param[in] key parameter key
			 * @param[in] val parameter value
			 * @return sub parameter values
			 */
			SettingParameterValues* getSubParameterValues( const char* key, const char* val = NULL );

			/**
			 * @fn bool hasSubParameterValues( const char* key, const char* val = NULL )
			 * @brief check the specified parameter has sub parameters
			 * @param[in] key parameter key
			 * @param[in] val parameter value
			 * @return If true, the specified parameter has sub parameters
			 */
			bool hasSubParameterValues( const char* key, const char* val = NULL );

		public:
			/**
			 * @fn static void writeAllChildParameters(
						kome::objects::SettingParameterValues& settings,
						kome::core::Properties& props,
						const char* prefix
					)
			 * @brief write all child parameters
			 * @param[in] settings parameters
			 * @param[out] props property objects to be stored.
			 * @param[in] prefix prefix string
			 */
			static void writeAllChildParameters(
					kome::objects::SettingParameterValues& settings,
					kome::core::Properties& props,
					const char* prefix
			);
		};
	}
}

#endif // __KOME_OBJECTS_SETTING_PARAMETER_VALUES_H__
