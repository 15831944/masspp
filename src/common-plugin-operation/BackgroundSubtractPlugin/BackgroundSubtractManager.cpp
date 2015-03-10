/**
 * @file BackgroundSubtractManager.cpp
 * @brief implements of BackgroundSubtractManager class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BackgroundSubtractManager.h"
#include "BackgroundSubtraction.h"
#include "BackgroundSubtractDialog.h"
#include "BackgroundOperation.h"	

using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BackgroundSubtractManager::BackgroundSubtractManager() {
	m_spec = NULL;
	m_currBgSub = NULL;
	m_flg = false;
	m_dlg = NULL;
}

// destructor
BackgroundSubtractManager::~BackgroundSubtractManager() {
	closeMainFrame();
}

// set background
void BackgroundSubtractManager::setBackground( kome::objects::Spectrum* spec ) {
	// initialize
	m_spec = spec;
	if( !m_flg && m_currBgSub != NULL ) {
		delete m_currBgSub;
	}
	m_currBgSub = NULL;

	// refresh
	kome::window::WindowTool::refresh();

	// check the spectrum
	if( spec == NULL ) {
		return;
	}

	// get spectrum
	kome::core::XYData* xyData = spec->getXYData();

	// background subtraction object
	if( xyData != NULL ) {
		m_currBgSub = new BackgroundSubtraction();
		m_flg = false;
		m_currBgSub->setBackground( *xyData );
	}
}

// get current background subtraction
BackgroundSubtraction* BackgroundSubtractManager::getCurrentBackgroundSubtraction() {
	return m_currBgSub;
}

// set background flag value
void BackgroundSubtractManager::setBackgroundFlag() {
	m_flg = true;
}

// get Spectrum		@date 2011.07.14 <Add> M.Izumi
kome::objects::Spectrum* BackgroundSubtractManager::getSpectrum(){
	return m_spec;
}

// open dialog
void BackgroundSubtractManager::openDialog() {
	// close the dialog
	if( m_dlg != NULL ) {
		delete m_dlg;
	}
	m_dlg = NULL;

	// create the dialog
	m_dlg = new BackgroundSubtractDialog( kome::window::WindowTool::getMainWindow());
	
	m_dlg->Show( true );
}

// draw label
void BackgroundSubtractManager::drawLabel( kome::objects::Spectrum& spec, kome::img::Graphics& g ) {
	// check the spectrum
	if( m_spec == NULL || spec.getOrgSpectrum() != m_spec->getOrgSpectrum() ) {
		return;
	}

	// get values
	kome::img::Graphics::TextHAlign ha = g.getTextHAlign();
	kome::img::Graphics::TextVAlign va = g.getTextVAlign();
	kome::img::Font oldFont = g.getFont();
	COLOR oldColor = g.getColor();

	// set values
	kome::img::Font font(
		oldFont.getSize() + 2,
		kome::img::Font::FAMILY_SERIF,
		kome::img::Font::STYLE_NORMAL,
		kome::img::Font::WEIGHT_BOLD
	);

	g.setFont( font );
	g.setTextHAlign( kome::img::Graphics::HALIGN_RIGHT );
	g.setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	g.setColor( kome::img::ColorTool::GREEN );

	// draw
	g.drawText(
		"Background",
		(int)g.getGraphPositionRight() - 1,
		(int)g.getGraphPositionTop() + 1
	);

	// restore
	g.setFont( oldFont );
	g.setTextHAlign( ha );
	g.setTextVAlign( va );
	g.setColor( oldColor );
}

// get instance
BackgroundSubtractManager& BackgroundSubtractManager::getInstance() {
	// create object (This is the only object.)
	static BackgroundSubtractManager mgr;

	return mgr;
}

// add spectrum canvas @date 2011.07.14 <Add> M.Izumi
void BackgroundSubtractManager::AddSpectrumCanvas( ){
	// set background
	setBackground( m_spec );

	if( m_dlg != NULL && m_dlg->IsShown() ){
		m_dlg->AddSpectrumCanvas();
	}
}

// single canvasÇï¬Ç∂ÇΩéû @date 2011.07.14 <Add> M.Izumi
void BackgroundSubtractManager::closeSpecCanvas(kome::objects::Spectrum* spec){
	// set background
	setBackground( m_spec );

	if( m_dlg != NULL && m_dlg->IsShown() ){
		m_dlg->closeCanvas( spec );
	}
}

// ÉÅÉCÉìÉtÉåÅ[ÉÄÇï¬Ç∂ÇΩéû @date 2011.07.14 <Add> M.Izumi
void BackgroundSubtractManager::closeMainFrame(){
	if( !m_flg && m_currBgSub != NULL ) {
		delete m_currBgSub;
	}

	m_currBgSub = NULL;
	if( m_dlg != NULL ){
		delete m_dlg;
	}
	m_dlg = NULL;
	m_spec = NULL;
}


