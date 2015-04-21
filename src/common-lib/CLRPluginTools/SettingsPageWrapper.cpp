/**
 * @file SettingsPageWrapper.cpp
 * @brief implements of SettingsPageWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsPageWrapper.h"

#include "SettingsFormWrapper.h"
#include "ParameterSettingsWrapper.h"
#include "PluginFunctionItemWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsPageWrapper::SettingsPageWrapper( kome::plugin::SettingsPage& page ) {
	m_page = &page;
	m_delFlg = false;
}

// constructor
SettingsPageWrapper::SettingsPageWrapper( ParameterSettingsWrapper^ settings ) {
	m_page = new kome::plugin::SettingsPage( settings->getParameterSettings() );
	m_delFlg = true;
}

// destructor
SettingsPageWrapper::~SettingsPageWrapper() {
	if( m_delFlg ) {
		delete m_page;
	}
	m_page = NULL;
}

// get settings page object
kome::plugin::SettingsPage& SettingsPageWrapper::getSettingsPage() {
	return *m_page;
}

// set name
void SettingsPageWrapper::setName( System::String^ name ) {
	m_page->setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ SettingsPageWrapper::getName() {
	return ClrObjectTool::convertString( m_page->getName(), NULL );
}

// set title
void SettingsPageWrapper::setTitle( System::String^ title ) {
	m_page->setTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get title
System::String^ SettingsPageWrapper::getTitle() {
	return ClrObjectTool::convertString( m_page->getTitle(), NULL );
}

// set order
void SettingsPageWrapper::setOrder( int order ) {
	m_page->setOrder( order );
}

// get order
int SettingsPageWrapper::getOrder() {
	return m_page->getOrder();
}

// set common
void SettingsPageWrapper::setCommon( bool common ) {
	m_page->setCommon( common );
}

// get common
bool SettingsPageWrapper::isCommon() {
	return m_page->isCommon();
}

// add form
SettingsFormWrapper^ SettingsPageWrapper::addForm() {
	kome::plugin::SettingsForm* form = m_page->addForm();
	if( form == NULL ) {
		return nullptr;
	}

	return gcnew SettingsFormWrapper( *form );
}

// get the number of forms
unsigned int SettingsPageWrapper::getNumberOfForms() {
	return m_page->getNumberOfForms();
}

// get form
SettingsFormWrapper^ SettingsPageWrapper::getForm( unsigned int index ) {
	kome::plugin::SettingsForm* form = m_page->getForm( index );
	if( form == NULL ) {
		return nullptr;
	}

	return gcnew SettingsFormWrapper( *form );
}

// set parameters
void SettingsPageWrapper::setParameters(
		SettingParameterValuesWrapper^ settings,
		System::String^ prefix
) {
	// check the parameter
	kome::objects::SettingParameterValues* stngs = ClrObjectTool::getSettings( settings );
	if( stngs == NULL ) {
		return;
	}

	// prefix
	std::string prfx = ClrObjectTool::convertString( prefix );

	// set parameters
	m_page->setParameters( *stngs, prfx.c_str() );
}

// clear check functions
void SettingsPageWrapper::clearChecks() {
	m_page->clearChecks();
}

// add check functions
void SettingsPageWrapper::addCheck( PluginFunctionItemWrapper^ fun ) {
	if( fun == nullptr ) {
		return;
	}

	m_page->addCheck( &fun->getPluginFunctionItem() );
}

// get the number of check functions
unsigned int SettingsPageWrapper::getNumberOfChecks() {
	return m_page->getNumberOfChecks();
}

// get check function
PluginFunctionItemWrapper^ SettingsPageWrapper::getCheck( unsigned int idx ) {
	kome::plugin::PluginFunctionItem* fun = m_page->getCheck( idx );
	if( fun == NULL ) {
		return nullptr;
	}

	return gcnew PluginFunctionItemWrapper( *fun );
}

// get parameter string
System::String^ SettingsPageWrapper::getParameterString( SettingParameterValuesWrapper^ settings ) {
	// check the parameter
	kome::objects::SettingParameterValues* stngs = ClrObjectTool::getSettings( settings );
	if( stngs == NULL ) {
		return nullptr;
	}

	// get string
	std::string s = m_page->getParameterString( *stngs );

	return ClrObjectTool::convertString( s.c_str(), NULL );
}

// set parameter string
void SettingsPageWrapper::setParameterString( SettingParameterValuesWrapper^ settings, System::String^ str ) {
	// check the parameter
	kome::objects::SettingParameterValues* stngs = ClrObjectTool::getSettings( settings );
	if( stngs == NULL ) {
		return;
	}

	// set
	std::string s = ClrObjectTool::convertString( str );
	m_page->setParameterString( *stngs, s.c_str() );
}
