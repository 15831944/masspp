/**
 * @file SettingsParamWrapper.h
 * @brief interfaces of SettingsParamWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_PARAM_WRAPPER_H__
#define __KOME_CLR_SETTINGS_PARAM_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SettingsValueWrapper;

		/**
		 * @class SettingsParamWrapper
		 * @brief settings parameter wrapper class to use on CLR environment
		 */
		public ref class SettingsParamWrapper {
		public:
			/**
			 * @fn SettingsParamWrapper( kome::plugin::SettingsParam& param )
			 * @brief constructor
			 * @param[in] param settings param object to be wrapped
			 */
			SettingsParamWrapper( kome::plugin::SettingsParam& param );

			/**
			 * @fn virtual ~SettingsParamWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsParamWrapper();

		protected:
			/** settings parameter object to be wrapped */
			kome::plugin::SettingsParam& m_param;

		public:
			/**
			 * @fn kome::plugin::SettingsParam& getSettingsParam()
			 * @brief gets settings parameter object
			 */
			kome::plugin::SettingsParam& getSettingsParam();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets parameter name
			 * @param[in] name parameter name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets parameter name
			 * @return parameter name
			 */
			System::String^ getName();

			/** 
			 * @fn void setPrefix( System::String^ prefix )
			 * @brief sets prefix
			 * @param[in] prefix prefix
			 */
			void setPrefix( System::String^ prefix );

			/** 
			 * @fn System::String^ getPrefix()
			 * @brief gets prefix
			 * @return prefix
			 */
			System::String^ getPrefix();

			/**
			 * @fn void setValue( SettingsValueWrapper^ value )
			 * @brief sets settings value
			 * @param[in] value settings value
			 */
			void setValue( SettingsValueWrapper^ value );

			/**
			 * @fn SettingsValueWrapper^ getValue()
			 * @brief gets settings value
			 * @return settings value
			 */
			SettingsValueWrapper^ getValue();

			/**
			 * @fn void setSection( System::String^ section )
			 * @brief gets parameter section
			 * @param[in] section parameter section
			 */
			void setSection( System::String^ section );

			/**
			 * @fn System::String^ getSection()
			 * @brief gets parameter section
			 * @return parameter section
			 */
			System::String^ getSection();

			/**
			 * @fn void setKey( System::String^ key )
			 * @brief sets parameter key
			 * @param[in] key parameter key
			 */
			void setKey( System::String^ key );

			/**
			 * @fn System::String^ getKey()
			 * @brief gets parameter key
			 * @return parameter key
			 */
			System::String^ getKey();
			
		};
	}
}

#endif		// __KOME_CLR_SETTINGS_PARAM_WRAPPER_H__
