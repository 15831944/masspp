/**
 * @file SettingsPage.cpp
 * @brief implements of SettingsPage class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsPage.h"

#include "PluginManager.h"
#include "PluginFunctionItem.h"
#include "ParameterSettings.h"
#include "SettingsForm.h"
#include "SettingsParam.h"
#include "SettingsValue.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsPage::SettingsPage( ParameterSettings& settings ) {
	m_order = -1;
	m_commonFlag = false;
	m_settings = &settings;

	m_settings->addPage( this );

	m_PropertiesPageType = PROP_NONE;
}

// destructor
SettingsPage::~SettingsPage() {
	// delete object
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		delete m_forms[ i ];
	}
	m_forms.clear();

	// delete dummy functions
	for( unsigned int i = 0; i < m_checks.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_checks[ i ];
		if( item->getCall() == NULL && strcmp( item->getLongName(), "_dummy_" ) == 0 ) {
			delete item;
		}
	}
}

// get parameter settings
ParameterSettings& SettingsPage::getSettings() {
	return *m_settings;
}

// set page name
void SettingsPage::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get page name
const char* SettingsPage::getName() {
	return m_name.c_str();
}

// set page title
void SettingsPage::setTitle( const char* title ) {
	m_title = NVL( title, "" );
}

// get page title
const char* SettingsPage::getTitle() {
	return m_title.c_str();
}

// set order
void SettingsPage::setOrder( const int order ) {
	m_order = order;
}

// get order
int SettingsPage::getOrder() {
	return m_order;
}

// set common page flag
void SettingsPage::setCommon( const bool common ) {
	m_commonFlag = common;
}

// get common page flag
bool SettingsPage::isCommon() {
	return m_commonFlag;
}

// add form
SettingsForm* SettingsPage::addForm() {
	SettingsForm* param = new SettingsForm();
	m_forms.push_back( param );

	return param;
}

// get the number of forms
unsigned int SettingsPage::getNumberOfForms() {
	return m_forms.size();
}

// get form information
SettingsForm* SettingsPage::getForm( const unsigned int index ) {
	// check the index
	if( index >= m_forms.size() ) {
		return NULL;
	}

	return m_forms[ index ];
}

// set parameters
void SettingsPage::setParameters(
		kome::objects::SettingParameterValues& settings,
		const char* prefix
) {
	// managers
 	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	PluginManager& plgMgr = PluginManager::getInstance();

	// parameter set
	std::set< std::string > paramSet;
	for( unsigned int i = 0; i < settings.getNumberOfParameters(); i++ ) {
		paramSet.insert( settings.getParameterName( i ) );
	}

	// ini
	kome::core::IniFile* ini = NULL;
	if( m_settings->isIni() ) {
		ini = msppMgr.getIniFile();
		if( ini == NULL ) {
			LOG_WARN( FMT( "Failed to get INI file information." ) );
		}
	}
	else {
		ini = msppMgr.getParameters();
		if( ini == NULL ) {
			LOG_WARN( FMT( "Failed to get parameters file information." ) );
		}
	}

	// prefix
	std::string prfx = NVL( prefix, "" );

	// set parameters
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		// parameter & value
		SettingsForm* form = m_forms[ i ];
		SettingsParam* param = form->getParam();
		SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
		if( value != NULL ) {
			// key
			std::string key = param->getKey();
			if( !prfx.empty() ) {
				if( key.empty() ) {
					key = prfx;
				}
				else {
					key = FMT( "%s_%s", prfx.c_str(), key.c_str() );
				}
			}

			// value
			std::string val = value->getDefaultValue();
			if( ini != NULL ) {
				val = ini->getString( param->getSection(), key.c_str(), val.c_str() );
			}

			// sub value
			std::string subVal;
			if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
				std::string::size_type pos = val.find( "[" );
				if( pos != val.npos ) {
					subVal = val.substr( pos );
					subVal = SettingsValue::removeOuterBracket( subVal.c_str() );
					val = val.substr( 0, pos );
				}
			}
				
			if( value->isRequired() && value->isEnumValue() ) {
				// check the enumeration
				bool found = false;
				std::vector< SettingsValue::EnumItem > enumeration;
				value->getEnumeration( enumeration );
				for( unsigned int j = 0; j < enumeration.size() && !found; j++ ) {
					if( val.compare( enumeration[ j ].value.c_str() ) == 0 ) {
						found = true;
					}
				}

				if( !found && enumeration.size() > 0 ) {
					val = enumeration[ 0 ].value;
				}
			}

			if( paramSet.find( param->getName() ) == paramSet.end() ) {
				settings.setValue( param->getName(), val.c_str() );
			}

			// sub value
			if( value->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
				PluginFunctionItem* item = plgMgr.getFunctionItem( value->getEnumKey(), val.c_str() );
				SettingsPage* subPage = ( item == NULL ? NULL : item->getSettingsPage() );
				if( subPage != NULL ) {
					kome::objects::SettingParameterValues* subSettings
						= settings.getSubParameterValues( param->getName(), item->getShortName() );
					if( subVal.empty() ) {
						subPage->setParameters( *subSettings, prefix );
					}
					else {
						subPage->setParameterString( *subSettings, subVal.c_str() );
					}
				}
			}
		}
	}
}

// clear check functions
void SettingsPage::clearChecks() {
	// reconstruct
	getNumberOfChecks();

	// clear
	m_checks.clear();
}

// add check funciton
void SettingsPage::addCheck( PluginFunctionItem* fun ) {
	// check the parameter
	if( fun == NULL ) {
		return;
	}

	// check the duplication
	int idx = -1;
	for( unsigned int i = 0; i < m_checks.size() && idx < 0; i++ ) {
		if( fun == m_checks[ i ] ) {
			idx = (int)i;
		}
	}
	if( idx >= 0 ) {
		return;
	}

	// add
	m_checks.push_back( fun );
}

// get number of check functions
unsigned int SettingsPage::getNumberOfChecks() { 
	// manager
	PluginManager& mgr = PluginManager::getInstance();

	// reconstruct check functions
	std::list< PluginFunctionItem* > itemList;

	for( int i = (int)m_checks.size() - 1; i >= 0; i-- ) {
		kome::plugin::PluginFunctionItem* item = m_checks[ i ];
		if( item->getCall() == NULL && strcmp( item->getLongName(), "_dummy_" ) == 0 ) {
			std::string shortName = item->getShortName();
			delete item;
			m_checks.erase( m_checks.begin() + i );

			item = mgr.getFunctionItem( "check_params", shortName.c_str() );
			if( item != NULL ) {
				itemList.push_front( item );
			}
		}
	}

	for( std::list< PluginFunctionItem* >::iterator it = itemList.begin();
			it != itemList.end(); it++ ) {
		kome::plugin::PluginFunctionItem* item = *it;
		m_checks.push_back( item );
	}

	return m_checks.size();
}

// get check function
PluginFunctionItem* SettingsPage::getCheck( const unsigned int idx ) {
	// reconstruct
	getNumberOfChecks();

	// get function
	if( idx >= m_checks.size() ) {
		return NULL;
	}

	return m_checks[ idx ];
}

// get parameter string
std::string SettingsPage::getParameterString( kome::objects::SettingParameterValues& settings ) {
	// manager
	PluginManager& mgr = PluginManager::getInstance();

	// string
	std::string s = "[";

	// each parameters
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		if( i > 0 ) {
			s.append( "," );
		}

		SettingsForm* form = m_forms[ i ];
		SettingsParam* param = form->getParam();
		SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

		if( value != NULL ) {
			const char* tmp = settings.getParameterValue( param->getName() );

			if( value->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
				const char* type = value->getEnumKey();
				PluginFunctionItem* item = mgr.getFunctionItem( type, tmp );
				if( item != NULL ) {
					s.append( item->getShortName() );
					SettingsPage* subPage = item->getSettingsPage();
					if( subPage != NULL && settings.hasSubParameterValues( param->getName(), tmp ) ) {
						std::string subParams = subPage->getParameterString(
							*( settings.getSubParameterValues( param->getName(), tmp ) )
						);
						s.append( subParams );
					}
				}
			}
			else {
				std::string v = NVL( tmp, "" );
				if( v.find( "[" ) != v.npos || v.find( "]" ) != v.npos || v.find( "\\" ) != v.npos
						|| v.find( "," ) != v.npos || v.find( "$" ) != v.npos || v.find( ":" ) != v.npos ) {
					v = FMT( "[%s]", v.c_str() );
				}

				s.append( v );
			}
		}
	}
	s.append( "]" );

	return s;
}

// set parameter string
void SettingsPage::setParameterString( kome::objects::SettingParameterValues& settings, const char* str ) {
	// manager
	PluginManager& mgr = PluginManager::getInstance();

	// trim string
	std::string s = SettingsValue::removeOuterBracket( str );
	if( s.empty() ) {
		return;
	}
	s.append( "," );

	// index
	int sIdx = 0;
	unsigned int fIdx = 0;	// @date 2013/06/10 <Mod> OKADA	
	int start = 0;
	int depth = 0;
	bool afterBslash = false;
	bool inDoubleQuote = false;

	int len = (int)s.length();
	while( sIdx < len && fIdx < (int)m_forms.size() ) {
		char c = s[ sIdx ];

		if( afterBslash ) {
			afterBslash = false;
		}
		else {
			if( c == '[' ) {
				depth = depth + 1;
			}
			else if( c == ']' ) {
				depth = depth - 1;
				if( depth < 0 ) {
					depth = 0;
				}
			}
			else if( c == '\"' ) {
				inDoubleQuote = !inDoubleQuote;
			}
			else if( c == '\\' ) {
				afterBslash = true;
			}
			else if( depth == 0 && !inDoubleQuote && c == ',' ) {
				std::string v;
				if( sIdx > start ) {
					v = s.substr( start, sIdx - start );
				}
				start = sIdx + 1;

				SettingsParam* param;
				if((int)m_forms.size() <= fIdx){
					param = NULL;
				}else{
					param = m_forms[ fIdx ]->getParam();
				}

				if( param != NULL ) {
					SettingsValue* val = param->getValue();
					if( val != NULL && val->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
						int idx = -1;
						for( unsigned int i = 0; i < v.length() && idx < 0; i++ ) {
							if( v[ i ] == '\\' ) {
								idx = -2;
							}
							else {
								if( idx == -1 && v[ i ] == '[') {
									idx = (int)i;
								}
								else {
									idx = -1;
								}
							}
						}

						if( idx >= 0 ) {
							std::string subV = v.substr( idx );
							v = v.substr( 0, idx );
							PluginFunctionItem* item = mgr.getFunctionItem( val->getEnumKey(), v.c_str() );
							if( item != NULL ) {
								SettingsPage* subPage = item->getSettingsPage();
								if( subPage != NULL ) {
									kome::objects::SettingParameterValues* subSettings = settings.getSubParameterValues( param->getName(), v.c_str() );
									subPage->setParameterString( *subSettings, subV.c_str() );
								}
							}
						}
					}
					v = SettingsValue::convertToDisplayingString( v.c_str() );
					settings.setValue( param->getName(), v.c_str() );
				}

				fIdx++;
				start = sIdx + 1;
			}
		}
		sIdx++;
	}
}

// get properties page type
PropertiesPageType SettingsPage::getPropertiesPageType()	// <properties-page>É^ÉO
{
	return m_PropertiesPageType;
}

// set properties page type
void SettingsPage::setPropertiesPageType( PropertiesPageType type )
{
	m_PropertiesPageType = type;
}
