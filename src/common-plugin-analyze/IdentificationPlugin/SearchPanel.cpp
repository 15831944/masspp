/**
 * @file SearchPanel.cpp
 * @brief implementation of SearchPanel class
 *
  * @author H.Parry
 * @date 2011.06.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"

#include "IdentificationDialogBase.h"
#include "ComparativeIdentificationDialog.h"
#include "SettingsPanelBase.h"
#include "SearchPanel.h"
#include "DatabaseFilesListCtrl.h"
#include "IdentificationOperation.h"

#include <sstream>

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SECTION "Search"
#define TARGET_DATABASE_PARAM_NAME	"database"
#define TARGET_DATABASE_PARAM_KEY	"DATABASE"
#define TARGET_TAXONOMY_PARAM_NAME	"taxonomy"
#define TARGET_TAXONOMY_PARAM_KEY	"TAXONOMY"
#define TARGET_DATABASE_FILES_PARAM_NAME	"databasefiles"
#define TARGET_DATABASE_FILES_PARAM_KEY		"DATABASEFILES"

#define INPUT_FILE_NAME    "Input File"
#define INPUT_FILE_KEY     "INPUT_FILE"
#define OUTPUT_FILE_NAME    "Output File"
#define OUTPUT_FILE_KEY     "OUTPUT_FILE"

BEGIN_EVENT_TABLE( SearchPanel, SettingsPanelBase )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, SearchPanel::onSettingChanged)
	EVT_SIZE( SearchPanel::onSize )
END_EVENT_TABLE()


SearchPanel::SearchPanel( wxWindow* pParent, IdentificationDialogBase* pParentDlg )
							: SettingsPanelBase(pParent, pParentDlg, "Search")
{
	// initialize
	m_pSettingsPanel1 = NULL;
	m_pSettingsPanel2 = NULL;
	m_pSettingsPanel3 = NULL;

	m_databaseFilesList = NULL;
	m_databaseFilesListpanel = NULL;
}

SearchPanel::~SearchPanel(void)
{
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool SearchPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;

	// validate settings
	if (!(m_pSettingsPanel1->Validate() && (((!m_pSettingsPanel2) || m_pSettingsPanel2->Validate()) && (m_pSettingsPanel3->Validate()))))
	{
		// Failure occurred, but no message to append to errorString.
		bSuccess = false;
	}

	// Usually, in the PreApplyValidationCheck function for a panel, we conduct all validation checks
	// and concatenate the errors into one string. However, since the SettingsPanel::Validate function
	// does not provide a string - it displays a dialog - we will not continue to the next validation
	// step until the first validation problem has been fixed by the user.
	if (bSuccess)
	{
		TransferDataFromWindow();	// get the dialog settings into m_settings object.
		std::string strValidationError = m_pParentDlg->GetSearchEngine()->validateSettings(&m_settings);
		if (!strValidationError.empty())	// A validation error occurred.
		{
			errorString << strValidationError;
			bSuccess = false;
		}
	}

	sError = errorString.str();

	return bSuccess;
}

// This method is called when prompting for export / import file paths.
bool SearchPanel::PromptForFilepaths(std::vector<std::string>& asInputFilePaths, std::string& sOutputFilePath)
{
	bool bSuccess = false;

	asInputFilePaths.clear();
	sOutputFilePath = "";

	// For the Search panel, both asInputFilePaths (single) and sOutputFilePath are required.
	// parameters

	// prepare page
	kome::plugin::ParameterSettings params( NULL, false );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );

	// input file
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_INPUT );
	val->addFilterItem( "Mascot Generic Format", "*.mgf", "Text file containing exported peaks" );
	val->setDefaultValue((GetDefaultSearchIODirectory() + "\\").c_str());

	kome::plugin::SettingsParam* param = params.addParam();
	param->setValue( val );
	param->setName( INPUT_FILE_NAME );
	param->setSection( (m_pParentDlg->GetSearchEngine()->getName() + " " + SECTION).c_str() );
	param->setKey( INPUT_FILE_KEY );
	param->setRequired( true );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Exported Peaks (Input)" );

	// output file
	val = params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_OUTPUT );

	std::string strResultFileType = m_pParentDlg->GetSearchEngine()->getName() + " Result";
	std::string strResultFileFilter = "*." + m_pParentDlg->GetSearchEngine()->getResultFileType();
	val->addFilterItem( strResultFileType.c_str(), strResultFileFilter.c_str(), "Output results file" );
	val->setDefaultValue((GetDefaultSearchIODirectory() + "\\").c_str());

	param = params.addParam();
	param->setValue( val );
	param->setName( OUTPUT_FILE_NAME );
	param->setSection( (m_pParentDlg->GetSearchEngine()->getName() + " " + SECTION).c_str() );
	param->setKey( OUTPUT_FILE_KEY );
	param->setRequired( true );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Results (Output)" );

	// dialog
	kome::objects::SettingParameterValues settings;
	kome::window::SettingsPageDialog dlg( this, page, &settings );
	dlg.SetTitle("Search - Input / Output Files");

	bool bCloseIOFilesDlg = false;

	while (!bCloseIOFilesDlg)
	{
		if( dlg.ShowModal() == wxID_OK ) {

			if (!(fileexists(settings.getParameterValue( OUTPUT_FILE_NAME ))) ||
				(kome::window::WindowTool::showOkCancel( "The specified Results (Output) file already exists. This will be overwritten with new search results." )))
			{
				asInputFilePaths.push_back(settings.getParameterValue( INPUT_FILE_NAME ));
				sOutputFilePath = settings.getParameterValue( OUTPUT_FILE_NAME );
				SetDefaultSearchIODirectory(getdir( sOutputFilePath.c_str() ));

				bCloseIOFilesDlg = true;
				bSuccess = true;
			}
		}
		else
		{
			bCloseIOFilesDlg = true;
		}
	}

	return bSuccess;
}

// check the spectrum info list for insufficient information
bool SearchPanel::CheckSpectrumInfoTags(std::vector<SpecInfo>& spectrumInfoList, std::vector<kome::objects::Peaks*>& peakLists, kome::core::Progress& prog)
{
	bool bSuccess = true;

	// This check is required to prevent a /0 error when there are no spectra.
	if (spectrumInfoList.size() > 0)
	{
		prog.setRange(0, spectrumInfoList.size());
		prog.setPosition(0);
	}

	for (int i = spectrumInfoList.size() - 1; (i >= 0) && (!prog.isStopped()); i--)
	{
		prog.setPosition(spectrumInfoList.size() - i);
		prog.setStatus( FMT( "Checking Spectrum Information [%d/%d] (%s)", spectrumInfoList.size() - i, spectrumInfoList.size(), spectrumInfoList[i].name.c_str()).c_str());

		if ( ((spectrumInfoList[i].specId == -1) || (spectrumInfoList[i].sampleIndex == -1))
			||
			((spectrumInfoList[i].path.empty()) || ((m_pParentDlg->ComparativeId()) && (spectrumInfoList[i].peakIds.size() == 0))) )
		{
			spectrumInfoList.erase(spectrumInfoList.begin() + i);
			delete peakLists[i];
			peakLists.erase(peakLists.begin() + i);
			bSuccess = false;
		}
	}

	if (!bSuccess)
	{
		std::string strMenuItemName = m_pParentDlg->GetSearchEngine()->getName();
		if (m_pParentDlg->ComparativeId())
		{
			strMenuItemName +=  " Comparative Identification";
		}
		else
		{
			strMenuItemName +=  " Identification";
		}
		std::string strError;

		if (spectrumInfoList.size() > 0)
		{
			strError = "Some spectra in the MGF file do not contain the required spectrum TITLE tags.\n";
			strError += strMenuItemName;
			strError += " Search will be performed on only the spectra with sufficient TITLE tags.";
			bSuccess = kome::window::WindowTool::showOkCancel( strError.c_str() );
		}
		else
		{
			strError = "The MGF file does not contain the required spectrum TITLE tags.\n";
			strError += strMenuItemName;
			strError += " Search cannot be performed.";
			kome::window::WindowTool::showError( strError.c_str() );
		}
	}

	return (!prog.isStopped()) && bSuccess;
}

bool SearchPanel::SavePanelSettings(kome::core::Progress *pProgress, std::vector<std::string>& sInputFilePaths, const std::string &sOutputFilePath)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	if ((sInputFilePaths.size() == 0) || sOutputFilePath.empty())
	{
		return false;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;
	// Do we want to suppress an error dialog?
	bool bSuppressErrorDialog = false;

	infoString << GetPanelName().c_str() << " input File: " << sInputFilePaths[0] << "\n";
	infoString << GetPanelName().c_str() << " output File: " << sOutputFilePath << "\n";

	// Add all settings values from the Search panel into infoString.

	for (unsigned int iParamIndex = 0; iParamIndex < m_settings.getNumberOfParameters(); iParamIndex++)
	{
		infoString << m_settings.getParameterName(iParamIndex) << ": ";
		infoString << m_settings.getParameterValue(iParamIndex) << "\n";
	}

	if (m_databaseFilesList != NULL)
	{
		// Database files
		std::vector< std::string > dbFilesFullList;
		m_pParentDlg->GetSearchEngine()->getDbFiles(m_settings.getParameterValue(TARGET_DATABASE_PARAM_NAME), dbFilesFullList);
		// Selected database files
		std::string selectedDatabaseFiles = "";
		for( unsigned int i = 0; i < m_databaseFilesList->getNumberOfData(); i++ ) {

			if (m_databaseFilesList->isSelected(m_databaseFilesList->getData( i )))
			{
				// Append each database file into selectedDatabaseFiles.
				if (!selectedDatabaseFiles.empty())
				{
					selectedDatabaseFiles += ":";
				}
				selectedDatabaseFiles += dbFilesFullList[m_databaseFilesList->getData( i )];
			}
		}

		infoString << "Database Files:\n";
		infoString << selectedDatabaseFiles;

		m_settings.setValue(TARGET_DATABASE_FILES_PARAM_NAME, selectedDatabaseFiles.c_str());
		// Save settings not implemented via SettingParameterValues in the INI file
		kome::core::IniFile* pParamsIniFile = GetParamsIniFile();
		if (pParamsIniFile != NULL)
		{
			pParamsIniFile->setString( "Search", TARGET_DATABASE_FILES_PARAM_KEY, selectedDatabaseFiles.c_str() );
		}
	}

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());

	std::vector<SpecInfo> spectrumInfoList;
	std::vector<kome::objects::Peaks*> peakLists;

	std::string mgfFilePath = sInputFilePaths[0];
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	kome::core::Progress* subProgress = pProgress;

	std::string strAdditionalSettings;
	strAdditionalSettings = FMT("-in %s", mgfFilePath.c_str());
	strAdditionalSettings += FMT(" -out %s", sOutputFilePath.c_str());
	IdentificationOperation* opt = CreateIdentificationOperation(strAdditionalSettings.c_str());

	if (!m_pParentDlg->ApplyAllProcessing())
	{
		// Apply was clicked on this panel only, so the contents of the MGF file have to be read, checked and possibly rewritten.
		// Otherwise, if Apply All was used, the input MGF file can be used as is.

		pProgress->createSubProgresses(6);
		subProgress = pProgress->getSubProgress( 0 );

		if (identMgr.readMgf(sInputFilePaths[0].c_str(), spectrumInfoList, peakLists, *subProgress))
		{
			subProgress = pProgress->getSubProgress( 1 );
			pProgress->setStatus("Checking Spectrum Information from MGF File.");
			int nSpectraCount = spectrumInfoList.size();
			if (CheckSpectrumInfoTags(spectrumInfoList, peakLists, *subProgress))
			{
				subProgress = pProgress->getSubProgress( 2 );

				// Set the spectrum info list in the result object.
				identMgr.getCommonSearchResult()->getSpectrumInfoList() = spectrumInfoList;

				// Set the peak lists in the result object.
				identMgr.getCommonSearchResult()->setPeakLists(peakLists, *subProgress);

				// Either:
				// 1. There was no problem - so the spectrum count is the same, or
				// 2. there was a problem with some, but not all, of the spectra, so the spectrum count has changed.

				if (spectrumInfoList.size() != nSpectraCount)
				{
					// Some, but not all, of the spectra were problemmatic and were removed, so write a
					// new MGF file.
					kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

					// tmp dir
					std::string tmpDir = getpath( msppMgr.getTmpDir(), "mascot" );
					if( !fileexists( tmpDir.c_str() ) ) {
						if( !makedirectory( tmpDir.c_str() ) ) {
							LOG_ERROR_CODE( FMT( "Failed to create the directory. [%s]", tmpDir.c_str() ), ERR_OTHER );
							tmpDir = "";
						}
					}

					if (!tmpDir.empty())
					{
						std::string mgfFile = msppMgr.getTmpFileName( "ms2peaks", ".txt", "mascot" );
						std::string tmpMgfPath = getpath( tmpDir.c_str(), mgfFile.c_str() );

						subProgress = pProgress->getSubProgress( 3 );
						if (identMgr.writeMgf(tmpMgfPath.c_str(), spectrumInfoList, peakLists, *subProgress))
						{
							mgfFilePath = tmpMgfPath;
						}
						else
						{
							// Failed to write the temporary MGF file, so search cannot be performed.
							LOG_ERROR_CODE( FMT( "Failed to write the MGF file. [%s]", tmpMgfPath.c_str() ), ERR_OTHER );
							mgfFilePath = "";
						}

					}
					else
					{
						// Failed to create the temporary directory, so search cannot be performed.
						mgfFilePath = "";
					}
				}
			}
			else
			{
				// Do not proceed with search.
				mgfFilePath = "";

				// If CheckSpectrumInfoTags has returned false, it has already displayed an error dialog,
				// so we do not want to display a 2nd one.
				bSuppressErrorDialog = true;
			}

			subProgress = pProgress->getSubProgress( 4 );

			// Must delete all Peaks objects that have been created.
			int nNumPeaks = peakLists.size();
			// This check is required to prevent a /0 error when there are no peak lists.
			if( nNumPeaks > 0 ) {
				subProgress->setRange(0, nNumPeaks);
				subProgress->setPosition(0);
			}
			while (!peakLists.empty())
			{
				pProgress->setStatus(FMT( "Freeing Temporary Peak Lists [%d/%d]", ( nNumPeaks - peakLists.size() + 1 ), nNumPeaks).c_str());
				subProgress->setPosition( nNumPeaks - peakLists.size() + 1 );

				delete peakLists.back();
				peakLists.pop_back();
			}
		}

		subProgress = pProgress->getSubProgress( 5 );
	}

	if (!mgfFilePath.empty())
	{
		if (m_pParentDlg->GetSearchEngine()->search(sOutputFilePath.c_str(), *identMgr.getCommonSearchResult(), mgfFilePath.c_str(), &m_settings, m_settings.getParameterValue(TARGET_DATABASE_PARAM_NAME), *subProgress))
		{
			if (!m_pParentDlg->GetSearchEngine()->openResult(identMgr.getCommonSearchResult()->getResultLocation().c_str()))
			{
				// Display a warning dialog if failed to display search results, but do NOT set bSuccess = false.
				std::string strWarning = "An error occurred while trying to display search results:\n";
				strWarning += getlasterrormessage();
				kome::window::WindowTool::showWarning( strWarning.c_str() );
			}

			if (!m_pParentDlg->ApplyAllProcessing())
			{
				// Apply was clicked on this panel only, so a warning dialog can be shown if multiple result files were necessary.
				// Otherwise, if Apply All was used, it should not be shown (because the user didn't specify the result file).

				std::vector<std::string> fileList;
				m_pParentDlg->GetSearchEngine()->getResultFiles(fileList);
				if (fileList.size() > 1)
				{
					std::ostringstream infoString;
					infoString << "Results have been output to multiple files:\n";
					for (int i = 0; i < static_cast<int>(fileList.size()); i++)
					{
						infoString << fileList[i] << "\n";
					}
					kome::window::WindowTool::showInfo( infoString.str().c_str() );
				}
			}

			bSuccess = true;
		}
	}

	if( !pProgress->isStopped() && bSuccess )
	{
		pProgress->fill();
	}
	else
	{
		if (opt != NULL)
		{
			kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
			optMgr.removeOperation( opt );
			delete opt;
		}
	}

	if ((!bSuccess && !bSuppressErrorDialog) && !pProgress->isStopped())
	{
		std::string strError = "An error occurred while searching:\n";
		strError += getlasterrormessage();
		kome::window::WindowTool::showError( strError.c_str() );
	}

	return bSuccess;
}

// transfer data to window
bool SearchPanel::TransferDataToWindow() {

	// default
	if( !kome::window::StandardPanel::TransferDataToWindow() ) {
		return false;
	}

	if (m_databaseFilesList != NULL)
	{
		m_databaseFilesList->setDbName(m_settings.getParameterValue(TARGET_DATABASE_PARAM_NAME));

		// Load settings not implemented via SettingParameterValues in the INI file
		kome::core::IniFile* pParamsIniFile = GetParamsIniFile();
		std::string selectedDatabaseFiles;
		if (pParamsIniFile != NULL)
		{
			selectedDatabaseFiles = pParamsIniFile->getString( "Search", TARGET_DATABASE_FILES_PARAM_KEY, "" );
		}

		// Parse the Database Files string.
		std::vector<std::string> selectedDatabaseFilesList;

		if (!selectedDatabaseFiles.empty())
		{
			unsigned int colonPos = 0;
			while (colonPos != std::string::npos)
			{
				colonPos = selectedDatabaseFiles.find(":");
				if (colonPos != std::string::npos)
				{
					// Get the next database file.
					selectedDatabaseFilesList.push_back(selectedDatabaseFiles.substr(0, colonPos));
					selectedDatabaseFiles = selectedDatabaseFiles.substr(colonPos + 1, selectedDatabaseFiles.length() - 1 - colonPos);
				}
				else
				{
					selectedDatabaseFilesList.push_back(selectedDatabaseFiles);
					selectedDatabaseFiles = "";
				}
			}
		}

		m_databaseFilesList->setSelectedDbs(selectedDatabaseFilesList);
	}

	return true;
}

// create inner sizer
void SearchPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer)
{
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );

	wxSizer* pTopSizer = new wxGridSizer( 2, 1, GRID_GAP );

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginInfo* info;
	info = plgMgr.getPlugin( "Identification" );
	kome::plugin::SettingsPage* page;
	page = info->getPage("search");
	page->setParameters(m_settings);

	// panel
	m_pSettingsPanel1
		= new kome::window::SettingsPanel( this, page, false, &m_settings );
	pTopSizer->Add( m_pSettingsPanel1, 1, wxALL | wxGROW, BORDER_SIZE );

	if ( (m_pParentDlg->GetSearchEngine()->getNumberOfDatabases() > 0) ||
		(m_pParentDlg->GetSearchEngine()->getNumberOfTaxonomies() > 0) )
	{
		wxStaticBoxSizer* pDBandTaxonomySizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Target" ) );

		// page
		page = new kome::plugin::SettingsPage( m_params );

		if (m_pParentDlg->GetSearchEngine()->getNumberOfDatabases() > 0)
		{
			// Database
			kome::plugin::SettingsValue* val = m_params.addValue();
			val->setType( kome::plugin::SettingsValue::TYPE_STRING );
			val->setEnumType(kome::plugin::SettingsValue::ENUM_FUNCTION);
			val->setEnumKey("databases");

			kome::plugin::SettingsParam* param = param = m_params.addParam();
			param->setName( TARGET_DATABASE_PARAM_NAME );
			param->setValue( val );
			param->setSection( SECTION );
			param->setKey( TARGET_DATABASE_PARAM_KEY );
			param->setRequired( true );

			kome::plugin::SettingsForm* form = page->addForm();
			form->setParam( param );
			form->setTitle( "Database" );
		}

		if (m_pParentDlg->GetSearchEngine()->getNumberOfTaxonomies() > 0)
		{
			// Taxonomy
			kome::plugin::SettingsValue* val = m_params.addValue();
			val->setType( kome::plugin::SettingsValue::TYPE_STRING );
			val->setEnumType(kome::plugin::SettingsValue::ENUM_FUNCTION);
			val->setEnumKey("taxonomies");

			kome::plugin::SettingsParam* param = param = m_params.addParam();
			param->setName( TARGET_TAXONOMY_PARAM_NAME );
			param->setValue( val );
			param->setSection( SECTION );
			param->setKey( TARGET_TAXONOMY_PARAM_KEY );
			param->setRequired( true );

			kome::plugin::SettingsForm* form = page->addForm();
			form->setParam( param );
			form->setTitle( "Taxonomy" );
		}

		page->setParameters(m_settings);

		// panel
		m_pSettingsPanel2 = new kome::window::SettingsPanel( this, page, false, &m_settings );
		pDBandTaxonomySizer->Add( m_pSettingsPanel2, 0, wxALL | wxGROW, BORDER_SIZE );

		// If the first database has at least 1 database file, add the database files list.
		std::vector< std::string > databaseFiles;
		m_pParentDlg->GetSearchEngine()->getDbFiles(m_pParentDlg->GetSearchEngine()->getDatabase(0).c_str(), databaseFiles);
		if (databaseFiles.size() > 0)
		{
			// panel
			m_databaseFilesListpanel = new kome::window::CheckGridListPanel( this, wxID_ANY, kome::window::CheckGridListPanel::BOTTOM_ALL );

			// list
			m_databaseFilesList = new DatabaseFilesListCtrl( m_databaseFilesListpanel, 200, 100, m_pParentDlg->GetSearchEngine());

			m_databaseFilesList->create();
			pDBandTaxonomySizer->Add( m_databaseFilesListpanel, 1, (wxALL & ~wxTOP) | wxALIGN_TOP | wxGROW, BORDER_SIZE );
		}

		pTopSizer->Add( pDBandTaxonomySizer, 0, wxALL | wxGROW, BORDER_SIZE );
	}

	pVerticalSizer->Add( pTopSizer, 0, wxALL | wxGROW, BORDER_SIZE );

	// Get the settings page for the selected search engine.
	page = m_pParentDlg->GetFuncItem()->getSettingsPage();
	page->setParameters(m_settings);

	// panel
	m_pSettingsPanel3
		= new kome::window::SettingsPanel( this, page, false, &m_settings );
	pVerticalSizer->Add( m_pSettingsPanel3, 1, wxALL | wxGROW, BORDER_SIZE );

	pInnerSizer->Add( pVerticalSizer, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// size event
void SearchPanel::onSize( wxSizeEvent& evt ) {
	// default
	kome::window::StandardPanel::OnSize( evt );

	int cx = 0;
	int cy = 0;

	if (m_databaseFilesList != NULL)
	{
		int nNumPeakListColumns = m_databaseFilesList->GetColumnCount();
		m_databaseFilesList->GetClientSize( &cx, &cy );
		cx -= 27;

		m_databaseFilesList->SetColumnWidth( 0, cx );
	}
}

// Set default panel settings
void SearchPanel::SetDefaultPanelSettings()
{
	// Set all other values, not using SettingsPanel, to their default values.

	if (m_databaseFilesList != NULL)
	{
		// Uncheck all values in the database files checklistbox.
		m_databaseFilesList->deselectAll();
	}

	// Set all values on the SettingsPanel to their default values.
	m_pSettingsPanel1->setDefaultValues();
	if (m_pSettingsPanel2)
	{
		m_pSettingsPanel2->setDefaultValues();
	}
	m_pSettingsPanel3->setDefaultValues();
}

// Validate panel settings 
bool SearchPanel::IsValid()
{
	// Nothing to do
	return true;
}

// Refresh panel contents for matrix
void SearchPanel::RefreshPanelContentsForMatrix()
{
	// Nothing to do for the Search panel, as all settings are global.
	// No settings are saved on a per-matrix basis.
}

// on a setting changing in the settings panel
void SearchPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	TransferDataFromWindow();

	SetEdited();

	if (m_databaseFilesList != NULL)
	{
		// If the database changes, we have to regenerate the Database Files list.
		if (strcmp(evt.getForm()->getTitle(), "Database") == 0)
		{
			m_databaseFilesList->setDbName(m_settings.getParameterValue(TARGET_DATABASE_PARAM_NAME));
		}
	}
}
