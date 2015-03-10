/**
 * @file PeakLabelDrawer.cpp
 * @brief implements of PeakLabelDrawer class
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakLabelDrawer.h"
#include "ThresholdSettingDialog.h"		// @date 2011.03.31 <Add> M.Izumi

#include <list>
#include <math.h>

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_LABEL_SIZE					8
#define LABEL_SPACE							2

#define SECTION								"Label Drawing"
#define CHROM_ACCURACY_PARAM_NAME			"CHROM_ACCURACY"
#define SPEC_ACCURACY_PARAM_NAME			"SPEC_ACCURACY"
#define LABEL_TYPE_PARAM_NAME				"LABEL_TYPE"
#define LABEL_TYPE_DIFFERENCE				"difference"
#define LABEL_FONT							"LABEL_FONT"

#define SECTION_2D_PEAK						"Drawing"
#define WIDTH_PEAK_2D						"WIDTH_PEAK_2D"
#define COLOR_PEAK_2D						"COLOR_PEAK_2D"
#define WIDTH_CLUSTER_2D					"WIDTH_CLUSTER_2D"
#define COLOR_CLUSTER_2D					"COLOR_CLUSTER_2D"
#define COLOR_DEF_PEAK						"aqua"
#define COLOR_DEF_CLUSTER					"lime"

#define SET_COLOR							kome::img::ColorTool::OLIVE	// @date 2011.03.31 <Add> M.Izumi
#define MARK_SIZE							10							// @date 2011.03.31 <Add> M.Izumi

// constructor
PeakLabelDrawer::PeakLabelDrawer() : m_font( DEFAULT_LABEL_SIZE ) {
	m_changing.leftFlg = false;
	m_changing.draggingFlg = false;
	m_changing.endPos	 = -1;
	m_specThreshold = 0.0;
	m_chromThreshold = 0.0;
	m_bflg = false;
	
	// peak label
	m_selPeakLabelSpec = 0.0;
	m_selPeakLabelChrom = 0.0;
	m_specRectListMap.clear();
	m_chromRectListMap.clear();
}

// destructor
PeakLabelDrawer::~PeakLabelDrawer() {
	// delete drawing data
	for( std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* >::iterator it = m_specDrawingMap.begin();
			it != m_specDrawingMap.end(); it++ ) {
		delete (*it).second;
	}
	for( std::map< kome::objects::Chromatogram*, kome::img::DrawingXYData* >::iterator it = m_chromDrawingMap.begin();
			it != m_chromDrawingMap.end(); it++ ) {
		delete (*it).second;
	}	
}

// create and get drawing data
kome::img::DrawingXYData* PeakLabelDrawer::getDrawingData( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return NULL;
	}

	// peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ) {
		return NULL;
	}

	// drawing data
	kome::img::DrawingXYData* drawing = NULL;

	if( m_specDrawingMap.find( spec ) == m_specDrawingMap.end() ) {  // create
		drawing = new kome::img::DrawingXYData( *peaks );
		m_specDrawingMap[ spec ] = drawing;
	}
	else {
		drawing = m_specDrawingMap[ spec ];
	}

	return drawing;
}

// create and get drawing data
kome::img::DrawingXYData* PeakLabelDrawer::getDrawingData( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return NULL;
	}

	// peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ) {
		return NULL;
	}

	// drawing data
	kome::img::DrawingXYData* drawing = NULL;

	if( m_chromDrawingMap.find( chrom ) == m_chromDrawingMap.end() ) {  // create
		drawing = new kome::img::DrawingXYData( *peaks );
		m_chromDrawingMap[ chrom ] = drawing;
	}
	else {
		drawing = m_chromDrawingMap[ chrom ];
	}

	return drawing;
}

// draw label
void PeakLabelDrawer::drawLabel(
		kome::objects::Spectrum* spec,
		kome::img::Graphics* g
) {
	// check members
	if( spec == NULL || g == NULL ) {
		return;
	}

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ) {
		return;
	}

	// accuracy 
	int accuracy = getAccuracy( SPEC_ACCURACY_PARAM_NAME );

	// get xy data
	kome::core::XYData* xyData = spec->getXYData();

	// get drawing data
	kome::img::DrawingXYData* drawing = getDrawingData( spec );
	
	m_rectList.clear();

	// draw label
	drawLabel( drawing, accuracy, peaks, xyData, g, false );	// @date 2011.03.29 <Mod> M.Izum
		
	m_specRectListMap[ spec ] = m_rectList;
}

// draw label
void PeakLabelDrawer::drawLabel(
		kome::objects::Chromatogram* chrom,
		kome::img::Graphics* g
) {
	// check members
	if( chrom == NULL || g == NULL ) {
		return;
	}

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ) {
		return;
	}

	// accuracy 	
	int accuracy = getAccuracy( CHROM_ACCURACY_PARAM_NAME );

	// get xy data
	kome::core::XYData* xyData = chrom->getXYData();

	// get drawing data
	kome::img::DrawingXYData* drawing = getDrawingData( chrom );
	
	m_rectList.clear();

	// draw label
	drawLabel( drawing, accuracy, peaks, xyData, g , true);	// @date 2011.03.29 <Mod> M.Izum

	m_chromRectListMap[ chrom ] = m_rectList;
}								

// draw label
void PeakLabelDrawer::drawLabel(
		kome::img::DrawingXYData* drawing,
		const int accuracy,
		kome::objects::Peaks* peaks,
		kome::core::XYData* xyData,
		kome::img::Graphics* g,
		bool bchromflg						// 引数追加 @date 2011.03.29 <Add> M.Izum
) {
	// check the parameter
	if( drawing == NULL || xyData == NULL || g == NULL ) {
		return;
	}

	// get label type
	std::string labelType = getLabelType( LABEL_TYPE_PARAM_NAME );
	if( labelType.compare( "none" ) == 0 || labelType.empty() ){
		return;
	}
	
	// get data
	std::vector< std::pair< double, double > > xyArray;

	unsigned short usLevel;
	usLevel = drawing->getLevel( g->getGraphWidth(), g->getMinX(), g->getMaxX() );

	usLevel += 1;	// label表示であるので、細かい表示は不要
		
	unsigned int uiLength = drawing->getLength( usLevel );

	for( unsigned int i = 0; i < uiLength; i++ ) {

		double dTopX = drawing->getTopX( usLevel, i );
		double dTopY = drawing->getTopY( usLevel, i );

		if( dTopX > g->getMinX() && dTopX < g->getMaxX() ) {
			xyArray.push_back( std::make_pair( dTopX, dTopY ) );
		}
	}
	
	// set color
	COLOR oldColor = g->getColor();
	kome::img::Font oldFont = g->getFont();

	g->setColor( kome::img::ColorTool::BLACK );
	g->setFont( m_font );
	
	double peakLabel = 0.0;
	if( bchromflg ){
		peakLabel = m_selPeakLabelChrom;
	}else{
		peakLabel = m_selPeakLabelSpec;
	}

	// draw label
	for( unsigned i = 0; i < xyArray.size(); i++ ) {
		// x, y
		double x = xyArray[ i ].first;
		double y = xyArray[ i ].second;

		// label
		std::string label;
		bool bflg = false;
		double tmpx = x;
	
		// RT / Mass
		if( labelType.compare( "rt_mass" ) == 0 ){
			label = FMT( "%d", roundnum( x ) );

		// Difference
		}else if( labelType.compare( LABEL_TYPE_DIFFERENCE ) == 0 ){

			if( peakLabel == 0.0 ){
				( bchromflg == true ? m_selPeakLabelChrom = x : m_selPeakLabelSpec = x );
				label = FMT( "%d", roundnum( x ) );
				peakLabel = x;
			}else{

				tmpx = x - peakLabel;
				if( tmpx == 0.0 ){
					tmpx = x;
				}
								
				label = FMT( "%d", roundnum(tmpx) );

				bflg = true;
			}
			
		}
		
		if( accuracy > 0 ) {
			std::string lblFmt = FMT( "%%.%df", accuracy );
			label = FMT( lblFmt.c_str(), tmpx );
			if( tmpx > 0.0 && peakLabel != x && bflg ){
				label = FMT( "+%s", label.c_str() );
			}
		}
	
		// get position
		double px = double();
		double py = double();
		
		g->transformDataToPosition( x, y, &px, &py );

		px = floor( px + 0.5 );
		py = floor( py + 0.5 );

		// get size
		int w = int();
		int h = int();
		g->getTextSize( label.c_str(), &w, &h );

		int l = (int)px - w / 2;
		if( l < g->getGraphPositionLeft() + LABEL_SPACE ) {
			l = g->getGraphPositionLeft() + LABEL_SPACE;
		}
		int r = l + w;
		if( r > g->getGraphPositionRight() - LABEL_SPACE ) {
			r = g->getGraphPositionRight() - LABEL_SPACE;
			l = r - w;
		}

		int b = (int)py;
		int t = (int)py - h;
		
		// check labeling position
		bool canDraw = ( l >= g->getGraphPositionLeft() && r <= g->getGraphPositionRight()
							&& t >= g->getGraphPositionTop() && b <= g->getGraphPositionBottom() );
		for( std::list< std::pair< kome::core::Rect< int >, double > >::iterator it = m_rectList.begin(); it != m_rectList.end() && canDraw; it++ ) {
			kome::core::Rect< int >& rect = (*it).first;
			if( l <= rect.right && r >= rect.left && b >= rect.top && t <= rect.bottom ) {
				canDraw = false;
			}
		}
		
		// 閾値の値セット
		if( canDraw ){
			double endPos;
			if( bchromflg ){
				endPos = m_chromThreshold;
			}else{
				endPos = m_specThreshold;
			}

			if( endPos > y && endPos > 0.0){
				canDraw = false;
			}
		}

		// draw label
		if( canDraw ) {
			// フォントの設定をiniファイルから取得する為の処理追加 @date 2013.04.26 <Add> M.izumi ->
			kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, LABEL_FONT );
			g->setFont( font );
			// @date 2013.04.26 <Add> <-
			g->setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
			g->setTextVAlign( kome::img::Graphics::VALIGN_TOP );

			g->drawText(
				label.c_str(),
				l,
				t
			);

			// ラベルの表示範囲とX値
			kome::core::Rect< int > rect( l, t, r, b );
			std::pair< kome::core::Rect<int>, double >tmp;
			tmp.first= rect;
			tmp.second = x;
			
			m_rectList.push_back( tmp );
		}
	}
	
	// recover color
	g->setColor( oldColor );
	// recover font 
	g->setFont( oldFont );
}

// prepare to draw
void PeakLabelDrawer::prepareDraw(
		kome::objects::Spectrum* spec,
		kome::img::Graphics* g
) {
	// check the member
	if( spec == NULL || g == NULL ) {
		return;
	}

	// get label height
	int width = int();
	int height = int();
	g->getTextSize( "|", &width, &height );

	// change margin
	int margin = height + LABEL_SPACE;
	if( g->getGraphMarginTop() < margin ) {
		g->setGraphMarginTop( margin );
	}
}

// prepare to draw
void PeakLabelDrawer::prepareDraw(
		kome::objects::Chromatogram* chrom,
		kome::img::Graphics* g
) {
	// check the member
	if( chrom == NULL || g == NULL ) {
		return;
	}

	// get peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ) {
		return;
	}

	// get label height
	int width = int();
	int height = int();
	g->getTextSize( "|", &width, &height );

	// change margin
	int margin = height + LABEL_SPACE;
	if( g->getGraphMarginTop() < margin ) {
		g->setGraphMarginTop( margin );
	}
}

// delete drawing data (spectrum)
void PeakLabelDrawer::deleteDrawData( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// check the map
	if( m_specDrawingMap.find( spec ) == m_specDrawingMap.end() ) {
		return;
	}

	delete m_specDrawingMap[ spec ];
	m_specDrawingMap.erase( spec );
}

// delete drawing data (chromatogram)
void PeakLabelDrawer::deleteDrawData( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// check the map
	if( m_chromDrawingMap.find( chrom ) == m_chromDrawingMap.end() ) {
		return;
	}

	delete m_chromDrawingMap[ chrom ];
	m_chromDrawingMap.erase( chrom );
}

// delete rect list map (spectrum)
void PeakLabelDrawer::deleteRectListMap( kome::objects::Spectrum* spec ){
	// check the parameter
	if( spec == NULL ){
		return;
	}
	// check the map
	if( m_specRectListMap.find( spec ) == m_specRectListMap.end() ){
		return;
	}
	
	m_specRectListMap.erase( spec );
}

// delete  rect list map (chromatogram)
void PeakLabelDrawer::deleteRectListMap( kome::objects::Chromatogram* chrom ){
	// check the parameter
	if( chrom == NULL ){
		return;
	}
	// check the map
	if( m_chromRectListMap.find( chrom ) == m_chromRectListMap.end() ){
		return;
	}

	m_chromRectListMap.erase( chrom );
}

// @date 2012/12/11 <Add> OKADA ------->
// on close sample
void PeakLabelDrawer::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {

	// get spectra
	std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* >::iterator  itSpec = m_specDrawingMap.begin();
	int nSizeSpec = m_specDrawingMap.size();
	for( int i=0; i<nSizeSpec; i++ ) {
		itSpec = m_specDrawingMap.begin();
		kome::objects::Spectrum* spec = (*itSpec).first;
		if( spec->getSample() == sample ) {
			deleteDrawData( spec );
			deleteRectListMap( spec );
		}
	}
	
	// get chromatogram
	std::map< kome::objects::Chromatogram*, kome::img::DrawingXYData* >::iterator itChrom = m_chromDrawingMap.begin();
	int nSizeChrom = m_chromDrawingMap.size();
	for( int i=0; i<nSizeChrom; i++ ) {
		itChrom = m_chromDrawingMap.begin();
		kome::objects::Chromatogram* chrom = (*itChrom).first;
		if( chrom->getSample() == sample ) {
			deleteDrawData( chrom );
			deleteRectListMap( chrom );
		}
	}

}
// @date 2012/12/11 <Add> OKADA <-------

// on close spectrum
void PeakLabelDrawer::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {
	deleteDrawData( spec );
	deleteRectListMap( spec );
}

// on close chromatogram
void PeakLabelDrawer::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
	deleteDrawData( chrom );
	deleteRectListMap( chrom );
}

// get accuracy
int PeakLabelDrawer::getAccuracy( const char* key ) {
	// default
	int accuracy = 2;

	// get value
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		accuracy = ini->getInt( SECTION, key, 2 );
	}

	return accuracy;
}

// get label type
std::string PeakLabelDrawer::getLabelType( const char* key ){
	// get value 
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	
	// default
	std::string labelType;
	if( ini != NULL ){
		labelType = ini->getString( SECTION, key, "rt_mass" );
	}

	return labelType;
}

// compare daraw peak data to sort
bool PeakLabelDrawer::lessPeak( std::pair< double, double > p0, std::pair< double, double > p1 ) {
	return ( p0.second > p1.second );
}

// get instance
PeakLabelDrawer& PeakLabelDrawer::getInstance() {
	// create object (This is the only object.)
	static PeakLabelDrawer drawer;

	return drawer;
}

// @date 2011.03.29 <Add> M.Izumi ->
// down button (spectrum)
bool PeakLabelDrawer::downButton(
	kome::objects::Spectrum& spec,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	if( !hasPeaks(spec) ){
		return false;
	}
	m_changing.canvas = canvas;
	
	std::list< std::pair< kome::core::Rect< int >, double > > list;
	if( m_specRectListMap.find( &spec ) != m_specRectListMap.end() ){
		list = m_specRectListMap[ &spec ];
	}

	return setDownButtonInfo( evt, graphics, false, list );
}

// down button (chromatogram)
bool PeakLabelDrawer::downButton(
	kome::objects::Chromatogram& chrom,
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){
	if( !hasPeaks(chrom) ){
		return false;
	}
	m_changing.canvas = canvas;

	std::list< std::pair< kome::core::Rect< int >, double > > list;
	if( m_chromRectListMap.find( &chrom ) != m_chromRectListMap.end() ){
		list = m_chromRectListMap[ &chrom ];
	}

	return setDownButtonInfo( evt, graphics, true, list );
}

// set Down Button Info
bool PeakLabelDrawer::setDownButtonInfo( 
	kome::evt::MouseEvent& evt, 
	kome::img::Graphics& graphics, 
	bool bchromflg,
	std::list< std::pair< kome::core::Rect< int >, double > > list )
{
	bool refresh = false;
	if ( evt.x() >= 0 && evt.x() < graphics.getGraphPositionLeft()	) {
		if( evt.lbutton() ){
			int x = graphics.getGraphPositionLeft()-MARK_SIZE;
			int y = graphics.getGraphPositionBottom();
			
			if( x <= evt.x() && evt.x() >= (x - MARK_SIZE) &&
				(y - MARK_SIZE) <= evt.y() && (y + MARK_SIZE) >= evt.y() )
			{
				setSettings( graphics.getGraphPositionBottom(), refresh );
				
			}else if( x <= evt.x() && evt.x() >= (x-MARK_SIZE) &&
				(m_changing.endPos-MARK_SIZE) <= evt.y() && (m_changing.endPos+MARK_SIZE) >= evt.y() ){
				setSettings( evt.y(), refresh );
				
			}else{
				m_bflg = true;
			}
		}else if( evt.rbutton() ){
			m_bflg = true;
		}
	}else{
		m_bflg = true;
		// get label type
		std::string labelType = getLabelType( LABEL_TYPE_PARAM_NAME );
		if( labelType.compare( LABEL_TYPE_DIFFERENCE ) == 0 ){
					
			double x = CLAMP( evt.x(),  graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
			double y = CLAMP( evt.y(),  graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );	
			
			// ラベル描画
			for( std::list< std::pair< kome::core::Rect< int >, double > >::iterator it = list.begin(); it != list.end(); it++ ) {
				kome::core::Rect< int >& rect = (*it).first;

				if( rect.left < x && x < rect.right &&  rect.top< y && y < rect.bottom ){
					if( bchromflg ){
						m_selPeakLabelChrom = (*it).second;
					}else{
						m_selPeakLabelSpec = (*it).second;
					}
				
					return true;
				}
			}	
		}
	}

	return refresh;
}

// move mouse (spectrum)
bool PeakLabelDrawer::moveMouse(
	kome::objects::Spectrum& spec,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){
	if( !hasPeaks(spec)  ){
		return false;
	}
		
	return setMoveMouseInfo( evt, graphics, canvas );	
}

// move mouse (chromatogram)
bool PeakLabelDrawer::moveMouse(
	kome::objects::Chromatogram& chrom,		
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){
	if( !hasPeaks(chrom) ){
		return false;
	}
		
	return setMoveMouseInfo( evt, graphics, canvas );	
}

// set mose move info
bool PeakLabelDrawer::setMoveMouseInfo( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics, wxWindow* canvas){
	if ( evt.x() >= 0
		&& evt.x() < graphics.getGraphPositionLeft()
	) {
		m_changing.leftFlg = true;
		m_changing.canvas = canvas;
		
	}else{
		m_changing.leftFlg = false;
	}

	if( !m_bflg ){ 
		// get position
		int endPos = m_changing.endPos;
		if( m_changing.leftFlg ) {		// y range
			endPos = CLAMP( evt.y(), graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );
		}

		m_changing.endPos = endPos;		
		
		return true;
	}
	
	return false;	
}

// up button 
bool PeakLabelDrawer::upButton(
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	bool bchromflg
) {
	return setUpButtonInfo( evt, graphics, bchromflg );	
}

// set up button info
bool PeakLabelDrawer::setUpButtonInfo( 
	kome::evt::MouseEvent& evt, 
	kome::img::Graphics& graphics, 
	bool bchromflg 
){
	bool refresh = false;
	m_bflg = false;
	if( m_changing.leftFlg && m_changing.draggingFlg ) {		// y range
		m_changing.endPos = CLAMP( evt.y(), graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );
	
		double px = double();
		double py = double();
		graphics.transformPositionToData( 0.0, m_changing.endPos, &px, &py );
		
		if( bchromflg ){ // chromatogaram
			m_chromThreshold = py;
		}else{			 // spectrum
			m_specThreshold = py;
		}
			
		refresh = true;
	}	
	
	m_changing.draggingFlg = false;
	
	return refresh;
}

// double click
bool PeakLabelDrawer::dblClick(
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	bool bchromflg
){
	bool refresh = false;
	if ( evt.x() >= 0
		&& evt.x() < graphics.getGraphPositionLeft()
	) {
		if( evt.lbutton() ){
			int x = graphics.getGraphPositionLeft()-MARK_SIZE;
			int y = graphics.getGraphPositionBottom();
	
			if( x <= evt.x() && evt.x() >= (x - MARK_SIZE) &&
				(y - MARK_SIZE) <= evt.y() && (y + MARK_SIZE) >= evt.y() )
			{
				// 閾値設定
				setThreshold( graphics, true, refresh, bchromflg );
				
			}else if( x <= evt.x() && evt.x() >= (x-MARK_SIZE) &&
				(m_changing.endPos-MARK_SIZE) <= evt.y() && (m_changing.endPos+MARK_SIZE) >= evt.y() ){
				// 閾値設定
				setThreshold( graphics, false, refresh, bchromflg );
			}else{
				refresh = false;
			}
		}
	}
	return refresh;
}

// draw foreground (spectrum)
void PeakLabelDrawer::drawForeground(
	kome::objects::Spectrum& spec,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){
	if( !hasPeaks(spec) || m_changing.canvas != canvas ){
		return;
	}

	drawForeground( graphics, false );	
}

// draw peaks
void PeakLabelDrawer::drawPeaks( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
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
	// カラーの変更・線の太さをiniファイルから取得するように変更 2013.04.23 <Mod> M.Izumi
	if( !g.isUnusedDefaultColor() ){
		g.setColor( kome::img::GraphTool::getColorFromIni( SECTION_2D_PEAK, COLOR_PEAK_2D, COLOR_DEF_PEAK  ) );
	}
	if( !g.isUnusedDefaultThickness() ){
		g.setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION_2D_PEAK, WIDTH_PEAK_2D ) );
	}

	for( unsigned int i = 0; i < peaks->getNumberOfPeaks(); i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks->getPeak( i );

		// draw
		if( peak->getEndMz() >= g.getMinX()
				&& peak->getStartMz() <= g.getMaxX()
				&& peak->getEndRt() >= g.getMinY()
				&& peak->getStartRt() <= g.getMaxY() ) {    // check range
			// draw
			drawPosition( g, peak->getStartRt(), peak->getEndRt(), peak->getStartMz(), peak->getEndMz() );
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw clusters
void PeakLabelDrawer::drawClusters( kome::objects::DataMapInfo& dataMap, kome::img::Graphics& g ) {
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
	// カラーの変更・線の太さをiniファイルから取得するように変更 2013.04.23 <Mod> M.Izumi
	if( !g.isUnusedDefaultColor() ){
		g.setColor( kome::img::GraphTool::getColorFromIni( SECTION_2D_PEAK, COLOR_CLUSTER_2D, COLOR_DEF_CLUSTER  ) );
	}
	if( !g.isUnusedDefaultThickness() ){
		g.setThicknessLine( kome::img::GraphTool::getWidthFromIni( SECTION_2D_PEAK, WIDTH_CLUSTER_2D ) ) ;
	}

	for( unsigned int i = 0; i < peaks->getNumberOfClusters(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* cluster = peaks->getCluster( i );

		// draw
		if( cluster->getEndMz() >= g.getMinX()
				&& cluster->getStartMz() <= g.getMaxX()
				&& cluster->getEndRt() >= g.getMinY()
				&& cluster->getStartRt() <= g.getMaxY() ) {    // check range
			// draw position
			drawPosition( g, cluster->getStartRt(), cluster->getEndRt(), cluster->getStartMz(), cluster->getEndMz() );
		}
	}

	// restore color
	g.setColor( oldColor );
}

// draw position
void PeakLabelDrawer::drawPosition(
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

// draw forground (chromatogaram)
void PeakLabelDrawer::drawForeground(
	kome::objects::Chromatogram& chrom,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){	
	if( !hasPeaks(chrom) || m_changing.canvas != canvas ){
		return;
	}
	
	drawForeground( graphics, true );
}

// draw foreground
void PeakLabelDrawer::drawForeground(kome::img::Graphics& graphics, bool bchromflg ){
	// points array
	int points[] = { 0, 0, 0, 0 };

	// set
	COLOR oldColor = graphics.getColor();
	kome::img::Graphics::DrawStyle oldStyle = graphics.getDrawStyle();

	graphics.setColor( SET_COLOR );
	graphics.setDrawStyle( kome::img::Graphics::STYLE_SOLID );
	
	// draw start position
	if( m_changing.leftFlg ) {  // selecting y range
		points[0] = graphics.getGraphPositionLeft();
		points[1] = m_changing.endPos;
		points[2] = graphics.getGraphPositionRight();
		points[3] = m_changing.endPos;
	}
	int y = m_changing.endPos;
	int x = graphics.getGraphPositionLeft()-10;
	if( y == -1 ){
		y = graphics.getGraphPositionBottom();
	}
	
	bool bflg = false;
	if( m_changing.draggingFlg ){
		graphics.drawLines( points,		2 );
		graphics.drawMark(kome::img::Graphics::MARK_TRIANGLE, x, y, MARK_SIZE );
		bflg = true;
	}

	if(!bflg && m_changing.leftFlg && !m_bflg ){
		int py;
		double posx;
		double posy;
		if( bchromflg ){
			py = m_chromThreshold;
		}else{
			py = m_specThreshold;
		}
		graphics.transformDataToPosition(0.0, py, &posx, &posy );
		if( py == 0.0 ){
			py = graphics.getGraphPositionBottom();
		}else{
			py = posy;
		}
		graphics.drawMark(kome::img::Graphics::MARK_TRIANGLE, x, py, MARK_SIZE );
	}
				
	// recover
	graphics.setColor( oldColor );
	graphics.setDrawStyle( oldStyle );
}

// has peaks (spectrum)
bool PeakLabelDrawer::hasPeaks( kome::objects::Spectrum& spec ){
	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	return mgr.hasPeaks( &spec );
}

// has peaks (chromatogram)
bool PeakLabelDrawer::hasPeaks( kome::objects::Chromatogram& chrom ){
	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	return mgr.hasPeaks( &chrom );
}

// 閾値設定
void PeakLabelDrawer::setThreshold(
	kome::img::Graphics& graphics, 
	bool initialPos,
	bool& refresh,
	bool bchromflg )
{
	ThresholdSettingDialog dlg( kome::window::WindowTool::getMainWindow() );
	
	if( bchromflg ){ // chromatogram
		dlg.setThreshold(m_chromThreshold);
		// ダイアログ表示
		if( dlg.ShowModal() == wxID_OK ){
			m_chromThreshold = dlg.getThreshold();
			if( !initialPos ){
				double px = double();
				double py = double();
				graphics.transformDataToPosition( 0.0, m_chromThreshold, &px, &py );
				m_changing.endPos = py;
			}
		}
	}else{			 // spectrum
		dlg.setThreshold(m_specThreshold);
		// ダイアログ表示
		if( dlg.ShowModal() == wxID_OK ){
			m_specThreshold = dlg.getThreshold();
			if( !initialPos ){
				double px = double();
				double py = double();
				graphics.transformDataToPosition( 0.0, m_specThreshold, &px, &py );
				m_changing.endPos = py;
			}
		}
	}
	
	refresh = true;
}

// 各設定 セット
void PeakLabelDrawer::setSettings( int endPos, bool& refresh ){
	m_changing.endPos	 = endPos;
	refresh = true;
	m_changing.draggingFlg = true;
	m_bflg = false;
}

// pdate 2011.03.29 <Add> M.Izumi <-


