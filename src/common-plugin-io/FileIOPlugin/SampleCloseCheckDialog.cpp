
/**
 * @file SampleCloseCheckDialog.cpp
 * @brief implements of SampleCloseCheckDialog class
 *
 * @author M.Izumi
 * @date 2010.11.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SampleCloseCheckDialog.h"

using namespace kome::io;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_CHECKLISTBOX		5010

BEGIN_EVENT_TABLE( SampleCloseCheckDialog, kome::window::StandardDialog )
	EVT_CHECKLISTBOX(ID_CHECKLISTBOX, SampleCloseCheckDialog::onCheck)
END_EVENT_TABLE()


// constructor 
SampleCloseCheckDialog::SampleCloseCheckDialog( wxWindow* parent ) 
		: kome::window::StandardDialog( parent, "Close", "Close" ){
	m_chkList = NULL;
}

// destructor
SampleCloseCheckDialog::~SampleCloseCheckDialog(){
}

wxSizer* SampleCloseCheckDialog::createMainSizer(){
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 1, 2, 5 ); 
    mainSizer->AddGrowableRow( 0 );
    mainSizer->AddGrowableCol( 0 );

 	// list sizer
	wxFlexGridSizer* listSizer = new wxFlexGridSizer( 2, 1, 5 );
	listSizer->AddGrowableRow( 0 );
	listSizer->AddGrowableCol( 1 );

	// instruments
	std::vector< wxString > instruments;
	for( unsigned int i = 0; i < m_ChkListInfos.size(); i++ ) {
		instruments.push_back( m_ChkListInfos[i].strName.c_str() );
	}

	// check list box
	m_chkList = new wxCheckListBox(
		this,
		ID_CHECKLISTBOX,
		wxDefaultPosition,
		wxDefaultSize,
		instruments.size(),
		instruments.size() == 0 ? NULL : &( instruments[ 0 ] ),
		wxLB_SINGLE | wxLB_HSCROLL | wxLB_NEEDED_SB
	);
	listSizer->Add(m_chkList, 1, wxALL | wxGROW, 5 );
	mainSizer->Add( listSizer, 1, wxALL | wxGROW, 5 );

    return mainSizer;
}


// ”z—ñ -> ListBox
bool SampleCloseCheckDialog::TransferDataToWindow() {
    // ListBox ‚É’Ç‰Á
 	m_chkList->Clear();

    for( unsigned int i = 0; i < m_ChkListInfos.size(); i++ ) {
		m_chkList->Append( m_ChkListInfos[i].strName.c_str() );
		m_chkList->Check(i,m_ChkListInfos[i].bCheck);

    }

    return true;
}

// set check list info
void SampleCloseCheckDialog::setChkListInfo(std::string strName, bool bChke){
	ChekListBoxInfo info = ChekListBoxInfo();
	info.strName = strName;
	info.bCheck = bChke;
	m_ChkListInfos.push_back( info );
}

// is checked
bool SampleCloseCheckDialog::isChecked(const unsigned int index){
	if( index >= m_ChkListInfos.size()){
		return false;
	}
	bool bflg = m_ChkListInfos[index].bCheck;

	return bflg;
}

// on chek
void SampleCloseCheckDialog::onCheck(wxCommandEvent& evt){
	int i = evt.GetSelection();
	bool bflg = m_chkList->IsChecked(i);
	m_ChkListInfos[i].bCheck = bflg;
}
