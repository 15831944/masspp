/**
 * @file SpectraGridViewManager.cpp
 * @brief implements of SpectraGridViewManager class
 *
 * @author S.Tanaka
 * @date 2007.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraGridViewManager.h"
#include "SpectraListCtrl.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SPECTRA_VIEW_STATUS_NAME			 "SPECTRA_VIEW"


// constructor
SpectraGridViewManager::SpectraGridViewManager() {
	m_dataSetView = NULL;
	m_currentSample = NULL;
}

// destructor
SpectraGridViewManager::~SpectraGridViewManager() {
}

// create spectra view
void SpectraGridViewManager::createSpectraGridView() {
	// check the member
	if( m_dataSetView != NULL ) {
		return;
	}

	// frame 
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	if( wnd == NULL ){
		return;
	}
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_dataSetView = new SpectraListCtrl( kome::window::WindowTool::getMainWindow() );
	
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_dataSetView, "Filtered Spectrum List", kome::window::WindowTool::DOCK_ALIGN_LEFT );	// @date 2011/06/15 <Add> OKADA
	}

	// select group
	// @date 2011.11.07 <Mod> M.Izumi ->	
	// SPEC:82830 Filter Spectra List‚ÉŒŸõŒ‹‰Ê‚ªo‚È‚¢ •s‹ï‡C³
	kome::objects::Sample* activeSample = NULL;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
		activeSample = obj.getSample();
	}else{
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		activeSample = aoMgr.getActiveSample();
	}
	// @date 2011.11.07 <Mod> M.Izumi <-

	selectSample( activeSample );
}

// select sample
void SpectraGridViewManager::selectSample( kome::objects::Sample* sample ) {
	// check the member
	if( m_currentSample == sample || m_dataSetView == NULL ) {
		return;
	}

	// sample
	m_currentSample = sample;
	m_dataSetView->setSample( sample );
	m_dataSetView->updateTitle();
}

// get current group
kome::objects::Sample* SpectraGridViewManager::getCurrentSample() {
	return m_currentSample;
}

// update view
void SpectraGridViewManager::updateView( kome::objects::DataSet& dataSet ) {
	// show spectra view
	if( !isVisibleSpectraGridView() ) {
		toggleSpectraGridViewVisibility();
	}
	if( m_dataSetView != NULL ){
		// update
		m_dataSetView->update();
		m_dataSetView->updateTitle();
	}
}

// toggle the visibility of the spectra view
void SpectraGridViewManager::toggleSpectraGridViewVisibility() {
	// check the member
	if( m_dataSetView == NULL ) {
		createSpectraGridView();
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_dataSetView );
}

// check the visibility of the spectra view
bool SpectraGridViewManager::isVisibleSpectraGridView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_dataSetView );
}

// release view
void SpectraGridViewManager::releaseView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	if( m_dataSetView != NULL ) {
		statusMgr.setValue(
			SPECTRA_VIEW_STATUS_NAME,
			( mainFrame != NULL && mainFrame->isVisibleBar( m_dataSetView ) ) ? "on" : "off"
		);
	}

	// release
	m_dataSetView = NULL;
}

// on close sample
void SpectraGridViewManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// check sample
	if( sample == NULL || m_currentSample == NULL ) {
		return;
	}

	// unset
	if( m_currentSample == sample ) {
		selectSample( NULL );
	}
}

// get instance
SpectraGridViewManager& SpectraGridViewManager::getInstance() {
	// create object (This is the only object.)
	static SpectraGridViewManager mgr;

	return mgr;
}
