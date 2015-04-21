/**
 * @file DialogProgress.h
 * @brief interfaces of DialogProgress class
 *
 * @author S.Tanaka
 * @date 2006.10.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_DIALOG_PROGRESS_H__
#define __KOME_WINDOW_DIALOG_PROGRESS_H__


#include <string>
#include <wx/wx.h>
#include <wx/progdlg.h>


namespace kome {
	namespace window {

		class ProgressDialog;

		/**
		 * @class DialogProgress
		 * @brief Showing progress with dialog
		 */
		class WINDOW_MANAGER_CLASS DialogProgress : public kome::core::Progress {
		public:
			/**
			 * @fn DialogProgress( wxWindow* parent, const char* title, const char* firstMsg = NULL )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title progress dialog title
			 * @param[in] firstMsg first message
			 */
			DialogProgress( wxWindow* parent, const char* title, const char* firstMsg = NULL );

			/**
			 * @fn virtual ~DialogProgress()
			 * @brief destructor
			 */
			virtual ~DialogProgress();

		protected:
			/** progress dialog */
			ProgressDialog* m_dialog;

		protected:
			/** parent windows */
			wxWindow* m_parent;

			/** progress length */
			int m_length;

			/** information */
			std::string m_info;

			/** information title */
			std::string m_infoTitle;

			/** message */
			std::string m_msg;

			/** stopped */
			bool m_stopped;

		protected:
			/**
			 * @fn void createDialog()
			 * @brief creates dialog
			 */
			void createDialog();

// >>>>>>	@Date:2014/01/31	<Add>	A.Ozaki
//
		private:
			void	Sleep( const DWORD unMsec );
//
// <<<<<<	@Date:2014/01/31	<Add>	A.Ozaki

		public:
			/**
			 * @fn void setInformation( const char* title, const char* info )
			 * @brief sets the informatin
			 * @param[in] title information title
			 * @param[in] info information
			 */
			void setInformation( const char* title, const char* info );

			/**
			 * @fn void setInformation( const char* info )
			 * @brief sets information
			 * @param[in] info information
			 */
			void setInformation( const char* info );

			/**
			 * @fn const char* getInformationTitle()
			 * @brief gets the information title
			 * @return information title
			 */
			const char* getInformationTitle();

			/**
			 * @fn const char* getInformation()
			 * @brief gets the information
			 */
			const char* getInformation();

		protected:
			/**
			 * @fn virtual void onSetRange( const int start, const int end )
			 * @brief This method is called by setRange method. (override method)
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			virtual void onSetRange( const int start, const int end );

			/**
			 * @fn virtual void onSetPosition( const int pos, const int prevPos )
			 * @brief This method is called by setPosition method. (override method)
			 * @param[in] pos progress position
			 * @param[in] prevPos previous position
			 */
			virtual void onSetPosition( const int pos, const int prevPos );

			/**
			 * @fn virtual void onSetStatus( const char* status, const bool bForced = false )
			 * @brief This method is called by setStatus method. (override method)
			 * @param[in] status status
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( const char* status, const bool bForced = false );		// @Date:2013/11/25	<Add>	A.Ozaki

			/**
			 * @fn virtual bool onIsStopped()
			 * @brief This method is called by isStopped method. (override method)
			 * @return If it has to finish, this method returns true
			 */
			virtual bool onIsStopped();

			// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual void onFill( void )
			 * @brief This method is called by fill method. (override method)
			 */
			virtual void onFill( void );
			//
			// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
		};
	}
}

#endif		// __KOME_WINDOW_DIALOG_PROGRESS_H__
