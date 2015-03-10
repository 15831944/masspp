/**
 * @file PeakEditManager.cpp
 * @brief implements of PeakEditManager class
 *
 * @author S.Tanaka
 * @date 2008.12.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakEditManager.h"
#include "PeakEditFunctions.h"

#include <float.h>

using namespace kome::peaks::edit;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_SHAPE_COLOR		kome::img::ColorTool::YELLOW
#define SELECTED_COLOR			kome::img::ColorTool::AQUA

#define NONE_EDIT_ID			 0
#define OLD_CTRL_EDIT_ID		 1
#define NEW_CTRL_EDIT_ID		-1
#define ADD_EDIT_ID			  -100
#define BASE_CTRL_EDIT_ID		-2
#define BASE_LEFT_EDIT_ID		-3
#define BASE_RIGHT_EDIT_ID		-4
#define LEFT_EDIT_ID			-5
#define RIGHT_EDIT_ID			-6


// constructor
PeakEditManager::PeakEditManager() : m_startPt( -1, -1 ) {
	// initialize
	unselectPeak();

	m_mode = MODE_NONE;
	m_flgInAdd = 0;
	m_flgSelect = 0;
	m_flgSelectSpec = 0;
	m_flgSelectChrom = 0;
	m_flgRedrawMemory = false;
	m_flgDeleteLast = false;
}

// destructor
PeakEditManager::~PeakEditManager() {
}

// unselect peak
void PeakEditManager::unselectPeak() {
	m_obj.type = kome::objects::Variant::OTHER;
	m_obj.prim.pt = NULL;

	m_peak = NULL;
	m_peakOnClick = NULL;	// @date 2011/10/14 <Add> OKADA

	m_ctrlPts.clear();

	m_editing = NONE_EDIT_ID;
}

// select spectrum peak
void PeakEditManager::setSelectedPeak(
		kome::objects::Spectrum& spec,
		kome::objects::PeakElement& peak
) {
	// xy data
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	kome::core::XYData* xyData = ptMgr.getXYData( &spec );

	// set spectrum
	m_obj.type = kome::objects::Variant::SPECTRUM;
	m_obj.prim.pt = &spec;

	// set peak
	m_peak = &peak;

	// control points
	setControlPoints( *xyData );
}

// select chromatogram peak
void PeakEditManager::setSelectedPeak(
		kome::objects::Chromatogram& chrom,
		kome::objects::PeakElement& peak
) {
	// xy data
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	kome::core::XYData* xyData = ptMgr.getXYData( &chrom );

	// set chromatogram
	m_obj.type = kome::objects::Variant::CHROMATOGRAM;
	m_obj.prim.pt = &chrom;

	// set peak
	m_peak = &peak;

	// control points
	setControlPoints( *xyData );
}

// get selected peak
kome::objects::PeakElement* PeakEditManager::getSelectedPeak() {
	return m_peak;
}

// get selected spectrum
kome::objects::Spectrum* PeakEditManager::getSelectedSpectrum() {
	// get spectrum
	kome::objects::MsDataVariant obj( m_obj );
	kome::objects::Spectrum* spec = obj.getSpectrum();

	return spec;
}

// get selected chromatogram
kome::objects::Chromatogram* PeakEditManager::getSelectedChromatogram() {
	// get chromatogram
	kome::objects::MsDataVariant obj( m_obj );
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	return chrom;
}

// set control points
void PeakEditManager::setControlPoints( kome::core::XYData& xyData ) {
	// clear
	m_ctrlPts.clear();
	if( m_peak == NULL ) {
		return;
	}

	// set baseline
	m_ctrlPts.resize( 2 );

	m_ctrlPts[ 0 ].x = m_peak->getLeftX();
	m_ctrlPts[ 0 ].y = m_peak->getLeftY();
	m_ctrlPts[ 0 ].edit = BASE_LEFT_EDIT_ID;

	m_ctrlPts[ 1 ].x = m_peak->getRightX();
	m_ctrlPts[ 1 ].y = m_peak->getRightY();
	m_ctrlPts[ 1 ].edit = BASE_RIGHT_EDIT_ID;
}


bool PeakEditManager::isChangeSpectrum( void )
{
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	bool bChangedFlag = false;
	do{	

		kome::objects::Spectrum* pSpec = getSelectedSpectrum();
		kome::objects::Peaks* backup = &( m_specPeaksMap[ pSpec ] );

		kome::objects::Peaks* peaks = pkMgr.getPeaks( pSpec );
		if( peaks == NULL ) {
			bChangedFlag = true;
			break;
		}

		unsigned int unNewSize = backup->getLength();
		unsigned int unOldSize = peaks->getLength();
		
		if( unNewSize != unOldSize ){
			bChangedFlag = true;
			break;
		}

		for( unsigned int i = 0; i<unNewSize; i++ ){
			double dNewX = backup->getX( i );
			double dNewY = backup->getY( i );
			double dOldX = peaks->getX( i );
			double dOldY = peaks->getY( i );
			double leaveX = abs(dNewX - dOldX);
			double leaveY = abs(dNewY - dOldY);

			if (( leaveX < 0.000001 ) &&
				( leaveY < 0.000001 ) )
			{
				;	// 一致したとみなす
			}else{
				// 不一致
				bChangedFlag = true;
				break;
			}
		
		}

		volatile int x = 0;
	
	}while( false );

	return bChangedFlag;
}


bool PeakEditManager::isChangeChromatogram( void )
{
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	bool bChangedFlag = false;
	do{	

		kome::objects::Chromatogram* pChrom = getSelectedChromatogram();
		kome::objects::Peaks* backup = &( m_chromPeaksMap[ pChrom ] );

		kome::objects::Peaks* peaks = pkMgr.getPeaks( pChrom );
		if( peaks == NULL ) {
			bChangedFlag = true;
			break;
		}

		unsigned int unNewSize = backup->getLength();
		unsigned int unOldSize = peaks->getLength();
		
		if( unNewSize != unOldSize ){
			bChangedFlag = true;
			break;
		}

		for( unsigned int i = 0; i<unNewSize; i++ ){
			double dNewX = backup->getX( i );
			double dNewY = backup->getY( i );
			double dOldX = peaks->getX( i );
			double dOldY = peaks->getY( i );
			double leaveX = abs(dNewX - dOldX);
			double leaveY = abs(dNewY - dOldY);

			if (( leaveX < 0.000001 ) &&
				( leaveY < 0.000001 ) )
			{
				;	// 一致したとみなす
			}else{
				// 不一致
				bChangedFlag = true;
				break;
			}
		
		}

		volatile int x = 0;
	
	}while( false );

	return bChangedFlag;
}

// set label mode
void PeakEditManager::setMode( const LabelMode mode ) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	bool bFlgSpec = false;
	bool bFlgChrom = false;
	if( mode == MODE_NONE && m_mode != MODE_NONE ){
		if( m_specPeaksMap.size() > 0 ){
			bFlgSpec = isChangeSpectrum();
		}
		if( m_chromPeaksMap.size() > 0 ){
			bFlgChrom = isChangeChromatogram();
		}
	}
	
	// commit, rollback
	if( mode == MODE_NONE && m_mode != MODE_NONE && ( bFlgSpec || bFlgChrom ) ) {		
		askCommitOrRollback();
	}

	// set mode
	m_mode = mode; 
}

// get label mode
PeakEditManager::LabelMode PeakEditManager::getMode() {
	return m_mode;
}

// delete selected peak
void PeakEditManager::deletePeak() {
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	kome::objects::Peaks* peaks = NULL;

	kome::objects::Spectrum* spec = getSelectedSpectrum();
	if( spec != NULL ) {
		peaks = pkMgr.getPeaks( spec );
	}

	kome::objects::Chromatogram* chrom = getSelectedChromatogram();
	if( chrom != NULL ) {
		peaks = pkMgr.getPeaks( chrom );
	}

	// delete
	if( peaks != NULL ) {
		int idx = -1;
		for( int i = 0; i < (int)peaks->getLength() && idx < 0; i++ ) {
			if( peaks->getPeak( i ) == m_peak ) {
				idx = i;
			}
		}

		if( idx >= 0 ) {
			peaks->deletePoint( idx );
		}
	}

	// unselect
	unselectPeak();

	// update
	if( spec != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	}
	if( chrom != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );
	}

	kome::window::WindowTool::refresh();
}

// draw peak position
void PeakEditManager::drawPeakPosition(
	   kome::img::Graphics& g,
	   kome::objects::Peaks& peaks,
	   kome::core::XYData& xyData
) {
	// old values
	COLOR oldColor = g.getColor();
	kome::img::Graphics::DrawStyle oldStyle = g.getDrawStyle();

	// set color
	g.setColor( kome::img::ColorTool::BLACK );
	g.setDrawStyle( kome::img::Graphics::STYLE_DOT );

	// draw position
	for( unsigned int i = 0; i < peaks.getLength(); i++ ) {
		kome::objects::PeakElement* peak = peaks.getPeak( i );
		drawPeakPosition( g, *peak, xyData );
	}

	// restore
	g.setColor( oldColor );
	g.setDrawStyle( oldStyle );
}

// draw peak shape
void PeakEditManager::drawPeakShape(
	   kome::img::Graphics& g,
	   kome::objects::Peaks& peaks,
	   kome::core::XYData& xyData
) {
	// old values
	COLOR oldColor = g.getColor();

	// set color
	g.setColor( kome::img::ColorTool::BLACK );
	g.setDrawStyle( kome::img::Graphics::STYLE_DOT );

	// draw shape
	bool drawSelected = false;

	g.setColor( PEAK_SHAPE_COLOR );
	for( unsigned int i = 0; i < peaks.getLength(); i++ ) {
		kome::objects::PeakElement* peak = peaks.getPeak( i );
		if( peak == m_peak ) {	// selected peak
			drawSelected = true;
		}
		else {
			drawPeakShape( g, *peak, xyData );
		}
	}

	// draw selected
	if( drawSelected ) {
		g.setColor( SELECTED_COLOR );

		drawPeakShape( g, *m_peak, xyData );

		m_flgSelect = 1;	// @date 2011/06/23 <Add> OKADA
	}

	// restore
	g.setColor( oldColor );
}

// draw peak position
void PeakEditManager::drawPeakPosition(
	   kome::img::Graphics& g,
	   kome::objects::PeakElement& peak,
	   kome::core::XYData& xyData
) {
	// draw baseline
	const double lx = peak.getLeftX();
	const double ly = peak.getLeftY();
	const double rx = peak.getRightX();
	const double ry = peak.getRightY();

	const int idx0 = xyData.getNearestIndex( lx );
	const int idx1 = xyData.getNearestIndex( rx );

	const double x0 = xyData.getX( idx0 );
	const double y0 = xyData.getY( idx0 );
	const double x1 = xyData.getX( idx1 );
	const double y1 = xyData.getY( idx1 );

	double px = double();
	double py = double();

	std::vector< int > points;

	g.transformDataToPosition( x0, y0, &px, &py );
	points.push_back( roundnum( px ) );
	points.push_back( roundnum( py ) );

	g.transformDataToPosition( lx, ly, &px, &py );
	points.push_back( roundnum( px ) );
	points.push_back( roundnum( py ) );

	g.transformDataToPosition( rx, ry, &px, &py );
	points.push_back( roundnum( px ) );
	points.push_back( roundnum( py ) );


	g.transformDataToPosition( x1, y1, &px, &py );
	points.push_back( roundnum( px ) );
	points.push_back( roundnum( py ) );

	g.drawLines( &( points[ 0 ] ), points.size() / 2 );

	// draw apices
	if( m_peak == &peak ) {
		for( unsigned int i = 0; i < m_ctrlPts.size(); i++ ) {
			double px = double();
			double py = double();

			g.transformDataToPosition(
				m_ctrlPts[ i ].x,
				m_ctrlPts[ i ].y,
				&px,
				&py
			);

			m_ctrlPts[ i ].px = roundnum( px );
			m_ctrlPts[ i ].py = roundnum( py );

			g.fillRect(
				m_ctrlPts[ i ].px - 2,
				m_ctrlPts[ i ].py - 2,
				4,
				4
			);
		}
	}
}

// draw peak shape
void PeakEditManager::drawPeakShape(
	   kome::img::Graphics& g,
	   kome::objects::PeakElement& peak,
	   kome::core::XYData& xyData
) {
	// left, right
	const double lx = peak.getLeftX();
	const double ly = peak.getLeftY();
	const double rx = peak.getRightX();
	const double ry = peak.getRightY();

	const double w = rx - lx;
	if( w < 0.0000001 ) {
		return;
	}

	// baseline
	const double a = ( ry - ly ) / w;

	// index
	int idx0 = xyData.getNearestIndex( lx );
	int idx1 = xyData.getNearestIndex( rx );

	// draw
	std::vector< int > points;

	double prevX = -1.0;
	double prevY = -1.0;
	double prevBy = -1.0;
	double px = double();
	double py = double();

	for( int i = idx0; i <= idx1; i++ ) {
		// coordinates
		const double x = xyData.getX( i );
		const double y = xyData.getY( i );
		const double by = a * ( x - lx ) + ly;
		const double d = fabs( x - prevX );

		if( y >= by ) {
			if( i == idx0 ) {    // baseline
				g.transformDataToPosition( x, by, &px, &py );
				points.push_back( roundnum( px ) );
				points.push_back( roundnum( py ) );
			}
			else if( prevY < prevBy ) {    // cross point
				const double hp = fabs( prevY - prevBy );
				const double h = fabs( y - by );
				const double tx = hp * d / ( h + hp );
				const double crossX = prevX + tx;
				const double crossY = a * ( crossX - lx ) + ly;
				g.transformDataToPosition( crossX, crossY, &px, &py );

				points.push_back( roundnum( px ) );
				points.push_back( roundnum( py ) );
			}

			// add point
			g.transformDataToPosition( x, y, &px, &py );
			points.push_back( roundnum( px ) );
			points.push_back( roundnum( py ) );

			// baseline
			if( i == idx1 ) {
				g.transformDataToPosition( x, by, &px, &py );
				points.push_back( roundnum( px ) );
				points.push_back( roundnum( py ) );

				// fill
				if( points.size() >= 6 ) {
					g.fillPolygon( &( points[ 0 ] ), points.size() / 2 );
				}
				points.clear();
			}
		}
		else if( i > idx0 ) {
			// cross ponit
			const double hp = fabs( prevY - prevBy );
			const double h = fabs( y - by );
			const double tx = hp * d / ( h + hp );
			const double crossX = prevX + tx;
			const double crossY = a * ( crossX - lx ) + ly;
			g.transformDataToPosition( crossX, crossY, &px, &py );

			points.push_back( roundnum( px ) );
			points.push_back( roundnum( py ) );

			// fill
			if( points.size() >= 6 ) {
				g.fillPolygon( &( points[ 0 ] ), points.size() / 2 );
			}
			points.clear();
		}

		// previous data
		prevX = x;
		prevY = y;
		prevBy = by;
	}
}

// start to add peak
bool PeakEditManager::startToAdd(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check event
	int cx = evt.x();
	int cy = evt.y();
	if( m_mode != MODE_ADD
			|| !evt.lbutton()
			|| cx < g.getGraphPositionLeft()
			|| cx > g.getGraphPositionRight()
			|| cy < g.getGraphPositionTop()
			|| cy > g.getGraphPositionBottom()
	) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		peaks = pkMgr.createPeaks( &spec );
	}
	peaks->setDataPoints( *xyData );
	backupPeaks( *peaks, m_specPeaksMap[ &spec ] );

	// add
	const bool ret = startToAdd( *xyData, *peaks, g, evt );

	// remember spectrum
	if( ret && m_peak != NULL ) {
		setSelectedPeak( spec, *m_peak );
	}

	return ret;
}

// start to add peak
bool PeakEditManager::startToAdd(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check event
	int cx = evt.x();
	int cy = evt.y();
	if( m_mode != MODE_ADD
			|| !evt.lbutton()
			|| cx < g.getGraphPositionLeft()
			|| cx > g.getGraphPositionRight()
			|| cy < g.getGraphPositionTop()
			|| cy > g.getGraphPositionBottom()
	) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		peaks = pkMgr.createPeaks( &chrom );
	}
	peaks->setDataPoints( *xyData );
	backupPeaks( *peaks, m_chromPeaksMap[ &chrom ] );

	// add
	const bool ret = startToAdd( *xyData, *peaks, g, evt );

	// remember chromatogram
	if( ret && m_peak != NULL ) {
		setSelectedPeak( chrom, *m_peak );
	}

	return ret;
}

// add peak
bool PeakEditManager::startToAdd(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// editing
	m_editing = ADD_EDIT_ID;

	// remember point
	int cx = evt.x();
	int cy = evt.y();
	m_startPt.px = cx;
	m_startPt.py = cy;

	// position
	double x0 = double();
	double y0 = double();
	g.transformPositionToData( (double)cx, (double)cy, &x0, &y0 );
	int idx = xyData.getNearestIndex( x0 );
	double x = xyData.getX( idx );
	double y = xyData.getY( idx );
	if( y0 > y ) {
		y0 = y;
	}

	// create new peak
	idx = peaks.searchIndex( x );
	if( idx < 0 ) {
		idx = - idx - 1;
	}
	peaks.insertPoint( idx, x, y );
	kome::objects::PeakElement* peak = peaks.getPeak( idx );
	peak->setApex( x, y );
	peak->setLeft( x0, y0 );
	peak->setRight( x0, y0 );
	m_peak = peak;

	return true;
}

// move cursor
bool PeakEditManager::moveAddingCursor(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getSpectrum() != &spec
				|| !evt.lbutton() || m_editing != ADD_EDIT_ID || m_mode != MODE_ADD ) {
		return false;
	}

	// move cursor
	return moveAddingCursor( *( spec.getXYData() ), g, evt );
}

// move cursor
bool PeakEditManager::moveAddingCursor(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getChromatogram() != &chrom
				|| !evt.lbutton() || m_editing != ADD_EDIT_ID || m_mode != MODE_ADD ) {
		return false;
	}

	// move cursor
	return moveAddingCursor( *( chrom.getXYData() ), g, evt );
}

// move cursor
bool PeakEditManager::moveAddingCursor(
		kome::core::XYData& xyData, 
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check data points
	if( xyData.getLength() == 0 ) {
		return false;
	}

	// position
	int lp = g.getGraphPositionLeft();
	int rp = g.getGraphPositionRight();
	int tp = g.getGraphPositionTop();
	int bp = g.getGraphPositionBottom();

	int cx = evt.x();
	int cy = evt.y();

	cx = CLAMP( cx, lp, rp );
	cy = CLAMP( cy, tp, bp );

	// left, right
	double x0 = double();
	double y0 = double();
	double x1 = double();
	double y1 = double();

	g.transformPositionToData( (double)m_startPt.px, (double)m_startPt.py, &x0, &y0 );
	g.transformPositionToData( (double)cx, (double)cy, &x1, &y1 );

	double lx = std::min( x0, x1 );
	double ly = ( x0 > x1 ? y1 : y0 );
	double rx = std::max( x0, x1 );
	double ry = ( x0 > x1 ? y0 : y1 );

	int lIdx = xyData.getNearestIndex( lx );
	lx = xyData.getX( lIdx );
	ly = std::min( ly, xyData.getY( lIdx ) );

	int rIdx = xyData.getNearestIndex( rx );
	rx = xyData.getX( rIdx );
	ry = std::min( ry, xyData.getY( rIdx ) );

	// peak position
	double ax = lx;
	double ay = ly;
	for( int i = lIdx; i <= rIdx; i++ ) {
		const double x = xyData.getX( i );
		const double y = xyData.getY( i );

		if( y > ay ) {
			ax = x;
			ay = y;
		}
	}

	// set peak
	m_peak->setX( getPeakPosition( xyData, lIdx, rIdx, ly, ry ) );
	m_peak->setY( ay );
	m_peak->setApex( ax, ay );
	m_peak->setLeft( lx, ly );
	m_peak->setRight( rx, ry );

	return true;
}

// move cursor
bool PeakEditManager::moveEditingCursor(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getSpectrum() != &spec
				|| !evt.lbutton() || m_mode != MODE_EDIT ) {
		return false;
	}

	// move cursor
	bool ret = moveEditingCursor( *( spec.getXYData() ), g, evt );
	if( ret ) {
		// ピークの節点■を動かしていることを検出した	// @date 2011/06/06 <Add> OKADA
		if( m_peak == NULL ) {
			unselectPeak();
		}
		else {

			// @date 2012/05/16 <Add> OKADA ------->
			// 幅0のピークの手動作成を抑制する	// SPEC 84519
			kome::objects::PeakElement* peak = getSelectedPeak();
			if( peak != NULL ){
				double dR = peak->getRightX();
				double dL = peak->getLeftX();

				static double dRXOld;
				static double dRYOld;
				static double dLXOld;
				static double dLYOld;

				double dWidth = dR - dL;
				int iii = 0;
				if( abs(dWidth) < 0.000001 ){
					// cancel
					peak->setRight( dRXOld, dRYOld );
					peak->setLeft( dLXOld, dLYOld );
					return  ret;
				}
				dRXOld = dR;
				dLXOld = dL;

				dRYOld = peak->getRightY();
				dLYOld = peak->getLeftY();
			}
			// @date 2012/05/16 <Add> OKADA <-------

			setSelectedPeak( spec, *m_peak );
			kome::plugin::PluginCallTool::onUpdateSpecPeaks( spec );
		}
	}

	return ret;
}

// move cursor
bool PeakEditManager::moveEditingCursor(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getChromatogram() != &chrom
				|| !evt.lbutton() || m_mode != MODE_EDIT ) {
		return false;
	}

	// move cursor
	bool ret = moveEditingCursor( *( chrom.getXYData() ), g, evt );
	if( ret ) {
		// ピークの節点■を動かしていることを検出した	// @date 2011/06/06 <Add> OKADA
		if( m_peak == NULL ) {
			unselectPeak();
		}
		else {

			// @date 2012/05/16 <Add> OKADA ------->
			// 幅0のピークの手動作成を抑制する	// SPEC 84519
			kome::objects::PeakElement* peak = getSelectedPeak();
			if( peak != NULL ){
				double dR = peak->getRightX();
				double dL = peak->getLeftX();

				static double dRXOld;
				static double dRYOld;
				static double dLXOld;
				static double dLYOld;

				double dWidth = dR - dL;
				int iii = 0;
				if( abs(dWidth) < 0.000001 ){
					// cancel
					peak->setRight( dRXOld, dRYOld );
					peak->setLeft( dLXOld, dLYOld );
					return  ret;
				}
				dRXOld = dR;
				dLXOld = dL;

				dRYOld = peak->getRightY();
				dLYOld = peak->getLeftY();
			}
			// @date 2012/05/16 <Add> OKADA <-------

			setSelectedPeak( chrom, *m_peak );
			kome::plugin::PluginCallTool::onUpdateChromPeaks( chrom );
		}
	}

	return ret;
}

// move cursor
bool PeakEditManager::moveEditingCursor(
		kome::core::XYData& xyData, 
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// peak
	if( m_peak == NULL ) {
		return false;
	}

	// position
	int cx = evt.x();
	int cy = evt.y();

	double x = double();
	double y = double();
	g.transformPositionToData( (double)cx, (double)cy, &x, &y );
	x = CLAMP( x, g.getMinX(), g.getMaxX() );
	y = CLAMP( y, g.getMinY(), g.getMaxY() );

	// index
	int lIdx = xyData.getNearestIndex( m_peak->getLeftX() );
	int rIdx = xyData.getNearestIndex( m_peak->getRightX() );
	if( lIdx == rIdx ) {
		return false;
	}

	double lx = xyData.getX( lIdx );
	double ly = xyData.getY( lIdx );
	double rx = xyData.getX( rIdx );
	double ry = xyData.getY( rIdx );

	// set
	if( m_editing == BASE_CTRL_EDIT_ID ) {	// base control						// @date 2011/02/17 <Del> OKADA ->
		x = CLAMP( x, lx, rx );
	}
	else if( m_editing == BASE_LEFT_EDIT_ID || m_editing == BASE_RIGHT_EDIT_ID
			|| m_editing == LEFT_EDIT_ID || m_editing == RIGHT_EDIT_ID ) {
		int idx = xyData.getNearestIndex( x );
		x = xyData.getX( idx );
		y = std::min( xyData.getY( idx ), y );

		lx = std::min( m_pt.px, x );
		ly = ( m_pt.px < x ? m_pt.py : y );
		rx = std::max( m_pt.px, x );
		ry = ( m_pt.px < x ? y : m_pt.py );

		m_peak->setLeft( lx, ly );
		m_peak->setRight( rx, ry );
		for( unsigned int i = 0; i < m_pts.size() / 3; i++ ) {
			kome::core::Point< double > leftPt = m_pts[ i * 3 ];
			kome::core::Point< double > ctrlPt = m_pts[ i * 3 + 1 ];
			kome::core::Point< double > rightPt = m_pts[ i * 3 + 2 ];

			double leftX = leftPt.px;
			double rightX = rightPt.px;
		}
	}
	else {
		return false;
	}					

	return true;
}

// finish to add peak
bool PeakEditManager::finishToAddPeak(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getSpectrum() != &spec
				|| !evt.lbutton() || m_editing != ADD_EDIT_ID) {
		return false;
	}

	// flag
	m_editing = NONE_EDIT_ID;

	// check the mode
	if( m_mode != MODE_ADD ) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		return false;
	}

	// edit
	const bool ret = finishToAddPeak( *xyData, *peaks, g, evt );
	if( ret ) {
		if( m_peak != NULL ) {
			setSelectedPeak( spec, *m_peak );
		}
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( spec );
	}

	return ret;
}

// finish to edit peak
bool PeakEditManager::finishToAddPeak(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getChromatogram() != &chrom
				|| !evt.lbutton() || m_editing != ADD_EDIT_ID ) {
		return false;
	}

	// flag
	m_editing = NONE_EDIT_ID;

	// check the mode
	if( m_mode != MODE_ADD ) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		return false;
	}

	// edit
	const bool ret = finishToAddPeak( *xyData, *peaks, g, evt );
	if( ret ) {
		if( m_peak != NULL ) {
			setSelectedPeak( chrom, *m_peak );
		}
		kome::plugin::PluginCallTool::onUpdateChromPeaks( chrom );
	}

	return ret;
}

// finish to edit peak
bool PeakEditManager::finishToAddPeak(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check position
	int cx = evt.x();
	int cy = evt.y();

	if( m_startPt.px == cx ) {
		int idx = -1;
		for( int i = 0; i < (int)peaks.getLength() && idx < 0; i++ ) {
			if( peaks.getPeak( i ) == m_peak ) {
				idx = i;
			}
		}

		if( idx >= 0 ) {
			peaks.deletePoint( idx );
		}
		unselectPeak();

		return true;
	}

	// set position
	int lIdx = xyData.getNearestIndex( m_peak->getLeftX() );
	int rIdx = xyData.getNearestIndex( m_peak->getRightX() );
	double base = m_peak->getApexY() / 2.0;

	double pos = 0.0;
	double w = 0.0;

	for( int i = lIdx; i <= rIdx; i++ ) {
		double x = xyData.getX( i );
		double y = xyData.getY( i ) - base;

		if( y > 0.0 ) {
			pos += x * y;
			w += y;
		}
	}

	if( w > 0.0 ) {
		pos /= w;
		m_peak->setX( pos );
	}

	return true;
}

// finish to add peak
bool PeakEditManager::finishToEditPeak(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getSpectrum() != &spec || !evt.lbutton() ) {
		return false;
	}

	// check the mode
	if( m_mode != MODE_EDIT ) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		return false;
	}

	// edit
	const bool ret = finishToEditPeak( *xyData, *peaks, g, evt );
	if( ret ) {
		if( m_peak != NULL ) {
			setSelectedPeak( spec, *m_peak );
		}
		kome::plugin::PluginCallTool::onUpdateSpecPeaks( spec );
	}
	m_editing = NONE_EDIT_ID;

	return ret;
}

// finish to edit peak
bool PeakEditManager::finishToEditPeak(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check the object
	kome::objects::MsDataVariant obj( m_obj );
	if( m_peak == NULL || obj.getChromatogram() != &chrom || !evt.lbutton() ) {
		return false;
	}

	// check the mode
	if( m_mode != MODE_EDIT ) {
		return false;
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		return false;
	}

	// edit
	const bool ret = finishToEditPeak( *xyData, *peaks, g, evt );
	if( ret ) {
		if( m_peak != NULL ) {
			setSelectedPeak( chrom, *m_peak );
		}
		kome::plugin::PluginCallTool::onUpdateChromPeaks( chrom );
	}
	m_editing = NONE_EDIT_ID;

	return ret;
}

// finish to edit peak
bool PeakEditManager::finishToEditPeak(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// check position
	int cx = evt.x();
	int cy = evt.y();

	if( m_startPt.px == cx ) {
		int idx = -1;
		for( int i = 0; i < (int)peaks.getLength() && idx < 0; i++ ) {
			if( peaks.getPeak( i ) == m_peak ) {
				idx = i;
			}
		}

		if( idx >= 0 ) {
			peaks.deletePoint( idx );
		}
		unselectPeak();

		return true;
	}

	// set
	if( m_editing == BASE_CTRL_EDIT_ID ) {	// base control
	}
	else if( m_editing == BASE_LEFT_EDIT_ID || m_editing == BASE_RIGHT_EDIT_ID
			|| m_editing == LEFT_EDIT_ID || m_editing == RIGHT_EDIT_ID ) {
	}
	else if( m_editing >= OLD_CTRL_EDIT_ID ) {
		int idx = m_editing - OLD_CTRL_EDIT_ID;

		double x = double();
		double y = double();
		g.transformPositionToData( (double)cx, (double)( cy + 1 ), &x, &y );
	}
	else {
		return false;
	}

	// set position
	int lIdx = xyData.getNearestIndex( m_peak->getLeftX() );
	int rIdx = xyData.getNearestIndex( m_peak->getRightX() );
	double base = m_peak->getApexY() / 2.0;

	double pos = 0.0;
	double w = 0.0;

	for( int i = lIdx; i <= rIdx; i++ ) {
		double x = xyData.getX( i );
		double y = xyData.getY( i ) - base;

		if( y > 0.0 ) {
			pos += x * y;
			w += y;
		}
	}

	if( w > 0.0 ) {
		pos /= w;
		m_peak->setX( pos );
	}

	return true;
}

// start to edit spectrum peak
bool PeakEditManager::startToEditPeak(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check event
	int cx = evt.x();
	int cy = evt.y();
	if( m_mode != MODE_EDIT
			|| !evt.lbutton()
			|| cx < g.getGraphPositionLeft()
			|| cx > g.getGraphPositionRight()
			|| cy < g.getGraphPositionTop()
			|| cy > g.getGraphPositionBottom()
	) {
		return false;	// グラフ制御領域外	// @date 2011/06/08 <Add> OKADA
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		return false;
	}

	// backup peaks
	if( m_specPeaksMap.find( &spec ) == m_specPeaksMap.end() ) {
		backupPeaks( *peaks, m_specPeaksMap[ &spec ] );
	}

	// unselect peak
	if( getSelectedSpectrum() != &spec ) {
		unselectPeak();
	}

	// start to edit
	const bool ret = startToEditPeak( *xyData, *peaks, g, evt );

	// remember spectrum
	if( ret && m_peak != NULL ) {
		setSelectedPeak( spec, *m_peak );
	}

	return ret;
}

// start to edit chromatogram peak
bool PeakEditManager::startToEditPeak(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// check event
	int cx = evt.x();
	int cy = evt.y();
	if( m_mode != MODE_EDIT
			|| !evt.lbutton()
			|| cx < g.getGraphPositionLeft()
			|| cx > g.getGraphPositionRight()
			|| cy < g.getGraphPositionTop()
			|| cy > g.getGraphPositionBottom()
	) {
		return false;	// グラフ制御領域外	// @date 2011/06/08 <Add> OKADA
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();
	if( xyData->getLength() == 0 ) {
		return false;
	}

	// peaks
	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		return false;
	}

	// backup peaks
	if( m_chromPeaksMap.find( &chrom ) == m_chromPeaksMap.end() ) {
		backupPeaks( *peaks, m_chromPeaksMap[ &chrom ] );
	}

	// unselect peak
	if( getSelectedChromatogram() != &chrom ) {
		unselectPeak();
	}

	// start to edit
	const bool ret = startToEditPeak( *xyData, *peaks, g, evt );

	// remember spectrum
	if( ret && m_peak != NULL ) {
		setSelectedPeak( chrom, *m_peak );
	}

	return ret;
}

// start to edit peak
bool PeakEditManager::startToEditPeak(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
) {
	// initialize
	m_editing = NONE_EDIT_ID;

	// cursor point
	int cx = evt.x();
	int cy = evt.y();

	double x = double();
	double y = double();
	g.transformPositionToData( (double)cx, (double)cy, &x, &y );

	// control point
	bool checkBase = true;
	ControlPoint* ctrlPt = NULL;
	int d = 10;
	for( unsigned int i = 0; i < m_ctrlPts.size(); i++ ) {
		int dx = abs( cx - m_ctrlPts[ i ].px );
		int dy = abs( cy - m_ctrlPts[ i ].py );

		// クリックした場所がｺﾝﾄﾛｰﾙpointから±3dot以内なら	// @date 2011/06/07 <Add> OKADA
		if( dx <= 3 && dy <= 3 ) {
			if( dx + dy < d ) {
				d = dx + dy;
				ctrlPt = &( m_ctrlPts[ i ] );	
				// ｺﾝﾄﾛｰﾙpointをクリックしたと見なす	// @date 2011/06/07 <Add> OKADA
				// ※m_ctrlPts[i].edit が BASE_LEFT_EDIT_ID または BASE_RIGHT_EDIT_ID なら、コントロールポイントの端点	// @date 2011/06/07 <Add> OKADA
			}
		}

		if( m_ctrlPts[ i ].edit == BASE_CTRL_EDIT_ID ) {
			checkBase = false;
		}
	}

	// check base line
	if( checkBase && m_peak != NULL && ctrlPt == NULL ) {
		bool flg = false;

		double lpx = double();
		double lpy = double();
		g.transformDataToPosition( m_peak->getLeftX(), m_peak->getLeftY(), &lpx, &lpy );

		double rpx = double();
		double rpy = double();
		g.transformDataToPosition( m_peak->getRightX(), m_peak->getRightY(), &rpx, &rpy );

		if( fabs( rpx - lpx ) < 0.01 ) {
			if( cy > roundnum( std::min( lpy, rpy ) ) - 1
					&& cy < roundnum( std::max( lpy, rpy ) ) + 1 ) {
				flg = true;
			}
		}
		else {
			double a = ( rpy - lpy ) / ( rpx - lpx );
			double b = rpy - a * rpx;

			if( abs( cy - roundnum( a * (double)cx + b ) ) <= 1 ) {
				flg = true;
			}
		}

		// add baseline control point
		if( flg ) {
			m_editing = BASE_CTRL_EDIT_ID;
			return true;
		}
	}

	// pick peak
	if( ctrlPt == NULL ) {
		kome::objects::PeakElement* oldPeak = m_peak;
		m_peak = pickPeak( xyData, peaks, g, cx, cy );
		m_editing = NONE_EDIT_ID;
		return ( m_peak != oldPeak );
	}
	else {
		m_editing = ctrlPt->edit;
	}

	// store points
	if( m_peak != NULL ) {
		if( m_editing != NONE_EDIT_ID ) {
			m_pts.clear();
		}

		if( m_editing == LEFT_EDIT_ID || m_editing == BASE_LEFT_EDIT_ID ) {
			m_pt.px = m_peak->getRightX();
			m_pt.py = m_peak->getRightY();
		}
		if( m_editing == RIGHT_EDIT_ID || m_editing == BASE_RIGHT_EDIT_ID ) {
			m_pt.px = m_peak->getLeftX();
			m_pt.py = m_peak->getLeftY();
		}
	}

	// add new control point
	if( m_editing == NEW_CTRL_EDIT_ID && m_peak != NULL ) {
		// index
		int idx = xyData.getNearestIndex( x );
		int lIdx = idx - 1;
		int rIdx = idx + 1;
		if( lIdx >= 0 && rIdx < (int)xyData.getLength() ) {
		}
	}

	return true;
}

// pick peak
kome::objects::PeakElement* PeakEditManager::pickPeak(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		kome::img::Graphics& g,
		const int px,
		const int py
) {
	// peak
	kome::objects::PeakElement* peak = NULL;

	// candidated peaks
	std::vector< kome::objects::PeakElement* > peaksArray;
	for( unsigned int i = 0; i < peaks.getLength(); i++ ) {
		kome::objects::PeakElement* peak = peaks.getPeak( i );
		if( peak->getRightX() >= g.getMinX() || peak->getLeftX() <= g.getMaxX() ) {
			peaksArray.push_back( peak );
		}
	}
	if( peaksArray.size() == 0 ) {
		return peak;
	}

	// pick
	double d = 0.0;
	for( unsigned int i = 0; i < peaksArray.size(); i++ ) {
		// candidated peak
		kome::objects::PeakElement* tmp = peaksArray[ i ];
		double tmpD = FLT_MAX;

		// apex
		double pax = double();
		double pay = double();
		g.transformDataToPosition( tmp->getApexX(), tmp->getApexY(), &pax, &pay );

		double dax = fabs( pax - (double)px );
		double day = fabs( pay - (double)py );

		double da = dax * dax + day * day;
		tmpD = std::min( da, tmpD );

		// left
		double plx = double();
		double ply = double();
		g.transformDataToPosition( tmp->getLeftX(), tmp->getLeftY(), &plx, &ply );

		double dlx = fabs( plx - (double)px );
		double dly = fabs( ply - (double)py );

		double dl = dlx * dlx + dly * dly;
		tmpD = std::min( dl, tmpD );

		// right
		double prx = double();
		double pry = double();
		g.transformDataToPosition( tmp->getRightX(), tmp->getRightY(), &prx, &pry );

		double drx = fabs( prx - (double)px );
		double dry = fabs( pry - (double)py );

		double dr = drx * drx + dry * dry;
		tmpD = std::min( dr, tmpD );

		// compare distance
		if( (double)px > plx - 2.0 && (double)px < prx + 2.0 ) {
			if( peak == NULL || tmpD < d ) {
				peak = tmp;
				d = tmpD;
			}
		}
	}

	return peak;
}

// commit spectrum peaks
void PeakEditManager::commitPeaks( kome::objects::Spectrum& spec ) {
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	if( m_specPeaksMap.find( &spec ) == m_specPeaksMap.end() ) {
		return;
	}
	
	kome::objects::Peaks* backup = &( m_specPeaksMap[ &spec ] );

	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		// erase from the map
		m_specPeaksMap.erase( &spec );	// Spec No.93399 :ピーク削除後にピーク保存するとビジーになる @date 2013.06.26 <Add> M.Izumi
		return;
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();

	// broadcast
	kome::plugin::PluginCallTool::onCommitSpecPeaks( spec, *xyData, *backup, *peaks );

	// erase from the map
	m_specPeaksMap.erase( &spec );
}

// commit chromatogram peaks
void PeakEditManager::commitPeaks( kome::objects::Chromatogram& chrom ) {
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	if( m_chromPeaksMap.find( &chrom ) == m_chromPeaksMap.end() ) {
		return;
	}
	kome::objects::Peaks* backup = &( m_chromPeaksMap[ &chrom ] );

	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		// erase from the map
		m_chromPeaksMap.erase( &chrom );	// Spec No.93399 :ピーク削除後にピーク保存するとビジーになる @date 2013.06.26 <Add> M.Izumi
		return;
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();

	// broadcast
	kome::plugin::PluginCallTool::onCommitChromPeaks( chrom, *xyData, *backup, *peaks );

	// erase from the map
	m_chromPeaksMap.erase( &chrom );
}

// commit peaks
void PeakEditManager::commitPeaks() {
	// commit spectrum peaks
	while( m_specPeaksMap.size() > 0 ) {
		commitPeaks( *( ( *( m_specPeaksMap.begin() ) ).first ) );
	}

	// commit chromatogram peaks
	while( m_chromPeaksMap.size() > 0 ) {
		commitPeaks( *( ( *( m_chromPeaksMap.begin() ) ).first ) );
	}
}

// rollback spectrum peaks
void PeakEditManager::rollbackPeaks( kome::objects::Spectrum& spec ) {
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	if( m_specPeaksMap.find( &spec ) == m_specPeaksMap.end() ) {
		return;
	}
	kome::objects::Peaks* backup = &( m_specPeaksMap[ &spec ] );

	kome::objects::Peaks* peaks = pkMgr.getPeaks( &spec );
	if( peaks == NULL ) {
		m_specPeaksMap.erase( &spec );	// Spec No.93399 :ピーク削除後にピーク保存するとビジーになる @date 2013.06.26 <Add> M.Izumi
		return;
	}

	// xy data
	kome::core::XYData* xyData = spec.getXYData();

	// rollback
	kome::objects::Peaks canceled;
	backupPeaks( *peaks, canceled );
	backupPeaks( *backup, *peaks );
	kome::plugin::PluginCallTool::onUpdateSpecPeaks( spec );
	kome::plugin::PluginCallTool::onRollbackSpecPeaks( spec, *xyData, *peaks, canceled );

	// delete from the map
	m_specPeaksMap.erase( &spec );
}

// rollback chromatogram peaks
void PeakEditManager::rollbackPeaks( kome::objects::Chromatogram& chrom ) {
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peaks
	if( m_chromPeaksMap.find( &chrom ) == m_chromPeaksMap.end() ) {
		return;
	}
	kome::objects::Peaks* backup = &( m_chromPeaksMap[ &chrom ] );

	kome::objects::Peaks* peaks = pkMgr.getPeaks( &chrom );
	if( peaks == NULL ) {
		m_chromPeaksMap.erase( &chrom );	// Spec No.93399 :ピーク削除後にピーク保存するとビジーになる @date 2013.06.26 <Add> M.Izumi
		return;
	}

	// xy data
	kome::core::XYData* xyData = chrom.getXYData();

	// rollback
	kome::objects::Peaks canceled;
	backupPeaks( *peaks, canceled );
	backupPeaks( *backup, *peaks );
	kome::plugin::PluginCallTool::onUpdateChromPeaks( chrom );
	kome::plugin::PluginCallTool::onRollbackChromPeaks( chrom, *xyData, *peaks, canceled );

	// delete from the map
	m_chromPeaksMap.erase( &chrom );
}

// rollback peaks
void PeakEditManager::rollbackPeaks() {
	// rollback spectrum peaks
	while( m_specPeaksMap.size() > 0 ) {
		rollbackPeaks( *( ( *( m_specPeaksMap.begin() ) ).first ) );
	}

	// rollback chromatogram peaks
	while( m_chromPeaksMap.size() > 0 ) {
		rollbackPeaks( *( ( *( m_chromPeaksMap.begin() ) ).first ) );
	}
}

// commit or rollback
void PeakEditManager::askCommitOrRollback() {
	// commit, roolback
	if( kome::window::WindowTool::showYesNo( "Save peaks?", "Save", "Don't save" ) ) {
		commitPeaks();
	}
	else {
		rollbackPeaks();
	}

	// clear backup
	m_specPeaksMap.clear();
	m_chromPeaksMap.clear();

	// refresh
	kome::window::WindowTool::refresh();
}

// rollback peaks
void PeakEditManager::backupPeaks( kome::objects::Peaks& src, kome::objects::Peaks& dst ) {
	// clear
	dst.clearPoints();

	// add peaks
	const unsigned int num = src.getLength();
	if( num > 0 ) {
		dst.reserve( num );
	}

	for( unsigned int i = 0; i < num; i++ ) {
		kome::objects::PeakElement* oldPeak  = src.getPeak( i );
		kome::objects::PeakElement* peak = dst.createPeak( oldPeak->getX(), oldPeak->getY() );

		peak->setLeft( oldPeak->getLeftX(), oldPeak->getLeftY() );
		peak->setRight( oldPeak->getRightX(), oldPeak->getRightY() );
		peak->setApex( oldPeak->getApexX(), oldPeak->getApexY() );
		peak->setArea( oldPeak->getArea() );
	}
}

// on close sample
void PeakEditManager::onExitSample( void  ) {
	// samples
	std::set< kome::objects::Sample* > sampleSet;

	for( std::map< kome::objects::Spectrum*, kome::objects::Peaks >::iterator it = m_specPeaksMap.begin();
			it != m_specPeaksMap.end(); it++ ) {
		sampleSet.insert( ( (*it).first )->getSample() );
	}
	for( std::map< kome::objects::Chromatogram*, kome::objects::Peaks >::iterator it = m_chromPeaksMap.begin();
			it != m_chromPeaksMap.end(); it++ ) {
		sampleSet.insert( ( (*it).first )->getSample() );
	}

	// commit, rollback
	if( !sampleSet.empty() ){
		askCommitOrRollback();
	}
}

// on close sample
void PeakEditManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// samples
	std::set< kome::objects::Sample* > sampleSet;

	for( std::map< kome::objects::Spectrum*, kome::objects::Peaks >::iterator it = m_specPeaksMap.begin();
			it != m_specPeaksMap.end(); it++ ) {
		sampleSet.insert( ( (*it).first )->getSample() );
	}
	for( std::map< kome::objects::Chromatogram*, kome::objects::Peaks >::iterator it = m_chromPeaksMap.begin();
			it != m_chromPeaksMap.end(); it++ ) {
		sampleSet.insert( ( (*it).first )->getSample() );
	}

	// commit, rollback
	if( sampleSet.find( sample ) != sampleSet.end() ) {
		askCommitOrRollback();
	}
}

// on close spectrum
void PeakEditManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	// commit or rollback
	if( m_specPeaksMap.find( spec ) != m_specPeaksMap.end() ) {
		askCommitOrRollback();
	}

	// unselect
	if( spec == getSelectedSpectrum() ) {
		unselectPeak();
	}
}

// on close chromatogram
void PeakEditManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	// commit or rollback
	if( m_chromPeaksMap.find( chrom ) != m_chromPeaksMap.end() ) {
		askCommitOrRollback();
	}

	// unselect
	if( chrom == getSelectedChromatogram() ) {
		unselectPeak();
	}
}

// get instance
PeakEditManager& PeakEditManager::getInstance() {
	// create object (This is the only object.)
	static PeakEditManager mgr;

	return mgr;
}


int PeakEditManager::isInAddArea(

		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt,
		int flgCallFromSpecChrom	// 0:Spectrum 1:Chromatogram
		) {
	// グラフ制御領域内で、編集可能でもなく削除エリアでもないとき追加可能とする

	// check event
	int cx = evt.x();
	int cy = evt.y();

	if( isInControlArea( g, evt ) ){
	}else{
		return false;	
	}

	if( isInEditArea( g, evt, flgCallFromSpecChrom ) ){
		return false;	// 編集可能なので追加可能とはしない
	}
	if( isInDeleteArea( g, evt, flgCallFromSpecChrom ) ){
		return false;	// 削除可能なので追加可能とはしない
	}

	return true;
}


int PeakEditManager::isInEditArea(
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt,
		int flgCallFromSpecChrom,	// 0:Spectrum 1:Chromatogram
		int flgNoChkSelect
		) {

	// check event
	int cx = evt.x();
	int cy = evt.y();

	int flgSelect;

	if( flgCallFromSpecChrom == CALL_FROM_SPECTRUM ){
		flgSelect = m_flgSelectSpec;
	}else if( flgCallFromSpecChrom == CALL_FROM_CHROMATOGRAM ){
		flgSelect = m_flgSelectChrom;
	}else{
		// error
		return false;	}


	if( isInControlArea( g, evt ) ){
		// // グラフ制御領域内
	}else{
		return false;	// グラフ制御領域外
	}

	// 2011/06/23 <DEBUG> 20:30 OKADA
	if( flgNoChkSelect != 0 ){
		// m_flgSelectはチェックしない
	}else{
		// m_flgSelectをチェック
		if( flgSelect != 1 ){

			return false;	// エリア選択されていないときは編集できないようにする
		}
	}

	double x = double();
	double y = double();
	g.transformPositionToData( (double)cx, (double)cy, &x, &y );

	// control point
	int d = 10;

	for( unsigned int i = 0; i < m_ctrlPts.size(); i++ ) {

		int ctrl_px;	ctrl_px = m_ctrlPts[ i ].px;
		int ctrl_py;	ctrl_py = m_ctrlPts[ i ].py;

		if(( m_ctrlPts[i].edit == BASE_LEFT_EDIT_ID ) || ( m_ctrlPts[i].edit == BASE_RIGHT_EDIT_ID )){
			;	// OK
		}else{
			continue;
		}

		double px;
		double py;
		g.transformDataToPosition( m_ctrlPts[ i ].x, m_ctrlPts[ i ].y, &px, &py );


		ctrl_px = (int)px;
		ctrl_py = (int)py;
		int dx = abs( cx - ctrl_px );
		int dy = abs( cy - ctrl_py );

		// クリックした場所がｺﾝﾄﾛｰﾙpointから±3dot以内なら	// @date 2011/06/07 <Add> OKADA
		if( dx <= 3 && dy <= 3 ) {
			if( dx + dy < d ) {
				d = dx + dy;
				// ｺﾝﾄﾛｰﾙpointをクリックしたと見なす	// @date 2011/06/07 <Add> OKADA
				// ※m_ctrlPts[i].edit が BASE_LEFT_EDIT_ID または BASE_RIGHT_EDIT_ID なら、コントロールポイントの端点	// @date 2011/06/07 <Add> OKADA
				if(( m_ctrlPts[i].edit == BASE_LEFT_EDIT_ID ) || ( m_ctrlPts[i].edit == BASE_RIGHT_EDIT_ID )){
					return true;
				}
			}
		}
	}

	return false;
}


int PeakEditManager::isInDeleteArea(
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt,
		int flgCallFromSpecChrom,	// 0:Spectrum 1:Chromatogram
		kome::objects::Parameters* params
		) {

	wxImage * p_wxiImage;
	
	if( params != NULL ){
		kome::objects::Variant retCanDeletePeak =  canDeletePeak( params );
		return retCanDeletePeak.prim.boolVal;
	}

	// check event
	int cx = evt.x();
	int cy = evt.y();

	if( isInControlArea( g, evt ) ){
		;	// グラフ制御領域内
	}else{
		return false;	// グラフ制御領域外
	}
	
	COLOR col;
	if( flgCallFromSpecChrom == CALL_FROM_SPECTRUM ){
		col  = m_graphicsSpec.getPixel( cx, cy );
		p_wxiImage = &wxiImageSpec;
	}else if( flgCallFromSpecChrom == CALL_FROM_CHROMATOGRAM ){
		col  = m_graphicsChrom.getPixel( cx, cy );
		p_wxiImage = &wxiImageChrom;
	}else{
		return false;	// ERROR
	}

	if( col == SELECTED_COLOR ){
		// 選択範囲内
		return true;
	}else{
		if( (*p_wxiImage).IsOk() ){
			int flgEnableImg = 0;
			int cntLoop = 0;
			do{
				if(	( cx < (*p_wxiImage).GetWidth() ) && ( cy < (*p_wxiImage).GetHeight() ) ){
					flgEnableImg = 1;
					COLOR col2 = kome::img::ColorTool::getColor( (*p_wxiImage).GetRed( cx, cy ), (*p_wxiImage).GetGreen( cx, cy ), (*p_wxiImage).GetBlue( cx, cy ) );
					if( col2 == SELECTED_COLOR ){
						// 選択範囲内
						return true;	// col と col2 の値は同じになるはずだが異なることがある。不可解な現象	// @date 2011/06/23 <Comment>
					}
					break;	//選択範囲外
				}else{
					// 画像作成中？
				}
				cntLoop++;
				if( 1000 < cntLoop ){
					break;	// 通常あり得ない
				}
			}while( flgEnableImg == 0 );

		}
	}

	return false;
}


int PeakEditManager::isInControlArea(
		kome::img::Graphics& g,
		kome::evt::MouseEvent& evt
		) {

	// check event
	int cx = evt.x();
	int cy = evt.y();

	if( m_mode != MODE_EDIT
		|| cx < g.getGraphPositionLeft()
		|| cx > g.getGraphPositionRight()
		|| cy < g.getGraphPositionTop()
		|| cy > g.getGraphPositionBottom()
	) {
		return false;	// グラフ制御領域外
	}

	return true;	// グラフ制御領域内
}


int PeakEditManager::setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName, wxWindow* canvas )
{
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::window::Canvas* acCanvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
	int px = evt.x();
	int py = evt.y();

	do{
		if( px >= graphics.getGraphPositionLeft()
				&& px <= graphics.getGraphPositionRight()
				&& py >= graphics.getGraphPositionTop()
				&& py <= graphics.getGraphPositionBottom()
		) {
			// グラフ表示内
		
			//マウスカーソル変更
			kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();
			kome::plugin::PluginInfo* info;
			kome::plugin::PluginIcon* icon;

			if( pluginName == NULL ){
				break;	// while()文の後ろに脱出
			}
			info = pluginMgr.getPlugin( pluginName );
			if( info == NULL ){
				break;	// while()文の後ろに脱出
			}
			icon = info->getIcon( iconName );
			if( icon == NULL ){
				break;	// while()文の後ろに脱出
			}
		
			wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
			wxImage img( bitmap.ConvertToImage() );
			int width = img.GetWidth();
			int height = img.GetHeight();

			// hotspot(アイコン中心)の設定
			if( strcmp( iconName, ICON_NAME_PEAK_ADD ) == 0 ){
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 5 );
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 21 );
			}else if( strcmp( iconName, ICON_NAME_PEAK_DEL ) == 0 ){
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 1 );
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 20 );
			}else if( strcmp( iconName, ICON_NAME_PEAK_EDIT ) == 0 ){
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, width/2);
				img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, height/2);
			}else{
				break;
			}
		
			// @date 2011/08/03 <Add> OKADA ------->
			// QuantitationダイアログではgetActiveFrame()しても取得できないため、呼び出し元からwindowを取得する
			if( canvas != NULL ){
				canvas->SetCursor(wxCursor(img));
			}else
			// @date 2011/08/03 <Add> OKADA <-------

			// This check is necessary to prevent a crash when no canvas is active.
			if (acCanvas != NULL)
			{
				acCanvas->SetCursor(wxCursor(img));
			}

			// 削除判定処理（マウスカーソルがDELからそれ以外に変わった場合に再描画フラグON）	// @date 2011/08/03 <Add> OKADA
			if( strcmp( iconName, ICON_NAME_PEAK_DEL ) == 0 ){
				m_flgDeleteLast = true;
			}else{
				if( m_flgDeleteLast ){
					m_flgRedrawMemory=true;
				}
				m_flgDeleteLast = false;
			}
	
			return 0;
		}
	}while(false);
	
	// @date 2011/08/03 <Add> OKADA ------->
	// QuantitationダイアログではgetActiveFrame()しても取得できないため、呼び出し元からwindowを取得する
	if( canvas != NULL ){
		canvas->SetCursor(wxCursor(wxCURSOR_ARROW));
	}else
	// @date 2011/08/03 <Add> OKADA <-------
	if (acCanvas != NULL)
	{
		// This check is necessary to prevent a crash when no canvas is active.
		acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
	}

	// 削除判定処理（マウスカーソルがDELからそれ以外に変わった場合に再描画フラグON）	// @date 2011/08/03 <Add> OKADA
	if( m_flgDeleteLast ){
		m_flgRedrawMemory=true;
	}
	m_flgDeleteLast = false;

	return 0;
}


int PeakEditManager::getInAddFlag( void ){

	return m_flgInAdd;
}


int PeakEditManager::setInAddFlag( int flg ){
	m_flgInAdd = flg;
	
	return m_flgInAdd;
}


void PeakEditManager::setModeNone( void ){
	setMode( MODE_NONE );
}


void PeakEditManager::setModeAdd( void ){
	setMode( MODE_ADD );
}


void PeakEditManager::setModeEdit( void ){
	setMode( MODE_EDIT );
}

// get the peak position
double PeakEditManager::getPeakPosition(
		kome::core::XYData& xyData,
		const int lIdx,
		const int rIdx,
		const double ly,
		const double ry
) {
	// peak position & intensity
	const double lx = xyData.getX( lIdx );
	const double rx = xyData.getX( rIdx );

	double a = ( ry - ly ) / ( rx - lx );
	double pos = 0.0;
	double intensity = 0.0;
	for( int i = lIdx; i <= rIdx; i++ ) {
		double tmpX = xyData.getX( i );
		double tmpY = xyData.getY( i );
		double baseY = a * ( tmpX - lx ) + ly;
		tmpY = tmpY - baseY;
		
		if( tmpY > 0 ) {
			intensity += tmpY;
			pos += tmpX * tmpY;
		}
	}
	
	if( intensity > 0.0 ) {
		pos /= intensity;
	}

	return pos;
}

