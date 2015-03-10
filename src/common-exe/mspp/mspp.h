/**
 * @file mspp.h
 * @brief mspp interfaces
 *
 * @author S.Tanaka
 * @date 2006.07.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MSPP_BATCH_H__
#define __KOME_MSPP_BATCH_H__


#include <string>
#include <wx/wx.h>
#include "wx/snglinst.h"	// @date 2011.02.16 <Add> M.Izumi

// @date 2011.09.14 <Add> M.Izumi ->
#define STATUSBAR_DISP		"DispMode"
#define STATUSBAR_CHROM		"ChromFilter"
#define STATUSBAR_EDIT		"EditPeaks"
#define STATUSBAR_SYNC		"SyncAxes"
#define STATUS_DISP			"WINDOW"
#define STATUS_CHROM		"CHROMATOGRAM_FILTER"
#define STATUS_SYNC			"SYNCHRONIZED_RANGE"
#define STATUS_EDIT			"LABEL_EDIT_MODE"
#define MODE_DISP			"Display Mode: "
#define MODE_CHROM			"Chromatogram Filter: "
#define MODE_SYNC			"Synchronize Axes: On"
#define MODE_EDIT			"Edit Peaks: On"
// @date 2011.09.14 <Add> M.Izumi <-

namespace kome {
	namespace mspp {
		/**
		 * @class MsppApp
		 * @brief mspp application class
		 */
		class MsppApp : public wxApp {
		public:
			/**
			 * @fn MsppApp()
			 * @brief constructor
			 */
			MsppApp();

			/**
			 * @fn virtual ~MsppApp()
			 * @brief destructor
			 */
			virtual ~MsppApp();

		protected:
			/**
			 * @fn void showAboutDialog()
			 * @brief shows about dialog
			 */
			void showAboutDialog();

		protected:
			/**
			 * @fn virtual bool OnInit()
			 * @brief This method is called when initializing application.
			 * @return true: continue application, false: exit application
			 */
			virtual bool OnInit();
			
			/**
			 * @fn virtual int OnExit()
			 * @brief This method is called after destroying all application windows.
			 * @return integer value. (This value is ignored.)
			 */
			virtual int OnExit();

		protected:
			 /**
			  * @fn void onKeyDown( wxKeyEvent& evt )
			  * @brief This method is called when a key is down.
			  * @param[in] evt key event
			  */
			 void onKeyDown( wxKeyEvent& evt );

			/**
			 * @fn void onKeyUp( wxKeyEvent& evt )
			 * @brief This method is called when a key is up.
			 * @param[in] evt key event
			 */
			 void onKeyUp( wxKeyEvent& evt );

		protected:
			/**
			 * @fn void onAbout( wxCommandEvent& evt )
			 * @brief This method is called when "About Mass++" menu is selected.
			 * @param evt event information
			 */
			void onAbout( wxCommandEvent& evt );
	
			/**
			 * @fn void onCommandLineHelp( wxCommandEvent& evt )
			 * @brief This method is called when "Command Line Help" menu is selected.
			 * @param evt event information
			 */
			void onCommandLineHelp( wxCommandEvent& evt );
			
		protected:
			/**
			 * @fn bool init()
			 * @brief initializes environment
			 * @return If it succeeded to initialize environment, this function returns true.
			 */
			bool init();
	
			/**
			 * @fn voidn finalize()
			 * @brief finalize the application
			 */
			void finalize();
	
			/**
			 * @fn bool loadPluginFiles()
			 * @brief load plug-in files
			 * @return - true: It succeeded to load plug-in files.
					   - false: It failed to load plug-in files.
			 */
			bool loadPluginFiles();
	
			/**
			 * @fn void executeCommand()
			 * @brief executes command
			 */
			void executeCommand();
	
			/**
			 * @fn bool loadOtherFolderPluginFiles()
			 * @brief load other folder plug-in files
			 */
			bool loadOtherFolderPluginFiles();

		protected:
			/**
			 * @fn static bool isPluginPath( const char* path )
			 * @brief judge whether specified path is plug-in XML file or directory.
			 * @param path file path
			 * @return If the specified file path is plug-in XML file or directory, this method returns true.
			 */
			static bool isPluginPath( const char* path );
	
		private:
			/** Single Instance Checker */
			wxSingleInstanceChecker* m_checker;	// @date 2011.02.16 <Add> M.Izumi
		
			/**
			 * @fn void setStatusModeValStatusBar()
			 * @brief ステータスバーに各モードの状態をセット
			 */
			void setStatusModeValStatusBar();  // @date 2011.09.14 <Add> M.Izumi
			
		protected:
			/**
			 * @fn static void setKeyEvent( wxKeyEvent& evt0, kome::evt::KeyEvent& evt1 )
			 * @brief sets key event
			 * @param[in] evt0 key event object of wxWidgets
			 * @param[out] evt1 key event object of Mass++
			 */
			static void setKeyEvent( wxKeyEvent& evt0, kome::evt::KeyEvent& evt1 );

		private:
			/**
			 * @fn void loadActiveTable()
			 * @brief プラグイン管理機能の初期設定をINIファイルから読み込み
			 */
			 void loadActiveTable();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

DECLARE_APP( kome::mspp::MsppApp )


#endif	// __KOME_MSPP_BATCH_H__
