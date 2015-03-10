/**
 * @file SampleTreeCtrl.cpp
 * @brief implements of SampleTreeCtrl class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeCtrl.h"

#include "SampleTreeData.h"
#include "EditSampleUserPropertiesDialog.h"

using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SAMPLE_TREE_ID 3000

// event
BEGIN_EVENT_TABLE( SampleTreeCtrl, wxTreeCtrl )
	EVT_TREE_SEL_CHANGED( SAMPLE_TREE_ID, SampleTreeCtrl::onSelChanged )
	EVT_TREE_SEL_CHANGING( SAMPLE_TREE_ID, SampleTreeCtrl::onSelChanging )
END_EVENT_TABLE()


// constructor
SampleTreeCtrl::SampleTreeCtrl(EditSampleUserPropertiesDialog* pParentDlg) 
	: wxTreeCtrl( 
		pParentDlg,
		SAMPLE_TREE_ID,
		wxDefaultPosition,
		wxSize( 300, 350 ),
		wxTR_SINGLE | wxTR_ROW_LINES | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT
	), m_pParentDlg(pParentDlg)
{
}

// destructor
SampleTreeCtrl::~SampleTreeCtrl() {   
}

// If a problem is found during validation of the current item's
// list values or if the user cancels the change of selection,
// ignore the user's new item selection.
void SampleTreeCtrl::onSelChanging( wxTreeEvent& evt )
{
	if (!m_pParentDlg->CheckForUnsavedChangesAndValidate())
	{
		evt.Veto();
	}
}

// on single click
void SampleTreeCtrl::onSelChanged( wxTreeEvent& evt )
{
	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	// get data
	SampleTreeData* data = (SampleTreeData*)GetItemData( id );
	if( data == NULL ) {
		return;
	}

	// open
	if( data->getData().type == kome::objects::Variant::SPECTRUM ) {	// spectrum
		// get spectrum
		kome::objects::Spectrum* spec
			= (kome::objects::Spectrum*)data->getData().prim.pt;
		if( spec != NULL ) {
			m_pParentDlg->SetActiveSpectrum(spec);
		}
	}
	else if( data->getData().type == kome::objects::Variant::CHROMATOGRAM ) {	// chromatogram
		// get chromatogram
		kome::objects::Chromatogram* chrom
			= (kome::objects::Chromatogram*)data->getData().prim.pt;
		if( chrom != NULL ) {
			m_pParentDlg->SetActiveChromatogram(chrom);
		}
	}
	else if( data->getData().type == kome::objects::Variant::DATA_GROUP_NODE ) {	// group
		// get grouo
		kome::objects::DataGroupNode* group
			= (kome::objects::DataGroupNode*)data->getData().prim.pt;
		if( group != NULL ) {
			m_pParentDlg->SetActiveGroup(group);
		}
	}
}
