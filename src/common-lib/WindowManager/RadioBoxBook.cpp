/**
 * @file RadioBoxBook.cpp
 * @brief implements of RadioBoxBook class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "RadioBoxBook.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE			3
#define GRID_GAP			3


BEGIN_EVENT_TABLE( RadioBoxBook, wxBookCtrlBase )
    EVT_RADIOBOX( wxID_ANY, RadioBoxBook::onRadioBoxSelected )
END_EVENT_TABLE()


// constructor
RadioBoxBook::RadioBoxBook() {
	init();
}

// constructor
RadioBoxBook::RadioBoxBook( wxWindow* parent, const int id, wxPoint pos, wxSize size ) {
	init();
	Create( parent, id, pos, size );
}

// destructor
RadioBoxBook::~RadioBoxBook() {
}

// initialize
void RadioBoxBook::init() {
	m_selection = wxNOT_FOUND;
	m_bookctrl = NULL;
}

// get radio box
wxRadioBox* RadioBoxBook::getRadioBox() const {
	return (wxRadioBox*)m_bookctrl;
}

// create radio box
void RadioBoxBook::createRadioBox() {
	// page names
	std::vector< wxString > pages;
	for( unsigned int i = 0; i < m_pages.size(); i++ ) {
		pages.push_back( wxT( m_pages[ i ].c_str() ) );
	}

	if( pages.empty() ) {
		pages.push_back( wxT( "No pages." ) );
	}

	// delete old radio box
	wxRadioBox* radiobox = getRadioBox();
	if( radiobox != NULL ) {
		m_controlSizer->Detach( radiobox );
		radiobox->Destroy();
	}

	// new radiobox
	radiobox = new wxRadioBox(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 350, -1 ),
		pages.size(),
		( pages.size() == 0 ? NULL : &( pages[ 0 ] ) ),
		1,
		wxRA_SPECIFY_ROWS
	);
	m_controlSizer->Add( radiobox, 1, wxALIGN_CENTER_VERTICAL );
	m_bookctrl = radiobox;

	// selection
	if( m_selection != wxNOT_FOUND && m_selection >= 0 && m_selection < (int)radiobox->GetCount() ) {
		radiobox->SetSelection( m_selection );
	}
}

// create
bool RadioBoxBook::Create( wxWindow* parent, const int id, wxPoint pos, wxSize size ) {
	// default
	if( !wxBookCtrlBase::Create( parent, id, pos, size, wxBK_TOP | wxBORDER_NONE ) ) {
		return false;
	}

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	m_controlSizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( m_controlSizer, 0, wxGROW );

	SetSizer( sizer );

	// radiobox
	createRadioBox();

	// update
	int sel = GetSelection();
	for( int i = (int)GetPageCount() - 1; i >= 0; i-- ) {
		ChangeSelection( i );
	}
	if( sel >= 0 ) {
		SetSelection( sel );
	}

	return true;
}

// get selection
int RadioBoxBook::GetSelection() const {
	return m_selection;
}

// set page text
bool RadioBoxBook::SetPageText( size_t page, const wxString& text ) {
	// radio box
	wxRadioBox* radiobox = getRadioBox();

	// set page
	radiobox->SetString( page, text );
	return true;
}

// get page text
wxString RadioBoxBook::GetPageText( size_t page ) const {
	// radio box
	wxRadioBox* radiobox = getRadioBox();

	// get page
	return radiobox->GetString( page );
}

// set page image
bool RadioBoxBook::SetPageImage( size_t page, int image ) {
	return false;
}

// get page image
int RadioBoxBook::GetPageImage( size_t page ) const {
	return wxNOT_FOUND;
}

// calculate size from page
wxSize RadioBoxBook::CalcSizeFromPage(const wxSize& sizePage) const {
	// min width
	int minWidth = 8;
	for( unsigned int i = 0; i < m_pages.size(); i++ ) {
		minWidth += (int)( m_pages[ i ].length() + 2 ) * 8;
	}

	// size elements
	wxSize clientSize = GetClientSize();
	wxSize radioSize = m_controlSizer->CalcMin();

	// calculation
	wxSize size = sizePage;
	if( clientSize.x > size.x ) {
		size.x = clientSize.x;
	}
	if( minWidth > size.x ) {
		size.x = minWidth;
	}

	size.y = radioSize.y + sizePage.y + GetInternalBorder();

	return size;
}

// inserts page
bool RadioBoxBook::InsertPage( size_t index, wxWindow* page, const wxString& text, bool select, int imageId ) {
	// default
	if( !wxBookCtrlBase::InsertPage( index, page, text, select, imageId ) ) {
		return false;
	}

	// insert to the array
	m_pages.insert( m_pages.begin() + index, text.c_str() );

	// selection
	if( select || m_selection == wxNOT_FOUND ) {
		m_selection = (int)index;
	}
	else {
		if( (int)index <= m_selection ) {
			m_selection = m_selection + 1;
		}
	}
	int sel = m_selection;

	// creates radio box
	createRadioBox();

	// selecting
	SetSelection( index );
	SetSelection( sel );

	return true;
}

// set selection
int RadioBoxBook::SetSelection( size_t page ) {
	return DoSetSelection( page, SetSelection_SendEvent );
}

// change selection
int RadioBoxBook::ChangeSelection( size_t page ) {
	return DoSetSelection( page );
}

// delete all pages
bool RadioBoxBook::DeleteAllPages() {
	// default
	if( !wxBookCtrlBase::DeleteAllPages() ) {
		return false;
	}

	// clear radiobox
	m_selection = wxNOT_FOUND;
	m_pages.clear();
	createRadioBox();

	return true;
}

// update selected page
void RadioBoxBook::UpdateSelectedPage( size_t newsel ) {
	m_selection = (int)newsel;
	getRadioBox()->Select( m_selection );
}

// create event
wxBookCtrlBaseEvent* RadioBoxBook::CreatePageChangingEvent() const {
	return new wxBookCtrlBaseEvent( wxEVT_COMMAND_RADIOBOX_SELECTED, m_windowId );
}

// make changed event
void RadioBoxBook::MakeChangedEvent(wxBookCtrlBaseEvent& evt ) {
	evt.SetEventType( wxEVT_COMMAND_RADIOBOX_SELECTED );
}

// do remove page
wxWindow* RadioBoxBook::DoRemovePage( size_t page ) {
	// default
	size_t num = GetPageCount();
	wxWindow* win = wxBookCtrlBase::DoRemovePage( page );
	if( win != NULL ) {
		// radio box

		m_pages.erase( m_pages.begin() + page );
		if( page == m_selection ) {
			m_selection = wxNOT_FOUND;
		}
		else if( (int)page < m_selection ) {
			m_selection = m_selection - 1;
		}

		createRadioBox();
	}

	return win;
}

// transfer data from window
bool RadioBoxBook::TransferDataFromWindow() {
	return wxBookCtrlBase::TransferDataFromWindow();
}

// validate
bool RadioBoxBook::Validate() {
	// selected page
	int sel = GetSelection();
	wxWindow* wnd = NULL;
	if( sel >= 0 && sel < (int)GetPageCount() ) {
		wnd = GetPage( sel );
	}

	// validate
	bool ret = true;
	if( wnd != NULL ) {
		ret = wnd->Validate();
	}

	return ret;
}

// on radiobox selected
void RadioBoxBook::onRadioBoxSelected( wxCommandEvent& evt ) {
	// check the object
	if( evt.GetEventObject() != m_bookctrl ) {
		evt.Skip();
		return;
	}

	// selection
	int sel = getRadioBox()->GetSelection();
	if( sel != m_selection ) {
		SetSelection( sel );
	}

	if( sel != m_selection ) {
		m_selection = sel;
		getRadioBox()->SetSelection ( sel );
	}

	Refresh();
}
