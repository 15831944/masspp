/**
 * @file Wizard.cpp
 * @brief implements of Wizard class
 *
 * @author S.Tanaka
 * @date 2012.10.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Wizard.h"
#include "WindowTool.h"

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3


#define ID_SKIP                       456


BEGIN_EVENT_TABLE( Wizard, wxWizard )
    EVT_BUTTON( ID_SKIP, Wizard::onSkip )
	EVT_UPDATE_UI( ID_SKIP, Wizard::onUpdateSkip )
	EVT_BUTTON( wxID_FORWARD, Wizard::onNextButton )
	EVT_BUTTON( wxID_BACKWARD, Wizard::onPrevButton )
END_EVENT_TABLE()


// constructor
Wizard::Wizard( wxWindow* parent, const char* title, wxBitmap& bitmap, const bool hasSkip )
        : wxWizard( parent, wxID_ANY, wxT( title ), bitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE ) {
    // initialize
	m_skipButton = NULL;
	m_hasSkip = hasSkip;
    insertSkipButton();

	SetAutoLayout( true );
	wxSizer* sizer = GetSizer();
	sizer->Fit( this );
	sizer->SetSizeHints( this );

	m_state = false;
}

// destructor
Wizard::~Wizard() {
}

// inserts skip button
void Wizard::insertSkipButton() {
	// check the member
	if( !m_hasSkip ) {
		return;
	}

	// Cancel Button
	wxWindow* cancelButton = FindWindow( wxID_CANCEL );
	if( cancelButton == NULL ) {
		return;
	}

	// sizer
	wxSizer* sizer = cancelButton->GetContainingSizer();
	if( sizer == NULL ) {
		return;
	}

	// index
	int idx = 0;
	wxSizerItemList& children = sizer->GetChildren();
	for( wxSizerItemList::iterator it = children.begin(); it != children.end(); it++ ) {
		wxSizerItem* item = (*it);
		if( item->GetWindow() != cancelButton ) {
			idx++;
		}
	}

	if( idx >= (int)children.size() ) {
		return;
	}

	// skip button
	m_skipButton = new wxButton( this, ID_SKIP, wxT( "&Skip" ) );
	sizer->Insert( idx, m_skipButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, BORDER_SIZE );
}

// on skip
void Wizard::onSkip( wxWizardPage* currentPage ) {
}

// can skip
bool Wizard::canSkip( wxWizardPage* currentPage ) {
	return false;
}

// on next
void Wizard::onNext( wxWizardPage* currentPage ) {
}

// on prev
void Wizard::onPrev( wxWizardPage* currentPage ) {
}

// on Skip
void Wizard::onSkip( wxCommandEvent& evt ) {
	// check the button
	_ASSERT( evt.GetEventObject() == m_skipButton );

	// on skip
	onSkip( GetCurrentPage() );

	// page
	wxWizardPage* skipPage = GetCurrentPage()->GetNext();
	ShowPage( skipPage, true );
}

// on update Skip
void Wizard::onUpdateSkip( wxUpdateUIEvent& evt ) {
	// page
	bool enabled = canSkip( GetCurrentPage() );

	evt.Enable( enabled );
}

// on next
void Wizard::onNextButton( wxCommandEvent& evt ) {
	// page
	wxWizardPage* page = GetCurrentPage();
	if( page == NULL ){
		return;
	}

	// check
	if( !page->Validate() || !page->TransferDataFromWindow() ) {
		return;
	}

	// on next
	onNext( page );

	// next page
	wxWizardPage* nextPage = page->GetNext();
	// QuantitationのIdentificationパネルのDB初期化処理
	// 初期化が終了していない場合Identificationパネルに移動しないように変更。 @date 2014.07.08 <Mod> M.Izumi ->
	if( !m_state ){
		ShowPage( nextPage, true );
	}else{
		kome::window::WindowTool::showInfo("Still initializing the database.");
	}
	m_state = false;
	// @date 2014.07.08 <Mod> M.Izumi <-
}

// on prev
void Wizard::onPrevButton( wxCommandEvent& evt ) {
	// page
	wxWizardPage* page = GetCurrentPage();
	if( page == NULL ){
		return;
	}

	// on next
	onPrev( page );

	// next page
	wxWizardPage* prevPage = page->GetPrev();
	ShowPage( prevPage, false );
}

// @date 2014.07.08 <Add> M.Izumi ->
// set thread condition
void Wizard::setThreadCondition( bool bflg ) {
	m_state = bflg;
}

// get thread condition
bool Wizard::getThreadCondition() {
	return	m_state;
}
// @date 2014.07.08 <Add> M.Izumi <-
