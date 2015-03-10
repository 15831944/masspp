/**
 * @file OverlappingSettingsViewManager.cpp
 * @brief implements of OverlappingSettingsViewManager class
 *
 * @author H.Parry
 * @date 2011.07.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corp., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingSettingsViewManager.h"
#include "OverlappingDialog.h"
#include "OverlappingSpectraCanvasEx.h"
#include "OverlappingChromatogramsCanvasEx.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define OVERLAPPING_SETTINGS_VIEW_STATUS_NAME			 "OVERLAPPING_SETTINGS_VIEW"


// constructor
OverlappingSettingsViewManager::OverlappingSettingsViewManager() {
	m_overlappingSettingsView = NULL;
	m_currentCanvas = NULL;
}

// destructor
OverlappingSettingsViewManager::~OverlappingSettingsViewManager() {
}

// create overlapping settings view
void OverlappingSettingsViewManager::createOverlappingSettingsView() {
	// check the member
	if( m_overlappingSettingsView != NULL ) {
		return;
	}

	// frame 
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_overlappingSettingsView = new OverlappingDialog( kome::window::WindowTool::getMainWindow() );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_overlappingSettingsView, "Overlapping Settings", kome::window::WindowTool::DOCK_ALIGN_RIGHT );
	}

	// select canvas base
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );
	selectCanvas( dynamic_cast< kome::window::CanvasBase* >(canvas) );
}

// select canvas
void OverlappingSettingsViewManager::selectCanvas( kome::window::CanvasBase* canvas ) {
	// check the member
	if( m_currentCanvas == canvas || m_overlappingSettingsView == NULL ) {
		return;
	}

	// canvas
	m_currentCanvas = canvas;
	m_overlappingSettingsView->setCanvas( canvas );
}

// get current canvas
kome::window::CanvasBase* OverlappingSettingsViewManager::getCurrentCanvas() {
	return m_currentCanvas;
}

// update view
void OverlappingSettingsViewManager::updateView() {

	// update
	if( m_overlappingSettingsView != NULL ) {
		m_overlappingSettingsView->update();
	}
}

// toggle the visibility of the spectra view
void OverlappingSettingsViewManager::toggleOverlappingSettingsViewVisibility() {
	// check the member
	if( m_overlappingSettingsView == NULL ) {
		createOverlappingSettingsView();
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	m_overlappingSettingsView->update();// @date 2012.04.16 <Add> M.Izumi

	// toggle visibility
	mainFrame->toggleVisibleBar( m_overlappingSettingsView );
}

// check the visibility of the spectra view
bool OverlappingSettingsViewManager::isVisibleOverlappingSettingsView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_overlappingSettingsView );
}

// release view
void OverlappingSettingsViewManager::releaseView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	if( m_overlappingSettingsView != NULL ) {
		statusMgr.setValue(
			OVERLAPPING_SETTINGS_VIEW_STATUS_NAME,
			"off"
		);
	}

	// release
	m_overlappingSettingsView = NULL;
	m_currentCanvas = NULL;
}

// on close sample
void OverlappingSettingsViewManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// check sample
	if( sample == NULL || m_currentCanvas == NULL || m_overlappingSettingsView == NULL ) {
		return;
	}

	updateView();
}   

// on open spectrum
void OverlappingSettingsViewManager::onOpenSpectrum( kome::objects::Spectrum* spec ) {

	// select canvas base
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );
	selectCanvas( dynamic_cast< kome::window::CanvasBase* >(canvas) );
		
	updateView();
}

// on close spectrum
void OverlappingSettingsViewManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	
	if( m_overlappingSettingsView != NULL ) {
		updateView();
	}
}

// on open chromatogram
void OverlappingSettingsViewManager::onOpenChromatogram( kome::objects::Chromatogram* chrom ) {

	// select canvas base
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );
	selectCanvas( dynamic_cast< kome::window::CanvasBase* >(canvas) );

	updateView();
}

// on close chromatogram
void OverlappingSettingsViewManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {

	if( m_overlappingSettingsView != NULL ) {
		updateView();
	}
}

// キャンバスで選択されたスペクトルを送る		@date 2011.10.26 <Add> M.Izumi
void OverlappingSettingsViewManager::setSelectionItem( kome::objects::Spectrum* spec ){
	if( m_overlappingSettingsView != NULL ){
		m_overlappingSettingsView->SelectSpecOfCanvas( spec );
	}
}

// キャンバスで選択されたクロマトグラムを送る		@date 2011.10.26 <Add> M.Izumi
void OverlappingSettingsViewManager::setSelectionItem( kome::objects::Chromatogram* chrom ){
	if( m_overlappingSettingsView != NULL ){
		m_overlappingSettingsView->SelectChromOfCanvas( chrom );
	}
}

void OverlappingSettingsViewManager::onSpecKeyUpMgr( kome::objects::Spectrum* spec ){
}

// get instance
OverlappingSettingsViewManager& OverlappingSettingsViewManager::getInstance() {
	// create object (This is the only object.)
	static OverlappingSettingsViewManager mgr;

	return mgr;
}
