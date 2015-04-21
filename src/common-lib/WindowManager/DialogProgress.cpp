/**
 * @file DialogProgress.cpp
 * @brief implements of DialogProgress class
 *
 * @author S.Tanaka
 * @date 2006.10.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DialogProgress.h"

#include "WindowTool.h"
#include "MainFrame.h"
#include "ProgressDialog.h"


#ifdef _MSC_VER
	#include <windows.h>
#endif		// _MSC_VER

#include	<sys/timeb.h>		// @Date:2014/01/31	<Add>	A.Ozaki 

#define DIALOG_WIDTH	 400
#define DIALOG_HEIGHT	 225


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DialogProgress::DialogProgress(
		wxWindow* parent,
		const char* title,
		const char* firstMsg
) : kome::core::Progress( title ) {
	// initialize
	m_dialog = NULL;
	m_length = 0;
	m_stopped = false;

	m_parent = parent;
	m_msg = NVL( firstMsg, "" );
}

// destructor
DialogProgress::~DialogProgress() {
	if( m_dialog != NULL ) {
		delete m_dialog;
		m_dialog = NULL;
	}
}

// create dialog
void DialogProgress::createDialog() {
	// delete
	if( m_dialog != NULL ) {
		delete m_dialog;
	}

	// new dialog
	m_dialog = new ProgressDialog( m_parent, m_title.c_str() );
	m_dialog->setMessage( m_msg.c_str() );
	m_dialog->setRange( m_length );
	m_dialog->setInformation( m_infoTitle.c_str(), m_info.c_str() );
}

// set information
void DialogProgress::setInformation( const char* title, const char* info ) {
	// information
	m_infoTitle = NVL( title, "" );
	m_info = NVL( info, "" );

	// create dialog
	if( m_dialog == NULL ) {
		createDialog();
	}

	// set information
	m_dialog->setInformation( m_infoTitle.c_str(), m_info.c_str() );
}

// set information
void DialogProgress::setInformation( const char* info ) {
	// information
	m_info = NVL( info, "" );

	// create dialog
	if( m_dialog == NULL ) {
		createDialog();
	}

	// set infromation
	m_dialog->setInformation( info );
}

// get the information title
const char* DialogProgress::getInformationTitle() {
	return m_infoTitle.c_str();
}

// get the information
const char* DialogProgress::getInformation() {
	return m_info.c_str();
}

// set range
void DialogProgress::onSetRange( const int start, const int end ) {
	// length
	int length = end - start;

	// set 
	m_length = length;

	// create dialog
	if( m_dialog == NULL ) {
		createDialog();
	}

	// set range
	m_dialog->setRange( m_length );
}

// set position
void DialogProgress::onSetPosition( const int pos, const int prevPos ) {
	// position
	int p = ( m_length < 0 ) ? ( m_start - pos ) : ( pos - m_start );
	if( p >= m_length ) {

// >>>>>>	@Date:2013/11/25	<Modified>	A.Ozaki
//
		if  ( NULL == m_dialog )
 		{
 			return;
 		}

		m_dialog->setPos( m_length );

#ifdef _MSC_VER
		// 先に描画しているプログレスバーを実際にダイアログ上に
		// 描画させる処置
		//
		if  ( wxIsMainThread( ) )
		{
			MSG	insMsg;
			HWND	hWnd = (HWND)( m_dialog->GetHandle( ) );

			while ( PeekMessage( &insMsg, hWnd, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &insMsg );
				DispatchMessage( &insMsg );
			}
			// 一度イベントループを回したぐらいでは、描画イベントが処理されない
			// 500msec待ってから再度実行させる
			Sleep( 500 );	// @Date:2014/01/31	<Add>	A.Ozaki
			while ( PeekMessage( &insMsg, hWnd, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &insMsg );
				DispatchMessage( &insMsg );
			}
		}
#endif		// _MSC_VER

		// 終わった感を出すために、500msec待たせます
		//
		Sleep( 500 );	// @Date:2014/01/31	<Add>	A.Ozaki

//
// <<<<<<	@Date:2013/11/25	<Modified>	A.Ozaki
		return;
	}

	// create dialog
	if( m_dialog == NULL ) {
		createDialog();
	}

	// show
	if( !m_dialog->IsShown() ) {
		m_dialog->Show( true );
	}

	// position
	m_dialog->setPos( pos );
}

// set status
void DialogProgress::onSetStatus( const char* status, const bool bForced ) {
	// message
	m_msg = NVL( status, "" );

	// create dialog
	if( m_dialog == NULL ) {
		createDialog();
	}

	// set message
	m_dialog->setMessage( m_msg.c_str() );

	// log
	LOG_TRACE( m_msg );

	// create dialog
	if( m_dialog == NULL ) {
		return;
	}
}

// judges whether the stop button is pressed
bool DialogProgress::onIsStopped() {
	// check the flag
	if( m_stopped ) {
		return true;
	}

	// create dialog
	if( m_dialog == NULL ) {
		return false;
	}

#ifdef _MSC_VER
	if( wxIsMainThread() ) {
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
#endif		// _MSC_VER

	// delete
	const bool stopped = m_dialog->isStopped();
	if( stopped ) {
		delete m_dialog;
		m_dialog = NULL;
		m_stopped = true;
	}

	return stopped;
}

// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
//
void	DialogProgress::onFill( void )
{
	if  ( NULL != m_dialog )
	{
		// set message
		if  ( false == m_strStatus.empty( ) )
		{
			m_dialog->setMessage( m_strStatus.c_str() );
			m_strStatus.clear( );
#ifdef _MSC_VER
			//
			if  ( wxIsMainThread( ) )
			{
				MSG	insMsg;
				HWND	hWnd = (HWND)( m_dialog->GetHandle( ) );

				while ( PeekMessage( &insMsg, hWnd, 0, 0, PM_REMOVE ) )
				{
					TranslateMessage( &insMsg );
					DispatchMessage( &insMsg );
				}
				// 一度イベントループを回したぐらいでは、描画イベントが処理されない
				// 500msec待ってから再度実行させる
				Sleep( 500 );	// @Date:2014/01/31	<Add>	A.Ozaki
				
				while ( PeekMessage( &insMsg, hWnd, 0, 0, PM_REMOVE ) )
				{
					TranslateMessage( &insMsg );
					DispatchMessage( &insMsg );
				}
			}
#endif		// _MSC_VER
		}
	}
	return;
}
//
// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki

// >>>>>>	@Date:2014/01/31	<Add>	A.Ozaki
//
void	DialogProgress::Sleep( const DWORD dwMsec )
{
	struct timeb	stTime;

	ftime( &stTime );
	unsigned long	ulTime = stTime.time * 1000 + stTime.millitm;
	unsigned long	ulPassed;
	while ( 1 )
	{
		ftime( &stTime );
		ulPassed = ( stTime.time * 1000 + stTime.millitm ) - ulTime;
		if  ( ulPassed >= (unsigned long)dwMsec )
		{
			break;
		}
	}
}
//
// <<<<<<	@Date:2014/01/31	<Add>	A.Ozaki
