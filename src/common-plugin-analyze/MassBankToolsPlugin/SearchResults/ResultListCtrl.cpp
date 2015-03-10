/**
 * @file ResultListCtrl.cpp
 * @brief implements of ResultListCtrl class
 *
 * @author S.Tanaka
 * @date 2009.09.14
 *
 * @modify M.Fukuda
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ResultListCtrl.h"
#include "../MassBankManager.h"

#define GRID_GAP						3
#define BORDER_SIZE						3

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// EVENT TABLE
BEGIN_EVENT_TABLE(ResultListCtrl, kome::window::GridListCtrl)
	EVT_LEFT_DCLICK(ResultListCtrl::onDblClick)
END_EVENT_TABLE()

// constructor
ResultListCtrl::ResultListCtrl(
	wxWindow* parent,
	const unsigned int width,
	const unsigned int height,
	const bool isModal
) : kome::window::GridListCtrl(parent, -1, width, height, false, false, true)
	, isModal_(isModal)
{
	setPopupMenuFlag( POPUP_COPY | POPUP_DELETE | POPUP_FILTER | POPUP_SAVE );
}

// destructor
ResultListCtrl::~ResultListCtrl() 
{
}

// close peaks dialog
void ResultListCtrl::closePeaksDialog() 
{
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	mgr.unsetActiveRecord();
}

// on create
void ResultListCtrl::onCreate() 
{
	// manager
	onReCreateCtrlList();
}

// on reCreate Ctrl List
void ResultListCtrl::onReCreateCtrlList() 
{	
	clearData(false);
	this->ClearAll();
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();
	
	// having Query?
	m_useScore = mgr.needScoreCol();

	// header
	addColumn( "ID", TYPE_STRING );
	addColumn( "Title", TYPE_STRING );
	addColumn( "Formula", TYPE_STRING );
	addColumn( "Exact Mass", TYPE_UDOUBLE );

	if ( m_useScore ) {
		addColumn( "Score", TYPE_UDOUBLE );
	}

	m_useQuery = mgr.IsUseQueryData();

	if ( m_useQuery ) {
		addColumn( "Query Count", TYPE_UINT );
		addColumn( "Query", TYPE_STRING );
		m_Col_QueryCnt = (m_useScore) ? COL_QUERYCNT : COL_NO_SC_QUERYCNT;
		m_Col_Query = (m_useScore) ? COL_QUERY : COL_NO_SC_QUERY;
	}

	if(isModal_) {
		std::vector<unsigned int>ids;
		mgr.getNumberOfSearchedResults(ids);
		for each(auto& x in ids) {
			addData(x, false);
		}
	} else {
		for( unsigned int i = 0; i < mgr.getNumberOfResults(); i++ ) {
			addData( i, false );
		}
	}

}

// Call Reset
void ResultListCtrl::upDateMe() 
{
	onReCreateCtrlList();
}

// get int value
int ResultListCtrl::getInt( const long long data, const int column ) const 
{
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// result
	MassBankManager::SearchResult* result = mgr.getResult( data );
	if( result == NULL ) {
		return -1;
	}

	double val = -1;
	if( m_useQuery && column == m_Col_QueryCnt ) {    // query count
		val = result->querycount;
	}

	return val;
}


// get double value
double ResultListCtrl::getDouble( const long long data, const int column ) const 
{
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// result
	MassBankManager::SearchResult* result = mgr.getResult( data );
	if( result == NULL ) {
		return -1.0;
	}

	double val = -1.0;
	if( column == COL_EXTMASS ) {	// exact mass
		val = result->mass;
	}
	else if( m_useScore && column == COL_SCORE ) {	// score
		val = result->score;
	}

	return val;
}

// get string value
std::string ResultListCtrl::getString( const long long data, const int column ) const 
{
	// manager
	MassBankManager& mgr = MassBankManager::getInstance();

	// string
	std::string s;

	// result
	MassBankManager::SearchResult* result = mgr.getResult( data );
	if( result == NULL ) {
		return s;
	}

	if( column == COL_ID ) {	// ID
		s = result->id;
	}
	else if( column == COL_TITLE ) {	// title
		s = result->title;
	}
	else if( column == COL_FORMULA ) {	// formula
		s = result->formula;
	}
	else if ( m_useQuery && column == m_Col_Query ) {    // query
		unsigned int count = MIN(10, result->queryArr.size());
		s = result->queryArr.at(0).first;
		for ( unsigned int idx = 1; idx < count; idx++ ) {
			s += " , " + result->queryArr.at(idx).first;
		}
	}

	return s;
}

// on double clicked
void ResultListCtrl::onDblClick( wxMouseEvent& evt ) 
{
	// get selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	if( items.size() == 0 ) {
		return;
	}

	// result index
	int item = items[ 0 ];
	if( item < 0 || item >= GetItemCount() ) {
		return;
	}
	int idx = m_data[ item ];

	// search result
	MassBankManager& mgr = MassBankManager::getInstance();
	mgr.getSearchRecordDetailsFromMassBank(idx,true);
	std::string title = mgr.getActiveRecordTitleName();

	// open dialog
	mgr.showMassBankRecordPane(title.c_str());
	kome::window::WindowTool::refresh();
}
