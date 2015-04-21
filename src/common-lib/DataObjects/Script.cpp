/**
 * @file Script.cpp
 * @brief implements of Script class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Script.h"


// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//
using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki



// static memeber
std::string Script::m_filePath;


// constructor
Script::Script() {
	ScriptManager& mgr = ScriptManager::getInstance();
	mgr.addScript( this );

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
	m_script = "";
	m_pParams = ( kome::objects::Parameters* )NULL;
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

}

// destructor
Script::~Script() {
	ScriptManager& mgr = ScriptManager::getInstance();
	mgr.removeScript( this );
}

// set the script
void Script::setScript( const char* script ) {
	m_script = NVL( script, "" );
}

// get the script
const char* Script::getScript() {
	return m_script.c_str();
}

// get template
std::string Script::getTemplate() {
	std::string t = onGetTemplate();

	return t;
}

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
// set variant
void	Script::setVariant( kome::objects::Variant &var )
{
	m_refVariant = var;
}

// get variant
const kome::objects::Variant &Script::getVariant( void )
{
	return	m_refVariant;
}

// set parameter
void	Script::setParameters( kome::objects::Parameters *pParams )
{
	m_pParams = pParams;
}

// get parameter
const kome::objects::Parameters	*Script::getParameters( void )
{
	return	m_pParams;
}

// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

// run
//std::string Script::run( const char* script ) {
std::string Script::run( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams ) {
	setScript( script );
// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
	setVariant( var );
	setParameters( pParams );
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

	std::string ret = run();

	return ret;
}

// run
std::string Script::run() {
// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
	std::string	ret = onRun( m_script.c_str( ), m_refVariant, m_pParams );
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

	return ret;
}

// clear varibles
void Script::clearVariables() {
	m_variables.clear();
}

// add variable
void Script::addVariable( const char* name, const char* desc ) {
	// check parameters
	std::string n = NVL( name, "" );
	if( n.empty() ) {
		return;
	}

	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_variables.size() && idx < 0; i++ ) {
		if( m_variables[ i ].name.compare( n ) == 0 ) {
			idx = (int)i;
		}
	}

	// add
	if( idx < 0 ) {
		m_variables.resize( m_variables.size() + 1 );
		m_variables.back().name = n;
		m_variables.back().desc = NVL( desc, "" );
	}
	else {
		m_variables[ idx ].desc = NVL( desc, "" );
	}
}

// get the variable count
unsigned int Script::getNumberOfVariables() {
	return m_variables.size();
}

// get variable name
const char* Script::getVariableName( const unsigned int idx ) {
	if( idx >= m_variables.size() ) {
		return NULL;
	}

	return m_variables[ idx ].name.c_str();
}

// get variable description
const char* Script::getVariableDescription( const unsigned int idx ) {
	if( idx >= m_variables.size() ) {
		return NULL;
	}

	return m_variables[ idx ].desc.c_str();
}

// set the script file path
void Script::setScriptFilePath( const char* path ) {
	m_filePath = NVL( path, "" );
}

// get the script file path
const char* Script::getScriptFilePath() {
	return m_filePath.c_str();
}


//
// ScriptManager
//

// constructor
ScriptManager::ScriptManager() {
}


// destructor
ScriptManager::~ScriptManager() {
	while( m_scriptSet.size() > 0 ) {
		delete *( m_scriptSet.begin() );
	}
}

// add script
void ScriptManager::addScript( Script* script ) {
	if( script != NULL ) {
		m_scriptSet.insert( script );
	}
}

// remove script
void ScriptManager::removeScript( Script* script ) {
	m_scriptSet.erase( script );
}

// get the instance
ScriptManager& ScriptManager::getInstance() {
	static ScriptManager mgr;

	return mgr;
}
