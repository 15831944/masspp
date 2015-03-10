/**
 * @file PluginManagerDlg.cpp
 * @brief implementation of PluginManagerDlg class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "PluginManagerDlg.h"
#include "PluginManagerPluginManager.h"
#include "PluginsListCtrl.h"

#include "ComponentsListCtrl.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE	3


BEGIN_EVENT_TABLE( PluginManagerDlg, PluginManagerDlgBase )
	EVT_SIZE( PluginManagerDlg::onSize )	// @date 2013/05/13 <Add> OKADA
END_EVENT_TABLE()


// constructor
PluginManagerDlg::PluginManagerDlg( wxWindow* parent, void* ptr )
		: PluginManagerDlgBase( parent, "Plugin Manager" )	// 第二引数はウインドウタイトルになります
{
	// initialize

	m_listPlugins = NULL;

	m_listComponents = NULL;
	m_parent = parent;

	m_wxIdPluginsList = 0;

	// @date 2013/05/13 <Add> OKADA ------->
	long style  = m_parent->GetWindowStyle();
	long styleNew = style & ~wxRESIZE_BORDER;	// 0x0040
	m_parent->SetWindowStyle( styleNew );	// リサイズ禁止できない

	wxSize sizeWindow = m_parent->GetSize();
	wxSize sizeThisWindow = this->GetSize();

	/** window size height */
	m_nHeight = -1;

	/** window size width */
	m_nWidth = -1;

	// @date 2013/05/13 <Add> OKADA <-------
}

// destructor
PluginManagerDlg::~PluginManagerDlg() {
}

// create plugins sizer
wxSizer* PluginManagerDlg::createPluginsSizer(){

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// title
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Plug-ins" ) ),	// @date 2013/03/05 <Add> OKADA
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// list
	kome::window::CheckGridListPanel* panel = new kome::window::CheckGridListPanel( this );	
	m_listPlugins = new PluginsListCtrl( panel, 600, 180 );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	panel->setDescription( "Select the plug-in." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	m_wxIdPluginsList = m_listPlugins->GetId();

	// engine
	TransferDataToWindow();

	onInitPackage( NULL );

	return sizer;
}

// on init package
void PluginManagerDlg::onInitPackage( void* ptr ){

	if( m_listPlugins == NULL ) {
		return;
	}

	onSelectPackage();

	// m_strSelectedPackageName : 選択されているパッケージ名
	std::string strSelectedPackage = PluginManagerDlgBase::getSelectedPackage();

	return;
}

// select package
int PluginManagerDlg::selectPackage( int index ){
	m_comboPackage->SetSelection( index );

	return 0;
}

// select package
int PluginManagerDlg::selectPackage( const char* pszPackage ){
	std::string strPackage = pszPackage;
	unsigned int uCount = m_comboPackage->GetCount();

	for( unsigned int i=0; i<uCount; i++ ){
		wxString wxsPackageCombobox = m_comboPackage->GetString( i );

		std::string strPackageCombobox = wxsPackageCombobox.c_str();
		
		if( strPackage.compare( strPackageCombobox ) == 0 ){
			// match
			m_comboPackage->SetSelection( i );
			return 0;
		}
	}

	return -1;
}

// on select package evt
void PluginManagerDlg::onSelectPackageEvt( wxCommandEvent& evt ){
	onSelectPackage();
}

// on select package
void PluginManagerDlg::onSelectPackage(){

	// m_strSelectedPackageName : 選択されているパッケージ名
	std::string strSelectedPackage = PluginManagerDlgBase::getSelectedPackage();

	m_strSelectedPackageName = strSelectedPackage;

	// FullとCustomizedの場合は、全部表示
	bool flgAllDisp = false;
	bool flgFull = false;
	bool flgCustomized = false;
	if( strSelectedPackage.compare( STR_PACKAGE_FULL ) == 0 ){
		flgAllDisp = true;
		flgFull = true;
	}else if( strSelectedPackage.compare( STR_PACKAGE_CUSTOMIZED ) == 0 ){
		flgAllDisp = true;
		flgCustomized = true;
	}

	// ここでPluginInfoからPlug-inを取得してm_listに入れる
	m_listPlugins->clearPlugins();

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	int nNum = plgMgr.getNumberOfPackages();
	int nNumPlugins = plgMgr.getNumberOfPlugins();

	kome::plugin::PluginInfo* ptrPlugin;
	for( int i=0; i<nNumPlugins; i++ ){
		ptrPlugin = plgMgr.getPlugin( i );

		// hiddenのものはリストに表示しない
		if( ptrPlugin->isHidden() ){	// @date 2013/08/23 <Add> OKADA
			continue;					// @date 2013/08/23 <Add> OKADA
		}								// @date 2013/08/23 <Add> OKADA

		const char* pszName = ptrPlugin->getName();
		const char* pszProvider = ptrPlugin->getProvider();
		const char* pszDir = ptrPlugin->getDir();

		bool flgHasPackage = false;
		unsigned int uCalls = ptrPlugin->getNumberOfCalls();
		for( unsigned int uIndexCall=0; uIndexCall<uCalls; uIndexCall++ ){

			kome::plugin::PluginCall* ptrPluginCall = ptrPlugin->getCall( uIndexCall );
			bool flgHas = ptrPluginCall->hasPackage( strSelectedPackage.c_str() );
			if( flgHas == true ){
				flgHasPackage = true;
				break;
			}
		}

		// Packageを含む/含まないに限らず全てのプラグインを表示する	// @date 2013/03/01 <Add> OKADA
		PLUGIN plugin;
		plugin.nCnt = i;
		plugin.strName = pszName;
		plugin.strProvider = pszProvider;
		plugin.strPath = pszDir;

		m_listPlugins->addPlugins( plugin );
		
		// Customized以外の場合は、ActiveTableを更新する
		if( flgCustomized == false ){
			PluginManagerView& mgr = PluginManagerView::getInstance();

			unsigned int uNumCompo = ptrPlugin->getNumberOfComponents();
			for( unsigned int uIndexCompo=0; uIndexCompo<uNumCompo; uIndexCompo++ ){
				kome::plugin::PluginComponent* plgCpmpo = ptrPlugin->getComponent( uIndexCompo );
				bool flgHas = plgCpmpo->hasPackage( strSelectedPackage.c_str() );

				if( ( flgHas == true )||( flgFull == true ) ){
					mgr.updateActiveTableCompo( i, uIndexCompo, true );
				}else{
					mgr.updateActiveTableCompo( i, uIndexCompo, false );
				}
			}
		}

	}

	m_listPlugins->create();
	m_listPlugins->Enable( true );
	m_listPlugins->updateList();
	onSelectPlugin();	// @date 2013/02/27 <Add> OKADA

	return;
}

// create functions sizer
wxSizer* PluginManagerDlg::createFunctionsSizer(){

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// title
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Components" ) ),	// @Date:2013/09/08	<Modified>	A.Ozaki
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// list
	kome::window::CheckGridListPanel::AllButtonAlign align = kome::window::CheckGridListPanel::TOP_ALL;	// @date 2013/04/23 <Add> OKADA

	kome::window::CheckGridListPanel* panel = new kome::window::CheckGridListPanel( this, -1, align, "All", false  );	// @date 2013/04/23 <Add> OKADA	// ○○Selectedを非表示にする処理(引数最後のfalse) 2013/04/23現在、上手く働かない

	m_listComponents = new ComponentsListCtrl( panel, 600, 180 );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	panel->setDescription( "Select available components." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	// engine
	TransferDataToWindow();

	onInitFunction( NULL );

	return sizer;
}

// on init function
void PluginManagerDlg::onInitFunction( void* ptr ){

	if( m_listComponents == NULL ) {
		return;
	}
	onSelectPlugin();

	// m_strSelectedPackageName : 選択されているパッケージ名
	std::string strSelectedPackage = PluginManagerDlgBase::getSelectedPackage();

	return;
}

// on select package
void PluginManagerDlg::onSelectPlugin(){
	// active状態を「active plugin-function table」へ格納

	// 選択されているパッケージ名
	std::string strSelectedPackage = PluginManagerDlgBase::getSelectedPackage();

	bool flgAll = false;
	if( strSelectedPackage.compare( STR_PACKAGE_FULL ) == 0 ){
		flgAll = true;
	}else if( strSelectedPackage.compare( STR_PACKAGE_CUSTOMIZED ) == 0 ){
		flgAll = true;
	}

	if( m_listPlugins == NULL ){
		return;
	}

	int nSelectedRawNoOfPlugin = m_listPlugins->getSelectedRawNo();

	if( nSelectedRawNoOfPlugin < 0 ){
		return;
	}
	if( (unsigned int)nSelectedRawNoOfPlugin < m_listPlugins->getNumberOfPlugins() ){	// @date 2013/05/13 <Mod> OKADA
		// OK
	}else{
		return;	// NG
	}

	PLUGIN plugin = m_listPlugins->getPlugin( nSelectedRawNoOfPlugin );

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	int nNumPlugins = plgMgr.getNumberOfPlugins();
	kome::plugin::PluginInfo* ptrPluginInfo = plgMgr.getPlugin( plugin.nCnt );

	PluginManagerView& mgr = PluginManagerView::getInstance();
	PluginManagerDlg* ptrDlg = mgr.getDlg();
	if( ptrDlg != NULL ){
		std::string strSelectedPackage = ptrDlg->getSelectedPackage();

		if( m_listComponents == NULL ){
			return;
		}
		m_listComponents->clearComponents();

		if( ptrPluginInfo != NULL ){
			
			unsigned int uNum = ptrPluginInfo->getNumberOfComponents();
			for( unsigned int index=0; index<uNum; index++ ){
				kome::plugin::PluginComponent* plgCpmpo = ptrPluginInfo->getComponent( index );
				if( plgCpmpo != NULL ){
					const char* szname = plgCpmpo->getName();
					int ddd = 0;

					// ここでPluginCallからFunctionを取得してm_listFunctionsに入れる
					COMPONENTS component;
					component.nPluginIndex   = plugin.nCnt;
					component.strName        = szname;
					component.uCompornentIndex = index;

					component.strDescription = plgCpmpo->getDesc();

#ifdef _DEBUG
					bool flgHas = plgCpmpo->hasPackage( strSelectedPackage.c_str() );
					component.flgActive = flgHas;
#endif

					int nRet = mgr.isActiveTableCompo( component.nPluginIndex, component.uCompornentIndex );
					if( nRet == 1 ){
						component.flgActive = true;
					}else{
						component.flgActive = false;
					}

					m_listComponents->addComponents( component );

				}
			}
		}
	}

	m_listComponents->create();
	m_listComponents->Enable( true );

	// >>>>>>	@Date:2013/09/08	<Add>	A.Ozaki
	// 「リスト下部の選択数が合わない」対策
	// 項目追加時にupdateListされているので、ここでは行いません
	//
//	m_listComponents->updateList();
	//
	// <<<<<<	@Date:2013/09/08	<Add>	A.Ozaki

	return;
}

// get window id plugins list
wxWindowID PluginManagerDlg::getWindowIdPluginsList(){
	return m_wxIdPluginsList;
}

// get selected package
std::string PluginManagerDlg::getSelectedPackage(){
	// m_strSelectedPackageName : 選択されているパッケージ名
	std::string strSelectedPackage = PluginManagerDlgBase::getSelectedPackage();

	return strSelectedPackage;
}

// Apply ボタン押下時 
void PluginManagerDlg::onApplyButton( wxCommandEvent& evt ) {

	// active 情報を変数に戻す

	unsigned int unNumFunc = m_listComponents->getNumberOfFunctions();

	// ウインドウ閉じる
	(this)->Close( true );

	SetReturnCode( wxID_APPLY );
}

// Close ボタン押下時 
void PluginManagerDlg::onCloseButton( wxCommandEvent& evt ) {

	(this)->Close( true );

	SetReturnCode( wxID_CANCEL );
}


// on size
void PluginManagerDlg::onSize( wxSizeEvent& evt ) {	// @date 2013/05/13 <Add> OKADA

	// リサイズを禁止
	wxSize sizeThisWindow0 = this->GetSize();
	if( m_nHeight < 0 ){
		m_nHeight = sizeThisWindow0.GetHeight();
	}
	if( m_nWidth < 0 ){
		m_nWidth = sizeThisWindow0.GetWidth();
	}

	this->SetSize( m_nWidth, m_nHeight );

	return;
}
