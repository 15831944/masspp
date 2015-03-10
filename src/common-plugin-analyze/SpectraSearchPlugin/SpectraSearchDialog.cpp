/**
 * @file SpectraSearchDialog.cpp
 * @brief implements of SpectraSearchDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SpectraSearchDialog.h"
#include "SpectraSearchFilter.h"
#include "FilterSpectraOperation.h" 

#include <wx/gbsizer.h>

using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					3
#define GRID_GAP					3
#define NUMBER_EDIT_WIDTH			75
#define STRING_EDIT_WIDTH			100

#define ID_SEARCH_RADIO_BOX			1
#define ID_PROP_KEY_TEXT			2
#define ID_PROP_COMBO				3
#define ID_PROP_VALUE_TEXT			4
#define ID_PROP_ADD_BUTTON			5
#define ID_PROP_DELETE_BUTTON		6
#define ID_PROP_LISTBOX				7
#define ID_MZ_BASE_POINT_COMBO		8
#define ID_MZ_POSITION_TEXT			9
#define ID_MZ_THRESHOLD_TEXT		10
#define ID_ABS_INTENSITY_TEXT		11
#define ID_REL_INTENSITY_TEXT		12
#define ID_SEARCH_BUTTON			13

#define STAGE_PARAM_NAME			"Stage"
#define SCAN_PARAM_NAME				"Scan"
#define RT_PARAM_NAME				"RT"
#define TIC_PARAM_NAME				"TIC"
#define BPC_PARAM_NAME				"BPC"
#define PRECURSOR_PARAM_NAME		"Precursor"


BEGIN_EVENT_TABLE( SpectraSearchDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_PROP_ADD_BUTTON, SpectraSearchDialog::onAddProp )
	EVT_BUTTON( ID_PROP_DELETE_BUTTON, SpectraSearchDialog::onDeleteProp )
	EVT_BUTTON( ID_SEARCH_BUTTON, SpectraSearchDialog::onSearch )
END_EVENT_TABLE()


// constructor
SpectraSearchDialog::SpectraSearchDialog( wxWindow* parent, kome::objects::DataGroupNode* group )
		: kome::window::ParametersAndHelpDialog( parent, "Filter Spectra" ), m_params( NULL, true ) {
	// initialize members
	m_type			 = SpectraSearchManager::SEARCH_NEW;
	m_searchRadioBox   = NULL;
	m_propKeyText	  = NULL;
	m_propValueText	= NULL;
	m_propCombo		= NULL;
	m_propList		 = NULL;
	m_group = group;	
	m_bCallFlg = false;
}

// constructor
SpectraSearchDialog::SpectraSearchDialog( wxWindow* parent, kome::objects::DataSet* dataset, kome::objects::DataSet& ds, bool bCallFlg )
	: kome::window::ParametersAndHelpDialog( parent, "Filter Spectra" ), m_params( NULL, true ) {
	// initialize members
	m_type			 = SpectraSearchManager::SEARCH_NEW;
	m_searchRadioBox   = NULL;
	m_propKeyText	  = NULL;
	m_propValueText	= NULL;
	m_propCombo		= NULL;
	m_propList		 = NULL;
	m_dataSet = dataset;
	m_ds = ds;
	m_bCallFlg = bCallFlg;

}

// destructor
SpectraSearchDialog::~SpectraSearchDialog() {
}

// get filter
SpectraSearchFilter& SpectraSearchDialog::getFilter() {
	return m_filter;
}

// get search type
SpectraSearchManager::SearchType SpectraSearchDialog::getType() {
	return m_type;
}

// create main sizer
wxSizer* SpectraSearchDialog::createMainSizer() {
	// create main sizer
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->AddGrowableCol( 1 );
	mainSizer->AddGrowableCol( 2 );

	// create spectrum sizer
	wxSizer* specSizer = createSpectrumSizer();
	mainSizer->Add( specSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// create properties sizer
	wxSizer* propsSizer = createPropertiesSizer();
	mainSizer->Add( propsSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return mainSizer;
}

// create button sizer
wxSizer* SpectraSearchDialog::createButtonSizer() {
	// create button sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  
	// search button
	sizer->Add(
		new wxButton( this, ID_SEARCH_BUTTON, wxT( "Filter" ) ),
		0,
		wxALIGN_RIGHT,
		BORDER_SIZE
	);

	return sizer;
}

// create spectrum sizer
wxSizer* SpectraSearchDialog::createSpectrumSizer() {
	// create spectrum sizer
	wxFlexGridSizer* specSearchSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	specSearchSizer->AddGrowableRow( 1 );
	specSearchSizer->AddGrowableCol( 0 );
	
	if( !m_bCallFlg ){
		// create search radio button
		wxString searches[] = {
			_T( "New" ),
			_T( "Append" ),
			_T( "Narrow" )
		};
		m_searchRadioBox = new wxRadioBox(
			this,
			ID_SEARCH_RADIO_BOX,
			wxT( "Search" ),
			wxDefaultPosition,
			wxDefaultSize,
			sizeof( searches ) / sizeof( wxString ),
			searches,
			0,
			wxRA_SPECIFY_COLS
		);
		m_searchRadioBox->SetSelection( 0 );
		specSearchSizer->Add( m_searchRadioBox, 0, wxGROW | wxALL, BORDER_SIZE );
	}
	// create spectrum sizer
	wxStaticBoxSizer* spectrumSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Spectrum" ) );

	// page information
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setCommon( false );
	page->setName( "spectra" );

	// @date 2011.03.17 <Mod> M.Izumi ->
	// strage
	kome::plugin::SettingsValue* strage_intVal = m_params.addValue();
	strage_intVal->setType( kome::plugin::SettingsValue::TYPE_INT_RANGE );
	strage_intVal->getNumberRestriction().setInclusiveMinValue( 1.0 );
	strage_intVal->setRequired( false );

	// scan
	kome::plugin::SettingsValue* scan_intVal = m_params.addValue();
	scan_intVal->setType( kome::plugin::SettingsValue::TYPE_INT_RANGE );
	scan_intVal->getNumberRestriction().setInclusiveMinValue( 0.0 );
	scan_intVal->setRequired( false );
	// <-

	kome::plugin::SettingsValue* dblVal = m_params.addValue();
	dblVal->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	dblVal->getNumberRestriction().setInclusiveMinValue( 0.0 );
	dblVal->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	kome::plugin::SettingsForm* form = page->addForm();
	param->setName( STAGE_PARAM_NAME );
	param->setValue( strage_intVal );		// @date 2011.03.17 <Mod> M.Izumi
	
	form->setTitle( STAGE_PARAM_NAME );
	form->setParam( param );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( SCAN_PARAM_NAME );
	param->setValue( scan_intVal );			// @date 2011.03.17 <Mod> M.Izumi
	
	form->setTitle( SCAN_PARAM_NAME );
	form->setParam( param );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( RT_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( RT_PARAM_NAME );
	form->setParam( param );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( TIC_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( TIC_PARAM_NAME );
	form->setParam( param );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( BPC_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( BPC_PARAM_NAME );
	form->setParam( param );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( PRECURSOR_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( PRECURSOR_PARAM_NAME );
	form->setParam( param );

	// panel
	kome::window::SettingsPanel* specPanel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings,
		false
	);
	spectrumSizer->Add( specPanel, 1, wxALL | wxGROW, BORDER_SIZE );
	specSearchSizer->Add( spectrumSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return specSearchSizer;
}

// create properties sizer
wxSizer* SpectraSearchDialog::createPropertiesSizer() {
	// create properties sizer
	wxStaticBoxSizer* propsSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Properties" ) );

	// create property grid sizer
	wxFlexGridSizer* propGridSizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	propGridSizer->AddGrowableRow( 2 );
	propGridSizer->AddGrowableCol( 0 );

	// create edit sizer
	wxFlexGridSizer* editSizer = new wxFlexGridSizer( 3, 2, GRID_GAP, GRID_GAP );
	editSizer->AddGrowableCol( 1 );

	// label (key)
	editSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "(Key)" ) ),
		0,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// key text
	m_propKeyText = new wxTextCtrl(
		this,
		ID_PROP_KEY_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 )
	);
	editSizer->Add(
		m_propKeyText,
		1,
		wxALIGN_CENTER_VERTICAL | wxGROW,
		BORDER_SIZE
	);

	// dummy
	editSizer->Add(
		new wxStaticText( this, wxID_ANY, wxEmptyString ),
		0,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// combo
	wxString propTypes[] = {
		wxT( "is" ),
		wxT( "is not" ),
		wxT( "contains" ),
		wxT( "does not contain" ),
		wxT( "is more than" ),
		wxT( "is not more than" ),
		wxT( "is less than" ),
		wxT( "is not less than" )
	};
	m_propCombo = new wxComboBox(
		this,
		ID_PROP_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		sizeof( propTypes ) / sizeof( wxString ),
		propTypes,
		wxCB_SIMPLE | wxCB_READONLY
	);
	m_propCombo->SetSelection( 0 );
	editSizer->Add(
		m_propCombo,
		0,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// label (value)
	editSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "(Value)" ) ),
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// value text
	m_propValueText = new wxTextCtrl(
		this,
		ID_PROP_VALUE_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 )
	);
	editSizer->Add(
		m_propValueText,
		1,
		wxALIGN_CENTER_VERTICAL | wxGROW,
		BORDER_SIZE
	);

	editSizer->AddGrowableCol( 2 );
	editSizer->AddGrowableCol( 4 );
	propGridSizer->Add( editSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// add button
	wxBoxSizer* btnSizer = new wxBoxSizer( wxVERTICAL );
	wxGridSizer* btnGridSizer = new wxGridSizer( 1, 2, GRID_GAP, GRID_GAP );

	btnGridSizer->Add(
		new wxButton( this, ID_PROP_ADD_BUTTON, wxT( "Add" ) ),
		0,
		wxALIGN_RIGHT,
		BORDER_SIZE
	);
	btnGridSizer->Add(
		new wxButton( this, ID_PROP_DELETE_BUTTON, wxT( "Delete" ) ),
		0,
		wxALIGN_RIGHT,
		BORDER_SIZE
	);

	btnSizer->Add( btnGridSizer, 0, wxALIGN_RIGHT, BORDER_SIZE );
	propGridSizer->Add( btnSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// create list
	m_propList = new wxListBox(
		this,
		ID_PROP_LISTBOX,
		wxDefaultPosition,
		wxSize( 100, 80 ),
		0,
		NULL,
		wxLB_EXTENDED  | wxLB_NEEDED_SB | wxLB_HSCROLL
	);
	propGridSizer->Add( m_propList, 1, wxGROW | wxALL, BORDER_SIZE );

	// add sizer
	propsSizer->Add( propGridSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return propsSizer;
}

// add property filter
void SpectraSearchDialog::onAddProp( wxCommandEvent& evt ) {
	// check key
	if( m_propKeyText->GetValue().empty() ) {
		wxMessageBox(
			wxT( "A parameter key must be specified. Type a parameter key." ),
			wxT( "Mass++" ),
			wxOK | wxICON_INFORMATION
		);
		return;
	}

	// get type
	SpectraSearchFilter::PropertyFilterType types[] = {
		SpectraSearchFilter::PROP_FILTER_IS,
		SpectraSearchFilter::PROP_FILTER_IS_NOT,
		SpectraSearchFilter::PROP_FILTER_CONTAINS,
		SpectraSearchFilter::PROP_FILTER_DOES_NOT_CONTAIN,
		SpectraSearchFilter::PROP_FILTER_IS_MORE_THAN,
		SpectraSearchFilter::PROP_FILTER_IS_NOT_MORE_THAN,
		SpectraSearchFilter::PROP_FILTER_IS_LESS_THAN,
		SpectraSearchFilter::PROP_FILTER_IS_NOT_LESS_THAN
	};
	int sel = CLAMP( m_propCombo->GetSelection(), 0, 7 );
	SpectraSearchFilter::PropertyFilterType type = types[ sel ];
	if( sel >= 4 ) {		//
		if( !isdouble( m_propValueText->GetValue().c_str() ) ) {
			std::string msg = FMT(
				"The property value must be a number when the filter type is \"%s\".",
				m_propCombo->GetString( sel ).c_str()
			);
			wxMessageBox(
				wxT( msg.c_str() ),
				wxT( "Mass++" ),
				wxOK | wxICON_INFORMATION
			);
			return;
		}
	}

	// add to object
	m_filter.addPropertyFilter(
		m_propKeyText->GetValue().c_str(),
		m_propValueText->GetValue().c_str(),
		type
	);

	// add to list
	std::string s = FMT(
		"\"%s\" %s \"%s\".",
		m_propKeyText->GetValue().c_str(),
		m_propCombo->GetString( sel ).c_str(),
		m_propValueText->GetValue().c_str()
	);
	m_propList->Append( wxT( s.c_str() ) );
}

// remove property filter
void SpectraSearchDialog::onDeleteProp( wxCommandEvent& evt ) {
	// get selection
	std::vector< int > items;
	for( unsigned int i = 0; i < m_propList->GetCount(); i++ ) {
		if( m_propList->IsSelected( i ) ) {
			items.push_back( i );
		}
	}
	if( items.size() == 0 ) {
		wxMessageBox(
			wxT( "Select a filter." ),
			wxT( "Mass++" ),
			wxOK | wxICON_INFORMATION
		);
		return;
	}

	// delete
	for( int i = (int)items.size() - 1; i >= 0; i-- ) {
		m_filter.removePropertyFilter( items[ i ] );
		m_propList->Delete( items[ i ] );
	}
}

// search
void SpectraSearchDialog::onSearch( wxCommandEvent& evt ) {
	// validate
	if( !Validate() ) {
		return;
	}

	// transfer
	if( !TransferDataFromWindow() ) {
		return;
	}

	int countOfItems = m_propList->GetCount();	
	if(	( countOfItems <= 0 ) && ( m_propKeyText->GetValue().empty() ) && (m_propValueText->GetValue()=="")
		&& ( m_strRange == ":,:,:,:,:,:" )//　SPEC 85551 (1)//　Spectrumの条件が全く入力されていない // @date 2012/05/15 <Add> OKADA
		){	
		
		//　SPEC 85551 (1)
		bool bRet = false;	// [Yes]=true [No]=false
		bRet = kome::window::WindowTool::showYesNo( 
						"No filter conditions have been added. Do you want to filter spectra anyway?", "Filter anyway", "Don't filter" );	   // 条件が入力されていないのですが、とにかく実行しますか？
		if( bRet ){
			;	// Yes
		}else{
			return;		// No
		}
	}	
	//　SPEC 85551 (2)
	else if(
		( m_filter.getNumberOfPropertyFilters() <= 0 )
		&& ( !( m_propKeyText->GetValue().empty() ) || (!(m_propValueText->GetValue()=="")) )
		)
	
	{	//  filter conditionがAddされていない
		
		bool bRet = false;	// [OK]=true [Cancel]=false
		bRet = kome::window::WindowTool::showOkCancel( 
					"There are filter condition values that have not been added to the condition list. If you continue, these values will not be used." );	   // filter conditionがAddされていません。もし続けるなら、これらの値は使用されません。
		if( bRet ){
			;	// OK
		}else{
			return;		// Cancel
		}
	}
	
	if( !m_bCallFlg ){
		// ------------------
		// Operation 作成
		// ------------------
		FilterSpectraOperation* opt = new FilterSpectraOperation();
	
		opt->setTargetSample( ( m_group == NULL ? NULL : m_group->getSample() ) );
	
		opt->setOperationName( "Filter Spectra" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "filter_spec" );
		opt->setType( m_type );
		opt->setFilter( m_filter );
		opt->setDescript( m_strOptDescript.c_str() );
		opt->setRange( m_strRange.c_str() );

		if( opt->execute() ){
		}else{
			delete opt;
		}
	}else{
		// call type "filter_spectra"
		SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

		// -------------------------------------------------
		// search	
		// -------------------------------------------------
		mgr.searchSpectra( m_dataSet, m_ds, m_type, m_filter );
		// set DataSet
		mgr.setFilterSpectra( &m_ds );
	}

	// end dialog
	EndDialog( wxID_OK );
}

// transfer data from window
bool SpectraSearchDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// set type
	SpectraSearchManager::SearchType types[] = {
		SpectraSearchManager::SEARCH_NEW,
		SpectraSearchManager::SEARCH_APPEND,
		SpectraSearchManager::SEARCH_NARROW
	};
	// @date 2014.01.15 <Mod> M.Izumi
	if( m_bCallFlg ){
		m_type = SpectraSearchManager::SEARCH_NARROW;
	}
	else {
		int sel = CLAMP( m_searchRadioBox->GetSelection(), 0, 2 );
		m_type = types[ sel ];
	}
		
	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();
	std::string str;

	m_strOptDescript = mgr.getSearchTypeName( m_type );
	m_strRange ="";
	// set range
	const char* range = m_settings.getParameterValue( STAGE_PARAM_NAME );
	m_filter.resetStageRange();
	m_filter.setStageRange( range );
	// description用の文字列セット
	m_strRange.append( FMT("%s", range));

	std::vector< std::string > strRange;
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",STAGE_PARAM_NAME, range );
		m_strOptDescript += FMT( ",%s", str.c_str() );
		
	}
	range = m_settings.getParameterValue( SCAN_PARAM_NAME );
	m_filter.resetScanRange();
	m_filter.setScanRange( range );
	m_strRange.append( FMT( ",%s",range ) );
	// description用の文字列セット
	strRange.clear();
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",SCAN_PARAM_NAME, range );
		m_strOptDescript += FMT( ",%s", str.c_str() );
	}

	range = m_settings.getParameterValue( RT_PARAM_NAME );
	m_filter.resetRtRange();
	m_filter.setRtRange( range );
	m_strRange.append( FMT( ",%s",range ) );
	// description用の文字列セット
	strRange.clear();
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",RT_PARAM_NAME, range );
		m_strOptDescript.append( FMT( ",%s", str.c_str() ));
	}

	range = m_settings.getParameterValue( TIC_PARAM_NAME );
	m_filter.resetTicRange();
	m_filter.setTicRange( range );
	m_strRange.append( FMT( ",%s",range ) );
	// description用の文字列セット
	strRange.clear();
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",TIC_PARAM_NAME, range );
		m_strOptDescript.append( FMT( ",%s", str.c_str() ));
	}

	range = m_settings.getParameterValue( BPC_PARAM_NAME );
	m_filter.resetBpcRange();
	m_filter.setBpcRange( range );
	m_strRange.append( FMT( ",%s",range ) );
	// description用の文字列セット
	strRange.clear();
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",BPC_PARAM_NAME, range );
		m_strOptDescript.append( FMT( ",%s", str.c_str() ));
	}

	range = m_settings.getParameterValue( PRECURSOR_PARAM_NAME );
	m_filter.resetPrecursorRange();
	m_filter.setPrecursorRange( range );
	m_strRange.append( FMT( ",%s",range ));
	// description用の文字列セット
	strRange.clear();
	stringtoken( range, ":", strRange );
	if( !strRange.empty() ){
		str = FMT( "[%s %s]",PRECURSOR_PARAM_NAME, range );
		m_strOptDescript.append( FMT( ",%s", str.c_str() ));
	}
	
	return true;
}




