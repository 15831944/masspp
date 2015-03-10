/**
 * @file ScriptListCtrl.cpp
 * @brief implementation of ScriptListCtrl class
 *
 * @author A.Ozaki
 * @date 2013.08.05
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ScriptListCtrl.h"


using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define	COLUMN_NAME				( 0 )
#define	COLUMN_ENGINE			( 1 )
#define	COLUMN_EVENT			( 2 )
#define	COLUMN_PATH				( 3 )


BEGIN_EVENT_TABLE( ScriptListCtrl, kome::window::GridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, ScriptListCtrl::OnSelect )
END_EVENT_TABLE( )


// constructor
ScriptListCtrl::ScriptListCtrl( wxWindow* pParent, const int width, const int height )
		: kome::window::GridListCtrl( pParent, wxID_ANY, width, height, false, true, true )
{
	m_vecScriptInfo.clear( );

	m_nSelectedIndex = -1;
	
	m_pParent = pParent;

	addColumn( "Name", TYPE_STRING );
	addColumn( "Engine", TYPE_STRING );
	addColumn( "Event", TYPE_STRING );
	addColumn( "Path", TYPE_STRING );
	
}

// destructor
ScriptListCtrl::~ScriptListCtrl( )
{
	return;
}

// get the number of spectra
unsigned int	ScriptListCtrl::getNumberOfScripts( void )
{
	return	m_vecScriptInfo.size( );
}

// get stScriptInfo
stScriptInfo	ScriptListCtrl::getScript( const unsigned int idx )
{
	return	m_vecScriptInfo[ idx ];
}

// set stScriptInfo
void	ScriptListCtrl::setScript( stScriptInfo insScriptInfo, const unsigned int idx )
{		
	std::vector< stScriptInfo > infos;
	
	infos = m_vecScriptInfo;
	
	clearScripts();

	for( unsigned int i=0; i < infos.size(); i++ ){
		stScriptInfo insInfo = infos[i];
		if( idx == i ){
			insInfo = insScriptInfo;
		}
		addScriptInfo( insInfo, false );
	}	
	
	updateList();
}

// clear spectra
void	ScriptListCtrl::clearScripts( void )
{
	m_vecScriptInfo.clear( );
	clearData( false );

	return;
}

// add script info
void	ScriptListCtrl::addScriptInfo( stScriptInfo insScriptInfo, const bool bUpdate )
{
	addData( m_vecScriptInfo.size( ), bUpdate );
	m_vecScriptInfo.push_back( insScriptInfo );

	return;
}

// delete script info
void	ScriptListCtrl::deleteScriptInfo(  const long long data, const bool bUpdate )
{
// >>>>>>	SPEC94872	@Date:2013/09/11	<Modified>	A.Ozaki
//
	std::vector< stScriptInfo > infos;
	
	infos = m_vecScriptInfo;
	
	clearScripts();

	for( unsigned int i=0; i < infos.size(); i++ ){
		stScriptInfo insInfo = infos[i];
		if( data != i ){
			//insInfo = insScriptInfo;
			addScriptInfo( insInfo, false );
		}
		//addScriptInfo( insInfo, false );
	}	

	updateList();
//
// <<<<<<	SPEC94872	@Date:2013/09/11	<Modified>	A.Ozaki		
	return;
}

// on create
void	ScriptListCtrl::onCreate( )
{
	// add data
	for ( unsigned int i = 0 ; i < m_vecScriptInfo.size( ) ; i++ )
	{
		addData( i, false );
	}
}

// get double value
double	ScriptListCtrl::getDouble( const long long data, const int column ) const
{
	// get value
	double	dVal = double( );

	return	dVal;
}

// get int value
int	ScriptListCtrl::getInt( const long long data, const int column ) const
{
	// get value
	int	nVal = int( );

	return	nVal;
}

// get string value
std::string	ScriptListCtrl::getString( const long long data, const int column ) const
{
	// string
	std::string	strTemp;

	strTemp = "";
	switch ( column )
	{
	case  COLUMN_NAME:
		strTemp = m_vecScriptInfo[data].strName;
		break;
	case  COLUMN_ENGINE:
		strTemp = m_vecScriptInfo[data].strEngine;
		break;
	case  COLUMN_EVENT:
		strTemp = m_vecScriptInfo[data].strEvent;
		break;
	case  COLUMN_PATH:
		strTemp = m_vecScriptInfo[data].strPath;
		break;
	default:
		break;
	}

	return	strTemp;
}

// get selected index
int	ScriptListCtrl::getSelectedIndex( void )
{
	return	m_nSelectedIndex;
}


// ƒŠƒXƒg‘I‘ðŽž‚Ìˆ—
void	ScriptListCtrl::OnSelect( wxListEvent& evt )
{
	OnSelect( );

	return;
}

// on select script
void	ScriptListCtrl::OnSelect( void )
{
	
	int index = -1;
	for( int i = 0; i < GetItemCount(); i++ ) {
		if( GetItemState( i, wxLIST_STATE_SELECTED ) > 0 ) {
			index = i;
		}
	}
	
	m_nSelectedIndex = index;

	return;
}

// on size
void	ScriptListCtrl::onSizeScriptCtrl( wxSizeEvent& evt )
{
	onSize( evt );

	return;
}


