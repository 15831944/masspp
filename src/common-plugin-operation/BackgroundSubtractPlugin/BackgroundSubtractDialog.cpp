/**
 * @file BackgroundSubtractDialog.cpp
 * @brief implements of BackgroundSubtractDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BackgroundSubtractDialog.h"
#include "BackgroundSubtractManager.h"
#include "BackgroundSubtraction.h"
#include "BackgroundOperation.h"
#include "BgSubScrollWin.h"

using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP				 3
#define BORDER_SIZE				 3
#define ID_SUBTRACTION			 4

#define OPERATION_NAME			"Background Subtract"
#define OPERATION_SHORT_NAME	"background_subtract"

BEGIN_EVENT_TABLE( BackgroundSubtractDialog, kome::window::StandardDialog )
	EVT_BUTTON( ID_SUBTRACTION, BackgroundSubtractDialog::onSubtraction )
END_EVENT_TABLE()


// constructor
BackgroundSubtractDialog::BackgroundSubtractDialog( wxWindow* parent ) 
		: kome::window::StandardDialog( parent, "Background Subtract" ) {
	m_scrWin  = NULL;
	m_tmpSpec = NULL;
}

// destructor
BackgroundSubtractDialog::~BackgroundSubtractDialog() {
	if( m_scrWin != NULL ){
		delete m_scrWin;
	}
}

// create main sizer
wxSizer* BackgroundSubtractDialog::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	int isel = -1;
	if( m_scrWin != NULL ){
		isel = m_scrWin->getSelRadioBtnNo();
		delete m_scrWin;
	}
	m_scrWin = NULL;
	m_scrWin = new BgSubScrollWin(this, isel);
	
	wxSize size = m_scrWin->GetSize();
	sizer->SetMinSize( size );
	
	sizer->Add( m_scrWin, 1, wxALL | wxGROW, BORDER_SIZE );


	return sizer;
}

// create button sizer
wxSizer* BackgroundSubtractDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	// subtraction button
	wxButton* button = new wxButton( this, ID_SUBTRACTION, wxT( "Subtract" ) );
	buttonSizer->Add( button, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	button = new wxButton( this, wxID_CANCEL, wxT( "Cancel" ) );
	buttonSizer->Add( button, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// on OK
bool BackgroundSubtractDialog::onOK() {
	// unset background spectrum
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	mgr.setBackground( NULL );

	// refresh
	kome::window::WindowTool::refresh();
	return kome::window::StandardDialog::onOK();
}

// on cancel
bool BackgroundSubtractDialog::onCancel() {
	// unset background spectrum
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	mgr.setBackground( NULL );
	
	// refresh
	kome::window::WindowTool::refresh();

	// default
	return kome::window::StandardDialog::onCancel();
}

// background subtraction
void BackgroundSubtractDialog::onSubtraction( wxCommandEvent& evt ) {
	// managers
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	
	BackgroundSubtraction* bs = mgr.getCurrentBackgroundSubtraction();
	if( bs == NULL ) {
		LOG_ERROR( FMT("The background spectrum is not specified yet.") );
		return;
	}
	
	// background subtraction
	wxBeginBusyCursor();
			
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		// get canvas
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
		if( canvas != NULL ) {
			int style = canvas->getCanvasStyle();
			//singleのSpectrumかどうか判定
			if( style == 0 && canvas->getActiveObject().type == kome::objects::Variant::SPECTRUM ){ 
				kome::objects::Spectrum* spec = NULL;
				// キャンバスのスペクトル
				spec = canvas->getSpectrum()->getOrgSpectrum();
				if( spec == NULL ){
					LOG_ERROR( FMT("This operation requires an active spectrum window.") );
					return;
				}
				// backgroundに指定されているスペクトル以外に処理を行なう
				if( spec != mgr.getSpectrum()->getOrgSpectrum() ){
					// ----------------
					// Operation 作成
					// ----------------
					BackgroundOperation* opt = new BackgroundOperation();
					opt->setTargetSample( spec->getSample() );
					opt->setOperationName( OPERATION_NAME );
					opt->setShortName( OPERATION_SHORT_NAME );
					opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
					opt->setSpec( spec );
					opt->setBackgroundSubtraction( bs );
					opt->setBackgroundSpec( mgr.getSpectrum() );

					// execute
					if( opt->execute() ){
						kome::window::WindowTool::refresh();
					}else{
						delete opt;
					}
				}
			}
		}
	}
	
	wxEndBusyCursor();
	
	kome::window::StandardDialog::onOkButton( evt );  // SPEC 82317 : Background Subtractoin doesn @date 2011.09.01 <Add> M.Izumi
	
}

// ダイアログにSpectrumCanvasを追加
void BackgroundSubtractDialog::AddSpectrumCanvas(){
	m_tmpSpec = NULL;
	SetSizer( createMainSizer(), true );
	
	// refresh
	kome::window::WindowTool::refresh();
}

// ダイアログから一致したSpectrumCanvasを削除
void BackgroundSubtractDialog::closeCanvas( kome::objects::Spectrum* spec ){
	m_tmpSpec = spec;
	
	SetSizer( createMainSizer(), true );
	// refresh
	kome::window::WindowTool::refresh();	
}

// 削除するSpctrum取得
kome::objects::Spectrum* BackgroundSubtractDialog::getTmpSpec(){
	return m_tmpSpec;
}

