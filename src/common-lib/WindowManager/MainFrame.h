/**
 * @file MainFrame.h
 * @brief interfaces of MainFrame class
 *
 * @author S.Tanaka
 * @date 2006.07.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_MAIN_FRAME_H__
#define __KOME_WINDOW_MAIN_FRAME_H__


#include "WindowTool.h"


#include <map>
#include <boost/function.hpp>
#include <wx/wx.h>
#include <wx/accel.h>


class wxAuiManager;


// >>>>>>	declare local user events	@Date:2013/10/22	<Add>	A.Ozaki
//
DECLARE_LOCAL_EVENT_TYPE( wxEVT_USER_EXIT_FORCE, -1 )
//
// <<<<<<	declare local user events	@Date:2013/10/22	<Add>	A.Ozaki


namespace kome {
	namespace window {

		/**
		 * @class MainFrame
		 * @brief main frame class
		 */
		class WINDOW_MANAGER_CLASS MainFrame : public wxMDIParentFrame {
		public:
			/**
			 * @fn MainFrame(
					const char* title,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxDEFAULT_FRAME_STYLE
				)
			 * @brief constructor
			 * @param[in] title frame title
			 * @param[in] pos frame position
			 * @param[in] size frame size
			 * @param[in] style frame style
			 */
			MainFrame(
				const char* title,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxDEFAULT_FRAME_STYLE
			);

			/**
			 * @fn virtual ~MainFrame()
			 * @brief destructor
			 */
			virtual ~MainFrame();

		protected:
			/** tool bar information */
			struct ToolbarInfo {
				wxToolBar* toolbar;
				std::string name;
				int order;
				kome::plugin::PluginMenu* lastMenu;
			};

			/** AUI manager */
			wxAuiManager* m_mgr;

			/** menu bar */
			wxMenuBar* m_menuBar;

			/** toolbars */
			std::vector< ToolbarInfo > m_toolbars;

			/** menu map */
			std::map< unsigned int, kome::plugin::PluginMenu* > m_menuMap;

			/** dynamic menu information */
			struct DynamicMenuInfo {
				boost::function< bool ( kome::objects::Variant obj, int id ) > execFn;
				boost::function< bool ( kome::objects::Variant obj, int id ) > checkedFn;
				boost::function< bool ( kome::objects::Variant obj, int id ) > enableFn;
			};

			/** dynamic menu map */
			std::map< unsigned int, DynamicMenuInfo* > m_dynamicMenuMap;

			/**
			 * @struct TimerInfo
			 * @brief timer information
			 */
			struct TimerInfo {
				int id;
				wxTimer* timer;
				boost::function< void () > fun;
			};

			/** timers array */
			std::vector< TimerInfo > m_timers;

			/** accelerator entries */
			std::vector< wxAcceleratorEntry > m_entries;

			/** MRU seperator */
			wxMenuItem* m_mruSeparator;

			/** menu frames */
			std::vector< wxMDIChildFrame* > m_menuChildren;

			/** title */
			std::string m_title;

		private:	
			/** MainFrame rect */
			wxRect m_rect;				 // 2010.10.18 Add M.Izumi
			
			/**
			 * @void SizeHoldsMainFrame()
			 * @brief size the Holds of the mainframe
			 */
			void SizeHoldsMainFrame();	 // 2010.10.18 Add M.Izumi

		public:
			/**
			 * @fn bool Create(
					wxWindow* parent,
					wxWindowID id,
					const wxString& title,
					const wxPoint& pos,
					const wxSize& size,
					long style
				)
			 * @brief creates frame
			 * @param[in] parent parent frame. (This parameter should be NULL.)
			 * @param[in] id window identifer
			 * @param[in] title frame title
			 * @param[in] pos frame position
			 * @param[in] size frame size
			 * @param[in] style frame style
			 */
			bool Create(
				wxWindow* parent,
				wxWindowID id,
				const wxString& title,
				const wxPoint& pos,
				const wxSize& size,
				long style
			);

			/**
			 * @fn virtual bool Destroy()
			 * @brief destroys frame (override method)
			 * @return If it succeeded to destroy frame, this method returns true.
			 */
			virtual bool Destroy();

			/**
			 * @fn virtual bool Show( bool show )
			 * @brief shows frame
			 * @param[in] show If true, displays the window. Otherwise, hides it.
			 */
			virtual bool Show( bool show );

		public:
			/**
			 * @fn unsigned int getNumberOfToolbars()
			 * @brief gets the number of toolbars
			 * @return the number of toolbars
			 */
			unsigned int getNumberOfToolbars();

			/**
			 * @fn wxToolBar* getToolbar( const unsigned int index )
			 * @brief gets toolbar
			 * @param[in] index toolbar index
			 * @return toolbar (If NULL, the index is illegal.)
			 */
			wxToolBar* getToolbar( const unsigned int index );

			/**
			 * @fn wxToolBar* getToolbar( const char* name )
			 * @brief gets toolbar
			 * @param[in] name toolbar name
			 * @return toolbar (If NULL, specified toolbar name was not found.)
			 */
			wxToolBar* getToolbar( const char* name );

		public:
			/**
			 * @fn void appendPane( wxWindow* wnd, const char* title, WindowTool::DockingAlign align )
			 * @brief appends docking pane
			 * @param[in] wnd window control to be appended
			 * @param[in] title docking pane title
			 * @param[in] align docking align
			 */
			void appendPane( wxWindow* wnd, const char* title, WindowTool::DockingAlign align );

			/**
			 * @fn void setPaneTitle( wxWindow* wnd, const char* title )
			 * @brief sets the pane title
			 * @param[in] wnd window control
			 * @param[in] title pane title
			 */
			void setPaneTitle( wxWindow* wnd, const char* title );

			/**
			 * @fn bool isVisibleBar( wxWindow* wnd )
			 * @brief judges whether specified docking window is shown.
			 * @param[in] wnd window control object to be judged
			 * @return If true, specified window is show
			 */
			bool isVisibleBar( wxWindow* wnd );

			/**
			 * @fn void showBar( wxWindow* wnd )
			 * @brief shows docking bar that has specified window
			 * @param[in] wnd window control object
			 */
			void showBar( wxWindow* wnd );

			/**
			 * @fn void hideBar( wxWindow* wnd )
			 * @brief hides docking bar that has specified window
			 * @param[in] wnd window control object
			 */
			void hideBar( wxWindow* wnd );

			/**
			 * @fn void toggleVisibleBar( wxWindow* wnd )
			 * @brief If docking bar that has specified window is shown, hides docking bar, otherwise shows it.
			 * @param[in] wnd window control object
			 */
			void toggleVisibleBar( wxWindow* wnd );

		public:
			/**
			 * @fn int setTimer(
					boost::function< void () > fun,
					int milliseconds,
					bool oneShot
				)
			 * @brief set timer
			 * @param[in] fun timer function
			 * @param[in] milliseconds timer interval (msec)
			 * @param[in] oneShot If true, timer function is called once, otherwise it is called repeatly.
			 * @return timer number (If negative value, it failed to start timer.)
			 */
			int setTimer(
				boost::function< void () > fun,
				int milliseconds,
				bool oneShot
			);

			/**
			 * @fn void killTimer( int timer )
			 * @brief kills timer
			 * @param[in] timer timer number
			 */
			void killTimer( int timer );

		public:
			/**
			 * @fn void setTitle()
			 * @brief sets the main frame title
			 */
			void setTitle();

		public:
			/**
			 * @fn void setExecuteFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj ) > fun )
			 * @brief set execute function
			 * @param[in] menuId menu ID
			 * @param[in] fun execute function
			 */
			void setExecuteFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id	 ) > fun );

			/**
			 * @fn void setCheckFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj ) > fun )
			 * @brief set check function
			 * @param[in] menuId menu ID
			 * @param[in] fun check function
			 */
			void setCheckFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id ) > fun );

			/**
			 * @fn void setEnableFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj ) > fun )
			 * @brief set enable function
			 * @param[in] menuId menu ID
			 * @param[in] fun enable function
			 */
			void setEnableFunction( const int menuId, boost::function< bool ( kome::objects::Variant obj, int id ) > fun );


		public:
			/**
			 * @fn void updateMRU()
			 * @brief updates MRU (Most Recent Used) menu.
			 */
			void updateMRU();

			// @date 2013/04/23 <Add> Ozaki ------->
			/**
			 * @fn void updateChildWindowMenu( wxWindow* closingWnd = NULL, bool bRestore = false )
			 * @brief updates child window menu
			 * @param[in] closingWnd closing window
			 * @param[in] bRestore restore processing
			 */
			void updateChildWindowMenu( wxWindow* closingWnd = NULL, bool bRestore = false );
			// @date 2013/04/23 <Add> Ozaki <-------

		protected:
			/**
			 * @fn void refreshTimers()
			 * @brief refreshes timers array
			 */
			void refreshTimers();

// >>>>>>	@Date:2013/10/22	<Add>	A.Ozaki
//
		public:
			typedef enum
			{
				USER_EVENT_NONE = -1,
				USER_EVENT_FORCE_EXIT = 0,
			} UserEventType;

			/**
			 * @fn wxEventType getUserEventType( const UserEventType eType )
			 * @brief get event type
			 * @param[in] eType event type
			 */
			wxEventType	getUserEventType( const UserEventType eType );
//
// <<<<<<	@Date:2013/10/22	<Add>	A.Ozaki

		public:
			/**
			 * @fn void onMenu( wxCommandEvent& evt )
			 * @brief execute from a menu
			 * @param[in] evt command event infomation
			 */
			void onMenu( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of menu
			 * @param[in] evt update UI event information
			 */
			void onUpdateMenu( wxUpdateUIEvent& evt );

			/**
			 * @fn void onTimer( wxEvent& evt )
			 * @brief This method is called on timer event
			 * @param[in] evt event information
			 */
			void onTimer( wxEvent& evt );
						
			/**
			 * @fn void onMenuToolbar( wxCommandEvent& evt )
			 * @brief execute from a menuToolbar
			 * @param[in] evt command event infomation
			 */
			void onMenuToolbar( wxCommandEvent& evt );			// 2010.10.25 Add M.Izumi

			/**
			 * @fn void onUpdateMenuToolbar( wxUpdateUIEvent& evt )
			 * @brief sets the status of menuToolbar
			 * @param[in] evt update UI event information
			 */
			void onUpdateMenuToolbar( wxUpdateUIEvent& evt );	// 2010.10.25 Add M.Izumi
			
			/**
			 * @fn wxRect getRect()
			 * @brief get Main Frame Rect
			 * @return wxRect (Main Frame)
			 */
			wxRect getRect();					// 2010.10.18 Add M.Izumi

			/**
			 * @fn void setRect( wxRect rect )
			 * @brief set Main Frame Rect
			 * @param[in] rect wxRect
			 */
			void setRect( wxRect rect );		// 2010.10.18 Add M.Izumi

			/**
			 * @fn onMove( wxMoveEvent& evt )
			 * @brief This method is called when the main frame is moved
			 * @param[in] evt move event
			 */
			void onMove( wxMoveEvent& evt );	// 2010.11.10 Add M.Izumi

			/**
			 * @fn void onKeyDown( wxKeyEvent& evt )
			 * @brief This method is called when a key is down
			 * @param[in] evt keyboard event
			 */
			void onKeyDown( wxKeyEvent& evt );

			/**
			 * @fn void onKeyUp( wxKeyEvent& evt )
			 * @brief This method is called when a key is up
			 * @param[in] evt keyboard event
			 */
			void onKeyUp( wxKeyEvent& evt );

		protected:
			/**
			 * @fn void onMRU( wxCommandEvent& evt )
			 * @brief opens most recent used file
			 * @param[in] evt event infromation
			 */
			void onMRU( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateMRU( wxUpdateUIEvent& evt )
			 * @brief sets the status of MRU file
			 * @param[in] evt update UI event information
			 */
			void onUpdateMRU( wxUpdateUIEvent& evt );

			/**
			 * @fn void onChildFrame( wxCommandEvent& evt )
			 * @brief activate a child frame
			 * @param[in] evt event information
			 */
			void onChildFrame( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateChildFrame( wxUpdateUIEvent& evt )
			 * @brief sets the status of child frame menu
			 * @param[in] evt event information
			 */
			void onUpdateChildFrame( wxUpdateUIEvent& evt );

			/**
			 * @fn void onExit( wxCommandEvent& evt )
			 * @brief This methos is called when the exit menu is selected
			 * @param[in] evt event information
			 */
			void onExit( wxCommandEvent& evt );

			// >>>>>>	@Date:2013/10/23	<Add>	A.Ozaki
			//
			/**
			 * @fn void onForceExit( wxCommandEvent& evt )
			 * @brief This methos is called when user custom event
			 * @param[in] evt event information
			 */
			void onForceExit( wxCommandEvent& evt );
			//
			// <<<<<<	@Date:2013/10/23	<Add>	A.Ozaki

		protected:
			/**
			 * @fn void onClose( wxCloseEvent& evt )
			 * @brief This method is called when the frame is closed.
			 * @param[in] evt close event
			 */
			void onClose( wxCloseEvent& evt );

		protected:
			/**
			 * @fn void createMenuBar()
			 * @brief creates menu bar
			 */
			void createMenuBar();

			/**
			 * @fn void createMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu )
			 * @brief creates menu
			 * @param[in] parentInfo parent menu information
			 * @param[in] parentMenu parent menu
			 */
			void createMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu );

			/**
			 * @fn void createToolBar()
			 * @brief creates tool bar
			 */
			void createToolBar();

			/**
			 * @fn void createToolBar( kome::plugin::PluginMenu* parentInfo )
			 * @brief creates toolbar
			 * @param[in] parentInfo parent menu information
			 */
			void createToolBar( kome::plugin::PluginMenu* parentInfo );
			
			// @Date 2013/04/21	Add	A.Ozaki	>>>>>>
			//
			/**
			 * @fn void updateToolBar( void )
			 * @brief creates tool bar
			 */
			void updateToolBar( void );

			/**
			 * @fn void updateToolBar( kome::plugin::PluginMenu* parentInfo )
			 * @brief update toolbar
			 * @param[in] parentInfo parent menu information
			 */
			void updateToolBar( kome::plugin::PluginMenu* parentInfo );
			
			//
			// <<<<<< @Date 2013/04/21	Add	A.Ozaki

			/**
			 * @fn void AddMenuItemToolbar()
			 * @brief Menu item "Toolbar" to add
			 */
			void AddMenuItemToolbar();			// 2010.10.25 Add M.Izumi

		public:
			/**
			 * @fn virtual wxMDIClientWindow* OnCreateClient()
			 * @brief creates client window
			 * @return client window
			 */
			virtual wxMDIClientWindow* OnCreateClient();

        protected:
			/**
			 * @fn static bool lessToolbar( ToolbarInfo t0, ToolbarInfo t1 )
			 * @brief compares toolbar information to sort
			 * @param[in] t0 toolbar information to be compared
			 * @param[in] t1 toolbar information to compare
			 * @return If true, t0 sorts before t1.
			 */
			static bool lessToolbar( ToolbarInfo t0, ToolbarInfo t1 );

			/**
			 * @fn void onUpdatePopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void onUpdatePopupMenu( wxUpdateUIEvent& evt );

		// メニューが全部削除できるかの確認用
		public:
			/**
			 * @fn void updateMenuAndToolbars()
			 * @brief update Menu And Toolbars
			 */
			void updateMenuAndToolbars();	// @date 2013/02/27 <Add> OKADA

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_MAIN_FRAME_H__
