/**
 * @file TableColumn.cpp
 * @brief implements of TableColumn class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TableColumn.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TableColumn::TableColumn( Table* table ) {
	m_table = table;

	m_type = VARCHAR;
	m_length = -1;
	m_notNull = false;
	m_autoIncrement = false;
}

// destructor
TableColumn::~TableColumn() {
}

// gets table
Table* TableColumn::getTable() {
	return m_table;
}

// set column name
void TableColumn::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get column name
const char* TableColumn::getName() {
	return m_name.c_str();
}

// set data type
void TableColumn::setDataType( const DataType type ) {
	m_type = type;
}

// get data type
TableColumn::DataType TableColumn::getDataType() {
	return m_type;
}

// set data length
void TableColumn::setDataLength( const int length ) {
	m_length = length;
}

// get data length
int TableColumn::getDataLength() {
	return m_length;
}

// set not null flag value
void TableColumn::setNotNull( const bool notNull ) {
	m_notNull = notNull;
}

// get not null flag value
bool TableColumn::isNotNull() {
	return m_notNull;
}

// set auto increment flag value
void TableColumn::setAutoIncrement( const bool autoIncrement ) {
	m_autoIncrement = autoIncrement;
}

// get auto increment flag vlaue
bool TableColumn::isAutoIncrement() {
	return m_autoIncrement;
}
