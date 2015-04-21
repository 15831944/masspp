/**
 * @file SettingsValueWrapper.h
 * @brief interfaces of SettingsValueWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_VALUE_WRAPPER_H__
#define __KOME_CLR_SETTINGS_VALUE_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginCallWrapper;

		/**
		 * @class SettingsValueWrapper
		 * @brief settings value wrapper class to use on CLR environment
		 */
		public ref class SettingsValueWrapper {
		public:
			/**
			 * @fn SettingsValueWrapper( kome::plugin::SettingsValue& value )
			 * @brief constructor
			 * @param[in] value settings value object to be wrapped
			 */
			SettingsValueWrapper( kome::plugin::SettingsValue& value );

			/**
			 * @fn virtual ~SettingsValueWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsValueWrapper();

		protected:
			/** settings value object to be wrapped */
			kome::plugin::SettingsValue& m_value;

		public:
			/**
			 * @fn kome::plugin::SettingsValue& getSettingsValue()
			 * @brief gets settings value object
			 * @return settings value object
			 */
			kome::plugin::SettingsValue& getSettingsValue();

		public:
			/** value type */
			ENUM_CLASS ValueType {
				UNKNOWN,
				BOOL,
				INT,
				DOUBLE,
				STRING,
				PASSWD,
				INPUT,
				OUTPUT,
				RAWDATA,
				DIR,
				COLOR,
				FONT,
				INT_RANGE,
				DOUBLE_RANGE,
                LIST,
				EMAIL
			};

			/** enumeration type */
			ENUM_CLASS EnumType {
				NONE,
				PLUGIN,
				INI,
				FUNCTION
			};

			/**
			 * @class EnumItem
			 * @brief enumeration item
			 */
			ref class EnumItem {
			public:
				System::String^ name;
				System::String^ value;
				System::String^ description;
			};

		public:
			/**
			 * @fn void setType( ValueType type )
			 * @brief sets value type
			 * @param[in] type value type
			 */
			void setType( ValueType type );

			/**
			 * @fn void setType( System::String^ type )
			 * @brief sets value type
			 * @param[in] type character string of value type
			 */
			void setType( System::String^ type );

			/**
			 * @fn ValueType getType()
			 * @brief gets value type
			 * @return value type
			 */
			ValueType getType();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets value name
			 * @param[in] name value name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets value name
			 * @return value name
			 */
			System::String^ getName();

			/**
			 * @fn void setDefaultValue( System::String^ value )
			 * @brief sets default value
			 * @param[in] value default value
			 */
			void setDefaultValue( System::String^ value );

			/**
			 * @fn System::String^ getDefaultValue()
			 * @brief gets default value
			 * @return default value
			 */
			System::String^ getDefaultValue();

			/**
			 * @fn void setEnumKey( System::String^ key )
			 * @brief sets enumeration key
			 * @param[in] key enumeration key
			 */
			void setEnumKey( System::String^ key );

			/**
			 * @fn System::String^ getEnumKey()
			 * @brief gets enumeration key
			 * @return enumeration key
			 */
			System::String^ getEnumKey();

			/**
			 * @fn vid setEnumType( EnumType type )
			 * @brief sets enumeration type
			 * @param[in] type enumeration type
			 */
			void setEnumType( EnumType type );

			/**
			 * @fn EnumType getEnumType()
			 * @brief gets enumeration type
			 * @return enumeration type
			 */
			EnumType getEnumType();

			/**
			 * @fn void addExcludeValue( System::String^ val )
			 * @brief adds exclude value
			 * @param[in] val execlude value
			 */
			void addExcludeValue( System::String^ val );

			/**
			 * @fn bool isExcluded( System::String^ val )
			 * @brief judges the specified value is excluded or not.
			 * @param[in] val character string
			 * @return If true, the specified character string is excluded.
			 */
			bool isExcluded( System::String^ val );

			/**
			 * @fn void clearEnumItems()
			 * @brief clears enumeration items
			 */
			void clearEnumItems();

			/**
			 * @fn void addEnumItem( System::String^ name, System::String^ value, System::String^ desc )
			 * @brief add to enumeration item
			 * @param[in] name enumeration item name
			 * @param[in] value enumeration item value
			 * @param[in] desc enumeration item description
			 */
			void addEnumItem( System::String^ name, System::String^ value, System::String^ desc );

			/**
			 * @fn cli::array< EnumItem >^ getEnumeration()
			 * @brief gets the enumeration
			 * @return enumeration
			 */
			cli::array< EnumItem^ >^ getEnumeration();

			/**
			 * @fn bool isEnumValue()
			 * @brief judges whether this value is enumeration or not.
			 * @return If true, this value is enumeration.
			 */
			bool isEnumValue();

			/**
			 * @fn kome::core::NumberRestrictionWrapper^ getNumberRestriction()
			 * @brief gets the restriction of number value
			 * @return restriction information object
			 */
			NumberRestrictionWrapper^ getNumberRestriction();

			/**
			 * @fn unsigned int getNumberofChildValues()
			 * @brief gets the number of child values
			 * @return the number of child values
			 */
			unsigned int getNumberOfChildValues();
			
			/**
			 * @fn void setParentValue( SettingsValueWrapper^ parent )
			 * @brief sets the parent value
			 * @param[in] parent parent value
			 */
			void setParentValue( SettingsValueWrapper^ parent );

			/**
			 * @fn SettingsValueWrapper^ getParentValue()
			 * @brief gets the parent value
			 * @return parent value
			 */
			SettingsValueWrapper^ getParentValue();

			/**
			 * @fn void addChildValue( SettingsValueWrapper^ child )
			 * @briefi adds child value
			 * @param[in] child child value to be added
			 */
			void addChildValue( SettingsValueWrapper^ child );

			/**
			 * @fn void clearChildValue()
			 * @brief clears child value
			 */
			void clearChildValue();

			/**
			 * @fn SettingsValueWrapper^ getChildValue( unsigned int idx )
			 * @brief gets child value
			 * @param[in] idx child index
			 * @return child value
			 */
			SettingsValueWrapper^ getChildValue( unsigned int idx );

			/**
			 * @fn void setRequired( bool required )
			 * @brief sets whether this parameter is required
			 * @param[in] required If true, this parameter is required.
			 */
			void setRequired( bool required );

			/**
			 * @fn bool isRequired()
			 * @brief judges whether this parameter is required
			 * @return If true, this parameter is required.
			 */
			bool isRequired();

		public:
			/**
			 * @fn System::String^ checkValue( System::String^ value, System::String^ name, bool canEmpty )
			 * @brief checks value
			 * @param[in] value value
			 * @param[in] name parameter name
			 * @param[in] canEmpty If true, the value can be empty.
			 * @return error message. If empty, specified value is valid.
			 */
			System::String^ checkValue( System::String^ value, System::String^ name, bool canEmpty );

		public:
			/**
			 * @fn static System::String^ convertToSavingString( System::String^ s )
			 * @brief convert to the character string for saving
			 * @param[in] s string
			 * @return character string for saving
			 */
			static System::String^ convertToSavingString( System::String^ s );

			/**
			 * @fn static System::String^ convertToDisplayingString( System::String^ s )
			 * @brief convert to the character string for displaying
			 * @param[in] s string
			 * @return character string for displaying
			 */
			 static System::String^ convertToDisplayingString( System::String^ s );

			 /**
			  * @fn static System::String^ removeOuterBracket( System::String^ s )
			  * @brief removes outer bracket
			  * @param[in] s character string
			  * @return character string which is removed outer bracket.
			  */
			 static System::String^ removeOuterBracket( System::String^ s );

			/**
			 * @fn static cli::array< System::String^ >^ separateListValue( System::String^ v )
			 * @brief separates value
			 * @param[in] v character string of values
			 * @return the array to store values
			 */
			static cli::array< System::String^ >^ separateListValue( System::String^ v );
		};
	}
}

#endif		// __KOME_CLR_SETTINGS_VALUE_WRAPPER_H__
