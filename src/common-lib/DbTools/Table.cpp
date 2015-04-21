/**
 * @file Table.cpp
 * @brief implements of table of Table class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Table.h"

#include "TableColumn.h"
#include "TableIndex.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Table::Table() {
}

// constructor
Table::Table( const char* name ) {
	setName( name );
}

// destructor
Table::~Table() {
	for( unsigned int i = 0; i < m_columns.size(); i++ ) {
		delete m_columns[ i ];
	}
	for( unsigned int i = 0; i < m_indices.size(); i++ ) {
		delete m_indices[ i ];
	}
}

// set table name
void Table::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get table name
const char* Table::getName() {
	return m_name.c_str();
}

// get the number of columns
unsigned int Table::getNumberOfColumns() {
	return m_columns.size();
}

// add column
TableColumn* Table::addColumn( const char* name ) {
	// create object
	TableColumn* col = new TableColumn( this );
	col->setName( name );

	// add 
	m_columns.push_back( col );

	return col;
}

// get column
TableColumn* Table::getColumn( const unsigned int col ) {
	if( col >= m_columns.size() ) {
		return NULL;
	}
	return m_columns[ col ];
}

// get the number of indices
unsigned int Table::getNumberOfIndices() {
	return m_indices.size();
}

// add table index
TableIndex* Table::addIndex( const char* name ) {
	// create object
	TableIndex* index = new TableIndex( this );
	index->setName( name );

	// add
	m_indices.push_back( index );

	return index;
}

// get index
TableIndex* Table::getIndex( const unsigned int index ) {
	if( index >= m_indices.size() ) {
		return NULL;
	}
	return m_indices[ index ];
}
