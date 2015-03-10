/**
 * @file DetailsViewManager.cpp
 * @brief implements of DetailsViewManager class
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DetailsViewManager.h"
#include "DetailsListCtrl.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PROPERTIES_VIEW_STATUS_NAME		"PROPERTIES_GRID_VIEW"


// constructor
DetailsViewManager::DetailsViewManager() {
	m_details = NULL;

	m_currentSpectrum = NULL;
	m_currentChromatogram = NULL;
	m_currentGroup = NULL;
}

// destructor
DetailsViewManager::~DetailsViewManager() {
}

// create file details view
void DetailsViewManager::createPropertiesView() {
	// check the member
	if( m_details != NULL ) {
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_details = new DetailsListCtrl( wnd );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_details, "Properties", kome::window::WindowTool::DOCK_ALIGN_RIGHT );
	}

	// active objects
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();

	// select
	if( spec != NULL ) {
		selectSpec( spec );
	}
	else if( chrom != NULL ) {
		selectChrom( chrom );
	}
	else {
		selectGroup( group );
	}
}

// select group
void DetailsViewManager::selectGroup( kome::objects::DataGroupNode* group ) {
	// check the member
	if( ( m_currentSpectrum == NULL && m_currentGroup == group
				&& m_currentChromatogram == NULL ) || m_details == NULL ) {
		return;
	}

	// set group
	m_currentGroup = group;
	m_currentSpectrum = NULL;
	m_currentChromatogram = NULL;

	// properties
	kome::core::Properties props;
	kome::core::Properties userProps;
	while( group != NULL ) {
		// add properties
		for( unsigned int i = 0; i < group->getProperties().getNumberOfProperties(); i++ ) {
			props.setValue(
				group->getProperties().getKey( i ),
				group->getProperties().getValue( i )
				);
		}
		// add user properties
		for( unsigned int i = 0; i < group->getUserProperties().getNumberOfProperties(); i++ ) {
			userProps.setValue(
				group->getUserProperties().getKey( i ),
				group->getUserProperties().getValue( i )
			);
		}
		
		// parent group
		group = group->getParentGroup();
	}
	m_details->setProperties( props, userProps );

	// refresh
	m_details->Refresh();
}

// select chromatogram
void DetailsViewManager::selectChrom( kome::objects::Chromatogram* chrom  ) {
	// check the member
	if( ( m_currentGroup == NULL && m_currentSpectrum == NULL
				&& m_currentChromatogram == chrom ) || m_details == NULL ) {
		return;
	}

	// set spectrum
	m_currentGroup = NULL;
	m_currentSpectrum = NULL;
	m_currentChromatogram = chrom;

	// properties
	kome::core::Properties props;
	chrom->getProperties( props );
	// user properties
	kome::core::Properties userProps;
	chrom->getUserProperties( userProps );
	
	m_details->setProperties( props, userProps );

	// refresh
	m_details->Refresh();
}

// select spectrum
void DetailsViewManager::selectSpec( kome::objects::Spectrum* spectrum ) {
	// check the member
	if( ( m_currentGroup == NULL && m_currentSpectrum == spectrum
				&& m_currentChromatogram == NULL ) || m_details == NULL ) {
		return;
	}

	// set spectrum
	m_currentGroup = NULL;
	m_currentSpectrum = spectrum;
	m_currentChromatogram = NULL;

	kome::core::Properties props;
	spectrum->getProperties( props );
	
	kome::core::Properties userProps;
	spectrum->getUserProperties( userProps );
	
	m_details->setProperties( props, userProps );

	// refresh
	m_details->Refresh();
}

// update spec
void DetailsViewManager::updateSpectrum( kome::objects::Spectrum* spectrum ) {
	// check the member
	if( spectrum == NULL
			|| m_currentSpectrum != spectrum
			|| m_details == NULL ) {
		return;
	}

	// update
	kome::core::Properties props;
	spectrum->getProperties( props );
	kome::core::Properties userProps;
	spectrum->getUserProperties( userProps );

	m_details->setProperties( props, userProps );

	// refresh
	m_details->Refresh();
}

// update chro
void DetailsViewManager::updateChromatogram( kome::objects::Chromatogram* chromatogram ) {
	// check the member
	if( chromatogram == NULL
			|| m_currentChromatogram != chromatogram
			|| m_details == NULL ) {
		return;
	}

	// update
	kome::core::Properties props;
	chromatogram->getProperties( props );
	kome::core::Properties userProps;
	chromatogram->getUserProperties( userProps );

	m_details->setProperties( props, userProps );

	// refresh
	m_details->Refresh();
}

// toggle the visibility of file details
void DetailsViewManager::togglePropertiesVisibility() {
	// check the member
	if( m_details == NULL ) {
		createPropertiesView();
		return;
	}

	// toggle visibility
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame != NULL ) {
		mainFrame->toggleVisibleBar( m_details );
	}
}

// check the visibility of the file details
bool DetailsViewManager::isVisiblePropertiesView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_details );
}

// release view
void DetailsViewManager::releaseView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	if( m_details != NULL ) {
		statusMgr.setValue(
			PROPERTIES_VIEW_STATUS_NAME,
			( mainFrame != NULL && mainFrame->isVisibleBar( m_details ) ) ? "on" : "off"
		);
	}

	// release
	m_details = NULL;
}

// on close sample
void DetailsViewManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// current sample
	kome::objects::Sample* currentSample = NULL;
	if( m_currentGroup != NULL ) {
		currentSample = m_currentGroup->getSample();
	}

	// close	
	if( sample == currentSample ) {
		selectGroup( NULL );
	}
}

// get instance
DetailsViewManager& DetailsViewManager::getInstance() {
	// create object (This is the only object.)
	static DetailsViewManager mgr;

	return mgr;
}
