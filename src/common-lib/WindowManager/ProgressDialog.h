/**
 * @file ProgressDialog.h
 * @brief interfaces of ProgressDialog class
 *
 * @author S.Tanaka
 * @date 2011.05.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation.
 */


#ifndef __KOME_WINDOW_PROGRESS_DIALOG_H__
#define __KOME_WINDOW_PROGRESS_DIALOG_H__


#include "StandardDialog.h" 

#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class ProgressDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS ProgressDialog : public StandardDialog {
		public:
			/**
			 * @fn ProgressDialog( wxWindow* parent, const char* title )

			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 */
			ProgressDialog( wxWindow* parent, const char* title );

			/**
			 * @fn virtual ~ProgressDialog()
			 * @brief destructor
			 */
			virtual ~ProgressDialog();

		protected:
			/** range */
			int m_range;

			/** position */
			int m_pos;

			/** stopped flag */
			volatile bool m_stopped;

			/** message */
			std::string m_message;

			/** information title */
			std::string m_infoTitle;

			/** information */
			std::string m_info;

		protected:
			/** gauge */
			wxGauge* m_gauge;

			/** message text */
			wxStaticText* m_msgText;

			/** elapsed time text */
			wxStaticText* m_elapsedText;

			/** estimated time text */
			wxStaticText* m_estimatedText;

			/** remaining time text */
			wxStaticText* m_remainingText;

			/** information text */
			wxTextCtrl* m_infoText;

			/** sizer */
			wxSizer* m_sizer;

			/** information control */
			wxWindow* m_infoCtrl;

		protected:
			/** timer */
			kome::core::Timer m_timer;

		protected:
			/** disabler */
			wxWindowDisabler* m_disabler;

		public:
			/**
			 * @fn void setRange( const int range )
			 * @brief set progress range
			 * @param[in] range progress range
			 */
			void setRange( const int range );

			/**
			 * @fn int getRange()
			 * @brief gets the progress range
			 * @return progress range
			 */
			int getRange();

			/**
			 * @fn void setPos( const int pos )
			 * @brief sets progress position
			 * @param[in] pos progress position
			 */
			void setPos( const int pos );

			/**
			 * @fn int getPos()
			 * @brief gets the progress position
			 * @return progress position
			 */
			int getPos();

			/**
			 * @fn void setMessage( const char* msg )
			 * @brief sets progress message
			 * @param[in] msg progress message
			 */
			void setMessage( const char* msg );

			/**
			 * @fn const char* getMessage()
			 * @brief gets the progress message
			 */
			const char* getMessage();

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

			/**
			 * @fn bool isStopped()
			 * @brief judges stopped or not
			 * @return If true, the process is stoped.
			 */
			bool isStopped();

		protected:
			/**
			 * @fn std::string getTimeString( const double sec )
			 * @brief gets time string
			 * @param[in] sec second
			 * @return time string
			 */
			std::string getTimeString( const double sec );
	
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn virtual bool onCancel()
			 * @brief This method is called when the "Cancel" button is clicked. (override method)
			 * @return If true, this dialog is closed.
			 */
			virtual bool onCancel();

		public:
			/**
			 * @fn virtual bool Show( bool show )
			 * @brief shows window (override method)
			 * @param[in] show If true, displays the window, Otherwise, hides it.
			 * @return true if the window has been shown or hidden.
			 */
			virtual bool Show( bool show );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_PROGRESS_DIALOG_H__
