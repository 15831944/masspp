/**
 * @file DatabaseFilesListCtrl.cpp
 * @brief implementation of DatabaseFilesListCtrl class
 *
 * @author H.Parry
 * @date 2012.3.15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "DatabaseFilesListCtrl.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( DatabaseFilesListCtrl, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
DatabaseFilesListCtrl::DatabaseFilesListCtrl( wxWindow* parent, const int width, const int height, SearchEngine* pSearchEngine )
		: kome::window::CheckGridListCtrl( parent, wxID_ANY, width, height, false ), m_pSearchEngine(pSearchEngine)
{
	m_fileCol = -1;
}

// destructor
DatabaseFilesListCtrl::~DatabaseFilesListCtrl(void)
{
}

// check whether a database is selected
bool DatabaseFilesListCtrl::databaseFileSelected(const char* dbName)
{
	bool bSelected = false;

	for (int i = 0; (i < static_cast<int>(m_selectedDbs.size())) && !bSelected; i++)
	{
		if (m_selectedDbs[i].compare(dbName) == 0)
		{
			bSelected = true;
		}
	}

	return bSelected;
}

// on create
void DatabaseFilesListCtrl::onCreate() {

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	addColumn( "Database File", TYPE_STRING );
	m_fileCol = 0;

	// add items
	for( unsigned int i = 0; i < m_databaseFiles.size(); i++ ) {
		addData( i, false );
	}
}

// get string value
std::string DatabaseFilesListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	if( column == m_fileCol ) {	// file

		if (data >= 0)
		{
			s = m_databaseFiles[data];
		}
	}

	return s;
}

// set the database name
void DatabaseFilesListCtrl::setDbName(const char* dbName)
{
	m_dbName = dbName;
	m_pSearchEngine->getDbFiles(m_dbName.c_str(), m_databaseFiles);

	// Clear all items from the checklistbox.
	clearData(true);

	for( unsigned int i = 0; i < m_databaseFiles.size(); i++ ) {
		addData( i, false );
	}

	deselectAll();
	updateList();

	Enable( m_databaseFiles.size() != 0 );
}

// set the selected databases list
void DatabaseFilesListCtrl::setSelectedDbs(std::vector<std::string> selectedDbs)
{
	m_selectedDbs = selectedDbs;

	// select items
	for( unsigned int i = 0; i < m_databaseFiles.size(); i++ ) {
		if (databaseFileSelected(m_databaseFiles[i].c_str()))
		{
			setSelected( i, true );
		}
	}
}
