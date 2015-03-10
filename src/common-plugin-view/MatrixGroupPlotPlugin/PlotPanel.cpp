/**
 * @file PlotPanel.cpp
 * @brief implements of PlotPanel class
 *
 * @author S.Tanaka
 * @date 2009.07.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "PlotPanel.h"
#include "PlotCanvas.h"
#include "MatrixGroupPlotManager.h"

#include <wx/tglbtn.h>
#include <wx/event.h>


using namespace kome::matrix::groupplot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_CLOSE_BUTTON					128
#define ID_GROUP_PLOT					130
#define ID_BOX_PLOT						131
#define GRID_GAP						3
#define BORDER_SIZE						3

#define GROUP_WIDTH						120

BEGIN_EVENT_TABLE( PlotPanel, kome::window::StandardPanel )
	EVT_BUTTON( ID_GROUP_PLOT, PlotPanel::onOpenGroupPlot )
	EVT_BUTTON( ID_BOX_PLOT, PlotPanel::onOpenBoxPlot )
END_EVENT_TABLE()

// constructor
PlotPanel::PlotPanel( wxWindow* parent, const int peakId )
		: kome::window::StandardPanel( parent, wxID_ANY ), m_peakId( peakId ) {
	m_canvas = NULL;
	createControls();
	
	m_groupButton = NULL;
	m_boxButton = NULL;
}

// destrucor
PlotPanel::~PlotPanel() {
	MatrixGroupPlotManager& mgr = MatrixGroupPlotManager::getInstance();
	mgr.setPanel( NULL );
}

// set peak ID
void PlotPanel::setPeakId( const int peakId ) {
	// sets the peak ID
	if( m_peakId == peakId ) {
		return;
	}

	// set peak ID
	m_peakId = peakId;
	m_canvas->setPeakId( peakId );
}

// create main size
wxSizer* PlotPanel::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* btnSizer = new wxBoxSizer( wxHORIZONTAL);
	
	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	m_groupButton = new wxBitmapButton( this,ID_GROUP_PLOT, *iconMgr.getIcon("group_plot") );	
	btnSizer->Add( m_groupButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	m_boxButton = new wxBitmapButton( this, ID_BOX_PLOT, *iconMgr.getIcon("box_plot"));
	btnSizer->Add( m_boxButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, BORDER_SIZE );
	sizer->Add( btnSizer );
	
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// groups
	int project = identMgr.getSelectedMatrix();
	std::vector< int > matrixGroups;
	std::set< int > groupSet;

	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		std::vector< int > samples;
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );

		if( samples.size() > 0 ) {
			groupSet.insert( matrixGroups[ i ] );
		}
	}

	// canvas width
	int width = ( GROUP_WIDTH * ( 1 + groupSet.size() ) ) / 4 + GROUP_WIDTH * groupSet.size();

	// canvas
	m_canvas = new PlotCanvas( this, width, 300, m_peakId, getGroupPlotStatus(), getBoxPlotStatus() );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		int group = matrixGroups[ i ];
		if( groupSet.find( group ) != groupSet.end() ) {
			m_canvas->addGroup( group );
		}
	}

	sizer->Add( m_canvas, 1, wxGROW | wxALL, BORDER_SIZE );   

	return sizer;    
}

// on open group plot
void PlotPanel::onOpenGroupPlot( wxCommandEvent& evt ){
	
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	
	bool bplot = getGroupPlotStatus();
	m_canvas->setGroupPlot( !bplot );
	
	if( bplot ){
		statusMgr.setValue( "group_plot", "off" );		
	}else{
		statusMgr.setValue( "group_plot", "on" );
	}

	m_canvas->Refresh();
}

// on open box plot
void PlotPanel::onOpenBoxPlot( wxCommandEvent& evt ){
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	bool bplot = getBoxPlotStatus();
	m_canvas->setBoxPlot( !bplot );

	if( bplot ){
		statusMgr.setValue( "box_plot", "off" );
	}else{
		statusMgr.setValue( "box_plot", "on" );
	}	

	m_canvas->Refresh();
}

// get Group Plot Status
bool PlotPanel::getGroupPlotStatus(){
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( "group_plot" );
	std::string status = NVL( s, "on" );
	if( status.compare( "on" ) == 0 ){
		return true;
	}
	return false;
}

// get Box Plot Status
bool PlotPanel::getBoxPlotStatus(){
	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( "box_plot" );
	std::string status = NVL( s, "on" );
	if( status.compare( "on" ) == 0 ){
		return true;
	}
	return false;
}


