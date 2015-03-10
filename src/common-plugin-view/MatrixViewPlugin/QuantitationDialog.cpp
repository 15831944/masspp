/**
 * @file QuantitationDialog.cpp
 * @brief implementsation of QuantitationDialog class
 *
 * @author H.Parry
 * @date 2011.04.27
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixViewManager.h"
#include "MatrixListCtrl.h"
#include "QuantitationDialog.h"
#include "CreatePeakMatrixDialog.h"
#include "PeakInfoDialog.h"

#include <wx/generic/propdlg.h>
#include <wx/notebook.h>
#include <wx/tglbtn.h>
#include <wx/event.h>
#include <sstream>


using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_MATRIX_DELETE_BUTTON		      2014
#define ID_EXPORT_BUTTON			      2017
#define ID_CLOSE_BUTTON				      2019
#define ID_PMATRIXSETTINGS_CPANE	      2020
#define ID_MIN_MATRIX_PEAK_BUTTON         2100
#define ID_MAX_MATRIX_PEAK_BUTTON         2200
#define ID_EDIT_BUTTON					  2300

#define NAME_PARAM						"Name"
#define VIEW_STATUS_VALUE				"on"

BEGIN_EVENT_TABLE( QuantitationDialog, kome::window::ParametersAndHelpDialog )
	EVT_SIZE( QuantitationDialog::onSize )
	EVT_BUTTON( ID_MATRIX_DELETE_BUTTON, QuantitationDialog::onDeleteMatrix )
	EVT_BUTTON( ID_EXPORT_BUTTON, QuantitationDialog::onExport )
	EVT_BUTTON( ID_CLOSE_BUTTON, QuantitationDialog::onClose )
	EVT_COMBOBOX ( wxID_ANY, QuantitationDialog::onSelectNewMatrix )
	EVT_CLOSE( QuantitationDialog::onCloseButton )
	EVT_COMMAND_RANGE( ID_MIN_MATRIX_PEAK_BUTTON, ID_MAX_MATRIX_PEAK_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, QuantitationDialog::onPeakButton )
	EVT_UPDATE_UI_RANGE( ID_MIN_MATRIX_PEAK_BUTTON, ID_MAX_MATRIX_PEAK_BUTTON, QuantitationDialog::onPeakUpdateUIButton )
	EVT_BUTTON( ID_EDIT_BUTTON, QuantitationDialog::onEditMatrix )
END_EVENT_TABLE()

// constructor
QuantitationDialog::QuantitationDialog(	wxWindow* pParent)
	: kome::window::ParametersAndHelpDialog(	pParent,"Quantitation")
{
    // manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

    // initialize
	m_editButton = NULL;
	m_pMatrixDeleteButton = NULL;
	m_pMatrixCombo = NULL;
	m_pExportButton = NULL;
	m_pCloseButton = NULL;
	m_nProject = 0;
	m_peakId = -1;
	m_yPos = 0;

	// create progress
	kome::window::DialogProgress* progress = new kome::window::DialogProgress( kome::window::WindowTool::getMainWindow(), "Create Peak List" );
	m_pMatrixList = new MatrixListCtrl(this, 500, 150, progress);

	// peak actions
	m_peakActions.clear();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int num = plgMgr.getNumberOfFunctionItems( "ON_MATRIX_PEAK" );
	if( num > 0 ) {
		m_peakActions.resize( num );
	}

	for( unsigned int i = 0 ; i < num; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "ON_MATRIX_PEAK", i );
		m_peakActions[ i ].fun = item;
		m_peakActions[ i ].button = NULL;
		m_peakActions[ i ].dialog = NULL;
		m_peakActions[ i ].pos = -1;

		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		if( icon != NULL ) {
			iconMgr.loadBitmap( &( m_peakActions[ i ].icon0 ), icon );
		}

		wxBitmap bitmap( 16, 16 );
		wxMemoryDC memDC( bitmap );
		memDC.SetBackground( *wxGREY_BRUSH );
		memDC.Clear();
		memDC.DrawBitmap( m_peakActions[ i ].icon0, 1, 0 );

		memDC.SelectObject( wxNullBitmap );

		m_peakActions[ i ].icon1.CopyFromBitmap( bitmap );
	}
}

// destructor
QuantitationDialog::~QuantitationDialog() {
	if (m_pMatrixList != NULL)
	{
		delete m_pMatrixList;
		m_pMatrixList = NULL;
	}		
}

// get matrix list control
MatrixListCtrl* QuantitationDialog::getMatrixListCtrl() {
	return m_pMatrixList;
}

// get the status name
std::string QuantitationDialog::getStatusName( const unsigned int idx ) {
	// string
	std::string s;

	// check the index
	if( idx >= m_peakActions.size() ) {
		return s;
	}

	// name
	s = FMT( "matrix_peak_%s", m_peakActions[ idx ].fun->getShortName() );

	return s;
}

// judges the specified dialog is visible or not
bool QuantitationDialog::isVisibleDialog( const unsigned int idx ) {
	// check the parameters
	if( idx >= m_peakActions.size() ) {
		return false;
	}

	if( m_peakActions[ idx ].dialog == NULL ) {
		return false;
	}

	return m_peakActions[ idx ].dialog->IsShown();
}

// open new dialog
void QuantitationDialog::openNewDialog( const unsigned int idx ) {
	// delete old window
	if( m_peakActions[ idx ].dialog != NULL ) {
		delete m_peakActions[ idx ].dialog;
		m_peakActions[ idx ].dialog = NULL;
	}

	if( m_peakId < 0 ) {
		return;
	}

	// dialog
	PeakInfoDialog* dlg = new PeakInfoDialog( this, m_peakActions[ idx ].fun, m_peakId );

	int pos = m_peakActions[ idx ].pos;
	bool updated = false;
	if( pos < 0 ) {
		pos = m_yPos;
		m_peakActions[ idx ].pos = pos;
		updated = true;
	}

	dlg->SetPosition( wxPoint( -1, pos ) );
	if( updated ) {
		m_yPos += dlg->GetSize().GetHeight();
	}

	m_peakActions[ idx ].dialog = dlg;

	dlg->Show( true );
}


// set peak ID
void QuantitationDialog::setPeakId( const int peakId ) {	
	// TEST用 時間計測
	clock_t  start, end;

    start = clock();
	LOG_DEBUG( FMT("★Start Time : %f", (double)start ) );

	m_peakId = peakId;

	for( unsigned int i = 0; i < m_peakActions.size(); i++ ) {
		if( isVisibleDialog( i ) ) {
			m_peakActions[ i ].dialog->setPeakId( peakId );
		}
	}
	end = clock();
	double margin = (double)(end - start) / CLOCKS_PER_SEC;
	
	LOG_DEBUG( FMT("★End Time : %f", (double)end ) );
	LOG_DEBUG( FMT("★Processing time : %f", margin));
}

// open peak
void QuantitationDialog::openPeak( const int peakId ) {
	
	// peak ID
	m_peakId = peakId;

	// manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// open
	for( unsigned int i = 0; i < m_peakActions.size(); i++ ) {
		if( isVisibleDialog( i ) ) {
			m_peakActions[ i ].dialog->setPeakId( peakId );
		}
		else {
			std::string statusName = getStatusName( i );
			const char* s0 = statusMgr.getValue( statusName.c_str() );
			std::string s1 = NVL( s0, "on" );
			if( s1.compare( "on" ) == 0 ) {
				openNewDialog( i );
			}
		}
	}	
}

// get project
int QuantitationDialog::getProject() {
	return m_nProject;
}

void QuantitationDialog::RecreateMatrixList()
{
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	m_nProject = getSelectedProject();

	if( m_nProject >= 0 ) {
		identMgr.selectMatrix( m_nProject );
	}

	if (m_pMatrixList != NULL)
	{
		m_pMatrixList->DeleteAllItems();
		m_pMatrixList->create();
		// After the matrix has been recreated, have to enable / disable the Export button accordingly.
		m_pExportButton->Enable(!MatrixListIsEmpty());

		Refresh();
	}
}

// create top sizer
wxSizer* QuantitationDialog::createMainSizer() {
	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// selected project
	m_nProject = identMgr.getSelectedMatrix();

	// field sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* MatrixOpsSizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Selected Matrix, Labelとコンボボックス
	wxStaticText* pSelectMatrixStaticText = new wxStaticText(this, wxID_ANY, "&Selected Matrix:");
	MatrixOpsSizer->Add(pSelectMatrixStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	m_pMatrixCombo = new wxComboBox(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize( 200, -1 ), 0, NULL, wxCB_SIMPLE | wxCB_READONLY);
	MatrixOpsSizer->Add(m_pMatrixCombo, 1, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	setDescription( m_pMatrixCombo, "Select the peak matrix." );

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	// [×]ボタン
	m_pMatrixDeleteButton = new wxBitmapButton(this, ID_MATRIX_DELETE_BUTTON, *iconMgr.getIcon("peak_mtrx_del") );
	m_pMatrixDeleteButton->SetToolTip("Delete the selected peak matrix.");
	m_pMatrixDeleteButton->SetBitmapDisabled(*iconMgr.getIcon("peak_mtrx_del_disabled"));
	buttonSizer->Add(m_pMatrixDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	MatrixOpsSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	mainSizer->Add(MatrixOpsSizer, 0, wxEXPAND | wxALL, 5);

	// MatrixGroupPlotPlugin, MatrixOverlappingViewPlugin 等のボタンを表示
	wxStaticBoxSizer* GroupOpsSizer = new wxStaticBoxSizer(wxVERTICAL, this);
	wxBoxSizer* GroupsOpsButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	for( unsigned int i = 0; i < m_peakActions.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_peakActions[ i ].fun;
		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		std::string tooltip = FMT( "Toggle %s on / off", item->getLongName() );

		wxBitmapButton* button = new wxBitmapButton(
			this,
			ID_MIN_MATRIX_PEAK_BUTTON + i,
			m_peakActions[ i ].icon0		 
		);
		m_peakActions[ i ].button = button;
		button->SetToolTip( wxT( tooltip.c_str() ) );

		std::string sName = getStatusName( i );
		const char* s0 = statusMgr.getValue( sName.c_str() );
		std::string s1 = NVL( s0, "" );

		if( s1.compare( "on" ) == 0 ) {
			button->SetBitmapLabel( m_peakActions[ i ].icon1 );
		}
		else {
			button->SetBitmapLabel( m_peakActions[ i ].icon0 );
		}
		
		GroupsOpsButtonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );
	}

	GroupOpsSizer->Add(GroupsOpsButtonSizer, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// peak matrix
	if( m_pMatrixList != NULL ) {
		m_pMatrixList->create();
	}
	GroupOpsSizer->Add(m_pMatrixList, 1, wxEXPAND);

	mainSizer->Add(GroupOpsSizer, 1, wxEXPAND);

	m_pExportButton = new wxBitmapButton(this, ID_EXPORT_BUTTON, *iconMgr.getIcon("peak_mtrx_export") );
	m_pExportButton->SetToolTip("Export the selected peak matrix as tab separated values file. (.txt).");
	m_pExportButton->SetBitmapDisabled(*iconMgr.getIcon("peak_mtrx_export_disabled"));
	// Enable / disable the Export button accordingly.
	m_pExportButton->Enable(!MatrixListIsEmpty());
	mainSizer->Add(m_pExportButton, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	updateMatrixCombo();

	return mainSizer;
}

// size event
void QuantitationDialog::onSize( wxSizeEvent& evt ) {
	// default
	kome::window::StandardDialog::OnSize( evt );

	int cx = 0;
	int cy = 0;

	// matrix list column size
	if( m_pMatrixList != NULL ) {
		// matrix list size
		m_pMatrixList->GetClientSize( &cx, &cy );

		int nNumMatrixListColumns = m_pMatrixList->GetColumnCount();

		for (int iColIndex = 0; iColIndex < nNumMatrixListColumns; iColIndex++)
		{
			// Resize the columns to fit the available grid width, but if there are more than 6 columns,
			// resize so that 6 columns can be seen at one time, and the scroll bar must be used to view more.
			m_pMatrixList->SetColumnWidth( iColIndex, cx/((nNumMatrixListColumns > 6) ? 6 : nNumMatrixListColumns));
		}
	}
}

// create button sizer
wxSizer* QuantitationDialog::createButtonSizer() {
	// Override this function so that OK and Cancel buttons are not included.
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	// Edit Button
	m_editButton = new wxButton( this, ID_EDIT_BUTTON, "Edit..." );
	m_editButton->Enable( m_projects.size() > 0 );
	buttonSizer->Add(m_editButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5); 

	m_pCloseButton = new wxButton(this, ID_CLOSE_BUTTON, "&Close");
	buttonSizer->Add(m_pCloseButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	sizer->Add( buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
	
	return sizer;
}

void QuantitationDialog::setnProject(){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	m_nProject = identMgr.getSelectedMatrix();
}

// update combo
void QuantitationDialog::updateMatrixCombo() {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get projects
	m_projects.clear();
	identMgr.getMatrices( m_projects );

	// update combobox
	m_pMatrixCombo->Clear();

	int sel = 0;
	for( unsigned int i = 0; i < m_projects.size(); i++ ) {
		int id = m_projects[ i ];
		m_pMatrixCombo->Append( wxT( identMgr.getMatrixName( id ).c_str() ), &( m_projects[ i ] ) );
		if( id == m_nProject ) {
			sel = (int)i;
		}
	}
	m_pMatrixCombo->Select( sel );

	m_pMatrixCombo->Enable( m_projects.size() > 0 );

	m_pMatrixDeleteButton->Enable( m_projects.size() > 0 );
	if( m_editButton != NULL ){
		m_editButton->Enable( m_projects.size() > 0 );
	}
	
}

// get combo index from project ID
int QuantitationDialog::ComboIndexFromProjectID(int iProjectID)
{
	int sel = -1;
	for( unsigned int i = 0; (i < m_projects.size()) && (sel == -1); i++ )
	{
		int id = m_projects[ i ];
		if( id == m_nProject ) {
			sel = (int)i;
		}
	}

	return sel;
}

// get selected project
int QuantitationDialog::getSelectedProject() {
	// selected item
	int sel = m_pMatrixCombo->GetSelection();

	// get project
	int project = -1;
	if( sel >= 0 && sel < (int)m_pMatrixCombo->GetCount() ) {
		project = *( (int*)m_pMatrixCombo->GetClientData( sel ) );
	}

	return project;
}

// on edit
void QuantitationDialog::onEditMatrix( wxCommandEvent& evt ) {
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	
	mgr.openPeakMatrixDialog( true );
	CreatePeakMatrixDialog* dlg = mgr.getPeakMatrixDlg();
}

// delete
void QuantitationDialog::onDeleteMatrix( wxCommandEvent& evt )
{
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	m_nProject = getSelectedProject();

	// check
	if( m_nProject < 0 ) {
		kome::window::WindowTool::showInfo( "There is no active matrix. Select a matrix." );
		return;
	}
	else {
		if( !kome::window::WindowTool::showYesNo( "Are you sure you want to delete the selected matrix?", "Delete", "Don't delete" ) ) {
			return;
		}
	}

	// delete
	identMgr.deleteMatrix( m_nProject );

	// update combo
	updateMatrixCombo();

	// Refresh the matrix list
	RecreateMatrixList();

	// Commit the changes
	int project = getProject();
	if( project >= 0 ) {
		identMgr.selectMatrix( project );
	}
}

// checks if a matrix with the specified name exists
bool QuantitationDialog::MatrixNameExists( const char* name )
{
	bool bMatrixNameExists = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get projects
	std::vector< int > projects;
	identMgr.getMatrices( projects );

	for( unsigned int i = 0; (i < projects.size()) && !bMatrixNameExists; i++ ) {

		int id = projects[ i ];
		if (!strcmp(name, identMgr.getMatrixName( id ).c_str()))
		{
			bMatrixNameExists = true;
		}
	}

	return bMatrixNameExists;
}

// select new matrix
void QuantitationDialog::onSelectNewMatrix( wxCommandEvent& evt )
{
	// Select the new item!
	m_nProject = getSelectedProject();
	RecreateMatrixList();
	// Set the combo back to the previous selection.
	m_pMatrixCombo->SetSelection(ComboIndexFromProjectID(m_nProject));
}


// on export
void QuantitationDialog::onExport( wxCommandEvent& evt ) {
	if (m_pMatrixList)
	{
		m_pMatrixList->exportData();
	}
}

// on close
void QuantitationDialog::onClose( wxCommandEvent& evt )
{
	onCancelButton(evt);
	
}

// on (system) close button
void QuantitationDialog::onCloseButton( wxCloseEvent& evt )
{
	wxCommandEvent dummy;
	onCancelButton(dummy);
}

// on peak button
void QuantitationDialog::onPeakButton( wxCommandEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_MATRIX_PEAK_BUTTON;
	if( idx < 0 || idx >= (int)m_peakActions.size() ) {
		return;
	}

	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// button
	wxBitmapButton* button = m_peakActions[ idx ].button;

	// status
	std::string name = getStatusName( idx );
	if( name.empty() ) {
		return;
	}

	const char* s = statusMgr.getValue( name.c_str() );
	std::string status = NVL( s, "" );

	// open / close
	if( status.compare( "on" ) == 0 ) {    // close
		statusMgr.setValue( name.c_str(), "" );
		button->SetBitmapLabel( m_peakActions[ idx ].icon0 );

		if( isVisibleDialog( idx ) ) {
			delete m_peakActions[ idx ].dialog;
			m_peakActions[ idx ].dialog = NULL;
		}
	}
	else {    // open
		statusMgr.setValue( name.c_str(), "on" );
		button->SetBitmapLabel( m_peakActions[ idx ].icon1 );

		openNewDialog( idx );
	}
}

// on update peak button
void QuantitationDialog::onPeakUpdateUIButton( wxUpdateUIEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_MATRIX_PEAK_BUTTON;
	if( idx < 0 || idx >= (int)m_peakActions.size() ) {
		return;
	}

	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// status
	std::string name = getStatusName( idx );
	if( name.empty() ) {
		return;
	}

	const char* s = statusMgr.getValue( name.c_str() );
	std::string status = NVL( s, "" );

	evt.Check( status.compare( "on" ) == 0 );
}

// transfer data from window
bool QuantitationDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::ParametersAndHelpDialog::TransferDataFromWindow() ) {
		return false;
	}

	// project
	m_nProject = getSelectedProject();

    return true;
}

// transfer data to window
bool QuantitationDialog::TransferDataToWindow() {

	return true;
}

// indicates whether there are any peaks in the matrix list
bool QuantitationDialog::MatrixListIsEmpty()
{
	return (m_pMatrixList->getNumberOfData() == 0);
}

// close peak info dialog
void QuantitationDialog::closePeakInfoDlg(){
	for( unsigned int i = 0; i < m_peakActions.size(); i++ ) {
		if( isVisibleDialog( i ) ) {
			delete m_peakActions[ i ].dialog;
			m_peakActions[ i ].dialog = NULL;
		}
	}
}
