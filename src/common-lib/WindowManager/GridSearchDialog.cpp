/**
 * @file GridSearchDialog.cpp
 * @brief implements of GridSearchCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GridSearchDialog.h"

#include "GridListCtrl.h"

#include <wx/notebook.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP				 3
#define BORDER_SIZE			  3


BEGIN_EVENT_TABLE( GridSearchDialog, StandardDialog )
END_EVENT_TABLE()


// constructor
GridSearchDialog::GridSearchDialog( wxWindow* parent ) : StandardDialog( parent, "Filter" ) {
	// initialize
	m_grid = dynamic_cast< GridListCtrl* >( parent );
	m_searchType = SEARCH_NEW;
	m_valueType = VALUE_KEYWORD;
	m_col = -1;

	m_searchBox = NULL;
	m_colBox = NULL;
	m_keywordText = NULL;
	m_minValText = NULL;
	m_maxValText = NULL;
}

// destructor
GridSearchDialog::~GridSearchDialog() {
}

// set search type
void GridSearchDialog::setSearchType( const SearchType type ) {
	m_searchType = type;
}

// get search type
GridSearchDialog::SearchType GridSearchDialog::getSearchType() {
	return m_searchType;
}

// set column
void GridSearchDialog::setColumn( const int col ) {
	m_col = col;
	m_col = std::max( -1, m_col );
	m_col = std::min( m_col, ( m_grid == NULL ? -1 : m_grid->GetColumnCount() - 1 ) );
}

// get columnn
int GridSearchDialog::getColumn() {
	return m_col;
}

// set value type
void GridSearchDialog::setValueType( const ValueType type ) {
	m_valueType = type;
}

// get value type
GridSearchDialog::ValueType GridSearchDialog::getValueType() {
	return m_valueType;
}

// set keyword
void GridSearchDialog::setKeyword( const char* word ) {
	m_keyword = trimstring( NVL( word, "" ) );
}

// get keyword
const char* GridSearchDialog::getKeyword() {
	return m_keyword.c_str();
}

// set value range
void GridSearchDialog::setValueRange( const char* minVal, const char* maxVal ) {
	m_minVal = trimstring( NVL( minVal, "" ) );
	m_maxVal = trimstring( NVL( maxVal, "" ) );
}

// get minimum value
const char* GridSearchDialog::getMinValue() {
	return m_minVal.c_str();
}

// get maximum value
const char* GridSearchDialog::getMaxValue() {
	return m_maxVal.c_str();
}

// create main sizer
wxSizer* GridSearchDialog::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableCol( 2 );

	// radio box
	wxString searches[] = {
		"New",
		"Narrow",
		"Append"
	};
	m_searchBox = new wxRadioBox(
		this,
		wxID_ANY,
		wxT( "Search Type" ),
		wxDefaultPosition,
		wxDefaultSize,
		sizeof( searches ) / sizeof( wxString ),
		searches,
		sizeof( searches ) / sizeof( wxString ),
		wxRA_SPECIFY_COLS
	 );

	sizer->Add( m_searchBox, 1, wxALL | wxGROW, BORDER_SIZE );

	// combobox
	std::vector< wxString > columns;
	columns.push_back( wxT( "All" ) );
	if( m_grid != NULL ) {
		for( int i = 0; i < m_grid->GetColumnCount(); i++ ) {
			columns.push_back( wxT( m_grid->getHeaderTitle( i ) ) );
		}
	}

	m_colBox = new wxComboBox(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		columns.size(),
		&( columns[ 0 ] ),
		wxCB_SIMPLE | wxCB_READONLY
	);

	sizer->Add( m_colBox, 1, wxALL | wxGROW, BORDER_SIZE );

	// notebook
	m_note = new wxNotebook(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxNB_TOP
	);

	// panel 0 (keyword)
	wxPanel* panel0 = new wxPanel( m_note );
	panel0->SetAutoLayout( true );

	wxSizer* keySizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	m_keywordText = new wxTextCtrl(
		panel0,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);

	keySizer->Add( m_keywordText, 1, wxALL | wxGROW, BORDER_SIZE );

	panel0->SetSizer( keySizer );
	keySizer->Fit( panel0 );
	keySizer->SetSizeHints( panel0 );

	m_note->AddPage( panel0, wxT( "Keyword" ), true );

	// panel 1 (range)
	wxPanel* panel1 = new wxPanel( m_note );
	panel1->SetAutoLayout( true );

	wxFlexGridSizer* rangeSizer = new wxFlexGridSizer( 1, 3, GRID_GAP, GRID_GAP );
	rangeSizer->AddGrowableRow( 0 );
	rangeSizer->AddGrowableCol( 0 );
	rangeSizer->AddGrowableCol( 2 );

	m_minValText = new wxTextCtrl( 
		panel1,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);

	rangeSizer->Add( m_minValText, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	rangeSizer->Add(
		new wxStaticText( panel1, wxID_ANY, wxT( "-" ) ),
		0,
		wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_maxValText = new wxTextCtrl(
		panel1,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);

	rangeSizer->Add( m_maxValText, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	panel1->SetSizer( rangeSizer );
	rangeSizer->Fit( panel1 );
	rangeSizer->SetSizeHints( panel1 );

	m_note->AddPage( panel1, wxT( "Range" ), true );

	sizer->Add( m_note, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// transfer data to window
bool GridSearchDialog::TransferDataToWindow() {
	// search type
	if( m_searchBox != NULL ) {
		int sel = 0;
		if( m_searchType == SEARCH_NARROW ) {
			sel = 1;
		}
		if( m_searchType == SEARCH_APPEND ) {
			sel = 2;
		}
		m_searchBox->SetSelection( sel );
	}

	// column
	if( m_colBox != NULL && m_grid != NULL ) {
		int sel = m_col < 0 ? 1 : m_col + 1;
		sel = CLAMP( sel, 0, m_grid->GetColumnCount() );

		m_colBox->SetSelection( sel );
	}

	// value mode
	if( m_note != NULL ) {
		m_note->SetSelection( m_valueType == VALUE_RANGE ? 1 : 0 );
	}

	// keyword
	if( m_keywordText != NULL ) {
		m_keywordText->SetValue( m_keyword.c_str() );
	}

	// range
	if( m_minValText != NULL ) {
		m_minValText->SetValue( m_minVal.c_str() );
	}
	if( m_maxValText != NULL ) {
		m_maxValText->SetValue( m_maxVal.c_str() );
	}

	return true;
}

// transfer data from window
bool GridSearchDialog::TransferDataFromWindow() {
	// search type
	if( m_searchBox != NULL ) {
		int sel = m_searchBox->GetSelection();
		if( sel == 1 ) {
			m_searchType = SEARCH_NARROW;
		}
		else if( sel == 2 ) {
			m_searchType = SEARCH_APPEND;
		}
		else {
			m_searchType = SEARCH_NEW;
		}
	}

	// column
	if( m_colBox != NULL ) {
		int sel = m_colBox->GetSelection();
		setColumn( sel - 1 );
	}

	// value type
	if( m_note != NULL ) {
		int sel = m_note->GetSelection();
		if( sel == 1 ) {
			m_valueType = VALUE_RANGE;
		}
		else {
			m_valueType = VALUE_KEYWORD;
		}
	}

	// keyword
	if( m_keywordText != NULL ) {
		setKeyword( m_keywordText->GetValue().c_str() );
	}

	// range
	std::string minVal;
	std::string maxVal;

	if( m_minValText != NULL ) {
		minVal = m_minValText->GetValue().c_str();
	}
	if( m_maxValText != NULL ) {
		maxVal = m_maxValText->GetValue().c_str();
	}

	setValueRange( minVal.c_str(), maxVal.c_str() );

	return true;
}
