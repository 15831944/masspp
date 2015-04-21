/**
 * @file SettingsFormWrapper.h
 * @brief interfaces of SettingsFormWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_FORM_WRAPPER_H__
#define __KOME_CLR_SETTINGS_FORM_WRAPPER_H__


namespace kome {
	namespace clr {


		ref class SettingsGroupWrapper;
		ref class SettingsParamWrapper;


		/**
		 * @class SettingsFormWrapper
		 * @brief settings form wrapper class to use on CLR environment
		 */
		public ref class SettingsFormWrapper {
		public:
			/**
			 * @fn SettingsFormWrapper( kome::plugin::SettingsForm& form )
			 * @brief constructor
			 * @param[in] form settings form object to be wrapped
			 */
			SettingsFormWrapper( kome::plugin::SettingsForm& form );

			/**
			 * @fn virtual ~SettingsFormWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsFormWrapper();

		protected:
			/** settings form objedt to be wrapped */
			kome::plugin::SettingsForm& m_form;

		public:
			/**
			 * @fn kome::plugin::SettingsForm& getSettingsForm()
			 * @brief gets settings form object to be wrapped
			 * @return settings form object to be wrapped
			 */
			kome::plugin::SettingsForm& getSettingsForm();

		public:
			/**
			 * @fn void setTitle( System::String^ title )
			 * @brief sets form title
			 * @param[in] title form title
			 */
			void setTitle( System::String^ title );

			/**
			 * @fn System::String^ getTitle()
			 * @brief gets form title
			 * @return form title
			 */
			System::String^ getTitle();

			/**
			 * @fn void setDesc( System::String^ desc )
			 * @brief sets form description
			 * @param[in] desc form description
			 */
			void setDesc( System::String^ desc );

			/**
			 * @fn System::String^ getDesc()
			 * @brief gets form description
			 * @return form description
			 */
			System::String^ getDesc();

			/**
			 * @fn void setImageFilePath( System::String^ path )
			 * @brief sets the image file path
			 * @param[in] path image file path
			 */
			void setImageFilePath( System::String^ path );

			/**
			 * @fn System::String^ getImageFilePath()
			 * @brief gets the image file path
			 * @return image file path
			 */
			System::String^ getImageFilePath();

			/**
			 * @fn void setGroup( SettingsGroupWrapper^ group )
			 * @brief sets settings group
			 * @param[in] group settings group
			 */
			void setGroup( SettingsGroupWrapper^ group );

			/**
			 * @fn SettingsGroupWrapper^ getGroup()
			 * @brief gets settings group
			 * @return settings group
			 */
			SettingsGroupWrapper^ getGroup();

			/**
			 * @fn void setParam( SettingsParamWrapper^ param )
			 * @brief sets settings parameter
			 * @param[in] param settings parameter
			 */
			void setParam( SettingsParamWrapper^ param );

			/**
			 * @fn SettingsParamWrapper^ getParam()
			 * @brief gets settings parameter
			 * @return settings parameter
			 */
			SettingsParamWrapper^ getParam();

			/**
			 * @fn void setDetail( bool detail )
			 * @brief sets detail flag
			 * @param detail detail flag value
			 */
			void setDetail( bool detail );

			/**
			 * @fn bool isDetail()
			 * @brief gets detail flag value
			 * @return detail flag value
			 */
			bool isDetail();
		};
	}
}

#endif		// __KOME_CLR_SETTINGS_FORM_WRAPPER_H__
