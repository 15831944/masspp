/**
 * @file ExportPeaksCOMPIDPanel.cpp
 * @brief implementation of ExportPeaksCOMPIDPanel class
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
#include "ComparativeIdentificationSettingsPanelBase.h"
#include "ExportPeaksCOMPIDPanel.h"
#include "PeakPositionListCtrl.h"
#include "IdentificationOperation.h"

#include <sstream>

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_SAMPLE_LIST_CTRL				1
#define ID_SAMPLE_ADD_BUTTON			2
#define ID_SAMPLE_DELETE_BUTTON			3
#define ID_SAMPLE_UP_BUTTON				4
#define ID_SAMPLE_DOWN_BUTTON			5

#define PEAK_DETECTOR_PARAM_NAME			 "Peak Detector"
#define CHARGE_DETECTOR_PARAM_NAME			 "Charge Detector"
#define RT_TOL_PARAM_NAME					 "RT Tolerance"
#define MZ_TOL_PARAM_NAME					 "m/z Tolerance"

#define PEAK_DETECTOR_PLUGIN_TYPE			 "SPEC_PEAK_PICKING"
#define CHARGE_DETECTOR_PLUGIN_TYPE			 "CHARGE_DETECT"
#define PARENT_PEAK_DETECTOR_PLUGIN_TYPE	 "CHARGE_PEAK_PICKING"

#define SECTION								 "Export Peaks"

#define ID_SAMPLELIST_CHECKLISTBOX	1000
#define ID_PEAKLIST_CHECKLISTBOX	1001

BEGIN_EVENT_TABLE( ExportPeaksCOMPIDPanel, ComparativeIdentificationSettingsPanelBase )
	EVT_SIZE( ExportPeaksCOMPIDPanel::onSize )
	EVT_LIST_ITEM_SELECTED( ID_SAMPLE_LIST_CTRL, ExportPeaksCOMPIDPanel::onSelectSampleItem )
	EVT_LIST_ITEM_DESELECTED( ID_SAMPLE_LIST_CTRL, ExportPeaksCOMPIDPanel::onDeselectSampleItem )
	EVT_BUTTON( ID_SAMPLE_ADD_BUTTON, ExportPeaksCOMPIDPanel::onAddSample )
	EVT_BUTTON( ID_SAMPLE_DELETE_BUTTON, ExportPeaksCOMPIDPanel::onDeleteSample )
	EVT_BUTTON( ID_SAMPLE_UP_BUTTON, ExportPeaksCOMPIDPanel::onUpSample )
	EVT_BUTTON( ID_SAMPLE_DOWN_BUTTON, ExportPeaksCOMPIDPanel::onDownSample )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, ExportPeaksCOMPIDPanel::onSettingChanged)
END_EVENT_TABLE()


ExportPeaksCOMPIDPanel::ExportPeaksCOMPIDPanel( wxWindow* parent, IdentificationDialogBase* pParentDlg )
							: ComparativeIdentificationSettingsPanelBase(parent, pParentDlg, "Export Peaks")
{
	// initialize
	m_pSampleList = NULL;
	m_pSampleAddButton = NULL;
	m_pSampleDeleteButton = NULL;
	m_pSampleUpButton = NULL;
	m_pSampleDownButton = NULL;

	m_peakList = NULL;
	m_peakListpanel = NULL;
	m_pSettingsPanel = NULL;
}

ExportPeaksCOMPIDPanel::~ExportPeaksCOMPIDPanel(void)
{
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool ExportPeaksCOMPIDPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;

	// paths
	if( m_samples.size() == 0 ) {
		errorString << "No samples are specified. Add at least one sample.";		
		bSuccess = false;
	}

	// peaks
	if( m_peakList->getNumberOfData() == 0 ) {
		if (!errorString.str().empty())
		{
			errorString << "\n";
		}

		errorString << "There are no peaks in the matrix. Perform peak detection to obtain some peaks.";
		bSuccess = false;
	}
	else
	{
		// peak list
		std::vector< int > peaks;
		peaks.clear();
		for( unsigned int i = 0; i < m_peakList->getNumberOfData(); i++ ) {

			if (m_peakList->isSelected(m_peakList->getData( i )))
			{
				peaks.push_back( m_peakList->getData( i ) );
			}
		}

		// selected peaks
		if( peaks.size() == 0 ) {
			if (!errorString.str().empty())
			{
				errorString << "\n";
			}

			errorString << "No peaks are selected. Select at least one peak.";
			bSuccess = false;
		}
	}

	sError = errorString.str();

	return bSuccess;
}

// This method is called when prompting for export / import file paths.
bool ExportPeaksCOMPIDPanel::PromptForFilepaths(std::vector<std::string>& asInputFilePaths, std::string& sOutputFilePath)
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
void ExportPeaksCOMPIDPanel::WriteAllChildParameters(kome::objects::SettingParameterValues* settingValues, std::ostringstream& infoString, kome::core::Properties& props)
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
bool ExportPeaksCOMPIDPanel::SavePanelSettings(kome::core::Progress *pProgress, std::vector<std::string>& sInputFilePaths, const std::string &sOutputFilePath)
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

	// peak list
	std::vector< int > peaks;
	peaks.clear();
	for( unsigned int i = 0; i < m_peakList->getNumberOfData(); i++ ) {

		if (m_peakList->isSelected(m_peakList->getData( i )))
		{
			peaks.push_back( m_peakList->getData( i ) );
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

		WriteAllChildParameters(m_peakSettings, infoString, peakDetectParams);
	}

	// RT tolerance
	double m_rtTol = m_settings.getDoubleValue( RT_TOL_PARAM_NAME, 0.5 );

	infoString << "RT Tolerance: " << m_rtTol << "\n";

	// m/z tolerance
	double m_mzTol = m_settings.getDoubleValue( MZ_TOL_PARAM_NAME, 0.5 );

	infoString << "m/z Tolerance: " << m_mzTol << "\n";

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());

	std::string strAdditionalSettings;
	strAdditionalSettings = FMT("-out %s", m_outputFile.c_str());
	strAdditionalSettings += " -samples ";
	for (int iSampleIndex = 0; iSampleIndex < static_cast<int>(m_samples.size()); iSampleIndex ++)
	{
		strAdditionalSettings += m_samples[iSampleIndex];
		if (iSampleIndex != m_samples.size() - 1)
		{
			strAdditionalSettings += ",";
		}
	}
	strAdditionalSettings += " -peaks ";
	for( unsigned int iPeakIndex = 0; iPeakIndex < peaks.size(); iPeakIndex++ )
	{
		strAdditionalSettings += FMT("%d", peaks[iPeakIndex]);
		if (iPeakIndex != peaks.size() - 1)
		{
			strAdditionalSettings += ",";
		}
	}
	IdentificationOperation* opt = CreateIdentificationOperation(strAdditionalSettings.c_str());

	// create peaks list
	MgfManager& mgfMgr = MgfManager::getInstance();

	bSuccess = mgfMgr.writeMisPeaksFromMatrix(
		m_samples,
		peaks,
		m_peakDetector,
		m_peakSettings,
		m_chargeDetector,
		m_chargeSettings,
		m_rtTol,
		m_mzTol,
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
void ExportPeaksCOMPIDPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer)
{
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );

	// target list
	wxStaticBoxSizer* SamplesSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "" ) );

	m_pSampleList = new wxListCtrl(
		this,
		ID_SAMPLE_LIST_CTRL,
		wxDefaultPosition,
		wxSize( 300, 100 ),
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES
	);
	m_pSampleList->InsertColumn( 0, wxT( "Samples" ), wxLIST_FORMAT_LEFT, 250 );

	SamplesSizer->Add( m_pSampleList, 1, (wxALL & ~wxLEFT) | wxGROW, BORDER_SIZE );

	// Add buttons
	wxBoxSizer* pButtonSizer = new wxBoxSizer( wxHORIZONTAL);

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// add button
	m_pSampleAddButton = new wxBitmapButton( this, ID_SAMPLE_ADD_BUTTON, *iconMgr.getIcon("sample_add") );
	m_pSampleAddButton->SetToolTip("Add a Sample to the List");
	m_pSampleAddButton->SetBitmapDisabled(*iconMgr.getIcon("sample_add_disabled"));
	pButtonSizer->Add( m_pSampleAddButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// delete button
	m_pSampleDeleteButton = new wxBitmapButton( this, ID_SAMPLE_DELETE_BUTTON, *iconMgr.getIcon("sample_de") );
	m_pSampleDeleteButton->SetToolTip("Delete the Selected Sample from the List");
	m_pSampleDeleteButton->SetBitmapDisabled(*iconMgr.getIcon("sample_de_disabled"));
	pButtonSizer->Add( m_pSampleDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// up button
	m_pSampleUpButton = new wxBitmapButton( this, ID_SAMPLE_UP_BUTTON, *iconMgr.getIcon("item_up") );
	m_pSampleUpButton->SetToolTip("Move the Selected Sample Up");
	m_pSampleUpButton->SetBitmapDisabled(*iconMgr.getIcon("item_up_disabled"));
	pButtonSizer->Add( m_pSampleUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// down button
	m_pSampleDownButton = new wxBitmapButton( this, ID_SAMPLE_DOWN_BUTTON, *iconMgr.getIcon("item_down") );
	m_pSampleDownButton->SetToolTip("Move the Selected Sample Down");
	m_pSampleDownButton->SetBitmapDisabled(*iconMgr.getIcon("item_down_disabled"));
	pButtonSizer->Add( m_pSampleDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	SamplesSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	pVerticalSizer->Add( SamplesSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	NoSampleItemsSelected();

	// peak list
	wxStaticBoxSizer* peakListSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Peak List" ) );
	// panel
	m_peakListpanel = new kome::window::CheckGridListPanel( this, wxID_ANY, kome::window::CheckGridListPanel::BOTTOM_ALL );
	// list
	m_peakList = new PeakPositionListCtrl( m_peakListpanel, 200, 100 );
	m_peakList->create();
	peakListSizer->Add( m_peakListpanel, 1, wxALL | wxGROW, BORDER_SIZE );

	pVerticalSizer->Add( peakListSizer, 1, wxALL | wxGROW, BORDER_SIZE );

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

	// RT Tolerance
	value = m_params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->setDefaultValue( "1.0" );
	value->getNumberRestriction().setInclusiveMinValue( 0.0 );
	value->getNumberRestriction().setInclusiveMaxValue(10000000000);

	param = m_params.addParam();
	param->setName( RT_TOL_PARAM_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( "RT_TOLERANCE" );
	param->setRequired( true );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Tolerance" );

	// m/z Tolerance
	value = m_params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->setDefaultValue( "0.5" );
	value->getNumberRestriction().setInclusiveMinValue( 0.0 );
	value->getNumberRestriction().setInclusiveMaxValue(10000000000);

	param = m_params.addParam();
	param->setName( MZ_TOL_PARAM_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( "MZ_TOLERANCE" );
	param->setRequired( true );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z Tolerance" );

	// panel
	m_pSettingsPanel
		= new kome::window::SettingsPanel( this, page, false, &m_settings);
    pVerticalSizer->Add(m_pSettingsPanel, 1, wxGROW|wxALL, BORDER_SIZE);

	pInnerSizer->Add( pVerticalSizer, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// size event
void ExportPeaksCOMPIDPanel::onSize( wxSizeEvent& evt ) {
	// default
	kome::window::StandardPanel::OnSize( evt );

	int cx = 0;
	int cy = 0;

	// sample list column size
	if( m_pSampleList != NULL ) {
		// sample list size
		m_pSampleList->GetClientSize( &cx, &cy );
		m_pSampleList->SetColumnWidth( 0, cx );
	}

	if (m_peakList != NULL)
	{
		int nNumPeakListColumns = m_peakList->GetColumnCount();
		m_peakListpanel->GetClientSize( &cx, &cy );
		cx -= 27;

		for (int iColIndex = 0; iColIndex < nNumPeakListColumns; iColIndex++)
		{
			// Resize the columns to fit the available grid width, but if there are more than 6 columns,
			// resize so that 6 columns can be seen at one time, and the scroll bar must be used to view more.
			m_peakList->SetColumnWidth( iColIndex, cx/((nNumPeakListColumns > 6) ? 6 : nNumPeakListColumns));
		}
	}
}

void ExportPeaksCOMPIDPanel::onSelectSampleItem( wxListEvent& evt )
{
	const long lSelectedItem = evt.GetIndex();

	m_pSampleDeleteButton->Enable();
	m_pSampleUpButton->Enable(lSelectedItem > 0);
	m_pSampleDownButton->Enable(lSelectedItem < (m_pSampleList->GetItemCount() - 1));
}

void ExportPeaksCOMPIDPanel::onDeselectSampleItem( wxListEvent& evt )
{
	NoSampleItemsSelected();
}

void ExportPeaksCOMPIDPanel::NoSampleItemsSelected()
{
	m_pSampleDeleteButton->Enable(false);
	m_pSampleUpButton->Enable(false);
	m_pSampleDownButton->Enable(false);
}

// add
void ExportPeaksCOMPIDPanel::onAddSample( wxCommandEvent& evt )
{
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// ini file
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// default directory
	std::string path;
	if( ini != NULL ) {
		const char* p = ini->getString( "File IO", "DEFAULT_DIR", "" );
		path = NVL( p, "" );
	}

	// file dialog
	kome::window::RawDataFileDialog fileDlg( this );

	if( fileDlg.ShowModal() == wxID_OK ) {

		bool bDuplicatePathFound = false;
		bool bDuplicateFilenameFound = false;

		// paths
		std::vector< std::string > paths;
		for( unsigned int iPathIndex = 0; iPathIndex < fileDlg.getNumberOfPaths(); iPathIndex++ ) {
			paths.push_back( fileDlg.getPath( iPathIndex ) );
		}
		std::sort( paths.begin(), paths.end() );

		std::string filename;
		// add paths
		for( unsigned int iPathIndex2 = 0; iPathIndex2 < paths.size(); iPathIndex2++ ) {
			// path
			path = absolutepath( paths[ iPathIndex2 ].c_str() );
			filename = getfilename(path.c_str());

			// add
			if( m_pathSet.find( path ) != m_pathSet.end() )
			{
				// This path has already been added to a category, so it will be ignored.
				bDuplicatePathFound = true;
			}
			else if( m_filenameSet.find( filename ) != m_filenameSet.end() )
			{
				// Although this path has not already been added, the same filename in
				// a different path has been added, so it will be ignored.
				bDuplicateFilenameFound = true;
			}
			else
			{
				m_pathSet.insert( path );
				m_filenameSet.insert( filename );

				m_samples.push_back( path );

				int item = m_pSampleList->GetItemCount();
				m_pSampleList->InsertItem( item, wxT( getfilename(path.c_str()).c_str() ) );

				kome::window::ListTool::unselectAll( m_pSampleList );
				NoSampleItemsSelected();
			}
		}

		if (bDuplicatePathFound && bDuplicateFilenameFound)
		{
			kome::window::WindowTool::showWarning( "Samples already added have been ignored.\n\nAlso, samples with the same filenames as those already added, although located in different folders, have been ignored." );
		}
		else if (bDuplicatePathFound)
		{
			kome::window::WindowTool::showWarning( "Samples already added have been ignored." );
		}
		else if (bDuplicateFilenameFound)
		{
			kome::window::WindowTool::showWarning( "Samples with the same filenames as those already added, although located in different folders, have been ignored." );
		}
	}
}

// delete
void ExportPeaksCOMPIDPanel::onDeleteSample( wxCommandEvent& evt ) {

	// selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pSampleList, items );

	// deselect all
	kome::window::ListTool::unselectAll( m_pSampleList );

	// delete selected items
	for( int i = (int)items.size() - 1; i >= 0; i-- ) {
		// item
		int item = items[ i ];

		// delete
		m_pSampleList->DeleteItem( item );
		m_pathSet.erase( m_samples[ item ] );
		m_filenameSet.erase( getfilename(m_samples[ item ].c_str()) );
		m_samples.erase( m_samples.begin() + item );
	}

	NoSampleItemsSelected();
}

// up
void ExportPeaksCOMPIDPanel::onUpSample( wxCommandEvent& evt )
{
	// selected items
	int num = (int)m_samples.size();
	if( num == 0 ) {
		return;
	}

	bool* selected = new bool[ num ];
	for( int i = 0; i < num; i++ ) {
		selected[ i ] = kome::window::ListTool::isSelected( m_pSampleList, i );
	}
	kome::window::ListTool::unselectAll( m_pSampleList );

	// up
	for( int i = 1; i < num; i++ ) {
		if( selected[ i ] && !selected[ i - 1 ] ) {
			// swap
			selected[ i ] = false;
			selected[ i - 1 ] = true;

			std::string tmp = m_samples[ i ];

			m_samples[ i ] = m_samples[ i - 1 ];
			m_pSampleList->SetItem( i, 0, wxT( getfilename(m_samples[ i - 1 ].c_str()).c_str() ));

			m_samples[ i - 1 ] = tmp;
			m_pSampleList->SetItem( i - 1, 0, wxT( getfilename(tmp.c_str()).c_str() ));
		}
	}

	// select
	for( int i = 0; i < num; i++ ) {
		if( selected[ i ] ) {
			kome::window::ListTool::selectItem( m_pSampleList, i );
		}
	}

	// delete
	delete[] selected;
}

// down
void ExportPeaksCOMPIDPanel::onDownSample( wxCommandEvent& evt )
{
	// selected items
	int num = (int)m_samples.size();
	if( num == 0 ) {
		return;
	}

	bool* selected = new bool[ num ];
	for( int i = 0; i < num; i++ ) {
		selected[ i ] = kome::window::ListTool::isSelected( m_pSampleList, i );
	}
	kome::window::ListTool::unselectAll( m_pSampleList );

	// up
	for( int i = num - 2; i >= 0; i-- ) {
		if( selected[ i ] && !selected[ i + 1 ] ) {
			// swap
			selected[ i ] = false;
			selected[ i + 1 ] = true;

			std::string tmp = m_samples[ i ];

			m_samples[ i ] = m_samples[ i + 1 ];
			m_pSampleList->SetItem( i, 0, wxT( getfilename(m_samples[ i + 1 ].c_str()).c_str() ) );

			m_samples[ i + 1 ] = tmp;
			m_pSampleList->SetItem( i + 1, 0, wxT( getfilename(tmp.c_str()).c_str() ) );
		}
	}

	// select
	for( int i = 0; i < num; i++ ) {
		if( selected[ i ] ) {
			kome::window::ListTool::selectItem( m_pSampleList, i );
		}
	}

	// delete
	delete[] selected;
}

// Set default panel settings
void ExportPeaksCOMPIDPanel::SetDefaultPanelSettings()
{
	// Load samples registered in the matrix
	// (i.e. ignore any additions / deletions done by the user)
	LoadAndUpdateSamples();

	// Check all values in the Peaks checklistbox.
	m_peakList->selectAll();

	// Set all values on the SettingsPanel to their default values.
	m_pSettingsPanel->setDefaultValues();
}

// Validate panel settings 
bool ExportPeaksCOMPIDPanel::IsValid()
{
	return m_pSettingsPanel->Validate();
}

// Update peaks on this panel
void ExportPeaksCOMPIDPanel::updatePeakList()
{
	// Clear all items from the peaks checklistbox.
	m_peakList->DeleteAllItems();
	// Add all peaks to the checklistbox, and check them all by default.
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		m_peakList->addData( m_peaks[ i ], false );
	}
	m_peakList->selectAll();
	m_peakList->updateList();

	m_peakList->Enable( m_peaks.size() != 0 );
}

// Update standard samples on this panel
void ExportPeaksCOMPIDPanel::updateSamples()
{
	// First of all, clear the sample list.
	m_pSampleList->DeleteAllItems();

	for( unsigned int i = 0; i < m_samples.size(); i++ )
	{
		int item = m_pSampleList->GetItemCount();
		m_pSampleList->InsertItem( item, wxT( getfilename(m_samples[i].c_str()).c_str() ) );
	}
}

// Refresh panel contents for matrix
void ExportPeaksCOMPIDPanel::RefreshPanelContentsForMatrix()
{
	LoadAndUpdateSamples();
	LoadAndUpdatePeaks();

	// Nothing extra to do for the Export Peaks panel, as all other settings are global.
	// No other settings are saved on a per-matrix basis.
}

// load and update samples for matrix
void ExportPeaksCOMPIDPanel::LoadAndUpdateSamples()
{
	m_pathSet.clear();
	m_filenameSet.clear();

	// Get the list of samples registered to the active matrix.
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// add default file paths
	std::vector< int > sampleSets;
	m_samples.clear();
	if( GetActiveMatrix() >= 0 ) {
		std::vector< int > groups;

		identMgr.getGroups( groups, GetActiveMatrix() );
		for( unsigned int i = 0; i < groups.size(); i++ ) {
			std::vector< int > samples;
			identMgr.getMatrixSamplesFromGroup( samples, groups[ i ] );

			for( unsigned int j = 0; j < samples.size(); j++ ) {
				int sample = identMgr.getMatrixSampleSample( samples[ j ] );
				std::string filePath = identMgr.getSampleFilePath( sample );

				if( !filePath.empty() ) {
					m_samples.push_back( filePath );

					m_pathSet.insert( filePath );
					m_filenameSet.insert( getfilename(filePath.c_str()) );
				}
			}
		}
	}

	updateSamples();
}

// load and update peaks for matrix
void ExportPeaksCOMPIDPanel::LoadAndUpdatePeaks()
{
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// peaks
	m_peaks.clear();
	if( GetActiveMatrix() >= 0 ) {
		int matrix = GetActiveMatrix();
		identMgr.getMatrixPeaks( m_peaks, matrix );
	}

	updatePeakList();
}

// on a setting changing in the settings panel
void ExportPeaksCOMPIDPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}
