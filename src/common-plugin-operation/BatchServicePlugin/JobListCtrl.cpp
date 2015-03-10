/**
 * @file JobListCtrl.cpp
 * @brief implements of JobListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.29
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "JobListCtrl.h"
#include "BatchServicePluginManager.h"


using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ID_UPDATE_TIMER              789
#define STATUS_UPDATE_INTERVAL		(30*1000)	/* ステータスの表示は 30秒(30*1000ms) 間隔 */
#define ID_DOWN_BUTTON                    70

// static member
kome::window::GridListDialog* JobListCtrl::m_dlg = NULL;

BEGIN_EVENT_TABLE( JobListCtrl, kome::window::GridListCtrl )
	EVT_TIMER( ID_UPDATE_TIMER, JobListCtrl::timerEvent )
	
END_EVENT_TABLE()


// constructor
JobListCtrl::JobListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 600, 300, false, true, false ) {
	m_timer = NULL;
		
	// Context Menu
	setPopupMenuFlag( POPUP_SELECT | POPUP_DESELECT | POPUP_COPY | POPUP_DELETE | POPUP_FILTER | POPUP_SAVE );
}


// destructor
JobListCtrl::~JobListCtrl() {
	if( m_timer != NULL ) {
		delete m_timer;
		m_timer = NULL;	// @date 2012/09/13 <Add> OKADA
	}
}


// update statuses
void JobListCtrl::updateStatuses() {
	// manager
	BatchServicePluginManager& mgr = BatchServicePluginManager::getInstance();
	mgr.updateStatuses();

	// set background color
	for( unsigned int i = 0; i < getNumberOfData(); i++ ) {
		int jobId = (int)getData( i );

		kome::plugin::BatchService::BatchStatus status = mgr.getJobStatus( jobId );
		COLOR color = kome::img::ColorTool::WHITE;
		if( status == kome::plugin::BatchService::STATUS_RUNNING ) {
			color = kome::img::ColorTool::getColor( 0xff, 0xff, 0x99 );
		}
		else if( status == kome::plugin::BatchService::STATUS_COMPLETED ) {
			color = kome::img::ColorTool::getColor( 0xb8, 0xea, 0xb8 );
		}
		else if( status == kome::plugin::BatchService::STATUS_ERROR ) {
			color = kome::img::ColorTool::getColor( 0xea, 0xb8, 0xb8 );
		}

		setBackgroundColor( (long long)jobId, color );
	}
}


// on create
void JobListCtrl::onCreate() {
	// manager
	BatchServicePluginManager& mgr = BatchServicePluginManager::getInstance();

	// job IDs
	std::vector< int > jobs;
	mgr.getJobs( jobs );

	for( unsigned int i = 0; i < jobs.size(); i++ ) {
		addData( jobs[ i ], false );
	}

	// columns
	addColumn( "Status", kome::window::GridListCtrl::TYPE_STRING, 120 );
	addColumn( "Date", kome::window::GridListCtrl::TYPE_STRING, 140 );
	addColumn( "Name", kome::window::GridListCtrl::TYPE_STRING, 120 );
	addColumn( "Server", kome::window::GridListCtrl::TYPE_STRING, 120 );
	addColumn( "Comment",  kome::window::GridListCtrl::TYPE_STRING, 180 );

	// update statuses
	updateStatuses();

	// create timer
	if( m_timer == NULL ) {							// @date 2012/09/13 <Mod> OKADA
		m_timer = new wxTimer( this, ID_UPDATE_TIMER );
		m_timer->Start( STATUS_UPDATE_INTERVAL, false );	// @date 2012/09/13 <Add> OKADA
	}
}


// get string
std::string JobListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// manager
	BatchServicePluginManager& mgr = BatchServicePluginManager::getInstance();

	// each columns
	int jobId = (int)data;
	if( column == 0 ) {    // status
		kome::plugin::BatchService::BatchStatus status = mgr.getJobStatus( jobId );
		if( status == kome::plugin::BatchService::STATUS_RUNNING ) {
			s = "Running";
		}
		else if( status == kome::plugin::BatchService::STATUS_COMPLETED ) {
			s = "Complete";
		}
		else if( status == kome::plugin::BatchService::STATUS_ERROR ) {
			s = "Error";
		}
		else {
			s = "Waiting";
		}
	}
	else if( column == 1 ) {    // date
		s = mgr.getJobDate( jobId );
	}
	else if( column == 2 ) {   // name
		s = mgr.getJobTitle( jobId );
	}
	else if( column == 3 ) {    // server
		int server = mgr.getJobServer( jobId );
		s = mgr.getServerDescription( server );
	}
	else if( column == 4 ) {    // comment
		s = mgr.getJobComment( jobId );
	}

	return s;
}

// delete data
bool JobListCtrl::onDeleteData( const long long dataLineNo ) {
	// manager
	BatchServicePluginManager& mgr = BatchServicePluginManager::getInstance();

	long long data = -1;
	if( dataLineNo < m_data.size() ){
		data = m_data[dataLineNo];
	}

	// each columns
	int jobId = (int)data;

	// delete
	mgr.deleteJob( jobId );

	// delete from array
	for( int i = (int)m_data.size() - 1; i >= 0; i-- ) {
		if( m_data[ i ] == data ) {
			m_data.erase( m_data.begin() + i );
		}
	}
	for( int i = (int)m_allData.size() - 1; i >= 0; i-- ) {
		if( m_allData[ i ] == data ) {
			m_allData.erase( m_allData.begin() + i );
		}
	}

	return false;
}

// timer event
void JobListCtrl::timerEvent( wxTimerEvent& evt ) {
	updateStatuses();
	Refresh();
}
