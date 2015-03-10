/**
 * @file GroupSettingsGridCtrl.cpp
 * @brief interfaces of GroupSettingsGridCtrl class
 * 
 * @author M.Izumi
 * @date 2012.12.07
 *
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "GroupSettingsPanel.h"
#include "GroupSettingsGridCtrl.h"

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_ADD_BUTTON                     66
#define ID_EDIT_BUTTON                    67
#define ID_DEL_BUTTON                     68
#define ID_UP_BUTTON                      69
#define ID_DOWN_BUTTON                    70

BEGIN_EVENT_TABLE( GroupsGridCtrl, kome::window::EditGridListCtrl )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, GroupsGridCtrl::onCategoryDoubleClicked )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, GroupsGridCtrl::onSelectCategoryItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, GroupsGridCtrl::onDeselectCategoryItem )
	EVT_MENU( ID_UP_BUTTON, GroupsGridCtrl::onUpCategory )
	EVT_MENU( ID_DOWN_BUTTON, GroupsGridCtrl::onDownCategory )
	EVT_MENU( ID_ADD_BUTTON,  GroupsGridCtrl::onAddCategory )
	EVT_MENU( ID_EDIT_BUTTON, GroupsGridCtrl::onEditCategory )
END_EVENT_TABLE()

// constructor
GroupsGridCtrl::GroupsGridCtrl( GroupSettingsPanel* pParent )
	: kome::window::EditGridListCtrl( (wxWindow*)pParent, false ), m_pParent( pParent ){

	clearData( true );	

	// Context Menu
	setAddMenuName( "New" );
	setEditMenuName( "Edit" );
	setUpDown( true );
	setPopupMenuFlag( POPUP_EDIT | POPUP_ADD | POPUP_DELETE  );
}

// destructor
GroupsGridCtrl::~GroupsGridCtrl(){
}

// active data
void GroupsGridCtrl::onActivateData( const long long data ){
}

// delete data
bool GroupsGridCtrl::onDeleteData( const long long data ){
	wxCommandEvent evtDummy;
	if( m_pParent != NULL ){
		CategoryInfo info = m_pParent->getGategoryInfo( data );
		m_pParent->SelectCategoryItem( info.name.c_str() );
		m_pParent->onDeleteCategory( evtDummy );
	}
	return false;
}

// get string
std::string GroupsGridCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;
	
	CategoryInfo info = m_pParent->getGategoryInfo( data );
	s = info.name;

	return s;
}

// select
void GroupsGridCtrl::onSelectCategoryItem( wxListEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onSelectCategoryItem( evt );
}

// deselect 
void GroupsGridCtrl::onDeselectCategoryItem( wxListEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onDeselectCategoryItem( evt );
}

// add 
void GroupsGridCtrl::onAddCategory( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	
	m_pParent->onNewCategory( evt );
}

// edit 
void GroupsGridCtrl::onEditCategory( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}

	m_pParent->onEditCategory( evt );
}

// up 
void GroupsGridCtrl::onUpCategory( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onUpCategory( evt );
}

// down 
void GroupsGridCtrl::onDownCategory( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onDownCategory( evt );
}

// double click
void GroupsGridCtrl::onCategoryDoubleClicked( wxListEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}

	m_pParent->onCategoryDoubleClicked( evt );
}
// ---------------------------------------------------------------------------
// Samples grid ctrl class
// ---------------------------------------------------------------------------
BEGIN_EVENT_TABLE( SamplesGridCtrl, kome::window::EditGridListCtrl )
	
	EVT_LIST_ITEM_SELECTED( wxID_ANY, SamplesGridCtrl::onSelectSampleItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, SamplesGridCtrl::onDeselectSampleItem )
	EVT_MENU( ID_ADD_BUTTON,  SamplesGridCtrl::onAddSample )
	EVT_MENU( ID_DEL_BUTTON, SamplesGridCtrl::onDeleteSample )
	EVT_MENU( ID_UP_BUTTON, SamplesGridCtrl::onUpSample )
	EVT_MENU( ID_DOWN_BUTTON, SamplesGridCtrl::onDownSample )
END_EVENT_TABLE()

// constructor
SamplesGridCtrl::SamplesGridCtrl( GroupSettingsPanel* pParent )
	: kome::window::EditGridListCtrl( (wxWindow*)pParent, false ), m_pParent( pParent ){

	clearData( true );
	
	// Context Menu
	setAddMenuName( "Add" );
	setActivateName( "Color" );
	setUpDown( true );
	setPopupMenuFlag( POPUP_EDIT | POPUP_ADD | POPUP_DELETE  );

}
			
// destructor
SamplesGridCtrl::~SamplesGridCtrl(){
}

// active date
void SamplesGridCtrl::onActivateData( const long long data ){
	wxCommandEvent evtDummy;
	if( m_pParent != NULL ){
		m_pParent->onColorSample( evtDummy );
	}

}

// delete data
bool SamplesGridCtrl::onDeleteData( const long long data ){
	wxCommandEvent evtDummy;
	if( m_pParent != NULL ){
		kome::window::ListTool::selectItem( this, data );
		m_pParent->onDeleteSample( evtDummy );
	}
	return false;
}

// get string
std::string SamplesGridCtrl::getString( const long long data, const int column ) const {
	
	// string
	std::string s;
	if( m_pParent == NULL ){
		return s;
	}

	CategoryInfo* info = m_pParent->getSelectedCategoryInfo();
	SampleInfo currentSample;
	if( info != NULL ){
		
		for( unsigned int i = 0; i < info->paths.size(); i++ ){
			currentSample = info->paths[i];
			if( data == i ){
				return currentSample.path;
			}
		}
	}else{
		info = m_pParent->getSelectGroup();
		if( info != NULL ){
			
			for( unsigned int i = 0; i < info->paths.size(); i++ ){
				currentSample = info->paths[i];
				if( data == i ){
					return currentSample.path;
				}
			}
		}
	}

	return s;
}

// select
void SamplesGridCtrl::onSelectSampleItem( wxListEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onSelectSampleItem( evt );
}

// deselect
void SamplesGridCtrl::onDeselectSampleItem( wxListEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onDeselectSampleItem( evt );
}

// add
void SamplesGridCtrl::onAddSample( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onAddSample( evt );
}

// delete
void SamplesGridCtrl::onDeleteSample( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onDeleteSample( evt );
}

// up
void SamplesGridCtrl::onUpSample( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onUpSample( evt );
}

// down
void SamplesGridCtrl::onDownSample( wxCommandEvent& evt ){
	if( m_pParent == NULL ){
		return;
	}
	m_pParent->onDownSample( evt );
}
