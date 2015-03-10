/**
 * @file PluginManagerDlgBase.cpp
 * @brief implementation of PluginManagerDlgBase class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PluginManagerDlgBase.h"
#include "PluginManagerPluginManager.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP                     3
#define BORDER_SIZE                  3


BEGIN_EVENT_TABLE( PluginManagerDlgBase, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_BUTTON( ID_APPLY_BUTTON, PluginManagerDlgBase::onApplyButton )
	EVT_BUTTON( ID_CLOSE_BUTTON, PluginManagerDlgBase::onCloseButton )
	EVT_COMBOBOX(ID_COMBOBOX_PACKAGE, PluginManagerDlgBase::onSelectPackageEvt)
END_EVENT_TABLE()


// constructor
PluginManagerDlgBase::PluginManagerDlgBase( wxWindow* pParent, const char* title )
		: kome::window::ParametersAndHelpDialog( pParent, title, "Plugin Manager" ), m_params( NULL, false ) {	// @Date:2014/01/24	<extend help>	A.Ozaki
	// initialize
	m_comboPackage = NULL;

	m_pParent = pParent;

}

// destructor
PluginManagerDlgBase::~PluginManagerDlgBase() {
}

// main sizer
wxSizer* PluginManagerDlgBase::createMainSizer() { 
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableCol( 0 );

	// search sizer
	wxSizer* searchSizer = createPackageSizer();
	if( searchSizer != NULL ){
		sizer->Add( searchSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	// plugin sizer
	wxSizer* pluginSizer = createPluginsSizer();
	if( pluginSizer != NULL ){
		sizer->Add( pluginSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	// function sizer
	wxSizer* peaksSizer = createFunctionsSizer();
	if( peaksSizer != NULL ){
		sizer->Add( peaksSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	return sizer;
}

// create peaks sier
wxSizer* PluginManagerDlgBase::createPackageSizer() {

	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );

	// "Batch Method"
	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Package" ), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( descrName, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// Package
	wxArrayString stringsPackage;
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	int nNum = plgMgr.getNumberOfPackages();
	stringsPackage.Add( STR_PACKAGE_FULL );			// "Full"
	for( int i=0; i<nNum; i++ ){
		char* pszPackage = plgMgr.getPackage( i );
		stringsPackage.Add( pszPackage );
		;
	}
	stringsPackage.Add( STR_PACKAGE_CUSTOMIZED );	// "Customized"

	// INIファイルから読んでくる
	
	//////////
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini

	// パッケージ名を読み込み
	const char* SECTION_NAME_HEAD="PluginManagerCompo";

	char szIniSectionName[256];
	char szIniParameterName[256];

	sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, "SelectedPackage" );
	sprintf( szIniParameterName, "SelectedPackage" );

	const char* szSelectedPackage = ini->getString( szIniSectionName, szIniParameterName, STR_PACKAGE_FULL );

	std::string strPackageNameLast = szSelectedPackage;	// INIファイルから読んでくる

	// PluginManagerDlg::createPluginsSizer()内で初期値選択を実装した
	wxSize wxsSize = wxDefaultSize;
	m_comboPackage = new wxComboBox(this, ID_COMBOBOX_PACKAGE, strPackageNameLast, wxDefaultPosition, wxsSize, stringsPackage, wxCB_SIMPLE | wxCB_READONLY );	 //　コンボボックスに追加
	sizer->Add( m_comboPackage, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_comboPackage, "Select the package." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	m_strSelectedPackageName = strPackageNameLast;

	return sizer;
}

// create spectra sizer
wxSizer* PluginManagerDlgBase::createPluginsSizer() {
	
	return NULL;

}

// create spectra sizer
wxSizer* PluginManagerDlgBase::createFunctionsSizer() {
	
	return NULL;

}

// main sizer
wxSizer* PluginManagerDlgBase::createButtonSizer() { 
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	buttonSizer->Add( new wxButton( this, ID_APPLY_BUTTON, wxT( "Apply" ) ) );
	buttonSizer->Add( new wxButton( this, ID_CLOSE_BUTTON, wxT( "Close" ) ) );

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );
	
	return sizer;
}

// Apply ボタン押下時 
void PluginManagerDlgBase::onApplyButton( wxCommandEvent& evt ) {

	(this)->Close( true );

	(this)->Destroy();

	SetReturnCode( wxID_APPLY );

	// ※PluginManagerDlgでオーバーライドしました
}

// Close ボタン押下時 
void PluginManagerDlgBase::onCloseButton( wxCommandEvent& evt ) {

	(this)->Close( true );

	(this)->Destroy();

	SetReturnCode( wxID_CANCEL );
	// ※PluginManagerDlgでオーバーライドしました
}

// on select package
void PluginManagerDlgBase::onSelectPackageEvt( wxCommandEvent& evt ){

	int nId = evt.GetId();	// nId には ID_COMBOBOX_PACKAGEが入る

	wxString wxstrSelectedPackage = m_comboPackage->GetValue();

	m_strSelectedPackageName = wxstrSelectedPackage.c_str();

	// mgr経由で PluginManagerDlg 内のメンバ関数を呼び出す
	PluginManagerView& mgr = PluginManagerView::getInstance();
	mgr.onSelectPackage();

	return;
}

// get selected package
std::string PluginManagerDlgBase::getSelectedPackage(){

	wxString wxstrSelectedPackage = m_comboPackage->GetValue();

	m_strSelectedPackageName = wxstrSelectedPackage.c_str();

	return m_strSelectedPackageName;
}
