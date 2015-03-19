/**
 * @file ExportPeaksIDPanel.cpp
 * @brief implementation of ExportPeaksIDPanel class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"

#include "IdentificationDialogBase.h"
#include "ComparativeIdentificationDialog.h"
#include "SettingsPanelBase.h"
#include "ExportPeaksIDPanel.h"
#include "SpectrumListCtrl.h"
#include "IdentificationOperation.h"

#include <sstream>

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define PEAK_DETECTOR_PARAM_NAME			 "Peak Detector"
#define CHARGE_DETECTOR_PARAM_NAME			 "Charge Detector"

#define PEAK_DETECTOR_PLUGIN_TYPE			 "SPEC_PEAK_PICKING"
#define CHARGE_DETECTOR_PLUGIN_TYPE			 "CHARGE_DETECT"

#define SECTION								 "Export Peaks"

#define ID_PEAKLIST_CHECKLISTBOX	1001

BEGIN_EVENT_TABLE( ExportPeaksIDPanel, SettingsPanelBase )
	EVT_SIZE( ExportPeaksIDPanel::onSize )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, ExportPeaksIDPanel::onSettingChanged)
END_EVENT_TABLE()


ExportPeaksIDPanel::ExportPeaksIDPanel( wxWindow* parent, IdentificationDialogBase* pParentDlg )
							: SettingsPanelBase(parent, pParentDlg, "Export Peaks")
{
	// initialize
	m_spectrumList = NULL;
	m_spectrumListpanel = NULL;
	m_pSettingsPanel = NULL;
}

ExportPeaksIDPanel::~ExportPeaksIDPanel(void)
{
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool ExportPeaksIDPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;

	// spectra
	if( m_spectrumList->getNumberOfData() == 0 ) {
		if (!errorString.str().empty())
		{
			errorString << "\n";
		}

		errorString << "There are no spectra with MS stage greater than 1. Open a sample containing suitable spectra.";
		bSuccess = false;
	}
	else
	{
		// spectrum list
		std::vector< int > spectra;
		spectra.clear();
		for( unsigned int i = 0; i < m_spectrumList->getNumberOfData(); i++ ) {

			if (m_spectrumList->isSelected(m_spectrumList->getData( i )))
			{
				spectra.push_back( m_spectrumList->getData( i ) );
			}
		}

		// selected spectra
		if( spectra.size() == 0 ) {
			if (!errorString.str().empty())
			{
				errorString << "\n";
			}

			errorString << "No spectra are selected. Select at least one spectrum.";
			bSuccess = false;
		}
	}

	sError = errorString.str();

	return bSuccess;
}

// This method is called when prompting for export / import file paths.
bool ExportPeaksIDPanel::PromptForFilepaths(std::vector<std::string>& asInputFilePaths, std::string& sOutputFilePath)
{
	bool bSuccess = false;

	asInputFilePaths.clear();
	sOutputFilePath = "";

	// For the Export Peaks panel, only sOutputFilePath is required.
	std::vector<std::string> asFilePaths;
	if (GetFilenamesFromDialog(wxT(GetPanelName().c_str() ), wxT( "Mascot Generic Format (*.mgf)|*.mgf" ), false, asFilePaths))
	{
		sOutputFilePath = asFilePaths[0];
		bSuccess = true;
	}

	return bSuccess;
}

// Write all child values to infoString and props, and call recursively if there are sub-parameters.
void ExportPeaksIDPanel::WriteAllChildParameters(kome::objects::SettingParameterValues* settingValues, std::ostringstream& infoString, kome::core::Properties& props)
{
	for (unsigned int iParamIndex = 0; iParamIndex < settingValues->getNumberOfParameters(); iParamIndex++)
	{
		props.setValue(settingValues->getParameterName(iParamIndex), settingValues->getParameterValue(iParamIndex));
		infoString << settingValues->getParameterName(iParamIndex) << ": ";
		infoString << settingValues->getParameterValue(iParamIndex) << "\n";

		if (settingValues->hasSubParameterValues(settingValues->getParameterName(iParamIndex), settingValues->getParameterValue(iParamIndex)))
		{
			WriteAllChildParameters(settingValues->getSubParameterValues(settingValues->getParameterName(iParamIndex), settingValues->getParameterValue(iParamIndex)), infoString, props);
		}
	}
}

// This method is called when saving values.
bool ExportPeaksIDPanel::SavePanelSettings(kome::core::Progress *pProgress, std::vector<std::string>& sInputFilePaths, const std::string &sOutputFilePath)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	if (sOutputFilePath.empty())
	{
		return false;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// spectrum index list
	std::vector< int > spectrumIndices;
	spectrumIndices.clear();
	for( unsigned int i = 0; i < m_spectrumList->getNumberOfData(); i++ ) {

		if (m_spectrumList->isSelected(m_spectrumList->getData( i )))
		{
			spectrumIndices.push_back( m_spectrumList->getData( i ) );
		}
	}

	std::string m_outputFile = sOutputFilePath;

	infoString << GetPanelName().c_str() << " to File: " << m_outputFile << "\n";

	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	// Set the peak detection properties in the result object.
	kome::core::Properties& peakDetectParams = identMgr.getCommonSearchResult()->getPeakDetectionParameters();

	// peak detector
	const char* s = m_settings.getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	peakDetectParams.setValue(PEAK_DETECTOR_PARAM_NAME, s);
	kome::plugin::PluginFunctionItem* m_peakDetector = plgMgr.getFunctionItem( PEAK_DETECTOR_PLUGIN_TYPE, s );

	kome::objects::SettingParameterValues* m_peakSettings = NULL;
	if( m_peakDetector != NULL ) {

		infoString << "Spectrum Peak Detector: ";
		infoString << m_peakDetector->getLongName() << "\n";

		m_peakSettings = m_settings.getSubParameterValues(
			PEAK_DETECTOR_PARAM_NAME,
			m_peakDetector->getShortName()
		);

		WriteAllChildParameters(m_peakSettings, infoString, peakDetectParams);
	}

	s = m_settings.getParameterValue( CHARGE_DETECTOR_PARAM_NAME );
	peakDetectParams.setValue(CHARGE_DETECTOR_PARAM_NAME, s);
	kome::plugin::PluginFunctionItem* m_chargeDetector = plgMgr.getFunctionItem( CHARGE_DETECTOR_PLUGIN_TYPE, s );

	kome::objects::SettingParameterValues* m_chargeSettings = NULL;
	if( m_chargeDetector !=NULL ) {

		infoString << "Charge Detector: ";
		infoString << m_chargeDetector->getLongName() << "\n";

		m_chargeSettings = m_settings.getSubParameterValues(
			CHARGE_DETECTOR_PARAM_NAME,
			m_chargeDetector->getShortName()
		);

		WriteAllChildParameters(m_chargeSettings, infoString, peakDetectParams);
	}

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());

	std::string strAdditionalSettings;
	strAdditionalSettings = FMT("-in %s", m_spectrumList->GetSamplePath());
	strAdditionalSettings += FMT(" -out %s", m_outputFile.c_str());
	strAdditionalSettings += " -spectra ";
	for( unsigned int iSpectrumIndex = 0; iSpectrumIndex < spectrumIndices.size(); iSpectrumIndex++ )
	{
		strAdditionalSettings += FMT("%d", spectrumIndices[iSpectrumIndex]);
		if (iSpectrumIndex != spectrumIndices.size() - 1)
		{
			strAdditionalSettings += ",";
		}
	}
	IdentificationOperation* opt = CreateIdentificationOperation(strAdditionalSettings.c_str());

	// create peaks list
	MgfManager& mgfMgr = MgfManager::getInstance();
	bSuccess = mgfMgr.writeMisPeaksFromSample(
		m_spectrumList->getSpectrumArray(),
		spectrumIndices,
		m_peakDetector,
		m_peakSettings,
		m_chargeDetector,
		m_chargeSettings,
		m_outputFile.c_str(),
		*identMgr.getCommonSearchResult(),
		pProgress
	);

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

	if (!bSuccess && !pProgress->isStopped())
	{
		std::string strError = "An error occurred while exporting peaks:\n";
		strError += getlasterrormessage();
		kome::window::WindowTool::showError( strError.c_str() );
	}

	return bSuccess;
}

// create inner sizer
void ExportPeaksIDPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer)
{
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );

	// panel
	m_spectrumListpanel = new kome::window::CheckGridListPanel( this, wxID_ANY, kome::window::CheckGridListPanel::BOTTOM_ALL );
	// spectrum list
	m_spectrumList = new SpectrumListCtrl( m_spectrumListpanel, 200, 100 );
	m_spectrumList->create();
	pVerticalSizer->Add( m_spectrumListpanel, 1, wxALL | wxGROW, BORDER_SIZE );

	// settings page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );

	// spectrum peak detector
	kome::plugin::SettingsValue* value = m_params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( PEAK_DETECTOR_PLUGIN_TYPE );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( PEAK_DETECTOR_PARAM_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( "PEAK_DETECTOR" );
	param->setRequired( true );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Spectrum Peak Detector" );

	// charge detector
	value = m_params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( CHARGE_DETECTOR_PLUGIN_TYPE );

	param = m_params.addParam();
	param->setName( CHARGE_DETECTOR_PARAM_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( "CHARGE_DETECTOR" );
	param->setRequired( false );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Charge Detector" );

	// panel
	m_pSettingsPanel
		= new kome::window::SettingsPanel( this, page, false, &m_settings);
    pVerticalSizer->Add(m_pSettingsPanel, 1, wxGROW|wxALL, BORDER_SIZE);

	pInnerSizer->Add( pVerticalSizer, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// size event
void ExportPeaksIDPanel::onSize( wxSizeEvent& evt ) {
	// default
	kome::window::StandardPanel::OnSize( evt );

	int cx = 0;
	int cy = 0;

	if (m_spectrumList != NULL)
	{
		int nNumSpectrumListColumns = m_spectrumList->GetColumnCount();
		m_spectrumListpanel->GetClientSize( &cx, &cy );
		cx -= 27;

		for (int iColIndex = 0; iColIndex < nNumSpectrumListColumns; iColIndex++)
		{
			// Resize the columns to fit the available grid width, but if there are more than 6 columns,
			// resize so that 6 columns can be seen at one time, and the scroll bar must be used to view more.
			m_spectrumList->SetColumnWidth( iColIndex, cx/((nNumSpectrumListColumns > 6) ? 6 : nNumSpectrumListColumns));
		}
	}
}

// Set default panel settings
void ExportPeaksIDPanel::SetDefaultPanelSettings()
{
	// Check all values in the Spectra checklistbox.
	m_spectrumList->selectAll();

	// Set all values on the SettingsPanel to their default values.
	m_pSettingsPanel->setDefaultValues();
}

// Validate panel settings 
bool ExportPeaksIDPanel::IsValid()
{
	return m_pSettingsPanel->Validate();
}

// on a setting changing in the settings panel
void ExportPeaksIDPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}
