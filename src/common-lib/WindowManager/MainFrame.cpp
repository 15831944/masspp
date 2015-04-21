
/**
 * @file MainFrame.cpp
 * @brief implements of MainFrame class
 *
 * @author S.Tanaka
 * @date 2006.07.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MainFrame.h"

#include "WindowTool.h"
#include "IconManager.h"
#include "ClientWindow.h"
#include "ChildFrame.h"
#include "CanvasBase.h"
#include "StatusBar.h"
#include "DialogProgress.h"
#include "FileDropTarget.h"
#include "mspp16x16.xpm"

#include <stack>
#include <wx/toolbar.h>
#include <wx/image.h>
#include <wx/aui/aui.h>
#include <wx/tooltip.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MENU_MIN_ID						  10001
#define MENU_MAX_ID						  50000

#define TIMER_MIN_ID					  1000
#define TIMER_MAX_ID					  1100

#define PLUGIN_INIT_GUI_METHOD_TYPE_NAME  "INIT_GUI"
#define PLUGIN_EXIT_GUI_METHOD_TYPE_NAME  "EXIT_GUI"


#define TOOLBAR_WIDTH					  32

#define SECTION							  "WINDOW"
#define RECT_X_PARAM_KEY				  "RECT_X"
#define RECT_Y_PARAM_KEY				  "RECT_Y"
#define RECT_HEIGHT_PARAM_KEY			  "RECT_HEIGHT"
#define RECT_WIDTH_PARAM_KEY			  "RECT_WIDTH"
#define MAXIMIZ							  "MAXIMIZED"

#define MENUTOOLBAR_MIN_ID					  50001
#define MENUTOOLBAR_MAX_ID					  50100
#define CHILDFRAME_MIN_ID                     50101
#define CHILDFRAME_MAX_ID                     51000

#define PLG_SECTION						  "PluginManagerCompo_SelectedPackage"
#define PLG_KEY							  "SelectedPackage"
#define OPERATIONLOG_PLUGIN				  "OperationLog Manager"

// >>>>>>	define user events	@Date:2013/10/22	<Add>	A.Ozaki
//
DEFINE_EVENT_TYPE( wxEVT_USER_EXIT_FORCE )
//
// <<<<<<	define user events	@Date:2013/10/22	<Add>	A.Ozaki

BEGIN_EVENT_TABLE( MainFrame, wxMDIParentFrame )
	EVT_MENU_RANGE( MENU_MIN_ID, MENU_MAX_ID, MainFrame::onMenu )
	EVT_UPDATE_UI_RANGE( MENU_MIN_ID, MENU_MAX_ID, MainFrame::onUpdateMenu )
	EVT_CUSTOM_RANGE( wxEVT_TIMER, TIMER_MIN_ID, TIMER_MAX_ID, MainFrame::onTimer )
	EVT_MENU_RANGE( MENUTOOLBAR_MIN_ID, MENUTOOLBAR_MAX_ID, MainFrame::onMenuToolbar )
	EVT_UPDATE_UI_RANGE( MENUTOOLBAR_MIN_ID, MENUTOOLBAR_MAX_ID, MainFrame::onUpdateMenuToolbar )
	EVT_MOVE(MainFrame::onMove)
	EVT_MENU_RANGE( wxID_FILE1, wxID_FILE9, MainFrame::onMRU )
	EVT_UPDATE_UI_RANGE( wxID_FILE1, wxID_FILE9, MainFrame::onUpdateMRU )
	EVT_MENU_RANGE( CHILDFRAME_MIN_ID, CHILDFRAME_MAX_ID, MainFrame::onChildFrame )
	EVT_UPDATE_UI_RANGE( CHILDFRAME_MIN_ID, CHILDFRAME_MAX_ID, MainFrame::onUpdateChildFrame )
	EVT_MENU( wxID_EXIT, MainFrame::onExit )
	EVT_CLOSE( MainFrame::onClose )
	EVT_UPDATE_UI_RANGE(ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, onUpdatePopupMenu)
	EVT_KEY_DOWN( onKeyDown )
	EVT_KEY_UP( onKeyUp )

	EVT_COMMAND( wxID_ANY, wxEVT_USER_EXIT_FORCE, onForceExit )	//	@Date:2013/10/23	<Add>	A.Ozaki
END_EVENT_TABLE()


// constructor
MainFrame::MainFrame( const char* title, const wxPoint& pos, const wxSize& size, long style ) {
	// log
	LOG_TRACE( FMT( "Creating new MDI Frame..." ) );

	// initialize
	m_mgr = NULL;
	m_menuBar = NULL;
	m_mruSeparator = NULL;

	// set object
	WindowTool::setMainWindow( this );

	// timers array
	m_timers.resize( TIMER_MAX_ID - TIMER_MIN_ID + 1 );
	for( unsigned int i = 0; i < m_timers.size(); i++ ) {
		m_timers[ i ].id = i;
		m_timers[ i ].timer = NULL;
	}

	// creating frame
	Create(
		NULL,
		wxID_ANY,
		wxT( NVL( title, "" ) ),
		pos,
		size,
		style | wxFRAME_NO_WINDOW_MENU
	);

	// title
	setTimer(
		boost::bind( &MainFrame::setTitle, this ),
		500,
		false
	);

	// drop target
	SetDropTarget( new FileDropTarget() );
}

// destructor
MainFrame::~MainFrame() {
	// delete timers
	for( unsigned int i = 0; i < m_timers.size(); i++ ) {
		wxTimer* timer = m_timers[ i ].timer;

		// stop the timer
		if( timer != NULL ) {
			if( timer->IsRunning() ) {
				timer->Stop();
			}

			delete timer;

			m_timers[ i ].timer = NULL;
		}
	}
	m_timers.clear();

	// uninitialize frame manager
	if( m_mgr != NULL ) {
		m_mgr->UnInit();
		delete m_mgr;
	}
	
	// Delete all the dynamic menu item objects.
	std::map< unsigned int, DynamicMenuInfo* >::iterator dynMapIterator;
	for (dynMapIterator = m_dynamicMenuMap.begin(); dynMapIterator != m_dynamicMenuMap.end(); dynMapIterator++)
	{
		delete (*dynMapIterator).second;
	}
	m_dynamicMenuMap.clear();

	// size the Holds of the mainframe
	SizeHoldsMainFrame();// 2010.10.18 Add M.Izumi
	
	// unset main window
	WindowTool::setMainWindow( NULL );
}

// create
bool MainFrame::Create(
			wxWindow* parent,
			wxWindowID id,
			const wxString& title,
			const wxPoint& pos,
			const wxSize& size,
			long style
		) {
	// default
	if( !wxMDIParentFrame::Create( parent, id, title, pos, size, style ) ) {
		return false;
	}

	// create AUI manager
	m_mgr = new wxAuiManager( this );

	// creating menu bar
	createMenuBar();

	// creating tool bar
	createToolBar();

	// get plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// call exit GUI plug-in functions
	plgMgr.callAllFunctions( NULL, PLUGIN_INIT_GUI_METHOD_TYPE_NAME );

	// Menu item "Toolbar" to add
	AddMenuItemToolbar();// @date 2010.10.25 <Add> M.Izumi
	
	// tooltip
	wxToolTip::SetDelay( 0 );
	wxToolTip::Enable( true );
	
	// Show the common status bar by default.
    kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance();
    statusBar.setStatusMsg( "common", "" );

	return true;
}

// destroy
bool MainFrame::Destroy() {
	// get plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// call exit GUI plug-in functions
	plgMgr.callAllFunctions( NULL, PLUGIN_EXIT_GUI_METHOD_TYPE_NAME );

	// Spec No.95125 ログ保存のGUIが不自然 @date 2013.10.10 <Mod> M.Izumi ->
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	if( optMgr.getSaveCancelFlg() ){
		optMgr.setSaveCancelFlg( false );
		return false;	
	}
	// default
	return wxMDIParentFrame::Destroy();
	// @date 2013.10.10 <Mod> M.Izumi <-
}

// show
bool MainFrame::Show( bool show ) {
	// default
	if( !wxMDIParentFrame::Show( show ) ) {
		return false;
	}

	return true;
}

// get the number of toolbars
unsigned int MainFrame::getNumberOfToolbars() { 
	return m_toolbars.size();
}

// get toolbar
wxToolBar* MainFrame::getToolbar( const unsigned int index ) { 
	if( index >= m_toolbars.size() ) {
		return NULL;
	}
	return m_toolbars[ index ].toolbar;
}

// get toolbar
wxToolBar* MainFrame::getToolbar( const char* name ) {
	// string object
	std::string nm = tolowercase( name );
	if( nm.size() > 0 ) {
		nm[ 0 ] = toupper( nm[ 0 ] );
	}

	// search
	for( unsigned int i = 0; i < m_toolbars.size(); i++ ) {
		ToolbarInfo* info = &( m_toolbars[ i ] );

		if( info->name.compare( nm ) == 0 ) {
			return info->toolbar;
		}
	}

	// not found
	return NULL;
}

// append pane
void MainFrame::appendPane( wxWindow* wnd, const char* title, WindowTool::DockingAlign align ) {
	// conversion
	kome::core::ConversionElement< WindowTool::DockingAlign, int > aligns[] = {
		{ WindowTool::DOCK_ALIGN_TOP,	wxTOP },
		{ WindowTool::DOCK_ALIGN_LEFT,   wxLEFT },
		{ WindowTool::DOCK_ALIGN_RIGHT,  wxRIGHT },
		{ WindowTool::DOCK_ALIGN_BOTTOM, wxBOTTOM }
	};
	int a = CONVERT_FROM_ARRAY( align, (int)wxLEFT, aligns );

	// append
	m_mgr->AddPane( wnd, a, wxT( NVL( title, "" ) ) );
	m_mgr->Update();
}

// set pane title
void MainFrame::setPaneTitle( wxWindow* wnd, const char* title ) {
	// get pane information
	wxAuiPaneInfo& info = m_mgr->GetPane( wnd );
	if( !info.IsOk() ) {
		return;
	}

	// set the title
	info.Caption( wxT( NVL( title, "" ) ) );
}

// judges whether specified docking window is shown.
bool MainFrame::isVisibleBar( wxWindow* wnd ) {
	// get pane information
	wxAuiPaneInfo& info = m_mgr->GetPane( wnd );
	if( !info.IsOk() ) {
		return false;
	}

	// get visibility
	return info.IsShown();
}

// show docking bar that has specified window
void MainFrame::showBar( wxWindow* wnd ) {
	// get pane information
	wxAuiPaneInfo& info = m_mgr->GetPane( wnd );
	if( !info.IsOk() ) {
		return;
	}

	// show
	info.Show();
	m_mgr->Update();
}

// hides docking bar that has specified window
void MainFrame::hideBar( wxWindow* wnd ) {
	// get pane information
	wxAuiPaneInfo& info = m_mgr->GetPane( wnd );
	if( !info.IsOk() ) {
		return;
	}

	// hide
	info.Hide();
	m_mgr->Update();
}

// toggles visibility of the docking bar that has specified window
void MainFrame::toggleVisibleBar( wxWindow* wnd ) {
	if( isVisibleBar( wnd ) ) {
		hideBar( wnd );
	}
	else {
		showBar( wnd );
	}
}

// set timer
int MainFrame::setTimer( 
		boost::function< void () > fun,
		int milliseconds,
		bool oneShot
) {
	// refresh timers array
	refreshTimers();

	// get timer number
	int num = -1;
	wxTimer* timer = NULL;

	for( unsigned int i = 0; i < m_timers.size() && num < 0 ; i++ ) {
		TimerInfo& ti = m_timers[ i ];

		// start timer
		if( ti.timer == NULL ) {
			num = i;
			timer = new wxTimer( this, TIMER_MIN_ID + num );

			ti.timer = timer;
			ti.id = num;
			ti.fun = fun;
		}
	}

	if( timer == NULL ) {
		LOG_WARN( FMT( "Failed to start timer." ) );
	}
	else {  // Start Timer
		timer->Start( milliseconds, oneShot );
	}

	return num;
}

// kill timer
void MainFrame::killTimer( int timer ) {
	// check the parameter
	if( timer < 0 || timer > (int)m_timers.size() ) {
		return;
	}

	// get timer
	TimerInfo& ti = m_timers[ timer ];

	if( ti.timer != NULL ) {
		if( ti.timer->IsRunning() ) {
			ti.timer->Stop();
		}
		delete ti.timer;
		ti.timer = NULL;
	}
}

// set title
void MainFrame::setTitle() {
	// application name
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// active sample
	kome::objects::Sample* sample = NULL;

	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		sample = aoMgr.getActiveSample();
	}
	else {
		sample = frame->getSample();
	}

	// file name
	std::string fileName;
	if( sample != NULL ) {
		kome::objects::SampleSet* ss = sample->getSampleSet();
		if( ss != NULL ) {
			fileName = ss->getFileName();
		}

		if( !fileName.empty() ) {
			if( sample->isEdited() ) {
				fileName = FMT( "*%s", fileName.c_str() );
			}
		}
	}

	// title
	std::string title = FMT( "%s (%s)", msppMgr.getAppName(), msppMgr.getPlatformName() );
	if( !fileName.empty() ) {
		title = FMT( "%s - %s", fileName.c_str(), title.c_str() );
	}

	// set title
	if( m_title.compare( title ) != 0 ) {
		m_title = title;
		SetTitle( wxT( title.c_str() ) );
	}
}

// update MRU
void MainFrame::updateMRU() {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// file menu
	int fileMenuItem = m_menuBar->FindMenu( wxT( "File" ) );
	wxMenu* fileMenu = NULL;
	if( fileMenuItem < 0 ) {
		fileMenu = new wxMenu();
		m_menuBar->Insert( 0, fileMenu, wxT( "&File" ) );
	}
	else {
		fileMenu = m_menuBar->GetMenu( fileMenuItem );
	}
	if( fileMenu == NULL ) {
		return;
	}

	// clear
	if( m_mruSeparator != NULL ) {
		fileMenu->Delete( m_mruSeparator );
	}

	for( int i = wxID_FILE1; i <= wxID_FILE9; i++ ) {
		wxMenuItem* item = fileMenu->FindItem( i );
		if( item != NULL ) {
			fileMenu->Delete( item );
		}
	}

	// add
	const unsigned int num = aoMgr.getNumberOfHistoryFiles();
	if( num == 0 ) {
		return;
	}

	// position
	int pos = -1;
	for( unsigned int i = 0; i < fileMenu->GetMenuItemCount(); i++ ) {
		wxMenuItem* item = fileMenu->FindItemByPosition( i );
		if( item->IsSeparator() ) {
			pos = (int)i;
		}
	}
	if( pos < 0 ) {
		pos = (int)fileMenu->GetMenuItemCount();
	}

	// add
	m_mruSeparator = fileMenu->InsertSeparator( pos );

	for( unsigned int i = 0; i < num; i++ ) {
		std::string fileName = getfilename( aoMgr.getHistoryFile( i ) );

		if( fileName.length() > 60 ) {
			std::string ext = getext( fileName.c_str() );
			if( !ext.empty() ) {
				ext = "." + ext;
			}

			int len = 60 - (int)ext.length();
			if( len < 0 ) {
				ext = "";
				len = 60;
			}

			fileName = shortenstring( fileName.c_str(), len, true );
			fileName = fileName + ext;
		}

		std::string name = FMT( "&%d: %s", ( i + 1 ), fileName.c_str() );
		std::string desc = FMT( "Open '%s'", fileName.c_str() );

		fileMenu->Insert(
			pos + 1 + i,
			wxID_FILE1 + i,
			wxT( name.c_str() ),
			wxT( desc.c_str() ),
			false
		);
	}
}

// update child window menu
void MainFrame::updateChildWindowMenu( wxWindow* closingWnd, bool bRestore ) {
	// window menu
	int windowMenuItem = m_menuBar->FindMenu( wxT( "Window" ) );
	wxMenu* windowMenu = NULL;
	if( windowMenuItem < 0 ) {
		// @Date:2013/04/23	Changed	A.Ozaki	>>>>>>
		if  ( 0 >= kome::window::ChildFrame::getNumberOfFrames( ) )
		{
			// not append Window menu
			return;
		}
		windowMenu = new wxMenu();
		int	nInsertPos =  m_menuBar->GetMenuCount( ) - 1;
		if  ( 0 > nInsertPos )
		{
			nInsertPos = 0;
		}
		m_menuBar->Insert( nInsertPos, windowMenu, wxT( "&Window" ) );
		// <<<<<< @Date:2013/04/23	Changed	A.Ozaki
	}
	else {
		windowMenu = m_menuBar->GetMenu( windowMenuItem );
	}
	if( windowMenu == NULL ) {
		return;
	}

	// clear
	if  ( !m_menuChildren.empty() && false == bRestore ) // @Date:2013/04/23	Changed	A.Ozaki
	{
		// last separator
		int idx = -1;
		for( unsigned int i = 0; i < windowMenu->GetMenuItemCount(); i++ ) {
			if( windowMenu->FindItemByPosition( i )->IsSeparator() ) {
				idx = (int)i;
			}
		}

		// delete
		for( int i = (int)windowMenu->GetMenuItemCount() - 1; i >= idx; i-- ) {
			wxMenuItem* item = windowMenu->FindItemByPosition( i );
			windowMenu->Destroy( item );
		}		
	}
	
	// clear the array
	m_menuChildren.clear();

	// SPEC 82546: Open several spectrum windows, close the file, items remain on Window menu. Select one -> crash 
	// @date 2011.09.30 <Mod> M.Izumi ->
	int inum = kome::window::ChildFrame::getNumberOfFrames()-1;
	while( inum >= 0 ){
		wxMDIChildFrame* child = kome::window::ChildFrame::getFrame(inum);
		if( child != NULL  ){
			m_menuChildren.push_back( child );
		}
		inum--;
	}
	// @date 2011.09.30 <Mod> M.Izumi <-

	// add
	if( m_menuChildren.size() == 0 ) {
		return;
	}

	windowMenu->AppendSeparator();

	for( unsigned int i = 0; i < m_menuChildren.size(); i++ ) {
		wxMDIChildFrame* child = m_menuChildren[ i ];
		std::string name = FMT( "&%d: %s", ( i + 1 ), child->GetTitle().c_str() );
		std::string desc = FMT( "Activate '%s' window", child->GetTitle().c_str() );

		wxMenuItem* item
			= windowMenu->Append( CHILDFRAME_MIN_ID + i, wxT( name.c_str() ), wxT( desc.c_str() ) );
		item->SetCheckable( true );
	}
}

// refresh timers array
void MainFrame::refreshTimers() {
	for( unsigned int i = 0; i < m_timers.size(); i++ ) {
		TimerInfo& ti = m_timers[ i ];

		if( ti.timer != NULL ) {
			if( !ti.timer->IsRunning() ) {
				delete ti.timer;
				ti.timer = NULL;
			}
		}
	}
}

// create menu bar
void MainFrame::createMenuBar() {
	// log
	LOG_TRACE( FMT( "Creating menu bar" ) );

	// get plug-in manager object
	kome::plugin::PluginManager& plgMgr
		= kome::plugin::PluginManager::getInstance();

	// get root menu
	kome::plugin::PluginMenu& rootMenu = plgMgr.getMenu();

	// sort
	rootMenu.sortMenu();

	// create menu bar
	if( m_menuBar == NULL ) {
		m_menuBar = new wxMenuBar();
		while( m_menuBar->GetMenuCount() > 0 ){
			m_menuBar->Remove( 0 );
		}

		for( unsigned int i = 0; i < rootMenu.getNumberOfChildren(); i++ ){
			kome::plugin::PluginMenu* menuInfo = rootMenu.getChildMenu( i );

			wxMenu* menu = new wxMenu();

			size_t uCntPre = menu->GetMenuItemCount();

			createMenu( menuInfo, menu );

			size_t uCntAfter = menu->GetMenuItemCount();

			int nCnt = uCntAfter - uCntPre;
			if( nCnt <= 0 ){
				delete( menu );
			}else{
				m_menuBar->Append( menu, wxT( menuInfo->getAccName() ) );
			}
		}
	}

	// file menu
	int fileMenuItem = m_menuBar->FindMenu( wxT( "File" ) );
	wxMenu* fileMenu = NULL;
	if( fileMenuItem < 0 ) {
		// menuに"File"が含まれない場合
		fileMenu = new wxMenu();
		m_menuBar->Insert( 0, fileMenu, wxT( "&File" ) );	// "File"メニューを新設
	}
	else {
		fileMenu = m_menuBar->GetMenu( fileMenuItem );	// "File"メニューを取得
	}

	// Exit
	fileMenu->AppendSeparator();
	wxMenuItem* exitItem = fileMenu->Append( wxID_EXIT, wxT( "E&xit" ) );	// "File"メニューに"Exit"を追加
	m_entries.resize( m_entries.size() + 1 );

	// MRU
	updateMRU();	// MRU (Most Recent Used) / 最近使ったファイルの更新

    // shortcut
	if( m_entries.size() > 0 ) {
		wxAcceleratorTable accel( m_entries.size(), &( m_entries[ 0 ] ) );
		SetAcceleratorTable( accel );
	}

	// set 
	SetMenuBar( m_menuBar );	// Tells the frame to show the given menu bar.

}

// create menu
void MainFrame::createMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu ) {
	// manager
	IconManager& iconMgr = IconManager::getInstance();

	// dummy menu
	wxMenuItem* dummy = new wxMenuItem( parentMenu, -1, wxT( "dummy" ), wxT( "dummy" ), wxITEM_CHECK );
	wxBitmap bitmap( 16, 16 );
	dummy->SetBitmap( bitmap );
	parentMenu->Append( dummy );

	// some items
	bool flgAppendedSomeItem = false;

	// append
	std::string prevGroup;
	for( unsigned int j = 0; j < parentInfo->getNumberOfChildren(); j++ ) {
		// child menu information
		kome::plugin::PluginMenu* menuInfo = parentInfo->getChildMenu( j );

		bool flgAppendSeparator = false;

		// separator
		if( (j > 0) && (prevGroup.compare( NVL( menuInfo->getGroupName(), "" ) ) != 0 ) && flgAppendedSomeItem ) {
			flgAppendSeparator = true;
		}

		prevGroup = std::string( NVL( menuInfo->getGroupName(), "" ) );

		// description
		std::string description = menuInfo->getDescription();

		// append menu
		if( menuInfo->getNumberOfChildren() == 0 ) {	// menu item
			std::string menuName = menuInfo->getShortcutKey();
			if( menuName.empty() ) {
				menuName = menuInfo->getAccName();
			}
			else {
				menuName = FMT( "%s\t%s", menuInfo->getAccName(), menuName.c_str() );
			}

			// item
			wxMenuItem* item = new wxMenuItem(
				parentMenu,
				menuInfo->getId(),
				wxT( menuName.c_str() ),
				wxT( description.c_str() ),
				menuInfo->isCheckable() ? wxITEM_CHECK : wxITEM_NORMAL
			);
			item->SetCheckable( menuInfo->isCheckable() );

			// shortcut
			int code = -1;
			int flg = wxACCEL_NORMAL;

			for( unsigned int k = 0; k < menuInfo->getNumberOfShortcutKeys(); k++ ) {
				const int c = menuInfo->getShortcutKey( k );

				if( c == kome::core::key::KEY_SHIFT ) {
					if( flg == wxACCEL_NORMAL ) {
						flg = wxACCEL_SHIFT;
					}
					else {
						flg |= wxACCEL_SHIFT;
					}
				}
				else if( c == kome::core::key::KEY_CONTROL ) {
					if( flg == wxACCEL_NORMAL ) {
						flg = wxACCEL_CTRL;
					}
					else {
						flg |= wxACCEL_CTRL;
					}
				}
				else if( c == kome::core::key::KEY_ALT ) {
					if( flg == wxACCEL_NORMAL ) {
						flg = wxACCEL_ALT;
					}
					else {
						flg |= wxACCEL_ALT;
					}
				}
				else if( c == kome::core::key::KEY_TAB ) {
					code = WXK_TAB;
				}
				else if( c == kome::core::key::KEY_RETURN ) {
					code = WXK_RETURN;
				}
				else if( c == kome::core::key::KEY_SPACE ) {
					code = WXK_SPACE;
				}
				else if( c == kome::core::key::KEY_LEFT ) {
					code = WXK_LEFT;
				}
				else if( c == kome::core::key::KEY_UP ) {
					code = WXK_UP;
				}
				else if( c == kome::core::key::KEY_RIGHT ) {
					code = WXK_RIGHT;
				}
				else if( c == kome::core::key::KEY_DOWN ) {
					code = WXK_DOWN;
				}
				else if( c == kome::core::key::KEY_F1 ) {
					code = WXK_F1;
				}
				else if( c == kome::core::key::KEY_F2 ) {
					code = WXK_F2;
				}
				else if( c == kome::core::key::KEY_F3 ) {
					code = WXK_F3;
				}
				else if( c == kome::core::key::KEY_F4 ) {
					code = WXK_F4;
				}
				else if( c == kome::core::key::KEY_F5 ) {
					code = WXK_F5;
				}
				else if( c == kome::core::key::KEY_F6 ) {
					code = WXK_F6;
				}
				else if( c == kome::core::key::KEY_F7 ) {
					code = WXK_F7;
				}
				else if( c == kome::core::key::KEY_F8 ) {
					code = WXK_F8;
				}
				else if( c == kome::core::key::KEY_F9 ) {
					code = WXK_F9;
				}
				else if( c == kome::core::key::KEY_F10 ) {
					code = WXK_F10;
				}
				else if( c == kome::core::key::KEY_F11 ) {
					code = WXK_F11;
				}
				else if( c == kome::core::key::KEY_F12 ) {
					code = WXK_F12;
				}
				else if( c == kome::core::key::KEY_ESCAPE ) {
					code = WXK_ESCAPE;
				}
				else {
					std::string s;
					s.push_back( (char)c );
					s = touppercase( s.c_str() );

					code = s[ 0 ];
				}
			}

			if( code >= 0 ) {
				m_entries.resize( m_entries.size() + 1 );
				m_entries.back().Set( flg, code, menuInfo->getId(), item );
			}

			// icon
			kome::plugin::PluginCall* call = menuInfo->getCallExec();
			kome::plugin::PluginIcon* icon = ( call == NULL ? NULL : call->getIcon() );

#ifdef _MSC_VER
			if( icon != NULL ) {
				wxIcon* ic = iconMgr.getIcon( icon );

				if( ic != NULL ) {
					item->SetBitmap( *ic );
				}
			}
#endif  // _MSC_VER


			// @date 2013/02/28 <Add> OKADA
			if( call->isActive() ){		// @date 2013/02/28 <Add> OKADA
				// @date 2013/02/28 <Add> OKADA
				// separator
				if( flgAppendSeparator ) {
					parentMenu->AppendSeparator();
				}
				flgAppendedSomeItem = true;


				// add
				parentMenu->Append( item );
			}else{						// @date 2013/02/28 <Add> OKADA
				delete( item );			// @date 2013/02/28 <Add> OKADA
				int nDebugBreakPoint=0;	// @date 2013/02/28 <Add> OKADA
				nDebugBreakPoint=1;		// @date 2013/02/28 <Add> OKADA
			}							// @date 2013/02/28 <Add> OKADA
		}
		else {											// sub menu
			wxMenu* subMenu = new wxMenu();

			size_t uCntPre = subMenu->GetMenuItemCount();

			// call method recursively
			createMenu( menuInfo, subMenu );
			
			size_t uCntAfter = subMenu->GetMenuItemCount();

			int nCnt = uCntAfter - uCntPre;
			if( nCnt <= 0 ){
				delete( subMenu );
			}else{

				// @date 2013/02/28 <Add> OKADA
				// separator
				if( flgAppendSeparator ) {
					parentMenu->AppendSeparator();
				}
				flgAppendedSomeItem = true;


				parentMenu->Append(
					menuInfo->getId(),
					wxT( menuInfo->getAccName() ),
					subMenu,
					wxT( description.c_str() )
				);
			}
		}

		// set to map
		m_menuMap[menuInfo->getId()] = menuInfo;
	}

	// delete dummy
	parentMenu->Delete( dummy );
}

// create tool bar
void MainFrame::createToolBar() {
	// log
	LOG_TRACE( FMT( "Creating tool bar" ) );

	// get plug-in manager object
	kome::plugin::PluginManager& plgMgr
		= kome::plugin::PluginManager::getInstance();

	// get root menu
	kome::plugin::PluginMenu& rootMenu = plgMgr.getMenu();

	// create toolbars
	createToolBar( &rootMenu );
	std::stable_sort( m_toolbars.begin(), m_toolbars.end(), lessToolbar );   

	// rows
	int width = 0;
	int maxWidth = std::max( GetSize().GetWidth(), 500 );
	std::vector< std::stack< ToolbarInfo* > > toolbars;
	toolbars.resize( 1 );
	
	for( unsigned int i = 0; i < m_toolbars.size(); i++ ) {
		// toolbar information
		ToolbarInfo* info = &( m_toolbars[ i ] );
		wxToolBar* toolbar = m_toolbars[ i ].toolbar;
		int w = toolbar->GetToolsCount() * 20;

		if( width + w > maxWidth ) {
			width = w;
			toolbars.resize( toolbars.size() + 1 );
		}

		toolbars.back().push( info );
	}

	// add toolbars
	for( unsigned int i = 0; i < toolbars.size(); i++ ) {
		std::stack< ToolbarInfo* >* toolbarrow = &( toolbars[ i ] );

		while( toolbarrow->size() > 0 ) {
			ToolbarInfo* info = toolbarrow->top();
			toolbarrow->pop();

			// pane info
			wxAuiPaneInfo paneInfo;
			paneInfo.ToolbarPane();
			paneInfo.Top();
			paneInfo.Name( info->name.c_str() );
			paneInfo.Caption( info->name.c_str() );
			paneInfo.Row( i + 1 );
			paneInfo.Position( toolbarrow->size() * 10 );

			// add
			wxToolBar* toolbar = info->toolbar;
			toolbar->SetId( MENUTOOLBAR_MIN_ID + i );
			toolbar->Realize();
			m_mgr->AddPane( toolbar, paneInfo );
			m_mgr->Update();
		}
	}
}

// create toolbar
void MainFrame::createToolBar( kome::plugin::PluginMenu* parentInfo ) {
	// check the parameter
	if( parentInfo == NULL ) {
		return;
	}

	// icon manager
	IconManager& iconMgr = IconManager::getInstance();

	// create tool bar
	std::string name = tolowercase( parentInfo->getToolbarName() );
	if( name.size() > 0 ) {
		name[ 0 ] = toupper( name[ 0 ] );
	}

	kome::plugin::PluginCall* call = parentInfo->getCallExec();
	kome::plugin::PluginIcon* icon = ( call == NULL ? NULL : call->getIcon() );

	// activeでない項目は追加しない(#81)  // @date 2013/02/28 <Add> OKADA
	if( call == NULL ){
		;
	}else{
		if( call->isActive() ){
			// 追加
		}else{
			return;	// 追加しない
		}
	}

	if( !name.empty() && icon != NULL ) {
		// search
		ToolbarInfo* toolbar = NULL;
		
		for( unsigned int i = 0; i < m_toolbars.size() && toolbar == NULL; i++ ) {
			ToolbarInfo* tmp = &( m_toolbars[ i ] );

			if( name.compare( tmp->name ) == 0 ) {
				toolbar = tmp;
			}
		}

		// new toolbar
		if( toolbar == NULL ) {
			m_toolbars.resize( m_toolbars.size() + 1 );
			toolbar = &( m_toolbars.back() );
			toolbar->toolbar = new wxToolBar(
				this,
				wxID_ANY,
				wxDefaultPosition,
				wxDefaultSize,
				wxTB_FLAT | wxTB_NODIVIDER
			);
			toolbar->toolbar->SetToolBitmapSize( wxSize( 16, 16 ) );

			toolbar->name = name;
			toolbar->order = parentInfo->getToolbarOrder();
			toolbar->lastMenu = NULL;
		}

		// separator
		bool sep = false;
		kome::plugin::PluginMenu* lastMenu = toolbar->lastMenu;
		if( lastMenu != NULL ) {
			if( lastMenu->getParentMenu() != parentInfo->getParentMenu() ) {		// different parents
				sep = true;
			}
			else {  // defferent groups
				std::string gr = NVL( parentInfo->getGroupName(), "" );
				std::string lastGr = NVL( lastMenu->getGroupName(), "" );

				sep = ( gr.compare( lastGr ) != 0 );
			}
		}
		if( sep ) {
			toolbar->toolbar->AddSeparator();
		}

		// add icon
		wxIcon* ic = iconMgr.getIcon( icon );
		if( ic != NULL ) {
			kome::plugin::PluginCall* call = parentInfo->getCallExec();

			std::string	description = (call == NULL) ? "" : call->getDescription();
			std::string shortcutKey = parentInfo->getShortcutKey();
			if (!shortcutKey.empty())
			{
				description += " (";
				description += shortcutKey;
				description += ")";
			}

			toolbar->toolbar->AddTool(
				parentInfo->getId(),
				parentInfo->getName(),
				*ic,
				wxNullBitmap,
				parentInfo->isCheckable() ? wxITEM_CHECK : wxITEM_NORMAL,
				wxT( description.c_str() )
			);

			// update last menu
			toolbar->lastMenu = parentInfo;
		}
	}

	// children
	for( unsigned int i = 0; i < parentInfo->getNumberOfChildren(); i++ ) {
		createToolBar( parentInfo->getChildMenu( i ) );
	}
}

// execute menu
void MainFrame::onMenu( wxCommandEvent& evt ) {
	// ID
	unsigned int id = evt.GetId();

	// get menu information
	DynamicMenuInfo* dynamicMenu = m_dynamicMenuMap[ id ];
	kome::plugin::PluginMenu* menu = NULL;
	if( dynamicMenu == NULL ) {
		menu = m_menuMap[ id ];
	}
	if(( menu == NULL ) && ( dynamicMenu == NULL )){
		return;
	}

	// active object
	kome::objects::Variant obj;
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::Sample* acSample = aoMgr.getActiveSample();

		if( acSample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode();
		}
	}
	else {
		obj = frame->getActiveObject();
	}
	
	// execute
	if (dynamicMenu != NULL)
	{
		if( !dynamicMenu->execFn( obj, id ) ) {
			// After calling getlasterrormessage once, the error is deleted.
			// We want to use it twice, so make a copy first.
			std::string lasterrormessage = getlasterrormessage();
			if (lasterrormessage.size() > 0)
			{
				wxMessageBox(
					wxT( lasterrormessage.c_str() ),
					wxT( "Mass++" ),
					wxOK | wxICON_ERROR
				);
			}
			else
			{
				// LOG_ERROR_CODE should have been called with an
				// error message to display to the user.
				_ASSERT(FALSE);
			}
		}
	}
	else
	{
		if( !menu->execute( obj ) ) {
			// After calling getlasterrormessage once, the error is deleted.
			// We want to use it twice, so make a copy first.
			std::string lasterrormessage = getlasterrormessage();
			if (lasterrormessage.size() > 0)
			{
				wxMessageBox(
					wxT( lasterrormessage.c_str() ),
					wxT( "Mass++" ),
					wxOK | wxICON_ERROR
				);
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

// update UI menu
void MainFrame::onUpdateMenu( wxUpdateUIEvent& evt ) {
	// get menu information
	unsigned int id = evt.GetId();
	DynamicMenuInfo* dynamicMenu = NULL;
	if( m_dynamicMenuMap.find( id ) != m_dynamicMenuMap.end() ) {
		dynamicMenu = m_dynamicMenuMap[ id ];
	}
	kome::plugin::PluginMenu* menu = NULL;
	if( dynamicMenu == NULL ) {
		menu = m_menuMap[ id ];
	}
	if(( dynamicMenu == NULL ) && ( menu == NULL )) {
		evt.Enable( false );
		return;
	}

	// active object
	kome::objects::Variant obj;
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::Sample* acSample = aoMgr.getActiveSample();

		if( acSample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode();
		}
	}
	else {
		obj = frame->getActiveObject();
	}

	// update UI
	if (dynamicMenu != NULL)
	{
		if( dynamicMenu->enableFn != NULL ) {
			evt.Enable( dynamicMenu->enableFn(obj, id));
		}
		if( dynamicMenu->checkedFn != NULL ) {
			bool checked = dynamicMenu->checkedFn(obj, id);
			evt.Check( checked );
		}
	}
	else
	{
		evt.Enable( menu->isEnabled( obj ) );
		if( menu->isCheckable() ) {
			bool checked = menu->isChecked( obj );
			evt.Check( checked );
		}
	}
}


// execute menutoolbar				2010.10.25 Add M.Izumi
void MainFrame::onMenuToolbar( wxCommandEvent& evt ) {
	unsigned int id = evt.GetId();
	wxToolBar* toolbar = getToolbar( id - MENUTOOLBAR_MIN_ID );
	int toolbar_id = toolbar->GetId();
	
	toggleVisibleBar( toolbar->GetWindowChild(toolbar_id));
}


// update UI menutoolbar			2010.10.25 Add M.Izumi
void MainFrame::onUpdateMenuToolbar( wxUpdateUIEvent& evt ) {
	unsigned int id = evt.GetId();
	wxToolBar* toolbar = getToolbar( id - MENUTOOLBAR_MIN_ID );
	
	if( toolbar == NULL ) return;

	int toolbar_id = toolbar->GetId();

	wxWindow* wnd = toolbar->GetWindowChild(toolbar_id);
	if( isVisibleBar( wnd )){
		evt.Check(true);
	}else{
		evt.Check(false);
	}
}

void MainFrame::setExecuteFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id ) > fun )
{
	DynamicMenuInfo* dynamicMenuInfo = NULL;

	if( m_dynamicMenuMap.find( menuId ) != m_dynamicMenuMap.end() )
	{	// check map
		dynamicMenuInfo = m_dynamicMenuMap[menuId];
	}
	else
	{
		dynamicMenuInfo = new DynamicMenuInfo;
		dynamicMenuInfo->enableFn = NULL;
		dynamicMenuInfo->checkedFn = NULL;
		dynamicMenuInfo->enableFn = NULL;
		m_dynamicMenuMap[menuId] = dynamicMenuInfo;
	}

	dynamicMenuInfo->execFn = fun;
}

void MainFrame::setCheckFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id ) > fun )
{
	DynamicMenuInfo* dynamicMenuInfo = NULL;

	if( m_dynamicMenuMap.find( menuId ) != m_dynamicMenuMap.end() )
	{	// check map
		dynamicMenuInfo = m_dynamicMenuMap[menuId];
	}
	else
	{
		dynamicMenuInfo = new DynamicMenuInfo;
		dynamicMenuInfo->enableFn = NULL;
		dynamicMenuInfo->checkedFn = NULL;
		dynamicMenuInfo->enableFn = NULL;
		m_dynamicMenuMap[menuId] = dynamicMenuInfo;
	}

	dynamicMenuInfo->checkedFn = fun;
}

void MainFrame::setEnableFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id ) > fun )
{
	DynamicMenuInfo* dynamicMenuInfo = NULL;

	if( m_dynamicMenuMap.find( menuId ) != m_dynamicMenuMap.end() )
	{	// check map
		dynamicMenuInfo = m_dynamicMenuMap[menuId];
	}
	else
	{
		dynamicMenuInfo = new DynamicMenuInfo;
		dynamicMenuInfo->enableFn = NULL;
		dynamicMenuInfo->checkedFn = NULL;
		dynamicMenuInfo->enableFn = NULL;
		m_dynamicMenuMap[menuId] = dynamicMenuInfo;
	}

	dynamicMenuInfo->enableFn = fun;
}

// on Timer
void MainFrame::onTimer( wxEvent& evt ) {
	// get index
	int id = evt.GetId();
	int idx = id - TIMER_MIN_ID;

	// execute
	if( idx < 0 || idx >= (int)m_timers.size() ) {
		return;
	}
	( m_timers[ idx ].fun )();
}

// create client window
wxMDIClientWindow* MainFrame::OnCreateClient() {
	// client window
#ifdef __unix
	m_clientWindow = new ClientWindow( this );
#else
	m_clientWindow = new ClientWindow();
#endif  // __unix

	return m_clientWindow;
}

// get Rect (Main Frame) 
wxRect MainFrame::getRect(){
	return m_rect;
}

// set Rect (Main Frame)
void  MainFrame::setRect( wxRect rect ){
	m_rect = rect;
}

// size the Holds of the mainframe 
void MainFrame::SizeHoldsMainFrame(){
	
	wxRect rect;
	
	if( !IsIconized() && !IsMaximized() ){
		rect = GetRect();	
	}else{
		rect = getRect();
	}
	
	// メインフレームがスクリーンの外側で閉じられた時
	// 次回起動時にX軸、Y軸を（0,0）にする。			
	int dispx = GetSystemMetrics(SM_CXSCREEN); 
	int dispy = GetSystemMetrics(SM_CYSCREEN); 
	int right = rect.GetRight();
	int bottom = rect.GetBottom();
	
	if( rect.x < 0 || right > dispx ){		
		rect.x = 0;
	}

	if( rect.y < 0 || bottom > dispy ){
		rect.y = 0;
	}

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	ini->setInt(SECTION, RECT_X_PARAM_KEY , rect.x );
	ini->setInt(SECTION, RECT_Y_PARAM_KEY , rect.y );
	ini->setInt(SECTION, RECT_WIDTH_PARAM_KEY , rect.width );
	ini->setInt(SECTION, RECT_HEIGHT_PARAM_KEY , rect.height );
	ini->setBool( SECTION, MAXIMIZ, IsMaximized() );
	
}

// Menu item "Toolbar" to add			2010.10.25 Add M.Izumi
void MainFrame::AddMenuItemToolbar(){
	wxMenuBar* menuBar = GetMenuBar();
	int iFind = menuBar->FindMenu(wxT("View"));

	wxMenu* menuToolbar = menuBar->GetMenu(iFind);
	// append sub items
	wxMenu* sub = new wxMenu();
	for( unsigned int i=0; i < m_toolbars.size(); i++ ){
		// string
		std::string name = m_toolbars[ i ].name;
		std::string desc = FMT( "Show / Hide the '%s' toolbar", name.c_str() );

		// item
		wxMenuItem* item = new wxMenuItem(
			sub,
			MENUTOOLBAR_MIN_ID + i,
			wxT( name.c_str() ),
			wxT( desc.c_str() ),
			wxITEM_CHECK
		);	
		sub->Append(item);
	}

	// separator
	int idx = -1;
	for( unsigned int i = 0; i < menuToolbar->GetMenuItemCount() && idx < 0; i++ ) {
		wxMenuItem* item = menuToolbar->FindItemByPosition( i );
		if( item->IsSeparator() ) {
			idx = (int)i;
		}
	}

	// Find the menu item ID.
	if( idx < 0 ) {
		idx = 0;
	}

	// add menu item 
	menuToolbar->Insert( idx, -1, "&Toolbar", sub, "Toolbar");
	
	SetMenuBar(menuBar);
}

// This method is called when the main frame is moved	2010.11.10 M.Izumi
void MainFrame::onMove( wxMoveEvent& evt ){
	if( !IsIconized() && !IsMaximized() ){
		if( GetRect() != getRect() ){
			setRect(GetRect());
		}
	}
}

// on key down
void MainFrame::onKeyDown( wxKeyEvent& evt ) {
	// active object
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		return;
	}

	kome::objects::MsDataVariant obj( frame->getActiveObject() );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataMapInfo* dataMap = obj.getDataMap();

	// canvas
	wxWindow* canvas = frame->getCanvas();
	CanvasBase* base = dynamic_cast< CanvasBase* >( canvas );
	kome::img::Graphics* graphics = ( base == NULL ? NULL : &( base->getGraphics() ) );

	// keyboard event
	kome::evt::KeyEvent keyEvent;
	keyEvent.alt( evt.AltDown() );
	keyEvent.ctrl( evt.ControlDown() );
	keyEvent.shift( evt.ShiftDown() );
	keyEvent.setCode( evt.GetKeyCode() );

	// post
	double flg = false;
	if( spec != NULL ) {
		flg = kome::plugin::PluginCallTool::onSpectrumKeyDown( *spec, graphics, keyEvent, canvas );
	}
	if( chrom != NULL ) {
		flg = kome::plugin::PluginCallTool::onChromatogramKeyDown( *chrom, graphics, keyEvent, canvas );
	}
	if( dataMap != NULL ) {
		flg = kome::plugin::PluginCallTool::onDataMapKeyDown( *dataMap, graphics, keyEvent, canvas );
	}

	// refresh
	if( flg ) {
		kome::window::WindowTool::refresh();
		evt.Skip();
	}
}

// on key up
void MainFrame::onKeyUp( wxKeyEvent& evt ) {
	// active object
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		return;
	}

	kome::objects::MsDataVariant obj( frame->getActiveObject() );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataMapInfo* dataMap = obj.getDataMap();

	// canvas
	wxWindow* canvas = frame->getCanvas();
	CanvasBase* base = dynamic_cast< CanvasBase* >( canvas );
	kome::img::Graphics* graphics = ( base == NULL ? NULL : &( base->getGraphics() ) );

	// keyboard event
	kome::evt::KeyEvent keyEvent;
	keyEvent.alt( evt.AltDown() );
	keyEvent.ctrl( evt.ControlDown() );
	keyEvent.shift( evt.ShiftDown() );
	keyEvent.setCode( evt.GetKeyCode() );

	// post
	double flg = false;
	if( spec != NULL ) {
		flg = kome::plugin::PluginCallTool::onSpectrumKeyUp( *spec, graphics, keyEvent, canvas );
	}
	if( chrom != NULL ) {
		flg = kome::plugin::PluginCallTool::onChromatogramKeyUp( *chrom, graphics, keyEvent, canvas );
	}
	if( dataMap != NULL ) {
		flg = kome::plugin::PluginCallTool::onDataMapKeyUp( *dataMap, graphics, keyEvent, canvas );
	}

	// refresh
	if( flg ) {
		kome::window::WindowTool::refresh();
		evt.Skip();
	}
}

// on MRU
void MainFrame::onMRU( wxCommandEvent& evt ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// index
	int idx = evt.GetId() - wxID_FILE1;

	// open
	std::string file = aoMgr.getHistoryFile( idx );
	std::string filename = getfilename( file.c_str() );
		
	// add wxBeginBusyCursor and wxEndBusyCursor  @date 2011.10.04 <Add> M.Izumi
	wxBeginBusyCursor();

	kome::plugin::PluginCallTool::openFile( aoMgr.getHistoryFile( idx ) );
	
	wxEndBusyCursor();
	
	//progress.setPosition( 1 );
}

// on update MRU
void MainFrame::onUpdateMRU( wxUpdateUIEvent& evt ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// index
	int idx = evt.GetId() - wxID_FILE1;
}

// on child frame
void MainFrame::onChildFrame( wxCommandEvent& evt ) {
	// index
	int idx = evt.GetId() - CHILDFRAME_MIN_ID;
	if( idx < 0 || idx >= (int)m_menuChildren.size() ) {
		return;
	}

	// child frame
	wxMDIChildFrame* child = m_menuChildren[ idx ];
	child->Activate();
}

// on update child frame menu
void MainFrame::onUpdateChildFrame( wxUpdateUIEvent& evt ) {
	// checked
	bool checked = false;

	// index
	wxMDIChildFrame* target = NULL;
	int idx = evt.GetId() - CHILDFRAME_MIN_ID;
	if( idx >= 0 && idx < (int)m_menuChildren.size() ) {
		target = m_menuChildren[ idx ];
	}

	// activate window
	wxMDIChildFrame* activeChild = GetActiveChild();
	checked = ( activeChild == target );

	// set status
	evt.Check( checked );
}

// on exit
void MainFrame::onExit( wxCommandEvent& evt ) {
	Close();
}

// >>>>>>	@Date:2013/10/23	<Add>	A.Ozaki
//
// on exit (force)
void	MainFrame::onForceExit( wxCommandEvent& evt )
{
	Destroy( );

	return;
}
//
// <<<<<<	@Date:2013/10/23	<Add>	A.Ozaki

// on close
void MainFrame::onClose( wxCloseEvent& evt ) {
	// PluginMangerの選択パッケージ確認 @date 2013.09.24 <Add> M.Izumi ->	
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();	
	std::string selectPackage = ini->getString( PLG_SECTION, PLG_KEY, "Full" );
		
	bool bcompHas = ( selectPackage.compare( "Full" ) == 0 ? true : false );

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	// OperationLog Manager Plugin
	kome::plugin::PluginInfo* info = plgMgr.getPlugin( OPERATIONLOG_PLUGIN );
	if( info != NULL && !bcompHas ){
		int inum = info->getNumberOfComponents();
		
		for( int i=0; i < inum && !bcompHas; i++ ){
			kome::plugin::PluginComponent* plgCompo = info->getComponent( i );
			bcompHas = plgCompo->hasPackage( selectPackage.c_str() );
		}

		if( !bcompHas ){
			// can exit
			if( kome::plugin::PluginCallTool::canExit() )
			{	
				if (evt.CanVeto())
				{
					Destroy();
				}
			}
			return;
		}
	}
	// @date 2013.09.24 <Add> M.Izumi <-

	// @date 2013.09.02 <Mod> M.Izumi ->
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	if( optMgr.getSaveCount() != optMgr.getNumberOfOperations() && optMgr.getNumberOfOperations() > 0 ){
		int nYesNoCancel = kome::window::WindowTool::showYesNoCancel( "There are unsaved changes. Do you want to save the changes?" );
		bool bflg = false;
		switch( nYesNoCancel )
		{
			case 1:		// Yes
				bflg = true;
				break;
			case 0:		// No
				break;
			case -1:	// Cancel
				break;
			default:	// default
				break;
		}

		optMgr.setSaveCountFlg( bflg );

		if( nYesNoCancel == -1 ){ 
			return;
			
		}

	}
	// @date 2013.09.02 <Mod> M.Izumi <-

	// can exit
	if( kome::plugin::PluginCallTool::canExit() )
	{	
		if (evt.CanVeto())
		{
			Destroy();
		}
	}

}

// compare toolbar information to sort
bool MainFrame::lessToolbar( ToolbarInfo t0, ToolbarInfo t1 ) {
	// compare specified order
	int cmp = t0.order - t1.order;
	return ( cmp < 0 );
}

// on update popup menu
void MainFrame::onUpdatePopupMenu( wxUpdateUIEvent& evt ){
	int nEventId = evt.GetId();

	kome::plugin::PluginMenu* ptrMenu = kome::window::Canvas::m_contextMenuTreeMap[nEventId];	// @date 2012/11/27 <Add> OKADA

	if( ptrMenu == NULL ) {
		return;
	}
	
	// active object
	kome::objects::Variant obj;
	ChildFrame* frame = ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::Sample* acSample = aoMgr.getActiveSample();

		if( acSample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode();
		}// ※acSampleがNULLの場合はobj初期値を使用する
	}
	else {
		obj = frame->getActiveObject();
	}

	if( Canvas::m_contextMenuTreeMap.find( nEventId ) == Canvas::m_contextMenuTreeMap.end() ){
		// 一致しなかった
	}else{
		// 一致した
		// enable / disable
		bool bEnable = ptrMenu->isEnabled( obj );
		evt.Enable( bEnable );
		// check / uncheck
		if( ptrMenu->isCheckable() ){
			bool checked = ptrMenu->isChecked( obj );
			evt.Check( checked );
		}
	}

	return;
}


// clear menu bar
//void MainFrame::clearMenuBar(){
void MainFrame::updateMenuAndToolbars(){

	// @date 2013/04/23 <Add> Ozaki ------->
	// メニューからPane配下を調べて、表示中のものは閉じます
	//
	// ★★先にPaneを閉じる方法として、この方法しかないのか確認★★
	//
	if  ( 1 )
	{
		//
		// objはとれないけど、いいのかな？
		//
		kome::plugin::PluginMenu* pMenu = NULL;
		kome::objects::Variant obj;
		ChildFrame* frame = ChildFrame::getActiveFrame();
		if( frame == NULL ) {
			kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
			kome::objects::Sample* acSample = aoMgr.getActiveSample();

			if( acSample != NULL ) {
				obj.type = kome::objects::Variant::DATA_GROUP_NODE;
				obj.prim.pt = acSample->getRootDataGroupNode();
			}
		}
		else
		{
			obj = frame->getActiveObject();
		}

		std::map< unsigned int, kome::plugin::PluginMenu* >::iterator	itr = m_menuMap.begin( );
		while ( itr != m_menuMap.end( ) )
		{
			kome::plugin::PluginMenu* pParentMenu = NULL;

			pMenu = itr->second;
			pParentMenu = pMenu->getParentMenu( );
			if  ( NULL != pParentMenu )
			{
				std::string	strTemp = pParentMenu->getName( );
				if  ( strTemp == "Pane" )
				{
					if  ( pMenu->isChecked( obj ) )
					{
						// 表示中なので、再度呼び出してPaneを閉じます
						pMenu->execute( obj );
					}
				}
			}
			itr++;
		}
	}
	// @date 2013/04/23 <Add> Ozaki <-------


	if( m_mgr != NULL ) {
		wxAuiPaneInfoArray& panes = m_mgr->GetAllPanes();
		for( unsigned int i = 0; i < panes.size(); i++ ) {
			wxWindow* wnd = panes[ i ].window;
			hideBar( wnd );
		}

		// >>>>>>	@Date:2013/09/24	<Add>	A.Ozaki
		// wxAuiManagerを作り直す準備
		//
		m_mgr->UnInit();
		delete	m_mgr;
		//
		// <<<<<<	@Date:2013/09/24	<Add>	A.Ozaki
	}

	// create AUI manager
	m_mgr = new wxAuiManager( this );

	if( m_menuBar != NULL ) {
		while( m_menuBar->GetMenuCount() > 0 ){
			wxMenu* ptrMenu = m_menuBar->GetMenu( 0 );
			m_menuBar->Remove( 0 );
			delete( ptrMenu );
		}
	}

	// get plug-in manager object
	kome::plugin::PluginManager& plgMgr
		= kome::plugin::PluginManager::getInstance();

	// get root menu
	kome::plugin::PluginMenu& rootMenu = plgMgr.getMenu();

	for( unsigned int i = 0; i < rootMenu.getNumberOfChildren(); i++ ){
		kome::plugin::PluginMenu* menuInfo = rootMenu.getChildMenu( i );

		wxMenu* menu = new wxMenu();

		size_t uCntPre = menu->GetMenuItemCount();

		createMenu( menuInfo, menu );

		size_t uCntAfter = menu->GetMenuItemCount();

		int nCnt = uCntAfter - uCntPre;
		if( nCnt <= 0 ){
			delete( menu );
		}else{
			m_menuBar->Append( menu, wxT( menuInfo->getAccName() ) );
		}

	}

// >>>>>>	@Date:2013/09/04	<Modified>	A.Ozaki
//
	//
	// 現在もっているtoolbarの項目を全て削除します
	//
	for ( unsigned int unVal = 0 ; unVal < m_toolbars.size( ) ; unVal++ )
	{
		m_toolbars[unVal].toolbar->ClearTools( );
		delete	m_toolbars[unVal].toolbar;
	}

	//
	// Toolbarを再度作り直します
	//
	m_toolbars.clear( );
	createToolBar( );
	m_mgr->Update( );

//
// <<<<<<	@Date:2013/09/24	<Modified>	A.Ozaki

	// about menuが消えるので、wxID_ABOUTのメニューを追加する必要あり

	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// set icon
    wxIcon icon( mspp16x16_xpm );
    this->SetIcon( icon );

	// add about menu
    wxMenuBar* menuBar = this->GetMenuBar();
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

// >>>>>>	@Date:2013/09/24	<Add>	A.Ozaki
// ここでもUpdateしておかないとToolbarが綺麗に表示されません
// いわゆるおまじない
	m_mgr->Update( );
//
// <<<<<<	@Date:2013/09/24	<Add>	A.Ozaki
}



// @Date 2013/04/21	Add	A.Ozaki	>>>>>>
//

// update tool bar
void	MainFrame::updateToolBar( void )
{
	// log
	LOG_TRACE( FMT( "Updateing tool bar" ) );

	// get plug-in manager object
	kome::plugin::PluginManager&	plgMgr = kome::plugin::PluginManager::getInstance( );

	// get root menu
	kome::plugin::PluginMenu&	rootMenu = plgMgr.getMenu( );

	//
	// 現在もっているtoolbarの項目を全て削除します
	//
	for ( unsigned int unVal = 0 ; unVal < m_toolbars.size( ) ; unVal++ )
	{
		m_toolbars[unVal].toolbar->ClearTools( );
		m_toolbars[unVal].lastMenu = NULL;
	}

	// update toolbars
	updateToolBar( &rootMenu );
	std::stable_sort( m_toolbars.begin(), m_toolbars.end(), lessToolbar );   

	// rows
	int	width = 0;
	int	maxWidth = std::max( GetSize().GetWidth(), 500 );
	std::vector< std::stack< ToolbarInfo* > >	toolbars;
	toolbars.resize( 1 );

	for ( unsigned int i = 0 ; i < m_toolbars.size() ; i++ )
	{
		// toolbar information
		ToolbarInfo*	info = &( m_toolbars[ i ] );
		wxToolBar*	toolbar = m_toolbars[ i ].toolbar;
		int	w = toolbar->GetToolsCount() * 20;

		if  ( width + w > maxWidth )
		{
			width = w;
			toolbars.resize( toolbars.size() + 1 );
		}

		toolbars.back().push( info );
	}

	// add toolbars
	for ( unsigned int i = 0 ; i < toolbars.size() ; i++ )
	{
		std::stack< ToolbarInfo* >*	toolbarrow = &( toolbars[ i ] );

		while ( 0 < toolbarrow->size( ) )
		{
			ToolbarInfo*	info = toolbarrow->top( );
			toolbarrow->pop( );

			// pane info
			wxAuiPaneInfo	paneInfo;
			paneInfo.ToolbarPane( );
			paneInfo.Top( );
			paneInfo.Name( info->name.c_str() );
			paneInfo.Caption( info->name.c_str() );
			paneInfo.Row( i + 1 );
			paneInfo.Position( toolbarrow->size() * 10 );

			// add
			wxToolBar* toolbar = info->toolbar;
			toolbar->SetId( MENUTOOLBAR_MIN_ID + i );
			toolbar->Realize( );

			//
			// 既にAuiManagerにPaneを登録しているかを確認します
			//
			wxAuiPaneInfo	existPaneInfo;
			existPaneInfo = m_mgr->GetPane( info->name );
			if  ( existPaneInfo.IsOk( ) )
			{
				// 既に同じものがあるので、追加しません
				if  ( 0 == toolbar->GetToolsCount( ) )
				{
					// ツールバー内に1つも項目がないので、paneを非表示にします
					if  ( true == isVisibleBar( toolbar ) )
					{
						toggleVisibleBar( toolbar );
					}
				}
				else
				{
					if  ( false == isVisibleBar( toolbar ) )
					{
						toggleVisibleBar( toolbar );
					}
					wxAuiPaneInfo& tempInfo = m_mgr->GetPane( toolbar );
					if  ( tempInfo.IsFixed( ) == true )
					{
						tempInfo.Resizable( true );
						m_mgr->Update( );
						tempInfo.Fixed( );
						m_mgr->Update( );
					}
				}
			}
			else
			{
				m_mgr->AddPane( toolbar, paneInfo );
			}
			m_mgr->Update( );
		}
	}
}

// update toolbar
void	MainFrame::updateToolBar( kome::plugin::PluginMenu* parentInfo )
{
	// check the parameter
	if  ( NULL == parentInfo )
	{
		return;
	}

	// icon manager
	IconManager&	iconMgr = IconManager::getInstance( );

	// update tool bar
	std::string	name = tolowercase( parentInfo->getToolbarName( ) );
	if  ( 0 < name.size( ) )
	{
		name[ 0 ] = toupper( name[ 0 ] );
	}

	kome::plugin::PluginCall*	call = parentInfo->getCallExec( );
	kome::plugin::PluginIcon*	icon = ( call == NULL ? NULL : call->getIcon( ) );

	if  ( !name.empty( ) && icon != NULL )
	{
		// search
		ToolbarInfo*	toolbar = NULL;
		
		for ( unsigned int i = 0 ; i < m_toolbars.size( ) && toolbar == NULL ; i++ )
		{
			ToolbarInfo* tmp = &( m_toolbars[ i ] );

			if  ( 0 == name.compare( tmp->name ) )
			{
				toolbar = tmp;
			}
		}

		// new toolbar
		if  ( toolbar == NULL )
		{
			m_toolbars.resize( m_toolbars.size( ) + 1 );
			toolbar = &( m_toolbars.back( ) );
			toolbar->toolbar = new wxToolBar( this,
											wxID_ANY,
											wxDefaultPosition,
											wxDefaultSize,
											wxTB_FLAT | wxTB_NODIVIDER
											);
			if  ( NULL != toolbar->toolbar )
			{
				toolbar->toolbar->SetToolBitmapSize( wxSize( 16, 16 ) );

				toolbar->name = name;
				toolbar->order = parentInfo->getToolbarOrder();
				toolbar->lastMenu = NULL;
			}
		}

		// separator
		bool	sep = false;
		kome::plugin::PluginMenu* lastMenu = toolbar->lastMenu;

		if  ( lastMenu != NULL )
		{
			if  ( lastMenu->getParentMenu( ) != parentInfo->getParentMenu( ) )
			{	// different parents
				sep = true;
			}
			else
			{	// defferent groups
				std::string gr = NVL( parentInfo->getGroupName( ), "" );
				std::string lastGr = NVL( lastMenu->getGroupName( ), "" );

				sep = ( gr.compare( lastGr ) != 0 );
			}
		}

		// add icon
		wxIcon* ic = iconMgr.getIcon( icon );
		if  ( NULL != ic && NULL != toolbar->toolbar )
		{
			kome::plugin::PluginCall*	call = parentInfo->getCallExec( );

			std::string	description = (call == NULL) ? "" : call->getDescription();
			std::string shortcutKey = parentInfo->getShortcutKey( );
			if  ( !shortcutKey.empty( ) )
			{
				description += " (";
				description += shortcutKey;
				description += ")";
			}

			if  ( NULL != call && true == call->isActive( ) )
			{
				//
				// プラグインが有効なときだけ処理します
				//

				if  ( sep )
				{
					toolbar->toolbar->AddSeparator( );
				}

				toolbar->toolbar->AddTool( parentInfo->getId(),
										parentInfo->getName(),
										*ic,
										wxNullBitmap,
										parentInfo->isCheckable() ? wxITEM_CHECK : wxITEM_NORMAL,
										wxT( description.c_str() )
										);

				// update last menu
				toolbar->lastMenu = parentInfo;
			}
		}
	}

	// children
	for ( unsigned int i = 0 ; i < parentInfo->getNumberOfChildren( ) ; i++ )
	{
		updateToolBar( parentInfo->getChildMenu( i ) );
	}
}

//
// <<<<<< @Date 2013/04/21	Add	A.Ozaki



// >>>>>>	@Date:2013/10/23	<Add>	A.Ozaki
//

wxEventType	MainFrame::getUserEventType( const UserEventType eType )
{
	wxEventType	insType = 0;

	switch ( eType )
	{
	case  USER_EVENT_FORCE_EXIT:
		insType = wxEVT_USER_EXIT_FORCE;
		break;
	default:
		break;
	}

	return	insType;
}

//
// <<<<<<	@Date:2013/10/23	<Add>	A.Ozaki
