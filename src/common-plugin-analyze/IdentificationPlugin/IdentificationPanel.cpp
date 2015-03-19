/**
 * @file IdentificationPanel.cpp
 * @brief interfaces of IdentificationPanel class
 *
 * @author M.Izumi
 * @date 2013.01.11
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "IdentificationPanel.h"
#include "SpectrumGetter.h"
#include "ComparativeIdentificationSpectrumGetter.h"
#include "IdentificationDialogBase.h"
#include "IdentificationPluginManager.h"

#include "../../common-lib/WindowManager/matrix_flow07.xpm"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP                     3
#define BORDER_SIZE                  3

#define SECTION                      "Identification"
#define TITLE_PARAM_NAME             "title"
#define TITLE_PARAM_KEY              "TITLE"
#define COMMENT_PARAM_NAME           "comment"
#define COMMENT_PARAM_KEY            "COMMENT"
#define ENGINE_PARAM_NAME            "engine"
#define ENGINE_PARAM_KEY             "ENGINE"
#define PEAK_DETECTOR_PARAM_NAME     "peaks"
#define PEAK_DETECTOR_PARAM_KEY      "PEAK_DETECTOR"
#define CHARGE_DETECTOR_PARAM_NAME   "charge"
#define CHARGE_DETECTOR_PARAM_KEY    "CHARGE_DETECTOR"
#define PEAK_FILTER_PARAM_NAME       "filter"
#define PEAK_FILTER_PARAM_KEY        "PEAK_FILTER"
#define IDENTIFICATION_FUNC_TYPE     "IDENTIFICATION"
#define PEAK_DETECT_FUNC_TYPE        "SPEC_PEAK_PICKING"
#define CHARGE_DETECT_FUNC_TYPE      "CHARGE_DETECT"
#define PEAK_FILTER_FUNC_TYPE        "SPEC_PEAK_FILTER"
#define RT_TOL_PARAM_NAME			 "rt tol"
#define RT_TOL_PARAM_KEY			 "RT_TOLERANCE"
#define MZ_TOL_PARAM_NAME			 "mz tol"
#define MZ_TOL_PARAM_KEY			 "MZ_TOLERANCE"


BEGIN_EVENT_TABLE( IdentificationPanel, kome::window::PeakMatrixSettingsPageBase )
END_EVENT_TABLE()

// constructor
IdentificationPanel::IdentificationPanel( wxWizard* parent, const char* name )
	: kome::window::PeakMatrixSettingsPageBase( parent, name ){
	// initialize
	m_progress = NULL;
	m_engine = NULL;
	m_searchSettings = NULL;
	m_searchItem = NULL;
	m_peakDetector = NULL;
	m_peakSettings = NULL;
	m_chargeDetector = NULL;
	m_chargeSettings = NULL;
	m_spectra = NULL;

	m_searchFlg = false;
	m_peakFlg = false;
	m_chargeFlg = false;

	m_helpPanel = NULL;
	m_book = NULL;

	SetSizer( createMainSizer() );
}

// destructor
IdentificationPanel::~IdentificationPanel(){
	if( m_spectra != NULL ){
		delete m_spectra;
	}
	m_spectra = NULL;
}

void IdentificationPanel::createInnerSizer(wxStaticBoxSizer* InnerSizer) {
	// set bitmap
	wxBitmap addBmp( matrix_flow07_xpm );
	
	wxBoxSizer* pCategoriesSizer = new wxBoxSizer( wxVERTICAL );
	pCategoriesSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( pCategoriesSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	InnerSizer->Add( gHoriznSizer );

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	// book
	m_book = new kome::window::RadioBoxBook( this );
	// panel
	m_helpPanel = new IdentificationHelpPanel( m_book );
	
	m_book->AddPage( m_helpPanel, wxT("Executing identification") );
	
	wxPanel* panel = new wxPanel( m_book );
	m_book->AddPage( panel, wxT("Not executing identification"));

	sizer->Add( m_book, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	InnerSizer->Add( sizer );
}


// Validate panel settings 
bool IdentificationPanel::IsValid() {
	return m_helpPanel->Validate();
}

// save panel settngs 
bool IdentificationPanel::SavePanelSettings( kome::core::Progress *pProgress ) {
	
	if( !TransferDataFromWindow() ) {
		return false;
	}

	if( m_helpPanel == NULL || m_book->GetSelection() != 0 ){
		return true;
	}
	m_progress = pProgress;
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int project = identMgr.getSelectedMatrix();
	if( project < 0 ) {
		LOG_WARN_CODE(
			FMT( "A matrix is not selected.\nComparative Identification will not be available.\n\nTo use this functionality, first create a matrix using Quantitation dialog." ),
			ERR_OTHER
		);
		
		return false;
	}

	bool ret = identify();
	
	return ret;
}

// 
bool IdentificationPanel::PreApplyValidationCheck(std::string& sError) {
	return true;
}

// Update standard samples on this panel
void IdentificationPanel::updateStandardSamples(bool bReadFromINIfile) {
}


// Refresh panel contents for matrix
void IdentificationPanel::RefreshPanelContentsForMatrix() {	
}

// identify
bool IdentificationPanel::identify(){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// return value
	bool ret = false;

	kome::ident::SearchEngine* engine = getSearchEngine();

	// progress
	kome::core::Progress* progress = m_progress;
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}
	progress->createSubProgresses( 3 );
	kome::core::Progress* prgs = progress->getSubProgress( 0 );

	// @date 2013.10.29 <Add> M.Izumi
	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settings;

	// identification
	int searchId = IdentificationPluginManager::identify(
		m_title.c_str(),
		m_comment.c_str(),
		m_searchItem,
		m_searchSettings,
		m_peakDetector,
		m_peakSettings,
		m_chargeDetector,
		m_chargeSettings,
		m_spectra,
		m_peakFilter.c_str(),
		prgs,
		items,
		settings
	);

	if( progress->isStopped() ) {
		return ret;
	}
	prgs->fill();

	// result
	prgs = progress->getSubProgress( 1 );
	std::string searchIdStr = identMgr.getSearchId( searchId );
	kome::ident::SearchEngine::SearchStatus status = engine->getSearchStatus( searchIdStr.c_str() );
	if( status == kome::ident::SearchEngine::STATUS_COMPLETED ) {
		kome::ident::SearchResult result;
		engine->getResult( result, searchIdStr.c_str(), *prgs );

		if( !progress->isStopped() ) {
			prgs = progress->getSubProgress( 2 );
			identMgr.registerResult( searchId, result, *prgs );
		}

		if( !progress->isStopped() ) {
			identMgr.setSearchStatus( searchId, status );
		}
	}

	if( !progress->isStopped() ) {
		return ret;
	}

	progress->fill();
	ret = true;

	return ret;
}

// transfer data from window
bool IdentificationPanel::TransferDataFromWindow() {
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	
	if( m_helpPanel == NULL ){
		return false;
	}
	// Spec No.91812 @date 2013.03.22 < Mod > M.Izumi 
	if(  m_book->GetSelection() != 0 ){
		return true;
	}

	m_spectra = createSpectrumGetter();
	
	m_title = m_helpPanel->getTitle();
	m_comment = m_helpPanel->getComment();
	m_searchSettings = m_helpPanel->getSearchSettings();
	m_searchItem = m_helpPanel->getSearchItem();
	m_peakDetector = m_helpPanel->getPeakDetector();
	m_peakSettings = m_helpPanel->getPeakDetectSettings();
	m_chargeDetector = m_helpPanel->getChargeDetector();
	m_chargeSettings = m_helpPanel->getChargeDetectSettings();
	m_peakFilter = m_helpPanel->getPeakFilter();
	
	// Config‚É‚¾‚·•¶Žš—ñƒZƒbƒg
	std::string strSearchParamName = ( m_searchItem != NULL ? m_searchItem->getLongName() : "" );
	std::string s = FMT( "- %s(%s)\n", m_sPanelName.c_str(), strSearchParamName.c_str() );
	setSettingParam( s.c_str() );
	

	return true;
}

// get spectrum getter
SpectrumGetter* IdentificationPanel::createSpectrumGetter() {
	// create
	SearchEngine* engine = getSearchEngine();
	if( engine == NULL ) {
		return NULL;
	}

	ComparativeIdentificationSpectrumGetter* spectra = new ComparativeIdentificationSpectrumGetter( engine );

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// files
	int project = identMgr.getSelectedMatrix();
	std::vector< int > groups;
	std::vector< int > samples;
	std::string paths;
	identMgr.getGroups( groups, project );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		std::vector< int > tmp;
		identMgr.getMatrixSamplesFromGroup( tmp, groups[ i ] );
		for( unsigned int j = 0; j < tmp.size(); j++ ) {
			int id = identMgr.getMatrixSampleSample( tmp[ j ] );
			std::string path = identMgr.getSampleFilePath( id );
			path = kome::plugin::SettingsValue::convertToDisplayingString( path.c_str() );

			if( !path.empty() ) {
				spectra->addFile( path.c_str() );
			}
		}
	}
		
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		spectra->addPeak( peaks[i] );
	}

	// tolerance
	spectra->setRtTol( m_helpPanel->getRtTolerance() );
	spectra->setMzTol( m_helpPanel->getMzTolerance() );

	return spectra;
}

// get search engine
kome::ident::SearchEngine* IdentificationPanel::getSearchEngine() {
	// function
	kome::plugin::PluginFunctionItem* item = ( m_helpPanel != NULL ? m_helpPanel->getSearchItem() : NULL );
	if( item == NULL ) {
		return NULL;
	}

	// search engine
	m_engine = (kome::ident::SearchEngine*)item->getCall()->invoke( NULL ).prim.pt;

	return m_engine;
}

// @date 2013.06.21 <Add> M.Izumi
// writes all child parameters
void IdentificationPanel::writeAllChildParameters( kome::objects::SettingParameterValues& settings, kome::core::Properties& props, const char* prefix ) {
	// prefix
	std::string p = NVL( prefix, "" );

	// write
	for( unsigned int i = 0; i < settings.getNumberOfParameters(); i++ ) {
		std::string name = settings.getParameterName( i );
		std::string value = settings.getParameterValue( i );
		std::string propName =  name;
		if( !p.empty() ) {
			propName = FMT( "%s : %s", p.c_str(), name.c_str() );
		}

		props.setValue( propName.c_str(), value.c_str() );

		// child
		if( settings.hasSubParameterValues( name.c_str(), value.c_str() ) ) {
			kome::objects::SettingParameterValues* child = settings.getSubParameterValues( name.c_str(), value.c_str() );
			writeAllChildParameters( *child, props, propName.c_str() );
		}
	}
}

// -------------------------------------------------------------------------------
// class IdentificationHelpPanel 
// -------------------------------------------------------------------------------

BEGIN_EVENT_TABLE( IdentificationHelpPanel, kome::window::ParametersAndHelpPanel )	
END_EVENT_TABLE()

IdentificationHelpPanel::IdentificationHelpPanel( wxWindow* parent )
	: kome::window::ParametersAndHelpPanel( parent ),m_params( NULL, false ){

	// create controls
	createControls();
}

IdentificationHelpPanel::~IdentificationHelpPanel(){
}

wxSizer* IdentificationHelpPanel::createMainSizer(){
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableCol( 0 );
		
	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Input parameters for attaching substance information to each matrix peaks.")) );
	textSizer->AddSpacer( 20 );
	sizer->Add( textSizer, 1, wxALL | wxGROW, 20 );	

	// search sizer
	wxSizer* searchSizer = createSearchSizer();
	sizer->Add( searchSizer, 1, wxALL | wxGROW, BORDER_SIZE );
		
	// spectra sizer
	wxSizer* spectraSizer = createSpectraSizer();
	sizer->Add( spectraSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// peaks sizer
	wxSizer* peaksSizer = createPeaksSizer();
	sizer->Add( peaksSizer, 1, wxALL | wxGROW, BORDER_SIZE );	

	return sizer;
}

// create search sizer
wxSizer* IdentificationHelpPanel::createSearchSizer() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// sizer
	wxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Search" ) );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "Search" );

	// title
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setRequired( true );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( TITLE_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( TITLE_PARAM_KEY );
	param->setValue( val );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "Title" );
	form->setParam( param );
	form->setDesc( "Input search title. This can be a guidepost for referring search result." );
	// comment
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setRequired( false );

	param = m_params.addParam();
	param->setName( COMMENT_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( COMMENT_PARAM_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setTitle( "Comment" );
	form->setParam( param );
	form->setDesc("Input appended information fo identification.");

	// search engine
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( IDENTIFICATION_FUNC_TYPE );
	val->setRequired( true );

	param = m_params.addParam();
	param->setName( ENGINE_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( ENGINE_PARAM_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setTitle( "Search Type" );
	form->setParam( param );
	form->setDesc("Select a search engine.");
	
	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// create spectra sizer
wxSizer* IdentificationHelpPanel::createSpectraSizer() {
	
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	// tolerance
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );

	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( val );
	param->setSection( SECTION );
	param->setName( RT_TOL_PARAM_NAME );
	param->setKey( RT_TOL_PARAM_KEY );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Tolerance" );
	form->setDesc( "RT tolerance for searching spectrum to post peaks to a search engine form peak positions." );

	param = m_params.addParam();
	param->setValue( val );
	param->setSection( SECTION );
	param->setName( MZ_TOL_PARAM_NAME );
	param->setKey( MZ_TOL_PARAM_KEY );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z Tolerance" );
	form->setDesc( "m/z tolerance for search spectrum to post peaks to a search engine from peak position." );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create peaks sizer
wxSizer* IdentificationHelpPanel::createPeaksSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "Peaks" );

	// peak detector
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( PEAK_DETECT_FUNC_TYPE );
	val->setRequired( true );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( PEAK_DETECTOR_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( PEAK_DETECTOR_PARAM_KEY );
	param->setValue( val );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "Spectrum Peak Detector" );
	form->setParam( param );
	form->setDesc( "Peak detection method for extract peaks from spectra." );

	// charge detector
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( CHARGE_DETECT_FUNC_TYPE );
	val->setRequired( true );

	param = m_params.addParam();
	param->setName( CHARGE_DETECTOR_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( CHARGE_DETECTOR_PARAM_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setTitle( "Charge Detector" );
	form->setParam( param );
	form->setDesc( "Charge calculation method." );

	// peak filter
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	val->setRequired( false );

	kome::plugin::SettingsValue* childVal = m_params.addValue();
	childVal->setType( kome::plugin::SettingsValue::TYPE_STRING );
	childVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	childVal->setEnumKey( PEAK_FILTER_FUNC_TYPE );
	childVal->setRequired( true );
	childVal->setName( "Peak Filter" );
	val->addChildValue( childVal );

	param = m_params.addParam();
	param->setName( PEAK_FILTER_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( PEAK_FILTER_PARAM_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setTitle( "Peak Filter" );
	form->setDesc( "Peak filter for peaks to be posted to a search engine." );

	// set menu Item @date 2012.12.06 <Add> M.Izumi
	form->setParam( param );
	form->setEditMenuItem( true );
	form->setEditMenuName( "Edit" );
	form->setAddMenuItem( true );
	form->setAddMenuName( "New" );
	form->setUpDownMenuItem( true );
	form->setSelectAllMenuItem( true );
	form->setDeleteMenuItem( true );
	form->setSaveAllMenuItem( true );
	form->setCopyMenuItem( false );
	form->setFilterMenuItem( false );
	
	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}
// get title
std::string IdentificationHelpPanel::getTitle() {
	const char* s = m_settings.getParameterValue( TITLE_PARAM_NAME );
	std::string title = NVL( s, "" );

	return title;
}

// get comment
std::string IdentificationHelpPanel::getComment() {
	const char* s = m_settings.getParameterValue( COMMENT_PARAM_NAME );
	std::string comment = NVL( s, "" );

	return comment;
}

// get search settings
kome::objects::SettingParameterValues* IdentificationHelpPanel::getSearchSettings() {
	// value
	const char* s = m_settings.getParameterValue( ENGINE_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// sub setting values
	kome::objects::SettingParameterValues* subSettings = m_settings.getSubParameterValues( ENGINE_PARAM_NAME, s );
	return subSettings;
}

// get search page
kome::plugin::PluginFunctionItem* IdentificationHelpPanel::getSearchItem() {
    // manger
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// value
	const char* s = m_settings.getParameterValue( ENGINE_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// function
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, s );
	return item;
}

// get peak detector
kome::plugin::PluginFunctionItem* IdentificationHelpPanel::getPeakDetector() {
	// manger
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// value
	const char* s = m_settings.getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// item
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( PEAK_DETECT_FUNC_TYPE, s );
	return item;
}

// get peak detect settings
kome::objects::SettingParameterValues* IdentificationHelpPanel::getPeakDetectSettings() {
	// value
	const char* s = m_settings.getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// sub setting values
	kome::objects::SettingParameterValues* subSettings = m_settings.getSubParameterValues( PEAK_DETECTOR_PARAM_NAME, s );
	return subSettings;
}

// get charge detector
kome::plugin::PluginFunctionItem* IdentificationHelpPanel::getChargeDetector() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// value
	const char* s = m_settings.getParameterValue( CHARGE_DETECTOR_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// item
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, s );
	return item;
}

// get charge settings
kome::objects::SettingParameterValues* IdentificationHelpPanel::getChargeDetectSettings() {
	// value
	const char* s = m_settings.getParameterValue( CHARGE_DETECTOR_PARAM_NAME );
	if( s == NULL ) {
		return NULL;
	}

	// sub setting values
	kome::objects::SettingParameterValues* subSettings = m_settings.getSubParameterValues( CHARGE_DETECTOR_PARAM_NAME, s );
	return subSettings;
}

// get peak filter
std::string IdentificationHelpPanel::getPeakFilter() {
	// filter
	const char* s = m_settings.getParameterValue( PEAK_FILTER_PARAM_NAME );
	std::string filter = NVL( s, "" );

	return filter;
}

// get RT tolerance
double IdentificationHelpPanel::getRtTolerance() {
	double rtTol = m_settings.getDoubleValue( RT_TOL_PARAM_NAME, -1.0 );
	return rtTol;
}

// get m/z tolerance
double IdentificationHelpPanel::getMzTolerance() {
	double mzTol = m_settings.getDoubleValue( MZ_TOL_PARAM_NAME, -1.0 );
	return mzTol;
}



