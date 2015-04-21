/**
 * @file SettingsForm.cpp
 * @brief implement of SettingsParam class
 *
 * @author S.Tanaka
 * @date 2007.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsForm.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// cosntructor
SettingsForm::SettingsForm() {
	m_group = NULL;
	m_param = NULL;
	m_detail = false;
}

// destructor
SettingsForm::~SettingsForm() {
}

// set title
void SettingsForm::setTitle( const char* title ) {
	m_title = NVL( title, "" );
	if( m_desc.empty() ) {
		m_desc = m_title;
	}
}

// get title
const char* SettingsForm::getTitle() {
	return m_title.c_str();
}

// set description
void SettingsForm::setDesc( const char* desc ) {
	m_desc = NVL( desc, "" );
}

// get description
const char* SettingsForm::getDesc() {
	return m_desc.c_str();
}

// set image file path
void SettingsForm::setImageFilePath( const char* path ) {
	m_img = NVL( path, "" );
}

// get image file path
const char* SettingsForm::getImageFilePath() {
	return m_img.c_str();
}

// set group
void SettingsForm::setGroup( SettingsGroup* group ) {
	m_group = group;
}

// get group
SettingsGroup* SettingsForm::getGroup() {
	return m_group;
}

// set parameter
void SettingsForm::setParam( SettingsParam* param ) {
	m_param = param;
}

// get parameter
SettingsParam* SettingsForm::getParam() {
	return m_param;
}

// set detail
void SettingsForm::setDetail( const bool detail ) {
	m_detail = detail;
}

// get detail
bool SettingsForm::isDetail() {
	return m_detail;
}


// Context Menu

// has edit menu item
bool SettingsForm::hasEditMenuItem( void ){ 
	return m_bEditMenuItem; 
}

// has add menu item
bool SettingsForm::hasAddMenuItem( void ){ 
	return m_bAddMenuItem; 
}

// has up down menu item
bool SettingsForm::hasUpDownMenuItem( void ){ 
	return m_bUpDownMenuItem; 
}

// has delete menu item
bool SettingsForm::hasDeleteMenuItem( void ){ 
	return m_bDeleteMenuItem; 
}

// has copy menu item
bool SettingsForm::hasCopyMenuItem( void ){ 
	return m_bCopyMenuItem; 
}

// has save all menu item
bool SettingsForm::hasSaveAllMenuItem( void ){ 
	return m_bSaveAllMenuItem; 
}

// has select all menu item
bool SettingsForm::hasSelectAllMenuItem( void ){ 
	return m_bSelectAllMenuItem; 
}

// has filter menu item
bool SettingsForm::hasFilterMenuItem( void ){ 
	return m_bFilterMenuItem; 
}

// set edit menu item
void SettingsForm::setEditMenuItem( bool bVal ){ 
	m_bEditMenuItem = bVal; 
}

// set add menu item
void SettingsForm::setAddMenuItem( bool bVal ){ 
	m_bAddMenuItem = bVal; 
}

// set up down menu item
void SettingsForm::setUpDownMenuItem( bool bVal ){ 
	m_bUpDownMenuItem = bVal; 
}

// set delete menu item
void SettingsForm::setDeleteMenuItem( bool bVal ){ 
	m_bDeleteMenuItem = bVal; 
}

// set copy menu item
void SettingsForm::setCopyMenuItem( bool bVal ){ 
	m_bCopyMenuItem = bVal; 
}

// set save all menu item
void SettingsForm::setSaveAllMenuItem( bool bVal ){ 
	m_bSaveAllMenuItem = bVal; 
}

// set select all menu item
void SettingsForm::setSelectAllMenuItem( bool bVal ){ 
	m_bSelectAllMenuItem = bVal; 
}

// set filter menu item
void SettingsForm::setFilterMenuItem( bool bVal ){ 
	m_bFilterMenuItem = bVal; 
}

// set edit menu name
void SettingsForm::setEditMenuName( std::string strName ){ 
	m_strEditMenuName = strName; 
}

// set add menu name
void SettingsForm::setAddMenuName( std::string strName ){ 
	m_strAddMenuName = strName; 
}

// get edit menu name
std::string SettingsForm::getEditMenuName( void ){ 
	return m_strEditMenuName; 
}

// get add menu name
std::string SettingsForm::getAddMenuName( void ){ 
	return m_strAddMenuName; 
}
