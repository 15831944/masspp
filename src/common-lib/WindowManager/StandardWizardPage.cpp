/**
 * @file StandardWizardPage.cpp
 * @brief implements of StandardWizardPage class
 *
 * @author S.Tanaka
 * @date 2013.02.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "StandardWizardPage.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP					    3


BEGIN_EVENT_TABLE( StandardWizardPage, wxWizardPageSimple )
	EVT_INIT_DIALOG( StandardWizardPage::onInitDialog )
END_EVENT_TABLE()


// constructor
StandardWizardPage::StandardWizardPage( wxWindow* parent ) : wxWizardPageSimple( dynamic_cast< wxWizard*>( parent ) ) {
	// style
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
}

// destructor
StandardWizardPage::~StandardWizardPage() {
}

// create controls
void StandardWizardPage::createControls() {
	// Auto Layout
	SetAutoLayout( true );

	// create top sizer
	wxSizer* mainSizer = createMainSizer();

	// Fit
	SetSizer( mainSizer );
	mainSizer->Fit( this );
	mainSizer->SetSizeHints( this );
}

// create main sizer
wxSizer* StandardWizardPage::createMainSizer() {
	return new wxBoxSizer( wxVERTICAL );
}

// on init dialog
void StandardWizardPage::onInitDialog( wxInitDialogEvent& evt ) {
	createControls();

	wxWizardPageSimple::OnInitDialog( evt );

	wxWindowList& controlList = GetChildren();
	wxWindow* firstNonStaticControl = NULL;
	for ( unsigned int i = 0; (i < controlList.size()) && (!firstNonStaticControl); i++ )
	{
		if ( controlList[i]->IsEnabled() && (controlList[i]->AcceptsFocus()) )
		{
			firstNonStaticControl = controlList[i];
		}
	}
	if ( firstNonStaticControl != NULL )
	{
		firstNonStaticControl->SetFocus();
	}
}
