/**
 * @file MatrixOverlappingViewManager.h
 * @brief interfaces of MatrixOverlappingViewManager class
 * 
 * @author M.Izumi
 * @date 2012.10.16 
 *
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixOverlappingViewManager.h"
#include "ProfilePanel.h"

using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MatrixOverlappingViewManager::MatrixOverlappingViewManager(){
	m_panel = NULL;
}

// destructor
MatrixOverlappingViewManager::~MatrixOverlappingViewManager(){
}

// create panel
void MatrixOverlappingViewManager::createPanel( wxWindow* parent, int peakId ){
	if( isPanel() ){
		delete m_panel;
	}
	m_panel = NULL;
	
	m_panel = new ProfilePanel( parent, peakId );
}

// is panel
bool MatrixOverlappingViewManager::isPanel(){
	return ( m_panel != NULL ? true : false  );
}

// set the panel
void MatrixOverlappingViewManager::setPanel( ProfilePanel* panel ){	
	m_panel = panel;
}

// get the panel
ProfilePanel* MatrixOverlappingViewManager::getPanel(){
	return m_panel;
}

// get object
MatrixOverlappingViewManager& MatrixOverlappingViewManager::getInstance(){
	static MatrixOverlappingViewManager mgr;
	
	return mgr;
}
