/**
 * @file BatchBase.cpp
 * @brief interfaces of BatchBase class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchBase.h"
#include "BatchActionSelectPanel.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( BatchBase, kome::window::StandardPanel )
END_EVENT_TABLE()


BatchBase::BatchBase( wxWindow* pParent,
	BatchProcessingMethodConfigurationDialog* pParentDlg,
	const char* sPanelName,
	bool bIncludeDefaultButton)
	: kome::window::StandardPanel(pParent),
		m_pParentDlg(pParentDlg)
{
}


BatchBase::~BatchBase(void)
{	
}


// create main sizer
wxSizer* BatchBase::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* InnerSizer = new wxStaticBoxSizer( wxHORIZONTAL, this );

	// This is a pure virtual function that must be implemented by each panel
	// to display its controls.
	createInnerSizer(InnerSizer);

	sizer->Add(InnerSizer, 1, wxALL | wxGROW | wxALIGN_CENTER, BORDER_SIZE);

	return sizer;
}

// >>>>>>	@Date:2014/02/04	<Add>	A.Ozaki
//
void	BatchBase::createWidgets( void )
{
	createControls( );

	return;
}
//
// <<<<<<	@Date:2014/02/04	<Add>	A.Ozaki
