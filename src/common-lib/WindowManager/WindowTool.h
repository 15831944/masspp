/**
 * @file WindowTool.h
 * @brief interfaces of WindowTool class
 *
 * @author S.Tanaka
 * @date 2006.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_TOOL_H__
#define __KOME_WINDOW_TOOL_H__


#include <boost/function.hpp>


class wxWindow;
class wxFrameLayout;
class cbBarInfo;


namespace kome {
	namespace window {

		class MainFrame;

		/**
		 * @class WindowTool
		 * @brief This class has some functions of operating GUI.
		 */
		class WINDOW_MANAGER_CLASS WindowTool {
		public:
			/**
			 * @typedef enum DockingAlign
			 * @brief  type definition of docking align
			 */
			typedef enum {
				DOCK_ALIGN_TOP,
				DOCK_ALIGN_LEFT,
				DOCK_ALIGN_RIGHT,
				DOCK_ALIGN_BOTTOM
			} DockingAlign;

		protected:
			/** Main Window */
			static wxWindow* m_mainWindow;

		public:
			/**
			 * @fn static MainFrame* createMainWindow(
					const char* title,
					const int px,
					const int py,
					const unsigned int cx,
					const unsigned int cy
				)
			 * @brief creates main window
			 * @return the pointer to main window
			 */
			static MainFrame* createMainWindow(
				const char* title,
				const int px,
				const int py,
				const unsigned int cx,
				const unsigned int cy
			);

			/**
			 * @fn static void setMainWindow( wxWindow* wnd )
			 * @brief sets main window
			 * @param[in] wnd the pointer to the main window
			 */
			static void setMainWindow( wxWindow* wnd );

			/**
			 * @fn static wxWindow* getMainWindow()
			 * @brief gets main window
			 * @return the pointer to the main window
			 */
			static wxWindow* getMainWindow();

			/**
			 * @fn static void refresh()
			 * @brief refreshes window
			 */
			static void refresh();

		public:
			/**
			 * @fn static void showInfo( const char* msg, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows information message box
			 * @param[in] msg information message
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 */
			static void showInfo( const char* msg, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static void showError( const char* msg, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows error message box
			 * @param[in] msg error message
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 */
			static void showError( const char* msg, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static void showWarning( const char* msg, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows warning message box
			 * @param[in] msg warning message
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 */
			static void showWarning( const char* msg, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static bool showYesNo( const char* msg, const char* yesBtnName = NULL, const char* noBtnName = NULL, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows message with Yes-No button
			 * @param[in] msg dialog message
			 * @param[in] yesBtnName alternative name for the "Yes" button
			 * @param[in] noBtnName alternative name for the "No" button
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 * @return If true, Yes button is clicked.
			 */
			static bool showYesNo( const char* msg, const char* yesBtnName = NULL, const char* noBtnName = NULL, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static int showYesNoCancel( const char* msg, const char* yesBtnName = NULL, const char* noBtnName = NULL, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows message with Yes-No-Cancel button
			 * @param[in] msg dialog message
			 * @param[in] yesBtnName alternative name for the "Yes" button
			 * @param[in] noBtnName alternative name for the "No" button
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 * @return - 0 ... No
					   - 1 ... Yes
					   - Negative Value ... Cancel
			 */
			static int showYesNoCancel( const char* msg, const char* yesBtnName = NULL, const char* noBtnName = NULL, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static bool showOkCancel( const char* msg, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows message with OK-Cancel button
			 * @param[in] msg dialog message
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 * @return If true, OK button is clicked.
			 */
			static bool showOkCancel( const char* msg, const char* infoTitle = NULL, const char* info = NULL );

			/**
			 * @fn static bool showOkCancelWarning( const char* msg, const char* infoTitle = NULL, const char* info = NULL )
			 * @brief shows message with OK-Cancel button and Warning icon
			 * @param[in] msg dialog message
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 * @return If true, OK button is clicked.
			 */
			static bool showOkCancelWarning( const char* msg, const char* infoTitle = NULL, const char* info = NULL );

		public:
			/**
			 * @fn static int setTimer(
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
			static int setTimer(
				boost::function< void () > fun,
				int milliseconds,
				bool oneShot
			);

			/**
			 * @fn void killTimer( int timer )
			 * @brief kills timer
			 * @param[in] timer timer number
			 */
			static void killTimer( int timer );
		};
	}
}

#endif	// __KOME_WINDOW_TOOL_H__
