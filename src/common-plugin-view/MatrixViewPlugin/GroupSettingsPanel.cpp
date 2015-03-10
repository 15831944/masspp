/**
 * @file GroupSettingsPanel.cpp
 * @brief implementation of GroupSettingsPanel class
 *
  * @author H.Parry
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "GroupSettingsPanel.h"
#include "PeakMatrixManager.h"
#include "MatrixViewManager.h"
#include "GroupSettingsGridCtrl.h"

#include "matrix_flow02.xpm"

#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						3
#define BORDER_SIZE						3

#define ID_CATEGORY_LIST_CTRL			128
#define ID_CATEGORY_NEW					129
#define ID_CATEGORY_DELETE				130
#define ID_CATEGORY_EDIT				131
#define ID_CATEGORY_UP					132
#define ID_CATEGORY_DOWN				133

#define NAME_PARAM						"Name"
#define COLOR_PARAM						"Color"

#define ID_SAMPLE_LIST_CTRL				1
#define ID_SAMPLE_ADD_BUTTON			2
#define ID_SAMPLE_DELETE_BUTTON			3
#define ID_SAMPLE_UP_BUTTON				4
#define ID_SAMPLE_DOWN_BUTTON			5
#define ID_SAMPLE_COLOR_BUTTON			7

#define ID_STANDARDS_COMBO				512

#define DEFAULT_COLOR					kome::img::ColorTool::BLUE

#define GPP_STANDARD_SAMPLE_PARAM_KEY	"GET_PEAK_POSITIONS_STANDARD_SAMPLE" 


BEGIN_EVENT_TABLE( GroupSettingsPanel, kome::window::PeakMatrixSettingsPageBase )

	EVT_SIZE( GroupSettingsPanel::onSize )
	
	EVT_LIST_ITEM_ACTIVATED( ID_CATEGORY_LIST_CTRL, GroupSettingsPanel::onCategoryDoubleClicked )
	EVT_LIST_ITEM_SELECTED( ID_CATEGORY_LIST_CTRL, GroupSettingsPanel::onSelectCategoryItem )
	EVT_LIST_ITEM_DESELECTED( ID_CATEGORY_LIST_CTRL, GroupSettingsPanel::onDeselectCategoryItem )
	EVT_BUTTON( ID_CATEGORY_NEW, GroupSettingsPanel::onNewCategory )
	EVT_BUTTON( ID_CATEGORY_DELETE, GroupSettingsPanel::onDeleteCategory )
	EVT_BUTTON( ID_CATEGORY_EDIT, GroupSettingsPanel::onEditCategory )
	EVT_BUTTON( ID_CATEGORY_UP, GroupSettingsPanel::onUpCategory )
	EVT_BUTTON( ID_CATEGORY_DOWN, GroupSettingsPanel::onDownCategory )

	EVT_LIST_ITEM_SELECTED( ID_SAMPLE_LIST_CTRL, GroupSettingsPanel::onSelectSampleItem )
	EVT_LIST_ITEM_DESELECTED( ID_SAMPLE_LIST_CTRL, GroupSettingsPanel::onDeselectSampleItem )
	EVT_BUTTON( ID_SAMPLE_ADD_BUTTON, GroupSettingsPanel::onAddSample )
	EVT_BUTTON( ID_SAMPLE_DELETE_BUTTON, GroupSettingsPanel::onDeleteSample )
	EVT_BUTTON( ID_SAMPLE_UP_BUTTON, GroupSettingsPanel::onUpSample )
	EVT_BUTTON( ID_SAMPLE_DOWN_BUTTON, GroupSettingsPanel::onDownSample )
	EVT_BUTTON( ID_SAMPLE_COLOR_BUTTON, GroupSettingsPanel::onColorSample )
	EVT_COMBOBOX( ID_STANDARDS_COMBO, GroupSettingsPanel::onSelectStandard )
END_EVENT_TABLE()

// constructor
GroupSettingsPanel::GroupSettingsPanel( wxWizard* parent )
		: kome::window::PeakMatrixSettingsPageBase( parent, "Group Settings" )
{
	// initialize
	m_pCategoryList = NULL;
	m_pCategoryNewButton = NULL;
	m_pCategoryEditButton = NULL;
	m_pCategoryDeleteButton = NULL;
	m_pCategoryUpButton = NULL;
	m_pCategoryDownButton = NULL;
	
	m_pSampleList = NULL;
	m_pSampleAddButton = NULL;
	m_pSampleDeleteButton = NULL;
	m_pSampleUpButton = NULL;
	m_pSampleDownButton = NULL;
	m_pSampleColorButton = NULL;

	m_stdCombo = NULL;

	m_categoryID = -1;

	m_bGroupSampleAdditionDeletion = false;
	m_color = kome::img::ColorTool::PURPLE;

	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( parent );

	createControls();
}

// destructor
GroupSettingsPanel::~GroupSettingsPanel(void)
{
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool GroupSettingsPanel::PreApplyValidationCheck(std::string& sError)
{
	// No Apply-Time validation is necessary for the Group Settings panel, so always return true.

	return true;
}

// update list
void GroupSettingsPanel::updateCategoryList() {

	// update list
	m_pCategoryList->clearData(true);
	for( unsigned int iCategoryIndex = 0; iCategoryIndex < m_categoryArray.size(); iCategoryIndex++ ) {
		m_pCategoryList->addData( iCategoryIndex, false );
		m_pCategoryList->setDataColor( iCategoryIndex, m_categoryArray[iCategoryIndex].color );
	}
	
	// update
	m_pCategoryList->updateList();
}

// get selected category CategoryInfo*
CategoryInfo* GroupSettingsPanel::getSelectedCategoryInfo()
{
	CategoryInfo* pCatInfo = NULL;

	int iIndex = getSelectedCategoryArrayIndex();

	if (iIndex >= 0 && !m_categoryArray.empty() )
	{
		pCatInfo = &m_categoryArray[iIndex];
	}

	return pCatInfo;
}

// get selected category array index
int GroupSettingsPanel::getSelectedCategoryArrayIndex() {
	// get selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pCategoryList, items );
	if( items.size() == 0 ) {
		return -1;
	}
	int item = items[ 0 ];

	return item;
}

// get selected category
int GroupSettingsPanel::getSelectedCategoryID()
{
	int nSelectedCatID = -1;

	// category ID
	if (getSelectedCategoryArrayIndex() >= 0)
	{
		nSelectedCatID = m_pCategoryList->GetItemData( getSelectedCategoryArrayIndex() );
	}

	return nSelectedCatID;
}

// get temporary category ID
int GroupSettingsPanel::getTempCategoryID()
{
	int nHighestCategoryID = -1;

	for( unsigned int i = 0; i < m_categoryArray.size(); i++ )
	{
		if (m_categoryArray[i].id > nHighestCategoryID)
		{
			nHighestCategoryID = m_categoryArray[i].id;
		}
	}

	// 15000 is an arbitary starting value for temporary category IDs
	return (nHighestCategoryID >= 15000) ? (nHighestCategoryID + 1) : 15000;
}

// checks if a category with the specified name exists
bool GroupSettingsPanel::CategoryNameExists( const char* name )
{
	bool bCategoryNameExists = false;

	for( unsigned int iCategoryIndex = 0; (iCategoryIndex < m_categoryArray.size()) && !bCategoryNameExists; iCategoryIndex++ )
	{
		if (!strcmp(name, m_categoryArray[iCategoryIndex].name.c_str()))
		{
			bCategoryNameExists = true;
		}
	}

	return bCategoryNameExists;
}

// selects the category item with the specified name
void GroupSettingsPanel::SelectCategoryItem( const char* name )
{
	wxString itemText;

	for (int i = 0; i < m_pCategoryList->GetItemCount(); i++)
	{
		itemText = m_pCategoryList->GetItemText(i);
		if (!itemText.compare(name))
		{
			kome::window::ListTool::selectItem( m_pCategoryList, i );
			break;
		}
	}
}

// selects the sample item with the specified name
void GroupSettingsPanel::SelectSampleItem( const char* name )
{
	wxString itemText;

	for (int i = 0; i < m_pSampleList->GetItemCount(); i++)
	{
		itemText = m_pSampleList->GetItemText(i);
		if (!itemText.compare(name))
		{
			kome::window::ListTool::selectItem( m_pSampleList, i );
			break;
		}
	}
}

// open edit dialog
bool GroupSettingsPanel::openEditDialog( const bool editMode ) {

	bool bEdited = false;

	// selected category
	int category = getSelectedCategoryID();
	if( editMode && category < 0 ) {
		kome::window::WindowTool::showInfo( "No group is selected. Select a group." );
		return bEdited;
	}

	CategoryInfo *pCurrentCategory;
	pCurrentCategory = getSelectedCategoryInfo();

	// name & color
	std::string name;	
	setColor();
	
	if( editMode ) {
		name = pCurrentCategory->name;
		m_color = pCurrentCategory->color;
	}

	// page
	kome::plugin::ParameterSettings params( NULL, true );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );

	// name
	kome::plugin::SettingsValue* value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	if( editMode ){
		value->setDefaultValue( name.c_str() );
	}else{
		value->setDefaultValue( " " );
	}
	value->setRequired( true );
	
	kome::plugin::SettingsParam* param = params.addParam();
	param->setName( NAME_PARAM );
	param->setValue( value );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Name" );

	if( editMode )
	{
		page->setTitle("Edit Group Name");
	}
	else
	{		
		page->setTitle("New Group");
	}

	// color
	value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_COLOR );
	value->setDefaultValue( kome::img::ColorTool::getString( m_color ).c_str() );
	value->setRequired( true );

	param = params.addParam();
	param->setName( COLOR_PARAM );
	param->setValue( value );
	
	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Color" );

	// panel
	kome::objects::SettingParameterValues settings;
	kome::window::SettingsPageDialog dlg( this, page, &settings, false, NULL, true, editMode ? "Change" : "Add" );
	
	if( dlg.ShowModal() == wxID_OK ) {
		// name
		const char* n = settings.getParameterValue( NAME_PARAM );
		n = NVL( n, name.c_str() );

		// Check that this name is unique.
		// In the cases:
		// Creating a new group.
		// Editing a group and the name has been edited. (Must not check the name if it hasn't changed)
		if ( ( (!editMode) || (editMode && pCurrentCategory->name.compare(n) != 0) ) && (CategoryNameExists(n)) )
		{
			kome::window::WindowTool::showInfo( "A group with that name already exists." );
		}
		else
		{
			// color
			const char* c = settings.getParameterValue( COLOR_PARAM );
			COLOR cl = kome::img::ColorTool::getColor( c );

			// order
			int order = -1;
			std::vector< int > items;
			kome::window::ListTool::getSelectedItems( m_pCategoryList, items );
			if( items.size() > 0 ) {
				order = items[ 0 ];
			}

			// update
			if( editMode ) {
				pCurrentCategory->name = n;
				pCurrentCategory->color = cl;

				// Update all its samples
				SampleInfo* pCurrentSample;

				for( unsigned int iSampleIndex = 0; iSampleIndex < pCurrentCategory->paths.size(); iSampleIndex++ )
				{
					pCurrentSample = &pCurrentCategory->paths[iSampleIndex];

					// Must update the category name with the new name.
					pCurrentSample->categoryName = n;
				}
			}
			else {
				m_categoryArray.resize( m_categoryArray.size() + 1 );

				m_categoryArray.back().name = n;
				m_categoryArray.back().id = getTempCategoryID();
				m_categoryArray.back().color = cl;
			}

			// update list
			updateCategoryList();

			NoCategoryItemsSelected();

			SelectCategoryItem(n);

			bEdited = true;
		}
	}

	return bEdited;
}

// create inner sizer
void GroupSettingsPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer) {
	wxBitmap addBmp( matrix_flow02_xpm );
	
	wxBoxSizer* bmtSizer = new wxBoxSizer( wxVERTICAL );
	bmtSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* horiznSizer = new wxBoxSizer( wxHORIZONTAL);
	horiznSizer->Add( bmtSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	pInnerSizer->Add( horiznSizer );

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);

	wxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Register groups and their samples.\n"
		"Group and sample color affects to viewers such as waveforms and plots.")) );
	sizer->Add( textSizer, 0, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	wxBoxSizer* pCategoriesSizer = new wxBoxSizer( wxVERTICAL );
	// list box
	m_pCategoryList = new GroupsGridCtrl( this );
	
	m_pCategoryList->InsertColumn( 0, wxT( "Groups" ) );
	m_pCategoryList->SetColumnWidth( 0, 100 );

	pCategoriesSizer->Add( m_pCategoryList, 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	// Add buttons
	wxBoxSizer* pButtonSizer = new wxBoxSizer( wxHORIZONTAL);

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// new button
	m_pCategoryNewButton = new wxBitmapButton( this, ID_CATEGORY_NEW, *iconMgr.getIcon("item_add") );
	m_pCategoryNewButton->SetToolTip("Add new group");
	pButtonSizer->Add( m_pCategoryNewButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// edit button
	m_pCategoryEditButton = new wxBitmapButton( this, ID_CATEGORY_EDIT, *iconMgr.getIcon("item_edit") );
	m_pCategoryEditButton->SetToolTip("Edit selected group");
	m_pCategoryEditButton->SetBitmapDisabled(*iconMgr.getIcon("item_edit_disabled"));
	pButtonSizer->Add( m_pCategoryEditButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// delete button
	m_pCategoryDeleteButton = new wxBitmapButton( this, ID_CATEGORY_DELETE, *iconMgr.getIcon("item_del") );
	m_pCategoryDeleteButton->SetToolTip("Delete selected group");
	m_pCategoryDeleteButton->SetBitmapDisabled(*iconMgr.getIcon("item_del_disabled"));
	pButtonSizer->Add( m_pCategoryDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// up button
	m_pCategoryUpButton = new wxBitmapButton( this, ID_CATEGORY_UP, *iconMgr.getIcon("item_up") );
	m_pCategoryUpButton->SetToolTip("Move selected group up");
	m_pCategoryUpButton->SetBitmapDisabled(*iconMgr.getIcon("item_up_disabled"));
	pButtonSizer->Add( m_pCategoryUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// down button
	m_pCategoryDownButton = new wxBitmapButton( this, ID_CATEGORY_DOWN, *iconMgr.getIcon("item_down") );
	m_pCategoryDownButton->SetToolTip("Move selected group down");
	m_pCategoryDownButton->SetBitmapDisabled(*iconMgr.getIcon("item_down_disabled"));
	pButtonSizer->Add( m_pCategoryDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// default
	pCategoriesSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	gHoriznSizer->Add( pCategoriesSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	// Sample 
	wxBoxSizer* SamplesSizer = new wxBoxSizer( wxVERTICAL );

	// list
	m_pSampleList = new SamplesGridCtrl( this );
	
	m_pSampleList->InsertColumn( 0, wxT( "Samples" ), wxLIST_FORMAT_LEFT, 250 );

	SamplesSizer->Add( m_pSampleList, 1, (wxALL & ~wxLEFT) | wxGROW, BORDER_SIZE );

	// Add buttons
	pButtonSizer = new wxBoxSizer( wxHORIZONTAL);

	// add button
	m_pSampleAddButton = new wxBitmapButton( this, ID_SAMPLE_ADD_BUTTON, *iconMgr.getIcon("sample_add") );
	m_pSampleAddButton->SetToolTip("Add sample");
	m_pSampleAddButton->SetBitmapDisabled(*iconMgr.getIcon("sample_add_disabled"));
	pButtonSizer->Add( m_pSampleAddButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// delete button
	m_pSampleDeleteButton = new wxBitmapButton( this, ID_SAMPLE_DELETE_BUTTON, *iconMgr.getIcon("sample_de") );
	m_pSampleDeleteButton->SetToolTip("Delete selected sample");
	m_pSampleDeleteButton->SetBitmapDisabled(*iconMgr.getIcon("sample_de_disabled"));
	pButtonSizer->Add( m_pSampleDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// up button
	m_pSampleUpButton = new wxBitmapButton( this, ID_SAMPLE_UP_BUTTON, *iconMgr.getIcon("item_up") );
	m_pSampleUpButton->SetToolTip("Move selected sample up");
	m_pSampleUpButton->SetBitmapDisabled(*iconMgr.getIcon("item_up_disabled"));
	pButtonSizer->Add( m_pSampleUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// down button
	m_pSampleDownButton = new wxBitmapButton( this, ID_SAMPLE_DOWN_BUTTON, *iconMgr.getIcon("item_down") );
	m_pSampleDownButton->SetToolTip("Move selected sample down");
	m_pSampleDownButton->SetBitmapDisabled(*iconMgr.getIcon("item_down_disabled"));
	pButtonSizer->Add( m_pSampleDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// color button
	m_pSampleColorButton = new wxBitmapButton( this, ID_SAMPLE_COLOR_BUTTON, *iconMgr.getIcon("setting_color") );
	m_pSampleColorButton->SetToolTip("Change selected sample color");
	m_pSampleColorButton->SetBitmapDisabled(*iconMgr.getIcon("setting_color_disabled"));
	pButtonSizer->Add( m_pSampleColorButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	SamplesSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	gHoriznSizer->Add( SamplesSizer, 2, (wxALL & ~wxLEFT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
		
	sizer->Add( gHoriznSizer );

	sizer->AddSpacer( 20 );

	wxSizer* groupTextSizer = new wxBoxSizer( wxVERTICAL );
	groupTextSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Select the standard sample. The standard sample is used in RT alignment,\nnormalization and so on.")) );
	
	groupTextSizer->AddSpacer( 20 );

	sizer->Add( groupTextSizer, 0, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	wxBoxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Standard Sample")), 0, wxALIGN_CENTER_VERTICAL );
	comboSizer->AddSpacer( 10 );

	m_stdCombo = new wxComboBox(
		this,
		ID_STANDARDS_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxCB_READONLY
	);
	
	comboSizer->Add( m_stdCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );
		
	sizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	
	pInnerSizer->Add( sizer );

	NoCategoryItemsSelected();
	NoSampleItemsSelected();
}

// transfer data from window
bool GroupSettingsPanel::TransferDataFromWindow() {
	
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	
	std::string group;
	for (int i = 0; i < m_pCategoryList->GetItemCount(); i++){
		if( i==0 ){
			group = FMT( "%s", m_pCategoryList->GetItemText(i) );
		}else{
			group.append( FMT(", %s", m_pCategoryList->GetItemText(i)) );
		}
	}
	
	int isel = m_stdCombo->GetSelection();
	wxString strSelName = m_stdCombo->GetString( isel );
	std::string s = FMT( "- %s(%s)\n- Set the standard sample.(%s)\n", 
	m_sPanelName.c_str(), group.c_str(), strSelName.c_str() );
	setSettingParam( s.c_str() );
	
	return true;
}

// size event
void GroupSettingsPanel::onSize( wxSizeEvent& evt ) {
	// default
	OnSize( evt );

	int cx = 0;
	int cy = 0;
	// category list width
	if( m_pCategoryList != NULL ) {
		m_pCategoryList->GetClientSize( &cx, &cy );
		m_pCategoryList->SetColumnWidth( 0, cx );
	}

	// sample list column size
	if( m_pSampleList != NULL ) {
		// sample list size
		m_pSampleList->GetClientSize( &cx, &cy );
		m_pSampleList->SetColumnWidth( 0, cx );
	}
}

// on select category item
void GroupSettingsPanel::onSelectCategoryItem( wxListEvent& evt )
{
	const long lSelectedItem = evt.GetIndex();

	m_pCategoryEditButton->Enable();
	m_pCategoryDeleteButton->Enable();
	m_pCategoryUpButton->Enable(lSelectedItem > 0);
	m_pCategoryDownButton->Enable(lSelectedItem < (m_pCategoryList->GetItemCount() - 1));
	m_pSampleAddButton->Enable();

	UpdateSampleList();
	NoSampleItemsSelected();
}

// on deselect category item
void GroupSettingsPanel::onDeselectCategoryItem( wxListEvent& evt )
{
	NoCategoryItemsSelected();
}

// no category items selected
void GroupSettingsPanel::NoCategoryItemsSelected()
{
	m_pCategoryEditButton->Enable(false);
	m_pCategoryDeleteButton->Enable(false);
	m_pCategoryUpButton->Enable(false);
	m_pCategoryDownButton->Enable(false);

	m_pSampleAddButton->Enable(false);
	UpdateSampleList();
	NoSampleItemsSelected();
}

// on new
void GroupSettingsPanel::onNewCategory( wxCommandEvent& evt )
{
	if (openEditDialog( false ))
	{
		m_bGroupSampleAdditionDeletion = true;
	}
}

// on edit
void GroupSettingsPanel::onEditCategory( wxCommandEvent& evt )
{
	openEditDialog( true );
}

// on delete
void GroupSettingsPanel::onDeleteCategory( wxCommandEvent& evt )
{
	// selected category
	int category = getSelectedCategoryID();
	if( category < 0 ) {
		kome::window::WindowTool::showInfo( "No group is selected. Select a group." );
		return;
	}

	// Check to see whether any of the samples in the selected group have been selected as the Standard Sample on the
	// Get Peak Positions, Fill Matrix or Peak Analysis panels.
	bool bUsedAsStandardSample = false;

	for( int i = (int)getSelectedCategoryInfo()->paths.size() - 1; (i >= 0) && !bUsedAsStandardSample; i-- ) {
		
		if (m_pParentDlg->SampleSelectedAsStandardOnGetPPFillMorPeakAnalysisPanels(
			getfilename(getSelectedCategoryInfo()->paths[ i ].path.c_str()).c_str()))
		{
			bUsedAsStandardSample = true;
		}
	}

	bool bContinueWithDelete = false;

	if (bUsedAsStandardSample)
	{
		bContinueWithDelete = kome::window::WindowTool::showYesNo( "One or more of the samples in the selected group is being used as the\nStandard Sample on one of the other tabs.\nIf it is deleted, a different sample will be automatically selected.\n\nAre you sure you want to delete the selected group?", "Delete anyway", "Don't delete" );
	}
	else
	{
		bContinueWithDelete = kome::window::WindowTool::showYesNo( "Are you sure you want to delete the selected group?", "Delete", "Don't delete" );
	}
	
	if (bContinueWithDelete)
	{
		// delete all the samples from the selected group from the m_pathSet and m_filenameSet arrays.
		// These arrays must be correctly maintained as they are used to ensure that duplicate filepaths or filenames are not added within a matrix.
		for( int j = (int)getSelectedCategoryInfo()->paths.size() - 1; j >= 0; j-- ) {

			// delete
			m_pathSet.erase( getSelectedCategoryInfo()->paths[ j ].path );
			m_filenameSet.erase( getfilename(getSelectedCategoryInfo()->paths[ j ].path.c_str()) );
		}

		m_categoryArray.erase(m_categoryArray.begin() + getSelectedCategoryArrayIndex());

		int iItemToSelect = getSelectedCategoryArrayIndex();

		// update list
		updateCategoryList();

		NoCategoryItemsSelected();

		// After a deletion, select the item at the same position, or the last item.
		if (iItemToSelect > m_pCategoryList->GetItemCount() - 1)
		{
			iItemToSelect = m_pCategoryList->GetItemCount() - 1;
		}
		if (iItemToSelect >= 0)
		{
			kome::window::ListTool::selectItem( m_pCategoryList, iItemToSelect );
		}

		SetEdited();

		m_bGroupSampleAdditionDeletion = true;

		m_pParentDlg->UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels();
	}
}

// on up
void GroupSettingsPanel::onUpCategory( wxCommandEvent& evt ) {
	// selected item
	int item = -1;
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pCategoryList, items );
	if( items.size() > 0 ) {
		item = items[ 0 ];
	}
	if( item < 1 ) {
		kome::window::WindowTool::showInfo( "No group is selected. Select a group." );
		return;
	}

	// category
	int category = m_pCategoryList->GetItemData( item );
	int prevCategory = m_pCategoryList->GetItemData( item - 1 );
	CategoryInfo catInfo = m_categoryArray[item];
	CategoryInfo prevCatInfo = m_categoryArray[item - 1];

	// change the order
	m_categoryArray[item - 1] = catInfo;
	m_pCategoryList->SetItemData( item - 1, category );

	m_categoryArray[item] = prevCatInfo;
	m_pCategoryList->SetItemData( item, prevCategory );

	// update list
	updateCategoryList();

	// select
	kome::window::ListTool::unselectAll( m_pCategoryList );
	kome::window::ListTool::selectItem( m_pCategoryList, item - 1 );


	SetEdited();

	m_bGroupSampleAdditionDeletion = true;
}

// on down
void GroupSettingsPanel::onDownCategory( wxCommandEvent& evt ) {
	// selected item
	int item = -1;
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pCategoryList, items );
	if( items.size() > 0 ) {
		item = items[ 0 ];
	}
	if( item < 0 || item >= m_pCategoryList->GetItemCount() - 1 ) {
		kome::window::WindowTool::showInfo( "No group is selected. Select a group." );
		return;
	}

	// category
	int category = m_pCategoryList->GetItemData( item );
	int nextCategory = m_pCategoryList->GetItemData( item + 1 );
	CategoryInfo catInfo = m_categoryArray[item];
	CategoryInfo nextCatInfo = m_categoryArray[item + 1];

	// change the order
	m_categoryArray[item + 1] = catInfo;
	m_pCategoryList->SetItemData( item + 1, category );

	m_categoryArray[item] = nextCatInfo;
	m_pCategoryList->SetItemData( item, nextCategory );

	// update list
	updateCategoryList();

	// select
	kome::window::ListTool::unselectAll( m_pCategoryList );
	kome::window::ListTool::selectItem( m_pCategoryList, item + 1 );

	SetEdited();

	m_bGroupSampleAdditionDeletion = true;
}

// on category double-clicked
void GroupSettingsPanel::onCategoryDoubleClicked( wxListEvent& evt ) {
	openEditDialog( true );
}

// Samples panel

void GroupSettingsPanel::onSelectSampleItem( wxListEvent& evt )
{
	const long lSelectedItem = evt.GetIndex();

	m_pSampleDeleteButton->Enable();
	m_pSampleUpButton->Enable(lSelectedItem > 0);
	m_pSampleDownButton->Enable(lSelectedItem < (m_pSampleList->GetItemCount() - 1));
	m_pSampleColorButton->Enable();
}

void GroupSettingsPanel::onDeselectSampleItem( wxListEvent& evt )
{
	NoSampleItemsSelected();
}

void GroupSettingsPanel::NoSampleItemsSelected()
{
	m_pSampleDeleteButton->Enable(false);
	m_pSampleUpButton->Enable(false);
	m_pSampleDownButton->Enable(false);
	m_pSampleColorButton->Enable(false);
}

// add
void GroupSettingsPanel::onAddSample( wxCommandEvent& evt )
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

	m_categoryID = getSelectedCategoryID();
	if( m_categoryID < 0 ) {
		kome::window::WindowTool::showInfo( "No group is selected. Select a group." );
		return;
	}

	// color
	CategoryInfo *pCurrentCategory;
	pCurrentCategory = getSelectedCategoryInfo();

	// file dialog
	
	// #172 AXIMAファイルを読み込みたい @date 2013.04.18 <Mod> M.Izumi
	kome::window::RawDataFileDialog fileDlg( this );

	if( fileDlg.ShowModal() == wxID_OK ) {

		bool bDuplicatePathFound = false;
		bool bDuplicateFilenameFound = false;
		bool bAtLeastOneSampleAddedOK = false;

		// paths
		std::vector< std::string > paths;

		for( unsigned int iPathIndex = 0; iPathIndex < fileDlg.getNumberOfPaths(); iPathIndex++ ) {
			paths.push_back( fileDlg.getPath( iPathIndex ) );
		}

		std::sort( paths.begin(), paths.end() );

		std::string filename;
		std::string lastFilenameAdded;
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
			else if( bDuplicatePathFound && m_filenameSet.find( filename ) != m_filenameSet.end() ) // フォルダとパス名が完全一致で確認ダイアログを出すように変更 @date 2013.04.18 <Mod> M.Izumi
			{
				// Although this path has not already been added to a category, the same filename in
				// a different path has been added to a category, so it will be ignored.
				bDuplicateFilenameFound = true;
			}
			else
			{
				m_pathSet.insert( path );
				m_filenameSet.insert( filename );
				pCurrentCategory->paths.resize( pCurrentCategory->paths.size() + 1 );

				pCurrentCategory->paths.back().path = path;
				pCurrentCategory->paths.back().color = pCurrentCategory->color;
				pCurrentCategory->paths.back().category = m_categoryID;
				pCurrentCategory->paths.back().categoryName = pCurrentCategory->name;

				m_pSampleList->addData( pCurrentCategory->paths.size()-1, false );
				lastFilenameAdded = getfilename(path.c_str()).c_str();

				m_pSampleList->setDataColor( pCurrentCategory->paths.size()-1, pCurrentCategory->color );

				kome::window::ListTool::unselectAll( m_pSampleList );
				NoSampleItemsSelected();

				bAtLeastOneSampleAddedOK = true;
			}
		}
		m_pSampleList->updateList();

		if (bAtLeastOneSampleAddedOK)
		{
			// Only set the edited flag if at least 1 sample is successfully added, i.e.
			// it has not already been registered for any category.
			SetEdited();

			m_bGroupSampleAdditionDeletion = true;

			// Additionally, if at least 1 sample is successfully added, update standard
			// samples on the Get Peak Positions, Fill Matrix and Peak Analysis panels.
			m_pParentDlg->UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels();

			// Automatically select the last sample added.
			SelectSampleItem(lastFilenameAdded.c_str());
		}

		if (bDuplicatePathFound && bDuplicateFilenameFound)
		{
			kome::window::WindowTool::showInfo( "Samples already in a group have been ignored.\n\nAlso, samples with the same filenames as those already in a group have been ignored." );
		}
		else if (bDuplicatePathFound)
		{
			kome::window::WindowTool::showInfo( "Samples already assigned to a group have been ignored." );
		}
		else if (bDuplicateFilenameFound)
		{
			kome::window::WindowTool::showInfo( "Samples with the same filenames as those already in a group have been ignored." );
		}
	}
}

// delete
void GroupSettingsPanel::onDeleteSample( wxCommandEvent& evt ) {

	if (!getSelectedCategoryInfo())
	{
		kome::window::WindowTool::showInfo( "No sample is selected. Select a sample." );
		return;
	}

	// selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_pSampleList, items );

	// Check to see whether any of the selected samples have been selected as the Standard Sample on the
	// Get Peak Positions, Fill Matrix or Peak Analysis panels.
	bool bUsedAsStandardSample = false;

	for( int i = (int)items.size() - 1; (i >= 0) && !bUsedAsStandardSample; i-- ) {
		// item
		int item = items[ i ];

		if (m_pParentDlg->SampleSelectedAsStandardOnGetPPFillMorPeakAnalysisPanels(
			getfilename(getSelectedCategoryInfo()->paths[ item ].path.c_str()).c_str()))
		{
			bUsedAsStandardSample = true;
		}
	}

	if ((!bUsedAsStandardSample) || kome::window::WindowTool::showYesNo( "One or more of the selected samples is being used as the\nStandard Sample on one of the other tabs.\nIf deleted, a different sample will be automatically selected.\n\nAre you sure you want to delete the selected sample(s)?", "Delete anyway", "Don't delete" ))
	{
		// deselect all
		kome::window::ListTool::unselectAll( m_pSampleList );

		// delete selected items
		int iItemToSelect = -1;
		for( int i = (int)items.size() - 1; i >= 0; i-- ) {
			// item
			int item = items[ i ];

			// delete
			m_pathSet.erase( getSelectedCategoryInfo()->paths[ item ].path );
			m_filenameSet.erase( getfilename(getSelectedCategoryInfo()->paths[ item ].path.c_str()) );
			getSelectedCategoryInfo()->paths.erase( getSelectedCategoryInfo()->paths.begin() + item );
			iItemToSelect = item;
		}

		UpdateSampleList();
		NoSampleItemsSelected();

		// After a deletion, select the item at the same position as the last one deleted, or the last item.
		int icount =  m_pSampleList->GetItemCount() - 1;
		if (iItemToSelect > m_pSampleList->GetItemCount() - 1)
		{
			iItemToSelect = m_pSampleList->GetItemCount() - 1;
		}
		if (iItemToSelect >= 0)
		{
			kome::window::ListTool::selectItem( m_pSampleList, iItemToSelect );
		}

		SetEdited();

		m_bGroupSampleAdditionDeletion = true;

		m_pParentDlg->UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels();
	}
}

// up
void GroupSettingsPanel::onUpSample( wxCommandEvent& evt )
{
	CategoryInfo *pCurrentCategory;
	pCurrentCategory = getSelectedCategoryInfo();

	if (!pCurrentCategory)
	{
		kome::window::WindowTool::showInfo( "No sample is selected. Select a sample." );
		return;
	}

	// selected items
	int num = (int)pCurrentCategory->paths.size();
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

			SampleInfo tmp = pCurrentCategory->paths[ i ];

			pCurrentCategory->paths[ i ] = pCurrentCategory->paths[ i - 1 ];
			m_pSampleList->setDataColor( i, pCurrentCategory->paths[ i -  1 ].color );

			pCurrentCategory->paths[ i - 1 ] = tmp;
			m_pSampleList->setDataColor( i-1, kome::img::ColorTool::getRed( tmp.color ) );
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

	SetEdited();

	m_bGroupSampleAdditionDeletion = true;
}

// down
void GroupSettingsPanel::onDownSample( wxCommandEvent& evt )
{
	CategoryInfo *pCurrentCategory;
	pCurrentCategory = getSelectedCategoryInfo();

	if (!pCurrentCategory)
	{
		kome::window::WindowTool::showInfo( "No sample is selected. Select a sample." );
		return;
	}

	// selected items
	int num = (int)pCurrentCategory->paths.size();
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

			SampleInfo tmp = pCurrentCategory->paths[ i ];

			pCurrentCategory->paths[ i ] = pCurrentCategory->paths[ i + 1 ];
			m_pSampleList->setDataColor( i, pCurrentCategory->paths[ i + 1 ].color );

			pCurrentCategory->paths[ i + 1 ] = tmp;
			m_pSampleList->setDataColor( i+1, tmp.color );
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

	SetEdited();

	m_bGroupSampleAdditionDeletion = true;
}

// on select standard
void GroupSettingsPanel::onSelectStandard( wxCommandEvent& evt ){
	SetEdited();
}

// check whether a sample is selected as the standard sample on this panel.
bool GroupSettingsPanel::IsSelectedStandardSample(const char* sSample)
{
	if( m_stdCombo == NULL ){
		return false;
	}
	return (!m_stdCombo->GetValue().compare(sSample));
}

// color
void GroupSettingsPanel::onColorSample( wxCommandEvent& evt ) {

	if (!getSelectedCategoryInfo())
	{
		kome::window::WindowTool::showInfo( "No sample is selected. Select a sample." );
		return;
	}

	// selected items
	int num = (int)getSelectedCategoryInfo()->paths.size();
	if( num == 0 ) {
		return;
	}

	bool* selected = new bool[ num ];
	for( int i = 0; i < num; i++ ) {
		selected[ i ] = kome::window::ListTool::isSelected( m_pSampleList, i );
	}

	// choose color
	wxColourDialog dlg( this );
	if( dlg.ShowModal() == wxID_OK ) {
		// color
		wxColour& color = dlg.GetColourData().GetColour();
		COLOR c= kome::img::ColorTool::getColor( color.Red(), color.Green(), color.Blue() );

		// change color
		for( int i = 0; i < num; i++ ) {
			if( selected[ i ] ) {
				m_pSampleList->setDataColor( i, c );
				getSelectedCategoryInfo()->paths[ i ].color = c;
			}
		}
		m_pSampleList->updateList();
	}

	// refresh
	Refresh();

	// part of the way
	delete[] selected;

	SetEdited();
}

// update sample list
void GroupSettingsPanel::UpdateSampleList()
{
	// First of all, clear the sample list.
	m_pSampleList->clearData(true);

	m_categoryID = getSelectedCategoryID();
	if( m_categoryID >= 0 )
	{
		CategoryInfo* pCurrentCategory = getSelectedCategoryInfo();

		if (pCurrentCategory != NULL)
		{
			SampleInfo currentSample;

			for( unsigned int i = 0; i < pCurrentCategory->paths.size(); i++ )
			{
				currentSample = pCurrentCategory->paths[i];

				m_pSampleList->addData( i, false );
				m_pSampleList->setDataColor( i, currentSample.color );
			}

			m_pSampleList->updateList();
		}
	}
}

// get total number of samples
int GroupSettingsPanel::GetTotalNumberOfSamples()
{
	int nCount = 0;

	for ( unsigned int iCategoryIndex = 0; iCategoryIndex < m_categoryArray.size(); iCategoryIndex++)
	{
		CategoryInfo* pCategory = &m_categoryArray[iCategoryIndex];

		if (pCategory != NULL)
		{
			nCount+= pCategory->paths.size();
		}
	}

	return nCount;
}

// save panel settings
bool GroupSettingsPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	int project = GetActiveMatrix();

	std::ostringstream infoString;
	// Save settings not implemented via SettingParameterValues in the INI file
	kome::core::IniFile* pParamsIniFile = GetParamsIniFile();
	if (pParamsIniFile != NULL)
	{
		pParamsIniFile->setString( GetINISectionNameForMatrix(), GPP_STANDARD_SAMPLE_PARAM_KEY, m_stdCombo->GetValue().c_str() );
		infoString << "Standard Sample: " << m_stdCombo->GetValue() << "\n";
	}

	// path count
	int cnt = (int) GetTotalNumberOfSamples();
	int nProgressCount = 0;

	// This check is required to prevent a /0 error when there are no samples to save.
	if (cnt > 0)
	{
		pProgress->setRange( 0, cnt );
		pProgress->setPosition( 0 );
	}

	// clear groups
	std::vector< int > groups;
	identMgr.getGroups( groups, project );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.deleteGroup( groups[ i ] );
	}

	// clear samples
	std::vector< int > samples;
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, groups[ i ] );
	}

	for( unsigned int j = 0; j < samples.size(); j++ ) {
		identMgr.deleteMatrixSample( samples[ j ] );
	}

	// Only delete the previous Get Peak Positions, Fill Matrix and Peak Analysis results if
	// a change has been made on the Group Settings panel that invalidates those results.
	if (m_bGroupSampleAdditionDeletion)
	{
		// clear peaks
		std::vector< int > peaks;
		identMgr.getMatrixPeaks( peaks, project );
		for( unsigned int i = 0; i < peaks.size(); i++ ) {
			identMgr.deleteMatrixPeak( peaks[ i ] );
		}

		// Always delete the added columns, so that previous Peak Analysis results are deleted.
		std::vector< int > columns;
		identMgr.getMatrixColumns( columns, project );
		for( unsigned int i = 0; i < columns.size(); i++ ) {
			identMgr.deleteMatrixColumn( columns[ i ] );
		}
	}

	int nCategories = m_categoryArray.size();

	m_pParentDlg->GetDialogProgressPtr()->setInformation( GetPanelName().c_str(), "" );

	for ( unsigned int iCategoryIndex = 0;
		iCategoryIndex < m_categoryArray.size() && !pProgress->isStopped();
		iCategoryIndex++)
	{
		CategoryInfo* pCategory = &m_categoryArray[iCategoryIndex];

		if (pCategory != NULL)
		{
			int nAssignedCategoryID = identMgr.addGroup( pCategory->name.c_str(), project, pCategory->color );

			// Must update the category ID with the newly-assigned value.
			pCategory->id = nAssignedCategoryID;

			SampleInfo* pCurrentSample;

			for( unsigned int iSampleIndex = 0;
				iSampleIndex < pCategory->paths.size() && !pProgress->isStopped();
				iSampleIndex++ )
			{
				pCurrentSample = &pCategory->paths[iSampleIndex];

				// Must update the category ID with the newly-assigned value.
				pCurrentSample->category = nAssignedCategoryID;

				std::string msg = FMT( "%s: sample [%d/%d] ... %s",
										GetPanelName().c_str(),
										++nProgressCount, cnt,
										getfilename(pCurrentSample->path.c_str()).c_str() );
				pProgress->setStatus( msg.c_str() );

				infoString << msg << "\n";
				m_pParentDlg->GetDialogProgressPtr()->setInformation(infoString.str().c_str());

				bool opened0 = false;
				kome::objects::SampleSet* ss = factory.searchFile( pCurrentSample->path.c_str() );
				if( ss == NULL ) {
					ss = factory.openFile( pCurrentSample->path.c_str() );
				}
				else {
					opened0 = true;
				}

				if( ss != NULL ) {
					for( unsigned int i = 0; i < ss->getNumberOfSamples(); i++ ) {
						kome::objects::Sample* s = ss->getSample( i );
						int sampleId = identMgr.addSample( s );
						std::string sampleName = s->getName();
						if( !sampleName.empty() ) {
							sampleName.append( " : " );
						}
						sampleName.append( getfilename( pCurrentSample->path.c_str() ) );
						if( sampleId >= 0 ) {
							identMgr.addMatrixSample( pCurrentSample->category, sampleId, sampleName.c_str(), pCurrentSample->color );
						}
					}
				}

				if( !opened0 ) {
					factory.closeFile( ss );
				}

				pProgress->setPosition( nProgressCount );
			}
		}
	}
	
	// standard sample set	
	std::vector< int > matrixGroups;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}
	
	int sel = ( m_stdCombo->IsEnabled() ? m_stdCombo->GetSelection() : -1 );
	if( sel >= 0 && sel < (int)samples.size() ) {
		int id = samples[ sel ];
		identMgr.setStandardSample( project, id );
	}
	// commit
	if( !pProgress->isStopped() ) {
		pProgress->fill();
		bSuccess = true;

		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.updateMatrixList();
	}

	return bSuccess;
}

// Validate panel settings 
bool GroupSettingsPanel::IsValid()
{
	// No validation is necessary for the Group Settings panel, so always return true.

	return true;
}

// Update standard samples on this panel
void GroupSettingsPanel::updateStandardSamples(bool bReadFromINIfile)
{
	std::vector< std::string > samples;
	m_pParentDlg->GetMatrixSamplesFromGroupSettingsPanel(&samples);

	updateStandardSampleCombo(bReadFromINIfile, samples, m_stdCombo, GPP_STANDARD_SAMPLE_PARAM_KEY);
}

// Refresh panel contents for matrix
void GroupSettingsPanel::RefreshPanelContentsForMatrix()
{
	kome::window::ListTool::unselectAll( m_pCategoryList );
	NoCategoryItemsSelected();

	// Load category - samples information for this matrix.
	LoadCurrentMatrixCategorySamplesInfo();

	// Refresh category list
	updateCategoryList();

	// After a change of matrix, select the first category by default.
	if (m_pCategoryList->GetItemCount() > 0)
	{
		kome::window::ListTool::selectItem( m_pCategoryList, 0 );
	}

	updateStandardSamples();

	updateSettings();

}

// Refresh settings for matrix
void GroupSettingsPanel::updateSettings()
{
	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

// Local current matrix categories  - samples information
void GroupSettingsPanel::LoadCurrentMatrixCategorySamplesInfo()
{
	// For all categories

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get categories
	std::vector< int > groups;
	identMgr.getGroups( groups, GetActiveMatrix() );

	m_categoryArray.clear();
	m_pathSet.clear();
	m_filenameSet.clear();

	for( unsigned int iCategoryIndex = 0; iCategoryIndex < groups.size(); iCategoryIndex++ ) {
		// category information
		std::string categoryName;
		COLOR categoryColor = kome::img::ColorTool::BLACK;
		std::vector< SampleInfo > paths;

		m_categoryArray.resize( m_categoryArray.size() + 1 );
		int groupId = groups[ iCategoryIndex ];

		m_categoryArray.back().name = identMgr.getGroupName( groupId );
		m_categoryArray.back().id = groupId;
		m_categoryArray.back().color = identMgr.getGroupColor( groupId );

		if ( groupId >= 0 )
		{
			int project = GetActiveMatrix();

			std::vector< int > samples;
			identMgr.getMatrixSamplesFromGroup( samples, groupId );		   
			for( unsigned int iGroupIndex = 0; iGroupIndex < samples.size(); iGroupIndex++ )
			{
				// sample information
				int matrixSampleId = samples[ iGroupIndex ];
				int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );
				std::string filePath = identMgr.getSampleFilePath( sampleId );
				std::string fileName = identMgr.getMatrixSampleName( matrixSampleId );
				COLOR color = identMgr.getMatrixSampleColor( matrixSampleId );

				m_pathSet.insert( filePath );
				m_filenameSet.insert( getfilename(filePath.c_str()) );
				paths.resize( paths.size() + 1 );

				paths.back().path = filePath;
				paths.back().color = color;
				paths.back().category = groupId;
				paths.back().categoryName = categoryName;
			}
		}

		m_categoryArray.back().paths = paths;
	}
}

// gets the list of samples that are currently registere (only relevant for the Group Settings panel).
void GroupSettingsPanel::GetMatrixSamples(std::vector< std::string > *pSamples)
{
	pSamples->clear();

	int nCategories = m_categoryArray.size();

	for ( int iCategoryIndex = 0;
		iCategoryIndex < nCategories;
		iCategoryIndex++)
	{
		CategoryInfo* pCategory = &m_categoryArray[iCategoryIndex];

		if (pCategory != NULL)
		{
			SampleInfo* pCurrentSample;

			for( unsigned int iSampleIndex = 0;
				iSampleIndex < pCategory->paths.size();
				iSampleIndex++ )
			{
				pCurrentSample = &pCategory->paths[iSampleIndex];

				pSamples->push_back(getfilename(pCurrentSample->path.c_str()));
			}
		}
	}
}

// get gategory info
CategoryInfo GroupSettingsPanel::getGategoryInfo(int item){
	return m_categoryArray[item];
}

// get select group
CategoryInfo* GroupSettingsPanel::getSelectGroup(){
	int iItemToSelect = getSelectedCategoryArrayIndex();
	if( iItemToSelect == -1 ){
		iItemToSelect = m_categoryID;
	}

	return &m_categoryArray[iItemToSelect];
}

// set color
void GroupSettingsPanel::setColor(){
	// デフォルトカラー設定 
	if( m_color == kome::img::ColorTool::BLUE ){
		m_color = kome::img::ColorTool::RED;
	}else if( m_color == kome::img::ColorTool::RED ){
		m_color = kome::img::ColorTool::GREEN;
	}else if( m_color == kome::img::ColorTool::GREEN ){
		m_color = kome::img::ColorTool::PURPLE;
	}else if( m_color == kome::img::ColorTool::PURPLE ){
		m_color = kome::img::ColorTool::BLUE;
	}
}
