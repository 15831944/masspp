/**
 * @file SettingsChapter.cpp
 * @brief implements of SettingsChapter class
 *
 * @author S.Tanaka
 * @date 2007.05.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SettingsChapter.h"

#include "SettingsPage.h"

#include <algorithm>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsChapter::SettingsChapter() {
	m_order = -1;
	m_parent = NULL;

	m_flgContextDisplay = false;	// @date 2012/11/22 <Add> Context Menu
}

// destructor
SettingsChapter::~SettingsChapter() {
	// delete children
	for( unsigned int i = 0; i < m_children.size(); i++ ) {
		delete m_children[ i ];
	}
	m_children.clear();
}

// set chapter name
void SettingsChapter::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get chapter name
const char* SettingsChapter::getName() {
	return m_name.c_str();
}

// set order
void SettingsChapter::setOrder( const int order ) {
	m_order = order;
}

// get order
int SettingsChapter::getOrder() {
	return m_order;
}

// get parent chapter
SettingsChapter* SettingsChapter::getParentChapter() {
	return m_parent;
}

// create child chapter
SettingsChapter* SettingsChapter::createChildChapter() {
	// create child
	SettingsChapter* child = new SettingsChapter();
	child->m_parent = this;

	// add to the array
	m_children.push_back( child );


	return child;
}

// get the number of child chapters
unsigned int SettingsChapter::getNumberOfChildren() {
	return m_children.size();
}

// get child chapter
SettingsChapter* SettingsChapter::getChildChapter( const unsigned int index ) {
	// check the index
	if( index >= m_children.size() ) {
		return NULL;
	}

	return m_children[ index ];
}

// get child chapter
SettingsChapter* SettingsChapter::getChildChapter( const char* name ) {
	// search
	SettingsChapter* child = NULL;
	for( unsigned int i = 0; i < m_children.size() && child == NULL; i++ ) {
		SettingsChapter* c = m_children[ i ];

		if( strcmp( name, c->getName() ) == 0 ) {
			child = c;
		}
	}

	// create
	if( child == NULL ) {
		child = createChildChapter();
		child->setName( name );
	}

	return child;
}

// add page
void SettingsChapter::addPage( SettingsPage* page ) {
	m_pages.push_back( page );
}

// get the number of pages
unsigned int SettingsChapter::getNumberOfPages() {
	return m_pages.size();
}

// get page
SettingsPage* SettingsChapter::getPage( const unsigned int index ) {
	// check the index
	if( index >= m_pages.size() ) {
		return NULL;
	}

	return m_pages[ index ];
}

// sort
void SettingsChapter::sort() {
	// sort page
	if( m_pages.size() > 0 ) {
		std::sort( m_pages.begin(), m_pages.end(), lessPage );
	}

	// sort children
	if( m_children.size() > 0 ) {
		std::sort( m_children.begin(), m_children.end(), lessChapter );

		for( unsigned int i = 0; i < m_children.size(); i++ ) {
			( m_children[ i ] )->sort();
		}
	}
}

// common chapter or not
bool SettingsChapter::isCommon() {
	// search page
	for( unsigned int i = 0; i < m_pages.size(); i++ ) {
		if( m_pages[ i ]->isCommon() ) {
			return true;
		}
	}

	// search child
	for( unsigned int i = 0; i < m_children.size(); i++ ) {
		if( m_children[ i ]->isCommon() ) {
			return true;
		}
	}

	return false;
}

// compare chapters to sort
bool SettingsChapter::lessChapter( SettingsChapter* c0, SettingsChapter* c1 ) {
	// compare order
	int o0 = c0->m_order;
	if( o0 < 0 ) {
		o0 = 5;
	}
	int o1 = c1->m_order;
	if( o1 < 0 ) {
		o1 = 5;
	}

	if( o0 < o1 ) {
		return true;
	}
	if( o0 > o1 ) {
		return false;
	}

	// compare name
	int cmp = c0->m_name.compare( c1->m_name );
	return ( cmp < 0 );
}

// compare pages to sort
bool SettingsChapter::lessPage( SettingsPage* p0, SettingsPage* p1 ) {
	// compare order
	int o0 = p0->getOrder();
	if( o0 < 0 ) {
		o0 = 5;
	}
	int o1 = p1->getOrder();
	if( o1 < 0 ) {
		o1 = 5;
	}

	if( o0 < o1 ) {
		return true;
	}
	if( o0 > o1 ) {
		return false;
	}

	// compare name
	std::string n0 = NVL( p0->getName(), "" );
	std::string n1 = NVL( p1->getName(), "" );
	int cmp = n0.compare( n1 );

	return ( cmp < 0 );
}

// set Flg Context Display
void SettingsChapter::setFlgContextDisplay( bool flg )
{
	m_flgContextDisplay = flg;
}

// get Flg Context Display
bool SettingsChapter::getFlgContextDisplay( void )
{
	return m_flgContextDisplay;
}

