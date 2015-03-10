/**
 * @file ProductIonSelectionDialog.cpp
 * @brief implementsation of ProductIonSelectionDialog class
 *
 * @author H.Parry
 * @date 2011.03.22
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "ProductIonSelectionDialog.h"

#include <vector>

using namespace kome::spectrum;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_SELECT_BUTTON	2010
#define ID_LISTBOX			2011

// constructor
ProductIonSelectionDialog::ProductIonSelectionDialog(
		wxWindow* parent,
		std::vector<kome::objects::Spectrum*>& productSpectra
) : kome::window::StandardDialog(
		parent,
		"Select Product Ion Spectrum"),
	m_productSpectra(productSpectra)
{
   // initialize
	std::string	strValue;
	std::vector<kome::objects::Spectrum*>::iterator spectrumIterator;
	for (spectrumIterator = m_productSpectra.begin();
			spectrumIterator != m_productSpectra.end(); ++spectrumIterator) {
		
		strValue = (*spectrumIterator)->getTitle();	// @date 2012/01/18 <Add> OKADA

		m_values.push_back(strValue);
	}

	m_list = NULL;
	m_iSelected = 0;
}

// destructor
ProductIonSelectionDialog::~ProductIonSelectionDialog() {
}

// create top sizer
wxSizer* ProductIonSelectionDialog::createMainSizer() {
	// field sizer
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 1, 2, 5 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->AddGrowableCol( 0 );

	// List box
	std::vector< wxString > values;
	for( unsigned int i = 0; i < m_values.size(); i++ ) {
		values.push_back( m_values[ i ].c_str() );
	}

	m_list = new wxListBox( this, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, (int)values.size(), &( values[ 0 ] ) );
	m_list->SetSelection(m_iSelected); // Select 1st item by default.

	mainSizer->Add(m_list, 1, wxALL | wxGROW, 5);

	return mainSizer;
}


// transfer data from window
bool ProductIonSelectionDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	m_iSelected = m_list->GetSelection();

    return true;
}

// transfer data to window
bool ProductIonSelectionDialog::TransferDataToWindow() {
    return true;
}
