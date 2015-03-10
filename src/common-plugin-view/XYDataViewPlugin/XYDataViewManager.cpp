/**
 * @file XYDataViewManager.cpp
 * @brief implements of XYDataViewManager class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "XYDataViewManager.h"
#include "XYData2DListCtrl.h"	// @date 2011/05/25 <Add> OKADA

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DATA_XY_DATA_STATUS_NAME	"DATA_XY_DATA_VIEW"
#define PEAKS_XY_DATA_STATUS_NAME   "PEAKS_XY_DATA_VIEW"

// constructor
XYDataViewManager::XYDataViewManager() {
	m_dataXYData = NULL;
	m_peaksXYData = NULL;
	m_currentSpec = NULL;
	m_currentChrom = NULL;

	m_group = NULL;
}

// destructor
XYDataViewManager::~XYDataViewManager() {
}

// get current sample
kome::objects::Sample* XYDataViewManager::getCurrentSample() {
	if( m_currentSpec != NULL ) {
		return m_currentSpec->getSample();
	}
	if( m_currentChrom != NULL ) {
		return m_currentChrom->getSample();
	}
	return NULL;
}

// get current spectrum
kome::objects::Spectrum* XYDataViewManager::getCurrentSpectrum() {
	return m_currentSpec;
}

// get current chromatogram
kome::objects::Chromatogram* XYDataViewManager::getCurrentChromatogram() {
	return m_currentChrom;
}

// create data view
void XYDataViewManager::createPointsView() {
	// check the member
	if( m_dataXYData != NULL ) {
		return;
	}

	// frame 
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_dataXYData = new XYData2DListCtrl( mainFrame );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_dataXYData, "Data Points", kome::window::WindowTool::DOCK_ALIGN_BOTTOM );
	}

	// current object
	m_currentSpec = NULL;
	m_currentChrom = NULL;

	// active object
	kome::objects::Spectrum* spec = NULL;
	kome::objects::Chromatogram* chrom = NULL;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );

		spec = obj.getSpectrum();
		chrom = obj.getChromatogram();
	}

	// set object
	selectSpectrum( spec );
	selectChromatogram( chrom );
}


// create peaks view
void XYDataViewManager::createPeaksView() {
	// check the member
	if( m_peaksXYData != NULL ) {
		return;
	}

	// check initialize
	if( ( m_peaksXYData == NULL ) ){
		// frame 
		wxWindow* wnd = kome::window::WindowTool::getMainWindow();
		m_mainFrameUnite = dynamic_cast< kome::window::MainFrame* >( wnd );
	}

	// create
	m_peaksXYData = new XYData2DListCtrl( m_mainFrameUnite );
	m_mainFrameUnite->appendPane( m_peaksXYData, "Peak List", kome::window::WindowTool::DOCK_ALIGN_BOTTOM );	// @date 2011/06/15 <Mod> OKADA
		
	// active object
	kome::objects::Spectrum* spec = NULL;
	kome::objects::Chromatogram* chrom = NULL;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );

		spec = obj.getSpectrum();
		chrom = obj.getChromatogram();
	}

	// set object
	selectSpectrum( spec );
	selectChromatogram( chrom );

	// active group
	kome::objects::DataGroupNode* group = NULL;
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		group = obj.getGroup();
	}

	return;
}

// select spectrum
void XYDataViewManager::selectSpectrum( kome::objects::Spectrum* spec ) {
	// アクティブなオブジェクトが変わらなかったら処理を抜ける @date 2012.06.27 <Mod> M.Izumi
	if( m_currentSpec == spec ){
		return;
	}
	
	// check the member
	m_currentSpec = spec;

	if( spec == NULL && m_currentChrom != NULL ) {
		return;
	}
	m_currentChrom = NULL;

	// main frame
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// get xy data
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if( m_dataXYData != NULL ) {
		kome::core::XYData* xyData = NULL;
		if( spec != NULL ) {
			xyData = spec->getXYData();
		}

		m_dataXYData->setData( spec, xyData, false );
		std::string title = "Data Points";
		if( xyData != NULL ) {
			title.append( FMT( " (%d points)", xyData->getLength() ) );
		}
		m_dataXYData->setTitle( title.c_str() );
	}
	if( m_peaksXYData != NULL ) {
		kome::objects::Peaks* peaks = NULL;
		if( spec != NULL ) {
			peaks = pkMgr.getPeaks( spec );
		}

		m_peaksXYData->setData( spec, peaks, true );
		std::string title = "Peak List";
		if( peaks != NULL ) {
			title.append( FMT( " (%d peaks)", peaks->getLength() ) );
		}

		m_peaksXYData->setTitle( title.c_str() );
	}
}

// select chromatogram
void XYDataViewManager::selectChromatogram( kome::objects::Chromatogram* chrom ) {
	// アクティブなオブジェクトが変わらなかったら処理を抜ける @date 2012.06.27 <Mod> M.Izumi
	if( m_currentChrom == chrom ){
		return;
	}
	// check the member
	m_currentChrom = chrom;
	
	if( chrom == NULL && m_currentSpec != NULL ) {
		return;
	}
	m_currentSpec = NULL;

	// main frame
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// get xy data
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if( m_dataXYData != NULL ) {
		kome::core::XYData* xyData = NULL;
		if( chrom != NULL ) {
			xyData = chrom->getXYData();
		}

		m_dataXYData->setData( chrom, xyData, false );
		std::string title = "Data Points";
		if( xyData != NULL ) {
			title.append( FMT( " (%d points)", xyData->getLength() ) );
		}
		m_dataXYData->setTitle( title.c_str() );
	}
	if( m_peaksXYData != NULL ) {
		kome::objects::Peaks* peaks = NULL;
		if( chrom != NULL ) {
			peaks = pkMgr.getPeaks( chrom );
		}

		m_peaksXYData->setData( chrom, peaks, true );
		std::string title = "Peak List";
		if( peaks != NULL ) {
			title.append( FMT( " (%d peaks)", peaks->getLength() ) );
		}
		m_peaksXYData->setTitle( title.c_str() );
	}
}

// toggle the visibility of the data grid
void XYDataViewManager::toggleDataVisibility() {
	// check the member
	if( m_dataXYData == NULL ) {
		createPointsView();
		return;
	}

	// frame 
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_dataXYData );
}

// toggle the visibility of the peaks grid
void XYDataViewManager::togglePeaksVisibility() {
	// check the member
	if( m_peaksXYData == NULL ) {
		createPeaksView();
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_peaksXYData );
}

// check the visibility of the data grid view
bool XYDataViewManager::isVisiblePointsXYPointsView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_dataXYData );
}

// check the visibility of the peaks grid view
bool XYDataViewManager::isVisiblePeaksXYPointsView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_peaksXYData );
}

// update spectrum grid view
void XYDataViewManager::updateSpectrumXYPointsView( kome::objects::Spectrum* spec ) {
	if( spec != m_currentSpec ) {
		return;
	}

	m_currentSpec = NULL;

	selectSpectrum( spec );
}

// update chromatogram grid view
void XYDataViewManager::updateChromatogramXYPointsView( kome::objects::Chromatogram* chrom ) {
	if( chrom != m_currentChrom ) {
		return;
	}

	m_currentChrom = NULL;

	selectChromatogram( chrom );
}

// release view
void XYDataViewManager::releaseView() {
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// unset
	selectSpectrum( NULL );
	selectChromatogram( NULL );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	if( m_dataXYData != NULL ) {
		statusMgr.setValue(
			DATA_XY_DATA_STATUS_NAME,
			( mainFrame != NULL && mainFrame->isVisibleBar( m_dataXYData ) ) ? "on" : "off"
		);
	}
	if( m_peaksXYData != NULL ) {
		statusMgr.setValue(
			PEAKS_XY_DATA_STATUS_NAME,
			( mainFrame != NULL && mainFrame->isVisibleBar( m_peaksXYData ) ) ? "on" : "off"
		);
	}

	// release
	m_dataXYData = NULL;
	m_peaksXYData = NULL;

}

// on close sample
void XYDataViewManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// current sample
	kome::objects::Sample* currentSample = getCurrentSample();
	if( sample == NULL || currentSample == NULL ) {
		return;
	}

	// unset
	if( currentSample == sample ) {
		selectSpectrum( NULL );
		selectChromatogram( NULL );
	}
}

// on close spectrum
void XYDataViewManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	// check the current spectrum
	if( m_currentSpec == NULL || spec == NULL ) {
		return;
	}

	// unset
	if( m_currentSpec == spec ) {
		selectSpectrum( NULL );
		selectChromatogram( NULL );
	}
}

// on close chromatogram
void XYDataViewManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	// check the current chromatogram
	if( m_currentChrom == NULL || chrom == NULL ) {
		return;
	}

	// unset
	if( m_currentChrom == chrom ) {
		selectSpectrum( NULL );
		selectChromatogram( NULL );
	}
}

// get instance
XYDataViewManager& XYDataViewManager::getInstance() {
	// create object (This is the only object.)
	static XYDataViewManager mgr;

	return mgr;
}

// @date 2011/05/12 <Add> OKADA ////////////////////////////////////////////////////////////
// toggle visibility of the peak list view
void XYDataViewManager::toggleVisibilityOfPeakListView() {

	togglePeaksVisibility();
}

// clear selected peaks
void XYDataViewManager::clearSelectedPeaks() {
	m_selectedPeaks.clear();
	m_selectedClusters.clear();
}

// add selected peak
void XYDataViewManager::addSelectedPeaks( kome::objects::Peak2DElement* peak ) {
	// add peak
	m_selectedPeaks.insert( peak );

	// add cluster
	kome::objects::PeaksCluster2D* cluster = peak->getCluster();
	if( cluster != NULL ) {
		m_selectedClusters.insert( cluster );
	}	
}

// check the peak list view is visible or not
bool XYDataViewManager::isVisiblePeakListView() {
	return isVisiblePeaksXYPointsView();
}

// create peaks view
void XYDataViewManager::createPeaksView2D() {
	createPeaksView();
}

void XYDataViewManager::selectGroup( kome::objects::DataGroupNode* group ) {
	// check the member
	if( m_peaksXYData == NULL ) {
		return;
	}

	// アクティブなオブジェクトが変わらなかったら処理を抜ける @date 2012.06.27 <Mod> M.Izumi
	if( m_group == group && m_currentChrom == NULL && m_currentSpec == NULL ) {
		return;
	}
	m_currentChrom = NULL;
	m_currentSpec = NULL;

	// main frame
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// select group
	m_group = group;
	m_peaksXYData->setGroup( group );

	// peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );

	// title
	std::string title = "Peak List";
	if( peaks != NULL ) {
		title.append( FMT( " (%d peaks, %d clusters)", peaks->getNumberOfPeaks(), peaks->getNumberOfClusters() ) );
	}
	m_peaksXYData->setTitle( title.c_str() );
	// refresh
	m_peaksXYData->Refresh();	
}


// update peak list view
void XYDataViewManager::updatePeakListView( kome::objects::DataGroupNode* group ) {
	// check 
	if( m_peaksXYData ==  NULL ) {
		return;
	}
	if( group != m_group ) {
		return;
	}

	// PeakList更新時titleも更新する // @date 2012/09/06 <Add> OKADA ------->
	// main frame
	kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// select group
	m_group = group;
	m_peaksXYData->setGroup( group );

	// peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );

	// title
	std::string title = "Peak List";
	if( peaks != NULL ) {
		title.append( FMT( " (%d peaks, %d clusters)", peaks->getNumberOfPeaks(), peaks->getNumberOfClusters() ) );
	}
	m_peaksXYData->setTitle( title.c_str() );
	// refresh
	m_peaksXYData->Refresh();	
	// @date 2012/09/06 <Add> OKADA <-------
}

void XYDataViewManager::onCloseDataMap( kome::objects::DataMapInfo* dataMap ){
	// check objects
	if( dataMap == NULL || m_group == NULL ) {
		return;
	}

	return;
}

// set peaks visibility
void XYDataViewManager::SetPeaksVisibility( int iFlgShowHide ){

	wxWindow* frame = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( frame );

	if( m_peaksXYData != NULL ){
		if( iFlgShowHide == 0 ){
			mainFrame->hideBar( m_peaksXYData );
		}else if( iFlgShowHide == 1 ){
			mainFrame->showBar( m_peaksXYData );
		}
	}

	return;
}

// set visibility of peak list view
void XYDataViewManager::SetVisibilityOfPeakListView( int iFlgShowHide ){

	wxWindow* frame = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( frame );

	if( m_peaksXYData != NULL ){
		if( iFlgShowHide == 0 ){
			mainFrame->hideBar( m_peaksXYData );
		}else if( iFlgShowHide == 1 ){
			mainFrame->showBar( m_peaksXYData );
		}
	}

	return;
}

// toggle the visibility of the peak grid
void XYDataViewManager::togglePeakVisibility( kome::objects::Parameters* param ) {
	int iFlgDisp = 0;

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	if( mainFrame->isVisibleBar( m_peaksXYData ) ){
		iFlgDisp = 1;
	}

	// check the member
	if( m_peaksXYData == NULL ) {
		createPeaksView();
	}

	if( iFlgDisp ){
		// 表示されているので表示をやめる
		mainFrame->hideBar( m_peaksXYData );
	}else{
		// 表示されていないので表示する
		mainFrame->showBar( m_peaksXYData );
	}
}

// check the peak list view is visible or not
bool XYDataViewManager::isVisibleXY2DPeaksView() {
	if( m_peaksXYData == NULL ) {
		return false;
	}

	wxWindow* frame = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( frame );
	if( mainFrame != NULL ) {	
		bool boolValXYData = mainFrame->isVisibleBar( (wxWindow *)m_peaksXYData );

		return ( boolValXYData );	
	}
	return false;
}


// Peaks2dManagerから移植	@date 2011.09.16 <Add> M.Izumi ->
// draw peaks
void XYDataViewManager::drawPeaks( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
	// color
	COLOR oldColor = g.getColor();

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataGroupNode* group = &( dataMap.getGroup() );
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL ) {
		return;
	}
	
	// samples
	kome::objects::Sample* dataSample = NULL;
	if( m_group != NULL ) {
		dataSample = m_group->getSample();
	}

	kome::objects::Sample* drawSample = dataMap.getSample();

	// draw selected peaks
	if( dataSample == drawSample ) {
		g.setColor( kome::img::ColorTool::FUCHSIA );
		for( std::set< kome::objects::Peak2DElement* >::iterator it = m_selectedPeaks.begin();
				it != m_selectedPeaks.end(); it++ ) {
			// peak
			kome::objects::Peak2DElement* peak = *it;
					
			// draw
			if( peak->getEndMz() >= g.getMinX()
					&& peak->getStartMz() <= g.getMaxX()
					&& peak->getEndRt() >= g.getMinY()
					&& peak->getStartRt() <= g.getMaxY() ){		// check range
				// draw
				drawPosition( g, peak->getStartRt(), peak->getEndRt(), peak->getStartMz(), peak->getEndMz() );
			}
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw clusters
void XYDataViewManager::drawClusters( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
	// color
	COLOR oldColor = g.getColor();

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::DataGroupNode* group = &( dataMap.getGroup() );
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL ) {
		return;
	}

	// samples
	kome::objects::Sample* dataSample = NULL;
	if( m_group != NULL ) {
		dataSample = m_group->getSample();
	}

	kome::objects::Sample* drawSample = dataMap.getSample();

	// draw selected clusters
	if( dataSample == drawSample ) {
		g.setColor( kome::img::ColorTool::FUCHSIA );

		for( std::set< kome::objects::PeaksCluster2D* > ::iterator it = m_selectedClusters.begin();
				it != m_selectedClusters.end(); it++ ) {
			// cluster
			kome::objects::PeaksCluster2D* cluster = *it;

			// draw
			if( cluster->getEndMz() >= g.getMinX()
					&& cluster->getStartMz() <= g.getMaxX()
					&& cluster->getEndRt() >= g.getMinY()
					&& cluster->getStartRt() <= g.getMaxY() ) {	// check range
				// draw position
				drawPosition( g, cluster->getStartRt(), cluster->getEndRt(), cluster->getStartMz(), cluster->getEndMz() );
			}
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw position
void XYDataViewManager::drawPosition(
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
// @date 2011.09.16 <Add> M.Izumi<-

