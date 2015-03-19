/**
 * @file SearchResultListCtrl.cpp
 * @brief implements of SearchResultListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SearchResultListCtrl.h"  
#include "SearchResultDialog.h"	// @date 2013/01/22 <Add> OKADA

#define ID_DELETE		13351
#define ID_TIMER         7000

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
kome::ident::SearchResultDialog* SearchResultListCtrl::m_dlg = NULL;			// @date 2013/01/23 <Add> OKADA


BEGIN_EVENT_TABLE( SearchResultListCtrl, kome::window::GridListCtrl )
    EVT_TIMER( ID_TIMER, SearchResultListCtrl::onTimer )
END_EVENT_TABLE()


// constructor
SearchResultListCtrl::SearchResultListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 600, 300, false, true, true ) {
	setPopupMenuFlag( POPUP_COPY | POPUP_DELETE | POPUP_FILTER | POPUP_SAVE );

	m_parent = parent;
	m_timer = NULL;

	setActivateName( "Open" );
}

// destructor
SearchResultListCtrl::~SearchResultListCtrl() {
	if( m_timer != NULL ) {
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
		}
		delete m_timer;
		m_timer = NULL;
	}
}

// get selected search
int SearchResultListCtrl::getSelectedSearch() {
	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	if( items.size() == 0 ) {
		return -1;
	}
	int item = items[ 0 ];

	// selected search
	if( item < 0 || item >= (int)m_searches.size() ) {
		return -1;
	}
	
	return m_searches[ getData( item ) ].id; // @date 2013.11.28 <Mod> M.Izumi
}

// on create
void SearchResultListCtrl::onCreate() {
	// manager
	IdentificationManager& mgr = IdentificationManager::getInstance();

	// initialize
	m_searches.clear();

	// search IDs
	std::vector< int > searches;
	mgr.getSearches( searches );
	if( searches.size() > 0 ) {
		m_searches.resize( searches.size() );
	}

	for( unsigned int i = 0; i < searches.size(); i++ ) {
		const int id = searches[ i ];
		kome::ident::SearchInfo searchInfo;
		mgr.getSearch( id, searchInfo );

		m_searches[ i ].title = searchInfo.name;
		m_searches[ i ].id = id;
		m_searches[ i ].searchId = searchInfo.searchId;
		m_searches[ i ].date = searchInfo.date;
		m_searches[ i ].db = searchInfo.db;
		m_searches[ i ].engineId = searchInfo.engineId;
		m_searches[ i ].engine = mgr.getSearchEngineName( searchInfo.engineId );
		m_searches[ i ].comment = searchInfo.comment;
		m_searches[ i ].status = mgr.getSearchStatus( id );

		addData( i, false );
	}

	updateStatus();

	// columns
	addColumn( "Title", kome::window::GridListCtrl::TYPE_STRING, 120 );
	addColumn( "Date", kome::window::GridListCtrl::TYPE_DATE_TIME_STRING, 120 );
	addColumn( "Search Engine",  kome::window::GridListCtrl::TYPE_STRING, 100 );
	addColumn( "Status", kome::window::GridListCtrl::TYPE_STRING, 80 );
	addColumn( "Comment",  kome::window::GridListCtrl::TYPE_STRING, 180 );

	// timer
	if( m_timer != NULL ) {
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
		}
		delete m_timer;
		m_timer = NULL;
	}

	m_timer = new wxTimer( this, ID_TIMER );
	m_timer->Start( 30000 );
}

// get string
std::string SearchResultListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// each columns
	int idx = (int)data;
	if( idx >= 0 && idx < (int)m_searches.size() ) {
		if( column == 0 ) {    // title
			s = m_searches[ idx ].title;
		}
		else if( column == 1 ) {    // date
			s = m_searches[ idx ].date;
		}
		else if( column == 2 ) {    // engine
			s = m_searches[ idx ].engine;
		}
		else if( column == 3 ) {    // status
			if( m_searches[ idx ].status == kome::ident::SearchEngine::STATUS_UNKNOWN ) {
				s = "Unknown";
			}
			else if( m_searches[ idx ].status == kome::ident::SearchEngine::STATUS_WAITING ) {
				s = "Waiting";
			}
			else if( m_searches[ idx ].status == kome::ident::SearchEngine::STATUS_RUNNING ) {
				s = "Running";
			}
			else if( m_searches[ idx ].status == kome::ident::SearchEngine::STATUS_COMPLETED ) {
				s = "Completed";
			}
			else if( m_searches[ idx ].status == kome::ident::SearchEngine::STATUS_ERROR ) {
				s = "Error";
			}
		}
		else if( column == 4 ) {    // comment
			s = m_searches[ idx ].comment;
		}
	}

	return s;
}

// on activate data
void SearchResultListCtrl::onActivateData( const long long data ) {
	
	kome::ident::SearchResultDialog* dlg = (kome::ident::SearchResultDialog*)m_parent;
	if( 0 <= data ){	// dataはダブルクリックされた行番号
		// ダブルクリックされたときの処理
		if( dlg != NULL ){
			dlg->onDoubleClickAtList();
		}
		return;
	}else{
		// ダブルクリック以外の処理 
		dispOnBrowser();	// リストで選択された項目をブラウザで表示
	}

}

// on delete data
bool SearchResultListCtrl::onDeleteData( const long long data ) {
	// confirm
	if( !kome::window::WindowTool::showYesNo( "Are you sure you want to delete the selected item?", "Delete", "Don't delete" ) ) {
		return false;
	}
	
	// search
	IdentificationManager& mgr = IdentificationManager::getInstance();
	const long long index = getData( data );
	// delete
	mgr.deleteSearch( index );

	m_data.erase( m_data.begin() + data );

	updateList();
	Refresh();

	return true;
}

// on activate data
void SearchResultListCtrl::onActivateDataOther( const long long data ) {

	onActivateData( data );
}

// display on browser
void SearchResultListCtrl::dispOnBrowser(){
	// search
	IdentificationManager& mgr = IdentificationManager::getInstance();
	
	// 選択中のItem取得
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	if( items.size() == 0 ) {
		return;
	}
	// @date 2013.11.28 <Mod> M.Izumi
	long long selItem = m_searches[ getData( items[0] ) ].id;	// search ID取得

	kome::ident::SearchInfo searchInfo;
	mgr.getSearch( selItem, searchInfo );	// search IDからSearch情報を取得		// selItem＝search ID

	// search engine
	std::string engineName = mgr.getSearchEngineName( searchInfo.engineId );
	LOG_DEBUG( FMT("search engine = %s", engineName.c_str()));
	SearchEngine* engine = NULL;
	for( int i = 0; i < mgr.getNumberOfSearchEngines() && engine == NULL; i++ ) {
		SearchEngine* tmp = mgr.getSearchEngine( i );
		if( engineName.compare( tmp->getName() ) == 0 ) {
			engine = tmp;
		}
	}

	// open the result
	if( engine != NULL ) {
		if (!engine->openResult( searchInfo.result.c_str() ))	// ブラウザを呼び出して結果表示
		{
			// After calling getlasterrormessage once, the error is deleted.
			// We want to use it twice, so make a copy first.
			std::string lasterrormessage = getlasterrormessage();
			if (lasterrormessage.size() > 0)
			{
				// Display an error dialog if failed to display search results.
				std::string strError = "An error occurred while trying to display search results:\n";
				strError += lasterrormessage;
				kome::window::WindowTool::showError( strError.c_str() );
			}
			else
			{
				LOG_ERROR( FMT("An error occurred while trying to display search results.") );
			}
		}
	}
}

// update status
bool SearchResultListCtrl::updateStatus() {
	// return value
	bool ret = false;

	// manager
	kome::ident::IdentificationManager& identMgr
			= kome::ident::IdentificationManager::getInstance();

	// check the status
	struct UpdateInfo {
		int index;
		kome::ident::SearchEngine* engine;
		kome::ident::SearchEngine::SearchStatus status;
	};

	std::vector< UpdateInfo > updated;
	for( unsigned int i = 0; i < m_searches.size(); i++ ) {
		// engine name
		std::string engineName = trimstring( tolowercase( m_searches[ i ].engine.c_str() ).c_str() );

		// status
		kome::ident::SearchEngine::SearchStatus status = m_searches[ i ].status;

		// update status
		if( status != kome::ident::SearchEngine::STATUS_COMPLETED
				&& status != kome::ident::SearchEngine::STATUS_ERROR ) {
			kome::ident::SearchEngine* engine = NULL;
			for( int j = 0; j < identMgr.getNumberOfSearchEngines() && engine == NULL; j++ ) {
				kome::ident::SearchEngine* tmp = identMgr.getSearchEngine( j );
				std::string tmpName = trimstring( tolowercase( tmp->getName().c_str() ).c_str() );

				if( tmpName.compare( engineName ) == 0 ) {
					engine = tmp;
				}
			}

			if( engine != NULL ) {
				kome::ident::SearchEngine::SearchStatus newStatus = engine->getSearchStatus( m_searches[ i ].searchId.c_str() );
				if( status != newStatus ) {
					status = newStatus;

					updated.resize( updated.size() + 1 );
					updated.back().index = i;
					updated.back().engine = engine;
					updated.back().status = newStatus;

					ret = true;
				}
			}
		}

		m_searches[ i ].status = status;
		COLOR color = kome::img::ColorTool::WHITE;
		if( status == kome::ident::SearchEngine::STATUS_RUNNING ) {
			color = kome::img::ColorTool::getColor( "FFFF99" );
		}
		else if( status == kome::ident::SearchEngine::STATUS_COMPLETED ) {
			color = kome::img::ColorTool::getColor( "B8EAB8" );
		}
		else if( status == kome::ident::SearchEngine::STATUS_ERROR ) {
			color = kome::img::ColorTool::getColor( "EAB8B8" );
		}

		setBackgroundColor( i, color );
	}

	// update
	if( updated.size() > 0 ) {
		kome::window::DialogProgress progress( this, "Registering Results..." );
		progress.createSubProgresses( updated.size() );

		for( unsigned int i = 0; i < updated.size() && !progress.isStopped(); i++ ) {
			kome::core::Progress* prgs = progress.getSubProgress( i );
			int index = updated[ i ].index;
			std::string searchId = m_searches[ index ].searchId;
			int id = m_searches[ index ].id;
			kome::ident::SearchEngine::SearchStatus status = updated[ i ].status;

			if( status == kome::ident::SearchEngine::STATUS_COMPLETED ) {
				prgs->createSubProgresses( 2 );
				kome::core::Progress* prgs0 = prgs->getSubProgress( 0 );

				kome::ident::SearchResult result;
				updated[ i ].engine->getResult( result, searchId.c_str(), *prgs0 );

				prgs0 = prgs->getSubProgress( 1 );
				identMgr.registerResult( id, result, *prgs0 );
			}

			if( !progress.isStopped() ) {
				identMgr.setSearchStatus( id, status );
				prgs->fill();
			}
		}

		if( !progress.isStopped() ) {
			progress.fill();

			identMgr.commit();
		}
	}
			
	return ret;
}

// on timer
void SearchResultListCtrl::onTimer( wxTimerEvent& evt ) {
	if( updateStatus() ) {
		Refresh();
	}
}
