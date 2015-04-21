/**
 * @file StandardWizardPage.h
 * @brief interfaces of StandardWizardPage class
 *
 * @author S.Tanaka
 * @date 2013.02.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_WINDOW_STANDARD_WIZARD_PAGE_H__
#define __KOME_WINDOW_STANDARD_WIZARD_PAGE_H__


#include <wx/wx.h>
#include <wx/wizard.h>


namespace kome {
	namespace window {

		/**
		 * @class StandardWizardPage
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS StandardWizardPage : public wxWizardPageSimple {
		public:
			/**
			 * @fn StandardWizardPage( wxWindow* parent )
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			StandardWizardPage( wxWindow* parent );

			/**
			 * @fn virtual ~StandardWizardPage()
			 * @brief destructor
			 */
			virtual ~StandardWizardPage();

		protected:
			/**
			 * @fn virtual void createControls()
			 * @brief creates controls and sizers
			 */
			virtual void createControls();

			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual void onInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init panel event
			 * @param[in] evt event information
			 */
			virtual void onInitDialog( wxInitDialogEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_STANDARD_WIZARD_PAGE_H__
