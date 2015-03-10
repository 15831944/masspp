/**
 * @file SampleTreeManager.cpp
 * @brief implements of SampleTreeManager class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeManager.h"
#include "SampleTreeData.h"
#include "SampleTreeCtrl.h"

#include <wx/wx.h>
#include <wx/imaglist.h>


using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ICON_WIDTH				16
#define ICON_HEIGHT			    16

#define CLOSE_FOLDER_ICON_NAME	"folder-close"
#define OPEN_FOLDER_ICON_NAME	 "folder-open"
#define MS1_ICON_NAME			 "MS1"
#define MS2_ICON_NAME			 "MS2"
#define MS3_ICON_NAME			 "MS3"
#define MS4_ICON_NAME			 "MS4"
#define MS5_ICON_NAME			 "MS5"
#define MS6_ICON_NAME			 "MS6"
#define MS7_ICON_NAME			 "MS7"
#define MSN_ICON_NAME			 "MSn"

#define ROOT_ITEM_NAME			 "root"



// constructor
SampleTreeManager::SampleTreeManager() {
	m_tree = NULL;
}

// destructor
SampleTreeManager::~SampleTreeManager() {
}

// create tree ctrl
void SampleTreeManager::createTreeCtrl(EditSampleUserPropertiesDialog* pWnd) {
	// gets icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// create tree ctrl
	m_tree = new SampleTreeCtrl(pWnd);

	// add root	
	m_rootId = m_tree->AddRoot( wxT( ROOT_ITEM_NAME ), 0, 0, new SampleTreeData() );

	// image list
	wxImageList* imageList = iconMgr.createImageList( ICON_WIDTH, ICON_HEIGHT );
	m_tree->AssignImageList( imageList );
}

// get tree ctrl
SampleTreeCtrl* SampleTreeManager::getTreeCtrl() {
	return m_tree;
}

// add sample node
void SampleTreeManager::addSample( kome::objects::Sample* sample ) {
	// check parameter and member
	if( sample == NULL || m_tree == NULL ) {
		return;
	}

	// create data object
	if( m_sampleMap.find( sample ) != m_sampleMap.end() ) {
		return;
	}
	m_sampleMap[ sample ] = addTreeItem( sample->getRootDataGroupNode(), m_rootId );

	// Expand
	m_tree->Expand( m_sampleMap[ sample ] );
}

// update nodes
void SampleTreeManager::updateNodes( kome::objects::DataGroupNode* group ) {
	// check objects
	if( m_tree == NULL || group == NULL ) {
		return;
	}

	// sample
	kome::objects::Sample* sample = group->getSample();
	if( sample == NULL ) {
		return;
	}

	// ID
	wxTreeItemId id = m_groupMap[ group ];

	// delete children
	m_tree->DeleteChildren( id );

	// add children
	addChildren( group, id );
}

// add tree item ID
wxTreeItemId SampleTreeManager::addTreeItem( kome::objects::DataGroupNode* group, wxTreeItemId& parentId ) {
	// check parameter and member
	if( group == NULL || m_tree == NULL ) {
		return parentId;
	}

	// create data
	SampleTreeData* groupData = new SampleTreeData();
	groupData->getData().type = kome::objects::Variant::DATA_GROUP_NODE;
	groupData->getData().prim.pt = group;

	// manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// get folder image
	int img = iconMgr.getIconIndex( CLOSE_FOLDER_ICON_NAME );

	// add spectrum group item
	wxTreeItemId id = m_tree->AppendItem(
		parentId,
		wxT( NVL( group->getName(), "" ) ),
		img,
		img,
		groupData
	);
	m_tree->SetItemImage( id, iconMgr.getIconIndex( OPEN_FOLDER_ICON_NAME ), wxTreeItemIcon_Expanded );

	m_groupMap[ group ] = id;

	// add children
	addChildren( group, id );

	return id;
}

// add children
void SampleTreeManager::addChildren( kome::objects::DataGroupNode* group, wxTreeItemId& groupId ) {
	// create child item (call method recursively)
	for( unsigned int i = 0; i < group->getNumberOfChildren(); i++ ) {
		addTreeItem( group->getChild( i ), groupId );
	}

	// manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// check spectra
	int specCnt = 0;
	for( unsigned int i = 0; i < group->getNumberOfSpectra() && specCnt < 2; i++ ) {
		kome::objects::Spectrum* spec = group->getSpectrum( i );
		if( spec->hasChromatogram() ) {
			specCnt++;
		}
	}

	// add chromatogram item
	for( unsigned int i = 0; i < group->getNumberOfChromatograms(); i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = group->getChromatogram( i );

		if (!chrom->isAutoCreated())
		{
			// icon
			int img = iconMgr.getIconIndex( chrom->getIcon() );
			if( img < 0 ) {
				img = iconMgr.getIconIndex( "chromatogram" );
			}

			// create chromatogram data
			SampleTreeData* chromData = new SampleTreeData();

			chromData->getData().type = kome::objects::Variant::CHROMATOGRAM;
			chromData->getData().prim.pt = chrom;

			m_tree->AppendItem(
				groupId,
				wxT( chrom->getName() ),
				img,
				img,
				chromData
			);
		}
	}

	// add spectrum item
	const char* iconNames[] = {
		MS1_ICON_NAME,		
		MS2_ICON_NAME,
		MS3_ICON_NAME,
		MS4_ICON_NAME,
		MS5_ICON_NAME,
		MS6_ICON_NAME,
		MS7_ICON_NAME,
		MSN_ICON_NAME
	};
		
	for( unsigned int i = 0; i < group->getNumberOfSpectra(); i++ ) {
		// get spectrum
		kome::objects::Spectrum* spec = group->getSpectrum( i );

		// get image
		int img = iconMgr.getIconIndex( spec->getIcon() );
		if( img < 0 ) {
			// stage
			int stage = spec->getMsStage() - 1;
			stage = CLAMP( stage, 0, 7 );

			std::string iconName = iconNames[ stage ];

			// get icon
			img = iconMgr.getIconIndex( iconName.c_str() );
			spec->setIcon( iconName.c_str() );
		}

		// create data
		SampleTreeData* specData = new SampleTreeData();
		specData->getData().type = kome::objects::Variant::SPECTRUM;
		specData->getData().prim.pt = spec;

		// name
		std::string name = NVL( spec->getName(), "" );
		if( spec->getRt() >= 0.0 && spec->getStartRt() == spec->getEndRt() ) {
			name += FMT( " (%.4f)", spec->getRt() );
		}
		if( spec->getPrecursor() > 0.0 ) {
			name += FMT( " [%.2f]", spec->getPrecursor() );
		}

		// add item
		wxTreeItemId specId = m_tree->AppendItem(
			groupId,
			wxT( name.c_str() ),
			img,
			img,
			specData
		);
	}
}
