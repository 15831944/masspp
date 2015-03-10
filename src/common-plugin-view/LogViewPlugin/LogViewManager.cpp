/**
 * @file LogViewManager.cpp
 * @brief implements of LogViewManager class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * @data 2011.03.17 <Add> M.Aihara
 *       add TBB mutex code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LogViewManager.h"
#include "LogView.h"

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

// @data 2011.03.17 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#include "tbb/spin_mutex.h"
using namespace tbb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

namespace
{
	typedef spin_mutex LOGVIEW_ParallelMutexType;
	LOGVIEW_ParallelMutexType  LOGVIEW_ParallelMutex;
}
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara

using namespace kome::view;

//2014/08/20 add by t.okuno -> leave the log
const int			LogViewManager::m_nDelLogDay	= 30;
const int			LogViewManager::m_nSecPerDay	= 24 * 60 * 60;
const unsigned int	LogViewManager::m_nLogFileLen	= 17;
//2014/08/20 add by t.okuno <- leave the log

// constructor
//2014/08/20 upd by t.okuno -> leave the log
//LogViewManager::LogViewManager() {
LogViewManager::LogViewManager()
{
//2014/08/20 upd by t.okuno <- leave the log
	m_fp = NULL;
	m_view = NULL;
	m_fileLevel = LOG_LEVEL_NONE;
}

// destructor
LogViewManager::~LogViewManager() {
}

// get log file path
const char* LogViewManager::getFilePath() {
	if( m_logFilePath.empty() ) {
		std::string dir = getLogDir();
		//2014/08/20 upd by t.okuno -> leave the log
	//	std::string file = FMT( "log%04x.txt", getprocessid() );
		const int	nLen = 16;
		char		szDate[ nLen ];
		time_t		nCurrDate = time( NULL );

		// initialize
		sprintf( szDate, "%08d", 0 );

		// get current date
		if( nCurrDate >= 0 ){
			struct tm*	pstDate = localtime( &nCurrDate );
			if( pstDate != NULL ){
				strftime( szDate, nLen, "%Y%m%d", pstDate );
			}
		}

		std::string file = FMT( "log_%s_%04x.txt", szDate, getprocessid() );
		//2014/08/20 upd by t.okuno <- leave the log

		m_logFilePath = getpath( dir.c_str(), file.c_str() );

		m_fp = fileopen( m_logFilePath.c_str(), "w" );
	}

	return m_logFilePath.c_str();
}

// set file log level
void LogViewManager::setFileLogLevel( const int level ) {
	m_fileLevel = level;

	setloglevel( MAX( m_fileLevel, m_viewLevel ) );
}

// get file log level
int LogViewManager::getFileLogLevel() {
	return m_fileLevel;
}

// set view log level
void LogViewManager::setViewLogLevel( const int level ) {
	m_viewLevel = level;

	setloglevel( MAX( m_fileLevel, m_viewLevel ) );
}

// get view log level
int LogViewManager::getViewLogLevel() {
	return m_viewLevel;
}

// create log view
void LogViewManager::createLogView() {
	// check the member
	if( m_view != NULL ) {
		return;
	}

	// create view
	m_view = new LogView(
		kome::window::WindowTool::getMainWindow()
	);

	// docking
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_view, "Diagnostic Log", kome::window::WindowTool::DOCK_ALIGN_BOTTOM );
	}
}

// set log function
void LogViewManager::setFunction() {
	// set function
	setoutputlogfunction( writeLog );

	// flush
	logflush();
}

// close log
void LogViewManager::closeLog() {
	// close file
	if( m_fp != NULL ) {
		fflush( m_fp );
		fclose( m_fp );
	}

	// initialize members
	m_fp = NULL;
	m_logFilePath.clear();
	m_fileLevel = LOG_LEVEL_NONE;
	m_viewLevel = LOG_LEVEL_NONE;

	// set function
	setoutputlogfunction( NULL );
}

// the log view is created or not
bool LogViewManager::hasView() {
	return ( m_view != NULL );
}

// release log view
void LogViewManager::releaseLogView() {
	m_view = NULL;
}

// the log view is visible or not
bool LogViewManager::isVisibleView() {
	// check the member
	if( m_view == NULL ) {
		return false;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// judges
	return mainFrame->isVisibleBar( m_view );
}

// toggles visibility of the log view
void LogViewManager::toggleView() {
	// check the member
	if( m_view == NULL ) {
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_view );
}

// get log directory
const char* LogViewManager::getLogDir() {
	if( m_logDir.empty() ) {
		// manager
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

		// make dir
		const char* confDir = msppMgr.getConfDir();
		m_logDir = getpath( confDir, "log" );
		if( !fileexists( m_logDir.c_str() ) ) {
			makedirectory( m_logDir.c_str() );
		}

		// delete files
		boost::filesystem::path p( m_logDir.c_str(), boost::filesystem::native );
		boost::filesystem::directory_iterator end;

		for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {
			boost::filesystem::path child = *it;
			child = boost::filesystem::absolute( child );

			std::string fileName = getfilename( child.string().c_str() );
			std::string pidStr;

			//2014/08/20 add by t.okuno -> leave the log
			if( fileName.length() > m_nLogFileLen ){
				DeleteOldLogFile( child.string().c_str() );
				continue;
			}
			//2014/08/20 add by t.okuno <- leave the log

			if( fileName.length() > 7 ) {
				pidStr = fileName.substr( 3, 7 );
			}
			int pid = toint( pidStr.c_str(), 16, -1 );

			if( !processexists( pid ) ) {
				removefile( child.string().c_str() );
			}
		}
	}

	return m_logDir.c_str();
}

//2014/08/20 add by t.okuno -> leave the log
// check log file date
bool LogViewManager::DeleteOldLogFile( const char* szName )
{
	const int	nDatePosL = 4;
	const int	nDatePosR = 12;
	const int	nLen = 16;
	bool		bRet = false;

	// check param
	if( szName != NULL ){
		if( strlen( szName ) > nDatePosR ){
			char		szDate[ nLen ];
			time_t		nDelDate = time( NULL ) - m_nDelLogDay * m_nSecPerDay;
			std::string strFileName = getfilename( szName );
			std::string	strFileDate = strFileName.substr( nDatePosL, nDatePosR );

			// initialize
			sprintf( szDate, "%08d", 0 );

			// get log file date
			if( nDelDate >= 0 ){
				struct tm*	pstDate = localtime( &nDelDate );
				if( pstDate != NULL ){
					strftime( szDate, nLen, "%Y%m%d", pstDate );
				}
			}

			// delete log file
			int		nDelDateNum = toint( szDate );
			int		nGetDateNum = toint( strFileDate.c_str() );

			if( nGetDateNum < nDelDateNum ){
				removefile( szName );
				bRet = true;
			}
		}
	}

	return bRet;
}
//2014/08/20 add by t.okuno <- leave the log

// write log
void LogViewManager::writeLog( const char* log, int level ) {
	// check parameters
	if( log == NULL || level <= LOG_LEVEL_NONE ) {
		return;
	}
// @data 2011.03.17 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
	{
		LOGVIEW_ParallelMutexType::scoped_lock lock( LOGVIEW_ParallelMutex );
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

	// get object
	LogViewManager& mgr = getInstance();

	// write log to file
	mgr.getFilePath();
	if( mgr.m_fp != NULL ) {
		if( level <= mgr.m_fileLevel ) {
			fprintf( mgr.m_fp, "%s\n", log );
			fflush( mgr.m_fp );
		}
	}

	// write log to view
	if( mgr.m_view != NULL && level <= mgr.m_viewLevel ) {
		mgr.m_view->addLog( log );
	}
// @data 2011.03.17 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
	}
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-
}

// get instance
LogViewManager& LogViewManager::getInstance() {
	// create object (This is the only object)
	static LogViewManager mgr;

	return mgr;
}
