/**
 * @file TppResultGrid.cpp
 * @brief implements of TppResultGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppResultGrid.h"
#include "TppManager.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;
using namespace kome::Tpp;
using namespace kome::db;

BEGIN_EVENT_TABLE( TppResultGrid, kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, TppResultGrid::OnColClick )
	EVT_LEFT_DCLICK( TppResultGrid::OnDClick )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, TppResultGrid::OnSelect )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, TppResultGrid::OnDeselect )

END_EVENT_TABLE()

#define COLUMNNAME_DATE		TABLE_RESULT_COLUMN_DATE
#define COLUMNNAME_TARGET	TABLE_RESULT_COLUMN_TARGET
#define COLUMNNAME_COMMENT	TABLE_RESULT_COLUMN_COMMENT

//constructor
TppResultGrid::TppResultGrid( wxWindow* parent, int id )
	: kome::window::GridListCtrl( parent, id, 400, 150, false ) {
		m_llSelectedIndex = -1;
}

//destructor
TppResultGrid::~TppResultGrid(){
	
}

//create table
void TppResultGrid::onCreate(){
	// single selection
	SetWindowStyle( GetWindowStyle() | wxLC_SINGLE_SEL );

	//column
	addColumn(COLUMNNAME_DATE, TYPE_STRING);
	addColumn(COLUMNNAME_TARGET, TYPE_STRING);
	addColumn(COLUMNNAME_COMMENT, TYPE_STRING);

	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	TppManager& tppMgr = TppManager::getInstance();

	Database* db = dbMgr.getDb();

	if(db == NULL){
		LOG_ERROR(FMT("Failed to get database."));
		return;
	}

	ResultSet* rs = db->executeQuery("select * from TPPRESULTS;");
	
	long long index = 1;
	while(rs->next()){
		T_result result;
		result.Date = rs->getString(0, "");
		result.Title = rs->getString(1, "");
		result.Comment = rs->getString(2, "");
		result.Path = rs->getString(3, "");
		m_resultList.push_back(result);
		addData(index++, false);
	}
	
	updateList();
}

std::string TppResultGrid::getString( const long long data, const int col ) const
{
	std::string strRet;

	if(data > m_resultList.size()){
		return "";
	}

	T_result result = m_resultList.at(data - 1);

   //select data
    if( col == 0 ) {    // title
        strRet = result.Date;
    }
    else if( col == 1 ) {    // date
		strRet = result.Title;
    }
    else if( col == 2 ) {    // name
		strRet = result.Comment;
	}
	else if (col == 3 ){		//comment
		strRet = result.Path;
	}
    return strRet;

}


// on column click
void TppResultGrid::OnDClick( wxMouseEvent& evt )
{
	TppManager& tppMgr = TppManager::getInstance();

	if(GetSelectedItemCount() > 0 && m_llSelectedIndex >= 0){
		tppMgr.openResult(m_llSelectedIndex);
	}

}

// on selected
void TppResultGrid::OnSelect( wxListEvent& evt )
{
	m_llSelectedIndex = evt.GetItem().GetId();
}

// deselect
void TppResultGrid::OnDeselect( wxListEvent& evt )
{
	m_llSelectedIndex = -1;
}

// column clicked
void TppResultGrid::OnColClick( wxListEvent& evt )
{
	//disable sort
}
