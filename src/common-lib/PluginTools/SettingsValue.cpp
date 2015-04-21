/**
 * @file SettingsValue.cpp
 * @brief implements of SettingsValue class
 *
 * @author S.Tanaka
 * @date 2007.01.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsValue.h"

#include "PluginFunctionItem.h"
#include "PluginManager.h"
#include "PluginCall.h"
#include "SettingsPage.h"
#include "SettingsParam.h"
#include "SettingsForm.h"


#include <boost/regex.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




#define ENUMRATION_PLUGIN_TYPE      "ENUMERATION"
#define ENUM_ARRAY_PARAM_NAME       "enumeration"


// constructor
SettingsValue::SettingsValue() {
	m_type = TYPE_UNKNOWN;
	m_enumType = ENUM_NONE;
	m_hideAllCheckbox = false;
	m_required = false;
	m_parent = NULL;
}

// destrucot
SettingsValue::~SettingsValue() {	
}

// set value type
void SettingsValue::setType( const ValueType type ) {
	m_type = type;
}

// set value type
void SettingsValue::setType( const char* type ) {
	// conversion array
	static kome::core::ConversionElement< std::string, ValueType > settingsValueTypeConversionArray[] = {
		{ "bool",         TYPE_BOOL },
		{ "int",          TYPE_INT },
		{ "double",       TYPE_DOUBLE },
		{ "string",       TYPE_STRING },
		{ "passwd",       TYPE_PASSWD },
		{ "input",        TYPE_INPUT },
		{ "output",       TYPE_OUTPUT },
		{ "rawdata",      TYPE_RAWDATA },
		{ "dir",          TYPE_DIR },
		{ "color",        TYPE_COLOR },
		{ "font",         TYPE_FONT },
		{ "int-range",    TYPE_INT_RANGE },
		{ "double-range", TYPE_DOUBLE_RANGE },
		{ "list",         TYPE_LIST },
		{ "email",		  TYPE_EMAIL }
	};

	// create string object
	std::string t = tolowercase( type );

	// convert
	m_type = CONVERT_FROM_ARRAY( t, TYPE_UNKNOWN, settingsValueTypeConversionArray );
}

// get type
SettingsValue::ValueType SettingsValue::getType() {
	return m_type;
}

// set name
void SettingsValue::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get name
const char* SettingsValue::getName() {
	return m_name.c_str();
}

// set default value
void SettingsValue::setDefaultValue( const char* value ) {
	m_default = NVL( value, "" );
}

// get default value
const char* SettingsValue::getDefaultValue() {
	return m_default.c_str();
}

// set "Hide All Checkbox" flag
void SettingsValue::setHideAllCheckbox( bool hide ) {
	m_hideAllCheckbox = hide;
}

// get "Hide All Checkbox" flag
bool SettingsValue::getHideAllCheckbox() {
	return m_hideAllCheckbox;
}

// add excluded value
void SettingsValue::addExcludeValue( const char* val ) {
	m_excludeSet.insert( NVL( val, "" ) );
}

// excluded or not
bool SettingsValue::isExcluded( const char* val ) {
	return ( m_excludeSet.find( NVL( val, "" ) ) != m_excludeSet.end() );
}

// set enumeration key
void SettingsValue::setEnumKey( const char* key ) {
	m_enumKey = NVL( key, "" );
}

// get enumeration key
const char* SettingsValue::getEnumKey() {
	return m_enumKey.c_str();
}

// set enumeration type
void SettingsValue::setEnumType( const EnumType type ) {
	m_enumType = type;
}

// get enumeration type
SettingsValue::EnumType SettingsValue::getEnumType() {
	return m_enumType;
}

// set required
void SettingsValue::setRequired( const bool required ) {
	m_required = required;
}

// get required
bool SettingsValue::isRequired() {
	return m_required;
}

// clear enumeration items
void SettingsValue::clearEnumItems() {
	m_enumItems.clear();
}

// add enumeration item
void SettingsValue::addEnumItem( const char* name, const char* value, const char* desc, const char* section ) {
	// create string object
	std::string n = NVL( name, "" );
	std::string v = NVL( value, n.c_str() );
	std::string d = NVL( desc, n.c_str() );
	std::string s = NVL( section, "" );
	
	// add to the array
	m_enumItems.resize( m_enumItems.size() + 1 );
	m_enumItems.back().name = n;
	m_enumItems.back().value = v;
	m_enumItems.back().desc = d;
	m_enumItems.back().section = s;
}

// get enumeration
void SettingsValue::getEnumeration( std::vector< EnumItem >& enumeration, const char* section ) {
	// check
	if( !isEnumValue() ) {
		return;
	}

	// section
	std::string s = NVL( section, "" );

	// create the enumeration list
	if( m_enumType == ENUM_NONE ) {    // manual
		for( unsigned int i = 0; i < m_enumItems.size(); i++ ) {
			std::string name = m_enumItems[ i ].name;
			std::string val = m_enumItems[ i ].value;
			std::string desc = m_enumItems[ i ].desc;
			std::string sec = m_enumItems[ i ].section;

			if( m_excludeSet.find( val ) == m_excludeSet.end() && ( s.empty() || s.compare( sec ) == 0 ) ) {
				enumeration.resize( enumeration.size() + 1 );
				enumeration.back().name = name;
				enumeration.back().value = val;
				enumeration.back().desc = desc;
				enumeration.back().section = sec;
			}
		}
	}
	if( m_enumType == ENUM_PLUGIN ) {    // plug-in
		// plug-in manager
		PluginManager& plgMgr = PluginManager::getInstance();

		// get enumration
		std::vector< std::string > funcTypes;
		separateListValue( m_enumKey.c_str(), funcTypes );

		for( unsigned int i = 0; i < funcTypes.size(); i++ ) {
			std::string itemName = funcTypes[ i ];
			unsigned int num = plgMgr.getNumberOfFunctionItems( itemName.c_str() );
			for( unsigned int j = 0; j < num; j++ ) {
				PluginFunctionItem* item = plgMgr.getFunctionItem( itemName.c_str(), j );
				std::string name = item->getLongName();
				std::string val = item->getShortName();
				std::string desc = item->getDescription();
				if( desc.empty() ) {
					desc = name;
				}

				if( m_excludeSet.find( val ) == m_excludeSet.end() ) {
					enumeration.resize( enumeration.size() + 1 );
					enumeration.back().section = item->getCall()->getType();
					enumeration.back().name = name;
					enumeration.back().value = val;
					enumeration.back().desc = desc;
				}
			}
		}
	}
	else if( m_enumType == ENUM_INI ) {  // ini file
		// ini file
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		// section & key
		std::vector< std::string > strs;
		stringtoken( m_enumKey.c_str(), ":@", strs );

		std::string section;
		std::string key;

		if( strs.size() >= 2 ) {
			section = strs[ 0 ];
			key = strs[ 1 ];
		}
		else {
			std::string key = m_enumKey;
		}

		// get enumeration
		std::string enums;
		if( ini != NULL ) {
			enums = ini->getString( section.c_str(), key.c_str(), "" );
		}

		if( !enums.empty() ) {
			strs.clear();
			separateListValue( enums.c_str(), strs );

			for( unsigned int i = 0; i < strs.size(); i++ ) {
				std::string str = removeOuterBracket( strs[ i ].c_str() );

				if( m_excludeSet.find( str ) == m_excludeSet.end() ) {
					enumeration.resize( enumeration.size() + 1 );
					enumeration.back().name = str;
					enumeration.back().value = str;
					enumeration.back().desc = str;
				}
			}
		}
	}
	else if( m_enumType == ENUM_FUNCTION ) {    // function
		// plug-in function
		PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		PluginFunctionItem* item
			= plgMgr.getFunctionItem( ENUMRATION_PLUGIN_TYPE, m_enumKey.c_str() );

		PluginCall* call = ( item == NULL ? NULL : item->getCall() );

		if( call != NULL ) {
			std::vector< EnumItem > tmp; 
			kome::objects::Parameters params;
			kome::objects::Variant& var = params.getValue( ENUM_ARRAY_PARAM_NAME );
			var.prim.pt = &tmp;
			var.type = kome::objects::Variant::OTHER;

			call->invoke( &params );

			for( unsigned int i = 0; i < tmp.size(); i++ ) {
				if( m_excludeSet.find( tmp[ i ].value ) == m_excludeSet.end() ) {
					enumeration.push_back( tmp[ i ] );
				}
			}

			for( int i = (int)enumeration.size() - 1; i >= 0; i-- ) {
				if( !s.empty() && s.compare( enumeration[ i ].section ) != 0 ) {
					enumeration.erase( enumeration.begin() + i );
				}
			}
		}		
	}
}

// enumeration of noe
bool SettingsValue::isEnumValue() {
	if( m_enumType == ENUM_NONE ) {
		return ( m_enumItems.size() > 0 );
	}

	return !m_enumKey.empty();
}

// clear filter items
void SettingsValue::clearFilterItems() {
	m_filterItems.clear();
}

// add filter item
void SettingsValue::addFilterItem( const char* name, const char* ext, const char* desc ) {
	// check excluded
	if( m_excludeSet.find( ext ) == m_excludeSet.end() ) {
		// create string object
		std::string n = NVL( name, "" );
		std::string v = NVL( ext, n.c_str() );
		std::string d = NVL( desc, n.c_str() );

		// add to the array
		m_filterItems.resize( m_filterItems.size() + 1 );
		m_filterItems.back().name = n;
		m_filterItems.back().ext = v;
		m_filterItems.back().desc = d;
	}
}

// get the number of filter items
unsigned int SettingsValue::getFilterSize() {
	return m_filterItems.size();
}

// get filter item name
const char* SettingsValue::getFilterItemName( const unsigned int index ) {
	// check the index
	if( index >= m_filterItems.size() ) {
		return NULL;
	}

	// get item name
	return m_filterItems[ index ].name.c_str();
}

// get filter item extension
const char* SettingsValue::getFilterItemExtension( const unsigned int index ) {
	// check the index
	if( index >= m_filterItems.size() ) {
		return NULL;
	}

	// get item extension
	return m_filterItems[ index ].ext.c_str();
}

// get filter item description
const char* SettingsValue::getFilterItemDesc( const unsigned int index ) {
	// check the index
	if( index >= m_filterItems.size() ) {
		return NULL;
	}

	// get item description
	return m_filterItems[ index ].desc.c_str();
}

// get the restriction of number value
kome::core::NumberRestriction& SettingsValue::getNumberRestriction() {
	return m_restriction;
}

// set the parent value
void SettingsValue::setParentValue( SettingsValue* parent ) {
	m_parent = parent;
}

// get the parent value
SettingsValue* SettingsValue::getParentValue() {
	return m_parent;
}

// get the number of child values
unsigned int SettingsValue::getNumberOfChildValues() {
	return m_childValues.size();
}

// add child value
void SettingsValue::addChildValue( SettingsValue* child ) {
	child->setParentValue( this );
	m_childValues.push_back( child );
}

// clear child values
void SettingsValue::clearChildValue() {
	for( unsigned int i = 0; i < m_childValues.size(); i++ ) {
		SettingsValue* child = m_childValues[ i ];
		if( child->getParentValue() == this ) {
			child->setParentValue( NULL );
		}
	}

	m_childValues.clear();
}

// get child value
SettingsValue* SettingsValue::getChildValue( const unsigned int idx ) {
	if( idx >= m_childValues.size() ) {
		return NULL;
	}
	return m_childValues[ idx ];
}

// check value
std::string SettingsValue::checkValue( const char* value, const char* name, const bool canEmpty ) {
	// return value
	std::string msg;

	// parameter name
	std::string n = NVL( name, "" );
	if( n.empty() ) {
		n = "value";
	}

	// string
	std::string s = NVL( value, "" );
	if( s.empty() ) {
		if( !canEmpty ) {
			msg = FMT( "%s cannot be empty.", n.c_str() );
		}

		return msg;
	}

	// value
	std::string val = NVL( value, "" );
	std::string subVal;
	std::string::size_type pos = val.find( "[" );
	if( pos != val.npos ) {
		int level = 1;
		std::string::size_type tmpPos = pos + 1;
		while( level > 0 && tmpPos < val.npos ) {
			char c = val[ tmpPos ];
			if( c == '[' ) {
				level = level + 1;
			}
			else if( c == ']' ) {
				level = level - 1;
			}

			if( level > 0 ) {
				subVal.append( FMT( "%c", c ) );
			}
			tmpPos++;
		}

		val = val.substr( 0, pos );
	}

	// excluded
	if( isExcluded( val.c_str() ) ) {
		msg = FMT( "The value \"%s\" is not permitted.", value );
	}

	// enumeration
	if( m_enumType != ENUM_NONE ) {
		std::set< std::string > enumSet;
		std::vector< EnumItem > enumItems;
		getEnumeration( enumItems );
		for( unsigned int i = 0; i < enumItems.size(); i++ ) {
			enumSet.insert( enumItems[ i ].value );
		}

		if( enumSet.find( val.c_str() ) == enumSet.end() && val.compare( "None" ) != 0 ) { // @date 2014.07.16 <Mod> M.Izumi
			msg = FMT( "The value \"%s\" is not permitted.", value );
		}
	}

	// sub value
	if( m_enumType == ENUM_PLUGIN ) {
		// page
		PluginManager& plgMgr = PluginManager::getInstance();
		PluginFunctionItem* item = plgMgr.getFunctionItem( m_enumKey.c_str(), val.c_str() );
		SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

		if( page != NULL ) {
			// sub values
			std::vector< std::string > subVals;
			separateListValue( subVal.c_str(), subVals );
			
			// check
			for( unsigned int i = 0; i < page->getNumberOfForms() && i < subVals.size(); i++ ) {
				SettingsForm* form = page->getForm( i );
				SettingsParam* param = form->getParam();
				SettingsValue* v = param->getValue();
				
				msg = v->checkValue( subVals[ i ].c_str(), form->getTitle(), !v->isRequired() );
				
				if( !msg.empty() ) {
					msg = FMT( "%s[%s] : %s", name, item->getLongName(), msg.c_str() );
					return msg;
				}
			}
		}
	}

	// list
	if( m_type == TYPE_LIST ) {
		std::vector< std::string > lines;

		separateListValue( value, lines );

		for( unsigned int i = 0; i < lines.size() && msg.empty(); i++ ) {
			std::vector< std::string > values;

			separateListValue( lines[ i ].c_str(), values );
			while( values.size() < m_childValues.size() ) {
				values.push_back( "" );
			}

			for( unsigned int j = 0; j < m_childValues.size() && msg.empty(); j++ ) {
				std::string valueName = m_childValues[ j ]->getName();
				if( valueName.empty() ) {
					valueName = FMT( "value%d", j );
				}

				valueName.append( FMT( " in %s", n.c_str() ) );

				std::string tmpV = removeOuterBracket( values[ j ].c_str() );
				msg = m_childValues[ j ]->checkValue( tmpV.c_str(), valueName.c_str(), canEmpty );
			}
		}

		return msg;
	}

	s = removeOuterBracket( s.c_str() );
	s = convertToDisplayingString( s.c_str() );

	// single value
	if( m_type == TYPE_INT || m_type == TYPE_INT_RANGE ) {    // integer
		// elements
		std::vector< std::string > elms;
		if( m_type == kome::plugin::SettingsValue::TYPE_INT_RANGE ) {	// range value
			stringseparate( s.c_str(), RANGE_VALUES_DELIMITER, elms);
		}
		else {	// single value
			elms.push_back( s.c_str() );
		}

		// get value
		std::vector< int > nums;

		int prev = INT_MIN;

		for( unsigned int j = 0; j < elms.size(); j++ ) {
			if( !canEmpty || !elms[ j ].empty() ) {
				int intVal = 0;
				if( isint( elms[ j ].c_str(), 10, &intVal ) ) {    // valid
					if( intVal < prev ) {	// check the range
						msg = FMT(
							"Min. %s must be less than or equal to Max. %s.",
							n.c_str(),
							n.c_str()
						);
						
						return msg;
					}

					nums.push_back( intVal );
					prev = intVal;
				}
				else {	// invalid
					msg = FMT( "\"%s\" must be an integer.", n.c_str() );
					return msg;
				}
			}
		}

		for( unsigned int j = 0; j < nums.size(); j++ ) {
			if( !m_restriction.checkIntValue( nums[ j ] ) ) {
				msg = m_restriction.getRangeStr( n.c_str() );
				return msg;
			}
		}
	}
	else if( m_type == TYPE_DOUBLE || m_type == TYPE_DOUBLE_RANGE ) {	// double
		// elements
		std::vector< std::string > elms;
		if( m_type == TYPE_DOUBLE_RANGE ) {	// range value
			stringseparate( s.c_str(), RANGE_VALUES_DELIMITER,elms );
		}
		else {	// single value
			elms.push_back( s.c_str() );
		}

		// get value
		std::vector< double > nums;

		double prev = - FLT_MAX;

		for( unsigned int j = 0; j < elms.size(); j++ ) {
			if( !canEmpty || !elms[ j ].empty() ) {
				double dblVal = 0.0;
				if( isdouble( elms[ j ].c_str(), &dblVal ) ) {  // valid
					if( dblVal < prev ) {	// check the range
						msg = FMT(
							"Min. %s must be less than or equal to Max. %s.",
							n.c_str(),
							n.c_str()
						);
						return msg;
					}
					nums.push_back( dblVal );
					prev = dblVal;
				}
				else {	// invalid
					msg = FMT( "\"%s\" must be a numeric value.", n.c_str() );
					return msg;
				}
			}
		}

		// restriction
		for( unsigned int j = 0; j < nums.size(); j++ ) {
			if( !m_restriction.checkDoubleValue( nums[ j ] ) ) {
				msg = m_restriction.getRangeStr( n.c_str() );
				return msg;
			}
		}
	}
	else if( m_type == TYPE_INPUT ) {	// input file
		if( !fileexists( s.c_str() ) ) {    // unfound
			msg = FMT(
				"\"%s\" must be an existing file. The specified file [%s] does not exist.",
				n.c_str(),
				s.c_str()
			);
			
			return msg;
		}
		else if( isdirectory( s.c_str() ) ) {	// directory
			msg = FMT(
				"\"%s\" cannot be a directory. The specified path [%s] is a directory.",
				n.c_str(),
				s.c_str()
			);

			return msg;
		}
	}
	else if( m_type == TYPE_RAWDATA ) {	// raw data
		if( !fileexists( s.c_str() ) ) {    // unfound
			msg = FMT(
				"\"%s\" must be existing rawdata. The specified rawdata [%s] does not exist.",
				n.c_str(),
				s.c_str()
			);
			
			return msg;
		}
	}
	else if( m_type == TYPE_OUTPUT ) {	// output file
		if( isdirectory( s.c_str() ) ) {
			msg = FMT(
				"\"%s\" cannot be a directory. The specified path [%s] is a directory.",				
				n.c_str(),
				s.c_str()
			);

			return msg;
		}
	}
	else if( m_type == TYPE_DIR ) {	// dir
		if( !fileexists( s.c_str() ) ) {   // unfound
			msg = FMT(
				"\"%s\" must be an existing directory. The specified directory [%s] does not exist.",
				n.c_str(),
				s.c_str()
			);

			return msg;
		}
		else if( !isdirectory( s.c_str() ) ) {	// directory
			msg = FMT(
				"\"%s\" must be a directory. The specified path [%s] is not a directory.",
				n.c_str(),
				s.c_str()
			);

			return msg;
		}
	}
	// @date 2011.09.30 <Add> M.Izumi
	else if( m_type == TYPE_EMAIL ){ // e-mail
		
		const char* strEmail_REGEX = "^(([0-9a-zA-Z]((\\.(?!\\.))|[-!#\\$%&'\\*\\+/=\\?\\^`\\{\\}\\|~\\w])*)(?<=[0-9a-zA-Z])@)?((\\[(\\d{1,3}\\.){3}\\d{1,3}\\])|(([0-9a-zA-Z][-\\w]*[0-9a-zA-Z]\\.)+[a-zA-Z]{2,6}))$";
				
		boost::regex  reg_exp( strEmail_REGEX );
		boost::smatch m;
		
		if( !boost::regex_search(s, m, reg_exp) ){
			msg = FMT("%s is an invalid email address.", s.c_str());
		}				

		return msg;
	}

	return msg;
}

// convert to saving string
std::string SettingsValue::convertToSavingString( const char* s ) {
	// string object
	std::string str = trimstring( s );

	str = replacestring( str.c_str(), "\\", "\\\\" );
	str = replacestring( str.c_str(), ",", "\\," );
	str = replacestring( str.c_str(), "[", "\\[" );
	str = replacestring( str.c_str(), "]", "\\]" );
	str = replacestring( str.c_str(), ":", "\\:" );
	str = replacestring( str.c_str(), "$", "\\$" );
	str = replacestring( str.c_str(), "\"", "\\\"" );

	return str;
}

// convert to displaying string
std::string SettingsValue::convertToDisplayingString( const char* s ) {
	// string object
	std::string str = removeOuterBracket( s );

	str = replacestring( str.c_str(), "\\\\", "\\" );
	str = replacestring( str.c_str(), "\\,", "," );
	str = replacestring( str.c_str(), "\\[", "[" );
	str = replacestring( str.c_str(), "\\]", "]" );
	str = replacestring( str.c_str(), "\\:", ":" );
	str = replacestring( str.c_str(), "\\$", "$" );
	str = replacestring( str.c_str(), "\\\"", "\"" );

	return str;
}

// remove outer bracket
std::string SettingsValue::removeOuterBracket( const char* s ) {
	// string
	std::string str = trimstring( s );

	// remove
	bool flg = true;
	while( flg ) {
		flg = ( str.length() >= 2 && str[ str.length() - 1 ] == ']' );

		int depth = 0;
		for( unsigned int i = 0; i < str.length() - 1 && flg; i++ ) {
			char c = str[ i ];
			if( c == '[' ) {
				depth = depth + 1;
			}
			else if( c == ']' ) {
				depth = depth - 1;
			}

			if( depth == 0 ) {
				flg = false;
			}
		}

		if( flg ) {
			str = trimstring( str.substr( 1, str.length() - 2 ).c_str() );
			flg = false;
		}
	}

	return str;
}

// separate value
void SettingsValue::separateListValue( const char* v, std::vector< std::string >& values ) {
	// string
	std::string s = NVL( v, "" );
	if( s.empty() ) {
		return;
	}
	s.append( "," );

	// separate
	int cIdx = 0;
	int depth = 0;
	int sIdx = 0;
	bool inDoubleQuote = false;
	bool afterBackSlash = false;
	while( cIdx < (int)s.length() ) {
		char c = s[ cIdx ];

		if( afterBackSlash ) {
			afterBackSlash = false;
		}
		else if( c == '\"' && !afterBackSlash ) {
			inDoubleQuote = !inDoubleQuote;
		}
		else if( c == '\\' && !inDoubleQuote ) {
			afterBackSlash = true;
		}
		else if( !inDoubleQuote && !afterBackSlash) {
			if( c == '[' ) {
				depth = depth + 1;
			}
			else if( c == ']' ) {
				depth = depth - 1;
				if( depth < 0 ) {
					depth = 0;
				}
			}
			else if( ( c == '$' || c == ':' || c == ',' ) && depth == 0 && !inDoubleQuote ) {
				std::string val = s.substr( sIdx, cIdx - sIdx );
				val = removeOuterBracket( val.c_str() );
				values.push_back( val );
				sIdx = cIdx + 1;
			}
		}

		cIdx++;
	}
}
