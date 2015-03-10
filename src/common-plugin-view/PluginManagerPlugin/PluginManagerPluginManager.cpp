/**
 * @file PluginManagerPluginManager.cpp
 * @brief implementation of PluginManager class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PluginManagerPluginManager.h"
#include "PluginManagerDlgBase.h"
#include "PluginManagerDlg.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginManagerView::PluginManagerView() {
	// initialize
	m_dlg = NULL;

	createActiveTable();	// PluginManager -> temporaryテーブル
}

// destructor
PluginManagerView::~PluginManagerView() {
}

// >>>>>>	@Date:2013/10/03	<Add>	A.Ozaki
//
#define	RESTART_OPTION			"/restart="
#define	RESTART_MESSAGE			"The program needs to be restarted to apply plug-in settings changes.\n   Do you want to restart?\n   Yes - Restart the program. (Operations on sample will be deleted if not saved. )\n   No - Cancel the plug-in settings changes."
//
// <<<<<<	@Date:2013/10/03	<Add>	A.Ozaki

int PluginManagerView::openDialog(){
	PluginManagerDlg dlg( kome::window::WindowTool::getMainWindow(), NULL );
	int nRet;

	m_dlg = &dlg;

	createActiveTable();	// PluginManager -> temporaryテーブル

	nRet = dlg.ShowModal();

	if( nRet == wxID_APPLY ){
// >>>>>>	@Date:2013/10/03	<Changed>	A.Ozaki
//   プラグイン管理から抜けた時の処理方法を変更します
//     変更時、自動的にMass++を再起動させます
//
		bool	bRestart = kome::window::WindowTool::showYesNo( RESTART_MESSAGE, "Yes", "No", "Restart Mass++" );

		if  ( true == bRestart )
		{
			writeMemoryActiveTable( );	// temporaryテーブル -> PluginManager
			saveActiveTable( );			// INIファイルへ保存

			// Mass++を再起動させます
			//
			STARTUPINFO	insInfo;
			ZeroMemory( &insInfo, sizeof( insInfo ) );
			PROCESS_INFORMATION	insProcessInfo;
			ZeroMemory( &insProcessInfo, sizeof( insProcessInfo ) );

			std::string	strCommand;
			DWORD	dwProcessId;

			// コマンドラインの作成
			//
			dwProcessId = getprocessid( );
			strCommand.clear( );

			for ( int i = 0; i < getargc() ; i++ )
			{
				const char	*arg = getargv( i );
				if  ( NULL != arg && 0 != strncmp( arg, RESTART_OPTION, strlen( RESTART_OPTION ) ) )
				{
					if  ( false == strCommand.empty( ) )
					{
						strCommand += " ";
					}
					strCommand += arg;
				}
			}

			// コマンドライン引数に起動待ちのための引数を追加します
			//  "/restart=プロセスID"
			//
			if  ( getargc( ) )
			{
				strCommand += FMT( " %s%d", RESTART_OPTION, dwProcessId );
			}

			if  ( 0 == CreateProcess( NULL,
									const_cast<char *>( strCommand.c_str( ) ),
									NULL,
									NULL,
									FALSE,
									CREATE_NO_WINDOW,
									NULL,
									NULL,
									&insInfo,
									&insProcessInfo ) )
			{
				// 再起動に失敗しました
				kome::window::WindowTool::showError( "Mass++ automatically restart failure." );

				LOG_FATAL( FMT( "Mass++ restart failure" ) );
				return	nRet;
			}

			// 強制終了させるためにMainFrameにイベントを通知します
			//
			wxWindow	*pWindow = kome::window::WindowTool::getMainWindow( );
			kome::window::MainFrame	*pMainFrame = dynamic_cast< kome::window::MainFrame * >( pWindow );

			if  ( pMainFrame )
			{
				wxCommandEvent	insEvent( pMainFrame->getUserEventType( kome::window::MainFrame::USER_EVENT_FORCE_EXIT ), wxID_ANY );
				insEvent.SetEventObject( pWindow );

				pMainFrame->GetEventHandler( )->AddPendingEvent( insEvent );
			}

			return nRet;
		}
//
// >>>>>>	@Date:2013/10/03	<Changed>	A.Ozaki
	}

	return nRet;
};

// on select package
int PluginManagerView::onSelectPackage(){
	
	if( m_dlg == NULL ){
		return -1;
	}

	m_dlg->onSelectPackage();
	return 0;
}

// get Dlg
PluginManagerDlg* PluginManagerView::getDlg(){
	return m_dlg;
};

// create active table
void PluginManagerView::createActiveTable(){

	m_vectActiveTable.clear();

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int nNumPlugins = plgMgr.getNumberOfPlugins();


	for( int i=0; i<nNumPlugins; i++ ){
		ACTIVE_TABLE activeTable;

		kome::plugin::PluginInfo* ptrPluginInfo;
		ptrPluginInfo = plgMgr.getPlugin( i );

		const char* pszName = ptrPluginInfo->getName();
		const char* pszProvider = ptrPluginInfo->getProvider();
		const char* pszDir = ptrPluginInfo->getDir();

/*		unsigned int uCalls = ptrPluginInfo->getNumberOfCalls();
		for( unsigned int uIndexCall=0; uIndexCall<uCalls; uIndexCall++ ){
			kome::plugin::PluginCall* ptrPluginCall = ptrPluginInfo->getCall( uIndexCall );

			activeTable.strPluginName  = ptrPluginInfo->getName();
			activeTable.nPluginIndex = i;

			activeTable.strFunctionName = ptrPluginCall->getName();
			activeTable.uFunctionIndex = uIndexCall;

			activeTable.flgActiveTemp = ptrPluginCall->isActive();
			m_vectActiveTable.push_back( activeTable );
					
		}
*/

		unsigned int uNumCompo = ptrPluginInfo->getNumberOfComponents();
		for( unsigned int uIndexCompo=0; uIndexCompo<uNumCompo; uIndexCompo++ ){
			kome::plugin::PluginComponent* plgCpmpo = ptrPluginInfo->getComponent( uIndexCompo );

			activeTable.strPluginName  = ptrPluginInfo->getName();
			activeTable.nPluginIndex = i;

			activeTable.strComponentName = plgCpmpo->getName();
			activeTable.uComponentIndex = uIndexCompo;

			activeTable.flgActiveTemp = plgCpmpo->isEnable();
			m_vectActiveTable.push_back( activeTable );
		}

	}

	return;
}

// clear active table
void PluginManagerView::clearActiveTable(){
	m_vectActiveTable.clear();
}

// 1:true 0:false -1:nothing -2:Parameter error
// check ative table
int PluginManagerView::checkActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex ){

	// 引数チェック
	if( nPluginIndex < 0 ){
		return -2;
	}

	unsigned int uNum = m_vectActiveTable.size();
	for( unsigned int i=0; i<uNum; i++ ){
		if( ( m_vectActiveTable[i].nPluginIndex == nPluginIndex )&&
			( m_vectActiveTable[i].uComponentIndex == uComponentIndex ) )	{
			if( m_vectActiveTable[i].flgActiveTemp == true ){
				return 1;
			}else{
				return 0;
			}
		}
	}

	// 該当無し
	return -1;
}


bool PluginManagerView::isActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex ){

	int nRet = checkActiveTableCompo( nPluginIndex, uComponentIndex );

	if( nRet == 1 ){
		return true;
	}

	return false;
}

int PluginManagerView::updateActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex, bool flgActive ){
	// 引数チェック
	if( nPluginIndex < 0 ){
		return -2;
	}

	unsigned int uNum = m_vectActiveTable.size();
	for( unsigned int i=0; i<uNum; i++ ){
		if( ( m_vectActiveTable[i].nPluginIndex == nPluginIndex )&&
			( m_vectActiveTable[i].uComponentIndex == uComponentIndex ) )	{

			m_vectActiveTable[i].flgActiveTemp = flgActive;

			if( m_vectActiveTable[i].flgActiveTemp == true ){
				return 1;
			}else{
				return 0;
			}
		}
	}

	// 該当無し
	return -1;
}

// write memory active table
void PluginManagerView::writeMemoryActiveTable(){

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int nNumPlugins = plgMgr.getNumberOfPlugins();

	for( int i=0; i<nNumPlugins; i++ ){
		ACTIVE_TABLE activeTable;

		kome::plugin::PluginInfo* ptrPluginInfo;
		ptrPluginInfo = plgMgr.getPlugin( i );

		const char* pszName = ptrPluginInfo->getName();
		const char* pszProvider = ptrPluginInfo->getProvider();
		const char* pszDir = ptrPluginInfo->getDir();

		unsigned int uNumCompo = ptrPluginInfo->getNumberOfComponents();
		for( unsigned int uIndexCompo=0; uIndexCompo<uNumCompo; uIndexCompo++ ){
			kome::plugin::PluginComponent* plgCpmpo = ptrPluginInfo->getComponent( uIndexCompo );

			activeTable.strPluginName  = ptrPluginInfo->getName();
			activeTable.nPluginIndex = i;

			activeTable.strComponentName = plgCpmpo->getName();
			activeTable.uComponentIndex = uIndexCompo;

			activeTable.flgActiveTemp = plgCpmpo->isEnable();

			// ここでplugin-info経由でcompoにenable / disable 設定する処理を追加 2013/08/22
			int nRet = checkActiveTableCompo( activeTable.nPluginIndex, activeTable.uComponentIndex );	// @date 2013/08/27 <Add> OKADA
			if( nRet == 0 ){
				ptrPluginInfo->resetEnableComponent( uIndexCompo );
			}else if( nRet == 1 ){
				ptrPluginInfo->setEnableComponent( uIndexCompo );
			}else{
				ptrPluginInfo->setEnableComponent( uIndexCompo );
			}
		}		
	}

	// status check
	std::set< std::string > statusSet;
	std::vector< std::pair< std::string, std::string > > statuses;
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );
		for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
			kome::plugin::PluginCall* call = plugin->getCall( j );
			if( call->isActive() ) {
				for( unsigned int k = 0; k < call->getNumberOfStatuses(); k++ ) {
					std::string n = call->getStatusName( k );
					std::string v = call->getStatusValue( k );
					std::string status = FMT( "%s:%s", n.c_str(), v.c_str() );
					statusSet.insert( status );
					statuses.push_back( std::make_pair( n, v ) );
				}
			}
		}
	}

	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	for( int i = (int)statusMgr.getNumberOfStatuses() - 1; i >= 0; i-- ) {
		std::string key = statusMgr.getStatusName( i );
		std::string val = statusMgr.getStatusValue( i );

		std::string status = FMT( "%s:%s", key.c_str(), val.c_str() );

		if( statusSet.find( status ) == statusSet.end() ) {
			statusMgr.setValue( key.c_str(), NULL );
		}
	}

	return;
}

// ActiveTableをINIファイルに保存
void PluginManagerView::saveActiveTable(){

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini

	// パッケージを保存
	const char* SECTION_NAME_HEAD="PluginManagerCompo";

	char szIniSectionName[256];
	char szIniParameterName[256];

	if( m_dlg != NULL ){
		std::string strSelectedPackage = m_dlg->getSelectedPackage();

		sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, "SelectedPackage" );
		sprintf( szIniParameterName, "SelectedPackage" );

		ini->setString( szIniSectionName, szIniParameterName, strSelectedPackage.c_str() );
	}

	// active情報を保存
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int nNumPlugins = plgMgr.getNumberOfPlugins();

	for( int i=0; i<nNumPlugins; i++ ){
		ACTIVE_TABLE activeTable;

		kome::plugin::PluginInfo* ptrPluginInfo;
		ptrPluginInfo = plgMgr.getPlugin( i );

		const char* pszName = ptrPluginInfo->getName();
		const char* pszProvider = ptrPluginInfo->getProvider();
		const char* pszDir = ptrPluginInfo->getDir();

/*		unsigned int uCalls = ptrPluginInfo->getNumberOfCalls();
		for( unsigned int uIndexCall=0; uIndexCall<uCalls; uIndexCall++ ){
			kome::plugin::PluginCall* ptrPluginCall = ptrPluginInfo->getCall( uIndexCall );

			activeTable.strPluginName  = ptrPluginInfo->getName();
			activeTable.nPluginIndex = i;

			activeTable.strFunctionName = ptrPluginCall->getName();
			activeTable.uFunctionIndex = uIndexCall;

			activeTable.flgActiveTemp = ptrPluginCall->isActive();

			sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, activeTable.strPluginName.c_str() );
			sprintf( szIniParameterName, "%s", ptrPluginCall->getName() );	// @date 2013/04/23 <有効化> OKADA

			ini->setBool( szIniSectionName, szIniParameterName, ptrPluginCall->isActive() );
		}
*/
		unsigned int uNumCompo = ptrPluginInfo->getNumberOfComponents();
		for( unsigned int uIndexCompo=0; uIndexCompo<uNumCompo; uIndexCompo++ ){
			kome::plugin::PluginComponent* plgCpmpo = ptrPluginInfo->getComponent( uIndexCompo );

			activeTable.strPluginName  = ptrPluginInfo->getName();
			activeTable.nPluginIndex = i;

//			activeTable.strFunctionName = ptrPluginCall->getName();
//			activeTable.uFunctionIndex = uIndexCall;

			activeTable.strComponentName = plgCpmpo->getName();
			activeTable.uComponentIndex = uIndexCompo;

			activeTable.flgActiveTemp = plgCpmpo->isEnable();

			sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, activeTable.strPluginName.c_str() );
			sprintf( szIniParameterName, "%s", plgCpmpo->getName() );	

			bool bEnable = isActiveTableCompo( activeTable.nPluginIndex, activeTable.uComponentIndex );
			// ここでinfo経由combo enable/disable関数を呼ぶ 2013/08/22 <Add> OKADA

			ini->setBool( szIniSectionName, szIniParameterName, bEnable );
		}
	}
}

// select package
int PluginManagerView::selectPackage( int index ){
	if( m_dlg != NULL ){
		m_dlg->selectPackage( index );
	}

	return 0;
}

// select package
int PluginManagerView::selectPackage( const char* pszPackage ){
	if( m_dlg != NULL ){
		m_dlg->selectPackage( pszPackage );
	}
	return 0;
}

// get MatrixViewManager object
PluginManagerView& PluginManagerView::getInstance() {
	// create object
	static PluginManagerView mgr;

	return mgr;
}
