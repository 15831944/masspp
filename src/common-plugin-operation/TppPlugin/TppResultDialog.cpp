/**
 * @file TppResultDialog.cpp
 * @brief implements of TppResultDialog class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppResultDialog.h"
#include "TppResultGrid.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;

#define DEFAULTSIZE_HEIGHT 300
#define DEFAULTSIZE_WIDTH 620

// static member
TppResultDialog* TppResultDialog::m_dlg = NULL;

BEGIN_EVENT_TABLE( TppResultDialog, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_INIT_DIALOG( TppResultDialog::OnInitDialog )
	EVT_BUTTON( ID_BUTTON_CLOSE, TppResultDialog::OnClickButtonClose )
END_EVENT_TABLE()

// constructor
TppResultDialog::TppResultDialog( wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, "TPP Result", NULL, -1, -1, DEFAULTSIZE_WIDTH, DEFAULTSIZE_HEIGHT) {
			// @Date:2014/01/24	<extend help>	A.Ozaki
	// initialize
	m_btnClose = NULL;
	
}

// destructor
TppResultDialog::~TppResultDialog() {
	
}

// create main sizer.
wxSizer* TppResultDialog::createMainSizer()
{

	//create main sizer (row 2, col 1)
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer(2,1,0,0);
	mainSizer->AddGrowableCol(0);
	mainSizer->AddGrowableRow(0);
	mainSizer->AddGrowableCol(1);

	m_listTPPResult = new kome::Tpp::TppResultGrid(this, ID_LIST_TPP_RESULT);
	m_listTPPResult->create();
	mainSizer->Add(m_listTPPResult, 0, wxALL | wxGROW, 20);

	//create footer sizer.
	wxFlexGridSizer* footerSizer = new wxFlexGridSizer(1,2,0,0);
	footerSizer->AddGrowableCol(0);

	m_btnClose = new wxButton(this, ID_BUTTON_CLOSE, "Close");
	footerSizer->Add(NULL, 0, wxALL | wxGROW);
	footerSizer->Add(m_btnClose, 1, wxALL | wxSHRINK);
	mainSizer->Add(footerSizer, 1, wxALL | wxGROW, 20);

	return mainSizer;
}

// initialize this dialog
void TppResultDialog::OnInitDialog( wxInitDialogEvent& evt ) {
	wxSizer* mainSizer = createMainSizer();	

	SetSizer( mainSizer );

	wxDialog::OnInitDialog( evt );
}

// clicked Close button event
void TppResultDialog::OnClickButtonClose( wxCommandEvent& evt ){
	this->Close();
}
