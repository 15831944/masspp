/**
 * @file SettingParameterValuesWrapper.h
 * @brief interfaces of SettingParameterValuesWrapper class
 *
 * @author S.Tanaka
 * @date 2008.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTING_PARAMETER_VALUES_WRAPPER_H__
#define __KOME_CLR_SETTING_PARAMETER_VALUES_WRAPPER_H__


namespace kome {
	namespace clr {

		/**
		 * @class SettingParameterValuesWrapper
		 * @brief setting parameter values wrapper class to use on CLR environment
		 */
		public ref class SettingParameterValuesWrapper {
		public:
			/**
			 * @fn SettingParameterValuesWrapper( kome::objects::SettingParameterValues& settings )
			 * @brief constructor
			 * @param[in] settings setting parameter values object to be wrapped
			 */
			SettingParameterValuesWrapper( kome::objects::SettingParameterValues& settings );

			/**
			 * @fn SettingParameterValuesWrapper()
			 * @brief constructor
			 */
			SettingParameterValuesWrapper();

			/**
			 * @fn virtual ~SettingParameterValuesWrapper()
			 * @brief destructor
			 */
			virtual ~SettingParameterValuesWrapper();

		protected:
			/** setting parameter values object */
			kome::objects::SettingParameterValues* m_settings;

			/** setting parameter values */
			kome::objects::SettingParameterValues* m_obj;

		public:
			/** 
			 * @fn kome::objects::SettingParameterValues& getSettingParameterValues()
			 * @brief gets setting parameter vaules object to be wrapped
			 * @return setting parameter values object
			 */
			kome::objects::SettingParameterValues& getSettingParameterValues();

		public:
			/**
			 * @fn void clear()
			 * @brief clears parameters
			 */
			void clear();
		
		public:
			/**
			 * @fn void setValue( unsigned int index, System::String^ value )
			 * @brief sets parameter value
			 * @param[in] index parameter index
			 * @param[in] value parameter value
			 */
			void setValue( unsigned int index, System::String^ value );

			/**
			 * @fn void setValue( System::String^ name, System::String^ value )
			 * @brief sets parameter value
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 */
			void setValue( System::String^ name, System::String^ value );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn System::String^ getParameterName( unsigned int index )
			 * @brief gets parameter name
			 * @param[in] index parameter index
			 * @return parameter name
			 */
			System::String^ getParameterName( unsigned int index );

			/**
			 * @fn System::String^ getParameterValue( unsigned int index )
			 * @brief gets parameter value
			 * @param[in] index parameter index
			 * @return parameter value
			 */
			System::String^ getParameterValue( unsigned int index );

			/**
			 * @fn System::String^ getParameterValue( System::String^ name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value (If NULL, there is no parameter that has specified name.)
			 */
			System::String^ getParameterValue( System::String^ name );

			/**
			 * @fn bool getBoolValue( unsigned int index, bool df )
			 * @brief gets boolean value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return boolean value
			 */
			bool getBoolValue( unsigned int index, bool df );

			/**
			 * @fn bool getBoolValue( System::String^ name, bool df )
			 * @brief gets boolean value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return boolean value
			 */
			bool getBoolValue( System::String^ name, bool df );

			/**
			 * @fn int getIntValue( unsigned int index, int df )
			 * @brief gets integer value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return integer value
			 */
			int getIntValue( unsigned int index, int df );

			/**
			 * @fn int getIntValue( System::String^ name, int df )
			 * @brief gets integer value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return integer value
			 */
			int getIntValue( System::String^ name, int df );

			/**
			 * @fn double getDoubleValue( unsigned int index, double df )
			 * @brief gets double value
			 * @param[in] index parameter index
			 * @param[in] df default value
			 * @return double value
			 */
			double getDoubleValue( unsigned int index, double df );

			/**
			 * @fn double getDoubleValue( System::String^ name, double df )
			 * @brief gets double value
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return double value
			 */
			double getDoubleValue( System::String^ name, double df );

			/** 
			 * @fn SettingParameterValuesWrapper^ getSubParameterValues( System::String^ key, System::String^ val );
			 * @brief gets sub setting values object
			 * @param[in] key parameter key
			 * @param[in] val parameter value
			 * @return sub setting values object (If NULL, there is not sub settings value object that has specified name.)
			 */
			SettingParameterValuesWrapper^ getSubParameterValues( System::String^ key, System::String^ val );

			/**
			 * @fn bool hasSubParameterValues( System::String^ key, System::String^ val )
			 * @brief check the specified parameter has sub parameters
			 * @param[in] key parameter key
			 * @param[in] val parameter value
			 * @return If true, the specified parameter has sub parameters
			 */
			bool hasSubParameterValues( System::String^ key, System::String^ val );
		};
	}
}

#endif // __KOME_CLR_SETTING_PARAMETER_VALUES_WRAPPER_H__
