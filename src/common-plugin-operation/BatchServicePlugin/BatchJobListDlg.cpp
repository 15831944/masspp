/**
 * @file BatchJobListDlg.cpp
 * @brief implements of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "BatchJobListDlg.h"
#include "BatchJobListGrid.h"
#include "BatchButtonDef.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE	3
#define GRID_GAP	3

BEGIN_EVENT_TABLE( BatchJobListDlg, kome::window::StandardDialog )
	EVT_BUTTON( ID_BAT_JOB_LIST_CLOSE_BUTTON, BatchJobListDlg::onBatJobListClose )
	EVT_BUTTON( ID_BAT_JOB_LIST_DELETE_BUTTON, BatchJobListDlg::onBatJobListDelete )
END_EVENT_TABLE()


// constructor
BatchJobListDlg::BatchJobListDlg( wxWindow* parent )
	: kome::window::StandardDialog(	parent,	"Batch Job List"), m_parameters( NULL, false )
{
	m_pBatchJobList = new BatchJobListGrid(this);

	m_pBatConfDeleteButton = NULL;

}

// destructor
BatchJobListDlg::~BatchJobListDlg() {
}

wxSizer* BatchJobListDlg::createMainSizer() {

	// field sizer
	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 2, 1, 0, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableRow( 1 );

	// 表
	if( m_pBatchJobList != NULL ) {
		m_pBatchJobList->create();
	}
	fieldSizer->Add(m_pBatchJobList, 1, wxEXPAND);

	// [Delete]ボタン
	fieldSizer->Add(
		createBatchButtonSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);

	// 初期化
	m_pBatchJobList->setDataColor( 1, 0x555555 );

	return fieldSizer;
}


wxSizer* BatchJobListDlg::createButtonSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// default button
	wxButton* button = new wxButton( this, ID_BAT_JOB_LIST_CLOSE_BUTTON, wxT( "Close" ) );
	sizer->Add( button, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT , BORDER_SIZE );

	return sizer;

}

// onBatJobList
void BatchJobListDlg::onBatJobListClose( wxCommandEvent& evt ) {

	int nCnt = m_pBatchJobList->GetSelectedItemCount();
	int nX;
	int nY;
	m_pBatchJobList->GetPosition( &nX, &nY );	// だめ

	Close();

	return;
}


void BatchJobListDlg::onBatJobListDelete( wxCommandEvent& evt ){

	// selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pBatchJobList, items );
	if(items.size() == 0){
		// 選択無し
	}else{
		// 選択されている
		int nSelectedRow = items[0];
		bool bRetVal = kome::window::WindowTool::showYesNo("Are you sure you want to delete the selected batch job?", "Delete", "Don't delete");
		
		if( bRetVal ){
			// 削除処理 nSelectedRow行目を削除（数字は0～）。
		}
	}

}

// create top sizer
wxSizer* BatchJobListDlg::createBatchButtonSizer() {
	// create sizer
	wxSizer* sizer = NULL;
	sizer = new wxBoxSizer(wxHORIZONTAL);

	// get peak pickig algorithms

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	m_pBatConfDeleteButton = new wxBitmapButton( this, ID_BAT_JOB_LIST_DELETE_BUTTON, *iconMgr.getIcon("item_del") );
	m_pBatConfDeleteButton->SetToolTip("Delete job");
	buttonSizer->Add(m_pBatConfDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 


	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	return sizer;
}
