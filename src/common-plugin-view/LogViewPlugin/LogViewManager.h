/**
 * @file LogViewManager.h
 * @brief interfaces of LogViewManager class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_LOG_VIEW_MANAGER_H__
#define __KOME_VIEW_LOG_VIEW_MANAGER_H__


#include <wx/wx.h>


namespace kome {
	namespace view {

		class LogView;

		/**
		 * @class LogViewManager
		 * @brief log view plug-in management class
		 */
		class LogViewManager {
		protected:
			/**
			 * @fn LogViewManager()
			 * @brief constructor
			 */
			LogViewManager();

			/**
			 * @fn virtual ~LogViewManager()
			 * @brief destructor
			 */
			virtual ~LogViewManager();

		protected:
			/** log directory */
			std::string m_logDir;

			/** log file path */
			std::string m_logFilePath;

			/** file descriptor */
			FILE* m_fp;

			/** file log level */
			int m_fileLevel;

			/** view log level */
			int m_viewLevel;

			/** log view */
			LogView* m_view;

		//2014/08/20 add by t.okuno -> leave the log
		private:
			/** log file deletion date */
		//2014/08/20 add by t.okuno -> leave the log
		private:
			/** log file deletion date */
			static const int m_nDelLogDay;

			/** number of seconds per day */

			/** number of seconds per day */
			static const int m_nSecPerDay;

			/** the length of the log file */

			/** the length of the log file */
			static const unsigned int m_nLogFileLen;
		//2014/08/20 add by t.okuno <- leave the log

		//2014/08/20 add by t.okuno <- leave the log

		public:
			/**
			 * @fn const char* getFilePath()
			 * @brief gets log file path
			 * @return log file path
			 */
			const char* getFilePath();

			/**
			 * @fn void setFileLogLevel( const int level )
			 * @brief sets file log level
			 * @param level file log leve
			 */
			void setFileLogLevel( const int level );

			/**
			 * @fn int getFileLogLevel()
			 * @brief gets file log level
			 * @return file log level
			 */
			int getFileLogLevel();

			/**
			 * @fn void setViewLogLevel( const int level )
			 * @brief sets view log level
			 * @param level view log level
			 */
			void setViewLogLevel( const int level );

			/**
			 * @fn int getViewLogLevel()
			 * @brief gets view log level
			 * @return view log level
			 */
			int getViewLogLevel();

			/**
			 * @fn void createLogView()
			 * @brief creates log view
			 */
			void createLogView();

			/**
			 * @fn void setFunction()
			 * @brief sets log function
			 */
			void setFunction();

			/**
			 * @fn void closeLog()
			 * @brief closes log
			 */
			void closeLog();

			/**
			 * @fn bool hasView()
			 * @brief judges whether the log view is created
			 * @return If true, the log view is created
			 */
			bool hasView();

			/**
			 * @fn void releaseLogView()
			 * @brief releases log view
			 */
			void releaseLogView();

			/**
			 * @fn bool isVisibleView()
			 * @brief judges whether the log view is visible
			 * @return If true, the log view is visible
			 */
			bool isVisibleView();

			/**
			 * @fn void toggleView()
			 * @brief toggles visibility of the log view
			 */
			void toggleView();

		protected:
			/**
			 * @fn const char* getLogDir()
			 * @brief gets log directory
			 * @return log directory
			 */
			const char* getLogDir();

			//2014/08/20 add by t.okuno -> leave the log
			/**
			 * @fn bool DeleteOldLogFile( const char* szName )
			 * @brief delete old log file
			 * @param szName file name
			 * @return If true, old log file is deleted
			 */
			bool DeleteOldLogFile( const char* szName );
			//2014/08/20 add by t.okuno <- leave the log

			/**
			 * @fn static void writeLog( const char* log, int level )
			 * @brief writes log
			 * @param log log message
			 * @param level
			 */
			static void writeLog( const char* log, int level );

		public:
			/**
			 * @fn static LogViewManager& getInstance()
			 * @brief gets log view plug-in management object
			 * @return log view plug-in management object (This is the only object.)
			 */
			static LogViewManager& getInstance();
		};
	}
}

#endif		// __KOME_VIEW_LOG_VIEW_MANAGER_H__
