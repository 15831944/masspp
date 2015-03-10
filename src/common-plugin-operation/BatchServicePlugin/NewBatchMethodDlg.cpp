/**
 * @file NewBatchMethodDlg.cpp
 * @brief implementsation of NewBatchMethodDlg class
 *
 * @author OKADA, H
 * @date 2012-03-05
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "NewBatchMethodDlg.h"
#include "BatchButtonDef.h"


using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP	3
#define BORDER_SIZE		3

#define DISP_TEMPLATE	// @date 2013/08/19 <Add> OKADA

BEGIN_EVENT_TABLE( NewBatchMethodDlg, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	
	EVT_BUTTON( wxOK, NewBatchMethodDlg::onOkButton )
	EVT_COMBOBOX(ID_COMBOBOX_NEW_METHOD, changeMethodCombobox )

	EVT_UPDATE_UI( ID_COMBOBOX_NEW_METHOD,  NewBatchMethodDlg::onUpdateUI )	// @date 2012/04/19 <Add> OKADA	// SPEC 85664

#ifdef DISP_TEMPLATE
	EVT_UPDATE_UI( ID_COMBOBOX_NEW_TEMPLATE,  NewBatchMethodDlg::onUpdateUI )	// @date 2012/04/19 <Add> OKADA
#endif
	EVT_UPDATE_UI( wxID_OK,  NewBatchMethodDlg::onUpdateUI )	// @date 2012/04/19 <Add> OKADA	// SPEC 85668

END_EVENT_TABLE()


// constructor
NewBatchMethodDlg::NewBatchMethodDlg( wxWindow* parent, void *pParent )
: kome::window::ParametersAndHelpDialog( parent, "New Batch Method")	// @Date:2014/01/24	<extend help>	A.Ozaki
{
	m_bMethod = false;
	m_bTemplate = false;
	m_bBlank = false;

	m_rbMethod = NULL;
	m_rbTemplate = NULL;
	m_rbBlank = NULL;
	m_pNameCtrl = NULL;
	m_pComboMethod = NULL;
	m_pComboTemplate = NULL;

	m_pParentDlg = (BatchProcessingMethodConfigurationDialog *)pParent;
	m_wxastrStringsMethod	= m_pParentDlg->getStringsMethod();
	m_wxstrNewMethodName	= m_pParentDlg->getCurrentMethod();
	m_strCurrentMethodParent= m_pParentDlg->getCurrentMethod();

	m_stNewMethodSetting.strNewMethodName= "";
	m_stNewMethodSetting.bMethod			= true;
	m_stNewMethodSetting.bTemplate			= false;
	m_stNewMethodSetting.bBlank				= false;
	m_stNewMethodSetting.wxstrMethod		= "";
	m_stNewMethodSetting.wxstrTemplate		= "";

}


// destructor
NewBatchMethodDlg::~NewBatchMethodDlg() {
}


// create main sizer
wxSizer* NewBatchMethodDlg::createMainSizer() {

	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableRow( 0 );
	fieldSizer->AddGrowableRow( 1 );
	fieldSizer->AddGrowableRow( 2 );

	fieldSizer->Add( createUpsideSizer(), 1, wxGROW | wxALL, BORDER_SIZE );
	fieldSizer->Add( createBottomSizer(), 1, wxGROW | wxALL, BORDER_SIZE );

	return fieldSizer;
}


wxSizer* NewBatchMethodDlg::createUpsideSizer() {

	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 1 );
	topSizer->AddGrowableRow( 0 );

	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Name" ), wxDefaultPosition, wxDefaultSize, 0 );
	topSizer->Add(descrName, 0, wxALIGN_LEFT|wxALL, 5);

	// A text control for the name of method
	m_pNameCtrl = new wxTextCtrl ( this, ID_NAME_NEW_METHOD_TEXT,"", wxDefaultPosition, wxDefaultSize, 0 );	// SPEC 85857
	topSizer->Add(m_pNameCtrl, 0, wxGROW|wxALL, 5);
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_pNameCtrl, "New Batch Method name" );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	return topSizer;
}


wxSizer* NewBatchMethodDlg::createBottomSizer() {

	wxFlexGridSizer* bottomSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );	// @date 2012/04/24 <Add> OKADA
	bottomSizer->AddGrowableCol( 1 );
	bottomSizer->AddGrowableRow( 0 );

	m_rbMethod = new wxRadioButton (this,ID_RADIOBUTTON_NEW_METHOD_TEXT, wxT( "&Method" ), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	m_rbMethod->SetValue(true);

#ifdef DISP_TEMPLATE
	m_rbTemplate = new wxRadioButton (this,ID_RADIOBUTTON_NEW_TEMPLATE_TEXT, wxT( "&Template" ));
#endif
	m_rbBlank = new wxRadioButton (this,ID_RADIOBUTTON_NEW_BLANK_TEXT, wxT( "&Blank" ));

	m_pComboMethod = new wxComboBox(this, ID_COMBOBOX_NEW_METHOD, wxT( m_strCurrentMethodParent.c_str()  ), wxDefaultPosition, wxDefaultSize, m_wxastrStringsMethod,  wxCB_SIMPLE | wxCB_READONLY);  // for SPEC 85700 

	m_pComboMethod->GetValue();

	if( m_wxastrStringsMethod.size() <= 0 ){
		// 既存のMethodがない為、disableにする
		m_pComboMethod->Disable();
		m_pComboMethod->SetValue( "" );

		// Methodが無いので、MethodのラジオボタンもDisableにする
		m_rbMethod->SetValue( false );	m_rbMethod->Disable();	m_bMethod=false;
#ifdef DISP_TEMPLATE
		m_rbTemplate->SetValue( true );	m_rbTemplate->Enable();	m_bTemplate=true;
#endif

	}

#ifdef DISP_TEMPLATE
	// [Template▼]
	wxArrayString stringsTemplate;

	// @date 2013/08/19 <Add> OKADA ------->
	// managers
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	const char* TEMPLATE_TYPE = "batch_set";
	std::string strTemplateDefault;

	unsigned int uNum = plgMgr.getNumberOfFunctionItems( TEMPLATE_TYPE );
	kome::plugin::PluginFunctionItem* item = NULL;
	for( unsigned int i=0; i<uNum; i++ ){
		 item = plgMgr.getFunctionItem( TEMPLATE_TYPE, i );
		if( item != NULL ){
			const char* szLongName = item->getLongName();
			if( szLongName != NULL ){
				stringsTemplate.Add(wxT( szLongName ));
				if( strTemplateDefault.empty() ){
					strTemplateDefault = szLongName;
				}
			}
		}
	}
//	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "batch_set", "" );
	// @date 2013/08/19 <Add> OKADA <-------

//	wxArrayString stringsTemplate;
/*
	stringsTemplate.Add(wxT( "Apple" ));
	stringsTemplate.Add(wxT( "Orange" ));
	stringsTemplate.Add(wxT( "Pear" ));
	stringsTemplate.Add(wxT( "Grapefruit" ));
*/
//	m_pComboTemplate = new wxComboBox(this, ID_COMBOBOX_NEW_TEMPLATE, wxT( "Orange" ), wxDefaultPosition, wxDefaultSize, stringsTemplate, wxCB_DROPDOWN);
	m_pComboTemplate = new wxComboBox(this, ID_COMBOBOX_NEW_TEMPLATE, wxT( strTemplateDefault.c_str() ), wxDefaultPosition, wxDefaultSize, stringsTemplate, wxCB_DROPDOWN);	// @date 2013/08/19 <Add> OKADA
#endif
// 2012/04/03 Templateは、今の所表示しない

#ifdef DISP_TEMPLATE
	const char	*pcRadioDesc = "Select the base method.\n\nMethod:copy the already saved Batch Method.\nTemplate:create the Batch Method the specified template.\nBlank:create the empty method.";
	setDescription( m_rbMethod, pcRadioDesc );
	setDescription( m_rbTemplate, pcRadioDesc );
	setDescription( m_rbBlank, pcRadioDesc );
	setDescription( m_pComboMethod, pcRadioDesc );
	setDescription( m_pComboTemplate, pcRadioDesc );
#else
	const char	*pcRadioDesc = "Select the base method.\n\nMethod:copy the already saved Batch Method.\nBlank:create the empty method.";
	setDescription( m_rbMethod, pcRadioDesc );
	setDescription( m_rbBlank, pcRadioDesc );
	setDescription( m_pComboMethod, pcRadioDesc );
#endif

	wxStaticText* staticText = new wxStaticText( this
		, wxID_STATIC
		, wxT( "     " )	// blank
		, wxDefaultPosition
		, wxDefaultSize
		, wxALIGN_LEFT);

	// Method
	bottomSizer->Add( m_rbMethod, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	bottomSizer->Add( m_pComboMethod, 0, wxGROW|wxALL, 5);

#ifdef DISP_TEMPLATE
	// Template
	bottomSizer->Add( m_rbTemplate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	bottomSizer->Add( m_pComboTemplate, 0, wxGROW|wxALL, 5);
// 2012/04/03 Templateは、今の所表示しない
#endif

	// None
	bottomSizer->Add( m_rbBlank, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	bottomSizer->Add( staticText, 0, wxGROW|wxALL, 5);

	wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, wxT( "Base Method" ));
	wxStaticBoxSizer* staticSizer = new wxStaticBoxSizer(staticBox,wxVERTICAL);
	staticSizer->Add( bottomSizer, 0, wxGROW|wxALL, 5);

#ifdef DISP_TEMPLATE
	if( m_wxastrStringsMethod.size() <= 0 ){
		// NONE を選択状態にする
		m_stNewMethodSetting.bMethod	= false;	m_rbMethod->SetValue( false );
		m_stNewMethodSetting.bTemplate	= false;	m_rbTemplate->SetValue( false );
		m_stNewMethodSetting.bBlank		= true;		m_rbBlank->SetValue( true );
	}
#else
	if( m_wxastrStringsMethod.size() <= 0 ){
		// NONE を選択状態にする
		m_stNewMethodSetting.bMethod	= false;	m_rbMethod->SetValue( false );
		m_stNewMethodSetting.bBlank		= true;		m_rbBlank->SetValue( true );
	}
#endif

	// SPEC 95912
	// NONE を選択状態にする
	m_stNewMethodSetting.bMethod	= false;	m_rbMethod->SetValue( false );
	m_stNewMethodSetting.bBlank		= true;		m_rbBlank->SetValue( true );

	return staticSizer;
//	return bottomSizer;
}


// create Button sizer
wxSizer* NewBatchMethodDlg::createButtonSizer() {

	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 2, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}



void NewBatchMethodDlg::onOkButton( wxCommandEvent& evt ){
	
	if( m_pParentDlg->checkSameNameConbobox( m_pNameCtrl->GetValue().c_str() ) != 0 ){
		// 同じ物があった
		kome::window::WindowTool::showInfo( "A method with that name already exists." );
		return;
	}

	m_bMethod  = m_rbMethod->GetValue();
	
#ifdef DISP_TEMPLATE
	m_bTemplate= m_rbTemplate->GetValue();
#else
	m_bTemplate= false;
#endif
	m_bBlank   = m_rbBlank->GetValue();

	wxString wxstrMethod = m_pComboMethod->GetValue();

#ifdef DISP_TEMPLATE
	wxString wxstrTemplate = m_pComboTemplate->GetValue();
#else
	wxString wxstrTemplate = "";
#endif
	
	m_wxstrNewMethodName = m_pNameCtrl->GetValue();

	// 構造体へデータ格納
	m_stNewMethodSetting.strNewMethodName	= m_wxstrNewMethodName.c_str();
	m_stNewMethodSetting.bMethod			= m_bMethod;
	m_stNewMethodSetting.bTemplate			= m_bTemplate;
	m_stNewMethodSetting.bBlank				= m_bBlank;
	m_stNewMethodSetting.wxstrMethod		= wxstrMethod;
	m_stNewMethodSetting.wxstrTemplate		= wxstrTemplate;

	StandardDialog::onOkButton( evt );

	return;
}


void NewBatchMethodDlg::changeMethodCombobox( wxCommandEvent& evt ){
	m_wxstrSelectedMethod = m_pComboMethod->GetValue();
}


void NewBatchMethodDlg::changeTemplateCombobox( wxCommandEvent& evt ){
	m_wxstrSelectedTemplate = m_pComboTemplate->GetValue();
}


NewMethodSetting NewBatchMethodDlg::getNewMethodSetting( void ){
	return m_stNewMethodSetting;
}


void NewBatchMethodDlg::onUpdateUI( wxUpdateUIEvent& evt ){

	int nId = evt.GetId();
	switch( nId ){
	case ID_COMBOBOX_NEW_METHOD:	// SPEC 85664
		{
			bool bStatus = m_rbMethod->GetValue();
			if( bStatus  == true ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
#ifdef DISP_TEMPLATE
	case ID_COMBOBOX_NEW_TEMPLATE:
		{
//			bool bStatusOld = m_pComboTemplate->GetValue();
			bool bStatus = m_rbTemplate->GetValue();	// @date 2013/08/20 <Add> OKADA
			if( bStatus  == true ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
#endif
	case wxID_OK:	// SPEC 85668
		{
			wxString wxstrNewMethodName = m_pNameCtrl->GetValue();
			if( wxstrNewMethodName.length() <= 0 ){
				evt.Enable( false );
			}else{
				evt.Enable( true );
			}
		}
		break;
	default:
		break;
	}

}
