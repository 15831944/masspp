/**
 * @file SampleMergeDialog.cpp
 * @brief implements of SampleMergeDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleMergeDialog.h"

using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE			3
#define GRID_GAP			3

#define SECTION				"Sample Merge"
#define RT_TOL_PARAM_KEY	"RT_TOLERANCE"
#define RT_TOL_PARAM_NAME	"RT Tol"
#define MZ_TOL_PARAM_KEY    "MZ_TOLERANCE"
#define MZ_TOL_PARAM_NAME   "m/z Tol"


BEGIN_EVENT_TABLE( SampleMergeDialog, kome::window::StandardDialog )
END_EVENT_TABLE()


// constructor
SampleMergeDialog::SampleMergeDialog( wxWindow* parent )
		: kome::window::StandardDialog( parent, "Average Samples", "Average" ), m_params( NULL, false ) {
	m_groupList = NULL;
	m_rtTol = 0.5;
	m_mzTol = 0.5;
}

// destructor
SampleMergeDialog::~SampleMergeDialog() {
}

// get RT tolerance
double SampleMergeDialog::getRtTol() {
	return m_rtTol;
}

// get m/z tolerance
double SampleMergeDialog::getMzTol() {
	return m_mzTol;
}

// get the number of samples
unsigned int SampleMergeDialog::getNumberOfSamples(){
	//return m_selectedGroups.size();
	return m_selectedSamples.size();
}

// get sample
kome::objects::Sample* SampleMergeDialog::getSample( const unsigned int idx ){
	
	if( idx >= m_selectedSamples.size() ){
		return NULL;
	}
	return m_selectedSamples[ idx ];
}

// create main sizer
wxSizer* SampleMergeDialog::createMainSizer() {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );

	// group list
	m_groupList = new wxCheckListBox(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxSize( 150, 120 ),
		0, 
		NULL,
		wxLB_HSCROLL | wxLB_NEEDED_SB
	);

	// 表示名変更 [番号]サンプル名-ファイル名 @date 2013.07.11 <Mod> M.Izumi ->
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		std::string name;
				
		name.append( FMT( "[%d]", i+1 ) );
		name.append( ( sample != NULL ? sample->getName() : "" ) );
		name.append( " - " );
		name.append( ( sample->getSampleSet() != NULL ? sample->getSampleSet()->getFileName() : "" ) );
	
		m_samples.push_back( sample );
		m_groupList->Append( wxT( name.c_str() ) );
	}
	// @date 2013.07.11 <Mod> M.Izumi

	sizer->Add( m_groupList, 1, wxGROW | wxALL, BORDER_SIZE );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );

	// RT Tol
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setDefaultValue( "0.5" );
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );
	
	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setSection( SECTION );
	param->setKey( RT_TOL_PARAM_KEY );
	param->setName( RT_TOL_PARAM_NAME );
	param->setValue( val );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( RT_TOL_PARAM_NAME );

	// m/z Tol
	val = m_params.addValue();
	val->setDefaultValue( "0.05" );
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );
	
	param = m_params.addParam();
	param->setSection( SECTION );
	param->setKey( MZ_TOL_PARAM_KEY );
	param->setName( MZ_TOL_PARAM_NAME );
	param->setValue( val );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( MZ_TOL_PARAM_NAME );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings
	);
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool SampleMergeDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// set groups
	m_selectedSamples.clear();
	for( unsigned int i = 0; i < m_groupList->GetCount(); i++ ) {
		if( m_groupList->IsChecked( i ) ) {
			m_selectedSamples.push_back( m_samples[i] );
		}
	}

	// set RT tolerance
	m_rtTol = m_settings.getDoubleValue( RT_TOL_PARAM_NAME, m_rtTol );
	m_mzTol = m_settings.getDoubleValue( MZ_TOL_PARAM_NAME, m_mzTol );

	// check the sample count
	if( getNumberOfSamples() <= 1 ){
		std::string error = "Sample Fusion cannot be performed because more than one sample should be selected.";
		LOG_ERROR_CODE( error, ERR_OTHER );
		kome::window::WindowTool::showError( error.c_str() );
		return false;
	}

	return true;
}
