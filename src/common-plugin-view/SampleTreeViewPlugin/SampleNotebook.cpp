/**
 * @file SampleNotebook.cpp
 * @brief implements of SampleNotebook class
 *
 * @author S.Tanaka
 * @date 2011.07.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "SampleNotebook.h"
#include "SampleTreeManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// event
BEGIN_EVENT_TABLE( SampleNotebook, wxNotebook )
	EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, SampleNotebook::onChangePage )
END_EVENT_TABLE()


// constructor
SampleNotebook::SampleNotebook( wxWindow* parent ) : wxNotebook( 
		parent,
		wxID_ANY,
		wxDefaultPosition,
		wxSize( 200, 300 ),
		wxNB_TOP
) {
}

// destructor
SampleNotebook::~SampleNotebook() {   
}

// on change page
void SampleNotebook::onChangePage( wxNotebookEvent& evt ) {
	// default
	wxNotebook::OnSelChange( evt );

	// managers
	SampleTreeManager& mgr = SampleTreeManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// sample
	kome::objects::Sample* sample = mgr.getSample( evt.GetSelection() );
	aoMgr.setActiveSample( sample );

	// activate
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::Variant var;
		if( sample != NULL ) {
			var.type = kome::objects::Variant::DATA_GROUP_NODE;
			var.prim.pt = sample->getRootDataGroupNode();
		}

		kome::plugin::PluginCallTool::onActivate( var );
	}
}
