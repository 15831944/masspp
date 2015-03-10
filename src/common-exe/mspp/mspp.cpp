/**
 * @file mspp.cpp
 * @brief mspp implements
 *
 * @author S.Tanaka
 * @date 2006.07.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AppInfo.h"

#include <set>
#include <list>
#include <vector>
#include <string>
#include <boost/bind.hpp>
#include <wx/image.h>
#include <wx/aboutdlg.h>

#include "mspp16x16.xpm"
#include "mspp48x48.xpm"

#include "mspp.h"

#include <wx/html/helpctrl.h>
#include <wx/msw/registry.h>
#include <wx/process.h>

#include <wx/splash.h>


using namespace kome::mspp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PLUGIN_INIT_METHOD_TYPE_NAME      "INIT"
#define PLUGIN_EXIT_METHOD_TYPE_NAME      "EXIT"

#define COMMAND_FUNC_TYPE                 "COMMAND"

#define FRAME_DEFAULT_WIDTH               840
#define FRAME_DEFAULT_HEIGHT              520
#define ID_ABOUT                          30001

// -> 2010.10.13 Add M.Izumi
#define SECTION							 "WINDOW"
#define RECT_X_PARAM_KEY				 "RECT_X"
#define RECT_Y_PARAM_KEY				 "RECT_Y"
#define RECT_HEIGHT_PARAM_KEY			 "RECT_HEIGHT"
#define RECT_WIDTH_PARAM_KEY			 "RECT_WIDTH"
#define MAXIMIZ							 "MAXIMIZED"
// <-

IMPLEMENT_APP( MsppApp )

BEGIN_EVENT_TABLE( MsppApp, wxApp )
    EVT_MENU( wxID_ABOUT, MsppApp::onAbout )
	EVT_KEY_DOWN( MsppApp::onKeyDown )
    EVT_KEY_UP( MsppApp::onKeyUp )
END_EVENT_TABLE()


// constructor
MsppApp::MsppApp() {
}

// destructor
MsppApp::~MsppApp() {
}

// show about dialog
void MsppApp::showAboutDialog() {
    // manager
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

    // version
    wxAboutDialogInfo info;
    std::string version = FMT( 
		"%s (build: %s %s)",
		msppMgr.getVersion(),
		__DATE__,
		__TIME__
	);

    // icon
    wxIcon icon( mspp48x48_xpm );

    // information
	std::string name = FMT( "%s (%s)", msppMgr.getAppName(), msppMgr.getPlatformName() );
    info.SetName( wxT( name.c_str() ) );
    info.SetVersion( wxT( version.c_str() ) );
    info.SetCopyright( wxT( APP_COPYRIGHT ) );
    info.SetIcon( icon );
    info.SetWebSite( wxT( APP_WEBSITE ) );

    // show
    wxAboutBox( info );
}

// >>>>>>	@Date:2013/10/04	<Add>	A.Ozaki
//
#define	RESTART_OPTION		"/restart="
#define	SPLASH_STANDARD		"Mass++Splash.png"
#define	SPLASH_WAITING		"Mass++Splash_waiting.png"
//
// <<<<<<	@Date:2013/10/04	<Add>	A.Ozaki


#define SPLASH_STYLE   ( wxFRAME_NO_TASKBAR | wxTRANSPARENT_WINDOW | wxFRAME_SHAPED | wxNO_BORDER )

// OnInit
bool MsppApp::OnInit() {

// >>>>>>	@Date:2013/10/03	<Add>	A.Ozaki
//   プラグイン管理で設定変更時に再起動要求されたときの対応
//
    // initialize image handlers
    wxInitAllImageHandlers( );

	wxBitmap	insBitmap;
	wxSplashScreen	*pSplash = NULL;

	for ( int i = 0 ; i < argc ; i++ )
	{
		if  ( (char *)NULL != argv[i] && 0 == strncmp( argv[i], RESTART_OPTION, strlen( RESTART_OPTION ) ) )
		{
			std::string	strTemp = argv[i];

			size_t	szPos = strTemp.find_last_of( "=" );
			if  ( std::string::npos == szPos )
			{
				// 引数に誤りがあるので、この時点で処理を終了します
				LOG_FATAL( FMT( "Application rebooting argument failure." ) );
				return	false;
			}
			szPos++;
			if  ( strTemp.size( ) <= szPos )
			{
				// 引数に誤りがあるので、この時点で処理を終了します
				LOG_FATAL( FMT( "Application rebooting argument failure." ) );
				return	false;
			}

			// 再起動待ち用のスプラッシュの設定
			//
			if  ( NULL == pSplash && ( 0 == access( SPLASH_WAITING, 0 ) ) )
			{
				if  ( insBitmap.LoadFile( SPLASH_WAITING, wxBITMAP_TYPE_PNG ) )
				{
					pSplash = new wxSplashScreen( insBitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
									6000, NULL, -1, wxDefaultPosition, wxDefaultSize, SPLASH_STYLE );
					wxYield( );
				}
			}
			std::string	strProcessId = strTemp.substr( szPos );

			if  ( false == strProcessId.empty( ) )
			{
				DWORD	dwId = atol( strProcessId.c_str( ) );
				DWORD	dwWaiting = 30 * 1000;	// 30秒待ち
				while ( true == processexists( dwId ) )
				{
					DWORD	dwSleep = 500;

					Sleep( dwSleep );
					dwWaiting -= dwSleep;
					if  ( 0 == dwWaiting )
					{
						// Error
						LOG_FATAL( FMT( "Timeout waiting..." ) );
						break;
					}
				}
			}

			break;
		}
	}

	// @date 2011.02.16 <Add> M.Izumi ->
	// Double-check Launch

    m_checker = new wxSingleInstanceChecker(GetAppName());
    if ( m_checker->IsAnotherRunning() )
    {
        wxLogError(_("Another program instance is already running, aborting."));
		delete m_checker;
        return false;
    }
	//<-

	// >>>>>>	@Date:2013/10/04	<Add>	A.Ozaki
	//
	// 標準スプラッシュの準備
	//
	if  ( NULL == pSplash && ( 0 == access( SPLASH_STANDARD, 0 ) ) )
	{
		if  ( insBitmap.LoadFile( SPLASH_STANDARD, wxBITMAP_TYPE_PNG ) )
		{
			pSplash = new wxSplashScreen( insBitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
										6000, NULL, -1, wxDefaultPosition, wxDefaultSize, SPLASH_STYLE );
			wxYield( );
		}
	}
	//
	// <<<<<<	@Date:2013/10/04	<Add>	A.Ozaki

	// initialize
    if( !init() ) {
		LOG_FATAL( FMT( "Failed to initialize environment." ) );
		finalize();
		return false;
	}

	// managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::plugin::CommandManager& cmdMgr = kome::plugin::CommandManager::getInstance();

	// help
	for( int i = 0; i < getargc(); i++ ) {
		const char* arg = getargv( i );
		if( arg != NULL && strcmp( arg, "--help" ) == 0 ) {
			wxSafeShowMessage( wxT( "Usage" ), wxT( cmdMgr.getUsage() ) );
			return false;
		}
	}

	// ini
	kome::core::IniFile* ini = msppMgr.getIniFile();
	int iX = ini->getInt( SECTION, RECT_X_PARAM_KEY, 0 );
	int iY = ini->getInt( SECTION, RECT_Y_PARAM_KEY, 0 );
	int iHeight = ini->getInt( SECTION, RECT_HEIGHT_PARAM_KEY, FRAME_DEFAULT_HEIGHT );
	int iWidth = ini->getInt( SECTION, RECT_WIDTH_PARAM_KEY, FRAME_DEFAULT_WIDTH );
	bool maximize = ini->getBool( SECTION, MAXIMIZ, false );

	// create frame
    kome::window::MainFrame* mainFrame = kome::window::WindowTool::createMainWindow(
		msppMgr.getAppName(),
		iX,
		iY,
		iWidth,
		iHeight		
	);

    if( mainFrame == NULL ) {
		LOG_FATAL_CODE(
			FMT(
			"Failed to create main frame window."
			),
			ERR_OTHER
		);
		return false;
    }

	// SPEC No.85550: Mass++ does not remember its maximised state. @date 2012.04.18 <Add> M.Izumi
	mainFrame->Maximize( maximize );	

    // set icon
    wxIcon icon( mspp16x16_xpm );
    mainFrame->SetIcon( icon );

	// add about menu
    wxMenuBar* menuBar = mainFrame->GetMenuBar();
    if( menuBar != NULL ) {
		// help menu
		wxMenu* menu = NULL;
		int m = menuBar->FindMenu( "Help" );
		if( m == wxNOT_FOUND ) {
			menu = new wxMenu();
			menuBar->Append( menu, wxT( "&Help..." ) );
		}
		else {
			menu = menuBar->GetMenu( m );
			menu->AppendSeparator();
		}
		
		// add "About"
		if( menu != NULL ) {    // add to menu
			std::string aboutMenuName = FMT( "&About %s...", msppMgr.getAppName() );
			wxMenuItem* aboutItem = new wxMenuItem(
				menu,
				wxID_ABOUT,
				wxT( aboutMenuName.c_str() ),
				wxT( "Show product information" )
			);

			aboutItem->SetBitmap( icon );
			menu->Append( aboutItem );
		}
    }
	
    // show main frame
    mainFrame->Show( true );
	
    SetTopWindow( mainFrame );

    // log
    setloglistsize( 0 );
    if( getloglevel() > MAX_LOG_LEVEL ) {
		setloglevel( LOG_LEVEL_NONE );
    }

    // command
    executeCommand();

	wxHtmlHelpController hc;	// 2010/08/16 APPEND OKADA
	// ステータスバーに各モードの状態をセット @date 2011.09.14 <Add> M.Izumi
	setStatusModeValStatusBar();
	
	return true;
}

// On Exit
int MsppApp::OnExit() {
    // finalize
    finalize();
	delete m_checker;	// @date 2011.02.16 <Add> M.Izumi

    return wxApp::OnExit();
}

// This method is called when "About Mass++..." menu is called.
void MsppApp::onAbout( wxCommandEvent& evt ) {
    showAboutDialog();
}

// This method is called when a key is down
void MsppApp::onKeyDown( wxKeyEvent& evt ) {
	// active window
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );
	kome::window::CanvasBase* cbase = dynamic_cast< kome::window::CanvasBase* >( canvas );
	kome::img::Graphics* g = ( cbase == NULL ? (kome::img::Graphics*)NULL : &( cbase->getGraphics() ) );

	// active object
	kome::objects::Variant var;
	if( frame != NULL ) {
		var = frame->getActiveObject();
	}
	kome::objects::MsDataVariant obj( var );

	// key event
	kome::evt::KeyEvent e;
	setKeyEvent( evt, e );

	// key event
	bool updated = false;
	// SPEC No.91758 @date 2013.04.02 <Mod> M.Izumi ->
	if( canvas == NULL || wxWindow::FindFocus() == canvas->GetParent() ){ 
		kome::objects::Spectrum* spec = obj.getSpectrum();
		if( spec != NULL ) {
			updated = kome::plugin::PluginCallTool::onSpectrumKeyDown( *spec, g, e, canvas );
		}

		kome::objects::Chromatogram* chrom = obj.getChromatogram();
		if( chrom != NULL && !updated ) {
			updated = kome::plugin::PluginCallTool::onChromatogramKeyDown( *chrom, g, e, canvas );
		}

		kome::objects::DataMapInfo* dm = obj.getDataMap();
		if( dm != NULL && !updated ) {
			updated = kome::plugin::PluginCallTool::onDataMapKeyDown( *dm, g, e, canvas );
		}
	}
	// SPEC No.91758 @date 2013.04.02 <Mod> M.Izumi <-

	// skip
	if( updated ) {
		kome::window::WindowTool::refresh();
	}
	else {
		evt.Skip();
	}
}

// This method is called when a key is up
void MsppApp::onKeyUp( wxKeyEvent& evt ) {
	// active window
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );
	kome::window::CanvasBase* cbase = dynamic_cast< kome::window::CanvasBase* >( canvas );
	kome::img::Graphics* g = ( cbase == NULL ? (kome::img::Graphics*)NULL : &( cbase->getGraphics() ) );

	// active object
	kome::objects::Variant var;
	if( frame != NULL ) {
		var = frame->getActiveObject();
	}
	kome::objects::MsDataVariant obj( var );

	// key event
	kome::evt::KeyEvent e;
	setKeyEvent( evt, e );

	// key event
	bool updated = false;

	kome::objects::Spectrum* spec = obj.getSpectrum();
	if( spec != NULL ) {
		updated = kome::plugin::PluginCallTool::onSpectrumKeyUp( *spec, g, e, canvas );
	}

	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	if( chrom != NULL && !updated ) {
		updated = kome::plugin::PluginCallTool::onChromatogramKeyUp( *chrom, g, e, canvas );
	}

	kome::objects::DataMapInfo* dm = obj.getDataMap();
	if( dm != NULL && !updated ) {
		updated = kome::plugin::PluginCallTool::onDataMapKeyUp( *dm, g, e, canvas );
	}

	// skip
	if( updated ) {
		kome::window::WindowTool::refresh();
	}
	else {
		evt.Skip();
	}
}

// initialize
bool MsppApp::init() {
    // set arguments
    setarguments( argc, argv );

#ifdef _MSC_VER
    // coinitialize
    CoInitialize( NULL );
#endif	// _MSC_VER

    // initialize image handlers
// >>>>>>	@Date:2013/10/04	<Delete>	A.Ozaki
// スプラッシュ表示のためにonInitの先頭に移動させました
//
//    wxInitAllImageHandlers();
//
// <<<<<<	@Date:2013/10/04	<Delete>	A.Ozaki

    // managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
    kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::CommandManager& cmdMgr = kome::plugin::CommandManager::getInstance();

    // log
    setloglevel( MAX_LOG_LEVEL + 1 );
    setloglistsize( -1 );

    // get iniFile
    std::string iniFilePath = getabsolutepath( msppMgr.getConfDir(), INI_FILE );
    kome::core::IniFile* ini = kome::core::IniFile::getIniFile( iniFilePath.c_str() );
    msppMgr.setIniFile( ini );

	if( ini != NULL ) {
		for( unsigned int i = 0; i < ini->getNumberOfSections(); i++ ) {
			std::string section = ini->getSection( i );
			for( unsigned int j = 0; j < ini->getNumberOfParameters( section.c_str() ); j++ ) {
				std::string key = ini->getParameterName( section.c_str(), j );
				std::string val = kome::plugin::SettingsValue::convertToDisplayingString(
					ini->getParameterValue( section.c_str(), j )
				);

				ini->setString( section.c_str(), key.c_str(), val.c_str() );
			}
		}
	}

    // parameters file
	// @date 2011.01.18 <Mod> M.Izumi ->
	// Spec No.79362 パラメータ設定ファイルの名前を変更してもパラメータが設定されない。
	const char* fileName = ini->getString( "Parameters", "FILE_PATH", PARAMS_FILE );
	std::string paramsFilePath = getabsolutepath( msppMgr.getConfDir(), fileName );
    // <-
	kome::core::IniFile* params = kome::core::IniFile::getIniFile( paramsFilePath.c_str() );
    msppMgr.setParameters( params );

    // get status
    if( ini != NULL ) {
		statusMgr.getFromIniFile( *ini, STATUS_SECTION );
    }

    // initialize socket
#ifdef _MSC_VER
    WSADATA data;
    WSAStartup( MAKEWORD(2, 0), &data );
#endif	// _MSC_VER

    // initialize xerces
    if( !kome::xml::XercesTool::initXml() ) {
		LOG_FATAL( FMT( "Failed to initialize XML." ) );
		return false;
	}

    // get plugin
	loadPluginFiles();
	loadOtherFolderPluginFiles();
	if( plgMgr.getNumberOfPlugins() == 0 ) {
		LOG_FATAL( FMT( "Failed to load plug-in files." ) );
		finalize();
		return false;
	}

	{	// @date 2013/05/24 <Add> OKADA
		// get plug-in management object
	    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		plgMgr.setActiveEnable( true );
		plgMgr.updateActiveLookupTable();
	}

	// @Date:2014/02/24	<Moved>	A.Ozaki
	loadActiveTable();	// @date 2013/02/26 <Add> OKADA	// @date 2013/04/17 <Add> OKADA	(再度有効化)

	// initialize
    cmdMgr.init( COMMAND_FUNC_TYPE );

    // call init plug-in functions
    plgMgr.callAllFunctions( NULL, PLUGIN_INIT_METHOD_TYPE_NAME );

    return true;
}

// finalize the application
void MsppApp::finalize() {
    // managers
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

    // ini file
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
    kome::core::IniFile* iniFile = msppMgr.getIniFile();
    if( iniFile != NULL ) {
		// set status
		kome::objects::StatusManager::getInstance().setToIniFile( *iniFile, STATUS_SECTION );
		
		// save
		iniFile->save();
	}
	
	kome::core::IniFile* params = msppMgr.getParameters();
	if( params != NULL ) {
		params->save();
	}
	
	// call exit plug-in functions
	plgMgr.callAllFunctions( NULL, PLUGIN_EXIT_METHOD_TYPE_NAME );
	
    // clean up socket
#ifdef _MSC_VER
    WSACleanup(); 
#endif	// _MSC_VER

	// deallocate error strings object
	errorcleanup();

    // terminate XML
    kome::xml::XercesTool::terminateXml();

#ifdef _MSC_VER
    // couninitialize
    CoUninitialize();
#endif	// _MSC_VER
}

// load plug-in files
bool MsppApp::loadPluginFiles() {
    // managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

    // plug-in path
    std::string pluginPath = getpath( msppMgr.getMsppDir(), PLUGIN_DIR );

    // set XML schema
    std::string pluginXsd = getpath( msppMgr.getMsppDir(), PLUGIN_XML_SCHEMA_FILE );
    std::string settingsXsd = getpath( msppMgr.getMsppDir(), SETTINGS_XML_SCHEMA_FILE );

    plgMgr.setXMLSchema( pluginXsd.c_str(), settingsXsd.c_str() );

    // load plugins
    if( !plgMgr.loadPath( pluginPath.c_str(), isPluginPath ) ) {
		return false;
	}
	
	return true;
}

// execute command
void MsppApp::executeCommand() {
	// managers
    kome::plugin::CommandManager& cmdMgr = kome::plugin::CommandManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	// execute
	if( !cmdMgr.empty() ) {
		if( !cmdMgr.execute() ) {

			// After calling getlasterrormessage once, the error is deleted.
			// We want to use it twice, so make a copy first.
			std::string lasterrormessage = getlasterrormessage();
			if (lasterrormessage.size() > 0)
			{
				kome::window::WindowTool::showError( lasterrormessage.c_str() );
			}
			else
			{
				// LOG_ERROR_CODE should have been called with an
				// error message to display to the user.
				_ASSERT(FALSE);
			}
		}
	}
}

// judge specified file path is plug-in file.
bool MsppApp::isPluginPath( const char* path ) {
    // get child
	std::string child = getfilename( path );
	if( child.empty() ) {
		return false;
	}
	
	// check
	if( isdirectory( path ) ) {    // directory
		return ( child.at( 0 ) != '.' );
	}
	
    return ( child.compare( PLUGIN_XML_FILE ) == 0 );
}

// set key event
void MsppApp::setKeyEvent( wxKeyEvent& evt0, kome::evt::KeyEvent& evt1 ) {
	// code
	static kome::core::ConversionElement< int, int > keyCodeConvertArray[] = {
		{ WXK_BACK,	  kome::core::key::KEY_BACKSPACE },
		{ WXK_TAB,	   kome::core::key::KEY_TAB },
		{ WXK_RETURN,	kome::core::key::KEY_RETURN },
		{ WXK_SPACE,	 kome::core::key::KEY_SPACE },
		{ WXK_DELETE,	kome::core::key::KEY_DELETE },
		{ WXK_SHIFT,	 kome::core::key::KEY_SHIFT },
		{ WXK_ALT,	   kome::core::key::KEY_ALT },
		{ WXK_CONTROL,   kome::core::key::KEY_CONTROL },
		{ WXK_PAUSE,	 kome::core::key::KEY_PAUSE },
		{ WXK_END,	   kome::core::key::KEY_END },
		{ WXK_HOME,	  kome::core::key::KEY_HOME },
		{ WXK_LEFT,	  kome::core::key::KEY_LEFT },
		{ WXK_UP,		kome::core::key::KEY_UP },
		{ WXK_RIGHT,	 kome::core::key::KEY_RIGHT },
		{ WXK_DOWN,	  kome::core::key::KEY_DOWN },
		{ WXK_INSERT,	kome::core::key::KEY_INSERT },
		{ WXK_F1,		kome::core::key::KEY_F1 },
		{ WXK_F2,		kome::core::key::KEY_F2 },
		{ WXK_F3,		kome::core::key::KEY_F3 },
		{ WXK_F4,		kome::core::key::KEY_F4 },
		{ WXK_F5,		kome::core::key::KEY_F5 },
		{ WXK_F6,		kome::core::key::KEY_F6 },
		{ WXK_F7,		kome::core::key::KEY_F7 },
		{ WXK_F8,		kome::core::key::KEY_F8 },
		{ WXK_F9,		kome::core::key::KEY_F9 },
		{ WXK_F10,	   kome::core::key::KEY_F10 },
		{ WXK_F11,	   kome::core::key::KEY_F11 },
		{ WXK_F12,	   kome::core::key::KEY_F12 },
		{ WXK_F13,	   kome::core::key::KEY_F13 },
		{ WXK_F14,	   kome::core::key::KEY_F14 },
		{ WXK_F15,	   kome::core::key::KEY_F15 },
		{ WXK_F15,	   kome::core::key::KEY_F16 },
		{ WXK_NUMLOCK,   kome::core::key::KEY_NUMLOCK },
		{ WXK_SCROLL,	kome::core::key::KEY_SCROLL },
		{ WXK_PAGEUP,	kome::core::key::KEY_PAGEUP },
		{ WXK_PAGEDOWN,  kome::core::key::KEY_PAGEDOWN },
		{ WXK_ESCAPE,   kome::core::key::KEY_ESCAPE }
	};

	// set code
	int code = CONVERT_FROM_ARRAY( evt0.GetKeyCode(), 0, keyCodeConvertArray );
	if( code == 0 ) {
		code = evt0.GetKeyCode();
	}

	evt1.setCode( code );

	// flags
	evt1.shift( evt0.ShiftDown() );
	evt1.ctrl( evt0.ControlDown() );
	evt1.alt( evt0.AltDown() );
}

// ステータスバーに各モードの設定を表示する @date 2011.09.14 <Add> M.Izumi
void MsppApp::setStatusModeValStatusBar()
{	
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	kome::window::StatusBar&      statusBar = kome::window::StatusBar::getInstance(); 
	
	// Display Mode
	const char* chStatus = statusMgr.getValue( STATUS_DISP );
	std::string strStatus = NVL( chStatus, "" );
	std::string strTmp="";
	statusBar.setFlgDisp( STATUSBAR_DISP, 1 );
	std::string strMode = MODE_DISP;
	if( strStatus.compare( "Canvas" ) == 0 ){
		strTmp =  strMode+"Single";
	}else if( strStatus.compare( "Overlapping" ) == 0 ){
		strTmp = strMode+"Overlapping";
	}else if( strStatus.compare( "vertical" ) == 0 ){
		strTmp = strMode+"Stacked";
	}
	statusBar.setStatusMsg( STATUSBAR_DISP, strTmp.c_str() );
	
	// Chromatogram Filter
	strStatus="";
	strTmp="";
	strMode = MODE_CHROM;
	statusBar.setFlgDisp( STATUSBAR_CHROM, 1 );
	chStatus = statusMgr.getValue( STATUS_CHROM );
	strStatus = NVL( chStatus, "" );
	if( strStatus.compare( "all" ) == 0 ){
		strTmp = strMode+"All";
	}else if( strStatus.compare( "ms" ) == 0 ){
		strTmp = strMode+"MS";
	}else if( strStatus.compare( "msms" ) == 0 ){
		strTmp = strMode+"MSn";
	}
	statusBar.setStatusMsg( STATUSBAR_CHROM, strTmp.c_str() );
	
	// Synchronaize Axes
	strStatus="";
	strTmp="";
	const char* sync = statusMgr.getValue( STATUS_SYNC );
	strStatus = NVL( sync, "" );
	if( strStatus.compare( "on" ) == 0 ){
		strTmp = MODE_SYNC;
	}
	statusBar.setFlgDisp( STATUSBAR_SYNC, 1 );
	statusBar.setStatusMsg(STATUSBAR_SYNC, strTmp.c_str() );
	
	// Edit Peaks
	strStatus="";
	strTmp="";
	const char* editMode = statusMgr.getValue( STATUS_EDIT );
	strStatus = NVL( editMode, "" );
	if( strStatus.compare( "edit" ) == 0 || strStatus.compare("add") == 0 ){
		strTmp = MODE_EDIT;
	}
	statusBar.setFlgDisp( STATUSBAR_EDIT, 1 );
	statusBar.setStatusMsg(STATUSBAR_EDIT, strTmp.c_str() );
}

// @date 2011.10.21 <Add> M.Izumi
// 環境変数”MSPP_PLUGIN_DIR”で指定されたPathからプラグイン取得
// load plug-in files 
bool MsppApp::loadOtherFolderPluginFiles() {
    // managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::CommandManager& cmdMgr = kome::plugin::CommandManager::getInstance();
	
	std::vector< std::string > strPaths;
	strPaths = 	msppMgr.getPluginDir();
	
	for( unsigned int i=0; i < strPaths.size(); i++ ){
		// plug-in path
		std::string pluginPath = strPaths[i].c_str();
		
		// load plugins
		if( !plgMgr.loadPath( pluginPath.c_str(), isPluginPath ) ) {
			return false;
		}
	}

    // parse option
	if( cmdMgr.parseOption( argc, argv ) ) {
		for( unsigned int i = 0; i < cmdMgr.getNumberOfPluginPaths(); i++ ) {
			plgMgr.loadPath( cmdMgr.getPluginPath( i ), isPluginPath );
		}
	}
	else {
		// After calling getlasterrormessage once, the error is deleted.
		// We want to use it twice, so make a copy first.
		std::string lasterrormessage = getlasterrormessage();
		if (lasterrormessage.size() > 0)
		{
			kome::window::WindowTool::showError( lasterrormessage.c_str() );
		}
		else
		{
			// LOG_ERROR_CODE should have been called with an
			// error message to display to the user.
			_ASSERT(FALSE);
		}
		return false;
	}

	return true;
}

// プラグイン管理機能の初期設定をINIファイルから読み込み
void MsppApp::loadActiveTable(){
	// ※保存は PluginManagerView::saveActiveTable() で行う

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini

	const char* SECTION_NAME_HEAD="PluginManagerCompo";

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int nNumPlugins = plgMgr.getNumberOfPlugins();


	for( int i=0; i<nNumPlugins; i++ ){

		kome::plugin::PluginInfo* ptrPluginInfo;
		ptrPluginInfo = plgMgr.getPlugin( i );

		const char* pszName = ptrPluginInfo->getName();
		const char* pszProvider = ptrPluginInfo->getProvider();
		const char* pszDir = ptrPluginInfo->getDir();

		unsigned int uCalls = ptrPluginInfo->getNumberOfCalls();
//		for( unsigned int uIndexCall=0; uIndexCall<uCalls; uIndexCall++ ){
//			kome::plugin::PluginCall* ptrPluginCall = ptrPluginInfo->getCall( uIndexCall );
//
//			char szIniSectionName[256];
//			char szIniParameterName[256];
//			sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, ptrPluginInfo->getName() );
//
//			sprintf( szIniParameterName, "%s", ptrPluginCall->getName() );	// @date 2013/04/23 <有効化> OKADA
//
//			bool flgIniActive = ini->getBool( szIniSectionName, szIniParameterName, true );
//
//			bool flgIniActiveTrue = ini->getBool( szIniSectionName, szIniParameterName, true );
//			bool flgIniActiveFalse = ini->getBool( szIniSectionName, szIniParameterName, false );
//			if(	( flgIniActiveTrue == true )&&
//				( flgIniActiveFalse == false )	){
//				int ddd = 0;	// 読み込み失敗
//				int eee = 0;
//			}
//
//			if( ptrPluginCall->isFixed() ){
//				ptrPluginCall->setActive( true );
//			}
//			else{
//				ptrPluginCall->setActive( flgIniActive );
//			}
//		}

		unsigned int uNumCompo = ptrPluginInfo->getNumberOfComponents();
		for( unsigned int uIndexCompo=0; uIndexCompo<uNumCompo; uIndexCompo++ ){
			kome::plugin::PluginComponent* plgCpmpo = ptrPluginInfo->getComponent( uIndexCompo );

			char szIniSectionName[256];
			char szIniParameterName[256];
			sprintf( szIniSectionName, "%s_%s", SECTION_NAME_HEAD, ptrPluginInfo->getName() );

			sprintf( szIniParameterName, "%s", plgCpmpo->getName() );	// @date 2013/04/23 <有効化> OKADA

			bool flgIniActive = ini->getBool( szIniSectionName, szIniParameterName, true );

			bool flgIniActiveTrue = ini->getBool( szIniSectionName, szIniParameterName, true );
			bool flgIniActiveFalse = ini->getBool( szIniSectionName, szIniParameterName, false );
			if(	( flgIniActiveTrue == true )&&
				( flgIniActiveFalse == false )	){
				int ddd = 0;	// 読み込み失敗
				int eee = 0;
				flgIniActive = true;	// @date 2013/08/22 <Add> OKADA
			}

			// ここでplugin-info経由でcompoにenable / disable 設定する処理を追加 2013/08/22
			if( flgIniActive ){
//				plgCpmpo->setEnable();
				ptrPluginInfo->setEnableComponent( uIndexCompo );
			}else{
//				plgCpmpo->resetEnable();
				ptrPluginInfo->resetEnableComponent( uIndexCompo );
			}

//			if( ptrPluginCall->isFixed() ){
//				ptrPluginCall->setActive( true );
//			}
//			else{
//				ptrPluginCall->setActive( flgIniActive );
//			}

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
