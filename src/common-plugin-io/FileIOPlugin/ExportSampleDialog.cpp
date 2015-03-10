/**
 * @file ExportSampleDialog.cpp
 * @brief interfaces of FileIOManager class
 *
 * @author M.Izumi
 * @date 2011.7.19 
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ExportSampleDialog.h"
#include "FileIOManager.h"

#include <boost/algorithm/string.hpp>
#include <string>

using namespace kome::io;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE					3
#define GRID_GAP					3

#define ID_RADIO_BOX				1

#define CURRENT_SPECTRUM			"Current Spectrum"
#define CURRENT_CHROMATOGRAM		"Current Chromatogram"
#define CURRENT_DATA				"Current Data"
#define FILTERED_SPECTRA			"Filtered Spectra"
#define SAMPLE						"Sample"
#define FILE_SAVE_FUNC_TYPE_NAME	"SAVE_FILE"
#define EXT_PARAM_NAME				"ext"
#define TARGET_PARAM_NAME           "target"
#define SECTION                     "file IO "
#define FILE_IO_KEY                 "file IO key"

#define LABELING_NAME				"Labeling"
#define FORMAT                      "Format"


BEGIN_EVENT_TABLE( ExportSampleDialog, kome::window::ParametersAndHelpDialog )
	EVT_RADIOBOX( ID_RADIO_BOX, ExportSampleDialog::onRadio )
	EVT_KOME_SETTINGS_PANEL( wxID_ANY, ExportSampleDialog::onSettingsPanel )
END_EVENT_TABLE()


// constructor 
ExportSampleDialog::ExportSampleDialog( wxWindow* parent, kome::objects::Variant acObj )
		: kome::window::ParametersAndHelpDialog( parent, "Export Sample" )
		, m_parameters( NULL, false ){
	// initialize
    m_target = TARGET_SPECTRUM;
	m_item = NULL; 
	m_obj = acObj;
	m_specFlg = false;
	m_chromFlg = false;

	// make the array
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	unsigned int num = plgMgr.getNumberOfFunctionItems( FILE_SAVE_FUNC_TYPE_NAME );
	for( unsigned int i = 0; i < num; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, i );
		std::string target = tolowercase( item->getCall()->getProperties().getStringValue( TARGET_PARAM_NAME, "" ) );

		m_items.resize( m_items.size() + 1 );
		m_items.back().item = item;

		if( target.compare( "spectrum" ) == 0 ) {
			m_items.back().target = TARGET_SPECTRUM;
		}
		else if( target.compare( "filtered" ) == 0 ) {
			m_items.back().target = TARGET_FILTERED;
		}
		else if( target.compare( "sample" ) == 0 ) {
			m_items.back().target = TARGET_SAMPLE;
		}
		else if( target.compare( "chromatogram" ) == 0 ) {	// @date 2011/11/28 <Add> OKADA
			m_items.back().target = TARGET_CHROMATOGRAM;	// @date 2011/11/28 <Add> OKADA
		}													// @date 2011/11/28 <Add> OKADA
		// SPEC 83703
		else if( target.compare( "spectra" ) == 0 ) {	// @date 2012/04/25 <Add> OKADA
			m_items.back().target = ( TARGET_SPECTRUM | TARGET_FILTERED | TARGET_SAMPLE  );	// @date 2012/04/25 <Add> OKADA
		}												// @date 2012/04/25 <Add> OKADA
		else {
			m_items.back().target = ( TARGET_SPECTRUM | TARGET_FILTERED | TARGET_SAMPLE | TARGET_CHROMATOGRAM );	// @date 2011/11/28 <Add> OKADA
		}
	}
}

// destructor
ExportSampleDialog::~ExportSampleDialog(){
}

wxSizer* ExportSampleDialog::createMainSizer(){
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// radio button
	wxString targets[] ={
		_T(CURRENT_SPECTRUM),
		_T(FILTERED_SPECTRA),
		_T(SAMPLE)
	};
	
	// @date 2011/11/29 <Add> OKADA ------->
	// targets[]文字列の初期化
	kome::objects::MsDataVariant var( m_obj );
	kome::objects::Sample* sample = var.getSample();
	kome::objects::Spectrum* spec = var.getSpectrum();
	kome::objects::Chromatogram* chrom = var.getChromatogram();
	
	if( spec != NULL ){
		targets[0] = _T(CURRENT_SPECTRUM);
		m_specFlg = true;
	}else if( chrom != NULL ){
		targets[0] = _T(CURRENT_CHROMATOGRAM);
		m_chromFlg = true;
	}else{
		targets[0] = _T(CURRENT_DATA);
	} // @date 2011/11/29 <Add> OKADA <-------

	m_RadioBox = new wxRadioBox(
		this,
		ID_RADIO_BOX,
		wxT("Target"),
		wxDefaultPosition,
		wxDefaultSize,
		sizeof( targets ) /sizeof( wxString ),
		targets,
		0,
		wxRA_SPECIFY_ROWS
	);

	if(( spec == NULL )&&( chrom == NULL )){
		m_RadioBox->Enable(0, false);
	}

	kome::objects::DataSet* dataSet = sample->getRootDataGroupNode();
	// SPEC No.85583 For a data file with only chro data, Filtered Spectra should be disabled in Export Samples dialog 
	// @date 2012.04.18 <Mod> M.Izumi
	if( dataSet->getNumberOfSpectra() == 0 ){
		m_RadioBox->Enable(1, false);
	}
	
	// @date 2011.09.07 <Add> M.Izumi<-

	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// target
	bool btarget = false;
	int target = ini->getInt(  EXPORT_SECTION, TARGET_KEY, 0 );
	if( target == (int)TARGET_FILTERED ) {		// filtered spectra
		m_target = TARGET_FILTERED;
		if( m_RadioBox->IsItemEnabled( 1 ) ){
			m_RadioBox->SetSelection( 1 );
			btarget = true;
		}
	}
	else if( target == (int)TARGET_SAMPLE ) {	// sample
		m_target = TARGET_SAMPLE;
		if( m_RadioBox->IsItemEnabled( 2 ) ){
			m_RadioBox->SetSelection( 2 );
			btarget = true;
		}
	}
	else if(( target == (int)TARGET_SPECTRUM )||( target == (int)TARGET_CHROMATOGRAM )){	// spectrum 
		if(targets[0] == _T(CURRENT_SPECTRUM)){
			m_target = TARGET_SPECTRUM;
		}else if( targets[0] == _T(CURRENT_CHROMATOGRAM) ){
			m_target = TARGET_CHROMATOGRAM;	
		}
		if( m_RadioBox->IsItemEnabled( 0 ) ){							// @date 2011/11/30 <Add> OKADA
			m_RadioBox->SetSelection( 0 );								// @date 2011/11/30 <Add> OKADA
			btarget = true;												// @date 2011/11/30 <Add> OKADA
		}																// @date 2011/11/30 <Add> OKADA
	}
	
	// Disable時の選択位置 @date 2011.09.08 <Add> M.Izumi
	if( !btarget ){
		m_target = TARGET_SAMPLE;	// @date 2011/12/08 <Add> OKADA
		m_RadioBox->SetSelection( 2 );
	}

	setDescription( m_RadioBox, "Select target objects to be exported.\n"
"[Sample]\n"
"  All spectra in the current sample.\n"
"[Filtered Spectra]\n"
"  Filtered spectra in the current sample.\n"
"[Current Spectrum]\n"
"  Export only current spectrum\n"
"[Current Chromatogram]\n"
"  Export only current chromatogram" );
	sizer->Add( m_RadioBox, 1, wxGROW | wxALL, 0 );
	sizer->AddSpacer(10);

	//コンボボックス設定
	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );

	// page	                                                                                //@date 2012/8/6 <Add> FUJITA
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );      //@date 2012/8/6 <Add> FUJITA

	kome::plugin::SettingsValue* value = m_parameters.addValue();    
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );  
	value->setEnumKey( FILE_SAVE_FUNC_TYPE_NAME );                       
	value->setRequired( true );  
	
	kome::plugin::SettingsParam* param = m_parameters.addParam();  
	param->setName( FORMAT );  
	param->setSection( SECTION );          
	param->setKey( FILE_IO_KEY );          
	param->setValue( value );
	
	kome::plugin::SettingsForm* form = page->addForm();   
	form->setParam( param );
	form->setTitle( FORMAT );
	form->setDesc( "Select export format." );

	updateRadios(); // @date 2014.02.26 <Add> M.Izumi

	//settingpanel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(this,page,false,&m_settings,true);
	comboSizer->Add( panel, 0, wxALL | wxGROW, BORDER_SIZE );
	
	sizer->Add( comboSizer, 0, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// creat Button Sizer
wxSizer* ExportSampleDialog::createButtonSizer(){
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	// add Export button
	buttonSizer->Add(
		new wxButton( this, wxID_OK, wxT( "Export" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
		
	// Cancel button
	buttonSizer->Add( new wxButton( this, wxID_CANCEL, wxT( "Cancel" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_RIGHT,
		BORDER_SIZE
	);

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );
	return sizer;
}

// set members
void ExportSampleDialog::setMembers() {
	// target
	int sel = m_RadioBox->GetSelection();
	if( sel == 1 ) {
		m_target = TARGET_FILTERED;
	}
	else if( sel == 2 ) {
		m_target = TARGET_SAMPLE;
	}
	else {
		m_target = TARGET_SPECTRUM;

		kome::objects::MsDataVariant obj( m_obj );
		kome::objects::Spectrum* spec = obj.getSpectrum();
		kome::objects::Chromatogram* chrom = obj.getChromatogram();
		if(( spec == NULL )&&( chrom != NULL )){
			m_target = TARGET_CHROMATOGRAM;
		}
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const char* save_format = m_settings.getParameterValue( FORMAT );
	m_item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, save_format );
	
	// values on child panel
	m_saveSettings = m_settings.getSubParameterValues( FORMAT, save_format );

	kome::io::FileIOManager& mgr = kome::io::FileIOManager::getInstance();      // @date 2012/8/20 <Add> FUJITA
	mgr.setSaveSettings(m_saveSettings);                                        // @date 2012/8/20 <Add> FUJITA

}

// get target
ExportSampleDialog::ExportTarget ExportSampleDialog::getTarget() {
	return m_target;
}

// get format
kome::plugin::PluginFunctionItem* ExportSampleDialog::getSelectedItem() {
	return m_item;
}

// transfer data from window
bool ExportSampleDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// set
	setMembers();

	// save
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	ini->setInt( EXPORT_SECTION, TARGET_KEY, (int)m_target );
	ini->setString( EXPORT_SECTION, FORMAT_KEY, ( m_item == NULL ? "" : m_item->getShortName() ) );

	// @date 2014.03.11 <Add> M.Izumi ->
	int icount = m_RadioBox->GetCount();
	bool bflg = false;
	for( int i=0; i < icount; i++ ){
		if( m_RadioBox->IsItemEnabled( i ) == true ){
			bflg = true;
			break;
		}
	}
	if( !bflg ){
		kome::window::WindowTool::showError( "This format does not support the specified target." );
		return false;
	}
	// @date 2014.03.11 <Add> M.Izumi
	
	return true;
}   

// on settings panel
void ExportSampleDialog::onSettingsPanel( kome::window::SettingsPanelEvent& evt ){
	updateRadios();
}

// update radios
bool ExportSampleDialog::updateRadios() {
	// default
	if( !kome::window::ParametersAndHelpDialog::TransferDataFromWindow() ) {
		return false;
	}
	
	// plug-in item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const char* save_format = m_settings.getParameterValue( FORMAT );
	// @date 2014.03.12 <Add> M.Izumi -> 初期設定
	if( save_format == NULL ){
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();
		save_format = ini->getString( EXPORT_SECTION, FORMAT_KEY, "" );

		m_settings.setValue( FORMAT, save_format );
	}
	// @date 2014.03.12 <Add> M.Izumi <-
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "save_file", save_format );

    // targets
	std::set< std::string > targetSet;

	if( item != NULL ) {
		std::string targets = item->getCall()->getProperties().getStringValue( "target", "" );
		std::vector< std::string > tokens;
		stringtoken( targets.c_str(), ",", tokens );

		for( unsigned int i = 0; i < tokens.size(); i++ ) {
			std::string token = trimstring( tokens[ i ].c_str() );
			if( !token.empty() ) {
				targetSet.insert( token );
			}
		}
	}
		
	// update radios
	if( targetSet.empty() || targetSet.find( "sample" ) != targetSet.end() ) {
		m_RadioBox->Enable( 0, true );
		m_RadioBox->Enable( 1, true );
		m_RadioBox->Enable( 2, true );
	}
	else {
	
		bool b0 = false;
		bool b1 = false;
		bool b2 = false;

		if( targetSet.find( "spectrum" ) != targetSet.end() ) {
			if( m_specFlg ) {
				b0 = true;
			}
		}
		if( targetSet.find( "chromatogram" ) != targetSet.end() ) {
			if( m_chromFlg ) {
				b0 = true;
			}
		}
		if( targetSet.find( "spectra" ) != targetSet.end() ) {
			if( m_specFlg ) {
				b0 = true;
			}
			
			b1 = true;
			b2 = true;
		}
		
		m_RadioBox->Enable( 0, b0 );
		m_RadioBox->Enable( 1, b1 );
		m_RadioBox->Enable( 2, b2 );		
	}

	int isel = m_RadioBox->GetSelection();

	if( m_RadioBox->IsItemEnabled( isel ) == false ){
		// 優先順位 1.選択されているもの 2.Sample 3.Current 4.Filtered Spectra
		if( m_RadioBox->IsItemEnabled( 2 ) != false ){
			isel = 2;
		}else if( m_RadioBox->IsItemEnabled( 0 ) != false ){
			isel = 0;
		}else if( m_RadioBox->IsItemEnabled( 1 ) != false ){
			isel = 1;
		}
	}

	m_RadioBox->SetSelection( isel );

	return true;
}
// @date 2013.03.25 <Add> M.Izumi <-

// on radio
void ExportSampleDialog::onRadio( wxCommandEvent& evt ) {
	// members
	setMembers();
}  
