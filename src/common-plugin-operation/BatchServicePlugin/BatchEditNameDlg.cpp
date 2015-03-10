/**
 * @file BatchEditNameDlg.cpp
 * @brief implementsation of BatchEditNameDlg class
 *
 * @author OKADA, H
 * @date 2012-04-19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchEditNameDlg.h"
#include "BatchButtonDef.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP		3
#define BORDER_SIZE		3

// constructor
BatchEditNameDlg::BatchEditNameDlg( wxWindow* parent, void *pParent )
	: kome::window::ParametersAndHelpDialog( parent,"Edit Batch Method Name" )	// @Date:2014/01/24	<extend help>	A.Ozaki
{
	m_pNameCtrl = NULL;

	m_strMethodName.clear();

	m_pParentDlg = (BatchProcessingMethodConfigurationDialog *)pParent;
	m_strCurrentMethodParent= m_pParentDlg->getCurrentMethod();
}


// destructor
BatchEditNameDlg::~BatchEditNameDlg()
{
}


// create top sizer
wxSizer* BatchEditNameDlg::createMainSizer() {
	
	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );		// @date 2012/04/23 <Add> OKADA
	fieldSizer->AddGrowableRow( 0 );
	
	// upside sizer
	wxFlexGridSizer* upsideSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	upsideSizer->AddGrowableRow( 0 );	// @date 2012/04/23 <Add> OKADA
	upsideSizer->AddGrowableCol( 1 );

	// "Batch Method Name"
	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Batch Method Name" ), wxDefaultPosition, wxDefaultSize, 0 );
	upsideSizer->Add( descrName, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);	
	
	// A text control for the name of method
	m_pNameCtrl = new wxTextCtrl ( this, ID_NAME_EDIT_METHOD_TEXT, m_strCurrentMethodParent.c_str(), wxDefaultPosition, wxDefaultSize, 0 );
	upsideSizer->Add(m_pNameCtrl, 0, wxGROW|wxALL, 5);
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_pNameCtrl, "Changed Batch Method name." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki
	
	fieldSizer->Add( upsideSizer, 1, wxGROW | wxALL, BORDER_SIZE );
	
	return fieldSizer;
}


bool BatchEditNameDlg::onOK()
{
	m_strMethodName.clear();

	if( m_pNameCtrl->GetValue().length() <= 0 ){
		kome::window::WindowTool::showInfo( "Method Name cannot be empty." );
		return false;
	}

	if( m_pParentDlg->checkSameNameConbobox( m_pNameCtrl->GetValue().c_str() ) != 0 &&
		m_strCurrentMethodParent.compare( m_pNameCtrl->GetValue().c_str() ) != 0 ){// Spec No.91861 @date 2013.03.28 <Add> M.Izumi
		// “¯‚¶•¨‚ª‚ ‚Á‚½
		kome::window::WindowTool::showInfo( "A method with that name already exists." );
		return false;
	}

	m_strMethodName = m_pNameCtrl->GetValue().c_str();

	return true;
}

std::string BatchEditNameDlg::getNewMethodName(){
	return m_strMethodName;
}
