/**
 * @file IdentificationDialogBase.cpp
 * @brief implementsation of IdentificationDialogBase class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "IdentificationDialogBase.h"        
#include "SpectrumGetter.h"
#include "IdentificationPluginManager.h"
#include "SearchResultListCtrl.h"
#include "SearchResultDialog.h"


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


BEGIN_EVENT_TABLE( IdentificationDialogBase, kome::window::ParametersAndHelpDialog )
    EVT_KOME_SETTINGS_PANEL( wxID_ANY, IdentificationDialogBase::onSettings )
END_EVENT_TABLE()


// constructor
IdentificationDialogBase::IdentificationDialogBase( wxWindow* pParent, const char* title )
		: kome::window::ParametersAndHelpDialog( pParent, title, "Identify" ), m_params( NULL, false ) {
	// initialize
	m_combo = NULL;
}

// destructor
IdentificationDialogBase::~IdentificationDialogBase() {
}

// get title
std::string IdentificationDialogBase::getTitle() {
	const char* s = m_settings.getParameterValue( TITLE_PARAM_NAME );
	std::string title = NVL( s, "" );

	return title;
}

// get comment
std::string IdentificationDialogBase::getComment() {
	const char* s = m_settings.getParameterValue( COMMENT_PARAM_NAME );
	std::string comment = NVL( s, "" );

	return comment;
}

// get search settings
kome::objects::SettingParameterValues* IdentificationDialogBase::getSearchSettings() {
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
kome::plugin::PluginFunctionItem* IdentificationDialogBase::getSearchItem() {
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
kome::plugin::PluginFunctionItem* IdentificationDialogBase::getPeakDetector() {
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
kome::objects::SettingParameterValues* IdentificationDialogBase::getPeakDetectSettings() {
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
kome::plugin::PluginFunctionItem* IdentificationDialogBase::getChargeDetector() {
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
kome::objects::SettingParameterValues* IdentificationDialogBase::getChargeDetectSettings() {
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
std::string IdentificationDialogBase::getPeakFilter() {
	// filter
	const char* s = m_settings.getParameterValue( PEAK_FILTER_PARAM_NAME );
	std::string filter = NVL( s, "" );

	return filter;
}

// get spectrum data manipulation
void IdentificationDialogBase::getSpecDataManipulat( 
	std::vector< kome::plugin::PluginFunctionItem* >& items,
	std::vector< kome::objects::SettingParameterValues >& settingVals
){

	std::string val;
	bool bflg = false;
	for( unsigned int i=0; i < m_settings.getNumberOfParameters() && !bflg; i++ ){
		std::string strName = m_settings.getParameterName(i);
		if( strName.compare( "data_manipulate" ) == 0 ){
			val = m_settings.getParameterValue( strName.c_str() );
			bflg = true;
		}
	}
		
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	std::vector< std::string > arry;
	kome::plugin::SettingsValue::separateListValue( val.c_str(), arry );


	for( unsigned int j=0; j < arry.size(); j++ ){
		std::string name = ( arry[j].c_str() );

		int ifind = name.find_first_of( "[" );
		std::string strItem = name.substr( 0, ifind );
		std::string strParam = name.substr( ifind );

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "spec_manipulation", strItem.c_str() );
		
		if( item != NULL ){
			kome::objects::SettingParameterValues manipulatSetting;
			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ){
				page->setParameterString( manipulatSetting, strParam.c_str() );	
			}
			
			items.push_back( item );
			settingVals.push_back( manipulatSetting );
		}
	}
}

// get search engine
kome::ident::SearchEngine* IdentificationDialogBase::getSearchEngine() {
	// function
	kome::plugin::PluginFunctionItem* item = getSearchItem();
	if( item == NULL ) {
		return NULL;
	}

	// search engine
	kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)item->getCall()->invoke( NULL ).prim.pt;

	return engine;
}


// create search sizer
wxSizer* IdentificationDialogBase::createSearchSizer() {
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
	form->setDesc( "The title of identification job. This title is shown in the Search Engine Results dialog." ); // @date 2014.01.23 <Add> M.Izumi

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
	form->setDesc( "The comment of identification job. This comment is shown in the Search Engine Results dialog." ); // @date 2014.01.23 <Add> M.Izumi

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
	form->setDesc( "Select the search engine for identification." ); // @date 2014.01.23 <Add> M.Izumi
	m_engineForm = form;

	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create peaks sier
wxSizer* IdentificationDialogBase::createPeaksSizer() {
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
	val->setDefaultValue( "gion" );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( PEAK_DETECTOR_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( PEAK_DETECTOR_PARAM_KEY );
	param->setValue( val );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "Spectrum Peak Detector" );
	form->setParam( param );
	form->setDesc( "Select the algorithm to make peak list for posting to search engine." ); // @date 2014.01.23 <Add> M.Izumi

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
	form->setDesc( "Select the charge calculation method." ); // @date 2014.01.23 <Add> M.Izumi

	// Data Manipulation
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	val->setRequired( false );

	kome::plugin::SettingsValue* childVal0 = m_params.addValue();
	childVal0->setType( kome::plugin::SettingsValue::TYPE_STRING );
	childVal0->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	childVal0->setEnumKey( "spec_manipulation" );
	childVal0->setRequired( true );
	childVal0->setName( "Data Manipulation" );
	
	val->addChildValue( childVal0 );

	param = m_params.addParam();
	param->setName( "data_manipulate" );
	param->setSection( SECTION );
	param->setKey( "DATA_MANIPULATE" );
	param->setValue( val );

	form = page->addForm();
	form->setTitle("Data Manipulation");
	form->setParam( param );
	form->setDesc( "Set basic data manipulation methods before making peak list." ); // @date 2014.01.23 <Add> M.Izumi

	// peak filter
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	val->setRequired( false );

	kome::plugin::SettingsValue* childVal1 = m_params.addValue();
	childVal1->setType( kome::plugin::SettingsValue::TYPE_STRING );
	childVal1->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	childVal1->setEnumKey( PEAK_FILTER_FUNC_TYPE );
	childVal1->setRequired( true );
	childVal1->setName( "Peak Filter" );
	val->addChildValue( childVal1 );

	param = m_params.addParam();
	param->setName( PEAK_FILTER_PARAM_NAME );
	param->setSection( SECTION );
	param->setKey( PEAK_FILTER_PARAM_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setTitle( "Peak Filter" );
	form->setDesc( "Set peak filters for posting to search engine." ); // @date 2014.01.23 <Add> M.Izumi
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

// on settings
void IdentificationDialogBase::onSettings( kome::window::SettingsPanelEvent& evt ) {
	
	// each forms
	if( evt.getForm() == m_engineForm ) {    // search engine
		// window->data
		TransferDataFromWindow();// @date 2012.12.17 <Mod> M.Izumi Up, Down処理がおかしくなるので検索エンジンが変更された時のみ実行

		kome::ident::SearchEngine* engine = getSearchEngine();
		onChangeEngine( engine );
	}
}

// main sizer
wxSizer* IdentificationDialogBase::createMainSizer() { 
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );

	wxFlexGridSizer* leftSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	leftSizer->AddGrowableRow( 0 );
	leftSizer->AddGrowableCol( 0 );
	sizer->Add( leftSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// search sizer
	wxSizer* searchSizer = createSearchSizer();
	leftSizer->Add( searchSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// spectra sizer
	wxSizer* spectraSizer = createSpectraSizer();
	leftSizer->Add( spectraSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// peaks sizer
	wxSizer* peaksSizer = createPeaksSizer();
	sizer->Add( peaksSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* IdentificationDialogBase::createButtonSizer() {
	// Override this function so that OK and Cancel buttons are not included.
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	buttonSizer->Add( new wxButton( this, wxID_OK, wxT( "Identify" ) ) );
	buttonSizer->Add( new wxButton( this, wxID_CANCEL, wxT( "Close" ) ) );

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// create spectrum getter
SpectrumGetter* IdentificationDialogBase::createSpectrumGetter() {
	return NULL;
}

// create spectra sizer
wxSizer* IdentificationDialogBase::createSpectraSizer() {
	return NULL;
}

// on change selected search engine
void IdentificationDialogBase::onChangeEngine( kome::ident::SearchEngine* engine ) {
}

// transfer data from window
bool IdentificationDialogBase::TransferDataFromWindow() {
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// search engine
	bool ret = true;
	kome::ident::SearchEngine* engine = getSearchEngine();
	kome::objects::SettingParameterValues* settings = getSearchSettings();
	if( engine == NULL ) {
		return false;
	}
	if( settings != NULL ) {
		std::string msg = engine->validateSettings( settings );
		if( !msg.empty() ) {
			kome::window::WindowTool::showError( msg.c_str() );
			ret = false;
		}
	}

	return ret;
}

// prepare for the DB search
bool IdentificationDialogBase::prepare() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// dialog progress
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Identification" );

	// search engines
	const unsigned int num = plgMgr.getNumberOfFunctionItems( IDENTIFICATION_FUNC_TYPE );
	if( num == 0 ) {
		kome::window::WindowTool::showError( "No search engine plug-ins are not installed.\nExist Mass++ and install at least one search engine plug-in." );
		return false;
	}
	progress.createSubProgresses( num );

	bool flg = true;
	for( unsigned int i = 0; i < num && !progress.isStopped(); i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, i );
		kome::core::Progress* prgs = progress.getSubProgress( i );

		if( item != NULL ) {
			progress.setStatus( FMT( "Preparation for %s ... [%d/%d]", item->getLongName(), ( i + 1 ), num ).c_str() );
			kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)( item->getCall()->invoke( NULL ).prim.pt );
			flg = engine->prepareSearch( *prgs );
			if( !flg ) {
				kome::window::WindowTool::showError( getlasterrormessage() );
			}
		}
		prgs->fill();
	}

	return !( progress.isStopped() );
}

// identification
bool IdentificationDialogBase::identify( IdentificationDialogBase* dlg, kome::objects::Sample* sample ) {
	// check the parameter
	if( dlg == NULL ) {
		LOG_ERROR( FMT( "Failed to get the dialog object." ) );
		return false;
	}

	// dialog progress
	kome::window::DialogProgress* progress = new kome::window::DialogProgress( kome::window::WindowTool::getMainWindow(), "Identification" );

	// >>>>>>	@Date:2013/09/19	<Add/Modified>	A.Ozaki
	// fixed memory leaks
	//
	SpectrumGetter	*pSpectrumGetter = dlg->createSpectrumGetter();

	// Data ManipulationのSettings取得
	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settingVals;
	dlg->getSpecDataManipulat( items, settingVals );

	int searchId = IdentificationPluginManager::identify(
		dlg->getTitle().c_str(),
		dlg->getComment().c_str(),
		dlg->getSearchItem(),
		dlg->getSearchSettings(),
		dlg->getPeakDetector(),
		dlg->getPeakDetectSettings(),
		dlg->getChargeDetector(),
		dlg->getChargeDetectSettings(),
		pSpectrumGetter,
		dlg->getPeakFilter().c_str(),
		progress,
		items,
		settingVals
	);

	bool ret = ( searchId >= 0 );

	if  ( (SpectrumGetter *)NULL != pSpectrumGetter )
	{
		delete	pSpectrumGetter;
	}

	if  ( (kome::window::DialogProgress *)NULL != progress )
	{
		progress->fill( );
		delete	progress;
	}
	// <<<<<<	@Date:2013/09/19	<Add/Modified>	A.Ozaki

	// open dialog
	kome::ident::SearchResultDialog* resultDlg = new kome::ident::SearchResultDialog(
		kome::window::WindowTool::getMainWindow(),
		"Search Engine Results",
		true
	);
		
	SearchResultListCtrl* listCtrl = new SearchResultListCtrl( resultDlg );
	resultDlg->setGridListCtrl( listCtrl );
	resultDlg->clearStatus();
		
	resultDlg->Show();
	SearchResultListCtrl::m_dlg = resultDlg;

	return ret;
}
