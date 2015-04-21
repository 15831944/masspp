/**
 * @file SettingsChapterWrapper.cpp
 * @brief implements of SettingsPageWrapper class
 *
 * @author S.Tanaka
 * @date 2007.05.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsChapterWrapper.h"

#include "SettingsPageWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsChapterWrapper::SettingsChapterWrapper( kome::plugin::SettingsChapter& chapter )
		: m_chapter( chapter ) {
}

// destructor
SettingsChapterWrapper::~SettingsChapterWrapper() {
}

// get settings chapter object
kome::plugin::SettingsChapter& SettingsChapterWrapper::getSettingsChapter() {
	return m_chapter;
}

// set name
void SettingsChapterWrapper::setName( System::String^ name ) {
	m_chapter.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ SettingsChapterWrapper::getName() {
	return ClrObjectTool::convertString( m_chapter.getName(), NULL );
}

// set order
void SettingsChapterWrapper::setOrder( int order ) {
	m_chapter.setOrder( order );
}

// get order
int SettingsChapterWrapper::getOrder() {
	return m_chapter.getOrder();
}

// get parent chapter
SettingsChapterWrapper^ SettingsChapterWrapper::getParentChapter() {
	kome::plugin::SettingsChapter* parent = m_chapter.getParentChapter();
	if( parent == NULL ) {
		return nullptr;
	}
	return gcnew SettingsChapterWrapper( *parent );
}

// create child chapter
SettingsChapterWrapper^ SettingsChapterWrapper::createChildChapter() {
	kome::plugin::SettingsChapter* child = m_chapter.createChildChapter();
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew SettingsChapterWrapper( *child );
}

// get the number of child chapters
unsigned int SettingsChapterWrapper::getNumberOfChildren() {
	return m_chapter.getNumberOfChildren();
}

// get child chapter
SettingsChapterWrapper^ SettingsChapterWrapper::getChildChapter( unsigned int index ) {
	kome::plugin::SettingsChapter* child = m_chapter.getChildChapter( index );
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew SettingsChapterWrapper( *child );
}

// get child chapter
SettingsChapterWrapper^ SettingsChapterWrapper::getChildChapter( System::String^ name ) {
	kome::plugin::SettingsChapter* child = m_chapter.getChildChapter( ClrObjectTool::convertString( name ).c_str() );
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew SettingsChapterWrapper( *child );

}

// add page
void SettingsChapterWrapper::addPage( SettingsPageWrapper^ page ) {
	kome::plugin::SettingsPage* p = NULL;
	if( page != nullptr ) {
		p = &page->getSettingsPage();
	}

	m_chapter.addPage( p );
}

// get the number of pages
unsigned int SettingsChapterWrapper::getNumberOfPages() {
	return m_chapter.getNumberOfPages();
}

// get page
SettingsPageWrapper^ SettingsChapterWrapper::getPage( unsigned int index ) {
	kome::plugin::SettingsPage* p = m_chapter.getPage( index );
	if( p == NULL ) {
		return nullptr;
	}
	return gcnew SettingsPageWrapper( *p );
}

// sort
void SettingsChapterWrapper::sort() {
	m_chapter.sort();
}

// common chapter or not
bool SettingsChapterWrapper::isCommon() {
	return m_chapter.isCommon();
}
