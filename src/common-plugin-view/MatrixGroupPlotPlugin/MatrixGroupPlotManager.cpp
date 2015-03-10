/**
 * @file MatrixGroupPlotManager.cpp
 * @brief interfaces of MatrixGroupPlotManager class
 * 
 * @author M.Izumi
 * @date 2012.10.16 
 *
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixGroupPlotManager.h"
#include "PlotPanel.h"

using namespace kome::matrix::groupplot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MatrixGroupPlotManager::MatrixGroupPlotManager(){
	m_panel = NULL;
}

// destructor
MatrixGroupPlotManager::~MatrixGroupPlotManager(){
}

// create Panel
void MatrixGroupPlotManager::createPanel( wxWindow* parent, int peakId ){
	if( isPanel() ){
		delete m_panel;
	}
	m_panel = NULL;
	
	m_panel = new PlotPanel( parent, peakId );
}

// is panel
bool MatrixGroupPlotManager::isPanel(){
	return ( m_panel != NULL ? true : false  );
}

// set the panel
void MatrixGroupPlotManager::setPanel( PlotPanel* panel ){	
	m_panel = panel;
}

// get the panel
PlotPanel* MatrixGroupPlotManager::getPanel(){
	return m_panel;             // ここで、seizerを返して、common-libがどこかにいったまま本プラグインに帰ってこない。
}

// get object
MatrixGroupPlotManager& MatrixGroupPlotManager::getInstance(){
	static MatrixGroupPlotManager mgr;
	
	return mgr;
}
