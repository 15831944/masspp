/**
 * @file XYDataHugePanel.cpp
 * @brief interfaces of plug-in functions
 *
 * @author fujita
 * @date 2012.04.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "XYDataHugePanel.h"

#include <boost/bind.hpp>

#include "XYDataViewManager.h"   //Managerクラスをincludeするの？
#include "XYData2DListCtrl.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE						 3
#define GRID_GAP						 3

#define ID_LIST							 1
#define LISTBOX_HEIGHT					200

#define ID_EVALFUNC_COMBO				 1

BEGIN_EVENT_TABLE( XYDataHugePanel, kome::window::StandardPanel )
	EVT_COMBOBOX( ID_EVALFUNC_COMBO, XYDataHugePanel::onEvalFuncComboBox )
END_EVENT_TABLE()



enum peak_kind {     // for m_peaksFlg用
	FLG_DATA,
	FLG_PEAK,
	FLG_2DPEAK
};

// constructor
XYDataHugePanel::XYDataHugePanel( wxWindow* parent, const bool peaksFlg ) : kome::window::StandardPanel( parent, wxID_ANY ) {    //StandardPanel::StandardPanel( wxWindow* parent, const int id )
	// initialize
//	m_dataXYData = NULL;
//	m_peaksXYData = NULL;
//	m_peaksFlg = FLG_DATA;
	m_EvalFuncFlag = 0;
	
	m_peaksFlg = peaksFlg;

	if ( m_peaksFlg == false ){
		m_dataXYData = new XYData2DListCtrl( this );   //thisを親としてXYData2DListCtrlのインスタンス生成
		createControls();   //StadardPanelクラスのメソッド.中でCreateMainSizerメソッドを呼ぶ.
	} else if ( m_peaksFlg == true ){
		m_peaksXYData = new XYData2DListCtrl( this );   //thisを親としてXYData2DListCtrlのインスタンス生成
		createControls();   //StadardPanelクラスのメソッド.中でCreateMainSizerメソッドを呼ぶ.
	}
//	createControls();   //StadardPanelクラスのメソッド.中でCreateMainSizerメソッドを呼ぶ.

/*	
	m_group = NULL;
*/                       
	// create list
//	create2D();
};

// destructor
XYDataHugePanel::~XYDataHugePanel() {
};

// creates main sizer
wxSizer* XYDataHugePanel::createMainSizer() {

	// main sizer
	if ( m_peaksFlg == false ){    //こちらはBoxSizerでよい
//		wxStaticBoxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Peak List" ));//wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
		wxBoxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, this);
		sizer->Add( m_dataXYData, 1, wxALL | wxGROW, BORDER_SIZE );
		return sizer;
	}
	else
	{   //if ( m_peaksFlg == true ){
		wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
		wxStaticBoxSizer* PeakListsizer = new wxStaticBoxSizer( wxHORIZONTAL, this, wxT( "Peak List" ));//wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
		sizer->Add( PeakListsizer, 1, wxALL | wxGROW, BORDER_SIZE );
		PeakListsizer->Add( m_peaksXYData, 1, wxALL | wxGROW, BORDER_SIZE );

		wxStaticBoxSizer* EvaluationFunctionsizer = new wxStaticBoxSizer( wxHORIZONTAL, this, wxT( "Evaluation Function" ));
		// list box
//		m_list = new wxListBox( this, ID_LIST, wxDefaultPosition, wxSize( -1, LISTBOX_HEIGHT ) );
//		sizer->Add( m_list, 1, wxALL | wxGROW, BORDER_SIZE );
		sizer->Add( EvaluationFunctionsizer, 1, wxALL | wxGROW, BORDER_SIZE );
		// combo
		wxString EvalFuncTypes[] = {
			wxT( "Peak_Multiplication" ),
			wxT( "Peak_Addition" ),
			wxT( "Peak_Division" ),
		};
		m_EvalFuncCombo = new wxComboBox(
			this,
			ID_EVALFUNC_COMBO,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			sizeof( EvalFuncTypes ) / sizeof( wxString ),
			EvalFuncTypes,
			wxCB_SIMPLE | wxCB_READONLY
		);

		EvaluationFunctionsizer->Add( m_EvalFuncCombo, 1, wxALL | wxGROW, BORDER_SIZE );
		m_EvalFuncCombo->SetSelection( 0 );    //初期表示をPeak_Multiplicationに設定


		return sizer;
	}

 // update
//	update();   //無くてもよい
		
//	return sizer;
}

// set spectrum
void XYDataHugePanel::setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg ){

	if ( false == peaksFlg ){
		m_dataXYData->setData( spec, xyData, peaksFlg );
	} else { // true == peaksFlg
		m_peaksXYData->setData( spec, xyData, peaksFlg );
	}
}

// set chromatogram
void XYDataHugePanel::setData(  kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg  ){
	
	if ( false == peaksFlg ){
		m_dataXYData->setData( chrom, xyData, peaksFlg );
	} else { //true == peaksFlg
		m_peaksXYData->setData( chrom, xyData, peaksFlg );
	}

}

// set group
void XYDataHugePanel::setGroup( kome::objects::DataGroupNode* group ){
	
	    m_peaksXYData->setGroup( group );

}

// Event of EvalFuncComboBox ( get m_EvalFuncFlag )
void XYDataHugePanel::onEvalFuncComboBox( wxCommandEvent& evt ){

	m_EvalFuncFlag = m_EvalFuncCombo->GetSelection();

//	m_peaksXYData->setData( spec, xyData, peaksFlg );

	kome::objects::Spectrum* spec = NULL;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );

		spec = obj.getSpectrum();
	}

	XYDataViewManager& mgr = XYDataViewManager::getInstance();
	//set evaluation function flag
	mgr.setEvalFuncFlg(m_EvalFuncFlag);
	// set object
	mgr.selectSpectrum( spec );

}

// get m_EvalFuncFlag
int XYDataHugePanel::getEvalFuncFlag(){

	return m_EvalFuncFlag;

}
