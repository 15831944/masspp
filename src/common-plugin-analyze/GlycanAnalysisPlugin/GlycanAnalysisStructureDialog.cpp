/**
 * @file GlycanAnalysisStructureDialog.cpp
 * @brief Dialog class of GlycanAnalysisPlugin
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "GlycanAnalysisManager.h"
#include "GlycanAnalysisStructureDialog.h"

/** button id */
#define ID_BUTTON_CLOSE 13579

/** window size height*/
#define WINDOW_HEIGHT 420

/** window size width*/
#define WINDOW_WIDTH 350

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;

BEGIN_EVENT_TABLE( GlycanAnalysisStructureDialog, wxDialog )
	EVT_BUTTON( ID_BUTTON_CLOSE, GlycanAnalysisStructureDialog::onCloseButton )
END_EVENT_TABLE()

// constructor
GlycanAnalysisStructureDialog::GlycanAnalysisStructureDialog( wxWindow* parent , unsigned int unGNumber, GlycanParams* params, Annotation *annotation )
:	wxDialog(parent, wxID_ANY, wxT(FMT("Glycan Analysis")),
	wxPoint::wxPoint( -1, -1 ), wxSize::wxSize( -1, -1 ),
	wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX ) {
	
	if(((parent == NULL) || (params == NULL) || (annotation == NULL))){
		LOG_ERROR(FMT("[GlycanAnalysisStructureDialog] Parent or params or annotation is NULL."));
		return;
	}	
	GLYCAN_STRUCTURE glycanStructure;
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.parseKCFFile(unGNumber, &glycanStructure);
	m_canvas = new GlycanAnalysisStructureCanvas(this, &glycanStructure, (unsigned int)1, params->strLabel.c_str(),
		false, params->strAnion.c_str(), annotation);

	wxSizer* mainSizer = createMainSizer();
	SetSizer( mainSizer );

}

// destructor
GlycanAnalysisStructureDialog::~GlycanAnalysisStructureDialog() {
	if(m_canvas != NULL){
		delete m_canvas;
	}
	this->SetSizer(NULL);
}

// show this dialog
bool GlycanAnalysisStructureDialog::Show( bool show ) {
	// show
	if ( show == true ) {
		// set dialog min size
		wxSize size;
		size.SetWidth( WINDOW_WIDTH );
		size.SetHeight( WINDOW_HEIGHT );
		SetMinSize(size);
		SetMaxSize(size);

		// set position
		wxPoint point;
		point.x = -1;
		point.y = -1;
		SetPosition( point );

		// set maximize
		Maximize( false );

		if ( IsIconized() ) {
			Iconize( false );
		}
	}
	// hide
	else {

	}

	return wxDialog::Show( show );
}

// create main Sizer
wxSizer* GlycanAnalysisStructureDialog::createMainSizer() {

	// create Sizer
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer(2, 1, 0, 0);

	// add sizers to main sizer
	mainSizer->Add(createCanvasSizer(), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 25);
	mainSizer->Add(createButtonSizer(), 1, wxALIGN_RIGHT | wxRIGHT, 20);
	return mainSizer;
	
}

// create canvas Sizer
wxSizer* GlycanAnalysisStructureDialog::createCanvasSizer() {
	wxFlexGridSizer* canvasSizer = new wxFlexGridSizer(1, 1, 0, 0);
	canvasSizer->Add(m_canvas ,0);
	return canvasSizer;
}

// create button Sizer
wxSizer* GlycanAnalysisStructureDialog::createButtonSizer() {
	wxFlexGridSizer* buttonSizer = new wxFlexGridSizer(1, 2, 0, 25);
	buttonSizer->AddGrowableCol(0);
	wxButton* btn = new wxButton(this, ID_BUTTON_CLOSE, "Close");
	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(btn);
	return buttonSizer;
}

// when press the Close button, called this function.
void GlycanAnalysisStructureDialog::onCloseButton( wxCommandEvent& evt ){
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	gaManager.closeStructureDialog();
}
