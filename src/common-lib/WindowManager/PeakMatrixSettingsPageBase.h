/**
 * @file PeakMatrixSettingsPageBase.h
 * @brief interfaces of PeakMatrixSettingsPageBase class
 *
 * @author H.Parry
 * @date 2011.05.17
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MATRIX_PEAKMATRIXSETTINGS_PANELBASE_H__
#define __KOME_MATRIX_PEAKMATRIXSETTINGS_PANELBASE_H__


#include "StandardWizardPage.h"

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/wizard.h>

namespace kome {
	namespace window {
		
		class WindowTool;

		class DialogProgress;
			
		/**
		 * @class PeakMatrixSettingsPageBase
		 * @brief fill matrix panel class
		 */
		class WINDOW_MANAGER_CLASS PeakMatrixSettingsPageBase : public StandardWizardPage
		{
			public:
				/**
				 * @fn PeakMatrixSettingsPageBase( wxWizard* pParent, 
						const char* sPanelName, 
						bool bSkipcheckbtn = true
					)
				 * @brief constructor
				 * @param pParent parent window
				 * @param sPanelName panel name
				 * @param bSkipcheckbtn whether to include a "Default" button
				 */
				PeakMatrixSettingsPageBase( wxWizard* pParent, 
					const char* sPanelName, 
					bool bSkipcheckbtn = true
					);

				/**
				 * @fn virtual ~PeakMatrixSettingsPageBase(void)
				 * @brief destructor
				 */
				virtual ~PeakMatrixSettingsPageBase(void);
		protected:
				std::string				m_sPanelName;
			private:
				bool					m_bIsEdited;
				int						m_nProject;			// Active matrix
				wxString				m_sectionName;

				wxWizard*				m_parent;
				bool					m_bSkipBtn;
				wxWizardPageSimple*		m_next;
				wxWizardPageSimple*	    m_prev;

				std::string				m_settingsParam;
			public:

				/**
				 * @fn virtual void SetEdited( bool bIsEdited = true )
				 * @brief sets edited flag
				 * @param bIsEdited edited state
				 */
				virtual void SetEdited( bool bIsEdited = true );

				/**
				 * @fn int IsEdited()
				 * @brief gets edited flag
				 * @return edited flag state
				 */
				inline bool IsEdited() {return m_bIsEdited;}

				/**
				 * @fn virtual bool PreApplyValidationCheck(std::string& sError) = 0
				 * @brief Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
				 * @param sError error string from a single panel's pre-apply validation
				 * @return If validation succeeded, return true, otherwise false.
				 */
				virtual bool PreApplyValidationCheck(std::string& sError) = 0;

				/**
				 * @fn virtual bool SavePanelSettings(kome::core::Progress *pProgress) = 0;
				 * @brief This method is called when saving values.
				 * @param pProgress progress
				 * @return If saving values failed, this method returns false.
				 */
				virtual bool SavePanelSettings(kome::core::Progress *pProgress) = 0;

				/**
				 * @fn virtual bool IsValid() = 0
				 * @brief validate panel settings
				 * @return If validation failed, this method returns false.
				 */
				virtual bool IsValid() = 0;

				/**
				 * @fn void SetActiveMatrix(int nProject)
				 * @brief sets active matrix index
				 * @param nProject matrix index
				 */
				void SetActiveMatrix(int nProject);

				/**
				 * @fn virtual void updateStandardSamples(bool bReadFromINIfile = true) = 0
				 * @brief Update standard samples on this panel
				 * @param bReadFromINIfile whether to read from the INI file
				 */
				virtual void updateStandardSamples(bool bReadFromINIfile = true) = 0;

				/**
				 * @fn std::string GetPanelName() {return m_sPanelName;}
				 * @brief get the name of the panel
				 */
				std::string GetPanelName() {return m_sPanelName;}

				/**
				 * @fn void SetPanelName( std::string  panelName ){	m_sPanelName = panelName; }
				 * @brief set panel name
				 * @param panelName panel name
				 */
				void SetPanelName( std::string  panelName ){
					m_sPanelName = panelName;
				};

				/**
				 * @fn virtual void RefreshPanelContentsForMatrix() = 0
				 * @brief refresh panel contents for matrix
				 */
				virtual void RefreshPanelContentsForMatrix() = 0;

				/**
				 * @fn virtual bool IsSelectedStandardSample(const char* sSample)
				 * @brief check whether a sample is selected as the standard sample on this panel.
				 * @param sSample sample filename
				 * @return If the sample is selected as a standard sample, return true.
				 */
				virtual bool IsSelectedStandardSample(const char* sSample);

				/**
				 * @fn void DeleteAllParamsForActiveMatrix()
				 * @brief Gets the INI section name for the current matrix.
				 */
				void DeleteAllParamsForActiveMatrix();

				/**
				 * @fn wxWizardPageSimple* GetNextPage(){ return m_next; }
				 * @brief get next page
				 * @return next page
				 */
				wxWizardPageSimple* GetNextPage(){ return m_next; }

				/**
				 * @fn void SetNextPage( wxWizardPageSimple* page ){ m_next = page; }
				 * @brief set next page
				 * @param page wizard page simple
				 */
				void SetNextPage( wxWizardPageSimple* page ){ m_next = page; }

				/**
				 * @fn wxWizardPageSimple* GetPrevPage(){ return m_prev; }
				 * @brief get prev page
				 * @return prev page
				 */ 
				wxWizardPageSimple* GetPrevPage(){ return m_prev; }

				/**
				 * @fn void SetPrevPage( wxWizardPageSimple* page ){ m_prev = page; }
				 * @brief set prev page
				 * @param page wizard page simple
				 */
				void SetPrevPage( wxWizardPageSimple* page ){ m_prev = page; }

				/**
				 * @fn std::string getSettingParam(){ return m_settingsParam; }
				 * @brief get setting param
				 * @return settings 
				 */
				std::string getSettingParam(){ return m_settingsParam; }
					
				/**
				 * @fn void setSettingParam( const char* settings ){ m_settingsParam = settings; }
				 * @brief set setting param
				 * @param settings settings
				 */
				void setSettingParam( const char* settings ){ m_settingsParam = settings; }

				/**
				 * @fn bool IsSkipButton(){ return m_bSkipBtn; }
				 * @brief is skip button
				 * @return skip button 
				 */
				bool IsSkipButton(){ return m_bSkipBtn; };

			protected:
				/**
				 * @fn virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer) = 0
				 * @brief creates inner sizer
				 * @param InnerSizer inner sizer
				 */
				virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer) = 0;

				/**
				 * @fn void updateStandardSampleCombo(bool bReadFromINIfile, std::vector< std::string > &samples, wxComboBox* stdCombo, const char* standardSampleKey)
				 * @brief updates the contents of the Standard Sample combo on the panel, setting / preserving the INI file / previous setting accordingly.
				 * @param bReadFromINIfile whether to read from the INI file
				 * @param samples the samples registered in this matrix
				 * @param stdCombo pointer to the combo box
				 * @param standardSampleKey the key which stores the Standard Sample setting in the parameters INI file
				*/
				void updateStandardSampleCombo(bool bReadFromINIfile, std::vector< std::string > &samples, wxComboBox* stdCombo, const char* standardSampleKey);

				/**
				 * @fn virtual bool TransferDataFromWindow()
				 * @brief transfer window control data to value
				 * @return If a transfer failed, this method returns false.
				 */
				virtual bool TransferDataFromWindow();

				/**
				 * @fn inline int GetActiveMatrix() {return m_nProject;}
				 * @brief get the active matrix index
				 * @return the active matrix.
				 */
				inline int GetActiveMatrix() {return m_nProject;}

				/**
				 * @fn kome::core::IniFile* GetParamsIniFile()
				 * @brief Gets a pointer to the object handling the Parameters.ini file
				 * @return the Parameters.ini file object pointer.
				*/
				kome::core::IniFile* GetParamsIniFile();
					
				/**
				 * @fn const char* GetINISectionNameForMatrix()
				 * @brief Gets the INI section name for the current matrix
				 * @return the section name for the current matrix.
				 */
				const char* GetINISectionNameForMatrix();
				
			public:				
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();
			private:
				/**
				 * @fn void onDefault( wxCommandEvent& evt )
				 * @brief This method is called when "Default" button is clicked.
				 * @param evt button event information
				 */
				void onDefault( wxCommandEvent& evt );
			
			private:
				DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_MATRIX_PEAKMATRIXSETTINGS_PANELBASE_H__
