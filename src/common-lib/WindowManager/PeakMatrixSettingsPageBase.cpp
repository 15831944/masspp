/**
 * @file PeakMatrixSettingsPageBase.cpp
 * @brief implementation of PeakMatrixSettingsPageBase class
 *
  * @author H.Parry
 * @date 2011.05.17
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "WindowTool.h"
#include "PeakMatrixSettingsPageBase.h"

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE						3

#define	ID_SKIP_CHCK					10

#define PATH_KEY						"FILE_PATH"
#define SECTION							"Peak Matrix"


BEGIN_EVENT_TABLE( PeakMatrixSettingsPageBase, StandardWizardPage )
END_EVENT_TABLE()

// constructor
PeakMatrixSettingsPageBase::PeakMatrixSettingsPageBase( wxWizard* pParent, const char* sPanelName, bool bSkipBtn ) 
	:  StandardWizardPage( pParent ),
	m_sPanelName(sPanelName),
	m_bSkipBtn( bSkipBtn )
{
	m_parent = pParent;
	m_next = NULL;
	m_prev = NULL;

	m_settingsParam = "";
	m_bIsEdited = false;
	m_sectionName = SECTION;
}

// destructor
PeakMatrixSettingsPageBase::~PeakMatrixSettingsPageBase(void)
{
}

// create main sizer
wxSizer* PeakMatrixSettingsPageBase::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	wxStaticBoxSizer* InnerSizer = new wxStaticBoxSizer( wxHORIZONTAL,this );

	// This is a pure virtual function that must be implemented by each panel
	// to display its controls.
	createInnerSizer(InnerSizer);

	sizer->Add(InnerSizer, 1, wxALL | wxGROW | wxALIGN_CENTER, BORDER_SIZE);

	return sizer;
}

// transfer data from window
bool PeakMatrixSettingsPageBase::TransferDataFromWindow()
{
	//default
	if( !StandardWizardPage::TransferDataFromWindow() ){
		return false;
	}
	
	return true;
}

// set edited
void PeakMatrixSettingsPageBase::SetEdited(bool bIsEdited)
{
	m_bIsEdited = bIsEdited;
}

// set active matrix
void PeakMatrixSettingsPageBase::SetActiveMatrix(int nProject)
{
	m_nProject = nProject;
	
	m_sectionName = SECTION;
	m_sectionName << ":" << GetActiveMatrix();

}

// check whether a sample is selected as the standard sample on this panel.
bool PeakMatrixSettingsPageBase::IsSelectedStandardSample(const char* sSample)
{
	return false;
}

// Gets a pointer to the object handling the Parameters.ini file
kome::core::IniFile* PeakMatrixSettingsPageBase::GetParamsIniFile()
{
	kome::core::IniFile* pParamsIniFile = NULL;

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL )
	{
		// parameters file path
		const char* fileName = ini->getString( "Parameters", PATH_KEY, "parameters.ini" );
		std::string path = getabsolutepath( msppMgr.getConfDir(), fileName );
		pParamsIniFile = kome::core::IniFile::getIniFile( path.c_str() );
	}

	return pParamsIniFile;
}

// Gets the INI section name for the current matrix
const char* PeakMatrixSettingsPageBase::GetINISectionNameForMatrix()
{
	return m_sectionName;
}

// Delete all settings for the active matrix in the Parameters.ini file
void PeakMatrixSettingsPageBase::DeleteAllParamsForActiveMatrix()
{
	kome::core::IniFile* pParamsIniFile = GetParamsIniFile();
	if (pParamsIniFile != NULL)
	{
		int nParamCount = pParamsIniFile->getNumberOfParameters(GetINISectionNameForMatrix());

		for (int i = nParamCount - 1; i >= 0; i--)
		{
			pParamsIniFile->deleteValue(GetINISectionNameForMatrix(), pParamsIniFile->getParameterName(GetINISectionNameForMatrix(), i));
		}
	}
}

// Update standard sample combo on the panel
void PeakMatrixSettingsPageBase::updateStandardSampleCombo(bool bReadFromINIfile, std::vector< std::string > &samples, wxComboBox* stdCombo, const char* standardSampleKey)
{
	// Backup the current settings for Standard Sample
	wxString standardSample = stdCombo->GetValue().c_str();

	// If bReadFromINIfile is false, this function is being called because of samples being added / deleted on Group
	// Settings panel. In that case, this panel is set to edited, and we must preserve the current settings.
	// We only want to load values from the INI file if the bReadFromINIfile is set to true.
	if (bReadFromINIfile)
	{
		// Load settings not implemented via SettingParameterValues in the INI file
		kome::core::IniFile* pParamsIniFile = GetParamsIniFile();
		if (pParamsIniFile != NULL)
		{
			standardSample = pParamsIniFile->getString( GetINISectionNameForMatrix(), standardSampleKey, "" );
		}
	}

	// Clear all items from the Standard Samples combo.
	stdCombo->Clear();

	for( unsigned int i = 0; i < samples.size(); i++ ) {

		stdCombo->Append( wxT( samples[i].c_str() ) );
	}

	bool bComboBoxValueSet = false;

	if ( (!standardSample.IsEmpty()))
	{
		if (stdCombo->FindString(standardSample) != wxNOT_FOUND)
		{
			stdCombo->SetValue(standardSample);
			bComboBoxValueSet = true;
		}
	}

	if ( !bComboBoxValueSet && !( (samples.size() == 0) && (standardSample.IsEmpty()) ) )
	{
		// Either the standard sample in the INI file no longer exists in the combo,
		// or there was no standard sample specified in the INI file.
		// In either case, auto-select the first item in the list (which could be empty.
		// NOTE: The situation where the list is empty and no standard was specified in the INI file / UI is an
		// exception - in that case, there is nothing to do.
		stdCombo->SetSelection( 0 );
	}

	stdCombo->Enable( samples.size() > 0 );
}

