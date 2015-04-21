/**
 * @file TableIndex.cpp
 * @brief implements of TableIndex class
 *
 * @author S.Tanaka
 * @date 2008.09.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TableIndex.h"

#include "Table.h"
#include "TableColumn.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TableIndex::TableIndex( Table* table ) {
	m_table = table;
}

// destructor
TableIndex::~TableIndex() {
}

// get table
Table* TableIndex::getTable() {
	return m_table;
}

// set name
void TableIndex::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get name
const char* TableIndex::getName() {
	return m_name.c_str();
}

// add column
void TableIndex::addColumn( TableColumn* col ) {
	if( col != NULL && col->getTable() == m_table ) {
		m_columns.push_back( col );
	}
}

// get the number of columns
unsigned int TableIndex::getNumberOfColumns() {
	return m_columns.size();
}

// get table column
TableColumn* TableIndex::getColumn( const unsigned int index ) {
	if( index >= m_columns.size() ) {
		return NULL;
	}
	return m_columns[ index ];
}
