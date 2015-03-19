/**
 * @file IdentificationDialog.cpp
 * @brief implementsation of IdentificationDialog class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "IdentificationDialog.h"          
#include "IdentificationSpectrumGetter.h"
#include "SpectrumListCtrl.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE                        3


BEGIN_EVENT_TABLE( IdentificationDialog, IdentificationDialogBase )
END_EVENT_TABLE()


// constructor
IdentificationDialog::IdentificationDialog( wxWindow* parent, kome::objects::Sample* acSample )
		: IdentificationDialogBase( parent, "Identify" )
{
	// initialize
	m_list = NULL;
	m_sample = acSample;
	m_currentEngine = NULL;
}

// destructor
IdentificationDialog::~IdentificationDialog() {
}

// get spectrum getter
SpectrumGetter* IdentificationDialog::createSpectrumGetter() {
	// spectrum getter
	IdentificationSpectrumGetter* spectra = new IdentificationSpectrumGetter( m_sample );

	// spectra
	std::vector< long long > selectedData;
	m_list->getSelectedData( selectedData );
	if( selectedData.size() == 0 ) {
		kome::window::WindowTool::showError(
			"Identification cannot be performed because no spectra are selected. Select one or more spectra."
		);
		return false;
	}

	spectra->clearSpectra();
	for( unsigned int i = 0; i < selectedData.size(); i++ ) {
		int idx = (int)selectedData[ i ];
		kome::objects::Spectrum* spec = m_list->getSpectrum( idx );
		if( spec != NULL ) {
			spectra->addSpectrum( spec );
		}
	}

	return spectra;
}

// create spectrum getter
wxSizer* IdentificationDialog::createSpectraSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// list
	kome::window::CheckGridListPanel* panel = new kome::window::CheckGridListPanel( this );
	panel->setDescription( "Select spectra to make peak list for posting to search engine." ); // @date 2014.01.23 <Add> M.Izumi
	m_list = new SpectrumListCtrl( panel, 300, 180 );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	// engine
	TransferDataToWindow();
	SearchEngine* engine = getSearchEngine();
	onChangeEngine( engine );

	return sizer;
}

// on change engine
void IdentificationDialog::onChangeEngine( kome::ident::SearchEngine* engine ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// check the member
	if( m_list == NULL ) {
		return;
	}
	if( engine == m_currentEngine ) {
		return;
	}

	// list
	m_list->clearSpectra();

	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( m_sample );
	if( dataSet != NULL && engine != NULL ) {
		for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
			kome::objects::Spectrum* spec = dataSet->getSpectrum( i );

			if( engine->isValidSpectrum( spec ) ) {
				m_list->addSpectrum( spec );
			}
		}
	}

	m_list->create();
	m_list->Enable( m_list->getNumberOfSpectra() > 0 );
	m_list->selectAll();
	m_currentEngine = engine;
}
