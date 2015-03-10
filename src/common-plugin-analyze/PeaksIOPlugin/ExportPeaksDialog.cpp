/**
 * @file ExportPeaksDialog.cpp
 * @brief interfaces of ExportPeaksDialog class
 *
 * @author M.Izumi
 * @date 2013.02.12
 *
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "ExportPeaksDialog.h"
#include "PeaksIOManager.h"


using namespace kome::io::peaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE					3
#define GRID_GAP					3

#define ID_RADIO_BOX				1

#define CURRENT_SPECTRUM			"Current Spectrum"
#define CURRENT_CHROMATOGRAM		"Current Chromatogram"	// @date 2011/11/29 <Add> OKADA
#define FILTERED_SPECTRA			"Filtered Spectra"
#define SAMPLE						"Sample"
#define FORMAT                      "Format"

BEGIN_EVENT_TABLE( ExportPeaksDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()

// constructor 
ExportPeaksDialog::ExportPeaksDialog( wxWindow* parent )
	: kome::window::ParametersAndHelpDialog( parent, "Export Peaks" ){
	
	// initialize
	m_target = TARGET_SPECTRUM;
	m_selectRadioBox = -1;
	
}

// destructor
ExportPeaksDialog::~ExportPeaksDialog(){
}

wxSizer* ExportPeaksDialog::createMainSizer(){
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// radio button
	wxString targets[] ={
		_T(CURRENT_SPECTRUM),
		_T(FILTERED_SPECTRA),
		_T(SAMPLE)
	};
	
	// @date 2011/11/29 <Add> OKADA ------->
	// targets[]•¶Žš—ñ‚Ì‰Šú‰»
	// get active form
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant var( frame->getActiveObject() );
		kome::objects::Sample* sample = var.getSample();
		kome::objects::Spectrum* spec = var.getSpectrum();
		kome::objects::Chromatogram* chrom = var.getChromatogram();
	
		if( spec != NULL ){
			targets[0] = _T(CURRENT_SPECTRUM);
		}else if( chrom != NULL ){
			targets[0] = _T(CURRENT_CHROMATOGRAM);
		}
		
		m_RadioBox = new wxRadioBox(
			this,
			ID_RADIO_BOX,
			wxT("Target"),
			wxDefaultPosition,
			wxDefaultSize,
			sizeof( targets ) /sizeof( wxString ),
			targets,
			0,
			wxRA_SPECIFY_ROWS
		);

		if(( spec == NULL )&&( chrom == NULL )){
			m_RadioBox->Enable(0, false);
		}

		kome::objects::DataSet* dataSet = sample->getRootDataGroupNode();
		if( dataSet->getNumberOfSpectra() == 0 ){
			m_RadioBox->Enable(1, false);
		}
	
		// ini file
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		// target
		bool btarget = false;
		int target = ini->getInt(  EXPORT_SECTION, TARGET_KEY, 0 );
		if( target == (int)TARGET_FILTERED ) {		// filtered spectra
			m_target = TARGET_FILTERED;
			if( m_RadioBox->IsItemEnabled( 1 ) ){
				m_RadioBox->SetSelection( 1 );
				btarget = true;
			}
		}
		else if( target == (int)TARGET_SAMPLE ) {	// sample
			m_target = TARGET_SAMPLE;
			if( m_RadioBox->IsItemEnabled( 2 ) ){
				m_RadioBox->SetSelection( 2 );
				btarget = true;
			}
		}
		else if(( target == (int)TARGET_SPECTRUM )||( target == (int)TARGET_CHROMATOGRAM )){	// spectrum 
			if(targets[0] == _T(CURRENT_SPECTRUM)){
				m_target = TARGET_SPECTRUM;
			}else if( targets[0] == _T(CURRENT_CHROMATOGRAM) ){
				m_target = TARGET_CHROMATOGRAM;	
			}
			if( m_RadioBox->IsItemEnabled( 0 ) ){							// @date 2011/11/30 <Add> OKADA
				m_RadioBox->SetSelection( 0 );								// @date 2011/11/30 <Add> OKADA
				btarget = true;												// @date 2011/11/30 <Add> OKADA
			}																// @date 2011/11/30 <Add> OKADA
		}
	
		// DisableŽž‚Ì‘I‘ðˆÊ’u @date 2011.09.08 <Add> M.Izumi
		if( !btarget ){
			m_target = TARGET_SAMPLE;	// @date 2011/12/08 <Add> OKADA
			m_RadioBox->SetSelection( 2 );
		}
	}
	setDescription( m_RadioBox, "Select the target spectra / chromatograms to export peaks." );
	sizer->Add( m_RadioBox, 1, wxGROW | wxALL, 0 );

	return sizer;
}

// create button sizer
wxSizer* ExportPeaksDialog::createButtonSizer(){
		// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	// add Export button
	buttonSizer->Add(
		new wxButton( this, wxID_OK, wxT( "Export" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
		
	// Cancel button
	buttonSizer->Add( new wxButton( this, wxID_CANCEL, wxT( "Cancel" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_RIGHT,
		BORDER_SIZE
	);

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );
	return sizer;
}

// transfer data from window
bool ExportPeaksDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	m_selectRadioBox = m_RadioBox->GetSelection();

	return true;
}   

