/**
 * @file LogGridList.cpp
 * @brief Log grid list ctrl class
 *
 * @author M.Izumi
 * @date 2011.12.09
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "LogGridList.h"
#include "OperationLogManager.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( LogGridList, kome::window::GridListCtrl )
END_EVENT_TABLE()

// constructor
LogGridList::LogGridList( wxWindow* parent )
	: kome::window::GridListCtrl( parent, wxID_ANY, -1, -1,  false )
{
	m_optInfo.clear();
	// create list ctrl
	create();

	addDataInfo();

	// update list
	updateList();
}

//destructor
LogGridList::~LogGridList(){
}

// create column 
void LogGridList::onCreate(){
	// insert column
	addColumn( "Name", TYPE_STRING );
	addColumn( "Type", TYPE_STRING );
	addColumn( "Description", TYPE_STRING );
}

// 行に表示する文字列取得
std::string LogGridList::getString( const long long data, const int column ) const{
	std::string s;

	if( data >=0 && data < m_optInfo.size() ){
		switch ( column ) {
		case 0:
			s = m_optInfo[(int)data].strName;
			break;
		case 1:		
			s = getStrType( (int)data );	
			break;
		case 2:
			s = m_optInfo[(int)data].strDescription;
			break;
		default:
			break;
		}
	}

	return s;
}

// get type
std::string LogGridList::getStrType( int index ) const{
	std::string s;
	
	int i = m_optInfo[index].iType;
	
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	s = mgr.getOperationType( i );

	return s;
}

void LogGridList::addDataInfo(){
	
	// Operation Manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int iFinish = optMgr.getNumberOfFinishedOperations();
	
	for( int i=0; i < iFinish; i++ ){
		addData( i, false );
		
		kome::operation::Operation* opt = NULL;
		// get finished operation
		opt = optMgr.getFinishedOperation( i );
				
		OperationInfo tmp;
		tmp.strName = opt->getOperationName();
		tmp.iType = opt->getOperationType();
		tmp.strDescription = opt->getDescription();
		tmp.opt = opt;
	
		m_optInfo.push_back( tmp );
	}
}

// update log list
void LogGridList::UpdateLogList(){
	
	m_optInfo.clear();
	
	// initialize
	deselectAll();
	clearData( false );

	addDataInfo();

	// update list
	updateList();
}

// 選択されたオペレーションを取得
void LogGridList::getSelectedOperation( std::vector< kome::operation::Operation* >& operations ){
	// get selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	// get operation
	for( unsigned int i = 0; i < items.size(); i++ ) {
		int item = items[ i ];
		if( item >= 0 && item < (int)m_optInfo.size() ) {

			operations.push_back( m_optInfo[ item ].opt );			
		}
	}

}
