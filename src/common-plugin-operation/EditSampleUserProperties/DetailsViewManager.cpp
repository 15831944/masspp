/**
 * @file DetailsViewManager.cpp
 * @brief implements of DetailsViewManager class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "DetailsViewManager.h"
#include "DetailsListCtrl.h"
#include "EditSampleUserPropertiesDialog.h"

using namespace kome::properties;


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
void DetailsViewManager::createPropertiesView(EditSampleUserPropertiesDialog* pParentDlg) {
	// check the member
	if( m_details != NULL ) {
		return;
	}

	// create
	m_details = new DetailsListCtrl( pParentDlg );
}

// get list ctrl
DetailsListCtrl* DetailsViewManager::getListCtrl() {
	return m_details;
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

	if( group == NULL ) {
		m_details->clear();
	}
	else {
		// properties
		kome::core::Properties userProps;
		while( group != NULL ) {
			// add properties
			for( unsigned int i = 0; i < group->getUserProperties().getNumberOfProperties(); i++ ) {
				userProps.setValue(
					group->getUserProperties().getKey( i ),
					group->getUserProperties().getValue( i )
				);
			}

			// parent group
			group = group->getParentGroup();
		}
		m_details->setProperties( userProps );
	}

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

	if( chrom == NULL ) {
		m_details->clear();
	}
	else {
		// properties
		kome::core::Properties userProps;
		userProps = chrom->getUserProperties(); // Don't include group props

		m_details->setProperties( userProps );
	}

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

	if( spectrum == NULL ) {
		m_details->clear();
	}
	else {
		kome::core::Properties userProps;
		userProps = spectrum->getUserProperties(); // Don't include group props

		m_details->setProperties( userProps );
	}

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
	kome::core::Properties userProps;
	spectrum->getUserProperties( userProps );

	m_details->setProperties( userProps );

	// refresh
	m_details->Refresh();
}

// delete items
void DetailsViewManager::deleteItems()
{
	if( m_details != NULL ) {
		m_details->deleteItems();
	}
}

// new item
void DetailsViewManager::newItem()
{
	if( m_details != NULL ) {
		m_details->newItem();
	}
}

// update item
void DetailsViewManager::updateItem(const long lRow, const int iColumn, const char *s)
{
	if( m_details != NULL ) {
		m_details->updateItem(lRow, iColumn, s);
	}
}
