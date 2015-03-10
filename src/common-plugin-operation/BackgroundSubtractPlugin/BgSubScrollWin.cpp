/**
 * @file BgSubScrollWin.cpp
 * @brief interfaces of BgSubScrollWin class
 *
 * @author M.Izumi
 * @date 2011.04.25
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * <補足説明>
 */

#include "stdafx.h"
#include "BgSubScrollWin.h"
#include "BackgroundSubtractManager.h"
#include "BackgroundSubtractDialog.h"


using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP				    3
#define BORDER_SIZE					3

BEGIN_EVENT_TABLE( BgSubScrollWin, wxScrolledWindow )
	EVT_RADIOBUTTON( wxID_ANY, BgSubScrollWin::onSelectRadioButton )
END_EVENT_TABLE()

// constructor
BgSubScrollWin::BgSubScrollWin( wxWindow* parent, int selNo ) 
	:  wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxSize(470,460), wxVSCROLL) {

	m_selRadioBtnNo = selNo;
	m_radioBtn = NULL;

	m_canvas.clear();

	SetSizer( setMainSizer(), true );
	
    SetScrollbars(0, 20, 0, 50,0, 0, true);
	SetScrollRate(0, 50);
}

// destructor
BgSubScrollWin::~BgSubScrollWin(){	
	// remove
	removeSpecAndCanvas();	
}

wxSizer* BgSubScrollWin::setMainSizer()
{	
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
		
	wxSizer* rbtnSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* canvasSizer = new wxBoxSizer( wxVERTICAL );
	
	//canvas
	kome::window::SpectrumCanvas* canvas = NULL;

	// set spectrums
	setSpectrums();

	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	bool bflg = false;
	int iSize = m_spectra.size()-1;
	
	wxWindowID wxid = -1;
	if( m_selRadioBtnNo == -1 ){
		m_selRadioBtnNo = iSize;
	}

	// 選択中のスペクトルキャンバスを先頭に表示
	if( m_selRadioBtnNo != -1 ){
		m_radioBtn = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
		m_radioBtn->SetToolTip("Select background spectrum");
		wxid = m_radioBtn->GetId(); // wxRadioButton ID
		m_selNoMap[wxid] = m_selRadioBtnNo;
		
		if( m_radioBtn != NULL ){
			m_radioBtn->SetValue(true);
			setBackground();

			kome::objects::Spectrum* spec = NULL;
			if(( m_selRadioBtnNo < (int)m_spectra.size() ) && ( 0 <= m_selRadioBtnNo ) ){	// @date 2012/12/11 <Add> OKADA
				spec = m_spectra[m_selRadioBtnNo];
			}																			// @date 2012/12/11 <Add> OKADA
			if( spec != NULL ){
				canvas = new kome::window::SpectrumCanvas( this, 400, 200, spec );
				m_canvas.push_back( canvas );
				// Context Menu	// @date 2012/11/30
				if( canvas != NULL ){
					canvas->setSubView( true );					
				}
				// set sizer
				rbtnSizer->Add( m_radioBtn, 1, wxGROW | wxALL,	BORDER_SIZE );
				canvasSizer->Add( new wxStaticText( this, wxID_ANY, spec->getTitle() ), 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
				canvasSizer->Add( canvas, 1, wxALL | wxGROW, BORDER_SIZE );
			}
		}
	}
	
	for( unsigned int i=0; i < m_spectra.size(); i++ ){
		if( m_selRadioBtnNo != i ){
			m_radioBtn = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize );
			m_radioBtn->SetToolTip("Select background spectrum");
			wxid = m_radioBtn->GetId(); // wxRadioButton ID
			m_selNoMap[wxid] = i;

			kome::objects::Spectrum* spec = NULL;
			spec = m_spectra[i];
			if( spec != NULL ){
				canvas = new kome::window::SpectrumCanvas( this, 400, 200, spec );
				m_canvas.push_back( canvas );
				// Context Menu	// @date 2012/11/30
				if( canvas != NULL ){
					canvas->setSubView( true );
				}
				// set sizer
				rbtnSizer->Add( m_radioBtn, 1, wxGROW | wxALL,	BORDER_SIZE );
				canvasSizer->Add( new wxStaticText( this, wxID_ANY, spec->getTitle() ), 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
				canvasSizer->Add( canvas, 1, wxALL | wxGROW, BORDER_SIZE );
			}
		}
	}
	
	sizer->Add( rbtnSizer, 0, wxALL | wxGROW, BORDER_SIZE );
	sizer->Add( canvasSizer, 0, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// on select radio button
void BgSubScrollWin::onSelectRadioButton( wxCommandEvent& evt ){
	int iEventId = evt.GetId();	

	m_selRadioBtnNo = m_selNoMap[iEventId];
	
	setBackground();
	
	return;
}

// get spectrum
kome::objects::Spectrum* BgSubScrollWin::getSpectrum(){
	
	if( m_selRadioBtnNo >= 0 && !m_spectra.empty()){
		m_selRadioBtnNo = m_spectra.size()-1;
		return m_spectra[m_selRadioBtnNo];
	}	
	
	return NULL;
}

// set Spectrums
void BgSubScrollWin::setSpectrums(){	
	// remove spectrums and canvas
	removeSpecAndCanvas();

	for( unsigned int i=0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		// get canvas
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
		if( canvas != NULL ) {
			int style = canvas->getCanvasStyle();
			//singleのSpectrumかどうか判定
			if( style == 0 && canvas->getActiveObject().type == kome::objects::Variant::SPECTRUM ){ 
				// TransformedSpectrumに変換
				kome::objects::Spectrum* spec = new kome::operation::TransformedSpectrum( *canvas->getSpectrum() );
				m_spectra.push_back( spec );				
			}
		}
	}

	// get dialog ( BackgroundSubtractDialog )
	BackgroundSubtractDialog* parent = (BackgroundSubtractDialog*)GetParent();
	kome::objects::Spectrum* tmpSpec = parent->getTmpSpec();
	int j = m_spectra.size()-1;
	while( j >= 0  ){
		if( tmpSpec != NULL && tmpSpec->getOrgSpectrum() == m_spectra[j]->getOrgSpectrum() ){
			delete m_spectra[j];
			m_spectra.erase( m_spectra.begin() + j );
			if( j == m_selRadioBtnNo ){
				m_selRadioBtnNo = -1;
			}
			return;
		}
		j--;
	}
}

// set Background
void BgSubScrollWin::setBackground( ){	
	// managers
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	if( !m_spectra.empty() && m_selRadioBtnNo != -1 ){
		if(( m_selRadioBtnNo < (int)m_spectra.size() ) && ( 0 <= m_selRadioBtnNo ) ){	// @date 2012/12/11 <Add> OKADA
			mgr.setBackground( m_spectra[m_selRadioBtnNo] );
		}																			// @date 2012/12/11 <Add> OKADA
	}else{
		mgr.setBackground( NULL );
	}	
}

// spectrum 削除
void BgSubScrollWin::removeSpecAndCanvas(){
	// Spec No.89454 :Background Subtract画面を閉じる際にm_rangeMapの使用箇所をeraseしていない問題 @date 2012.12.25 <Mod> M.Izumi
	for( unsigned int i=0; i < m_canvas.size(); i++ ){
		for( unsigned int j=0; j < m_canvas[i]->getNumberOfSpectra(); j++ ){
			kome::objects::Spectrum* spec = m_canvas[i]->getSpectrum(j);
			for( std::vector<kome::objects::Spectrum*>::iterator it = m_spectra.begin(); it != m_spectra.end(); it++ ){
				if( spec == *it ){
					delete *it;
				}
			}
		}
	}
	m_canvas.clear();
	m_spectra.clear();
}

// get selected radio button Number
int BgSubScrollWin::getSelRadioBtnNo(){
	return m_selRadioBtnNo;
}
