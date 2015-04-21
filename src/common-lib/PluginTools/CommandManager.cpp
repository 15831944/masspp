/**
 * @file CommandManager.cpp
 * @brief implements of CommandManager class
 *
 * @author S.Tanaka
 * @date 2007.07.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CommandManager.h"

#include "PluginManager.h"
#include "PluginCall.h"
#include "PluginInfo.h"
#include "PluginFunctionItem.h"
#include "SettingsPage.h"
#include "SettingsForm.h"
#include "SettingsParam.h"
#include "SettingsValue.h"
#include "PluginCallTool.h"

#include <algorithm>
#include <map>
#include <boost/bind.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define HELP_OPTION			  "--help"
#define PARAMS_OPTION		  "--parameters"
#define PLUGINPATH_OPTION    "--pluginpath"

#define	RESTART_OPTION			"/restart="		// @Date:2013/10/03	<Add>	A.Ozaki

// constructor
CommandManager::CommandManager() {
}

// destructor
CommandManager::~CommandManager() {
}

// create array
void CommandManager::init( const char* type ) {
	// plug-in manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// clear array
	m_commandOptions.clear();

	// getting command functions
	unsigned int num = plgMgr.getNumberOfFunctionItems( type );
	for( unsigned int i = 0; i < num; i++ ) {
		PluginFunctionItem* item = plgMgr.getFunctionItem( type, i );
		addPluginFunction( item );
	}

	// check
	checkDuplicated();

	// sort
	if( m_commandOptions.size() > 0 ) {
		std::sort(
			m_commandOptions.begin(),
			m_commandOptions.end(),
			lessOption
		);
	}
}

// check array
void CommandManager::checkDuplicated() {
	// array -> map
	std::map< std::string, std::vector< OptionInfo > > optMap;
	for( unsigned int i = 0; i < m_commandOptions.size(); i++ ) {
		// option
		OptionInfo& opt = m_commandOptions[ i ];

		PluginFunctionItem* item = opt.item;
		if( item != NULL ) {
			std::string name = item->getShortName();
			std::vector< OptionInfo >& arr = optMap[ name ];
			arr.resize( arr.size() + 1 );

			OptionInfo& o = arr.back();
			o.item = item;
			o.step = opt.step;
		}
	}

	// clear array
	m_commandOptions.clear();

	// check duplicated
	for( std::map< std::string, std::vector< OptionInfo > >::iterator it0 = optMap.begin();
			it0 != optMap.end(); it0++ ) {
		// check duplicated option name
		std::vector< OptionInfo >& values = (*it0).second;

		if( values.size() > 1 ) {	// duplicated
			std::string msg = FMT( "The option \"-%s\" is duplicated.", (*it0).first.c_str() );

			for( unsigned int i = 0; i < values.size(); i++ ) {
				PluginFunctionItem* item = values[ i ].item;

				msg += FMT(
					" [plugin=%s, function=%s]",
					item->getCall()->getPlugin()->getName(),
					item->getCall()->getName()
				);
			}

			LOG_WARN( msg );
		}
		else {   // add to array
			m_commandOptions.resize( m_commandOptions.size() + 1 );

			OptionInfo& opt = m_commandOptions.back();

			opt.item = values[ 0 ].item;
			opt.step = values[ 0 ].step;
		}
	}
}

// judges empty
bool CommandManager::empty() {
	return ( m_execFunctions.size() == 0 );
}

// show usage
const char* CommandManager::getUsage() {
	// check the member
	if( !m_usage.empty() ) {
		return m_usage.c_str();
	}

	// get module file name
	std::string prog = getfilename( getmodulefile().c_str() );

	// clear
	m_usage.clear();
	m_optionParams.clear();

	// print help
	m_usage.append( FMT( "Usage: %s [Option...]%s", prog.c_str(), NEW_LINE ) );
	m_usage.append( NEW_LINE );

	// options
	m_usage.append( FMT( "Options:%s", NEW_LINE ) );
	m_usage.append( NEW_LINE );
	m_usage.append( FMT( "    %s%s", HELP_OPTION, NEW_LINE ) );
	m_usage.append( FMT( "        Display this help list%s", NEW_LINE ) );

	m_usage.append( NEW_LINE );
	m_usage.append( FMT( "    %s val%s", PARAMS_OPTION, NEW_LINE ) );
	m_usage.append( FMT( "        Display the parameters information about the specified value%s", NEW_LINE ) );
	m_usage.append( FMT( "            val parameter value%s", NEW_LINE ) );

	m_usage.append( NEW_LINE );
	m_usage.append( FMT( "    %s paths%s", PLUGINPATH_OPTION, NEW_LINE ) );
	m_usage.append( FMT( "        Add plug-in path.%s", NEW_LINE ) );
	m_usage.append( FMT( "            paths plug-in directories( %s separated)%s", PATH_SEPARATOR, NEW_LINE ) );

	for( unsigned int i = 0; i < m_commandOptions.size(); i++ ) {
		// option
		OptionInfo& o = m_commandOptions[ i ];

		// blank
		m_usage.append( NEW_LINE );

		// show option information
		PluginFunctionItem* item = o.item;
		SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

		m_usage.append( FMT( "    -%s", item->getShortName() ) );
		if( page != NULL ) {
			for( unsigned int j = 0; j < page->getNumberOfForms(); j++ ) {
				m_usage.append( FMT( "        %s", page->getForm( j )->getParam()->getName() ) );
			}
		}
		m_usage.append( NEW_LINE );

		if( strlen( item->getCall()->getDescription() ) > 0 ) {
			m_usage.append(
				FMT(
					"            %s%s",
					item->getCall()->getDescription(),
					NEW_LINE
				)
			);
		}

		// show argument information
		if( page != NULL ) {
			for( unsigned j = 0; j < page->getNumberOfForms(); j++ ) {
				showArgInfo( page->getForm( j ), 12 );
				m_usage.append( NEW_LINE );
			}
		}
	}

	return m_usage.c_str();
}

// show option argument information
void CommandManager::showArgInfo( SettingsForm* form, unsigned int space ) {
	// manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// value
	kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
	SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// space
	for( unsigned int i = 0; i < space; i++ ) {
		m_usage.append( " " );
	}

	// set parameters
	ParameterInfo* optionParam = NULL;
	std::string itemType;
	if( value->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
		itemType = value->getEnumKey();
	}

	bool paramsFlg = false;

	if( !itemType.empty() ) {
		for( unsigned int i = 0; i < m_optionParams.size() && optionParam == NULL; i++ ) {
			if( m_optionParams[ i ].type.compare( itemType ) == 0 ) {
				optionParam = &( m_optionParams[ i ] );
			}
		}

		if( optionParam == NULL ) {
			m_optionParams.resize( m_optionParams.size() + 1 );
			optionParam = &( m_optionParams.back() );

			optionParam->type = itemType;
			optionParam->title = form->getTitle();

			paramsFlg = true;
		}
	}

	if( paramsFlg ) {
		unsigned int num = plgMgr.getNumberOfFunctionItems( optionParam->type.c_str() );
		for( unsigned int i = 0; i < num; i++ ) {
			PluginFunctionItem* item = plgMgr.getFunctionItem( optionParam->type.c_str(), i );
			SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

			if( page != NULL && page->getNumberOfForms() > 0 ) {
				optionParam->items.push_back( item );
			}
		}
	}

	// print argument name
	m_usage.append( FMT( "%s", param->getName() ) );

	if( optionParam != NULL && optionParam->items.size() > 0 ) {
		m_usage.append( "[parameters]" );
	}
	m_usage.append( FMT( "	" ) );

	// print description
	if( strlen( form->getDesc() ) > 0 ) {
		m_usage.append( FMT( "%s ", form->getDesc() ) );
	}

	// print type
	m_usage.append( "{" );
	if( value->getType() == SettingsValue::TYPE_INT
				|| value->getType() == SettingsValue::TYPE_DOUBLE
				|| value->getType() == SettingsValue::TYPE_INT_RANGE
				|| value->getType() == SettingsValue::TYPE_DOUBLE_RANGE ) {	// number
		// print type
		if( value->getType() == SettingsValue::TYPE_INT
				|| value->getType() == SettingsValue::TYPE_INT_RANGE ) {
			m_usage.append( "integer " );
		}
		else{
			m_usage.append( "real number " );
		}

		if( value->getType() == SettingsValue::TYPE_INT_RANGE
				|| value->getType() == SettingsValue::TYPE_DOUBLE_RANGE ) {
			m_usage.append( "range" );
		}
		else {
			m_usage.append( "value" );
		}

		// range
		std::string rangeStr = value->getNumberRestriction().getRangeStr( form->getParam()->getName() );
		if( !rangeStr.empty() ) {
			m_usage.append( FMT( " %s", rangeStr.c_str() ) );
		}
	}
	else if( value->getType() == SettingsValue::TYPE_BOOL ) {	// bool
		m_usage.append( "bool" );
	}
	else if( value->getType() == SettingsValue::TYPE_INPUT ) {	// input
		if( value->getFilterSize() > 0 ) {	// filters
			for( unsigned int i = 0; i < value->getFilterSize(); i++ ) {
				if( i > 0 ) {
					m_usage.append( "/" );
				}
				m_usage.append( FMT( "%s", value->getFilterItemExtension( i ) ) );
			}
		}
		else {		// any input
			m_usage.append( "input" );
		}
	}
	else if( value->getType() == SettingsValue::TYPE_OUTPUT ) {	// output
		m_usage.append( "output" );
	}
	else if( value->getType() == SettingsValue::TYPE_DIR ) {	// dir
		m_usage.append( "dir" );
	}
	else if( value->getType() == SettingsValue::TYPE_COLOR ) {	// color
		m_usage.append( "color" );
	}
	else {	// string
		if( value->isEnumValue() ) {    // enumration
			std::vector< kome::plugin::SettingsValue::EnumItem > enumeration;
			value->getEnumeration( enumeration );
			for( unsigned int i = 0; i < enumeration.size(); i++ ) {
				if( i > 0 ) {
					m_usage.append( "/" );
				}
				m_usage.append( enumeration[ i ].value );
			}
		}
		else {		// any string
			m_usage.append( "string" );
		}
	}
	m_usage.append( "}" );

	// enumeration description
	space = space + 4;
	if( value->isEnumValue() ) {    // enumeration
		std::vector< kome::plugin::SettingsValue::EnumItem > enumeration;
		value->getEnumeration( enumeration );

		for( unsigned int i = 0; i < enumeration.size(); i++ ) {
			m_usage.append( FMT( "%s", NEW_LINE ) );
			for( unsigned int j = 0; j < space; j++ ) {
				m_usage.append( " " );
			}
			m_usage.append(
				FMT(
					"%s - %s",
					enumeration[ i ].value.c_str(),
					enumeration[ i ].desc.c_str()
				) 
			);
		}
	}

	// filters description
	space = space + 4;
	if( value->getFilterSize() > 0 ) {	// filters
		for( unsigned int i = 0; i < value->getFilterSize(); i++ ) {
			m_usage.append( FMT( "%s", NEW_LINE ) );
			for( unsigned int j = 0; j < space; j++ ) {
				m_usage.append( " " );
			}
			m_usage.append(
				FMT(
					"%s - %s",
					value->getFilterItemExtension( i ),
					value->getFilterItemDesc( i )
				) 
			);
		}
	}
}

// get parameter usage
std::string CommandManager::getParameterUsage( const char* value ) {
	// return value
	std::string paramUsage;

	// get usage
	getUsage();

	m_usage.append( FMT( "%s%s", NEW_LINE, NEW_LINE ) );
	m_usage.append( FMT( "Parameters:%s", NEW_LINE ) );

	for( unsigned int i = 0; i < m_optionParams.size(); i++ ) {
		if( m_optionParams[ i ].items.size() > 0 ) {
			// blank
			m_usage.append( NEW_LINE );

			// title
			m_usage.append( FMT( "	%s\n", m_optionParams[ i ].title.c_str() ) );

			for( unsigned int j = 0; j < m_optionParams[ i ].items.size(); j++ ) {
				PluginFunctionItem* item = m_optionParams[ i ].items.at( j );
				SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

				m_usage.append( FMT( "		%s[", ( item->getShortName() ) ) );
				for( unsigned int k = 0; k < page->getNumberOfForms(); k++ ) {
					SettingsForm* form = page->getForm( k );
					SettingsParam* param = form->getParam();

					if( k > 0 ) {
						m_usage.append( "," );
					}

					m_usage.append( param == NULL ? "" : param->getName() );
				}
				m_usage.append( FMT( "]%s", NEW_LINE ) );
				m_usage.append( FMT( "			%s%s", item->getLongName(), NEW_LINE ) );

				// parameters
				for( unsigned int k = 0; k < page->getNumberOfForms(); k++ ) {
					showArgInfo( page->getForm( k ), 16 );
					m_usage.append( NEW_LINE );
				}
			}
		}
	}

	// parameters usage
	int cnt = 0;
	m_usage.clear();

	paramUsage = FMT( "Parameters:%s", NEW_LINE );

	for( unsigned int i = 0; i < m_optionParams.size(); i++ ) {
		std::string title = m_optionParams[ i ].title;

		PluginFunctionItem* item = NULL;

		for( unsigned int j = 0; j < m_optionParams[ i ].items.size() && item == NULL; j++ ) {
			if( value == NULL || strlen( value ) == 0
					|| strcmp( m_optionParams[ i ].items.at( j )->getShortName(), value ) == 0 ) {
				item = m_optionParams[ i ].items.at( j );
			}
		}

		if( item != NULL ) {
			SettingsPage* page = item->getSettingsPage();

			paramUsage.append( NEW_LINE );
			paramUsage.append( FMT( "	%s%s", title.c_str(), NEW_LINE ) );
			paramUsage.append( FMT( "		%s[", item->getShortName() ) );

			for( unsigned int j = 0; j < page->getNumberOfForms(); j++ ) {
				SettingsForm* form = page->getForm( j );
				SettingsParam* param = form->getParam();

				if( j > 0 ) {
					paramUsage.append( "," );
				}

				paramUsage.append( param == NULL ? "" : param->getName() );
			}

			paramUsage.append( FMT( "]%s", NEW_LINE ) );
			paramUsage.append( FMT( "			%s%s", item->getLongName(), NEW_LINE ) );

			// parameters
			for( unsigned int j = 0; j < page->getNumberOfForms(); j++ ) {
				m_usage.clear();
				showArgInfo( page->getForm( j ), 16 );
				paramUsage.append( m_usage );
				paramUsage.append( NEW_LINE );
			}

			cnt++;
		}
	}

	if( cnt == 0 ) {
		paramUsage = FMT( "The specified value does not have any parameters. [%s]", value );
	}

	return paramUsage;
}

// parse option
bool CommandManager::parseOption( int argc, char** argv ) {
	// return value
	bool result = true;

	// check common options
	bool pathFlg = false;
	std::set< int > skipSet;
	for( int i = 1; i < argc; i++ ) {
		char* arg = argv[ i ];
		if( strcmp( arg, HELP_OPTION ) == 0 ) {
			return true;
		}
// >>>>>>	@Date:2013/10/03	<Add>	A.Ozaki
//
		else if( strncmp( arg, RESTART_OPTION, strlen( RESTART_OPTION ) ) == 0 ) {
			return true;
		}
//
// <<<<<<	@Date:2013/10/03	<Add>	A.Ozaki
		else if( strcmp( arg, PARAMS_OPTION ) == 0 ) {
			if( i < argc - 1 ) {
				m_usage = getParameterUsage( argv[ i + 1 ] );
				return true;
			}
			else {
				LOG_ERROR_CODE( FMT( "%s needs a value argument.", PARAMS_OPTION ), ERR_OTHER );
				return false;
			}
		}
		else if( strcmp( arg, PLUGINPATH_OPTION ) == 0 ) {
			if( i < argc - 1 ) {
				pathFlg = true;
				skipSet.insert( i );
			}
			else {
				LOG_ERROR_CODE( FMT( "%s needs plug-in paths argument.", PLUGINPATH_OPTION ), ERR_OTHER );
				return false;
			}
		}
		else if( pathFlg ) {
			std::vector< std::string > paths;
			stringtoken( arg, PATH_SEPARATOR, paths );
			for( unsigned int j = 0; j < paths.size(); j++ ) {
				m_pluginPaths.push_back( paths[ j ] );
			}
			pathFlg = false;
			skipSet.insert( i );
		}
	}

	// parse
	for( int i = 1; i < argc && result; i++ ) {
		int last = i;

		// get function call
		char* arg = argv[ i ];
		OptionInfo* o = NULL;
		if( skipSet.find( i ) == skipSet.end() ) {
			o = getOptionInfo( arg );
			if( o == NULL ) {   // invalid option
				LOG_ERROR_CODE( FMT( "The option \"%s\" is not supported.", arg ), ERR_OTHER );
				result = false;
			}
		}

		// parse arguments
		if( o != NULL ) {
			// add function information to the array
			m_execFunctions.resize( m_execFunctions.size() + 1 );
			m_execFunctions.back().first.item = o->item;
			m_execFunctions.back().first.step = o->step;

			// get parameters
			SettingsPage* page = ( o->item == NULL ? NULL : o->item->getSettingsPage() );
			kome::objects::SettingParameterValues& settings = m_execFunctions.back().second;
			if( page != NULL ) {
				for( unsigned int j = 0; j < page->getNumberOfForms() && result; j++ ) {
					// get character string of parameter
					last = i + j + 1;
					char* p = NULL;
					if( last < argc ) {
						p = argv[ last ];
					}
					std::string paramVal = NVL( p, "" );

					// add parameter
					SettingsForm* form = page->getForm( j );
					SettingsParam* param = form->getParam();
					SettingsValue* val = param->getValue();

					// item name
					std::string itemName;
					if( val->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
						itemName = val->getEnumKey();
					}

					// sub parameters
					if( !itemName.empty() && !paramVal.empty() ) {
						paramVal = getSubParameters( paramVal.c_str(), settings, itemName.c_str(), param->getName() );
					}

					std::string msg = val->checkValue( paramVal.c_str(), param->getName(), !( val->isRequired() ) );
					if( msg.empty() ) {	// valid
						settings.setValue( param->getName(), paramVal.c_str() );
					}
					else {	// invalid value
						msg = FMT(
							"Option Error: (-%s) %s=%s   %s",
							o->item->getShortName(),
							param->getName(),
							NVL( p, "" ),
							msg.c_str()
						);

						LOG_ERROR_CODE( msg, ERR_OTHER );
						result = false;
					}
				}
			}
		}

		i = last;
	}

	// check result
	if( !result ) {
		m_execFunctions.clear();
	}

	return result;
}

// execute
bool CommandManager::execute() {
	// @date 2013/08/02 <ADD> OKADA
	// steps
	OptionStep steps[] = {
		STEP_INITIALIZE,
		STEP_INPUT,
		STEP_DATA_SET,
		STEP_OPERATION,
		STEP_FILTER,	// * priority changed
		STEP_MANIPULATE,	// add
		STEP_ANALYZE,
		STEP_IDENTIFY,
		STEP_RESULTS,	// add
		STEP_OUTPUT,
		STEP_FINALIZE,
		STEP_OTHER
	};


	unsigned int size = sizeof( steps ) / sizeof( OptionStep );

	// execute
	for( unsigned int i = 0; i < size; i++ ) {
		// step
		OptionStep step = steps[ i ];

		for( unsigned int j = 0; j < m_execFunctions.size(); j++ ) {
			// invoke
			if( m_execFunctions[ j ].first.step == step ) {
				// get function and parameter
				PluginFunctionItem* item = m_execFunctions[ j ].first.item;
				kome::objects::SettingParameterValues& settings = m_execFunctions[ j ].second;

				// execute
				try{
					kome::objects::Parameters params;
					PluginCallTool::setSettingValues( params, settings );

					kome::objects::Variant ret = item->getCall()->invoke( &params );
					if( !ret.prim.boolVal ) {
							return false;
					}
				}
				catch( ... ) {
					return false;
				}
			}
		}
	}

	return true;
}

// add plug-in function
void CommandManager::addPluginFunction( PluginFunctionItem* item ) {
	// check the parameter
	if( item == NULL ) {
		return;
	}

	// check option name
	if( strlen( NVL( item->getShortName(), "" ) ) == 0 ) {   // invalid name
		LOG_WARN(
			FMT(
				"There is not the option name. [plugin=%s, function=%s]",
				item->getCall()->getPlugin()->getName(),
				item->getCall()->getName()
			)
		);
	}
	else {  // add to the array
		m_commandOptions.resize( m_commandOptions.size() + 1 );

		m_commandOptions.back().item = item;

		const char* stepStr = item->getCall()->getProperties().getStringValue( "step", "" );
		OptionStep step = STEP_OTHER;
		if( compareignorecase( stepStr, "initialize" ) == 0 ) {
			step = STEP_INITIALIZE;
		}
		else if( compareignorecase( stepStr, "input" ) == 0 ) {
			step = STEP_INPUT;
		}
		else if( compareignorecase( stepStr, "data_set" ) == 0 ) {
			step = STEP_DATA_SET;
		}
		else if( compareignorecase( stepStr, "operation" ) == 0 ) {
			step = STEP_OPERATION;
		}
		else if( compareignorecase( stepStr, "analyze" ) == 0 ) {
			step = STEP_ANALYZE;
		}
		else if( compareignorecase( stepStr, "identify" ) == 0 ) {
			step = STEP_IDENTIFY;
		}
		else if( compareignorecase( stepStr, "filter" ) == 0 ) {
			step = STEP_FILTER;
		}
		else if( compareignorecase( stepStr, "output" ) == 0 ) {
			step = STEP_OUTPUT;
		}
		else if( compareignorecase( stepStr, "finalize" ) == 0 ) {
			step = STEP_FINALIZE;
		}
		// @date 2013/08/02 <Add> OKADA ------->
		else if( compareignorecase( stepStr, "manipulate" ) == 0 ) {
			step = STEP_MANIPULATE;
		}
		else if( compareignorecase( stepStr, "results" ) == 0 ) {
			step = STEP_RESULTS;
		}
		// @date 2013/08/02 <Add> OKADA <-------

		m_commandOptions.back().step = step;

	}
}

// get option information
CommandManager::OptionInfo* CommandManager::getOptionInfo( char* arg ) {
	// check parameter
	if( arg == NULL ) {
		return NULL;
	}
	if( strlen( arg ) < 2 || *arg != '-' ) {
		LOG_ERROR( FMT( "Argument \"%s\" is not supported.", arg ) );
		return NULL;
	}
	arg++;

	// get option information
	OptionInfo* oi = NULL;
	bool optionFound = false;

	for( unsigned int i = 0; i < m_commandOptions.size() && oi == NULL; i++ ) {
		OptionInfo& o = m_commandOptions[ i ];

		// compare option name
		if( strcmp( o.item->getShortName(), arg ) == 0 ) {
			optionFound = true;
			oi = &o;
		}
	}

	// log
	if( oi == NULL ) {
		LOG_ERROR( FMT( "Option \"-%s\" is not supported.", arg ) );
	}

	return oi;
}

// get sub parameters
std::string CommandManager::getSubParameters(
		const char* arg,
		kome::objects::SettingParameterValues& settings,
		const char* itemName,
		const char* paramName
) {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// return value
	std::string ret;

	// check parameters
	std::string str = NVL( arg, "" );
	if( str.empty() || itemName == NULL || strlen( itemName ) == 0 ) {
		return ret;
	}

	// find '['
	size_t pos = str.find( '[' );
	if( pos == str.npos ) {
		return str;
	}

	// actual value
	ret = str.substr( 0, pos );

	// item
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( itemName, ret.c_str() );
	kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
	if( page == NULL || paramName == NULL || strlen( paramName ) == 0 ) {
		return ret;
	}
	settings.setValue( paramName, ret.c_str() );

	kome::objects::SettingParameterValues* subSettings = settings.getSubParameterValues( paramName, ret.c_str() );
	page->setParameters( *subSettings );

	// parse sub parameters
	int paramIdx = 0;
	int level = 0;
	int start = pos + 1;
	const char* paramVal = str.c_str();

	for( unsigned int i = pos + 1;
			i < str.length() && paramIdx < (int)page->getNumberOfForms(); i++ ) {
		// level
		if( str[ i ] == '[' ) {
			level = level + 1;
		}
		else if( str[ i ] == ']' ) {
			level = level - 1;
		}

		// delim
		if( level < 0 || ( level == 0 &&  str[ i ] == ',' ) ) {
			// value
			std::string subVal( paramVal + start, paramVal + i );

			if( !subVal.empty() ) {
				kome::plugin::SettingsForm* form = page->getForm( paramIdx );
				kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
				kome::plugin::SettingsValue* val = ( param == NULL ? NULL : param->getValue() );

				if( val != NULL ) {
					std::string itemName;
					if( val->getEnumType() == SettingsValue::ENUM_PLUGIN ) {
						itemName = val->getEnumKey();
					}

					if( !itemName.empty() ) {
						subVal = getSubParameters( subVal.c_str(), *subSettings, itemName.c_str(), param->getName() );
					}
				}
				if( param != NULL ) {
					std::string msg = val->checkValue( subVal.c_str(), param->getName(), !( val->isRequired() ) );
					if( msg.empty() ) {	// valid
						subSettings->setValue( param->getName(), subVal.c_str() );
					}
					else {	// invalid value
						msg = FMT(
							"Parameter Error: (%s) %s=%s   %s",
							itemName,
							param->getName(),
							subVal.c_str(),
							msg.c_str()
						);

						LOG_ERROR_CODE( msg, ERR_OTHER );
					}
				}
			}

			// restore
			start = i + 1;
			level = 0;
			paramIdx++;
		}
	}

	return ret;
}

// add plug-in path
void CommandManager::addPluginPath( const char* path ) {
	std::string p = NVL( path, "" );
	if( !p.empty() ) {
		m_pluginPaths.push_back( p );
	}
}

// get the number of plug-in paths
unsigned int CommandManager::getNumberOfPluginPaths() {
	return m_pluginPaths.size();
}

// get plug-in path
const char* CommandManager::getPluginPath( const unsigned int idx ) {
	if( idx >= m_pluginPaths.size() ) {
		return NULL;
	}

	return m_pluginPaths[ idx ].c_str();
}

// get instance
CommandManager& CommandManager::getInstance() {
	// create object (This is the only object.)
	static CommandManager mgr;

	return mgr;
}

// compare options
bool CommandManager::lessOption ( OptionInfo opt0, OptionInfo opt1 ) {
	// compare option name
	std::string name0 = opt0.item->getShortName();
	std::string name1 = opt1.item->getShortName();

	return ( name0.compare( name1 ) < 0 );
}

