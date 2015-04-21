/**
 * @file SettingsFormWrapper.cpp
 * @brief implements of SettingsFormWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsFormWrapper.h"

#include "SettingsGroupWrapper.h"
#include "SettingsParamWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsFormWrapper::SettingsFormWrapper( kome::plugin::SettingsForm& form ) : m_form( form ) {
}

// destructor
SettingsFormWrapper::~SettingsFormWrapper() {
}

// get settings form object to be wrapped
kome::plugin::SettingsForm& SettingsFormWrapper::getSettingsForm() {
	return m_form;
}

// set title
void SettingsFormWrapper::setTitle( System::String^ title ) {
	m_form.setTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get title
System::String^ SettingsFormWrapper::getTitle() {
	return ClrObjectTool::convertString( m_form.getTitle(), NULL );
}

// set description
void SettingsFormWrapper::setDesc( System::String^ desc ) {
	m_form.setDesc( ClrObjectTool::convertString( desc ).c_str() );
}

// get description
System::String^ SettingsFormWrapper::getDesc() {
	return ClrObjectTool::convertString( m_form.getDesc(), NULL );
}

// set image file path
void SettingsFormWrapper::setImageFilePath( System::String^ path ) {
	m_form.setImageFilePath( ClrObjectTool::convertString( path ).c_str() );
}

// get image file path
System::String^ SettingsFormWrapper::getImageFilePath() {
	return ClrObjectTool::convertString( m_form.getImageFilePath(), NULL );
}

// set group
void SettingsFormWrapper::setGroup( SettingsGroupWrapper^ group ) {
	kome::plugin::SettingsGroup* g = NULL;
	if( group != nullptr ) {
		g = &group->getSettingsGroup();
	}

	m_form.setGroup( g );
}

// get group
SettingsGroupWrapper^ SettingsFormWrapper::getGroup() {
	kome::plugin::SettingsGroup* g = m_form.getGroup();
	if( g == NULL ) {
		return nullptr;
	}

	return gcnew SettingsGroupWrapper( *g );
}

// set parameter
void SettingsFormWrapper::setParam( SettingsParamWrapper^ param ) {
	kome::plugin::SettingsParam* p = NULL;
	if( param != nullptr ) {
		p = &param->getSettingsParam();
	}

	m_form.setParam( p );
}

// get parameter
SettingsParamWrapper^ SettingsFormWrapper::getParam() {
	kome::plugin::SettingsParam* p = m_form.getParam();
	if( p == NULL ) {
		return nullptr;
	}

	return gcnew SettingsParamWrapper( *p );
}

// set detail
void SettingsFormWrapper::setDetail( bool detail ) {
	m_form.setDetail( detail );
}

// get detail
bool SettingsFormWrapper::isDetail() {
	return m_form.isDetail();
}
