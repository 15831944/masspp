/**
 * @file SettingsValue.h
 * @brief interfaces of SettingsValue class
 *
 * @author S.Tanaka
 * @date 2007.01.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_VALUE_H__
#define __KOME_PLUGIN_SETTINGS_VALUE_H__


#include <string>
#include <vector>
#include <utility>
#include <boost/function.hpp>


#define LIST_VALUES_DELIMITER         ","
#define ELEMENT_VALUES_DELIMITER      ":"
#define RANGE_VALUES_DELIMITER        ":"


namespace kome {
	namespace plugin {

		/**
		 * @class SettingsValue 
		 * @brief settings parameter value type information class
		 */
		class PLUGIN_TOOLS_CLASS SettingsValue {
		public:
			/**
			 * @fn SettingsValue()
			 * @brief constructor
			 */
			SettingsValue();

			/**
			 * @fn virtual ~SettingsValue()
			 * @brief destructor
			 */
			virtual ~SettingsValue();

		public:
			/**
			 * @typedef enum ValueType
			 * @brief settings value type definition
			 */
			typedef enum {
				TYPE_UNKNOWN,
				TYPE_BOOL,
				TYPE_INT,
				TYPE_DOUBLE,
				TYPE_STRING,
				TYPE_PASSWD,
				TYPE_INPUT,
				TYPE_OUTPUT,
				TYPE_RAWDATA,
				TYPE_DIR,
				TYPE_COLOR,
				TYPE_FONT,
				TYPE_INT_RANGE,
				TYPE_DOUBLE_RANGE,
				TYPE_LIST,
				TYPE_EMAIL
			} ValueType;

			/**
			 * @typedef enum EnumType
			 * @brief enumeration type
			 */
			typedef enum {
				ENUM_NONE,
				ENUM_PLUGIN,
				ENUM_INI,
				ENUM_FUNCTION
			} EnumType;

		public:
			/**
			 * @struct EnumItem
			 * @brief enumeration item
			 */
			struct EnumItem {
				std::string section;
				std::string name;
				std::string value;
				std::string desc;
			};

		protected:			
			/**
			 * @struct FilterItem
			 * @brief filter item
			 */
			struct FilterItem {
				std::string name;
				std::string ext;
				std::string desc;
			};

			/** value type */
			ValueType m_type;

			/** value name */
			std::string m_name;

			/** default value */
			std::string m_default;

			/** "Hide All Checkbox" flag */
			bool m_hideAllCheckbox;

			/** enumuration array */
			std::vector< EnumItem > m_enumItems;

			/** filter array */
			std::vector< FilterItem > m_filterItems;

			/** restriction of number value */
			kome::core::NumberRestriction m_restriction;

			/** exclude set */
			std::set< std::string > m_excludeSet;

			/** parent values */
			SettingsValue* m_parent;

			/** element values */
			std::vector< SettingsValue* > m_childValues;

			/** enumeration key */
			std::string m_enumKey;

			/** enumeration type */
			EnumType m_enumType;

			/** required or not */
			bool m_required;

		public:
			/**
			 * @fn void setType( const ValueType type )
			 * @brief sets value type
			 * @param[in] type value type
			 */
			void setType( const ValueType type );

			/**
			 * @fn void setType( const char* type )
			 * @brief sets value type
			 * @param[in] type character string of value type
			 */
			void setType( const char* type );

			/**
			 * @fn void setHideAllCheckbox( bool hide )
			 * @brief sets "Hide All Checkbox" flag
			 * @param[in] hide whether to hide "All" checkbox for a checkbox list
			 */
			void setHideAllCheckbox( bool hide );

			/**
			 * @fn void setName( const char* name )
			 * @brief sets value name
			 * @param[in] name value name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets value name
			 * @return value name
			 */
			const char* getName();

			/**
			 * @fn ValueType getType()
			 * @brief gets value type
			 * @return value type
			 */
			ValueType getType();

			/**
			 * @fn bool getHideAllCheckbox()
			 * @brief gets "Hide All Checkbox" flag
			 * @return if true, do not show "All" checkbox for a checkbox list
			 */
			bool getHideAllCheckbox();

			/**
			 * @fn void setDefaultValue( const char* value )
			 * @brief sets default value
			 * @param[in] value default value
			 */
			void setDefaultValue( const char* value );

			/**
			 * @fn const char* getDefaultValue()
			 * @brief gets default value
			 * @return default value
			 */
			const char* getDefaultValue();

			/**
			 * @fn void addExcludeValue( const char* val )
			 * @brief adds exclude value
			 * @param[in] val execlude value
			 */
			void addExcludeValue( const char* val );

			/**
			 * @fn bool isExcluded( const char* val )
			 * @brief judges the specified value is excluded or not.
			 * @param[in] val character string
			 * @return If true, the specified character string is excluded.
			 */
			bool isExcluded( const char* val );

			/**
			 * @fn void setEnumKey( const char* key )
			 * @brief sets enumeration key
			 * @param[in] key enumeration key
			 */
			void setEnumKey( const char* key );

			/**
			 * @fn const char* getEnumKey()
			 * @brief gets enumeration keyi
			 * @return enumeration key
			 */
			const char* getEnumKey();

			/**
			 * @fn void setEnumType( const EnumType type )
			 * @brief sets enumeration type
			 * @param[in] type enumeration type
			 */
			void setEnumType( const EnumType type );

			/**
			 * @fn EnumType getEnumType()
			 * @brief gets enumertion type
			 * @return enumeration type
			 */
			EnumType getEnumType();

			/**
			 * @fn void clearEnumItems()
			 * @brief clears enumeration items
			 */
			void clearEnumItems();

			/**
			 * @fn void addEnumItem( const char* name, const char* value, const char* desc, const char* section = NULL )
			 * @brief adds enumeration item
			 * @param[in] name enumeration item name
			 * @param[in] value enumeration item value
			 * @param[in] desc enumeration item description
			 * @param[in] section the section name of item
			 */
			void addEnumItem( const char* name, const char* value, const char* desc, const char* section = NULL );

			/**
			 * @fn void getEnumeration( std::vector< EnumItem >& enumeration, const char* section = NULL )
			 * @brief gets the enumeration
			 * @param[out] enumeration the array object to store enumeration information
			 * @param[in] section section value
			 */
			void getEnumeration( std::vector< EnumItem >& enumeration, const char* section = NULL );

			/**
			 * @fn bool isEnumValue()
			 * @brief judges whether this value is enumeration or not.
			 * @return If true, this value is enumeration.
			 */
			bool isEnumValue();

			/**
			 * @fn void clearFilterItems()
			 * @brief clears filter items
			 */
			void clearFilterItems();

			/**
			 * @fn void addFilterItem( const char* name, const char* ext, const char* desc )
			 * @brief adds filter item
			 * @param[in] name filter item name
			 * @param[in] ext filter item extension
			 * @param[in] desc filter item description
			 */
			void addFilterItem( const char* name, const char* ext, const char* desc );

			/**
			 * @fn unsigned int getFilterSize()
			 * @brief gets number of filter items
			 * @return the number of filter items
			 */
			unsigned int getFilterSize();

			/**
			 * @fn const char* getFilterItemName( const unsigned int index )
			 * @brief gets filter item name
			 * @param[in] index filter item index
			 * @return filter item name (If index is illegal, this method returns NULL.)
			 */
			const char* getFilterItemName( const unsigned int index );

			/**
			 * @fn const char* getFilterItemExtension( const unsigned int index )
			 * @brief gets filter item extension
			 * @param[in] index filter item extension
			 * @return filter item value (If index is illegal, this method returns NULL.)
			 */
			const char* getFilterItemExtension( const unsigned int index );

			/**
			 * @fn const char* getFilterItemDesc( const unsigned int index )
			 * @brief gets filter item description
			 * @param[in] index filter item index
			 * @return filter item description (If index is illega, this method returns NULL.)
			 */
			const char* getFilterItemDesc( const unsigned int index );

			/**
			 * @fn kome::core::NumberRestriction& getNumberRestriction()
			 * @brief gets the restriction of number value
			 * @return restriction information object
			 */
			kome::core::NumberRestriction& getNumberRestriction();

		public:
			/**
			 * @fn void setParentValue( SettingsValue* parent )
			 * @brief sets the parent value
			 * @param[in] parent parent value
			 */
			void setParentValue( SettingsValue* parent );	

			/**
			 * @fn SettingsValue* getParentValue()
			 * @brief gets the parent value
			 * @return parent value
			 */
			SettingsValue* getParentValue();

			/**
			 * @fn unsigned int getNumberOfChildValues()
			 * @brief gets the number of child values
			 * @return the number of child values
			 */
			unsigned int getNumberOfChildValues();

			/**
			 * @fn void addChildValue( SettingsValue* child )
			 * @brief adds child value
			 * @param[in] child child value to be added
			 */
			void addChildValue( SettingsValue* child );

			/**
			 * @fn void clearChildValue()
			 * @brief clears child value
			 */
			void clearChildValue();

			/**
			 * @fn SettingsValue* getChildValue( const unsigned int idx )
			 * @brief gets child value
			 * @param[in] idx child index
			 * @return child value
			 */
			SettingsValue* getChildValue( const unsigned int idx );
						
			/**
			 * @fn void setRequired( const bool required )
			 * @brief sets whether this parameter is required
			 * @param[in] required If true, this parameter is required.
			 */
			void setRequired( const bool required );

			/**
			 * @fn bool isRequired()
			 * @brief judges whether this parameter is required
			 * @return If true, this parameter is required.
			 */
			bool isRequired();

			/**
			 * @fn std::string checkValue( const char* value, const char* name, const bool canEmpty = false )
			 * @brief checks value
			 * @param[in] value value
			 * @param[in] name parameter name
			 * @param[in] canEmpty If true, the value can be empty.
			 * @return error message. If empty, specified value is valid.
			 */
			std::string checkValue( const char* value, const char* name, const bool canEmpty = false );

		public:
			/**
			 * @fn static std::string convertToSavingString( const char* s )
			 * @brief convert to the character string for saving
			 * @param[in] s string
			 * @return character string for saving
			 */
			static std::string convertToSavingString( const char* s );

			/**
			 * @fn static std::string convertToDisplayingString( const char* s )
			 * @brief convert to the character string for displaying
			 * @param[in] s string
			 * @return character string for displaying
			 */
			 static std::string convertToDisplayingString( const char* s );

			 /**
			  * @fn static std::string removeOuterBracket( const char* s )
			  * @brief removes outer bracket
			  * @param[in] s character string
			  * @return character string which is removed outer bracket.
			  */
			 static std::string removeOuterBracket( const char* s );

			/**
			 * @fn static void separateListValue( const char* v, std::vector< std::string >& values )
			 * @brief separates value
			 * @param[in] v character string of values
			 * @param[out] values the array to store values
			 */
			static void separateListValue( const char* v, std::vector< std::string >& values );			
		};
	}
}

#endif		// __KOME_PLUGIN_SETTINGS_VALUE_H__
