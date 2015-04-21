/**
 * @file Wizard.h
 * @brief interfaces of Wizard class
 *
 * @author S.Tanaka
 * @date 2012.10.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation.
 */


#ifndef __KOME_WINDOW_WIZARD_H__
#define __KOME_WINDOW_WIZARD_H__


#include <wx/wx.h>
#include <wx/wizard.h>


namespace kome {
	namespace window {

		/**
		 * @class Wizard
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS Wizard : public wxWizard {
		public:
			/**
			 * @fn Wizard( wxWindow* parent, const char* title, wxBitmap& bitmap = wxNullBitmap, const bool hasSkip = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] bitmap bitmap
			 * @param[in] hasSkip If true, this dialog has skip button.
			 */
			Wizard( wxWindow* parent, const char* title, wxBitmap& bitmap = wxNullBitmap, const bool hasSkip = false );

			/**
			 * @fn virtual ~Wizard()
			 * @brief destructor
			 */
			virtual ~Wizard();

		protected:
			/** skip button */
			wxButton* m_skipButton;

			/** skip flag */
			bool m_hasSkip;

			/*** status thread condition*/
			bool m_state;
		public:
			/**
			 * @fn void setThreadCondition( bool bflg )
			 * @brief set thread condition
			 * @param bflg status flag
			 */
			void setThreadCondition( bool bflg );

			/**
			 * @fn bool getThreadCondition()
			 * @brief get thread condition
			 * @return status
			 */
			bool getThreadCondition();

		protected:
			/**
			 * @fn void insertSkipButton()
			 * @brief inserts skip button
			 */
			void insertSkipButton();

		protected:
			/**
			 * @fn virtual void onSkip( wxWizardPage* currentPage )
			 * @brief This method is called when the Skip button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onSkip( wxWizardPage* currentPage );

			/**
			 * @fn virtual bool canSkip( wxWizardPage* currentPage )
			 * @brief judges whether the Skip button is enabled or not
			 * @return If true, the Skip button is enabled.
			 */
			virtual bool canSkip( wxWizardPage* currentPage );

			/**
			 * @fn virtual void onNext( wxWizardPage* currentPage )
			 * @brief This method is called when the Next button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onNext( wxWizardPage* currentPage );

			/**
			 * @fn virtual void onPrev( wxWizardPage* currentPage )
			 * @brief This method is called when the Back button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onPrev( wxWizardPage* currentPage );

		protected:
			/**
			 * @fn void onSkip( wxCommandEvent& evt )
			 * @brief This method is called when the Skip button is clicked.
			 * @param[in] evt event information
			 */
			void onSkip( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateSkip( wxUpdateUIEvent& evt )
			 * @brief This method is called when the UI of the Skip button is updated.
			 * @param[in] evt event information
			 */
			void onUpdateSkip( wxUpdateUIEvent& evt );

			/**
			 * @fn void onNextButton( wxCommandEvent& evt )
			 * @brief This method is called when the Next button is clicked.
			 * @param[in] evt event information
			 */
			void onNextButton( wxCommandEvent& evt );

			/**
			 * @fn void onPrevButton( wxCommandEvent& evt )
			 * @brief This method is called when the Back button is clicked.
			 * @param[in] evt event information
			 */
			void onPrevButton( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_WIZARD_H__
