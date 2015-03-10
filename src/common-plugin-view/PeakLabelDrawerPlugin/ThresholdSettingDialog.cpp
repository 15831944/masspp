/**
 * @file ThresholdSettingDialog.cpp
 * @brief implements of ThresholdSettingDialog class
 *
 * @author M.Izumi
 * @date 2011.03.29
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "ThresholdSettingDialog.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					 3
#define STRING_EDIT_WIDTH			 100
#define ID_THRESHOLD_TEXT	  	  	 1

BEGIN_EVENT_TABLE( ThresholdSettingDialog, kome::window::StandardDialog )

END_EVENT_TABLE()

// constructor
ThresholdSettingDialog::ThresholdSettingDialog( wxWindow* parent )
	: kome::window::StandardDialog( parent, "Threshold Setting" ){
		m_ThresholdText = NULL;
		m_Threshold = -1.0;
}

// destructor
ThresholdSettingDialog::~ThresholdSettingDialog(){
}

// create Main Sizer
wxSizer* ThresholdSettingDialog::createMainSizer(){
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 1, 2, 5 ); 
    mainSizer->AddGrowableRow( 0 );
    mainSizer->AddGrowableCol( 0 );
	
	// edit sizer
	wxFlexGridSizer* editSizer = new wxFlexGridSizer( 2, 1, 5 );
		
	// text
	m_ThresholdText = new wxTextCtrl(
		this,
		ID_THRESHOLD_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 )
	);
	editSizer->Add(
		m_ThresholdText,
		1,
		wxALIGN_CENTER_VERTICAL | wxGROW,
		BORDER_SIZE
	);
	mainSizer->Add( editSizer, 1, wxALL | wxGROW, 5 );
	return mainSizer;
}

// transfer data from window
bool ThresholdSettingDialog::TransferDataFromWindow(){
    // default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	if( m_ThresholdText == NULL || m_ThresholdText->GetValue().IsEmpty() ){
		return false;
	}

	double d = 1.0;
	// ƒ_ƒuƒ‹Œ^‚©‚Ç‚¤‚©
	if( isdouble( m_ThresholdText->GetValue().c_str(), &d )){
		// set threshold
		setThreshold( d );
	}else{
		std::string msg = "Scaling must be a numeric value.";
		kome::window::WindowTool::showInfo( msg.c_str() );
		return false;
	}
	
	return true;
}

// transfer data to window
bool ThresholdSettingDialog::TransferDataToWindow(){
	double d = getThreshold();
	m_ThresholdText->SetValue( FMT("%.2f",d).c_str() );
	return true;
}

// set threshold
void ThresholdSettingDialog::setThreshold( double d ){
	m_Threshold = d;
}

// get threshold
double ThresholdSettingDialog::getThreshold(){
	double d = 0.0;
	if( m_Threshold >= 0.0 ){
		return m_Threshold;
	}

	return d;
}
