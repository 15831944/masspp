/**
 * @file LogView.cpp
 * @brief implements of LogView class
 *
 * @author S.Tanaka
 * @date 2008.07.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LogView.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




// constructor
LogView::LogView( wxWindow* parent ) : wxTextCtrl(
		parent,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 400, 200 ),
		wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL
) {
	m_bDestructed = false;		// @Date:2013/09/24	<Add>	A.Ozaki
}


// destrictor
LogView::~LogView() {
	m_bDestructed = true;		// @Date:2013/09/24	<Add>	A.Ozaki
}

// add log message
void LogView::addLog( const char* msg ) {
#pragma omp critical ( writeLog )
	if  ( false == m_bDestructed )	//	@Date:2013/09/24	<Add>	A.Ozaki
	{
		// add to message list
		if( msg != NULL ) {
			m_msgList.push_back( FMT( "%s\n", msg ) );
		}

		// append message
		if( wxIsMainThread() ) {		// main thread
			while( m_msgList.size() > 0 ) {
				AppendText( m_msgList.front().c_str() );
				m_msgList.pop_front();
				SetInsertionPointEnd();
			}
		}
	}
}
