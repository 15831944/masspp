/**
 * @file TppSearchGrid.cpp
 * @brief implements of TppSearchGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppSearchGrid.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;
using namespace kome::Tpp;

#define COLUMNNAME_TITLE		"Title"
#define COLUMNNAME_DATE			"Date"
#define COLUMNNAME_SEARCHENGINE	"Search Engine"
#define COLUMNNAME_COMMENT		"Comment"

BEGIN_EVENT_TABLE( TppSearchGrid, kome::window::CheckGridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, TppSearchGrid::OnColClick )
END_EVENT_TABLE()

//constructor
TppSearchGrid::TppSearchGrid( wxWindow* parent, int id )
	: kome::window::CheckGridListCtrl( parent, id, 400, 150, false ) {

}

//destructor
TppSearchGrid::~TppSearchGrid(){
	m_resultList.clear();
	
}

//create table
void TppSearchGrid::onCreate(){
	// single selection
	SetWindowStyle( GetWindowStyle() | wxLC_SINGLE_SEL );

	//column
	addColumn(COLUMNNAME_TITLE, TYPE_STRING);
	addColumn(COLUMNNAME_DATE, TYPE_STRING);
	addColumn(COLUMNNAME_SEARCHENGINE, TYPE_STRING);
	addColumn(COLUMNNAME_COMMENT, TYPE_STRING);


	ident::IdentificationManager& identMgr = ident::IdentificationManager::getInstance();
	std::vector< int > searches;

	identMgr.getSearches(searches);
	

	for(int i=0;i < searches.size(); i++){
		addData(searches[i], false);
		// 20130415 tashiro modified
		//kome::ident::SearchResult result;
		//identMgr.getSearch(searches[i], result);
		kome::ident::SearchInfo result;
		identMgr.getSearch(searches[i], result);
		// /20130415 tashiro modified

		T_searchResult searchResult;
		// 20130415 tashiro modified
//		searchResult.comment = result.getComment();
//		searchResult.date = result.getDate();
//		searchResult.title = result.getTitle();
//		searchResult.searchEngine = result.getSearchEngine()->getName();
		searchResult.comment = result.comment;
		searchResult.date = result.date;
		searchResult.title = result.name;
		searchResult.searchEngine = identMgr.getSearchEngineName( result.engineId );
		// /20130415 tashiro modified
		m_resultList.push_back(searchResult);
	}
	
	updateList();
}

//get staring value
std::string TppSearchGrid::getString( const long long data, const int col ) const {
	
	std::string strRet;

	if(data > m_resultList.size()){
		return "";
	}

	T_searchResult result = m_resultList.at(data - 1);

   //select data
    if( col == 0 ) {    // title
        strRet = result.title;
    }
    else if( col == 1 ) {    // date
        strRet = result.date;
    }
    else if( col == 2 ) {    // name
		strRet = result.searchEngine;
	}
	else if (col == 3 ){		//comment
		strRet = result.comment;
	}
	
    return strRet;
}

// on column click
void TppSearchGrid::OnColClick( wxListEvent& evt )
{
	// To lose the sorting application, this function that doesn't do anything is called.
}

