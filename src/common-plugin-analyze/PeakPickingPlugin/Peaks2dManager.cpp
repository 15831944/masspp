/**
 * @file Peaks2dManager.cpp
 * @brief implements of Peaks2dManager
 *
 * @author S.Tanaka
 * @date 2007.10.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks2dManager.h"      
#include "PeaksListCtrl.h"


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAKS_2D_VIEW_STATUS_NAME			  "PEAKS_2D_VIEW"
#define LABELING_NAME						  "labeling"
#define CLUSTERING_NAME						  "clustering"

#define START_RT_PARAM_NAME					  "start_rt"
#define END_RT_PARAM_NAME					  "end_rt"
#define START_MZ_PARAM_NAME					  "start_mz"
#define END_MZ_PARAM_NAME					  "end_mz"


// constructor
Peaks2dManager::Peaks2dManager() {
	m_listCtrl = NULL;
	m_group = NULL;
}

// destructor
Peaks2dManager::~Peaks2dManager() {
}

// detects peaks
void Peaks2dManager::detectPeaks(
		kome::objects::DataGroupNode& group,
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D& peaks,
		kome::plugin::PluginFunctionItem* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		kome::core::Progress& progress,
		const double startRt,
		const double endRt,
		const double startMz,
		const double endMz
) {
	// initialize peaks
	peaks.clearClusters();
	if( peakDetector != NULL ) {
		peaks.clearPeaks();
	}
	kome::plugin::PluginCallTool::onUpdate2DPeaks( group );
	
	// progress
	kome::core::Progress* pgrs0 = &progress;

	// peak detect
	if( peakDetector != NULL ) {
		// progress
		progress.setStatus( "Detecting Peaks..." );

		// parameters
		kome::objects::Parameters peakParams; 

		kome::plugin::PluginCallTool::setGroup( peakParams, group );
		kome::plugin::PluginCallTool::setDataSet( peakParams, dataSet );
		kome::plugin::PluginCallTool::setPeaks2D( peakParams, peaks );
		kome::plugin::PluginCallTool::setProgress( peakParams, *pgrs0 );
		kome::plugin::PluginCallTool::setDouble( peakParams, START_RT_PARAM_NAME, startRt );
		kome::plugin::PluginCallTool::setDouble( peakParams, END_RT_PARAM_NAME, endRt );
		kome::plugin::PluginCallTool::setDouble( peakParams, START_MZ_PARAM_NAME, startMz );
		kome::plugin::PluginCallTool::setDouble( peakParams, END_MZ_PARAM_NAME, endMz );
		if( peakSettings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( peakParams, *peakSettings );
		}

		// timer
		kome::core::Timer tm;
		tm.start();

		// peak detection
		peakDetector->getCall()->invoke( &peakParams );

		tm.stop();
		LOG_INFO( FMT( "Peak Detection Time : %f", tm.getTotalTime() ) );
	}

	// broadcast
	if( !progress.isStopped() ) {
		kome::plugin::PluginCallTool::onUpdate2DPeaks( group );
	}
	kome::plugin::PluginCallTool::onUpdate2DPeaks( group );
	kome::window::WindowTool::refresh();
}

// create peaks view
void Peaks2dManager::createPeaksView() {
	// check the member
	if( m_listCtrl != NULL ) {
		return;
	}

	// main frame
	wxWindow* mainWnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( mainWnd );

	// create
	m_listCtrl = new PeaksListCtrl( mainWnd );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_listCtrl, "2D Peaks", kome::window::WindowTool::DOCK_ALIGN_BOTTOM );
	}

	// active group
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::objects::DataGroupNode* group = NULL;
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		group = obj.getGroup();
	}

	// select group
	selectGroup( group );
}

// release view
void Peaks2dManager::releaseView() {
	// check the member
	if( m_listCtrl == NULL ) {
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( PEAKS_2D_VIEW_STATUS_NAME, mainFrame->isVisibleBar( m_listCtrl ) ? "on" : "off" );

	// release
	m_listCtrl = NULL;
}

// toggle visibility of the peak list view
void Peaks2dManager::toggleVisibilityOfPeakListView() {
	// check the member
	if( m_listCtrl == NULL ) {
		createPeaksView();
		return;
	}

	// toggle visibility
	wxWindow* frame = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( frame );
	if( mainFrame != NULL ) {
		mainFrame->toggleVisibleBar( m_listCtrl );
	}
}

// check the peak list view is visible or not
bool Peaks2dManager::isVisiblePeakListView() {
	if( m_listCtrl == NULL ) {
		return false;
	}

	wxWindow* frame = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( frame );
	if( mainFrame != NULL ) {	
		return mainFrame->isVisibleBar( m_listCtrl );
	}
	return false;
}

// update peak list view
void Peaks2dManager::updatePeakListView( kome::objects::DataGroupNode* group ) {
	// check 
	if( m_listCtrl ==  NULL ) {
		return;
	}
	if( group != m_group ) {
		return;
	}

	// update
	m_listCtrl->setGroup( group );
	m_listCtrl->Refresh();
}

// select spectrum group
void Peaks2dManager::selectGroup( kome::objects::DataGroupNode* group ) {
	// check the member
	if( m_listCtrl == NULL ) {
		return;
	}
	if( m_group == group ) {
		return;
	}

	// select group
	m_group = group;
	m_listCtrl->setGroup( group );

	// refresh
	m_listCtrl->Refresh();	
}

// get selected spectrum group
kome::objects::DataGroupNode* Peaks2dManager::getSelectedGroup() {
	return m_group;
}

// clear selected peaks
void Peaks2dManager::clearSelectedPeaks() {
	m_selectedPeaks.clear();
	m_selectedClusters.clear();
}

// add selected peak
void Peaks2dManager::addSelectedPeaks( kome::objects::Peak2DElement* peak ) {
	// add peak
	m_selectedPeaks.insert( peak );

	// add cluster
	kome::objects::PeaksCluster2D* cluster = peak->getCluster();
	if( cluster != NULL ) {
		m_selectedClusters.insert( cluster );
	}	
}

// draw peaks
void Peaks2dManager::drawPeaks( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
	// color
	COLOR oldColor = g.getColor();

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataGroupNode* group = &( dataMap.getGroup() );
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL ) {
		return;
	}

	// draw unselected peaks
	g.setColor( kome::img::ColorTool::AQUA );
	for( unsigned int i = 0; i < peaks->getNumberOfPeaks(); i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks->getPeak( i );

		// draw
		if( peak->getEndMz() >= g.getMinX()
				&& peak->getStartMz() <= g.getMaxX()
				&& peak->getEndRt() >= g.getMinY()
				&& peak->getStartRt() <= g.getMaxY()
				&& m_selectedPeaks.find( peak ) == m_selectedPeaks.end() ) {	// check range

			// draw
			drawPosition( g, peak->getStartRt(), peak->getEndRt(), peak->getStartMz(), peak->getEndMz() );
		}
	}

	// draw selected peaks
	g.setColor( kome::img::ColorTool::FUCHSIA );
	for( std::set< kome::objects::Peak2DElement* >::iterator it = m_selectedPeaks.begin();
				it != m_selectedPeaks.end(); it++ ) {
		// peak
		kome::objects::Peak2DElement* peak = *it;

		// draw
		if( peak->getEndRt() >= g.getMinX()
				&& peak->getStartRt() <= g.getMaxX()
				&& peak->getEndMz() >= g.getMinY()
				&& peak->getStartMz() <= g.getMaxY() ) {	// check range
			// draw
			drawPosition( g, peak->getStartRt(), peak->getEndRt(), peak->getStartMz(), peak->getEndMz() );
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw clusters
void Peaks2dManager::drawClusters( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
	// color
	COLOR oldColor = g.getColor();

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataGroupNode* group = &( dataMap.getGroup() );
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL ) {
		return;
	}

	// draw unselected clusters
	g.setColor( kome::img::ColorTool::LIME );
	for( unsigned int i = 0; i < peaks->getNumberOfClusters(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* cluster = peaks->getCluster( i );

		// draw
		if( cluster->getEndMz() >= g.getMinX()
				&& cluster->getStartMz() <= g.getMaxX()
				&& cluster->getEndRt() >= g.getMinY()
				&& cluster->getStartRt() <= g.getMaxY() 
				&& m_selectedClusters.find( cluster ) == m_selectedClusters.end() ) {	// check range
			// draw position
			drawPosition( g, cluster->getStartRt(), cluster->getEndRt(), cluster->getStartMz(), cluster->getEndMz() );
		}
	}

	// draw selected clusters
	g.setColor( kome::img::ColorTool::FUCHSIA );
	for( std::set< kome::objects::PeaksCluster2D* > ::iterator it = m_selectedClusters.begin();
			it != m_selectedClusters.end(); it++ ) {
		// cluster
		kome::objects::PeaksCluster2D* cluster = *it;

		// draw
		if( cluster->getEndRt() >= g.getMinX()
				&& cluster->getStartRt() <= g.getMaxX()
				&& cluster->getEndMz() >= g.getMinY()
				&& cluster->getStartMz() <= g.getMaxY() ) {	// check range
			// draw position
			drawPosition( g, cluster->getStartRt(), cluster->getEndRt(), cluster->getStartMz(), cluster->getEndMz() );
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw position
void Peaks2dManager::drawPosition(
		kome::img::Graphics& g,
		const double startRt,
		const double endRt,
		const double startMz,
		const double endMz
) {
	// position
	double l = double();
	double r = double();
	double t = double();
	double b = double();

	g.transformDataToPosition( startMz, endRt, &l, &t );
	g.transformDataToPosition( endMz, startRt, &r, &b );

	int left   = roundnum( l ) - 1;
	int right  = roundnum( r ) + 1;
	int top	= roundnum( t ) - 1;
	int bottom = roundnum( b ) + 1;

	// draw
	int rectPts[] = {
		left,  top,
		right, top,
		right, bottom,
		left,  bottom,
		left, top
	};
	g.drawLines( rectPts, sizeof( rectPts ) / ( sizeof( int ) * 2 ) );
}

// save peaks
bool Peaks2dManager::savePeaks(
		kome::objects::Peaks2D& peaks,
		const char* filePath,
		kome::core::Progress& progress
) {
	// open file
	FILE* fp = fileopen( filePath, "w" );
	if( fp == NULL ) {
		return false;
	}

	// header
	fprintf( fp, "RT\tm/z\tIntensity\tArea\n" );

	// progress
	if( peaks.getNumberOfPeaks() == 0 ) {
		progress.fill();
		return true;
	}
	progress.setRange( 0, peaks.getNumberOfPeaks() );

	// write peaks
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks(); i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );

		const double rt = peak->getRt();
		const double mz = peak->getMz();
		const double intensity = peak->getIntensity();

		// write
		if( rt >= 0.0 && mz >= 0.0 && intensity >= 0.0 ) {
			fprintf( fp, "%f\t%f\t%f\n", rt, mz, intensity );
		}

		progress.setPosition( i + 1 );
	}

	// close
	fflush( fp );
	fclose( fp );
	progress.fill();

	return true;
}

// on close sample
void Peaks2dManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// check objects
	if( sample == NULL || m_group == NULL ) {
		return;
	}

	// unset group
	if( m_group->getSample() == sample ) {
		selectGroup( NULL );
	}
}

// get instance
Peaks2dManager& Peaks2dManager::getInstance() {
	// create object
	static Peaks2dManager mgr;

	return mgr;
}
