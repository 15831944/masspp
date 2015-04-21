/**
 * @file InputDialog.cpp
 * @brief implements of InputDialog class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "InputDialog.h"

#include <wx/spinctrl.h>
#include <boost/bind.hpp>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE			3
#define GRID_GAP			   3
#define INTEGER_EDIT_WIDTH   100
#define NUMBER_EDIT_WIDTH	100
#define STRING_EDIT_WIDTH	150


BEGIN_EVENT_TABLE( InputDialog, wxDialog )
	EVT_INIT_DIALOG( InputDialog::onInitDialog )
END_EVENT_TABLE()


// constructor
InputDialog::InputDialog( wxWindow* parent, const char* title ) 
		: wxDialog(
			parent,
			wxID_ANY,
			wxT( NVL( title, "" ) ),
			wxDefaultPosition,
			wxDefaultSize ) {
}

// destructor
InputDialog::~InputDialog() {
}

// set message
void InputDialog::setMessage( const char* message ) {
	m_message = NVL( message, "" );
}

// get message
const char* InputDialog::getMessage() {
	return m_message.c_str();
}

// add input
void InputDialog::addInput(
		const char* name,
		DataType type,
		const char* defaultValue,
		kome::core::NumberRestriction* restriction
) {
	// add to array
	m_inputs.resize( m_inputs.size() + 1 );

	// set element
	m_inputs.back().id = m_inputs.size();
	m_inputs.back().name = NVL( name, "" );
	m_inputs.back().type = type;
	m_inputs.back().value = NVL( defaultValue, "" );
	if( restriction != NULL ) {
		m_inputs.back().restriction = *restriction;
	}
}

// get the number of inputs
unsigned int InputDialog::getNumberOfInputs() {
	return m_inputs.size();
}

// get input name
const char* InputDialog::getInputName( const int index ) {
	if( index < 0 || index >= (int)m_inputs.size() ) {
		return NULL;
	}
	return m_inputs[ index ].name.c_str();
}

// get input value
const char* InputDialog::getInputValue( const int index ) {
	if( index < 0 || index >= (int)m_inputs.size() ) {
		return NULL;
	}
	return m_inputs[ index ].value.c_str();
}

// get boolean value
bool InputDialog::getBoolValue( const int index, const bool dfVal ) {
	bool v = tobool( getInputValue( index ), dfVal );
	return v;
}

// get double value
double InputDialog::getDoubleValue( const int index, const double dfVal ) {
	double v = double();
	if( isdouble( getInputValue( index ), &v ) ) {
		return v;
	}
	return dfVal;
}

// get int value
double InputDialog::getIntValue( const int index, const int dfVal ) {
	int v = int();
	if( isint( getInputValue( index ), 10, &v ) ) {
		return v;
	}
	return dfVal;
}

// create controls
void InputDialog::create() {
	// Auto Layout
	SetAutoLayout( true );

	// create top sizer
	wxSizer* topSizer = createTopSizer();

	// Fit
	SetSizer( topSizer );
	topSizer->Fit( this );
	topSizer->SetSizeHints( this );	
}

// create top sizer
wxSizer* InputDialog::createTopSizer() {
	// top sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( ( m_message.empty() ? 2 : 3 ), 1, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );
	topSizer->AddGrowableRow( m_message.empty() ? 0 : 1 );

	// message
	if( !m_message.empty() ) {
		topSizer->Add(
			new wxStaticText( this, wxID_ANY, wxT( m_message.c_str() ) ),
			0,
			wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
			BORDER_SIZE
		);
	}

	// input sizer
	wxSizer* inputSizer = createInputSizer();
	topSizer->Add( inputSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// button sizer
	wxSizer* buttonSizer = CreateButtonSizer( wxOK | wxCANCEL );
	topSizer->Add( buttonSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return topSizer;
}

// create input sizer
wxSizer* InputDialog::createInputSizer() {
	// create sizer
	wxFlexGridSizer* inputSizer = new wxFlexGridSizer( MAX( m_inputs.size(), 1 ), 2, GRID_GAP, GRID_GAP );

	// growable
	inputSizer->AddGrowableCol( 1 );
	for( unsigned int i = 0; i < m_inputs.size(); i++ ) {
		inputSizer->AddGrowableRow( i );
	}

	// add input
	for( unsigned int i = 0; i < m_inputs.size(); i++ ) {
		// information
		InputInfo& info = m_inputs[ i ];

		// add name
		inputSizer->Add(
			new wxStaticText(
				this,
				wxID_ANY,
				wxT( info.type == TYPE_BOOL ? "" : info.name.c_str() )
			),
			0,
			wxALIGN_CENTER_VERTICAL,
			BORDER_SIZE
		);

		// add input
		if( info.type == TYPE_BOOL ) {		        // boolean
			addBoolInput( inputSizer, info );
		}
		else if( info.type == TYPE_INT ) {		    // int
			addIntInput( inputSizer, info );
		}
		else if( info.type == TYPE_DOUBLE ) {		// double
			addDoubleInput( inputSizer, info );
		}
		else if( info.type == TYPE_STRING ) {		// string
			addStringInput( inputSizer, info );
		}
		else if( info.type == TYPE_SELECTION ) {	// selection
			addSelectInput( inputSizer, info );
		}
	}

	return inputSizer;
}

// add boolean input
void InputDialog::addBoolInput( wxSizer* sizer, InputInfo& info ) {
	// radio
	wxCheckBox* check = new wxCheckBox(
		this,
		info.id,
		wxT( info.name.c_str() )
	);

	// add radio
	sizer->Add( check, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// add int input
void InputDialog::addIntInput( wxSizer* sizer, InputInfo& info ) {
	// get range
	int minVal = roundnum( info.restriction.getLowerBound() );
	if( info.restriction.hasExclusiveMinValue() ) {
		if( roundnum( info.restriction.getExclusiveMinValue() ) == minVal ) {
			minVal++;
		}
		else if( !info.restriction.hasInclusiveMinValue() ) {
			minVal = INT_MIN;
		}
	}

	int maxVal = roundnum( info.restriction.getUpperBound() );
	if( info.restriction.hasExclusiveMaxValue() ) {
		if( roundnum( info.restriction.getExclusiveMaxValue() ) == maxVal ) {
			maxVal--;
		}
	}
	else if( !info.restriction.hasInclusiveMaxValue() ) {
		maxVal = INT_MAX;		
	}

	// spin
	wxSpinCtrl* spin = new wxSpinCtrl(
		this,
		info.id,
		wxT( info.value.c_str() ),
		wxDefaultPosition,
		wxSize( INTEGER_EDIT_WIDTH, -1 ),
		wxSP_ARROW_KEYS,
		minVal,
		maxVal,
		toint( info.value.c_str(), 10, minVal )
	);

	// add spin
	sizer->Add( spin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// add double input
void InputDialog::addDoubleInput( wxSizer* sizer, InputInfo& info ) {
	// text
	wxTextCtrl* text = new wxTextCtrl(
		this,
		info.id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( NUMBER_EDIT_WIDTH, -1 )
	);
	text->SetValue( wxT( info.value.c_str() ) );

	// add spin
	sizer->Add( text, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// add string input
void InputDialog::addStringInput( wxSizer* sizer, InputInfo& info ) {
	// text
	wxTextCtrl* text = new wxTextCtrl(
		this,
		info.id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 )
	);
	text->SetValue( wxT( info.value.c_str() ) );

	// add text control
	sizer->Add( text, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// add select input
void InputDialog::addSelectInput( wxSizer* sizer, InputInfo& info ) {
	// create vector
	std::vector< wxString > items;
	std::vector<std::string> items_str;
	stringtoken(
		info.value.c_str(),
		"\n",
		items_str
		//boost::bind( &std::vector< wxString >::push_back, &items, _1 )
	);

	for (unsigned int i = 0; i < items_str.size(); i++) {
		items.push_back(items_str[i]);
	}

	// combo
	wxComboBox* combo = new wxComboBox(
		this,
		info.id,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		(int)items.size(),
		items.size() == 0 ? NULL : &( items[ 0 ] ),
		wxCB_SIMPLE | wxCB_READONLY
	);

	if( items.size() == 0 ) {
		combo->Enable( false );
	}
	else {
		combo->SetSelection( 0 );
	}

	// add
	sizer->Add( combo, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// init dialog
void InputDialog::onInitDialog( wxInitDialogEvent& evt ) {
	create();
}

// validate
bool InputDialog::Validate() {
	// check value
	std::string msg;
	for( unsigned int i = 0; i < m_inputs.size() && msg.empty(); i++ ) {
		// input
		InputInfo& info = m_inputs[ i ];

		// check
		if( info.type == TYPE_DOUBLE ) {		// double
			// get value
			std::string s;
			wxTextCtrl* text = (wxTextCtrl*)FindWindow( info.id );
			if( text != NULL ) {
				s = text->GetValue().c_str();
			}

			// convert
			double v = double();
			if( !isdouble( s.c_str(), &v ) ) {
				msg = FMT( "\"%s\" must be a numeric value.", info.name.c_str() );
			}

			// check restriction
			if( msg.empty() && info.restriction.hasInclusiveMinValue() ) {  // min (inclusive)
				double minVal = info.restriction.getInclusiveMinValue();
				if( v < minVal ) {
					msg = FMT( "\"%s\" must be greater than or equal to %f.", info.name.c_str(), minVal );
				}
			}
			if( msg.empty() && info.restriction.hasInclusiveMaxValue() ) {  // max (inclusive)
				double maxVal = info.restriction.getInclusiveMaxValue();
				if( v > maxVal ) {
					msg = FMT( "\"%s\" must be less than or equal to %f.", info.name.c_str(), maxVal );
				}
			}
			if( msg.empty() && info.restriction.hasExclusiveMinValue() ) {  // min (exclusive)
				double minVal = info.restriction.getExclusiveMinValue();
				if( v <= minVal ) {
					msg = FMT( "\"%s\" must be greater than %f.", info.name.c_str(), minVal );
				}
			}
			if( msg.empty() && info.restriction.hasExclusiveMaxValue() ) {  // max (exclusive)
				double maxVal = info.restriction.getExclusiveMaxValue();
				if( v >= maxVal ) {
					msg = FMT( "\"%s\" must be less than %f.", info.name.c_str(), maxVal );
				}
			}
		}
	}

	// check message
	if( !msg.empty() ) {
		wxMessageBox(
			wxT( msg.c_str() ),
			wxT( "Mass++" ),
			wxOK | wxICON_INFORMATION
		);
	}

	return msg.empty();
}

// windos -> data
bool InputDialog::TransferDataFromWindow() {
	// set value
	for( unsigned int i = 0; i < m_inputs.size(); i++ ) {
		// input
		InputInfo& info = m_inputs[ i ];

		if( info.type == TYPE_BOOL ) {
			wxCheckBox* check = (wxCheckBox*)FindWindow( info.id );
			if( check != NULL ) {
				info.value = ( check->GetValue() ? "true" : "false" );
			}
		}
		else if( info.type == TYPE_INT ) {		// integer
			wxSpinCtrl* spin = (wxSpinCtrl*)FindWindow( info.id );
			if( spin != NULL ) {
				info.value = FMT( "%d", spin->GetValue() );
			}
		}
		else if( info.type == TYPE_SELECTION ) {	// selection
			wxComboBox* combo = (wxComboBox*)FindWindow( info.id );
			if( combo != NULL ) {
				info.value = FMT( "%d", combo->GetSelection() );
			}
		}
		else {
			wxTextCtrl* text = (wxTextCtrl*)FindWindow( info.id );
			if( text != NULL ) {
				info.value = text->GetValue().c_str();
			}
		}
	}

	return true;
}
