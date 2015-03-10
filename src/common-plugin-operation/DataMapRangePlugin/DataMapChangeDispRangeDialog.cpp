/**
 * @file DataMapChangeDispRangeDialog.cpp
 * @brief interface of DataMapChangeDispRangeDialog class
 * @author M.Izumi
 * @date 2013.02.04
 *
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "DataMapChangeDispRangeDialog.h"
#include "DataMapRangeManager.h"

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE				 3


BEGIN_EVENT_TABLE( DataMapChangeDispRangeDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()

// constructor
DataMapChangeDispRangeDialog::DataMapChangeDispRangeDialog( wxWindow* parent )
	:  kome::window::ParametersAndHelpDialog( parent, "Change Displayed Range" ), m_params( NULL, false ){

	m_startRt = -1.0;
	m_endRt = -1.0;
	m_startMz = -1.0;
	m_endMz = -1.0;
}

// destructor
DataMapChangeDispRangeDialog::~DataMapChangeDispRangeDialog(){
}

// create main sizer
wxSizer* DataMapChangeDispRangeDialog::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	kome::plugin::SettingsValue* val;	
	kome::plugin::SettingsParam* param;
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	kome::plugin::SettingsForm* form;
	
	// m/z
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );

	param = m_params.addParam();
	param->setName( "rt_range" );
	param->setValue( val );
	
	form = page->addForm();
	form->setParam( param ); 
	form->setTitle( "RT" );
	form->setDesc( "Set the RT range." ); // @date 2014.01.16 <Add> M.Izumi

	param = m_params.addParam();
	param->setName( "mz_range" );
	param->setValue( val );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z" );
	form->setDesc( "Set the m/z range." );	// @date 2014.01.16 <Add> M.Izumi

	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, false );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool DataMapChangeDispRangeDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}
		
	return ( setRange( "mz_range", m_startMz, m_endMz ) && setRange( "rt_range", m_startRt, m_endRt ) ? true : false );
}

// transfer data to window
bool DataMapChangeDispRangeDialog::TransferDataToWindow() {
	
	// set the range	
	std::string mzRange = FMT( "%.4f%s%.4f", m_startMz, RANGE_VALUES_DELIMITER, m_endMz );
	m_settings.setValue( "mz_range", mzRange.c_str() );

	std::string rtRange = FMT( "%.4f%s%.4f", m_startRt, RANGE_VALUES_DELIMITER, m_endRt );
	m_settings.setValue( "rt_range", rtRange.c_str() );
	
	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// set range
bool DataMapChangeDispRangeDialog::setRange( const char* paramName, double& startPos, double& endPos ){
	
	// range
	const char* range = m_settings.getParameterValue( paramName );
	if( range == NULL ){
		return false;
	}

	std::vector< std::string > vals;
	stringtoken( range, RANGE_VALUES_DELIMITER, vals );
	if( vals.size() == 0 ) {
		return false;
	}

	double sPos;
	if( isdouble( vals[ 0 ].c_str(), &sPos ) ) {
		startPos = sPos;
	}
	else {
		return false;
	}

	double ePos;
	if( vals.size() == 1 ) {
		endPos = sPos;
	}
	else if( isdouble( vals[ 1 ].c_str(), &ePos ) ) {
		endPos = ePos;
	}
	else {
		return false;
	}

	return true;
}
